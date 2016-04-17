#include "StdAfx.h"
#include "WaitThreadPools.h"

CWaitThreadPools::CWaitThreadPools(int nThreadCount) : CThreadPools(nThreadCount)
{
	m_pWait				= NULL;
	m_pWaitCallback		= WaitCallback;

	m_hEvent			= CreateEvent(NULL, FALSE, FALSE, NULL);
}

CWaitThreadPools::~CWaitThreadPools(void)
{
	CloseWaitThread();

	CloseHandle(m_hEvent);
}

VOID CALLBACK CWaitThreadPools::WaitCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParameter, PTP_WAIT pWait, TP_WAIT_RESULT WaitResult)
{
	// Instance, Parameter, and Work not used in this example.
	UNREFERENCED_PARAMETER(pInstance);
	UNREFERENCED_PARAMETER(pParameter);
	UNREFERENCED_PARAMETER(pWait);
	UNREFERENCED_PARAMETER(WaitResult);

	return;
}

BOOL CWaitThreadPools::CreateWaitThread(PVOID pParameter)
{
	if (NULL==m_pPool || NULL==m_pCleanupGroup) return FALSE;

	PTP_WAIT pWait = CreateThreadpoolWait((PTP_WAIT_CALLBACK)WaitCallback, pParameter, &m_CallBackEnviron);
	if (NULL==pWait) return FALSE;

	SetThreadpoolWait(pWait, m_hEvent, NULL);

	return TRUE;
}

void CWaitThreadPools::CloseWaitThread()
{
	if (NULL==m_pWait) return;

	WaitForThreadpoolWaitCallbacks(m_pWait, TRUE);

	CloseThreadpoolWait(m_pWait);

	m_pWait = NULL;
}

BOOL CWaitThreadPools::SetEvent1()
{
	if (m_hEvent==NULL) return FALSE;

	return SetEvent(m_hEvent);
}

BOOL CWaitThreadPools::ResetEvent1()
{
	if (m_hEvent==NULL) return FALSE;

	return ResetEvent(m_hEvent);
}