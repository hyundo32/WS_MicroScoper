#include "StdAfx.h"
#include "IOCPController.h"

BOOL CIOCPController::m_bIsStartUp = FALSE;

CIOCPController::CIOCPController()
{
	InitializeCriticalSection(&m_csContext);

	m_pPerSocketCtxMemPool = NULL;
	m_pRecvMemPool = NULL;
	m_pSendMemPool = NULL;

	ZeroMemory(m_strFileName, MAX_PATH);
	m_nLogYear = 0;
	m_nLogMonth = 0;
	m_nLogDay = 0;

	m_nPort = 0;
	m_strIP = _T("");

	m_pSocketCtx = NULL;
	m_bReconnect = FALSE;
	m_bConnected = FALSE;

	m_hMutexLog = CreateMutex(NULL, FALSE, _T("Mutex_IOCPLog"));
}

CIOCPController::~CIOCPController()
{
	DeinitNetwork();

	if (m_pPerSocketCtxMemPool)
		delete m_pPerSocketCtxMemPool;
	m_pPerSocketCtxMemPool = NULL;
	if (m_pRecvMemPool)
		delete m_pRecvMemPool;
	m_pRecvMemPool = NULL;
	if (m_pSendMemPool)
		delete m_pSendMemPool;
	m_pSendMemPool = NULL;

	CloseHandle(m_hMutexLog);
	DeleteCriticalSection(&m_csContext);
}

/********************************************************************************/
// �ʱ�ȭ �۾�
/********************************************************************************/
BOOL CIOCPController::StartUp()
{
	// ���� �ʱ�ȭ ó��
	if (!m_bIsStartUp)
	{
		WSADATA wsd;
		if(WSAStartup(MAKEWORD(2,2), &wsd) != 0)
		{
			return FALSE;
		}
		if (LOBYTE( wsd.wVersion ) != 2 || HIBYTE( wsd.wVersion ) != 2 )
		{
			WSACleanup( );
			return FALSE;
		}
		m_bIsStartUp = TRUE;
	}

	return TRUE;
}

BOOL CIOCPController::CleanUp()
{
	// ���� ��� ��!!!!!
	if (m_bIsStartUp)
		WSACleanup();

	m_bIsStartUp = FALSE;

	return TRUE;
}

