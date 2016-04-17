#include "StdAfx.h"
#include "NetServer.h"
#include "NetPacket.h"

CNetServer::CNetServer()
{
	StartUp();

	m_pIN2P = NULL;
	m_pPacket = NULL;
	m_nTotalPacketCount = 0;
	m_nPacketIdx = 0;

	m_pClientJoiner = NULL;
	m_nClientCount = 0;

	InitializeCriticalSection(&m_csPacket);
}

CNetServer::~CNetServer()
{
	if (m_pPacket)
	{
		delete [] m_pPacket;
		m_pPacket = NULL;
	}
	if (m_pClientJoiner)
	{
		delete [] m_pClientJoiner;
		m_pClientJoiner = NULL;
		m_nClientCount = 0;
	}
	DeleteCriticalSection(&m_csPacket);

	CleanUp();
}

BOOL CNetServer::SetClientInfo(int ST, int nCount, short sNetworkCode, short sVersion, BOOL bUseHeartbeat, int nPacketCount)
{
	if (nCount <= 0)
		return FALSE;

	int i;

	m_Clientint = ST;	// 연결할 Client의 종류.
	if (nPacketCount <= 0)
		nPacketCount = 200;

	m_nClientCount = nCount;
	m_nTotalPacketCount = nCount * nPacketCount;
	m_pPacket = new CNetPacket[m_nTotalPacketCount];
	for (i = 0; i < m_nTotalPacketCount; i++)
		m_pPacket[i].SetVersion(sNetworkCode, sVersion);

	m_pClientJoiner = new CNetJoiner[nCount];
	for (i = 0; i < nCount; i++)
		m_pClientJoiner[i].SetUseHeartbeat(bUseHeartbeat);

	m_sNetworkCode = sNetworkCode;
	m_sVersion = sVersion;

	return TRUE;
}

BOOL CNetServer::InitNetwork(NetworkMode Mode, int nSocketCount, int nPort, CString strIP)
{
	m_nListenPort = nPort;
	return CIOCPController::InitNetwork(Mode, nSocketCount, nPort, strIP);
}

CNetPacket* CNetServer::GetPacket()
{
	if (!m_pPacket)
		return NULL;

	CNetPacket* pPacket = NULL;
	EnterCriticalSection(&m_csPacket);

	int nCount = 0;
	do 
	{
		pPacket = &m_pPacket[m_nPacketIdx];
		m_nPacketIdx = ++m_nPacketIdx % m_nTotalPacketCount;
		if (!pPacket->IsUsing())
		{
			pPacket->LockPacket();
			break;
		}
		Sleep(0);
		if (++nCount > 1000)
		{
			LeaveCriticalSection(&m_csPacket);
			return NULL;
		}
	} while(TRUE);

	pPacket->ResetValues();

	LeaveCriticalSection(&m_csPacket);

	return pPacket;
}

BOOL CNetServer::DisconnectClient(int nModuleNo)
{
	BOOL bRet = FALSE;
	EnterCriticalSection(&m_csContext);
	for (int i = 0; i < m_nClientCount; i++)
	{
		if (m_pClientJoiner[i].GetModuleNo() == nModuleNo)
		{
			CloseSocket(m_pClientJoiner[i].GetSocketContext(), FALSE, FALSE);
			m_pClientJoiner[i].Reset();
			bRet = TRUE;
			break;
		}
	}
	LeaveCriticalSection(&m_csContext);

	return bRet;
}

// Server Socket이 접속을 허용했을 때 호출하는 함수.
int CNetServer::AcceptedSocket(pPerSocketContext pCtx, ULONG ulIP, int nPort)
{
	if (nPort != m_nListenPort)
	{
		NoticeMsg("Accepte 주소가 잘못됨. %d - %d", m_nListenPort, nPort);
		return FALSE;
	}

	int nRet = -1;
	for (int i = 0; i < m_nClientCount; i++)
	{
		if (m_pClientJoiner[i].GetClientIP() == ulIP)
		{
			NoticeMsg("AcceptedSocket : Close Socket : %d, %u", i, ulIP);
			pCtx = m_pClientJoiner[i].GetSocketContext();
			nRet = 0;
			break;
		}
	}

	if (nRet == -1)
	{
		for (int i = 0; i < m_nClientCount; i++)
		{
			if (m_pClientJoiner[i].GetClientIP() == -1)
			{
				m_pClientJoiner[i].SetClientIP(ulIP);
				m_pClientJoiner[i].SetSocketContext(pCtx);

				nRet = 1;
				break;
			}
		}
	}

	// 접속 과다.
	if (nRet == -1)
		NoticeMsg("접속 과다로 종료, ListenPort %d, CurPort %d\n", m_nListenPort, nPort);

	return nRet;
}

