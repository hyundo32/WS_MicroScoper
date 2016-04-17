#include "StdAfx.h"
#include "GlassResult.h"

CGlassResult::CGlassResult(void)
{
	InitializeCriticalSection(&m_csProcess);
	InitializeCriticalSection(&m_csDefect);
	Reset();
}

CGlassResult::~CGlassResult(void)
{
	Reset();
	DeleteCriticalSection(&m_csProcess);
	DeleteCriticalSection(&m_csDefect);
}

void CGlassResult::Reset()
{
	m_nCurrentDefectIndex = 0;

	// module result
	m_vecModuleResult_Review.clear();
	m_vecModuleResult_User.clear();
	m_vecModuleResult_Wsi.clear();
	m_vecModuleResult_Measure.clear();
}

// glass result
CGlassData_Review* CGlassResult::GetGlassResult()
{
	return &m_GlassResult;
}

const CGlassData_Review* CGlassResult::GetGlassResult() const
{
	return &m_GlassResult;
}

// recipe result
CRecipeData_Review*	CGlassResult::GetRecipeResult()
{
	return &m_RecipeResult;
}

const CRecipeData_Review* CGlassResult::GetRecipeResult() const
{
	return &m_RecipeResult;
}

// align result
CAlignData_Review* CGlassResult::GetAlignResult()
{
	return &m_AlignResult;
}

const CAlignData_Review* CGlassResult::GetAlignResult() const
{
	return &m_AlignResult;
}

// cell result
int	CGlassResult::GetCellResultCount() const
{
	return (int)m_vecCellResult.size();
}

CCellData_Review* CGlassResult::GetCellResult(int nCellIdx)
{
	if (nCellIdx<0 || nCellIdx>=GetCellResultCount()) return NULL;
	return &(m_vecCellResult[nCellIdx]);
}

const CCellData_Review*	CGlassResult::GetCellResult(int nCellIdx) const
{
	if (nCellIdx<0 || nCellIdx>=GetCellResultCount()) return NULL;
	return &(m_vecCellResult[nCellIdx]);
}

void CGlassResult::AddCellResult(const CCellData_Review& cellResult)
{
	m_vecCellResult.push_back(cellResult);
}

// defect result
int	CGlassResult::GetDefectResultCount() const
{
	return (int)m_mapDefectResult.size();
}

CDefectData_Review* CGlassResult::GetDefectResult(int nDefectIdx)
{
	CDefectData_Review* pDefectResult = NULL;
	for(MapDefectResultIt it=m_mapDefectResult.begin();it!=m_mapDefectResult.end();it++)
	{
		pDefectResult = static_cast<CDefectData_Review*>(it->second);
		if (pDefectResult==NULL) continue;
		if (pDefectResult->m_nDefectIndex!=nDefectIdx) continue;
		return pDefectResult;
	}
	return pDefectResult;
}

const CDefectData_Review* CGlassResult::GetDefectResult(int nDefectIdx) const
{
	const CDefectData_Review* pDefectResult = NULL;
	for(constMapDefectResultIt it=m_mapDefectResult.begin();it!=m_mapDefectResult.end();it++)
	{
		pDefectResult = static_cast<const CDefectData_Review*>(it->second);
		if (pDefectResult==NULL) continue;
		if (pDefectResult->m_nDefectIndex!=nDefectIdx) continue;
		return pDefectResult;
	}
	return pDefectResult;
}

BOOL CGlassResult::AddDefectResult(CDefectData_Review* pDefectResult)
{
	EnterCriticalSection(&m_csDefect);

	if (pDefectResult==NULL)
	{
		LeaveCriticalSection(&m_csDefect);
		return FALSE;
	}

	pDefectResult->m_nDefectIndex = m_nCurrentDefectIndex;
	m_mapDefectResult.insert(std::make_pair(pDefectResult->m_nOriginPosX, pDefectResult));
	m_nCurrentDefectIndex++;

	//ClassifyDefectResult(pDefectResult);

	LeaveCriticalSection(&m_csDefect);

	return TRUE;
}


// review result
int	CGlassResult::GetReviewModuleResultCount() const
{
	return (int)m_vecModuleResult_Review.size();
}

