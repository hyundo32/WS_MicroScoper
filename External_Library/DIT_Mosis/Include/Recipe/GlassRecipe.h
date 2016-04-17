// CFGlassRecipe.h: interface for the CGlassRecipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFGLASSRECIPE_H__6BC9EE1D_9AE2_4A47_8520_E6FA1A7869DC__INCLUDED_)
#define AFX_CFGLASSRECIPE_H__6BC9EE1D_9AE2_4A47_8520_E6FA1A7869DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CFRecipeJudge.h"
#include "PADRecipe.h"
#include "GlassRecipeHeader.h"


class AFX_EXT_CLASS CModelRecipe
{
public:
	CModelRecipe();
	virtual ~CModelRecipe();
	CModelRecipe(CModelRecipe& rhs);
	CModelRecipe& operator=(CModelRecipe& rhs);

	void				ResetCFRecipeModel();
	BOOL				ReadRecipeFile(int nIdx, CHMRegiConfig* pFile, CString strRecipeName);
	BOOL				WriteRecipeFile(int nIdx, CHMRegiConfig* pFile, CString strRecipeName);
	BOOL				ReadRecipe(CMosisPacket *pPacket);
	BOOL				WriteRecipe(CMosisPacket *pPacket);
	
	int					GetCellStart(int iCell){return m_pXCellStart==NULL?0:m_pXCellStart[iCell];}

	CMODEL_RECIPE_PARM	*GetModelRcpPrm(){return &m_pModelRcpParm;}
	void				SetModelRcpPrm(CMODEL_RECIPE_PARM &pPrm){m_pModelRcpParm=pPrm;}

	CMODEL_COORDI_PARM	*GetCoordiRcpPrm(){return &m_pModelRcpParm.m_pCoordiParm;}
	void				SetCoordiRcpPrm(CMODEL_COORDI_PARM &pPrm){m_pModelRcpParm.m_pCoordiParm=pPrm;}

	CMODEL_INSPECT_PARM	*GetInsRcpPrm(CameraType Camtype){return Camtype==CamType_Reflex?&m_pModelRcpParm.m_pRefInsParm:&m_pModelRcpParm.m_pTransInsParm;}
	void				SetInsRcpPrm(CMODEL_INSPECT_PARM &pPrm,CameraType Camtype){Camtype==CamType_Reflex?m_pModelRcpParm.m_pRefInsParm=pPrm:m_pModelRcpParm.m_pTransInsParm=pPrm;}

	CMODEL_FINDPATTERN_PARM	*GetFindEdgeRcpPrm(CameraType Camtype){return Camtype==CamType_Reflex?&m_pModelRcpParm.m_pRefFindParm:&m_pModelRcpParm.m_pTransFindParm;}
	void				SetFindEdgeRcpPrm(CMODEL_FINDPATTERN_PARM &pPrm,CameraType Camtype){Camtype==CamType_Reflex?m_pModelRcpParm.m_pRefFindParm=pPrm:m_pModelRcpParm.m_pTransFindParm=pPrm;}

	//////////////////////////////////////////////////////////////////////////
	// Cell 수와 Cell간 거리.
	int					GetXCellNum()			{ return GetCoordiRcpPrm()->m_CoordInfoX.m_nCellNum; }
	int					GetXEdge2Pat()			{ return GetCoordiRcpPrm()->m_CoordInfoX.m_nEdgetoPattern; }	
	int					GetXPattern()			{ return GetCoordiRcpPrm()->m_CoordInfoX.m_nPattern; }	
	int					GetXPat2Edge()			{ return GetCoordiRcpPrm()->m_CoordInfoX.m_nPatterntoEdge; }	
	int					GetXPat2Pat(int nIdx);
	int					GetXPat2Pat(ScanDirectionIns SDI,int nIdx);
	int					GetYCellNum()			{ return GetCoordiRcpPrm()->m_CoordInfoY.m_nCellNum; }
	int					GetYEdge2Pat()			{ return GetCoordiRcpPrm()->m_CoordInfoY.m_nEdgetoPattern; }
	int					GetYPat2Pat(int nIdx);
	int					GetYPat2Pat(ScanDirectionIns SDI,int nIdx);
	int					GetYPattern()			{ return GetCoordiRcpPrm()->m_CoordInfoY.m_nPattern; }
	int					GetYPat2Edge()			{ return GetCoordiRcpPrm()->m_CoordInfoY.m_nPatterntoEdge; }	

	void				SetXCellNum();
	void				SetXCellNum(int nCount);
	void				SetXPat2Pat(int nIdx, int nValue);
	void				SetYCellNum();
	void				SetYCellNum(int nCount);
	void				SetYPat2Pat(int nIdx, int nValue);

