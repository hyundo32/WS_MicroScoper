#pragma once

class CWorkThreadData
{
public:
	CWorkThreadData(PVOID pPtr) 
	{
		pCallerPtr = pPtr;
	}
	PVOID	pCallerPtr;
};

class AFX_EXT_CLASS CThreadPools
{
public:
	CThreadPools(int nThreadCount=1);
	virtual ~CThreadPools(void);
	int	GetThreadCount() const;

private:
	void CreateThreadPools(int nThreadCount=1);
	void CloseThreadPools();

protected:
	int						m_nThreadCount;
	int						m_nRollback;
	PTP_POOL				m_pPool;
	TP_CALLBACK_ENVIRON		m_CallBackEnviron;
	PTP_CLEANUP_GROUP		m_pCleanupGroup;
};