BOOL CIOCPController::InitNetwork(NetworkMode Mode, int nSocketCount, int nPort, CString strIP)
{
	NoticeMsg("--- IOCP::Init() - Start!");
	if (!m_bIsStartUp)
		return FALSE;

	if (!strIP.IsEmpty())
		m_strIP = strIP;
	if (nPort > 0)
		m_nPort = nPort;

	if (m_nPort == 0)
		return FALSE;
	SOCKADDR_IN serverSockAddr;

	/********************************************************************************/
	// Memory Pool �����!!
	/********************************************************************************/
	if (m_pPerSocketCtxMemPool)
		delete m_pPerSocketCtxMemPool;
	m_pPerSocketCtxMemPool = NULL;
	if (m_pRecvMemPool)
		delete m_pRecvMemPool;
	m_pRecvMemPool = NULL;
	if (m_pSendMemPool)
		delete m_pSendMemPool;
	m_pSendMemPool = NULL;

	m_pPerSocketCtxMemPool	= new CMemPooler<PerSocketContext>(nSocketCount * 2);
	m_pRecvMemPool			= new CMemPooler<PerIoContext>(nSocketCount * 2);
	m_pSendMemPool			= new CMemPooler<PerIoContext>(nSocketCount * 2);
	if (!m_pPerSocketCtxMemPool || !m_pRecvMemPool || !m_pSendMemPool)
	{
		NoticeMsg("IOCP::Init() - Create Memory Pool Failed");
		return FALSE;
	}

	m_NetMode = Mode;
	if (Mode == ServerMode)
	{
		m_bConnected = TRUE;

		/********************************************************************************/
		// Listen Socket ���� - Overlapped IO ���
		/********************************************************************************/
		m_ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if(m_ListenSocket == INVALID_SOCKET)
		{
			NoticeMsg("IOCP::Init() - Listen Socket Creation Failed : %d", WSAGetLastError());
			return FALSE;
		}

		int nZero = 1;
		if (SOCKET_ERROR == setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nZero, sizeof(int)))
		{
			NoticeMsg("IOCP::Init() - setsockopt Fail : %d", WSAGetLastError());
			closesocket(m_ListenSocket);
			m_ListenSocket = NULL;
			return FALSE;
		}

		serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serverSockAddr.sin_family = AF_INET;
		serverSockAddr.sin_port = htons((short)m_nPort);

		// Bind
		if(bind(m_ListenSocket, (LPSOCKADDR)&serverSockAddr, sizeof(serverSockAddr)))
		{
			NoticeMsg("IOCP::Init() - Bind Failed : %d", WSAGetLastError());
			closesocket(m_ListenSocket);
			m_ListenSocket = NULL;
			return FALSE;
		}

		// Listen
		if(SOCKET_ERROR == listen(m_ListenSocket, SOMAXCONN))
		{
			NoticeMsg("IOCP::Init() - Change to Listen Mode Error : %d", WSAGetLastError());
			closesocket(m_ListenSocket);
			m_ListenSocket = NULL;
			return FALSE;
		}
	}
	else if (Mode == ClientMode)
	{
		m_ListenSocket = INVALID_SOCKET;
	}

	/********************************************************************************/
	// IOCP �ʱ�ȭ ó��
	// m_hIOCP=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,nMaxNumberOfConcurrentThreads)
	/********************************************************************************/
	
	int ErrCode;
	if (!Create(0, &ErrCode))
	{
		NoticeMsg("IOCP::Init() - Create IO Completion Port Error: %d", ErrCode);
		return FALSE;
	}

	/********************************************************************************/
	// Thread Pool �� ����!
	// BEGINTHREADEX(NULL,0,IocpWorkerThreadStartingPoint,piProcessThread,0,&dwThreadId)
	/********************************************************************************/
	if (!CreateThreadPool(this))
	{
		NoticeMsg("IOCP::Init() - Create Thread Pool Failed");
		return FALSE;
	}

	/********************************************************************************/
	// Accept Thread ����.
	/********************************************************************************/
	if (Mode == ServerMode)
	{
		DWORD dwThreadId;
		CloseHandle(CreateThread(NULL, 0, AcceptThread, this, 0, &dwThreadId));
	}

	return TRUE;
}

/********************************************************************************/
// Accept �۾� ó��!!
/********************************************************************************/
void CIOCPController::AcceptProcess()
{
	int ErrCode = 0;

	int sockaddr_size = sizeof(SOCKADDR_IN);    
	SOCKADDR_IN clientsockaddr;

	SOCKET clientsocket = INVALID_SOCKET;
	pPerSocketContext pPerSocketCtx = NULL;
	
	NoticeMsg("IOCP::AcceptProcess() - Accept Process Start");

	while(TRUE)
	{
		__try
		{
			/********************************************************************************/
			// Client�κ��� accept
			/********************************************************************************/
			clientsocket = accept(m_ListenSocket, (LPSOCKADDR)&clientsockaddr, &sockaddr_size);
			EnterCriticalSection(&m_csContext);
			if(clientsocket == INVALID_SOCKET)
			{
				// ���� ������ Ŭ���� �ϸ� �� ������ �����Ƿ�
				// �� �����ÿ� Accept ������ ����������.
				if(WSAGetLastError() == WSAEINTR || m_ListenSocket == INVALID_SOCKET)
				{
					NoticeMsg("IOCP::AcceptProcess() - Listen Socket Closed!");
					LeaveCriticalSection(&m_csContext);
					return ;
				}
				NoticeMsg("IOCP::AcceptProcess() - Accepting Client Error: %d ",WSAGetLastError());
				__leave;
			}

			/********************************************************************************/
			// ���� ���ؽ�Ʈ �Ҵ� -> Completion Key
			// Per Socket Context �޸� �Ҵ�
			/********************************************************************************/
			pPerSocketCtx = AllocPerSocketContext(clientsocket);
            if(pPerSocketCtx == NULL)
			{
				NoticeMsg("IOCP::AcceptProcess() - Socket Context Allocation Error %d, Socket %d, %d", m_nPort, clientsocket, GetSocketContextAllocCount());
				closesocket(clientsocket);
			}
			else
			{
				/********************************************************************************/
				// IOCP Ŀ�� ��ü�� ����
				/********************************************************************************/
				if (!Associate(clientsocket, reinterpret_cast<ULONG_PTR>(pPerSocketCtx), &ErrCode))
				{
					NoticeMsg("IOCP::AcceptProcess() - Associating Error: %d", ErrCode);
					CloseSocket(pPerSocketCtx);
				}
				else
				{
					/********************************************************************************/
					// �ʱ� Recv ��û
					/********************************************************************************/
					int nRet = AcceptedSocket(pPerSocketCtx, clientsockaddr.sin_addr.S_un.S_addr, m_nPort);
					if (nRet == 0)	// �̹� ����.
					{
						DeallocPerSocketContext(pPerSocketCtx);
						NoticeMsg("IOCP::AcceptProcess() - Already Accepted!");
					}
					else if (nRet == -1)
					{
						NoticeMsg("IOCP::AcceptProcess() - Accept Socket Error!");
						CloseSocket(pPerSocketCtx);
					}
					else
					{
						RecvPost(pPerSocketCtx);
					}
				}

			}
			LeaveCriticalSection(&m_csContext);
		}
		__finally
		{
			if (AbnormalTermination())
			{
				NoticeMsg("IOCP::AcceptProcess() - Called CloseSocket() in __finally!");
				if (!pPerSocketCtx)
					CloseSocket(pPerSocketCtx);
			}
		}
	}
}

