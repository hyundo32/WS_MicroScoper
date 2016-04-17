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
	SOCKET							m_ListenSocket;					// ���� ����
	CMemPooler<PerSocketContext> *	m_pPerSocketCtxMemPool;
	CMemPooler<PerIoContext> *		m_pRecvMemPool;
	CMemPooler<PerIoContext> *		m_pSendMemPool;

	pPerSocketContext AllocPerSocketContext(SOCKET clientSocket);		// PerSocketContext �޸� �Ҵ�
	int GetSocketContextAllocCount();
	void DeallocPerSocketContext(pPerSocketContext pPerSocketCtx);		// PerSocketContext �޸� ����

private:
	pPerIoContext AllocPerIoContextForSend(void);					// PerSendIoContext �޸� �Ҵ�
	pPerIoContext AllocPerIoContextForRecv(void);					// PerRecvIoContext �޸� �Ҵ�
	void DeallocPerIoContextForRecv(pPerIoContext pPerIoCtx);		// PerRecvIoContext �޸� ����
	void DeallocPerIoContextForSend(pPerIoContext pPerIoCtx);		// PerSendIoContext �޸� ����

};