CModuleResult* CGlassResult::GetReviewModuleResult(int nModuleIdx)
{
	for (VectorModuleResultIt it=m_vecModuleResult_Review.begin(); it!=m_vecModuleResult_Review.end(); it++)
	{
		if (nModuleIdx==it->GetModuleIndex())
		{
			return static_cast<CModuleResult*>(&(*it));
		}
	}
	return NULL;
}

const CModuleResult* CGlassResult::GetReviewModuleResult(int nModuleIdx) const
{
	for (constVectorModuleResultIt it=m_vecModuleResult_Review.begin(); it!=m_vecModuleResult_Review.end(); it++)
	{
		if (nModuleIdx==it->GetModuleIndex())
		{
			return static_cast<const CModuleResult*>(&(*it));
		}
	}
	return NULL;
}

void CGlassResult::AddReviewModuleResult(const CModuleResult& moduleResult)
{
	m_vecModuleResult_Review.push_back(moduleResult);
}

int	CGlassResult::GetTotalReviewResultCount() const
{
	int nTotalCount = 0;
	int nModuleIdx = 0, nMIdx2 = 0;
	for (nModuleIdx=0, nMIdx2 = 0; nModuleIdx<GetReviewModuleResultCount(); nModuleIdx++,nMIdx2++)
	{
		const CModuleResult *pNode = GetReviewModuleResult(nMIdx2);
		if (pNode==NULL) 
		{
			nModuleIdx--;
			continue;
		}

		nTotalCount += pNode->GetResultCount();
	}
	return nTotalCount;
}

int	CGlassResult::GetReviewResultCount(int nModuleIdx) const
{
	const CModuleResult* pModuleResult = GetReviewModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return -1;
	return pModuleResult->GetResultCount();
}

CDefectData_Review*	CGlassResult::GetReviewResult(int nModuleIdx, int nResultIdx)
{
	CModuleResult* pModuleResult = GetReviewModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return NULL;
	return pModuleResult->GetResult(nResultIdx);
}

const CDefectData_Review* CGlassResult::GetReviewResult(int nModuleIdx, int nResultIdx) const
{
	const CModuleResult* pModuleResult = GetReviewModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return NULL;
	return pModuleResult->GetResult(nResultIdx);
}

void CGlassResult::AddReviewResult(int nModuleIdx, const CDefectData_Review& result)
{
	CModuleResult* pModuleResult = GetReviewModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return;
	return pModuleResult->AddResult(result);
}

// user result
int	CGlassResult::GetUserModuleResultCount() const
{
	return (int)m_vecModuleResult_User.size();
}

CModuleResult* CGlassResult::GetUserModuleResult(int nModuleIdx)
{
	for (VectorModuleResultIt it=m_vecModuleResult_User.begin(); it!=m_vecModuleResult_User.end(); it++)
	{
		if (nModuleIdx==it->GetModuleIndex())
		{
			return static_cast<CModuleResult*>(&(*it));
		}
	}
	return NULL;
}

const CModuleResult* CGlassResult::GetUserModuleResult(int nModuleIdx) const
{
	for (constVectorModuleResultIt it=m_vecModuleResult_User.begin(); it!=m_vecModuleResult_User.end(); it++)
	{
		if (nModuleIdx==it->GetModuleIndex())
		{
			return static_cast<const CModuleResult*>(&(*it));
		}
	}
	return NULL;
}

void CGlassResult::AddUserModuleResult(const CModuleResult& moduleResult)
{
	m_vecModuleResult_User.push_back(moduleResult);
}

int	CGlassResult::GetTotalUserResultCount() const
{
	int nTotalCount = 0;
	int nModuleIdx = 0, nMIdx2 = 0;
	for (nModuleIdx=0, nMIdx2 = 0; nModuleIdx<GetUserModuleResultCount(); nModuleIdx++,nMIdx2++)
	{
		const CModuleResult *pNode = GetUserModuleResult(nMIdx2);
		if (pNode==NULL) 
		{
			nModuleIdx--;
			continue;
		}

		nTotalCount += pNode->GetResultCount();
	}
	return nTotalCount;
}

int	CGlassResult::GetUserResultCount(int nModuleIdx) const
{
	const CModuleResult* pModuleResult = GetUserModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return -1;
	return pModuleResult->GetResultCount();
}

