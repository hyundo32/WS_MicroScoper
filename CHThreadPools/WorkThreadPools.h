#pragma once

#include "ThreadPools.h"

class AFX_EXT_CLASS CWorkThreadPools : public CThreadPools
{
public:
	CWorkThreadPools(int nThreadCount=1);
	virtual ~CWorkThreadPools(void);

protected:
	virtual void WorkThreadProcess(PVOID pParameter) = 0;

protected:
	void RunWorkThread(PVOID pParameter);
	BOOL CreateWorkThread(PVOID pParameter);
	void CloseWorkThread();
	static VOID CALLBACK WorkCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParameter, PTP_WORK pWork);

protected:
	PTP_WORK				m_pWork;
	PTP_WORK_CALLBACK		m_pWorkCallback;
};

