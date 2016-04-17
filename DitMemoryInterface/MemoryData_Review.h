#pragma once

#include "MemoryInterface.h"

namespace MemoryInterface_Review
{
	enum ReviewType			{ ReviewType_Review = 0, ReviewType_User, ReviewType_Wsi, ReviewType_Measure };
	enum ReviewResultCode	{ ReviewResult_None = 0, ReviewResult_TriggerMissing=1, ReviewResult_Snap_Complete=3, ReviewResult_Processing=7, ReviewResult_Process_Fail=15, ReviewResult_Process_Complete=31 };
	enum WsiResultCode		{ WsiResult_None = 0, WsiResult_Fail=1, WsiResultSuccess=2 };
	enum MeasureResultCode	{ MeasureResult_None = 0, MeasureResult_Fail=1, MeasureResult_Success=2 };
	enum WsiResultInfo		{	Wsi_X_Height=0, 
								Wsi_X_Width, 
								Wsi_X_Area,
								Wsi_X_A_Ratio, 
								Wsi_X_LEFT_1_A_Ratio, 
								Wsi_X_LEFT_2_A_Ratio, 
								Wsi_X_LEFT_3_A_Ratio, 
								Wsi_X_LEFT_4_A_Ratio, 
								Wsi_X_Right_1_A_Ratio, 
								Wsi_X_Right_2_A_Ratio, 
								Wsi_X_Right_3_A_Ratio, 
								Wsi_X_Right_4_A_Ratio,
								Wsi_Result_Info_Count };

	enum StringSize			{	StringSize_GlassID=30, 
								StringSize_RecipeID=20, 
								StringSize_CellID=20, 
								StringSize_DefectInfo=20, 
								StringSize_DefectCode=20, 
								StringSize_DefectZone=20, 
								StringSize_StackStep=50,
								StringSize_ImageFilename=50
							};

	enum ReviewTypeCount	{ AlignCameraCount=2, ImageFileCount=2, ImageDataCount=5, FindResultCount=10, MeasureResultCount=10, WsiResultCount=10 };

	enum ReviewImageInfo	{ ReviewImageSize=2456*2058*4 };
};

using namespace MemoryInterface_Review;

class CMemoryData_Review : public CMemoryData
{
public:
	CMemoryData_Review()	
	{ 
		memset(this, 0, sizeof(CMemoryData_Review)); 
	}
	int		m_nGlassCount;
	int 	m_nGlassIndex;
	int		m_nImageCount;
	int		m_nImageIndex;
};

class CGlassData_Review
{
public:
	CGlassData_Review()		
	{ 
		memset(this, 0, sizeof(CGlassData_Review)); 
	}
	TCHAR	m_strGlassID[StringSize_GlassID];
	TCHAR	m_strRecipeID[StringSize_RecipeID];
	int		m_nCellCount;
	int		m_nResultCount;
	int		m_nCellIndex;
	int		m_nDefectIndex;
};

class CRecipeData_Review
{
public:
	CRecipeData_Review()	
	{ 
		memset(this, 0, sizeof(CRecipeData_Review)); 
	}
	TCHAR	m_strRecipeID[StringSize_RecipeID];
	int		m_nCellCount;
	int		m_nResultCount;
	int		m_nCellIndex;
	int		m_nDefectIndex;
};

class CAlignData_Review
{
public:
	CAlignData_Review()
	{ 
		memset(this, 0, sizeof(CAlignData_Review)); 
	}
	int		m_nFindCount;
	int		m_nResultCode;
	int		m_nFindAlign[AlignCameraCount];
	int		m_nControlLightLevel[AlignCameraCount];

	float	m_fFindPixelX[AlignCameraCount];			// pixel
	float	m_fFindPixelY[AlignCameraCount];			// pixel
	float	m_fFindScore[AlignCameraCount];			// score (0~1)

	float	m_fFindPositionX[AlignCameraCount];		// find motor pos mm
	float	m_fFindPositionY[AlignCameraCount];		// find motor pos mm

	int		m_nAutoLightResult[AlignCameraCount];
	float	m_fLastLightLevel[AlignCameraCount];
};

class CCellData_Review
{
public:
	CCellData_Review()		
	{ 
		memset(this, 0, sizeof(CCellData_Review)); 
	}
	int			m_nCellIndex;
	int			m_nModelIndex;
	int			m_nUMOriginX;
	int			m_nUMOriginY;
	int			m_nUMOriginWidth;
	int			m_nUMOriginHeight;
	int			m_nDefectCount;
	TCHAR		m_strCellID[StringSize_CellID];
};

class CDefectData_Review
{
public:
	CDefectData_Review()		
	{ 
		memset(this, 0, sizeof(CDefectData_Review)); 
	}
	// AOI Defect Data
	int			m_nCameraIndex;
	int 		m_nScanIndex;
	int 		m_nCellIndex;
	int 		m_nFrameIndex;
	int			m_nFramePixelX;
	int			m_nFramePixelY;
	int			m_nOriginPosX;
	int 		m_nOriginPosY;
	float		m_fOpticResolution;
	int			m_nDefectIndex;
	int			m_nDefectIndexCD;
	int			m_nDefectIndexSD;
	int			m_nDefectSize;
	int			m_nDefectSizeS;
	int			m_nDefectSizeW;
	int			m_nDefectSizeH;
	int			m_nDefectPeak;
	int			m_nDefectPeakMin;
	int			m_nDefectPeakMax;
	int			m_nDefectGray;
	int			m_nDefectGrayMin;
	int			m_nDefectGrayMax;
	int			m_nOpticType;
	int			m_nGrayType;
	int 		m_nSizeType;
	int			m_nStackType;
	int			m_nLocationType;
	int			m_nJudgeType;
	int			m_nSubType;
	int			m_nStackStepCount;
	int			m_nSortValue;

	TCHAR		m_strStackStep[StringSize_StackStep];
	TCHAR		m_strDefectInfo[StringSize_DefectInfo];
	TCHAR		m_strDefectCode[StringSize_DefectCode];
	TCHAR		m_strDefectZone[StringSize_DefectZone];
	
	// Review Result Data
	int			m_nReviewType;
	int			m_nResultCode;
	int			m_nModuleIndex;
	int			m_nResultIndex;
	int			m_nMotorPosX;
	int			m_nMotorPosY;
	int			m_nGlassPosX;
	int			m_nGlassPosY;
	int			m_nLightLevel;
	float		m_fDurationTime;	

	int			m_nMagnificIndex;
	float		m_fManification;
	float		m_fManificResoultion;
	TCHAR		m_strImageFilename[ImageFileCount][StringSize_ImageFilename];
	LPVOID		m_pImageData[ImageDataCount];

	// Find Result Data
	int			m_nFind_ResultCode;
	int			m_nFind_RecipeIndex;
	float		m_fFind_ResultData[FindResultCount];

	// Measure Result Data
	int			m_nMeasure_ResultCode;
	int			m_nMeasure_RecipeIndex;
	float		m_fMesure_ResultData[MeasureResultCount];

	// Wsi Result Data
	int			m_nWsi_ResultCode;
	int			m_nWsi_RecipeIndex;
	float		m_fWsi_ResultData[WsiResultCount];
};

class CImageData_Review
{
public:
	CImageData_Review()		
	{
		memset(this, 0, sizeof(CImageData_Review)); 
	}
	int		m_nDefectIndex;
	int		m_nImageWidth;
	int		m_nImageHeight;
	int		m_nImageSteps;
	int		m_nImageChannels;
	BYTE	m_pImageData[ReviewImageSize];
};