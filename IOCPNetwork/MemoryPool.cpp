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
// 접속에 대한 메모리를 할당하고 연결
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

// Per Io Context 메모리 할당
pPerIoContext CMemoryPool::AllocPerIoContextForSend(void)
{
	pPerIoContext pPerSendIoCtx = NULL;

	pPerSendIoCtx = m_pSendMemPool->Alloc();

	return pPerSendIoCtx;
}

// Per Socket Context 메모리 할당
pPerIoContext CMemoryPool::AllocPerIoContextForRecv(void)
{
	pPerIoContext pPerRecvIoCtx = NULL;

	pPerRecvIoCtx = m_pRecvMemPool->Alloc();

	return pPerRecvIoCtx;
}

// PerSocketContext 메모리 제거
void CMemoryPool::DeallocPerSocketContext(pPerSocketContext pPerSocketCtx)
{
	assert(pPerSocketCtx);

	// IO Context 할당한 것 제거
	pPerSocketCtx->recvContext->ReleaseBuffer();
	pPerSocketCtx->sendContext->ReleaseBuffer();
	DeallocPerIoContextForRecv(pPerSocketCtx->recvContext);
	DeallocPerIoContextForSend(pPerSocketCtx->sendContext);

	// 소켓 컨텍스트 제거 
	BOOL bRet=m_pPerSocketCtxMemPool->Free(pPerSocketCtx);
	assert(bRet);
}

void CMemoryPool::DeallocPerIoContextForSend(pPerIoContext pPerSendIoCtx)
{
	assert(pPerSendIoCtx);
	BOOL bRet = m_pSendMemPool->Free(pPerSendIoCtx);
	assert(bRet);

}

// PerIoContext 메모리 제거
void CMemoryPool::DeallocPerIoContextForRecv(pPerIoContext pPerRecvIoCtx)
{
	assert(pPerRecvIoCtx);
	BOOL bRet = m_pRecvMemPool->Free(pPerRecvIoCtx);
	assert(bRet);
}