	//////////////////////////////////////////////////////////////////////////
	// 카메라별 값.
	int					GetCamBMThreshold(int nIdx,CameraType CamType);
	int					GetCamPatThreshold(int nIdx,CameraType CamType);
	int					GetCamSkipSize(int nIdx,CameraType CamType);
	
	void				SetCamNum(int nCount,CameraType CamType);
	void				SetCamBMThreshold(int nIdx, int nValue,CameraType CamType);
	void				SetCamPatThreshold(int nIdx, int nValue,CameraType CamType);
	void				SetCamSkipSize(int nIdx, int nValue,CameraType CamType);
	
	DefectLocation		GetDefectLoc()					{ return GetModelRcpPrm()->m_DefectLoc; }
	void				SetDefectLoc(int nValue)		{ GetModelRcpPrm()->m_DefectLoc = static_cast<DefectLocation>(nValue); }
	BOOL				GetReportResult()				{ return GetModelRcpPrm()->m_bReportResult; }
	void				SetReportResult(BOOL bReport)	{ GetModelRcpPrm()->m_bReportResult = bReport; }

protected:
	CMODEL_RECIPE_PARM	m_pModelRcpParm;		

// minion't,2009-10-29		
public:
	void SetJudgeRecipe(CCFRecipeJudge *pJudgeRecipe) { m_pJudgeCondition  = pJudgeRecipe; }
	const CCFRecipeJudge *GetJudgeRecipe(void) { return m_pJudgeCondition;}

	void SetModelID(int iID) { m_iModelID = iID; }

protected:
	CCFRecipeJudge		*m_pJudgeCondition;		// Pointer만 갖고 있자.
	int					m_iModelID;

	int*				m_pXCellStart;
};

//////////////////////////////////////////////////////////////////////////
// Glass Recipe
class AFX_EXT_CLASS CGlassRecipe  
{
public:
	CGlassRecipe();
	CGlassRecipe(int nCameraNum, int nScanNum);
	virtual ~CGlassRecipe();
	CGlassRecipe(CGlassRecipe& rhs);
	CGlassRecipe& operator=(CGlassRecipe& rhs);

	BOOL operator==(CGlassRecipe& rhs);
	BOOL operator!=(CGlassRecipe& rhs);
	
	void				SetCamScanInfo(int nTransCam,int nRefCam, int nScanNum);
	void				SetCamNScanNum(int nTransCam,int nRefCam, int nScanNum);	
	void				ResetCFGlassRecipe();

	//////////////////////////////////////////////////////////////////////////
	// 읽고 쓰기.
	BOOL				IsRecipeFile(CString strRecipe);
	BOOL				MakeRecipeFileName(CString strRecipe);
	BOOL				MakeRecipeDelegateFile(CString strRecipe);
	BOOL				ReadRecipeTitle();
	BOOL				ReadRecipeFile();
	BOOL				WriteRecipeFile(BOOL bCheck = TRUE);
	BOOL				ReadRecipePacket(CMosisPacket *pPacket);
	BOOL				WriteRecipePacket(CMosisPacket *pPacket);

	BOOL				ReadJudgePixelDefect();
	BOOL				ReadJudgeZoneJudge();
	BOOL				ReadJudgeRScaleDefect();

	//////////////////////////////////////////////////////////////////////////
	// Recipe 정보에 접근
	CPADRecipe*			GetPADRecipe(){return &m_pPADRecipe;}
	CPADRecipeModel		*GetPADRecipeModel(int iModel){return GetPADRecipe()->GetPADModel(iModel);}
	pPADLightSetting	GetPADLightParm(int iLight){return GetPADRecipe()->GetLightSetting(iLight);}	

	CGLASS_RECIPE_PARM	*GetGlassRcpPrm(){return &m_pGlassRcpParm;}
	void				SetGlassRcpPrm(CGLASS_RECIPE_PARM &pPrm){m_pGlassRcpParm=pPrm;}

	CRECIPE_INFO_PARM	*GetRcpInfoPrm(){return &GetGlassRcpPrm()->m_pRcpInfoPrm;}
	void				SetRcpInfoPrm(CRECIPE_INFO_PARM &pPrm){GetGlassRcpPrm()->m_pRcpInfoPrm=pPrm;}

	CCAMERA_INSPECT_PARM *GetInspectPrm(CameraType Camtype){return Camtype==CamType_Reflex?&GetGlassRcpPrm()->m_pRefInspectPrm:&GetGlassRcpPrm()->m_pTransInspectPrm;}
	void				SetInspectPrm(CameraType Camtype,CCAMERA_INSPECT_PARM &pPrm){Camtype==CamType_Reflex?GetGlassRcpPrm()->m_pRefInspectPrm=pPrm:GetGlassRcpPrm()->m_pTransInspectPrm=pPrm;}

