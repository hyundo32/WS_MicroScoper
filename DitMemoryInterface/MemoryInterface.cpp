#include "StdAfx.h"
#include "MemoryInterface.h"


CMemoryInterface::CMemoryInterface(int nType, IMemoryInterface2Parent* pIMI2P, DWORD dwPeriod) : m_nMemoryType(nType), m_pIMI2P(pIMI2P), CTimerThreadPools(dwPeriod)
{
	m_pSemaPhore		= NULL; 
	m_hAsignedMemory	= NULL;
	m_pSharedMemory		= NULL;
	m_dwMemorySize		= 0;
}

CMemoryInterface::~CMemoryInterface(void)
{
	DeleteMemory();
}

void CMemoryInterface::DeleteMemory()
{
	if(m_pSharedMemory)
	{
		UnmapViewOfFile(m_pSharedMemory); 		
		m_pSharedMemory = NULL;		
	}

	if(m_hAsignedMemory)
	{
		CloseHandle(m_hAsignedMemory);
		m_hAsignedMemory = NULL;
	}

	if (m_pSemaPhore)
	{
		delete m_pSemaPhore;
		m_pSemaPhore = NULL;
	}

	m_dwMemorySize = 0;
}

BOOL CMemoryInterface::CreateMemory(DWORD64 dwSize, const CString& strName)
{
	DeleteMemory(); 
	
	m_dwMemorySize		= -1;
	DWORD64 nMaxSize	= dwSize;

	// check size
	if (nMaxSize>=m_dwMemorySize) 
	{
		return FALSE;
	}

	// set size
	m_dwMemorySize = DWORD(nMaxSize);

	// set name
	CString strTemp = strName;

	// Create File
	m_hAsignedMemory = (HANDLE) CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, m_dwMemorySize, strTemp);
	if(m_hAsignedMemory==NULL) 
	{
		DeleteMemory();
		return FALSE;
	}

	// Map File
	m_pSharedMemory = (BYTE *)MapViewOfFile(m_hAsignedMemory, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if(m_pSharedMemory==NULL) 
	{
		DeleteMemory();
		return FALSE;
	}

	// Alloc Semaphore
	m_pSemaPhore = new CSemaphore(1, 1);

	// Timer Thread
	if (m_pIMI2P)
	{
		return CreateTimerThread(this);
	}

	return TRUE;
}

BOOL CMemoryInterface::StartThread()
{
	return CreateTimerThread(this);
}

void CMemoryInterface::StopThread()
{
	CTimerThreadPools::StopThread();
}


LPVOID CMemoryInterface::GetMemoryAddress(DWORD dwAddress)
{
	if (m_pSharedMemory==NULL) return NULL;

	if (dwAddress >= m_dwMemorySize) return NULL;

	return &(m_pSharedMemory[dwAddress]);
}

const LPVOID CMemoryInterface::GetMemoryAddress(DWORD dwAddress) const
{
	if (m_pSharedMemory==NULL) return NULL;

	if (dwAddress >= m_dwMemorySize) return NULL;

	return &(m_pSharedMemory[dwAddress]);
}

LPVOID	CMemoryInterface::GetMemoryData()
{
	return GetMemoryAddress(0);
}

const LPVOID	CMemoryInterface::GetMemoryData() const
{
	return GetMemoryAddress(0);
}

void CMemoryInterface::TimerThreadProcess(PVOID pParameter)
{
	if (m_pIMI2P==NULL) return;

	const CMemoryData* pData = static_cast<const CMemoryData*>(GetMemoryAddress(0));
	if (pData==NULL) return;

	// check prev signal
	for (int nSignalType=0; nSignalType<MAX_SIGNAL_TYPE_COUNT; nSignalType++)
	{
		if(pData->m_dwSendSignal[nSignalType] != m_prevMemoryData.m_dwSendSignal[nSignalType]) 
		{
			int nBitCount = sizeof(CMemoryData) * 8;

			DWORD64 dwBitValue = 1;
			BOOL bCurValue = FALSE;
			BOOL bPrevValue = FALSE;
			for (int nSignalIdx=0; nSignalIdx<nBitCount; nSignalIdx++)
			{
				// compare current and prev
				bCurValue = BOOL(dwBitValue & pData->m_dwSendSignal[nSignalType]);
				bPrevValue = BOOL(dwBitValue & m_prevMemoryData.m_dwSendSignal[nSignalType]);

				// change check
				if ( bCurValue != bPrevValue )
				{
					m_pIMI2P->IMI2P_SendSignalValue(m_nMemoryType, nSignalType, nSignalIdx, bCurValue);
				}			

				dwBitValue = dwBitValue << 1;
			}
		}
	}
		
	// save current signal
	m_prevMemoryData = *pData;
}