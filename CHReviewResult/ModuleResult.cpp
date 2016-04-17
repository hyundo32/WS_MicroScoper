#include "StdAfx.h"
#include "ModuleResult.h"


CModuleResult::CModuleResult(int nModuleIdx) : m_nModuleIndex(nModuleIdx)
{
	Reset();
}

CModuleResult::~CModuleResult(void)
{
	Reset();
}

void CModuleResult::Reset(void)
{
	m_nLastResultIndex		= -1;
	m_nStartResultIndex		= 0;
	m_nEndResultIndex		= MAX_MOTOR_ADDRESS_SIZE-1;
	m_dwLastResultTick		= 0;
	m_vecResultData.clear();
}

void CModuleResult::SetModuleIndex(int nIdx)		
{ 
	m_nModuleIndex = nIdx; 
}

int	CModuleResult::GetModuleIndex() const				
{ 
	return m_nModuleIndex; 
}

void CModuleResult::SetLastResultIndex(int nIdx)			
{ 
	m_nLastResultIndex = nIdx; 
}

int	CModuleResult::GetLastResultIndex()	const			
{ 
	return m_nLastResultIndex; 
}

void CModuleResult::SetStartResultIndex(int nIdx)	
{ 
	m_nStartResultIndex = nIdx;
}

int	CModuleResult::GetStartResultIndex() const		
{ 
	return m_nStartResultIndex;
}

void CModuleResult::SetEndResultIndex(int nIdx)		
{ 
	m_nEndResultIndex = nIdx;
}

int	CModuleResult::GetEndResultIndex() const		
{
	return m_nEndResultIndex;
}

void CModuleResult::SetLastResultTick(DWORD dwTick)		
{ 
	m_dwLastResultTick = dwTick; 
}

DWORD CModuleResult::GetLastResultTick() const		
{ 
	return m_dwLastResultTick; 
}

float CModuleResult::GetTotalDurationTime() const
{
	float fTime = 0.f;
	for (constVectorDefectResultIt it=m_vecResultData.begin(); it!=m_vecResultData.end(); it++)
	{
		fTime += it->m_fDurationTime;
	}
	return fTime;
}

float CModuleResult::GetTotalDurationTime(int nIndex) const
{
	if (nIndex<0) return -1.f;

	float fTime = 0.f;
	for (int i=0; i<(int)m_vecResultData.size(); i++)
	{
		fTime += m_vecResultData[i].m_fDurationTime;
	}

	return fTime;
}

float CModuleResult::GetDurationTime(int nIndex) const
{
	if (nIndex<0 || nIndex>=(int)m_vecResultData.size()) return -1.f;

	return m_vecResultData[nIndex].m_fDurationTime;
}

float CModuleResult::GetDurationTime(int nStartIdx, int nEndIdx) const
{
	if (nStartIdx<0) return -1.f;

	if (nStartIdx>nEndIdx) return -1.f;

	if (nEndIdx>=(int)m_vecResultData.size()) return -1.f;

	float fTime = 0.f;
	for (int i=nStartIdx; i<=nEndIdx; i++)
	{
		fTime += m_vecResultData[i].m_fDurationTime;
	}

	return fTime;
}

void CModuleResult::AddResult(const CDefectData_Review& result)
{
	m_vecResultData.push_back(result);
}

void CModuleResult::ResizeResult(int nCount)
{
	if (nCount < 0) return;

	m_vecResultData.resize(nCount);
}

int	CModuleResult::GetResultCount(int nResultCode) const
{
	if (nResultCode==ReviewResult_None) 
	{
		return (int)m_vecResultData.size();
	}

	int nCount = 0;
	for (constVectorDefectResultIt it=m_vecResultData.begin(); it!=m_vecResultData.end(); it++)
	{
		if ((it->m_nResultCode & nResultCode))
		{
			nCount++;
		}
	}

	return nCount;
}

BOOL CModuleResult::CheckComplete() const
{
	for (constVectorDefectResultIt it=m_vecResultData.begin(); it!=m_vecResultData.end(); it++)
	{
		if (it->m_nResultCode < ReviewResult_Process_Fail)
		{
			return FALSE;
		}
	}
	return TRUE;
}

CDefectData_Review* CModuleResult::GetResult(int nIndex)
{
	if (nIndex<0 || nIndex>=(int)m_vecResultData.size()) return NULL;

	return &m_vecResultData[nIndex];
}

const CDefectData_Review* CModuleResult::GetResult(int nIndex) const
{
	if (nIndex<0 || nIndex>=(int)m_vecResultData.size()) return NULL;

	return &m_vecResultData[nIndex];
}
