#pragma once

#include "IOCPHandler.h"

class AFX_EXT_CLASS CIOCPController : public CIOCPHandler
{
public:
	CIOCPController(void);
	virtual ~CIOCPController(void);

	// 초기화 처리
	BOOL	StartUp();
	BOOL	CleanUp();
	BOOL	InitNetwork(NetworkMode Mode, int nSocketCount, int nPort = 0, CString strIP = _T(""));	// Buffer를 몇 개를 생성해야하는지 지정 Default : 100

	BOOL	Connect(BOOL bReconnect);							// 재접속 여부를 받는다.
	void	DeinitNetwork();									// 중단

	CString	GetCurIP()			{ return m_strIP; }
	int		GetCurPort()		{ return m_nPort; }

	// Server Socket이 접속을 허용했을 때 호출하는 함수.
	virtual int AcceptedSocket(pPerSocketContext pSocketCtx, ULONG ulIP, int nPort) = 0;

	// Client Socket이 접속되었을 때 호출하는 함수
	virtual BOOL ConnectedSocket(pPerSocketContext pSocketCtx) = 0;

	// Socket에서 Receive가 완료되었을 때 CIOCPController에서 호출하는 함수 - Data Parseing
	virtual BOOL RecvCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes) = 0;

	// Socket을 통해 Send가 완료되었을 때 CIOCPController에서 호출하는 함수. - 후처리
	virtual BOOL SendCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes) = 0;

	// Socket이 끊어졌음을 알려주기 위한 함수
	virtual void SocketClosed(pPerSocketContext pSocketCtx) = 0;

	// 변수에 접근
protected:
	BOOL	RecvPost(pPerSocketContext pPerSocketCtx);			// RECV 요청
	int		SendPost(pPerSocketContext pPerSocketCtx);			// Send 요청
	int		SendPost(pPerSocketContext pPerSocketCtx, void* pBuffer, int nLen);					// Send 요청

	void	CloseSocket(pPerSocketContext pPerSocketCtx, BOOL bGraceful = FALSE, BOOL bNotify = TRUE);			// 클라이언트 소켓 컨텍스트 제거하고 소켓 닫음
	void	NoticeMsg(char *str, ...);

	CRITICAL_SECTION	m_csContext;
private:
	static DWORD __stdcall AcceptThread(PVOID pvParam);			// Accept 작업 처리
	static DWORD __stdcall ConnectThread(PVOID pvParam);		// Connect 작업 처리.
	void	AcceptProcess();
	BOOL	ConnectProcess();

	// 입출력이 완료되었을 떄 처리 함수.
	BOOL	RecvCompleteEvent(pPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred);	// 리시브 이벤트 처리 핸들러 함수
	BOOL	SendCompleteEvent(pPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred);	// Send 완료 패킷 처리 핸들러 함수
	BOOL	OtherCompleteEvent(pPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred);	// Send 완료 패킷 처리 핸들러 함수

	void	ProcessingThread(void);															// 완료 패킷 처리 함수

private:
	static BOOL			m_bIsStartUp;

	DWORD				m_dwLockThreadId;
	HANDLE				m_hMutexLog;
	CString				m_strIP;
	int					m_nPort;

	NetworkMode			m_NetMode;

	// Log
	char				m_strFileName[MAX_PATH];
	int					m_nLogYear;
	int					m_nLogMonth;
	int					m_nLogDay;

	// Client Mode
	pPerSocketContext	m_pSocketCtx;
	BOOL				m_bReconnect;
	BOOL				m_bConnected;
	HANDLE				m_EventExitConnectThread;
};

