/********************************************************************
	created:	2005/03/10
	created:	10:3:2005   15:33
	filename: 	E:\Project\무인?�담?�스??HMIOCPNetwork\HMIOCPDefinition.h
	file path:	E:\Project\무인?�담?�스??HMIOCPNetwork
	file base:	HMIOCPDefinition
	file ext:	h
	author:		Chae Hi-myung
	
	purpose:	���ǵ�...
*********************************************************************/

#include <winsock2.h>

//typedef unsigned long	ULONG_PTR;

#define		MAX_BUFFER_SIZE				8192
#define		SIZE_IOCONTEXT				(sizeof(WSAOVERLAPPED) + sizeof(WSABUF) + (MAX_BUFFER_SIZE + 1) + sizeof(BOOL))
#define		SIZE_SOCKETCONTEXT			16

enum NetworkMode { ServerMode, ClientMode};

// OVERLAPPED ����ü�� Ȯ��
typedef struct _PERIOCONTEXT	// BulkBuffer�� ©�� reint...Cast �ϱ� ������ ������, �Ҹ��ڰ� ȣ����� �ʴ´�.
{
protected:
	WSAOVERLAPPED	overlapped;
	WSABUF			wsaBuf;
	char			Buffer[MAX_BUFFER_SIZE + 1];
	BOOL			bIsUse;

public:
	BOOL IsLocked()					{ return bIsUse; }
	void LockBuffer()				{ bIsUse = TRUE; }
	void UnlockBuffer()				{ bIsUse = FALSE; }
	
	int GetLen()					{ return wsaBuf.len; }
	WSAOVERLAPPED* GetOverlapped()	{ return &overlapped; }
	
	void ResetOverlapped()			{ ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED)); }
	void ResetBuffer()				{ wsaBuf.buf = Buffer; }

	int StringCompare(char* str)	{ return strcmp(wsaBuf.buf, str); }

	WSABUF* GetLockedBuffer()		{ return &wsaBuf; }
	char* GetBulkBuffer()			{ return Buffer; }

	WSABUF* GetBuffer()
	{
		int nCount = 0;
		while (bIsUse)
			return NULL; 

		LockBuffer();

		return &wsaBuf;
	}

	void ReleaseBuffer()
	{
		ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED));

		ZeroMemory(Buffer, MAX_BUFFER_SIZE + 1);
		wsaBuf.buf		= Buffer;
		wsaBuf.len		= MAX_BUFFER_SIZE;

		UnlockBuffer();
	}

	// ���� ������.
	_PERIOCONTEXT(const _PERIOCONTEXT& rhs)
	{
		if(this != &rhs)
		{
			ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED));

			ZeroMemory(Buffer, MAX_BUFFER_SIZE + 1);
			memcpy(Buffer, rhs.Buffer, MAX_BUFFER_SIZE);

			wsaBuf.buf	= Buffer;
			wsaBuf.len	= rhs.wsaBuf.len;
			bIsUse		= FALSE;
		}
	}

	// ���Կ����� �����ε�.
    _PERIOCONTEXT& operator=(const _PERIOCONTEXT& rhs)
	{
		if(this != &rhs)
		{
			ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED));

			ZeroMemory(Buffer, MAX_BUFFER_SIZE + 1);
			memcpy(Buffer, rhs.Buffer, MAX_BUFFER_SIZE);

			wsaBuf.buf	= Buffer;
			wsaBuf.len	= rhs.wsaBuf.len;
			bIsUse		= FALSE;
		}
		return *this;
	}
} PerIoContext, *pPerIoContext;

// IP Completion Key �� ���Ǵ� ����ü
typedef struct _PERSOCKETCONTEXT
{
	SOCKET socket;
	pPerIoContext recvContext;
	pPerIoContext sendContext;

	_PERSOCKETCONTEXT()
	{
		Reset();
	}

	void Reset()
	{
		recvContext->ReleaseBuffer();
		sendContext->ReleaseBuffer();
		socket = INVALID_SOCKET;
	}
} PerSocketContext, *pPerSocketContext;