// Socket이 끊어졌음을 알려주기 위한 함수
void CNetServer::SocketClosed(pPerSocketContext pSocketCtx)
{
	for (int i = 0; i < m_nClientCount; i++)
	{
		if (reinterpret_cast<int>(m_pClientJoiner[i].GetSocketContext()) == reinterpret_cast<int>(pSocketCtx))
		{
			m_pIN2P->IOCPNet2P_Disconnected(m_Clientint, m_pClientJoiner[i].GetModuleNo());
			m_pClientJoiner[i].Reset();

			break;
		}
	}
}

// Client Socket이 접속되었을 때 호출하는 함수
BOOL CNetServer::ConnectedSocket(pPerSocketContext pSocketCtx)
{
	return TRUE;
}

// Socket에서 Receive가 완료되었을 때 CIOCPController에서 호출하는 함수 - Data Parseing
BOOL CNetServer::RecvCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes)
{
	// 버퍼가 빌 때까지 처리.
	WSABUF* pBuf = pSocketCtx->recvContext->GetLockedBuffer();
	if (!pBuf)
	{
		NoticeMsg("Packet Lost :: Buf is not Available. Buf - %d", reinterpret_cast<int>(pBuf));
		return TRUE;
	}

	int nRead = 0;
	DWORD dwCompBytes = 0;
	DWORD dwCurrentBytes = dwBytes + (MAX_BUFFER_SIZE - pBuf->len);
	pBuf->buf = pSocketCtx->recvContext->GetBulkBuffer();

	int nPacketCount = 0, nOldPacket = 0;;
	while (TRUE)
	{
		if (dwCurrentBytes < PACKET_HEADERSIZE)		// 최소 패킷 크기.
		{
			// 패킷을 다 받지 못하면 더 받는다.
			if (dwCompBytes > 0 && dwCurrentBytes > 0)
			{
				//				NoticeMsg("처리한 부분 삭제 1 Recv %d, Cur %d, Comp %d", dwBytes, dwCurrentBytes, dwCompBytes);
				char* strTemp = new char[dwCurrentBytes + 1];
				ZeroMemory(strTemp, dwCurrentBytes + 1);
				memcpy(strTemp, pBuf->buf, dwCurrentBytes);
				pBuf->buf = pSocketCtx->recvContext->GetBulkBuffer();
				ZeroMemory(pBuf->buf, MAX_BUFFER_SIZE + 1);
				memcpy(pBuf->buf, strTemp, dwCurrentBytes);
				delete [] strTemp;
				dwCompBytes = 0;
			}
			//			NoticeMsg("헤더패킷보다 작음 Recv %d, Cur %d, Comp %d", dwBytes, dwCurrentBytes, dwCompBytes);
			pBuf->buf = pSocketCtx->recvContext->GetBulkBuffer() + dwCurrentBytes;
			pBuf->len = MAX_BUFFER_SIZE - dwCurrentBytes;
			return FALSE;
		}

		CNetPacket* pPacket = GetPacket();
		if (!pPacket)
		{
			NoticeMsg("RecvCompleted :: Packet is not Available. pPacket - %d", reinterpret_cast<int>(pPacket));
			break;
		}

		nRead = pPacket->Read_PacketHeader(pBuf);
		if (nRead == 0)
		{
			NoticeMsg("Packet Lost :: Read Packet Header. Read - %d", nRead);
			pPacket->UnlockPacket();
			return TRUE;
		}
		if (pPacket->GetVersion() != m_sVersion)
		{
			NoticeMsg("Packet Header :: Version Mismatch!! Set %d, Packet %d, Recv %d, Cur %d, Len %d, PktCount %d, Pkt %d, Comp %d, Size %d"
				, m_sVersion, pPacket->GetVersion(), dwBytes, dwCurrentBytes, pBuf->len, nPacketCount, nOldPacket, dwCompBytes, pPacket->GetSize());
			pPacket->UnlockPacket();
			return TRUE;
		}
		if (pPacket->GetSize() <= 0)	// 예외처리 - 받은 데이터는 지워버리고 다시 받는다.
		{
			NoticeMsg("Packet Lost :: Exception in Size - Size %d, Code %d, Module %d", pPacket->GetSize(), pPacket->GetPacketCode(), pPacket->GetModuleNo());
			pPacket->UnlockPacket();
			return TRUE;
		}

		if (dwCurrentBytes >= (DWORD)pPacket->GetSize())	// 길이 체크.
		{
			if (pPacket->GetSize() > PACKET_HEADERSIZE)
			{
				nRead = pPacket->Read_PacketBody(pBuf->buf + nRead);
				if (nRead == -1)
				{
					NoticeMsg("Packet Lost :: Read Packet Body. Read - %d, Cur %d, Size %d", nRead, dwCurrentBytes, pPacket->GetSize());
					pPacket->UnlockPacket();
					return TRUE;
				}
				else if (nRead == -2)
				{
					NoticeMsg("Packet Lost :: Read Packet Body. Exception - %d, Cur %d, Size %d", nRead, dwCurrentBytes, pPacket->GetSize());
					pPacket->UnlockPacket();
					return TRUE;
				}
			}

			// Context와 Module 연결을 위해 pSocketCtx를 넘긴다.
			if (m_pIN2P)
				m_pIN2P->IOCPNet2P_Received(m_Clientint, pPacket, reinterpret_cast<int>(pSocketCtx));
			nPacketCount++;
			nOldPacket = pPacket->GetPacketCode();
			if (dwCurrentBytes == (DWORD)pPacket->GetSize())
			{
				pPacket->UnlockPacket();
				return TRUE;		// 모든 버퍼를 다 처리했다.
			}

			// 버퍼가 남아있으면 다시 처리한다.
			dwCompBytes += (DWORD)pPacket->GetSize();
			dwCurrentBytes -= (DWORD)pPacket->GetSize();
			pBuf->buf = pBuf->buf + pPacket->GetSize();
			pPacket->UnlockPacket();
		}
		else
		{
			if (dwCompBytes > 0 && dwCurrentBytes > 0)
			{
				//				NoticeMsg("처리한 부분 삭제 2 Recv %d, Pak %d, Cur %d, Comp %d, PktSize %d", dwBytes, pPacket->GetSize(), dwCurrentBytes, dwCompBytes, pPacket->GetSize());
				char* strTemp = new char[dwCurrentBytes + 1];
				ZeroMemory(strTemp, dwCurrentBytes + 1);
				memcpy(strTemp, pBuf->buf, dwCurrentBytes);
				pBuf->buf = pSocketCtx->recvContext->GetBulkBuffer();
				ZeroMemory(pBuf->buf, MAX_BUFFER_SIZE + 1);
				memcpy(pBuf->buf, strTemp, dwCurrentBytes);
				delete [] strTemp;

				//				CNetPacket* pTmpPacket = GetPacket();
				//				if (pTmpPacket)
				//				{
				//					nRead = pTmpPacket->Read_PacketHeader(pBuf);
				//					NoticeMsg("삭제 후 헤더 Version %d, Code %d, Size %d", pTmpPacket->GetVersion(), pTmpPacket->GetPacketCode(), pTmpPacket->GetSize());
				//					pTmpPacket->UnlockPacket();
				//				}
			}
			//			NoticeMsg("모자라서 다시 받기 Recv %d, Pak %d, Cur %d, Comp %d", dwBytes, pPacket->GetSize(), dwCurrentBytes, dwCompBytes);
			pBuf->buf = pSocketCtx->recvContext->GetBulkBuffer() + dwCurrentBytes;
			pBuf->len = MAX_BUFFER_SIZE - dwCurrentBytes;
			pPacket->UnlockPacket();
			return FALSE;
		}
	}

	return TRUE;
}

