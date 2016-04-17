#pragma once

#include "MemPooler.h"
#include "IOCPDefinition.h"
#include "IOCPProcessThread.h"

class AFX_EXT_CLASS CMemoryPool : public IIOCPProcessThread
{
public:
	CMemoryPool(void);
	virtual ~CMemoryPool(void);

	virtual void ProcessingThread() = 0;

protected:
	SOCKET							m_ListenSocket;					// 리슨 소켓
	CMemPooler<PerSocketContext> *	m_pPerSocketCtxMemPool;
	CMemPooler<PerIoContext> *		m_pRecvMemPool;
	CMemPooler<PerIoContext> *		m_pSendMemPool;

	pPerSocketContext AllocPerSocketContext(SOCKET clientSocket);		// PerSocketContext 메모리 할당
	int GetSocketContextAllocCount();
	void DeallocPerSocketContext(pPerSocketContext pPerSocketCtx);		// PerSocketContext 메모리 제거

private:
	pPerIoContext AllocPerIoContextForSend(void);					// PerSendIoContext 메모리 할당
	pPerIoContext AllocPerIoContextForRecv(void);					// PerRecvIoContext 메모리 할당
	void DeallocPerIoContextForRecv(pPerIoContext pPerIoCtx);		// PerRecvIoContext 메모리 제거
	void DeallocPerIoContextForSend(pPerIoContext pPerIoCtx);		// PerSendIoContext 메모리 제거

};

