#include "StdAfx.h"
#include "MainData.h"
#include "MemoryData_Review.h"
#include "GlassResult.h"

using namespace MemoryInterface;
using namespace MemoryInterface_Review;

CMainData::CMainData(void)
{
	m_pAoiMemory		= NULL;
	m_pReviewMemory		= NULL;
	m_pControlMemory	= NULL;

	InitData();
	
	InitializeCriticalSection(&m_csResultData);	
}

CMainData::~CMainData(void)
{
	DeinitData();

	DeleteCriticalSection(&m_csResultData); 
}

void CMainData::IMI2P_SendSignalValue(int nType, int nSignalType, int nSignalIndex, int nValue)
{
	switch(nType)
	{
	case MemoryType_AOI:
		ProcessSignal_Aoi(nSignalType, nSignalIndex, nValue);
		break;

	case MemoryType_Review:
		ProcessSignal_Review(nSignalType, nSignalIndex, nValue);
		break;

	case MemoryType_Control:
		ProcessSignal_Control(nSignalType, nSignalIndex, nValue);
		break;
	}
}

void CMainData::InitData()
{
	InitAoiInterfaceMemory();

	InitReviewInterfaceMemory();

	InitControlInterfaceMemory();

	InitGlassResult();

	InitModuleStatus();
}

void CMainData::DeinitData()
{
	DeinitModuleStatus();

	DeinitGlassResult();

	DeinitControlInterfaceMemory();

	DeinitReviewInterfaceMemory();

	DeinitAoiInterfaceMemory();
}


// Aoi interface memory
int	CMainData::InitAoiInterfaceMemory()
{
	if (m_pAoiMemory) delete m_pAoiMemory;
	m_pAoiMemory = new CMemoryInterface_Aoi(MemoryType_AOI, static_cast<IMemoryInterface2Parent*>(this), 50);
	return 1;
}

void CMainData::DeinitAoiInterfaceMemory()
{
	if (m_pAoiMemory)
	{
		delete m_pAoiMemory;
		m_pAoiMemory = NULL;
	}
}

// review interface memory
int	CMainData::InitReviewInterfaceMemory()
{
	if (m_pReviewMemory) delete m_pReviewMemory;
	m_pReviewMemory = new CMemoryInterface_Review(MemoryType_Review, static_cast<IMemoryInterface2Parent*>(this), 50);
	return 1;
}

void CMainData::DeinitReviewInterfaceMemory()
{
	if (m_pReviewMemory)
	{
		delete m_pReviewMemory;
		m_pReviewMemory = NULL;
	}
}

// control interface memory
int	CMainData::InitControlInterfaceMemory()
{
	if (m_pControlMemory) delete m_pControlMemory;
	m_pControlMemory = new CMemoryInterface_Control(MemoryType_Control, static_cast<IMemoryInterface2Parent*>(this), 50);
	return 1;
}

void CMainData::DeinitControlInterfaceMemory()
{
	if (m_pControlMemory)
	{
		delete m_pControlMemory;
		m_pControlMemory = NULL;
	}
}

// glass result
int	CMainData::InitGlassResult()
{
	// glass result
	m_nTotalGlassResultCount		= 0;
	m_nCurrentGlassResultIndex		= 0;
	m_pCurrentGlassResult			= NULL;
	m_vecGlassResult.reserve(MAX_GLASS_RESULT_COUNT);
	return 1;
}

void CMainData::DeinitGlassResult()
{
	int nGlassResultCount = (int)m_vecGlassResult.size();
	for(int i=0; i<nGlassResultCount; i++)
	{
		CGlassResult* pGlassResult;
		pGlassResult = m_vecGlassResult[i];
		delete pGlassResult;
	}
	m_vecGlassResult.clear();
}

// module status
int	CMainData::InitModuleStatus()
{
	return 1;
}

void CMainData::DeinitModuleStatus()
{

}


void CMainData::ProcessSignal_Aoi(int nSignalType, int nSignalIndex, int nValue)
{

}

void CMainData::ProcessSignal_Review(int nSignalType, int nSignalIndex, int nValue)
{

}

void CMainData::ProcessSignal_Control(int nSignalType, int nSignalIndex, int nValue)
{

}


CGlassResult* CMainData::GlassResult_GetNewGlassResult(int& nTotalCount)
{
	EnterCriticalSection(&m_csResultData);
	
	if(m_vecGlassResult.size() < MAX_GLASS_RESULT_COUNT)
	{
		CGlassResult *pGlassResult = new CGlassResult();
		m_vecGlassResult.push_back(pGlassResult);
		m_pCurrentGlassResult = pGlassResult;
		m_nCurrentGlassResultIndex = int(m_vecGlassResult.size() - 1);
	}
	else
	{
		m_nCurrentGlassResultIndex++;
		if(m_nCurrentGlassResultIndex >= MAX_GLASS_RESULT_COUNT)
		{
			m_nCurrentGlassResultIndex = 0;
		}
		m_vecGlassResult.at(m_nCurrentGlassResultIndex)->Reset();	
		m_pCurrentGlassResult = m_vecGlassResult.at(m_nCurrentGlassResultIndex);
	}

	m_nTotalGlassResultCount += 1;

	nTotalCount = m_nTotalGlassResultCount;

	LeaveCriticalSection(&m_csResultData);

	return m_pCurrentGlassResult;
}