DWORD __stdcall CIOCPController::AcceptThread(PVOID pvParam)
{
	CIOCPController* pCont = static_cast<CIOCPController*>(pvParam);
	pCont->AcceptProcess();

	return 0;
}

/********************************************************************************/
// Connect �۾� ó��!!
/********************************************************************************/
BOOL CIOCPController::Connect(BOOL bReconnect)
{
	m_bReconnect = bReconnect;
	if (!m_bReconnect)
		return ConnectProcess();
	else
	{
		DWORD dwThreadId;
		CloseHandle(CreateThread(NULL, 0, ConnectThread, this, 0, &dwThreadId));
	}

	return TRUE;
}

DWORD _tinet_addr(const TCHAR *cp)
{
#ifdef UNICODE
	char IP[16];
	int Ret = 0;
	Ret = WideCharToMultiByte(CP_ACP, 0, cp, (int)_tcslen(cp), IP, 15, NULL, NULL);
	IP[Ret] = 0;

	return inet_addr(IP);
#endif

#ifndef UNICODE
	return init_addr(cp);
#endif
}


BOOL CIOCPController::ConnectProcess()
{
	m_EventExitConnectThread = CreateEvent(NULL, TRUE, FALSE, NULL);

	int ErrCode = 0;

	SOCKET clientsocket;
	int sockaddr_size = sizeof(SOCKADDR_IN);    
	SOCKADDR_IN clientsockaddr;
	clientsockaddr.sin_family = AF_INET;
	clientsockaddr.sin_port = htons(m_nPort);

	// setup the host entry
	struct hostent *host_entry;
	char lServerName[256];
	WideCharToMultiByte(CP_ACP, 0, m_strIP, -1, lServerName, 255, NULL, NULL);
	if ((host_entry = gethostbyname(lServerName)) == NULL)
	{
		clientsockaddr.sin_addr.S_un.S_addr = _tinet_addr(m_strIP);
	}
	else
	{
		clientsockaddr.sin_addr.S_un.S_addr = *(unsigned long*) host_entry->h_addr;
	}

	NoticeMsg("IOCP::ConnectProcess() - Connecting Started");

	int retval;
	__try
	{
		do
		{
			/********************************************************************************/
			// Server�� Connect
			/********************************************************************************/
			retval = SOCKET_ERROR;
			if (!m_bConnected)
			{
				clientsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
				if(clientsocket == INVALID_SOCKET)
				{
					NoticeMsg("IOCP::ConnectProcess() - SOCKET Create Failed");
					return FALSE;
				}

				retval = connect(clientsocket, (SOCKADDR *)&clientsockaddr, sizeof(clientsockaddr));
				if (retval == SOCKET_ERROR)
					closesocket(clientsocket);
				
				int n = WSAGetLastError();
			}

			if (retval == SOCKET_ERROR)
			{
				if (m_bReconnect)
				{
					Sleep(100);
					continue;
				}
				else
					return FALSE;
			}

			EnterCriticalSection(&m_csContext);
			m_bConnected = TRUE;
			
			/********************************************************************************/
			// ���� ���ؽ�Ʈ �Ҵ� -> Completion Key
			// Per Socket Context �޸� �Ҵ�
			/********************************************************************************/
			m_pSocketCtx = AllocPerSocketContext(clientsocket);
			if(m_pSocketCtx == NULL)
			{
				NoticeMsg("IOCP::AcceptProcess() - Socket Context Allocation Error");
				closesocket(clientsocket);
			}
			else
			{
				/********************************************************************************/
				// IOCP Ŀ�� ��ü�� ����
				/********************************************************************************/
				if(!Associate(clientsocket, reinterpret_cast<ULONG_PTR>(m_pSocketCtx), &ErrCode))
				{
					NoticeMsg("IOCP::AcceptProcess() - Associating Error: %d", ErrCode);
					CloseSocket(m_pSocketCtx);
					m_pSocketCtx = NULL;
				}
				else
				{
					/********************************************************************************/
					// �ʱ� Recv ��û
					/********************************************************************************/
					if(!RecvPost(m_pSocketCtx))
					{
						NoticeMsg("IOCP::AcceptProcess() - Init RecvPost Error!");
						CloseSocket(m_pSocketCtx);
						m_pSocketCtx = NULL;
					}
					else
						ConnectedSocket(m_pSocketCtx);
				}
			}

			LeaveCriticalSection(&m_csContext);		
		} while (m_bReconnect);
	}
	__finally
	{	
		if(AbnormalTermination())
		{
			
			NoticeMsg("IOCP::AcceptProcess() - Called CloseSocket() in __finally!");
			if (!m_pSocketCtx)
			{
				CloseSocket(m_pSocketCtx);

				m_pSocketCtx = NULL;
			}
			//return FALSE;
		}
	}

	SetEvent(m_EventExitConnectThread);

	return TRUE;
}