CDefectData_Review*	CGlassResult::GetUserResult(int nModuleIdx, int nResultIdx)
{
	CModuleResult* pModuleResult = GetUserModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return NULL;
	return pModuleResult->GetResult(nResultIdx);
}

const CDefectData_Review* CGlassResult::GetUserResult(int nModuleIdx, int nResultIdx) const
{
	const CModuleResult* pModuleResult = GetUserModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return NULL;
	return pModuleResult->GetResult(nResultIdx);
}

void CGlassResult::AddUserResult(int nModuleIdx, const CDefectData_Review& result)
{
	CModuleResult* pModuleResult = GetUserModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return;
	return pModuleResult->AddResult(result);
}

// wsi result
int	CGlassResult::GetWsiModuleResultCount() const
{
	return (int)m_vecModuleResult_Wsi.size();
}

CModuleResult* CGlassResult::GetWsiModuleResult(int nModuleIdx)
{
	for (VectorModuleResultIt it=m_vecModuleResult_Wsi.begin(); it!=m_vecModuleResult_Wsi.end(); it++)
	{
		if (nModuleIdx==it->GetModuleIndex())
		{
			return static_cast<CModuleResult*>(&(*it));
		}
	}
	return NULL;
}

const CModuleResult* CGlassResult::GetWsiModuleResult(int nModuleIdx) const
{
	for (constVectorModuleResultIt it=m_vecModuleResult_Wsi.begin(); it!=m_vecModuleResult_Wsi.end(); it++)
	{
		if (nModuleIdx==it->GetModuleIndex())
		{
			return static_cast<const CModuleResult*>(&(*it));
		}
	}
	return NULL;
}

void CGlassResult::AddWsiModuleResult(const CModuleResult& moduleResult)
{
	m_vecModuleResult_Wsi.push_back(moduleResult);
}

int	CGlassResult::GetTotalWsiResultCount() const
{
	int nTotalCount = 0;
	int nModuleIdx = 0, nMIdx2 = 0;
	for (nModuleIdx=0, nMIdx2 = 0; nModuleIdx<GetWsiModuleResultCount(); nModuleIdx++,nMIdx2++)
	{
		const CModuleResult *pNode = GetWsiModuleResult(nMIdx2);
		if (pNode==NULL) 
		{
			nModuleIdx--;
			continue;
		}

		nTotalCount += pNode->GetResultCount();
	}
	return nTotalCount;
}

int	CGlassResult::GetWsiResultCount(int nModuleIdx) const
{
	const CModuleResult* pModuleResult = GetWsiModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return -1;
	return pModuleResult->GetResultCount();
}

CDefectData_Review*	CGlassResult::GetWsiResult(int nModuleIdx, int nResultIdx)
{
	CModuleResult* pModuleResult = GetWsiModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return NULL;
	return pModuleResult->GetResult(nResultIdx);
}

const CDefectData_Review* CGlassResult::GetWsiResult(int nModuleIdx, int nResultIdx) const
{
	const CModuleResult* pModuleResult = GetWsiModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return NULL;
	return pModuleResult->GetResult(nResultIdx);
}

void CGlassResult::AddWsiResult(int nModuleIdx, const CDefectData_Review& result)
{
	CModuleResult* pModuleResult = GetWsiModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return;
	return pModuleResult->AddResult(result);
}

// measure result
int	CGlassResult::GetMeasureModuleResultCount() const
{
	return (int)m_vecModuleResult_Measure.size();
}

CModuleResult* CGlassResult::GetMeasureModuleResult(int nModuleIdx)
{
	for (VectorModuleResultIt it=m_vecModuleResult_Measure.begin(); it!=m_vecModuleResult_Measure.end(); it++)
	{
		if (nModuleIdx==it->GetModuleIndex())
		{
			return static_cast<CModuleResult*>(&(*it));
		}
	}
	return NULL;
}

const CModuleResult* CGlassResult::GetMeasureModuleResult(int nModuleIdx) const
{
	for (constVectorModuleResultIt it=m_vecModuleResult_Measure.begin(); it!=m_vecModuleResult_Measure.end(); it++)
	{
		if (nModuleIdx==it->GetModuleIndex())
		{
			return static_cast<const CModuleResult*>(&(*it));
		}
	}
	return NULL;
}

