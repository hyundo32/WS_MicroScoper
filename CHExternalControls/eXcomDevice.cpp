#include "StdAfx.h"
#include "eXcomDevice.h"

CeXcomDevice::CeXcomDevice(int nNo /*= 0*/)
{
	if (!AfxSocketInit())
	{
		//g_pLog->DisplayMessage(_T("[eXcom Umac]Windows 소켓 초기화에 실패하였습니다."));
	}

	m_socket = NULL;
	m_bConnected = FALSE;
	m_hMutex = NULL;

	CString strMutex;
	strMutex.Format(_T("Mutex_eXcom%d"), nNo);

	m_hMutex = CreateMutex(NULL, FALSE, strMutex);
}

CeXcomDevice::~CeXcomDevice(void)
{
	Close();
}

BOOL CeXcomDevice::Connect(DWORD IPAddress, int nPort)
{
	struct sockaddr_in sin; 
	int rc = 0;
	CString strTemp;

	if ( m_bConnected )
		return FALSE;

	Lock();

	if ( m_socket==NULL )
	{
		m_socket = new SOCKET;
	}

	*m_socket = socket(PF_INET,SOCK_STREAM,0); 	

	if( *m_socket == INVALID_SOCKET)
	{
		WSACleanup();
		Release();
		//g_pLog->DisplayMessage(_T("[eXcom Umac]Cannot create eXcom sokcet!"));
		return FALSE;
	}

	// Embedded Ethernet's IP address
	sin.sin_addr.s_impno  = (char)(0x000000FF  & IPAddress);
	sin.sin_addr.s_lh     = (char)((0x0000FF00 & IPAddress) >>  8);
	sin.sin_addr.s_host   = (char)((0x00FF0000 & IPAddress) >> 16);
	sin.sin_addr.s_net    = (char)((0xFF000000 & IPAddress) >> 24);
	sin.sin_family = PF_INET; 

	sin.sin_port = htons(nPort); 

	rc = connect(*m_socket,(struct sockaddr*)&sin,sizeof(sin)); 

	if(rc == SOCKET_ERROR) 
	{
		Release();
		//g_pLog->DisplayMessage(_T("[eXcom Umac]Cannot open eXcom communication port!"));
		return FALSE;
	} 

	m_bConnected = TRUE;
	Release();

	return TRUE;
}

void CeXcomDevice::Close()
{
	if ( m_socket!=NULL )
	{
		Lock();
		closesocket(*(SOCKET *)m_socket);
		delete m_socket;
		m_socket = NULL;
		m_bConnected = FALSE;
		Release();
	}
}

BOOL CeXcomDevice::ReadOneData(long nAddress, long &nValue)
{
	DWORD dwStartAddr = nAddress;
	return GetMemory(dwStartAddr * 4, sizeof(long), &nValue);
}

BOOL CeXcomDevice::WriteOneData(long nAddress, long nValue)
{
	DWORD dwStartAddr = nAddress;
	return SetMemory(dwStartAddr * 4, sizeof(long), &nValue);
}

BOOL CeXcomDevice::ReadMultyData(long nAddress, long *pValue, int nSize)
{
	if(pValue == NULL)
		return FALSE;
	DWORD dwStartAddr = nAddress;
	return GetMemory(dwStartAddr * 4, sizeof(long) * nSize, pValue);
}
BOOL CeXcomDevice::WriteMultyData(long nAddress, long *pValue, int nSize)
{
	if(pValue == NULL)
		return FALSE;
	DWORD dwStartAddr = nAddress;
	return SetMemory(dwStartAddr * 4, sizeof(long) * nSize, pValue);
}

BOOL CeXcomDevice::ReadOneData(long nAddress, float &fValue)
{
	DWORD dwStartAddr = nAddress;
	return GetMemory(dwStartAddr * 4, sizeof(float), &fValue);
}
BOOL CeXcomDevice::WriteOneData(long nAddress, float fValue)
{
	DWORD dwStartAddr = nAddress;
	return SetMemory(dwStartAddr * 4, sizeof(float), &fValue);
}