DWORD __stdcall CIOCPController::ConnectThread(PVOID pvParam)
{
	CIOCPController* pCont = static_cast<CIOCPController*>(pvParam);
	pCont->ConnectProcess();

	return 0;
}

/********************************************************************************/
// Work Thread Function!!!!!
/********************************************************************************/
void CIOCPController::ProcessingThread(void)
{
	pPerSocketContext pPerSocketCtx = NULL;
	pPerIoContext pPerIoCtx = NULL;
	DWORD dwBytesTransferred = 0;
	int ErrCode = 0;

	while (TRUE)
	{
		// IO Completion Packet ���´�.
		BOOL bRet = GetCompletionStatus(reinterpret_cast<ULONG_PTR*>(&pPerSocketCtx), &dwBytesTransferred,
										reinterpret_cast<LPOVERLAPPED*>(&pPerIoCtx), &ErrCode);
		// ���� ó��.
		if(bRet)
		{
			if(((int)pPerSocketCtx) == THREAD_DIE)
			{
				NoticeMsg("IOCP::ProcessingThread() - THREAD_DIE!");
				break;
			}
			if(NULL == pPerIoCtx)
			{
				NoticeMsg("IOCP::ProcessingThread() - Getting Completion Packet Failed %d",ErrCode);
				continue;
			}
		}
		else
		{
			EnterCriticalSection(&m_csContext);
			if(NULL == pPerIoCtx)
				NoticeMsg("IOCP::ProcessingThread() - Getting Completion Packet Failed %d",ErrCode);
			else
			{
				// ����� ���� ������ 64�� ���ɼ��� ����.
				// �� ������ ��Ʈ��ũ �̸��� ����� �� �����ϴ�. �̴�.
				// �� �̷��� ���� ��������� ���� OK�̴�.
			    CloseSocket(pPerSocketCtx);
				NoticeMsg("@@@ IOCP::ProcessingThread() - CloseSocket() in pPerIoCtx != NULL\r\n");
			}
			LeaveCriticalSection(&m_csContext);
			continue;
		}

		try
		{
			// Ŭ���̾�Ʈ�� ���� ���� 
			if (dwBytesTransferred == 0)
			{
				char str[100];
				sprintf_s(str, "@@@ Client (%d) Connection Closed.", pPerSocketCtx->socket);
				throw str;
			}

			// IO ���ݿ� ���� �׿� ���� ó��
			if (pPerIoCtx == pPerSocketCtx->recvContext)
			{
				// RECV Operation 
			    if (!RecvCompleteEvent(pPerSocketCtx, dwBytesTransferred))	
				{
					EnterCriticalSection(&m_csContext);
					CloseSocket(pPerSocketCtx);
					LeaveCriticalSection(&m_csContext);
					throw "RecvCompleteEvent Error\n";
				}
			}
			else if (pPerIoCtx == pPerSocketCtx->sendContext)
			{
				// SEND Operation
                if(!SendCompleteEvent(pPerSocketCtx, dwBytesTransferred))	
				{
					EnterCriticalSection(&m_csContext);
					CloseSocket(pPerSocketCtx);
					LeaveCriticalSection(&m_csContext);
					throw "SendCompleteEvent Error\n";				
				}
			}
			else
			{
				if(!OtherCompleteEvent(pPerSocketCtx, dwBytesTransferred))	
				{
					EnterCriticalSection(&m_csContext);
					CloseSocket(pPerSocketCtx);
					LeaveCriticalSection(&m_csContext);
					throw "OtherCompleteEvent Error\n";
				}	
			}
		}
		catch (char* errText)
		{
			EnterCriticalSection(&m_csContext);
			CloseSocket(pPerSocketCtx);
			LeaveCriticalSection(&m_csContext);
			NoticeMsg("IOCP::ProcessingThread() - Exception \'%s\'", errText);
		}
	}
}

