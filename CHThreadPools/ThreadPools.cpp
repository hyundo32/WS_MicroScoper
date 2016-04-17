#include "StdAfx.h"
#include "ThreadPools.h"


CThreadPools::CThreadPools(int nThreadCount) : m_nThreadCount(nThreadCount)
{
	m_nRollback			= 0;
	m_pPool				= NULL;
	m_pCleanupGroup		= NULL;

	CreateThreadPools(m_nThreadCount);
}

CThreadPools::~CThreadPools(void)
{
	CloseThreadPools();
}

int	CThreadPools::GetThreadCount() const
{
	return m_nThreadCount;
}

void CThreadPools::CreateThreadPools(int nThreadCount)
{
	BOOL bRet = FALSE;

	InitializeThreadpoolEnvironment(&m_CallBackEnviron);

	// Create a custom, dedicated thread pool
	m_pPool = CreateThreadpool(NULL);

	if (NULL==m_pPool) 
	{
		CloseThreadPools();
	}

	m_nRollback = 1; // pool creation succeeded

	// The thread pool is made persistent simply by setting
	// both the minimum and maximum threads to 1.
	SetThreadpoolThreadMaximum(m_pPool, nThreadCount);

	bRet = SetThreadpoolThreadMinimum(m_pPool, 1);

	if (FALSE==bRet) 
	{
		CloseThreadPools();
	}

	//Create a cleanup group for this thread pool
	m_pCleanupGroup = CreateThreadpoolCleanupGroup();

	if (NULL==m_pCleanupGroup) 
	{
		CloseThreadPools(); 
	}

	m_nRollback = 2;  // Cleanup group creation succeeded

	// Associate the callback environment with our thread pool.
	SetThreadpoolCallbackPool(&m_CallBackEnviron, m_pPool);

	// Associate the cleanup group with our thread pool.
	// Objects created with the same callback environment
	// as the cleanup group become members of the cleanup group.
	SetThreadpoolCallbackCleanupGroup(&m_CallBackEnviron, m_pCleanupGroup, NULL);

	m_nRollback = 3;  // Creation of work succeeded

}

void CThreadPools::CloseThreadPools()
{
	switch (m_nRollback) 
	{
	case 4:
	case 3:		
		// Clean up the cleanup group members.
		CloseThreadpoolCleanupGroupMembers(m_pCleanupGroup, FALSE, NULL);

	case 2:
		// Clean up the cleanup group.
		CloseThreadpoolCleanupGroup(m_pCleanupGroup);
		m_pCleanupGroup = NULL;

	case 1:
		// Clean up the pool.
		CloseThreadpool(m_pPool);
		m_pPool = NULL;

	default:
		break;
	}
}