void CGlassResult::AddMeasureModuleResult(const CModuleResult& moduleResult)
{
	m_vecModuleResult_Wsi.push_back(moduleResult);
}

int	CGlassResult::GetTotalMeasureResultCount() const
{
	int nTotalCount = 0;
	int nModuleIdx = 0, nMIdx2 = 0;
	for (nModuleIdx=0, nMIdx2 = 0; nModuleIdx<GetMeasureModuleResultCount(); nModuleIdx++,nMIdx2++)
	{
		const CModuleResult *pNode = GetMeasureModuleResult(nMIdx2);
		if (pNode==NULL) 
		{
			nModuleIdx--;
			continue;
		}

		nTotalCount += pNode->GetResultCount();
	}
	return nTotalCount;
}

int	CGlassResult::GetMeasureResultCount(int nModuleIdx) const
{
	const CModuleResult* pModuleResult = GetMeasureModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return -1;
	return pModuleResult->GetResultCount();
}

CDefectData_Review*	CGlassResult::GetMeasureResult(int nModuleIdx, int nResultIdx)
{
	CModuleResult* pModuleResult = GetMeasureModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return NULL;
	return pModuleResult->GetResult(nResultIdx);
}

const CDefectData_Review* CGlassResult::GetMeasureResult(int nModuleIdx, int nResultIdx) const
{
	const CModuleResult* pModuleResult = GetMeasureModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return NULL;
	return pModuleResult->GetResult(nResultIdx);
}

void CGlassResult::AddMeasureResult(int nModuleIdx, const CDefectData_Review& result)
{
	CModuleResult* pModuleResult = GetMeasureModuleResult(nModuleIdx);
	if (pModuleResult==NULL) return;
	return pModuleResult->AddResult(result);
}

BOOL CGlassResult::CheckReviewComplete(int nModuleIdx)
{
	EnterCriticalSection(&m_csProcess);

	BOOL bRet = TRUE;
	if (nModuleIdx==-1)
	{
		for (constVectorModuleResultIt it=m_vecModuleResult_Review.begin(); it!=m_vecModuleResult_Review.end(); it++)
		{
			bRet = bRet && it->CheckComplete();
			if (bRet==FALSE) break;
		}
	}
	else
	{
		bRet = FALSE;
		CModuleResult *pModuleResult = GetReviewModuleResult(nModuleIdx);
		if (pModuleResult)
		{
			bRet = pModuleResult->CheckComplete();
		}
	}

	LeaveCriticalSection(&m_csProcess);

	return bRet;
}

BOOL CGlassResult::CheckUserComplete(int nModuleIdx)
{
	EnterCriticalSection(&m_csProcess);

	BOOL bRet = TRUE;
	if (nModuleIdx==-1)
	{
		for (constVectorModuleResultIt it=m_vecModuleResult_User.begin(); it!=m_vecModuleResult_User.end(); it++)
		{
			bRet = bRet && it->CheckComplete();
			if (bRet==FALSE) break;
		}
	}
	else
	{
		bRet = FALSE;
		CModuleResult *pModuleResult = GetUserModuleResult(nModuleIdx);
		if (pModuleResult)
		{
			bRet = pModuleResult->CheckComplete();
		}
	}

	LeaveCriticalSection(&m_csProcess);

	return bRet;
}

BOOL CGlassResult::CheckWsiComplete(int nModuleIdx)
{
	EnterCriticalSection(&m_csProcess);

	BOOL bRet = TRUE;
	if (nModuleIdx==-1)
	{
		for (constVectorModuleResultIt it=m_vecModuleResult_Wsi.begin(); it!=m_vecModuleResult_Wsi.end(); it++)
		{
			bRet = bRet && it->CheckComplete();
			if (bRet==FALSE) break;
		}
	}
	else
	{
		bRet = FALSE;
		CModuleResult *pModuleResult = GetWsiModuleResult(nModuleIdx);
		if (pModuleResult)
		{
			bRet = pModuleResult->CheckComplete();
		}
	}

	LeaveCriticalSection(&m_csProcess);

	return bRet;
}

