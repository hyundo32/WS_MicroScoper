#include "StdAfx.h"
#include "NetClient.h"
#include "NetPacket.h"

CNetClient::CNetClient()
{
	StartUp();

	m_pIN2P = NULL;
	m_nServerSocketType = 0;

	m_nPacketCount = 0;
	m_pPacket = NULL;
	m_nPacketIdx = 0;
	InitializeCriticalSection(&m_csPacket);
	InitializeCriticalSection(&m_csSending);
}

CNetClient::~CNetClient()
{	
	if (m_pPacket)
		delete [] m_pPacket;
	m_nPacketCount = 0;

	DeleteCriticalSection(&m_csPacket);
	DeleteCriticalSection(&m_csSending);

	CleanUp();
}

BOOL CNetClient::SetServerInfo(int ST, short sNetworkCode, short sVersion, int nPacketCount)
{
	m_nServerSocketType = ST;	// 연결할 Server의 종류.

	if (nPacketCount <= 0)
		nPacketCount = 100;

	if (m_pPacket)
		delete [] m_pPacket;
	m_pPacket = new CNetPacket[nPacketCount];
	m_nPacketCount = nPacketCount;

	for (int i = 0; i < m_nPacketCount; i++)
		m_pPacket[i].SetVersion(sNetworkCode, sVersion);

	m_sNetworkCode = sNetworkCode;
	m_sVersion = sVersion;

	return TRUE;
}


CNetPacket* CNetClient::GetPacket(int nMilSecond)
{
	CNetPacket* pPacket;
	EnterCriticalSection(&m_csPacket);
	
	int nCount = 0;
	do 
	{
		pPacket = &m_pPacket[m_nPacketIdx];
		m_nPacketIdx = ++m_nPacketIdx % m_nPacketCount;
		if (!pPacket->IsUsing())
		{
			pPacket->LockPacket();
			break;
		}
		Sleep(1);
		if (++nCount > nMilSecond)
		{
			LeaveCriticalSection(&m_csPacket);
			return NULL;
		}
	} while(TRUE);
	pPacket->ResetValues();

	LeaveCriticalSection(&m_csPacket);

	return pPacket;
}

BOOL CNetClient::DisconnectServer()
{
	if (m_SocketJoiner.GetSocketContext())
	{
		CloseSocket(m_SocketJoiner.GetSocketContext(), FALSE, FALSE);
		m_SocketJoiner.Reset();
		return TRUE;
	}

	return FALSE;
}

// Server Socket이 접속을 허용했을 때 호출하는 함수.
int CNetClient::AcceptedSocket(pPerSocketContext pSocketCtx, ULONG ulIP, int nPort)
{
	return TRUE;
}

// Client Socket이 접속되었을 때 호출하는 함수
BOOL CNetClient::ConnectedSocket(pPerSocketContext pSocketCtx)
{
	if (m_pIN2P)
	{
		m_SocketJoiner.SetSocketContext(pSocketCtx);
		m_pIN2P->IOCPNet2P_Connected(m_nServerSocketType);
	}

	return TRUE;
}

BOOL CNetClient::RecvCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes)
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

	int nPacketCount = 0;
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
			NoticeMsg("Packet Header :: Version Mismatch!! Set %d, Packet %d, Recv %d, Cur %d, Len %d, PktCount %d, Comp %d, Size %d"
				, m_sVersion, pPacket->GetVersion(), dwBytes, dwCurrentBytes, pBuf->len, nPacketCount, dwCompBytes, pPacket->GetSize());
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
				m_pIN2P->IOCPNet2P_Received(m_nServerSocketType, pPacket);
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
BOOL CNetClient::SendCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes)
{
	return FALSE;
}

// Socket이 끊어졌음을 알려주기 위한 함수
void CNetClient::SocketClosed(pPerSocketContext pSocketCtx)
{
	if (m_pIN2P)
	{
		m_pIN2P->IOCPNet2P_Disconnected(m_nServerSocketType);
		m_SocketJoiner.Reset();
	}
}

//////////////////////////////////////////////////////////////////////////
// Packet Send
BOOL CNetClient::SendToServer(int nType, SHORT sResult, int nModuleNo)
{
	return SendToServer(nType, sResult, nModuleNo, -1, NULL);
}

BOOL CNetClient::SendToServer(int nType, SHORT sResult, int nModuleNo, CNetPacket* pPacket)
{
	return SendToServer(nType, sResult, nModuleNo, -1, pPacket);
}

BOOL CNetClient::SendToServer(int nType, SHORT sResult, int nModuleNo, SHORT sPacketIdx, CNetPacket* pPacket)
{
	EnterCriticalSection(&m_csSending);
	BOOL bRet = FALSE;
	if (m_SocketJoiner.GetSocketContext())
	{
		WSABUF* pWsaBuf = NULL;
		while (TRUE)
		{
			pWsaBuf = m_SocketJoiner.GetSendBuffer();
			if (!pWsaBuf)
				Sleep(0);
			else
				break;
		}

		// Buffer를 채운다.
		pPerSocketContext pCtx = m_SocketJoiner.GetSocketContext();
		int nWriteSize = 0, i;

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
			for (i = 0; i < nCount; i++)
			{
				short sValue = pPacket->GetShort(i);
				memcpy(pWsaBuf->buf + nWriteSize, &sValue, sizeof(short));
				nWriteSize += sizeof(short);
			}

			nCount = pPacket->GetIntCount();
			memcpy(pWsaBuf->buf + nWriteSize, &nCount, sizeof(int));
			nWriteSize += sizeof(int);
			for (i = 0; i < nCount; i++)
			{
				int nValue = pPacket->GetInt(i);
				memcpy(pWsaBuf->buf + nWriteSize, &nValue, sizeof(int));
				nWriteSize += sizeof(int);
			}

			nCount = pPacket->GetDoubleCount();
			memcpy(pWsaBuf->buf + nWriteSize, &nCount, sizeof(int));
			nWriteSize += sizeof(int);
			for (i = 0; i < nCount; i++)
			{
				double dValue = pPacket->GetDouble(i);
				memcpy(pWsaBuf->buf + nWriteSize, &dValue, sizeof(double));
				nWriteSize += sizeof(double);
			}

			CString strString = _T("");
			int nLength;
			nCount = pPacket->GetStringCount();
			memcpy(pWsaBuf->buf + nWriteSize, &nCount, sizeof(int));
			nWriteSize += sizeof(int);
			for (i = 0; i < nCount; i++)
			{
				strString = pPacket->GetString(i);

				char * str_Data = NULL;
#ifdef _UNICODE
				int length = WideCharToMultiByte(CP_ACP, 0, strString.GetBuffer(), -1, NULL, 0, NULL,NULL); 
				str_Data = new char[length]; 
				WideCharToMultiByte(CP_ACP, 0, strString.GetBuffer(), -1, str_Data, length, NULL, NULL); 
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

				nLength = strString.GetLength();
				memcpy(pWsaBuf->buf + nWriteSize, &nLength, sizeof(int));
				nWriteSize += sizeof(int);

				if (nLength > 0)
				{
					memcpy(pWsaBuf->buf + nWriteSize, str_Data, nLength);
					nWriteSize += nLength;
				}

				delete [] str_Data;
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

		bRet = SendPost(m_SocketJoiner.GetSocketContext());
	}
	else
	{
		if (pPacket)
			pPacket->UnlockPacket();
	}
	LeaveCriticalSection(&m_csSending);
	return bRet;
}