/********************************************************************************/
// Recv ��û, Send ó��.
// return ��: TRUE  -> ���� ���� ���������� ó����
//            FALSE -> �Ϸ� ��Ŷ ó�� ���� �� ���� �߻�
// ���ú� �̺�Ʈ ó�� �ڵ鷯 �Լ�
/********************************************************************************/
BOOL CIOCPController::RecvCompleteEvent(pPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred)
{
	BOOL bRet = TRUE;

	// ������ ���ۺ��� ó��.
	WSABUF* pBuf = NULL;
	while ( TRUE )
	{
		pPerSocketCtx->recvContext->ResetBuffer();
		pBuf = pPerSocketCtx->recvContext->GetBuffer();
		if (pBuf)
		{
			bRet = RecvCompleted(pPerSocketCtx, dwBytesTransferred);	// �� ó���ߴٰ� ����.
			break;
		}
		Sleep(0);
	}

	if (bRet)
		pPerSocketCtx->recvContext->ReleaseBuffer();		// Buffer �ʱ�ȭ
	else
		pPerSocketCtx->recvContext->UnlockBuffer();

	// �ٽ� Recv ��û�� �ϰ� WTQ�� ����.
	return RecvPost(pPerSocketCtx);
}

// Send �Ϸ� ��Ŷ ó�� �ڵ鷯 �Լ�
// return ��: TRUE  -> ���� ���� ���������� ó����
//            FALSE -> �Ϸ� ��Ŷ ó�� ���� �� ���� �߻�
BOOL CIOCPController::SendCompleteEvent(pPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred)
{
	if (dwBytesTransferred < 0 || dwBytesTransferred > MAX_BUFFER_SIZE)
	{
		pPerSocketCtx->sendContext->ReleaseBuffer();
//		NoticeMsg("Error : SendCompleteEvent. Trans Size Mismatch %d", dwBytesTransferred);
		return FALSE;
	}

	WSABUF* pBuf = pPerSocketCtx->sendContext->GetLockedBuffer();
	if (dwBytesTransferred < static_cast<DWORD>(pPerSocketCtx->sendContext->GetLen()))
	{
		// ��� �������� �ʾұ� ������ ������.
		if (!pBuf)
		{
			NoticeMsg("Buffer Error : SendCompleteEvent");
			return FALSE;
		}

//		NoticeMsg("SendPost Size Mismatch %d, Sent %d", pBuf->len, dwBytesTransferred);
		pBuf->buf += dwBytesTransferred;
		pBuf->len -= dwBytesTransferred;
		SendPost(pPerSocketCtx);
	}
	else
	{
		SendCompleted(pPerSocketCtx, dwBytesTransferred);
		pPerSocketCtx->sendContext->ReleaseBuffer();
	}

	return TRUE;
}