BOOL CeXcomDevice::ReadMultyData(long nAddress, float *pValue, int nSize)
{
	if(pValue == NULL)
		return FALSE;
	DWORD dwStartAddr = nAddress;
	return GetMemory(dwStartAddr * 4, sizeof(float) * nSize, pValue);
}
BOOL CeXcomDevice::WriteMultyData(long nAddress, float *pValue, int nSize)
{
	if(pValue == NULL)
		return FALSE;
	DWORD dwStartAddr = nAddress;
	return SetMemory(dwStartAddr * 4, sizeof(float) * nSize, pValue);
}	

/////////////////////////////////////////////////////////////////////////
// DPRAM Memory에 데이터를 설정하는 함수..
BOOL CeXcomDevice::SetMemory(DWORD dwStartAddr, DWORD dwLength, PVOID val)
{
	if ( !m_bConnected )
		return FALSE;

	int rc = 0;
	int nLen = 0, nBufLen = 0;

	int iTimeout = 100;
	long nBufRemain = dwLength;
	long lbufIndex = 0;
	BYTE *byData = (BYTE *)val;

	EXCOM_ETH cmdSend, cmdRecv;

	Lock();

	while(nBufRemain>0)
	{
		if ( nBufRemain>MAXDATA_LEN )
			nBufLen = MAXDATA_LEN;
		else
			nBufLen = nBufRemain;

		nBufRemain -= nBufLen;

		if ( dwStartAddr+nBufLen > ARRAY_MAX )
		{
			nBufRemain = 0;
			nBufLen = ARRAY_MAX - dwStartAddr;
			ASSERT(nBufLen>0);
		}

		cmdSend.RequestType = 0x11;	// DPRAM Write Command
		cmdSend.wIndex = (WORD)(dwStartAddr/4);
		cmdSend.wLength = (WORD)(nBufLen/4);	// 4Byte 단위로만 전송...

		memcpy(cmdSend.bData, byData+lbufIndex, nBufLen);
		rc = send(*(SOCKET *)m_socket, (char *)&cmdSend, nBufLen+8, 0);
		if ( rc==SOCKET_ERROR )
		{
			Release();
			return FALSE;
		}

		rc = setsockopt(*(SOCKET *)m_socket,
			SOL_SOCKET,
			SO_RCVTIMEO,
			(const char *)&iTimeout,
			sizeof(int));

		rc = recv(*(SOCKET *)m_socket, (char *)&cmdRecv, sizeof(EXCOM_ETH), 0);
		if ( rc==SOCKET_ERROR )
		{
			Release();
			return FALSE;
		}
		if (cmdRecv.RequestType==0x15)	// NAK received
		{
			int nErrorNo = cmdRecv.Request;
			Release();
			return FALSE;
		}

		dwStartAddr += nBufLen;
		lbufIndex += nBufLen;
	}

	Release();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// DPRAM Memory에서 데이터를 읽어오는 함수..
BOOL CeXcomDevice::GetMemory(DWORD dwStartAddr, DWORD dwLength, PVOID val)
{
	if ( !m_bConnected )
		return FALSE;

	int rc = 0;
	int nLen = 0, nBufLen = 0;

	int iTimeout = 100;
	long nBufRemain = dwLength;
	long lbufIndex = 0;
	BYTE *byData = (BYTE *)val;

	EXCOM_ETH cmdSend, cmdRecv;

	Lock();

	while(nBufRemain>0)
	{
		if ( nBufRemain>MAXDATA_LEN )
			nBufLen = MAXDATA_LEN;
		else
			nBufLen = nBufRemain;

		nBufRemain -= nBufLen;

		if ( dwStartAddr+nBufLen > ARRAY_MAX )
		{
			nBufRemain = 0;
			nBufLen = ARRAY_MAX - dwStartAddr;
			ASSERT(nBufLen>0);
		}

		cmdSend.RequestType = 0x10;	// DPRAM Read Command
		cmdSend.wIndex = (WORD)(dwStartAddr/4);
		cmdSend.wLength = (WORD)(nBufLen/4);	// 4Byte 단위로만 전송...

		//		memcpy(cmdSend.bData, byData+lbufIndex, nBufLen);
		rc = send(*(SOCKET *)m_socket, (char *)&cmdSend, nBufLen+8, 0);
		if ( rc==SOCKET_ERROR )
		{
			Release();
			return FALSE;
		}

		rc = setsockopt(*(SOCKET *)m_socket,
			SOL_SOCKET,
			SO_RCVTIMEO,
			(const char *)&iTimeout,
			sizeof(int));

		rc = recv(*(SOCKET *)m_socket, (char *)&cmdRecv, sizeof(EXCOM_ETH), 0);
		if ( rc==SOCKET_ERROR )
		{
			Release();
			return FALSE;
		}
		if (cmdRecv.RequestType==0x15)	// NAK received
		{
			int nErrorNo = cmdRecv.Request;
			Release();
			return FALSE;
		}

		memcpy(byData+lbufIndex, cmdRecv.bData, nBufLen);

		dwStartAddr += nBufLen;
		lbufIndex += nBufLen;
	}

	Release();

	return TRUE;
}

//#define EQUARRAY_MAX	120000
//#define MAXDWORD_PACKET	350
//
//BOOL CeXcomDevice::SetEQUMem(DWORD dwStartAddr, DWORD dwLength, PVOID val)
//{
//	if ( !m_bConnected )
//		return FALSE;
//
//	int rc = 0;
//	int nLen = 0, nBufLen = 0;
//	char sendbuf[256], recvbuf[256];
//
//	int iTimeout = 100;
//	long nBufRemain = dwLength;
//	long lbufIndex = 0;
//
//	memset(recvbuf, '\0', sizeof(BYTE)*256);
//
//	Lock();
//
//	while(nBufRemain>0)
//	{
//		if ( nBufRemain>MAXDWORD_PACKET )
//			nBufLen = MAXDWORD_PACKET;
//		else
//			nBufLen = nBufRemain;
//
//		nBufRemain -= nBufLen;
//
//		if ( dwStartAddr+nBufLen > EQUARRAY_MAX )
//		{
//			nBufRemain = 0;
//			nBufLen = EQUARRAY_MAX - dwStartAddr;
//		}
//
//		sprintf(sendbuf, "DSET%ld,%ld", dwStartAddr, nBufLen);
//		nLen = strlen(sendbuf);
//		rc = send(*(SOCKET *)m_socket, sendbuf, nLen, 0);
//		if ( rc==SOCKET_ERROR )
//		{
//			Release();
//			return FALSE;
//		}
//
//		rc = setsockopt(*(SOCKET *)m_socket,
//						SOL_SOCKET,
//						SO_RCVTIMEO,
//						(const char *)&iTimeout,
//						sizeof(int));
//		
//		rc = recv(*(SOCKET *)m_socket, recvbuf, 255, 0);
//		if ( rc==SOCKET_ERROR || recvbuf[0]!='\r' )
//		{
//			Release();
//			return FALSE;
//		}
//
//		rc = send(*(SOCKET *)m_socket, (const char FAR *)((char *)val+lbufIndex*sizeof(long)), nBufLen*sizeof(long), 0);
//		if ( rc==SOCKET_ERROR )
//		{
//			Release();
//			return FALSE;
//		}
//		rc = recv(*(SOCKET *)m_socket, recvbuf, 255, 0);
//		if ( rc==SOCKET_ERROR || recvbuf[0]!='\r' )
//		{
//			Release();
//			return FALSE;
//		}
//
//		dwStartAddr += nBufLen;
//		lbufIndex += nBufLen;
//	}
//
//	Release();
//
//	return TRUE;
//}

void CeXcomDevice::Lock()
{
	if ( m_hMutex )
		WaitForSingleObject(m_hMutex, INFINITE);
}

void CeXcomDevice::Release()
{
	if ( m_hMutex )
		ReleaseMutex(m_hMutex);
}