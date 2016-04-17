#include "StdAfx.h"
#include "TimerWorkThreadPools.h"
#include <afxmt.h> 

CTimerWorkThreadPools::CTimerWorkThreadPools(DWORD dwPeriod, int nThreadCount) : m_dwPeriod(dwPeriod), CThreadPools(nThreadCount)
{
	// for timer thread
	m_pTimer			= NULL;
	m_pTimerCallback	= TimerCallback;

	// for work thread
	m_pWork				= NULL;
	m_pWorkCallback		= WorkCallback;

// 	m_pThreadIndex		= new bool[m_nThreadCount];
// 	for (int i=0; i<m_nThreadCount; i++) 
// 	{
// 		m_pThreadIndex[i] = false;
// 	}
// 	m_pCriticalSection	= new CCriticalSection();
}

CTimerWorkThreadPools::~CTimerWorkThreadPools(void)
{
	CloseTimerThread();

	CloseWorkThread();

// 	if (m_pThreadIndex) 
// 	{
// 		delete [] m_pThreadIndex;
// 		m_pThreadIndex = NULL;
// 	}
// 
// 	if (m_pCriticalSection)
// 	{
// 		delete m_pCriticalSection;
// 		m_pCriticalSection = NULL;
// 	}
}

/*
int	CTimerWorkThreadPools::LockThreadIndex()
{
	if (m_pCriticalSection==NULL || m_pThreadIndex==NULL) return -1;

	m_pCriticalSection->Lock();

	int nThreadIdx = -1;
	for (int i=0; i<m_nThreadCount; i++)
	{
		if (!m_pThreadIndex[i])
		{
			m_pThreadIndex[i] = true;

			nThreadIdx = i;
			break;
		}
	}

	m_pCriticalSection->Unlock();

	return nThreadIdx;
}

void CTimerWorkThreadPools::UnlockThreadIndex(int nIndex)
{
	if (m_pCriticalSection==NULL || m_pThreadIndex==NULL) return;

	m_pCriticalSection->Lock();

	if (nIndex>-1 && nIndex<m_nThreadCount) 
	{
		m_pThreadIndex[nIndex] = false;
	}

	m_pCriticalSection->Unlock();
}

*/

BOOL CTimerWorkThreadPools::StartThread()
{
	return CreateTimerThread(this);
}

void CTimerWorkThreadPools::StopThread()
{
	CloseTimerThread();
}

VOID CALLBACK CTimerWorkThreadPools::TimerCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParameter, PTP_TIMER pTimer)
{
	// Instance, Parameter, and Work not used in this example.
	UNREFERENCED_PARAMETER(pInstance);
	UNREFERENCED_PARAMETER(pParameter);
	UNREFERENCED_PARAMETER(pTimer);

	// Do something when the work callback is invoked.
	CTimerWorkThreadPools *pThreadPtr = static_cast<CTimerWorkThreadPools*>(pParameter);
	if (pThreadPtr)
	{
		pThreadPtr->TimerThreadProcess(pParameter);
	}

	return;
}

BOOL CTimerWorkThreadPools::CreateTimerThread(PVOID pParameter)
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

void CTimerWorkThreadPools::CloseTimerThread()
{
	if (NULL==m_pTimer) return;

	WaitForThreadpoolTimerCallbacks(m_pTimer, TRUE);

	CloseThreadpoolTimer(m_pTimer);

	m_pTimer = NULL;
}

VOID CALLBACK CTimerWorkThreadPools::WorkCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParameter, PTP_WORK pWork)
{
	// Instance, Parameter, and Work not used in this example.
	UNREFERENCED_PARAMETER(pInstance);
	UNREFERENCED_PARAMETER(pParameter);
	UNREFERENCED_PARAMETER(pWork);

	// Do something when the work callback is invoked.
	CTimerWorkThreadData *pDataPtr = static_cast<CTimerWorkThreadData*>(pParameter);
	if (pDataPtr)
	{
		CTimerWorkThreadPools *pThreadPtr = static_cast<CTimerWorkThreadPools*>(pDataPtr->pCallerPtr);
		pThreadPtr->RunWorkThread(pParameter);
	}

	return;
}

BOOL CTimerWorkThreadPools::CreateWorkThread(PVOID pParameter)
{
	if (NULL==m_pPool || NULL==m_pCleanupGroup) return FALSE;

	PTP_WORK pWork = CreateThreadpoolWork((PTP_WORK_CALLBACK)WorkCallback, pParameter, &m_CallBackEnviron);
	if (NULL==pWork) return FALSE;

	SubmitThreadpoolWork(pWork);

	return TRUE;
}

void CTimerWorkThreadPools::CloseWorkThread()
{
	if (NULL==m_pWork) return;

	WaitForThreadpoolWorkCallbacks(m_pWork, TRUE);

	CloseThreadpoolWork(m_pWork);

	m_pWork = NULL;
}

void CTimerWorkThreadPools::RunWorkThread(PVOID pParameter)
{
	CTimerWorkThreadData *pDataPtr = static_cast<CTimerWorkThreadData*>(pParameter);
	if (pDataPtr==NULL) return;

	CTimerWorkThreadPools *pThreadPtr = static_cast<CTimerWorkThreadPools*>(pDataPtr->pCallerPtr);
	if (pThreadPtr) 
	{
		//int nThreadIndex = LockThreadIndex();
		int nThreadIndex = rand() % m_nThreadCount;
		if (nThreadIndex!=-1)
		{
			pDataPtr->nThreadIndex = nThreadIndex;

			pThreadPtr->WorkThreadProcess(pParameter);

			//UnlockThreadIndex(nThreadIndex);
		}
	}

	delete pParameter;
}