	CEDGECRACK_RECIPE_PARM	*GetEdgeCrackPrm(){return &GetGlassRcpPrm()->m_pEdgeCrack;}
	void				SetEdgeCrackPrm(CEDGECRACK_RECIPE_PARM &pPrm){GetGlassRcpPrm()->m_pEdgeCrack=pPrm;}

	CLIGHT_RECIPE_PARM	*GetLightPrm(CameraType Camtype){return Camtype==CamType_Reflex?&GetGlassRcpPrm()->m_pRefLightParm:&GetGlassRcpPrm()->m_pTransLightParm;}
	void				SetLightPrm(CameraType Camtype,CLIGHT_RECIPE_PARM &pPrm){Camtype==CamType_Reflex?GetGlassRcpPrm()->m_pRefLightParm=pPrm:GetGlassRcpPrm()->m_pTransLightParm=pPrm;}

	CGLASS_ALIGN_PARM	*GetAlignPrm(){return &GetGlassRcpPrm()->m_pGlassAlignParm;}
	void				SetAlignPrm(CGLASS_ALIGN_PARM &pPrm){GetGlassRcpPrm()->m_pGlassAlignParm=pPrm;}

	CGLASS_MASKING_PARM	*GetMaskingPrm(){return &GetGlassRcpPrm()->m_pGlassMaskParm;}
	void				SetMaskingPrm(CGLASS_MASKING_PARM &pPrm){GetGlassRcpPrm()->m_pGlassMaskParm=pPrm;}

	CGLASS_DEFECT_JUDG_PARM	*GetDefectJudePrm(){return &GetGlassRcpPrm()->m_pDefectJudge;}
	void				SetDefectJudePrm(CGLASS_DEFECT_JUDG_PARM &pPrm){GetGlassRcpPrm()->m_pDefectJudge=pPrm;}

	CGLASS_STACK_PARM	*GetStackPrm(){return &GetGlassRcpPrm()->m_pStackParm;}
	void				SetStackPrm(CGLASS_STACK_PARM &pPrm){GetGlassRcpPrm()->m_pStackParm=pPrm;}

	CGLASS_RECIPE_ETC_PARM	*GetEtcPrm(){return &GetGlassRcpPrm()->m_pGlassEtcParm;}
	void				SetEtcPrm(CGLASS_RECIPE_ETC_PARM &pPrm){GetGlassRcpPrm()->m_pGlassEtcParm=pPrm;}

	int					GetModelCount()						{ return m_nModelCount; }	
	CModelRecipe*		GetRecipeModel(int id){return m_pModel+id;}
	BOOL				ReadIsZoneData();	
	
	//////////////////////////////////////////////////////////////////////////
	// X - Y에 종속적인 가공된 정보 제공.
	int					GetUMCellXPatStart(int nModelIdx, int nCellXIdx);
	int					GetUMCellXPatEnd(int nModelIdx, int nCellXIdx);
	int					GetUMCellYPatStart(int nModelIdx, int nCellYIdx);
	int					GetUMCellYPatEnd(int nModelIdx, int nCellYIdx);

	//////////////////////////////////////////////////////////////////////////
	// Utility Function
	int					GetPreviousCellNumber(int iPreviousModel);				// minion't,2009-10-29. purpose : preventing Cell ID duplication.
	int					GetModelIndexByCellID(int iCellID);				     	// minion't,2009-10-29
	const CCFRecipeJudge		*GetJudgeRecipeyByCellID(int iCellID);					// minion't,2009-10-29
	int					GetCellIdx(int nModelIdx, int nXIdx, int nYIdx);
	int					GetCellIdxContinue(int nModelIdx, int nXIdx, int nYIdx);
	CString				MakeCellNum(int nCellNum, int nSubpanel2nd = 2);
	
	
protected:
	CPADRecipe					m_pPADRecipe;			// PAD Recipe
	CGLASS_RECIPE_PARM			m_pGlassRcpParm;		// Glass Recipe Parm	

	//////////////////////////////////////////////////////////////////////////
	// 읽고 쓰기
	CHMRegiConfig				m_RecipeFile;

	
	//////////////////////////////////////////////////////////////////////////
	// Model
	int							m_nModelCount;	
	CModelRecipe*				m_pModel;

	void						SetModelCount(int nCount);			
};

#endif // !defined(AFX_CFGLASSRECIPE_H__6BC9EE1D_9AE2_4A47_8520_E6FA1A7869DC__INCLUDED_)