// �޽����� �޾Ƽ� ó��
BOOL CIOCPController::RecvPost(pPerSocketContext pPerSocketCtx)
{
	DWORD dwRecvBytes = 0;
	DWORD dwFlags = 0;

	try
	{
		// ����.
		WSABUF* pBuf = pPerSocketCtx->recvContext->GetLockedBuffer();
		WSAOVERLAPPED* pOverlapped = pPerSocketCtx->recvContext->GetOverlapped();
		if (!pBuf || !pOverlapped)
			return FALSE;

		int ret = WSARecv(pPerSocketCtx->socket, pBuf, 1, &dwRecvBytes, &dwFlags, pOverlapped, NULL);
		if(SOCKET_ERROR == ret)
		{
			int ErrCode = WSAGetLastError();
			if(ErrCode != WSA_IO_PENDING && ErrCode != WSAEWOULDBLOCK)
			{
				NoticeMsg("IOCP::RecvPost() - WSARecv() Failed: %d, %d", ErrCode, pPerSocketCtx->recvContext->GetLen());
				return FALSE;
			}
		}
	}
	catch(...)
	{
		NoticeMsg("IOCP::RecvPost() - Exception");
		return FALSE;
	}

	return TRUE;
}

// Send ��û
int CIOCPController::SendPost(pPerSocketContext pPerSocketCtx)
{
	if (!pPerSocketCtx)
		return 0;

	int iSend;
	DWORD dwSended;//, dwIOBytes, dwTotBytes;

	try
	{
		// Size �ʱ�ȭ.
		WSABUF* pBuf = pPerSocketCtx->sendContext->GetLockedBuffer();
		WSAOVERLAPPED* pOverlapped = pPerSocketCtx->sendContext->GetOverlapped();
		if (!pBuf || !pOverlapped)
			return 0;
		
		pPerSocketCtx->sendContext->ResetOverlapped();
		// ����.
		dwSended = 0;
		iSend = WSASend(pPerSocketCtx->socket, pBuf, 1, &(dwSended), 0, pOverlapped, NULL);
		if (SOCKET_ERROR == iSend)	// Error�� ���� Break;
		{
			int nError = WSAGetLastError();
			if (WSA_IO_PENDING == nError || WSAEWOULDBLOCK == nError)
			{
//				NoticeMsg("WSASend() Error : %d", nError);
			}
			else if (nError == WSAECONNRESET)
			{
				EnterCriticalSection(&m_csContext);
				NoticeMsg("WSASend() - WSAECONNRESET");
				CloseSocket(pPerSocketCtx);
				LeaveCriticalSection(&m_csContext);
			}
			else
			{
				NoticeMsg("WSASend() - Unknown Error : %d", nError);
				return 1;
			}
		}
		else
		{
			if (dwSended == 0)		// ���� Bytes ���� 0�� ��� Break;
				return 2;
		}
	}
	catch(...)
	{
		NoticeMsg("IOCP::SendPost() - Exception");
		return 0;
	}

	return 1;
}

int CIOCPController::SendPost(pPerSocketContext pPerSocketCtx, void* pBuffer, int nLen)
{
	WSABUF* pBuf = pPerSocketCtx->sendContext->GetLockedBuffer();
	if (!pBuf)
		return 0;

	if (nLen > MAX_BUFFER_SIZE)
		return 0;

	memcpy(pBuf->buf, pBuffer, nLen);
	pBuf->len = nLen;
	pPerSocketCtx->sendContext->UnlockBuffer();

	return SendPost(pPerSocketCtx);
}

// Recv, Send �Ϸ� ���� ���� ó�� �ڵ鷯 �Լ�
// return ��: TRUE  -> ���� ���� ���������� ó����
//            FALSE -> �Ϸ� ��Ŷ ó�� ���� �� ���� �߻�
BOOL CIOCPController::OtherCompleteEvent(pPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred)
{
	// ���� ����� ���� Recv , Send �̿��� �̻��� ������ ����Ŵ ���� ���������.
    CloseSocket(pPerSocketCtx);

	return FALSE; // ������ ����Ŵ
}

