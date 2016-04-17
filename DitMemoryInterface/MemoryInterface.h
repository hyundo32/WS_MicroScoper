#pragma once

#include <afxmt.h>
#include "TimerThreadPools.h"

#define MAX_SIGNAL_TYPE_COUNT	5

namespace MemoryInterface
{
	enum MemoryType		{ MemoryType_AOI=0, MemoryType_Review, MemoryType_Control, MemoryType_Count };	
};

interface IMemoryInterface2Parent
{
	virtual void IMI2P_SendSignalValue(int nType, int nSignalType, int nSignalIndex, int nValue) = 0;
};

class CMemoryData
{
public:
	CMemoryData()	
	{ 
		memset(this, 0, sizeof(CMemoryData)); 
	}
	DWORD64	m_dwSendSignal[MAX_SIGNAL_TYPE_COUNT];
	DWORD64	m_dwRecvSignal[MAX_SIGNAL_TYPE_COUNT];
};

class AFX_EXT_CLASS CMemoryInterface : public CTimerThreadPools
{
public:
	CMemoryInterface(int nType, IMemoryInterface2Parent* pIMI2P=NULL, DWORD dwPeriod=50);
	virtual ~CMemoryInterface(void);
	BOOL	LockMemory();
	BOOL	UnlockMemory();

	// CTimerThreadPool	
	virtual BOOL	StartThread();
	virtual void	StopThread();


	LPVOID			GetMemoryData();
	const LPVOID	GetMemoryData() const;

protected:
	BOOL			CreateMemory(DWORD64 dwSize, const CString& strName);
	void			DeleteMemory();
	
	LPVOID			GetMemoryAddress(DWORD dwAddress);
	const LPVOID	GetMemoryAddress(DWORD dwAddress) const;

	virtual void	TimerThreadProcess(PVOID pParameter);

protected:
	IMemoryInterface2Parent*	m_pIMI2P;	
	int							m_nMemoryType;
	CMemoryData					m_prevMemoryData;

private:
	DWORD						m_dwMemorySize;
	CSemaphore*					m_pSemaPhore;
	HANDLE						m_hAsignedMemory;
	BYTE*						m_pSharedMemory;
};

