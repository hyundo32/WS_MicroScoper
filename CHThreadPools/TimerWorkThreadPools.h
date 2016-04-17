#pragma once

class CCriticalSection;

#include "ThreadPools.h"

class CTimerWorkThreadData : public CWorkThreadData
{
public:
	CTimerWorkThreadData(PVOID pPtr) : CWorkThreadData(pPtr)
	{
		nThreadIndex = -1;
	}
	int		nThreadIndex;
};

class AFX_EXT_CLASS CTimerWorkThreadPools : public CThreadPools
{
public:
	CTimerWorkThreadPools(DWORD dwPeriod=100, int nThreadCount=1);
	virtual ~CTimerWorkThreadPools(void);
	virtual BOOL StartThread();
	virtual void StopThread();

protected:
	virtual void TimerThreadProcess(PVOID pParameter) = 0;
	virtual void WorkThreadProcess(PVOID pParameter) = 0;

protected:
// 	int	LockThreadIndex();
// 	void UnlockThreadIndex(int nIndex);

	// for timer thread
	BOOL CreateTimerThread(PVOID pParameter);
	void CloseTimerThread();
	static VOID CALLBACK TimerCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParameter, PTP_TIMER pTimer);

	// for work thread
	void RunWorkThread(PVOID pParameter);
	BOOL CreateWorkThread(PVOID pParameter);
	void CloseWorkThread();
	static VOID CALLBACK WorkCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParameter, PTP_WORK pWork);

protected:
// 	bool*				m_pThreadIndex;
// 	CCriticalSection*	m_pCriticalSection;

	// for timer thread
	DWORD				m_dwPeriod;
	PTP_TIMER			m_pTimer;
	PTP_TIMER_CALLBACK	m_pTimerCallback;

	// for work thread
	PTP_WORK			m_pWork;
	PTP_WORK_CALLBACK	m_pWorkCallback;
};

