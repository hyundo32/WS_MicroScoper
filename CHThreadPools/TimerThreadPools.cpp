#include "StdAfx.h"
#include "TimerThreadPools.h"

CTimerThreadPools::CTimerThreadPools(DWORD dwPeriod, int nThreadCount) : m_dwPeriod(dwPeriod), CThreadPools(nThreadCount)
{
	m_pTimer			= NULL;
	m_pTimerCallback	= TimerCallback;
}

CTimerThreadPools::~CTimerThreadPools(void)
{
	StopThread();
}

DWORD CTimerThreadPools::GetPeriod() const
{
	return m_dwPeriod;
}

VOID CALLBACK CTimerThreadPools::TimerCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParameter, PTP_TIMER pTimer)
{
	// Instance, Parameter, and Work not used in this example.
	UNREFERENCED_PARAMETER(pInstance);
	UNREFERENCED_PARAMETER(pParameter);
	UNREFERENCED_PARAMETER(pTimer);

	// Do something when the work callback is invoked.
	CTimerThreadPools *pThreadPtr = static_cast<CTimerThreadPools*>(pParameter);
	if (pThreadPtr)
	{
		pThreadPtr->TimerThreadProcess(pParameter);
	}

	return;
}

BOOL CTimerThreadPools::CreateTimerThread(PVOID pParameter)
{
	if (m_pTimer) return FALSE;

	if (NULL==m_pPool || NULL==m_pCleanupGroup) return FALSE;

	m_pTimer = CreateThreadpoolTimer((PTP_TIMER_CALLBACK)TimerCallback, pParameter, &m_CallBackEnviron);
	if (NULL==m_pTimer) return FALSE;

	ULARGE_INTEGER ulDueTime;
	FILETIME FileDueTime;

	ulDueTime.QuadPart = (ULONGLONG)-(1 * 10 * 1000 * 1000);
	FileDueTime.dwHighDateTime = ulDueTime.HighPart;
	FileDueTime.dwLowDateTime = ulDueTime.LowPart;

	SetThreadpoolTimer(m_pTimer, &FileDueTime, m_dwPeriod, 0);

	return TRUE;
}

void CTimerThreadPools::CloseTimerThread()
{
	if (NULL==m_pTimer) return;

	WaitForThreadpoolTimerCallbacks(m_pTimer, TRUE);
	
	CloseThreadpoolTimer(m_pTimer);
	
	m_pTimer = NULL;
}

BOOL CTimerThreadPools::StartThread()
{
	return CreateTimerThread(this);
}

void CTimerThreadPools::StopThread()
{
	CloseTimerThread();
}