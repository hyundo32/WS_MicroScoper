#include "StdAfx.h"
#include "MemoryPool.h"

CMemoryPool::CMemoryPool()
{
	m_ListenSocket = NULL;
}

CMemoryPool::~CMemoryPool()
{
	if(m_ListenSocket != NULL)
	{
		closesocket(m_ListenSocket);
		m_ListenSocket = NULL;
	}
}

/********************************************************************************/
// ���ӿ� ���� �޸𸮸� �Ҵ��ϰ� ����
/********************************************************************************/
pPerSocketContext CMemoryPool::AllocPerSocketContext(SOCKET clientSocket)
{
	pPerSocketContext pPerSocketCtx = NULL;

	if (m_pPerSocketCtxMemPool->GetAllocCount() == m_pPerSocketCtxMemPool->GetBlockCount())
		return NULL;

	pPerSocketCtx = m_pPerSocketCtxMemPool->Alloc();

	if (!pPerSocketCtx)
		return NULL;

	pPerSocketCtx->socket = clientSocket;
	pPerSocketCtx->recvContext = AllocPerIoContextForRecv();
	pPerSocketCtx->sendContext = AllocPerIoContextForSend();
	pPerSocketCtx->recvContext->ReleaseBuffer();
	pPerSocketCtx->sendContext->ReleaseBuffer();

	return pPerSocketCtx;
}

int CMemoryPool::GetSocketContextAllocCount()
{
	return m_pPerSocketCtxMemPool->GetAllocCount();
}

// Per Io Context �޸� �Ҵ�
pPerIoContext CMemoryPool::AllocPerIoContextForSend(void)
{
	pPerIoContext pPerSendIoCtx = NULL;

	pPerSendIoCtx = m_pSendMemPool->Alloc();

	return pPerSendIoCtx;
}

// Per Socket Context �޸� �Ҵ�
pPerIoContext CMemoryPool::AllocPerIoContextForRecv(void)
{
	pPerIoContext pPerRecvIoCtx = NULL;

	pPerRecvIoCtx = m_pRecvMemPool->Alloc();

	return pPerRecvIoCtx;
}

// PerSocketContext �޸� ����
void CMemoryPool::DeallocPerSocketContext(pPerSocketContext pPerSocketCtx)
{
	assert(pPerSocketCtx);

	// IO Context �Ҵ��� �� ����
	pPerSocketCtx->recvContext->ReleaseBuffer();
	pPerSocketCtx->sendContext->ReleaseBuffer();
	DeallocPerIoContextForRecv(pPerSocketCtx->recvContext);
	DeallocPerIoContextForSend(pPerSocketCtx->sendContext);

	// ���� ���ؽ�Ʈ ���� 
	BOOL bRet=m_pPerSocketCtxMemPool->Free(pPerSocketCtx);
	assert(bRet);
}

void CMemoryPool::DeallocPerIoContextForSend(pPerIoContext pPerSendIoCtx)
{
	assert(pPerSendIoCtx);
	BOOL bRet = m_pSendMemPool->Free(pPerSendIoCtx);
	assert(bRet);

}

// PerIoContext �޸� ����
void CMemoryPool::DeallocPerIoContextForRecv(pPerIoContext pPerRecvIoCtx)
{
	assert(pPerRecvIoCtx);
	BOOL bRet = m_pRecvMemPool->Free(pPerRecvIoCtx);
	assert(bRet);
}
