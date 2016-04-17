#pragma once

#include "IOCPHandler.h"

class AFX_EXT_CLASS CIOCPController : public CIOCPHandler
{
public:
	CIOCPController(void);
	virtual ~CIOCPController(void);

	// �ʱ�ȭ ó��
	BOOL	StartUp();
	BOOL	CleanUp();
	BOOL	InitNetwork(NetworkMode Mode, int nSocketCount, int nPort = 0, CString strIP = _T(""));	// Buffer�� �� ���� �����ؾ��ϴ��� ���� Default : 100

	BOOL	Connect(BOOL bReconnect);							// ������ ���θ� �޴´�.
	void	DeinitNetwork();									// �ߴ�

	CString	GetCurIP()			{ return m_strIP; }
	int		GetCurPort()		{ return m_nPort; }

	// Server Socket�� ������ ������� �� ȣ���ϴ� �Լ�.
	virtual int AcceptedSocket(pPerSocketContext pSocketCtx, ULONG ulIP, int nPort) = 0;

	// Client Socket�� ���ӵǾ��� �� ȣ���ϴ� �Լ�
	virtual BOOL ConnectedSocket(pPerSocketContext pSocketCtx) = 0;

	// Socket���� Receive�� �Ϸ�Ǿ��� �� CIOCPController���� ȣ���ϴ� �Լ� - Data Parseing
	virtual BOOL RecvCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes) = 0;

	// Socket�� ���� Send�� �Ϸ�Ǿ��� �� CIOCPController���� ȣ���ϴ� �Լ�. - ��ó��
	virtual BOOL SendCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes) = 0;

	// Socket�� ���������� �˷��ֱ� ���� �Լ�
	virtual void SocketClosed(pPerSocketContext pSocketCtx) = 0;

	// ������ ����
protected:
	BOOL	RecvPost(pPerSocketContext pPerSocketCtx);			// RECV ��û
	int		SendPost(pPerSocketContext pPerSocketCtx);			// Send ��û
	int		SendPost(pPerSocketContext pPerSocketCtx, void* pBuffer, int nLen);					// Send ��û

	void	CloseSocket(pPerSocketContext pPerSocketCtx, BOOL bGraceful = FALSE, BOOL bNotify = TRUE);			// Ŭ���̾�Ʈ ���� ���ؽ�Ʈ �����ϰ� ���� ����
	void	NoticeMsg(char *str, ...);

	CRITICAL_SECTION	m_csContext;
private:
	static DWORD __stdcall AcceptThread(PVOID pvParam);			// Accept �۾� ó��
	static DWORD __stdcall ConnectThread(PVOID pvParam);		// Connect �۾� ó��.
	void	AcceptProcess();
	BOOL	ConnectProcess();

	// ������� �Ϸ�Ǿ��� �� ó�� �Լ�.
	BOOL	RecvCompleteEvent(pPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred);	// ���ú� �̺�Ʈ ó�� �ڵ鷯 �Լ�
	BOOL	SendCompleteEvent(pPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred);	// Send �Ϸ� ��Ŷ ó�� �ڵ鷯 �Լ�
	BOOL	OtherCompleteEvent(pPerSocketContext pPerSocketCtx, DWORD dwBytesTransferred);	// Send �Ϸ� ��Ŷ ó�� �ڵ鷯 �Լ�

	void	ProcessingThread(void);															// �Ϸ� ��Ŷ ó�� �Լ�

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

