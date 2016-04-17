#pragma once

#include "ThreadPools.h"

class AFX_EXT_CLASS CTimerThreadPools : public CThreadPools
{
public:
	CTimerThreadPools(DWORD dwPeriod=100, int nThreadCount=1);
	virtual ~CTimerThreadPools(void);
	DWORD GetPeriod() const;
	virtual BOOL StartThread();
	virtual void StopThread();

protected:
	virtual void TimerThreadProcess(PVOID pParameter) = 0;

protected:
	BOOL CreateTimerThread(PVOID pParameter);
	void CloseTimerThread();
	static VOID CALLBACK TimerCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParameter, PTP_TIMER pTimer);

protected:
	DWORD					m_dwPeriod;
	PTP_TIMER				m_pTimer;
	PTP_TIMER_CALLBACK		m_pTimerCallback;
};