BOOL CGlassResult::CheckMeasureComplete(int nModuleIdx)
{
	EnterCriticalSection(&m_csProcess);

	BOOL bRet = TRUE;
	if (nModuleIdx==-1)
	{
		for (constVectorModuleResultIt it=m_vecModuleResult_Measure.begin(); it!=m_vecModuleResult_Measure.end(); it++)
		{
			bRet = bRet && it->CheckComplete();
			if (bRet==FALSE) break;
		}
	}
	else
	{
		bRet = FALSE;
		CModuleResult *pModuleResult = GetMeasureModuleResult(nModuleIdx);
		if (pModuleResult)
		{
			bRet = pModuleResult->CheckComplete();
		}
	}

	LeaveCriticalSection(&m_csProcess);

	return bRet;
}

CDefectData_Review* CGlassResult::SetReviewResultData(int nModuleIdx, int nResultIdx, VectorImageData& vectorImageData, int nGlassPosX, int nGlassPosY, int nMotorPosX, int nMotorPosY, float fDurationTime)
{
	EnterCriticalSection(&m_csProcess);

	CDefectData_Review *pResult = GetReviewResult(nModuleIdx, nResultIdx);

	if (pResult)
	{
		pResult->m_nResultCode		= ReviewResult_Snap_Complete;
		pResult->m_nGlassPosX		= nGlassPosX;
		pResult->m_nGlassPosY		= nGlassPosY;
		pResult->m_nMotorPosX		= nMotorPosX;
		pResult->m_nMotorPosY		= nMotorPosY;
		pResult->m_fDurationTime	= fDurationTime;
	//	pResult->vectorImageData	= vectorImageData;

	}

	LeaveCriticalSection(&m_csProcess);

	return pResult;
}

CDefectData_Review* CGlassResult::SetUserResultData(int nModuleIdx, int nResultIdx, VectorImageData& vectorImageData, int nGlassPosX, int nGlassPosY, int nMotorPosX, int nMotorPosY, float fDurationTime)
{
	EnterCriticalSection(&m_csProcess);

	CDefectData_Review *pResult = GetUserResult(nModuleIdx, nResultIdx);

	if (pResult)
	{
		pResult->m_nResultCode		= ReviewResult_Snap_Complete;
		pResult->m_nGlassPosX		= nGlassPosX;
		pResult->m_nGlassPosY		= nGlassPosY;
		pResult->m_nMotorPosX		= nMotorPosX;
		pResult->m_nMotorPosY		= nMotorPosY;
		pResult->m_fDurationTime	= fDurationTime;
		//	pResult->vectorImageData	= vectorImageData;

	}

	LeaveCriticalSection(&m_csProcess);

	return pResult;
}

CDefectData_Review* CGlassResult::SetMeasureResultData(int nModuleIdx, int nResultIdx, VectorImageData& vectorImageData, int nGlassPosX, int nGlassPosY, int nMotorPosX, int nMotorPosY, float fDurationTime)
{
	EnterCriticalSection(&m_csProcess);

	CDefectData_Review *pResult = GetMeasureResult(nModuleIdx, nResultIdx);

	if (pResult)
	{
		pResult->m_nResultCode		= ReviewResult_Snap_Complete;
		pResult->m_nGlassPosX		= nGlassPosX;
		pResult->m_nGlassPosY		= nGlassPosY;
		pResult->m_nMotorPosX		= nMotorPosX;
		pResult->m_nMotorPosY		= nMotorPosY;
		pResult->m_fDurationTime	= fDurationTime;
		//	pResult->vectorImageData	= vectorImageData;

	}

	LeaveCriticalSection(&m_csProcess);

	return pResult;
}

CDefectData_Review* CGlassResult::SetWsiResultData(int nModuleIdx, int nResultIdx, const CDefectData_Review& wsiResult, int nGlassPosX, int nGlassPosY, int nMotorPosX, int nMotorPosY, float fDurationTime)
{
	EnterCriticalSection(&m_csProcess);

	CDefectData_Review *pResult = GetMeasureResult(nModuleIdx, nResultIdx);

	if (pResult)
	{
		*pResult = wsiResult;
	}

	LeaveCriticalSection(&m_csProcess);

	return pResult;
}