// Socket을 통해 Send가 완료되었을 때 CIOCPController에서 호출하는 함수. - 후처리
BOOL CNetServer::SendCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes)
{
	//	TRACE("SendCompleted. Trans %d - Len %d\n", dwBytes, pSocketCtx->sendContext->GetLen());
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Packet Send

CNetJoiner* CNetServer::GetJoinerUser(int nModuleNo)
{
	// Client 찾기
	if (nModuleNo < 0 || nModuleNo >= m_nClientCount)
		return NULL;

	CNetJoiner* pClientJoiner = NULL;
	for (int i = 0; i < m_nClientCount; i++)
	{
		if (m_pClientJoiner[i].GetSocketContext() && m_pClientJoiner[i].GetModuleNo() == nModuleNo)
		{
			pClientJoiner = &m_pClientJoiner[i];
			break;
		}
	}

	return pClientJoiner;
}

//////////////////////////////////////////////////////////////////////////
// Packet Send.
BOOL CNetServer::InvokeToClient(int nType, SHORT sResult)
{
	BOOL bRet = TRUE;
	for (int i = 0; i < m_nClientCount; i++)
	{
		if (!SendToClient(i, nType, sResult))
			bRet = FALSE;
	}

	return bRet;
}



BOOL CNetServer::InvokeToClient(int nType, SHORT sResult, CNetPacket* pPacket)
{
	BOOL bRet = TRUE;
	for (int i = 0; i < m_nClientCount; i++)
	{
		if (!SendToClient(i, nType, sResult, pPacket))
			bRet = FALSE;
	}

	return bRet;
}

BOOL CNetServer::InvokeToClient(int nType, SHORT sResult, SHORT sPacketIdx, CNetPacket* pPacket)
{
	BOOL bRet = TRUE;
	for (int i = 0; i < m_nClientCount; i++)
	{
		if (!SendToClient(i, nType, sResult, sPacketIdx, pPacket))
			bRet = FALSE;
	}

	return bRet;
}

BOOL CNetServer::SendToClient(int nModuleNo, int nType, SHORT sResult)
{
	return SendToClient(nModuleNo, nType, sResult, -1, NULL);
}

BOOL CNetServer::SendToClient(int nModuleNo, int nType, SHORT sResult, CNetPacket* pPacket)
{
	return SendToClient(nModuleNo, nType, sResult, -1, pPacket);
}

BOOL CNetServer::SendToClient(int nModuleNo, int nType, SHORT sResult, SHORT sPacketIdx, CNetPacket* pPacket)
{
	CNetJoiner* pClientJoiner = GetJoinerUser(nModuleNo);

	int nRet = 0;
	if (pClientJoiner && pClientJoiner->GetSocketContext())
	{
		pPerSocketContext pCtx = pClientJoiner->GetSocketContext();
		WSABUF* pWsaBuf;
		int nCount = 0;
		while (TRUE)
		{
			pWsaBuf = pClientJoiner->GetSendBuffer();
			if (!pWsaBuf)
				Sleep(0);
			else
				break;
		}

		if(pWsaBuf->buf == NULL)
			return FALSE;

		// Buffer를 채운다.
		int i;
		int nWriteSize = 0;
		SHORT sPacketIdx = 0;

		// Header 채우기.
		//		memcpy(pWsaBuf->buf + nWriteSize, &nSize, sizeof(int));
		nWriteSize += sizeof(int);
		memcpy(pWsaBuf->buf + nWriteSize, &m_sNetworkCode, sizeof(SHORT));
		nWriteSize += sizeof(SHORT);
		memcpy(pWsaBuf->buf + nWriteSize, &m_sVersion, sizeof(SHORT));
		nWriteSize += sizeof(SHORT);
		memcpy(pWsaBuf->buf + nWriteSize, &nType, sizeof(int));
		nWriteSize += sizeof(int);
		memcpy(pWsaBuf->buf + nWriteSize, &sPacketIdx, sizeof(SHORT));
		nWriteSize += sizeof(SHORT);
		memcpy(pWsaBuf->buf + nWriteSize, &sResult, sizeof(SHORT));
		nWriteSize += sizeof(SHORT);
		memcpy(pWsaBuf->buf + nWriteSize, &nModuleNo, sizeof(int));
		nWriteSize += sizeof(int);

		// Body 채우기
		if (pPacket)
		{
			int nCount = pPacket->GetShortCount();
			memcpy(pWsaBuf->buf + nWriteSize, &nCount, sizeof(int));
			nWriteSize += sizeof(int);
			short sValue;
			for (i = 0; i < nCount; i++)
			{
				sValue = pPacket->GetShort(i);
				memcpy(pWsaBuf->buf + nWriteSize, &sValue, sizeof(short));
				nWriteSize += sizeof(short);
			}

			nCount = pPacket->GetIntCount();
			memcpy(pWsaBuf->buf + nWriteSize, &nCount, sizeof(int));
			nWriteSize += sizeof(int);
			int nValue;
			for (i = 0; i < nCount; i++)
			{
				nValue = pPacket->GetInt(i);
				memcpy(pWsaBuf->buf + nWriteSize, &nValue, sizeof(int));
				nWriteSize += sizeof(int);
			}

			nCount = pPacket->GetDoubleCount();
			memcpy(pWsaBuf->buf + nWriteSize, &nCount, sizeof(int));
			nWriteSize += sizeof(int);
			double dValue;
			for (i = 0; i < nCount; i++)
			{
				dValue = pPacket->GetDouble(i);
				memcpy(pWsaBuf->buf + nWriteSize, &dValue, sizeof(double));
				nWriteSize += sizeof(double);
			}

			CString String;
			int nLength;
			nCount = pPacket->GetStringCount();
			memcpy(pWsaBuf->buf + nWriteSize, &nCount, sizeof(int));
			nWriteSize += sizeof(int);
			for (i = 0; i < nCount; i++)
			{
				String = pPacket->GetString(i);

				char * str_Data = NULL;
#ifdef _UNICODE
				int length = WideCharToMultiByte(CP_ACP, 0, String.GetBuffer(), -1, NULL, 0, NULL,NULL); 
				str_Data = new char[length]; 
				WideCharToMultiByte(CP_ACP, 0, String.GetBuffer(), -1, str_Data, length, NULL, NULL); 
#else
				int length = strString.GetLength();
				str_Data = new char[length+1]; 
				memcpy(str_Data, strString.GetBuffer(), sizeof(char)*length);
				str_Data[length] = NULL;
#endif
				if (str_Data==NULL) 
				{
					continue;
				}

				nLength = String.GetLength();
				memcpy(pWsaBuf->buf + nWriteSize, &nLength, sizeof(int));
				nWriteSize += sizeof(int);

				if (nLength > 0)
				{
					memcpy(pWsaBuf->buf + nWriteSize, str_Data, nLength);
					nWriteSize += nLength;
				}

				delete [] str_Data;

				/*
				nLength = String.GetLength();
				if (nLength > 0)
				{
					memcpy(pWsaBuf->buf + nWriteSize, &nLength, sizeof(int));
					nWriteSize += sizeof(int);
					memcpy(pWsaBuf->buf + nWriteSize, (LPSTR)(LPCTSTR)String, nLength);
					nWriteSize += nLength;
				}
				*/
			}

			nLength = pPacket->GetBufferSize();
			memcpy(pWsaBuf->buf + nWriteSize, &nLength, sizeof(int));
			nWriteSize += sizeof(int);
			if (nLength > 0)
			{
				memcpy(pWsaBuf->buf + nWriteSize, pPacket->GetBuffer(), nLength);
				nWriteSize += nLength;
			}
			pPacket->UnlockPacket();
		}
		memcpy(pWsaBuf->buf, &nWriteSize, sizeof(int));
		pWsaBuf->len = nWriteSize;

		nRet = SendPost(pClientJoiner->GetSocketContext());
		if (nRet == 2)
			NoticeMsg("0 Bytes Sent : %d", nModuleNo);
	}
	else
	{
		if (pPacket)
			pPacket->UnlockPacket();
	}

	return nRet;
}