/********************************************************************************/
// Ŭ���̾�Ʈ ���� ���ؽ�Ʈ �����ϰ� ���� ����
/********************************************************************************/
void CIOCPController::CloseSocket(pPerSocketContext pPerSocketCtx, BOOL bGraceful, BOOL bNotify)
{
	if (!pPerSocketCtx)
		return;

	try
	{
		EnterCriticalSection(&m_csContext);
//		TRACE("CloseSocket ; %d, Socket %d, this %d\n", reinterpret_cast<int>(pPerSocketCtx), pPerSocketCtx->socket, reinterpret_cast<int>(this));
		if(pPerSocketCtx != NULL)
		{
			if (m_NetMode == ClientMode && !m_bConnected)
			{
				LeaveCriticalSection(&m_csContext);
				return;
			}

			if (pPerSocketCtx->socket != INVALID_SOCKET)
			{
				// Socket�� ���������� ����ϴ� Class�� �˷��ش�.
				if (bNotify)
					SocketClosed(pPerSocketCtx);

				if(!bGraceful)
				{
					LINGER LingerStruct;
					LingerStruct.l_onoff = 1;
					LingerStruct.l_linger = 0;	// closesocket�� ��ٷ� �����ϰ� �۽� ������ �����ʹ� ������ �� TCP ������ ���� �����Ѵ�.
					setsockopt(pPerSocketCtx->socket, SOL_SOCKET, SO_LINGER, (char*)&LingerStruct, sizeof(LingerStruct));
				}

				// ���� �ݰ� �޸� ��ȯ
				closesocket(pPerSocketCtx->socket);
				pPerSocketCtx->socket = INVALID_SOCKET;
				
				DeallocPerSocketContext(pPerSocketCtx);
			}

			pPerSocketCtx = NULL;

			if (m_NetMode == ClientMode)
				m_bConnected = FALSE;
		}
		LeaveCriticalSection(&m_csContext);
	}
	catch(...)
	{
		return;
	}
}

/********************************************************************************/
// ���� �ߴ�
/********************************************************************************/
void CIOCPController::DeinitNetwork()
{
	NoticeMsg("--- IOCP::Server Close()");

	if (m_NetMode == ServerMode)
	{
		m_bConnected = FALSE;
		closesocket(m_ListenSocket);
		m_ListenSocket = INVALID_SOCKET;
	}
	else
	{
		if (m_bReconnect && m_bConnected)
		{
			m_bReconnect = FALSE;
			CloseSocket(m_pSocketCtx);
			m_pSocketCtx = NULL;
			WaitForSingleObject(m_EventExitConnectThread, 500);
		}
		m_bConnected = FALSE;
	}

	CloseAllThreads();
}

//////////////////////////////////////////////////////////////
// �����޽���, �����
// ����� ���
void CIOCPController::NoticeMsg(char *str, ...)
{
	/*
	if (!m_bConnected)
		return;

	WaitForSingleObject(m_hMutexLog, INFINITE);
	
	try
	{
		// TRACE�� ����.
		va_list list;
		char szText[1024] = {0, };

		va_start(list, str);
		vsprintf_s(szText, 1024, str, list);
		strcat_s(szText, 1024, "\r\n");

//		OutputDebugString(szText);

		va_end(list);
 
		// ���Ͽ� ����
		static char		Message[1024];
		ZeroMemory(Message, 1024);
		SYSTEMTIME st;

		GetLocalTime(&st);

		int nYear = st.wYear;
		int nMonth = st.wMonth;
		int nDay = st.wDay;

		wsprintf(Message, "[%02d:%02d:%02d] %s", st.wHour, st.wMinute, st.wSecond, szText);

		// log file�� �α� �����.
		if (m_nLogYear != nYear || m_nLogMonth != nMonth || m_nLogDay != nDay)
		{
			wsprintf(m_strFileName, "./Log/HMIOCP_%2.0d%2.0d%2.0d.log", nYear, nMonth, nDay);
			CreateDirectory(_T("./Log"), NULL);
		}

		// ���Ͽ� �α� �����
		DWORD dwWritten;
		HANDLE hFile = NULL;

		hFile = CreateFile(m_strFileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hFile)
		{
			SetFilePointer(hFile, 0, NULL, FILE_END);
			WriteFile(hFile, Message, strlen(Message), &dwWritten, NULL);
			CloseHandle(hFile);
		}
	}
	catch (...)
	{
		TRACE("Exception in NoticeMsg");
	}

	ReleaseMutex(m_hMutexLog);
	*/
}