CGlassResult* CMainData::GlassResult_GetGlassResult(int nIndex)
{
	EnterCriticalSection(&m_csResultData);

	CGlassResult* pGlassResult = NULL;

	if (m_vecGlassResult.size() >= MAX_GLASS_RESULT_COUNT)	
	{
		int nSIdx = (m_nCurrentGlassResultIndex + 1) % MAX_GLASS_RESULT_COUNT;
		nIndex = (nSIdx + nIndex) % MAX_GLASS_RESULT_COUNT;
	}

	int nCount = 0;
	for (VectorGlassResultIt it=m_vecGlassResult.begin(); it!=m_vecGlassResult.end(); it++)
	{
		if (nCount==nIndex)
		{
			pGlassResult = *it;
			break;
		}
		nCount++;
	}

	LeaveCriticalSection(&m_csResultData);

	return pGlassResult;
}

int	CMainData::GlassResult_GetTotalGlassResultCount()			
{ 
	return m_nTotalGlassResultCount; 
}

int	CMainData::GlassResult_GetGlassResultCount()				
{ 
	return (int)m_vecGlassResult.size(); 
}

int	CMainData::GlassResult_GetCurrentGlassResultIndex()				
{ 
	return m_nCurrentGlassResultIndex; 
}

CGlassResult* CMainData::GlassResult_GetCurrentGlassResult()				
{ 
	EnterCriticalSection(&m_csResultData);

	CGlassResult *pGlassResult = m_pCurrentGlassResult;

	LeaveCriticalSection(&m_csResultData);

	return pGlassResult; 
}


// module status
int	CMainData::ModuleStatus_GetModuleStatusCount(int nModuleType) const
{
	if (nModuleType==0)
	{
		return (int)m_vecModuleStatus.size();
	}

	int nCount = 0;
	for (constVectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleType() & nModuleType)
		{
			nCount++;
		}
	}
	return nCount;
}

CModuleStatus* CMainData::ModuleStatus_GetModuleStatus(int nModuleIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return &(*it);
		}
	}
	return NULL;
}

SExternalIndex* CMainData::ModuleStatus_GetExternalIndex(int nModuleIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return it->GetExternalIndex();
		}
	}
	return NULL;
}

SCameraStatus* CMainData::ModuleStatus_GetCameraStatus(int nModuleIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return it->GetCameraStatus();
		}
	}
	return NULL;
}

SMotorPosition*	CMainData::ModuleStatus_GetMotorPosition(int nModuleIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return it->GetMotorPosition();
		}
	}
	return NULL;
}

SMotorStatus* CMainData::ModuleStatus_GetMotorStatus(int nModuleIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return it->GetMotorStatus();
		}
	}
	return NULL;
}

SAxisIndex*	CMainData::ModuleStatus_GetAxisIndex(int nModuleIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return it->GetAxisIndex();
		}
	}
	return NULL;
}

SCoordInfo*	CMainData::ModuleStatus_GetCoordInfo(int nModuleIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return it->GetCoordInfo();
		}
	}
	return NULL;
}

SCameraInfo* CMainData::ModuleStatus_GetCurrentCameraInfo(int nModuleIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return it->GetCurrentCameraInfo();
		}
	}
	return NULL;
}

SCameraInfo* CMainData::ModuleStatus_GetCameraInfo(int nModuleIdx, int nMagIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return it->GetCameraInfo(nMagIdx);
		}
	}
	return NULL;
}

SMagnificInfo* CMainData::ModuleStatus_GetCurrentMagnificInfo(int nModuleIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return it->GetCurrentMagnificInfo();
		}
	}
	return NULL;
}

SMagnificInfo* CMainData::ModuleStatus_GetMagnificInfo(int nModuleIdx, int nMagIdx)
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetModuleIndex() == nModuleIdx)
		{
			return it->GetMagnificInfo(nMagIdx);
		}
	}
	return NULL;
}

// standard module status
int	CMainData::ModuleStatus_GetStandardModuleIndex() const
{
	for (constVectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetStandard())
		{
			return it->GetModuleIndex();
		}
	}
	return -1;
}

CModuleStatus* CMainData::ModuleStatus_GetStandardModuleStatus()
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetStandard())
		{
			return &(*it);
		}
	}
	return NULL;
}

SMotorPosition* CMainData::ModuleStatus_GetStandardMotorPosition()
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetStandard())
		{
			return it->GetMotorPosition();
		}
	}
	return NULL;
}

SAxisIndex*	CMainData::ModuleStatus_GetStandardAxisIndex()
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetStandard())
		{
			return it->GetAxisIndex();
		}
	}
	return NULL;
}

// selected module status
int	CMainData::ModuleStatus_GetSelectedModuleIndex() const
{
	for (constVectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetSelected())
		{
			return it->GetModuleIndex();
		}
	}
	return -1;
}

CModuleStatus* CMainData::ModuleStatus_GetSelectedModuleStatus()
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetSelected())
		{
			return &(*it);
		}
	}
	return NULL;
}

SMotorPosition* CMainData::ModuleStatus_GetSelectedMotorPosition()
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetSelected())
		{
			return it->GetMotorPosition();
		}
	}
	return NULL;
}

SAxisIndex* CMainData::ModuleStatus_GetSelectedAxisIndex()
{
	for (VectorModuleStatusIt it=m_vecModuleStatus.begin(); it!=m_vecModuleStatus.end(); it++)
	{
		if(it->GetSelected())
		{
			return it->GetAxisIndex();
		}
	}
	return NULL;
}
