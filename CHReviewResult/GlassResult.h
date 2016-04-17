#pragma once

#include <map>
#include <vector>
#include "ModuleResult.h"

class CHImageControls;
typedef std::vector<CHImageControls*>							VectorImageData;
typedef std::vector<CHImageControls*>::iterator					VectorImageDataIt;
typedef std::vector<CHImageControls*>::const_iterator			constVectorImageData;

typedef std::vector<CString>									VectorString;
typedef std::vector<CString>::iterator							VectorStringIt;
typedef std::vector<CString>::const_iterator					constVectorStringIt;

typedef std::vector<int>										VectorInteger;
typedef std::vector<int>::iterator								VectorIntegerIt;
typedef std::vector<int>::const_iterator						constVectorIntegerIt;

typedef std::vector<DWORD>										VectorDWORD;
typedef std::vector<DWORD>::iterator							VectorDWORDIt;
typedef std::vector<DWORD>::const_iterator						constVectorDWORDIt;

typedef std::vector<CCellData_Review>							VectorCellResult;
typedef std::vector<CCellData_Review>::iterator					VectorCellResultIt;
typedef std::vector<CCellData_Review>::const_iterator			constVectorCellResultIt;

typedef std::multimap<int, CDefectData_Review*>					MapDefectResult;
typedef std::multimap<int, CDefectData_Review*>::iterator		MapDefectResultIt;
typedef std::multimap<int, CDefectData_Review*>::const_iterator	constMapDefectResultIt;

typedef std::vector<CModuleResult>								VectorModuleResult;
typedef std::vector<CModuleResult>::iterator					VectorModuleResultIt;
typedef std::vector<CModuleResult>::const_iterator				constVectorModuleResultIt;

class AFX_EXT_CLASS CGlassResult
{
public:
	CGlassResult(void);
	~CGlassResult(void);
	void Reset();

	// glass result
	CGlassData_Review*				GetGlassResult();
	const CGlassData_Review*		GetGlassResult() const;

	// recipe result
	CRecipeData_Review*				GetRecipeResult();
	const CRecipeData_Review*		GetRecipeResult() const;

	// align result
	CAlignData_Review*				GetAlignResult();
	const CAlignData_Review*		GetAlignResult() const;

	// cell result
	int								GetCellResultCount() const;
	CCellData_Review*				GetCellResult(int nCellIdx);
	const CCellData_Review*			GetCellResult(int nCellIdx) const;
	void							AddCellResult(const CCellData_Review& cellResult);

	// defect result
	int								GetDefectResultCount() const;
	CDefectData_Review*				GetDefectResult(int nDefectIdx);
	const CDefectData_Review*		GetDefectResult(int nDefectIdx) const;
	BOOL							AddDefectResult(CDefectData_Review* pDefectResult);

	// review result
	int								GetReviewModuleResultCount() const;
	CModuleResult*					GetReviewModuleResult(int nModuleIdx);
	const CModuleResult*			GetReviewModuleResult(int nModuleIdx) const;
	void							AddReviewModuleResult(const CModuleResult& moduleResult);

	int								GetTotalReviewResultCount() const;
	int								GetReviewResultCount(int nModuleIdx) const;
	CDefectData_Review*				GetReviewResult(int nModuleIdx, int nResultIdx);
	const CDefectData_Review*		GetReviewResult(int nModuleIdx, int nResultIdx) const;
	void							AddReviewResult(int nModuleIdx, const CDefectData_Review& result);

	// user result
	int								GetUserModuleResultCount() const;
	CModuleResult*					GetUserModuleResult(int nModuleIdx);
	const CModuleResult*			GetUserModuleResult(int nModuleIdx) const;
	void							AddUserModuleResult(const CModuleResult& moduleResult);

	int								GetTotalUserResultCount() const;
	int								GetUserResultCount(int nModuleIdx) const;
	CDefectData_Review*				GetUserResult(int nModuleIdx, int nResultIdx);
	const CDefectData_Review*		GetUserResult(int nModuleIdx, int nResultIdx) const;
	void							AddUserResult(int nModuleIdx, const CDefectData_Review& result);

	// wsi result
	int								GetWsiModuleResultCount() const;
	CModuleResult*					GetWsiModuleResult(int nModuleIdx);
	const CModuleResult*			GetWsiModuleResult(int nModuleIdx) const;
	void							AddWsiModuleResult(const CModuleResult& moduleResult);

	int								GetTotalWsiResultCount() const;
	int								GetWsiResultCount(int nModuleIdx) const;
	CDefectData_Review*				GetWsiResult(int nModuleIdx, int nResultIdx);
	const CDefectData_Review*		GetWsiResult(int nModuleIdx, int nResultIdx) const;
	void							AddWsiResult(int nModuleIdx, const CDefectData_Review& result);

	// measure result
	int								GetMeasureModuleResultCount() const;
	CModuleResult*					GetMeasureModuleResult(int nModuleIdx);
	const CModuleResult*			GetMeasureModuleResult(int nModuleIdx) const;
	void							AddMeasureModuleResult(const CModuleResult& moduleResult);

	int								GetTotalMeasureResultCount() const;
	int								GetMeasureResultCount(int nModuleIdx) const;
	CDefectData_Review*				GetMeasureResult(int nModuleIdx, int nResultIdx);
	const CDefectData_Review*		GetMeasureResult(int nModuleIdx, int nResultIdx) const;
	void							AddMeasureResult(int nModuleIdx, const CDefectData_Review& result);

	// func
	BOOL					CheckReviewComplete(int nModuleIdx=-1);
	BOOL					CheckUserComplete(int nModuleIdx=-1);
	BOOL					CheckWsiComplete(int nModuleIdx=-1);
	BOOL					CheckMeasureComplete(int nModuleIdx=-1);

	// add result
	CDefectData_Review*		SetReviewResultData(int nModuleIdx, int nResultIdx, VectorImageData& vectorImageData, int nGlassPosX, int nGlassPosY, int nMotorPosX, int nMotorPosY, float fDurationTime);
	CDefectData_Review*		SetUserResultData(int nModuleIdx, int nResultIdx, VectorImageData& vectorImageData, int nGlassPosX, int nGlassPosY, int nMotorPosX, int nMotorPosY, float fDurationTime);
	CDefectData_Review*		SetMeasureResultData(int nModuleIdx, int nResultIdx, VectorImageData& vectorImageData, int nGlassPosX, int nGlassPosY, int nMotorPosX, int nMotorPosY, float fDurationTime);
	CDefectData_Review*		SetWsiResultData(int nModuleIdx, int nResultIdx, const CDefectData_Review& wsiResult, int nGlassPosX, int nGlassPosY, int nMotorPosX, int nMotorPosY, float fDurationTime);
	
protected:

	// current defect index
	int							m_nCurrentDefectIndex;

	// glass result
	CGlassData_Review			m_GlassResult;

	// recipe result
	CRecipeData_Review			m_RecipeResult;

	// align result
	CAlignData_Review			m_AlignResult;

	// cell result
	VectorCellResult			m_vecCellResult;

	// defect result
	MapDefectResult				m_mapDefectResult;

	// module result
	VectorModuleResult			m_vecModuleResult_Review;
	VectorModuleResult			m_vecModuleResult_User;
	VectorModuleResult			m_vecModuleResult_Wsi;
	VectorModuleResult			m_vecModuleResult_Measure;

	// critical section
	CRITICAL_SECTION			m_csProcess;
	CRITICAL_SECTION			m_csDefect;
};

