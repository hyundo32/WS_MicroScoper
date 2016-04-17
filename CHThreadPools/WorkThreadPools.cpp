#include "StdAfx.h"
#include "WorkThreadPools.h"

CWorkThreadPools::CWorkThreadPools(int nThreadCount) : CThreadPools(nThreadCount)
{
	m_pWork				= NULL;
	m_pWorkCallback		= WorkCallback;
}

CWorkThreadPools::~CWorkThreadPools(void)
{
	CloseWorkThread();
}

VOID CALLBACK CWorkThreadPools::WorkCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParameter, PTP_WORK pWork)
{
	// Instance, Parameter, and Work not used in this example.
	UNREFERENCED_PARAMETER(pInstance);
	UNREFERENCED_PARAMETER(pParameter);
	UNREFERENCED_PARAMETER(pWork);

	// Do something when the work callback is invoked.
	CWorkThreadData *pDataPtr = static_cast<CWorkThreadData*>(pParameter);
	if (pDataPtr)
	{
		CWorkThreadPools *pThreadPtr = static_cast<CWorkThreadPools*>(pDataPtr->pCallerPtr);
		pThreadPtr->RunWorkThread(pParameter);
	}

	return;
}

BOOL CWorkThreadPools::CreateWorkThread(PVOID pParameter)
{
	if (NULL==m_pPool || NULL==m_pCleanupGroup) return FALSE;

	PTP_WORK pWork = CreateThreadpoolWork((PTP_WORK_CALLBACK)WorkCallback, pParameter, &m_CallBackEnviron);
	if (NULL==pWork) return FALSE;

	SubmitThreadpoolWork(pWork);

	return TRUE;
}

void CWorkThreadPools::CloseWorkThread()
{
	if (NULL==m_pWork) return;

	WaitForThreadpoolWorkCallbacks(m_pWork, TRUE);

	CloseThreadpoolWork(m_pWork);

	m_pWork = NULL;
}

void CWorkThreadPools::RunWorkThread(PVOID pParameter)
{
	CWorkThreadData *pDataPtr = static_cast<CWorkThreadData*>(pParameter);
	if (pDataPtr==NULL) return;

	CWorkThreadPools *pThreadPtr = static_cast<CWorkThreadPools*>(pDataPtr->pCallerPtr);
	if (pThreadPtr) 
	{
		pThreadPtr->WorkThreadProcess(pParameter);
	}

	delete pParameter;
}

