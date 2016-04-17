#if !defined(__MODEL_RECIPE_HEADER_INCLUDED__)
#define __MODEL_RECIPE_HEADER_INCLUDED__

#include "HMRegiConfig.h"
#include "MosisPacket.h"

typedef struct _CAMERARECIPEINFO
{
	int			m_nBMThreshold;		// BM 검사 문턱값
	int			m_nPatThreshold;	// Pattern 검사 문턱값
	int			m_nSkipSize;		// 위/아래에서 검사를 하지 않는 영역.

	_CAMERARECIPEINFO()
	{
		Reset();
	}
	void Reset()
	{
		m_nBMThreshold		= 0;
		m_nPatThreshold		= 0;
		m_nSkipSize		= 0;
	}	
	_CAMERARECIPEINFO& operator=(const _CAMERARECIPEINFO& rhs)
	{
		if (this != &rhs)
		{
			m_nBMThreshold		= rhs.m_nBMThreshold;
			m_nPatThreshold		= rhs.m_nPatThreshold;
			m_nSkipSize			= rhs.m_nSkipSize;
		}
		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile,int nModelIdx,int nCamIdx,CameraType Camtype)
	{
		CString		strSection,strTmp;

		switch(Camtype)
		{
		case CamType_Transfer: strTmp="T"; break;
		case CamType_Reflex: strTmp="R"; break;
		}
		
		strSection.Format("MODEL_%sCAMBMTHRES_%d",strTmp,nCamIdx);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nBMThreshold);
		strSection.Format("MODEL_%sCAMPATTHRES_%d",strTmp,nCamIdx);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nPatThreshold);
		strSection.Format("MODEL_%sCAMSKIPSIZE_%d",strTmp,nCamIdx);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nSkipSize);
		return TRUE;
	}		
	BOOL WriteRecipe(CHMRegiConfig *pFile,int nModelIdx,int nCamIdx,CameraType Camtype)
	{
		CString		strSection,strTmp;

		switch(Camtype)
		{
		case CamType_Transfer: strTmp="T"; break;
		case CamType_Reflex: strTmp="R"; break;
		}

		strSection.Format("MODEL_%sCAMBMTHRES_%d",strTmp,nCamIdx);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nBMThreshold);
		strSection.Format("MODEL_%sCAMPATTHRES_%d",strTmp,nCamIdx);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nPatThreshold);
		strSection.Format("MODEL_%sCAMSKIPSIZE_%d",strTmp,nCamIdx);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nSkipSize);
		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{
		pPacket->ReadValue(m_nBMThreshold);
		pPacket->ReadValue(m_nPatThreshold);
		pPacket->ReadValue(m_nSkipSize);
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		pPacket->WriteValue(m_nBMThreshold);
		pPacket->WriteValue(m_nPatThreshold);
		pPacket->WriteValue(m_nSkipSize);
		return TRUE;
	}
} CameraRecipeInfo, *pCameraRecipeInfo;

//////////////////////////////////////////////////////////////////////////
// 모델 정보 중 좌표에 종속적인 정보들.
typedef struct _MODELCOORDINFO
{
	// Glass 셀 수 및 Size
	int 				m_nEdgetoPattern;	
	int 				m_nPattern;
	int 				m_nPatterntoEdge;

	int					m_nCellNum;
	int					*m_pCellPat2Pat;

	_MODELCOORDINFO()
	{
		m_pCellPat2Pat = NULL;
		Reset();
	}
	~_MODELCOORDINFO()
	{
		DeleteBuffer();
	}
	void Reset()
	{
		m_nEdgetoPattern		= 0;		
		m_nPattern				= 0;
		m_nPatterntoEdge		= 0;		
		m_nCellNum				= 0;
		DeleteBuffer();
	}
	void DeleteBuffer()
	{
		if(m_pCellPat2Pat != NULL)
			delete[] m_pCellPat2Pat, m_pCellPat2Pat=NULL;
		m_nCellNum = 0;
	}
	void SetPat2PatNum(int nCount)
	{
		DeleteBuffer();
		m_nCellNum = nCount;
		if(nCount-1 <= 0)
		{
			m_nCellNum = 0;
			return;
		}
		m_pCellPat2Pat = new int[nCount-1];
		ZeroMemory(m_pCellPat2Pat,sizeof(int)*(nCount-1));
	}	
	_MODELCOORDINFO& operator=(const _MODELCOORDINFO& rhs)
	{
		if (this != &rhs)
		{
			m_nEdgetoPattern	= rhs.m_nEdgetoPattern;			
			m_nPattern			= rhs.m_nPattern;
			m_nPatterntoEdge	= rhs.m_nPatterntoEdge;			
			SetPat2PatNum(rhs.m_nCellNum);	
			if(rhs.m_nCellNum-1 > 0 && m_pCellPat2Pat != NULL && rhs.m_pCellPat2Pat != NULL)
			{				
				int			i;
				for(i=0;i<rhs.m_nCellNum-1;i++)
				{
					m_pCellPat2Pat[i] = rhs.m_pCellPat2Pat[i];
				}
			}
		}
		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile,int nModelIdx,ScanCoordinate dir)
	{
		CString		strSection,strTmp;
		
		switch(dir)
		{
		case SC_XCoord: strTmp="X"; break;
		case SC_YCoord: strTmp="Y"; break;
		}

		strSection.Format("MODEL_%sEDGE2PATTERN",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nEdgetoPattern);
		strSection.Format("MODEL_%sPATTERN",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nPattern);
		strSection.Format("MODEL_%sPATTERN2EDGE",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nPatterntoEdge);		
		strSection.Format("MODEL_%sCELLNUM",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nCellNum);

		SetPat2PatNum(m_nCellNum);
		int			iLoop;
		for(iLoop=0;iLoop<m_nCellNum-1;iLoop++)
		{
			strSection.Format("MODEL_%sCELL2CELL_%d",strTmp,iLoop);
			pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_pCellPat2Pat[iLoop]);
		}		
		return TRUE;
	}	
	BOOL WriteRecipe(CHMRegiConfig *pFile,int nModelIdx,ScanCoordinate dir)
	{
		CString		strSection,strTmp;

		switch(dir)
		{
		case SC_XCoord: strTmp="X"; break;
		case SC_YCoord: strTmp="Y"; break;
		}

		strSection.Format("MODEL_%sEDGE2PATTERN",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nEdgetoPattern);
		strSection.Format("MODEL_%sPATTERN",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nPattern);
		strSection.Format("MODEL_%sPATTERN2EDGE",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nPatterntoEdge);		
		strSection.Format("MODEL_%sCELLNUM",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nCellNum);
		
		int			iLoop;
		for(iLoop=0;iLoop<m_nCellNum-1;iLoop++)
		{
			strSection.Format("MODEL_%sCELL2CELL_%d",strTmp,iLoop);
			pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_pCellPat2Pat[iLoop]);
		}
		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{
		pPacket->ReadValue(m_nEdgetoPattern);
		pPacket->ReadValue(m_nPattern);
		pPacket->ReadValue(m_nPatterntoEdge);
		pPacket->ReadValue(m_nCellNum);
		
		SetPat2PatNum(m_nCellNum);

		if(m_pCellPat2Pat != NULL)
		{
			int			iLoop;
			for(iLoop=0;iLoop<m_nCellNum-1;iLoop++)
				pPacket->ReadValue(m_pCellPat2Pat[iLoop]);
		}
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		pPacket->WriteValue(m_nEdgetoPattern);
		pPacket->WriteValue(m_nPattern);
		pPacket->WriteValue(m_nPatterntoEdge);
		pPacket->WriteValue(m_nCellNum);

		if(m_pCellPat2Pat != NULL && m_nCellNum-1 > 0)
		{
			int			iLoop;
			for(iLoop=0;iLoop<m_nCellNum-1;iLoop++)
				pPacket->WriteValue(m_pCellPat2Pat[iLoop]);
		}
		return TRUE;
	}
} ModelCoordInfo, *pModelCoordInfo;

class AFX_EXT_CLASS CMODEL_FINDPATTERN_PARM
{	
public:
	FindBoundary		m_FindBoundary;				// 0 - 안찾음, 1 - 매칭, 2 - 혼잡도, 3 - 밝기, 4 - Template Matching, // kys  5 - Pitch Matching 
	int					m_nThresholdBMtoPattern;	// BM - Pat 찾기 문턱값	
	double				m_dBoundaryOffset;			// 경계찾기 Offset - 상관계수가 이 값 이하인 지점에서 경계로 인정.
	
	CMODEL_FINDPATTERN_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_FindBoundary = FB_PitchMatching;
		m_nThresholdBMtoPattern = 0;
		m_dBoundaryOffset = 0.;
	}	
	CMODEL_FINDPATTERN_PARM& operator=(const CMODEL_FINDPATTERN_PARM &rh)
	{
		m_FindBoundary = rh.m_FindBoundary;
		m_nThresholdBMtoPattern = rh.m_nThresholdBMtoPattern;
		m_dBoundaryOffset = rh.m_dBoundaryOffset;
		return *this;
	}	
	BOOL ReadRecipe(CHMRegiConfig *pFile,int nModelIdx,CameraType Camtype)
	{
		CString		strSection,strTmp;
		int			nValue;

		switch(Camtype)
		{
		case CamType_Transfer: strTmp="T"; break;
		case CamType_Reflex: strTmp="R"; break;
		}
		
		strSection.Format("MODEL_FINDEDGEMETHOD_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, nValue);
		m_FindBoundary = (FindBoundary)nValue;
		strSection.Format("MODEL_THRESBM2PAT_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nThresholdBMtoPattern);
		strSection.Format("MODEL_BOUNDARYOFFSET_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_dBoundaryOffset);
		return TRUE;
	}	
	BOOL WriteRecipe(CHMRegiConfig *pFile,int nModelIdx,CameraType Camtype)
	{
		CString		strSection,strTmp;
		int			nValue;

		switch(Camtype)
		{
		case CamType_Transfer: strTmp="T"; break;
		case CamType_Reflex: strTmp="R"; break;
		}
		
		strSection.Format("MODEL_FINDEDGEMETHOD_%s",strTmp);
		nValue = (int)m_FindBoundary;
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, nValue);		
		strSection.Format("MODEL_THRESBM2PAT_%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nThresholdBMtoPattern);
		strSection.Format("MODEL_BOUNDARYOFFSET_%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_dBoundaryOffset);
		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{
		int			nValue;		
		pPacket->ReadValue(nValue);
		m_FindBoundary = (FindBoundary)nValue;
		pPacket->ReadValue(m_nThresholdBMtoPattern);
		pPacket->ReadValue(m_dBoundaryOffset);		
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		int			nValue;
		nValue = (int)m_FindBoundary;
		pPacket->WriteValue(nValue);
		pPacket->WriteValue(m_nThresholdBMtoPattern);
		pPacket->WriteValue(m_dBoundaryOffset);
		return TRUE;
	}
};

class AFX_EXT_CLASS CMODEL_INSPECT_PARM
{	
public:
	int					m_nCamNum;
	int					m_nPatternDummyX;	
	int					m_nPatternDummyY;
	int					m_nCamMainPatThres;
	int					m_nCamMainBMThres;
	int					m_nCamMainSkipSize;
	int					m_nInspMethodPatIdx;
	int					m_nInspMethodBMIdx;
	int					m_nAddThresLevel;
	int					m_nAddThreshold;
	int					m_nAddGradient;	
	pCameraRecipeInfo	m_pCameraRecipe;
	
	CMODEL_INSPECT_PARM()
	{
		m_pCameraRecipe = NULL;
		Reset();
	}
	~CMODEL_INSPECT_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_nCamNum = m_nPatternDummyX = m_nPatternDummyY = m_nCamMainPatThres = m_nCamMainBMThres = m_nCamMainSkipSize = 0;
		m_nInspMethodPatIdx = m_nInspMethodBMIdx = m_nAddThresLevel = m_nAddThreshold = m_nAddGradient = 0;
		if(m_pCameraRecipe != NULL)
			delete[] m_pCameraRecipe, m_pCameraRecipe=NULL;
	}	
	void SetCamNum(int nCount)
	{
		m_nCamNum = nCount;
		if(m_pCameraRecipe != NULL)
			delete[] m_pCameraRecipe, m_pCameraRecipe=NULL;
		if(nCount<=0)
			return;
		m_pCameraRecipe = new CameraRecipeInfo[nCount];
	}
	CMODEL_INSPECT_PARM& operator=(const CMODEL_INSPECT_PARM &rh)
	{
		m_nCamNum = rh.m_nCamNum;
		m_nPatternDummyX = rh.m_nPatternDummyX;
		m_nPatternDummyY = rh.m_nPatternDummyY;
		m_nCamMainPatThres = rh.m_nCamMainPatThres;
		m_nCamMainBMThres = rh.m_nCamMainBMThres;
		m_nCamMainSkipSize = rh.m_nCamMainSkipSize;
		m_nInspMethodPatIdx = rh.m_nInspMethodPatIdx;
		m_nInspMethodBMIdx = rh.m_nInspMethodBMIdx;
		m_nAddThresLevel = rh.m_nAddThresLevel;
		m_nAddThreshold = rh.m_nAddThreshold;
		m_nAddGradient = rh.m_nAddGradient;		
		SetCamNum(m_nCamNum);		
		if(rh.m_pCameraRecipe != NULL && m_nCamNum > 0)
		{
			int iLoop;
			for(iLoop=0;iLoop<m_nCamNum;iLoop++)
				m_pCameraRecipe[iLoop] = rh.m_pCameraRecipe[iLoop];
		}
		return *this;
	}	
	BOOL ReadRecipe(CHMRegiConfig *pFile,int nModelIdx,CameraType Camtype)
	{
		CString		strSection,strTmp;

		switch(Camtype)
		{
		case CamType_Transfer: strTmp="T"; break;
		case CamType_Reflex: strTmp="R"; break;
		}
		
		strSection.Format("MODEL_CAMMAINPATTHRES_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nCamMainPatThres, 0);
		strSection.Format("MODEL_CAMMPATDUMMYTB_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nPatternDummyX, 0);
		strSection.Format("MODEL_CAMMPATDUMMYLR_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nPatternDummyY, 0);
		strSection.Format("MODEL_CAMMAINBMTHRES_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nCamMainBMThres, 0);
		strSection.Format("MODEL_CAMMAINSKIPSIZE_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nCamMainSkipSize, 0);
		strSection.Format("MODEL_INSPMETHODIDXPAT%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nInspMethodPatIdx, 0);
		strSection.Format("MODEL_INSPMETHODIDXBM%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nInspMethodBMIdx, 0);
		strSection.Format("MODEL_ADDTHRESLEVEL_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nAddThresLevel, 0);
		strSection.Format("MODEL_ADDTHRESHOLD_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nAddThreshold, 0);
		strSection.Format("MODEL_ADDGRADIENT_%s",strTmp);
		pFile->GetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nAddGradient, 0);

		SetCamNum(m_nCamNum);

		if(m_pCameraRecipe != NULL && m_nCamNum > 0)
		{
			int iLoop;
			for(iLoop=0;iLoop<m_nCamNum;iLoop++)
				m_pCameraRecipe[iLoop].ReadRecipe(pFile,nModelIdx,iLoop,Camtype);
		}

		return TRUE;
	}	
	BOOL WriteRecipe(CHMRegiConfig *pFile,int nModelIdx,CameraType Camtype)
	{
		CString		strSection,strTmp;

		switch(Camtype)
		{
		case CamType_Transfer: strTmp="T"; break;
		case CamType_Reflex: strTmp="R"; break;
		}

		strSection.Format("MODEL_CAMMAINPATTHRES_%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nCamMainPatThres);
		strSection.Format("MODEL_CAMMPATDUMMYTB_%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nPatternDummyX);
		strSection.Format("MODEL_CAMMPATDUMMYLR_%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nPatternDummyY);
		strSection.Format("MODEL_CAMMAINBMTHRES_%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nCamMainBMThres);
		strSection.Format("MODEL_CAMMAINSKIPSIZE_%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nCamMainSkipSize);
		strSection.Format("MODEL_INSPMETHODIDXPAT%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nInspMethodPatIdx);
		strSection.Format("MODEL_INSPMETHODIDXBM%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nInspMethodBMIdx);
		strSection.Format("MODEL_ADDTHRESLEVEL_%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nAddThresLevel);
		strSection.Format("MODEL_ADDTHRESHOLD_%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nAddThreshold);
		strSection.Format("MODEL_ADDGRADIENT_%s",strTmp);
		pFile->SetItemValue(nModelIdx, (LPSTR)(LPCTSTR)strSection, m_nAddGradient);

		if(m_pCameraRecipe != NULL && m_nCamNum > 0)
		{
			int iLoop;
			for(iLoop=0;iLoop<m_nCamNum;iLoop++)
				m_pCameraRecipe[iLoop].WriteRecipe(pFile,nModelIdx,iLoop,Camtype);
		}

		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{		
		pPacket->ReadValue(m_nCamMainPatThres);	
		pPacket->ReadValue(m_nPatternDummyX);
		pPacket->ReadValue(m_nPatternDummyY);
		pPacket->ReadValue(m_nCamMainBMThres);		
		pPacket->ReadValue(m_nCamMainSkipSize);
		pPacket->ReadValue(m_nInspMethodPatIdx);
		pPacket->ReadValue(m_nInspMethodBMIdx);
		pPacket->ReadValue(m_nAddThresLevel);
		pPacket->ReadValue(m_nAddThreshold);
		pPacket->ReadValue(m_nAddGradient);
		
		SetCamNum(m_nCamNum);

		if(m_pCameraRecipe != NULL && m_nCamNum > 0)
		{
			int iLoop;
			for(iLoop=0;iLoop<m_nCamNum;iLoop++)
				m_pCameraRecipe[iLoop].ReadRecipe(pPacket);
		}
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		pPacket->WriteValue(m_nCamMainPatThres);	
		pPacket->WriteValue(m_nPatternDummyX);
		pPacket->WriteValue(m_nPatternDummyY);
		pPacket->WriteValue(m_nCamMainBMThres);		
		pPacket->WriteValue(m_nCamMainSkipSize);
		pPacket->WriteValue(m_nInspMethodPatIdx);
		pPacket->WriteValue(m_nInspMethodBMIdx);
		pPacket->WriteValue(m_nAddThresLevel);
		pPacket->WriteValue(m_nAddThreshold);
		pPacket->WriteValue(m_nAddGradient);

		if(m_pCameraRecipe != NULL && m_nCamNum > 0)
		{
			int iLoop;
			for(iLoop=0;iLoop<m_nCamNum;iLoop++)
				m_pCameraRecipe[iLoop].WriteRecipe(pPacket);
		}
		return TRUE;
	}
};

class AFX_EXT_CLASS CMODEL_COORDI_PARM
{
public:
	ModelCoordInfo		m_CoordInfoX;
	ModelCoordInfo		m_CoordInfoY;
	CellLineType		m_CellXLine;
	CellLineType		m_CellYLine;
	int					m_nCellXDir;			// Glass X축과 Cell X축이 같으면 1, 반대면 -1
	int					m_nCellYDir;			// Glass Y축과 Cell Y축이 같으면 1, 반대면 -1
	
	CMODEL_COORDI_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_CoordInfoX.Reset();
		m_CoordInfoY.Reset();
		m_CellXLine = CellLine_Gate;
		m_CellYLine = CellLine_Data;
		m_nCellXDir = m_nCellYDir = 0;
	}	
	CMODEL_COORDI_PARM& operator=(const CMODEL_COORDI_PARM &rh)
	{
		m_CoordInfoX = rh.m_CoordInfoX;
		m_CoordInfoY = rh.m_CoordInfoY;
		m_CellXLine = rh.m_CellXLine;
		m_CellYLine = rh.m_CellYLine;
		m_nCellXDir = rh.m_nCellXDir;
		m_nCellYDir = rh.m_nCellYDir;
		return *this;
	}	
	BOOL ReadRecipe(CHMRegiConfig *pFile,int nModelIdx)
	{
		m_CoordInfoX.ReadRecipe(pFile,nModelIdx,SC_XCoord);
		m_CoordInfoY.ReadRecipe(pFile,nModelIdx,SC_YCoord);
		pFile->GetItemValue(nModelIdx, "MODEL_CELLXDIR", m_nCellXDir, 1);
		pFile->GetItemValue(nModelIdx, "MODEL_CELLYDIR", m_nCellYDir, 1);
		
		return TRUE;
	}	
	BOOL WriteRecipe(CHMRegiConfig *pFile,int nModelIdx)
	{
		m_CoordInfoX.WriteRecipe(pFile,nModelIdx,SC_XCoord);
		m_CoordInfoY.WriteRecipe(pFile,nModelIdx,SC_YCoord);
		pFile->SetItemValue(nModelIdx, "MODEL_CELLXDIR", m_nCellXDir);
		pFile->SetItemValue(nModelIdx, "MODEL_CELLYDIR", m_nCellYDir);

		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{		
		m_CoordInfoX.ReadRecipe(pPacket);
		m_CoordInfoY.ReadRecipe(pPacket);
		pPacket->ReadValue(m_nCellXDir);	
		pPacket->ReadValue(m_nCellYDir);
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		m_CoordInfoX.WriteRecipe(pPacket);
		m_CoordInfoY.WriteRecipe(pPacket);
		pPacket->WriteValue(m_nCellXDir);	
		pPacket->WriteValue(m_nCellYDir);
		return TRUE;
	}
};

class AFX_EXT_CLASS CMODEL_RECIPE_PARM
{	
public:
	BOOL				m_bTCSNum;
	BOOL				m_bUseDPC;	
	BOOL				m_bReportResult;		// 결과를 보고할 것인지 확인.
	double				m_dCellPitchConv;
	double				m_dCellPitchScan;
	int					m_nPitchCycleConv;
	int					m_nPitchCycleScan;	
	DefectLocation		m_DefectLoc;	

	CMODEL_COORDI_PARM	m_pCoordiParm;
	CMODEL_INSPECT_PARM	m_pTransInsParm;
	CMODEL_INSPECT_PARM	m_pRefInsParm;
	CMODEL_FINDPATTERN_PARM	m_pTransFindParm;
	CMODEL_FINDPATTERN_PARM	m_pRefFindParm;


	CMODEL_RECIPE_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_bTCSNum = m_bUseDPC = m_bReportResult = FALSE;
		m_dCellPitchConv = m_dCellPitchScan = 0.;
		m_nPitchCycleConv = m_nPitchCycleScan = 0;
		m_DefectLoc = DefectLoc_Pattern;
		m_pCoordiParm.Reset();
		m_pTransInsParm.Reset();
		m_pRefInsParm.Reset();
		m_pTransFindParm.Reset();
		m_pRefFindParm.Reset();
	}	
	CMODEL_RECIPE_PARM& operator=(const CMODEL_RECIPE_PARM &rh)
	{
		m_bTCSNum = rh.m_bTCSNum;
		m_bUseDPC = rh.m_bUseDPC;
		m_bReportResult = rh.m_bReportResult;
		m_dCellPitchConv = rh.m_dCellPitchConv;
		m_dCellPitchScan = rh.m_dCellPitchScan;
		m_nPitchCycleConv = rh.m_nPitchCycleConv;
		m_nPitchCycleScan = rh.m_nPitchCycleScan;
		m_DefectLoc = rh.m_DefectLoc;
		m_pCoordiParm = rh.m_pCoordiParm;
		m_pTransInsParm = rh.m_pTransInsParm;
		m_pRefInsParm = rh.m_pRefInsParm;
		m_pTransFindParm = rh.m_pTransFindParm;
		m_pRefFindParm = rh.m_pRefFindParm;
		return *this;
	}	
	BOOL ReadRecipe(CHMRegiConfig *pFile,int nModelIdx)
	{
		if(pFile == NULL)
			return FALSE;

		int			nValue;
		
		pFile->GetItemValue(nModelIdx, "MODEL_TCSNUM", m_bTCSNum, FALSE);
		pFile->GetItemValue(nModelIdx, "MODEL_USEDPC", m_bUseDPC, FALSE);
		pFile->GetItemValue(nModelIdx, "MODEL_REPORTRESULT", m_bReportResult, FALSE);
		pFile->GetItemValue(nModelIdx, "MODEL_CELLPITCHCONV", m_dCellPitchConv, 0.);
		pFile->GetItemValue(nModelIdx, "MODEL_CELLPITCHSCAN", m_dCellPitchScan, 0.);
		pFile->GetItemValue(nModelIdx, "MODEL_PITCHCYCLECONV", m_nPitchCycleConv, 0);
		pFile->GetItemValue(nModelIdx, "MODEL_PITCHCYCLESCAN", m_nPitchCycleScan, 0);
		pFile->GetItemValue(nModelIdx, "MODEL_DEFECTLOC", nValue, FALSE);
		m_DefectLoc = (DefectLocation)nValue;
		
		m_pCoordiParm.ReadRecipe(pFile,nModelIdx);
		m_pTransInsParm.ReadRecipe(pFile,nModelIdx,CamType_Transfer);
		m_pRefInsParm.ReadRecipe(pFile,nModelIdx,CamType_Reflex);
		m_pTransFindParm.ReadRecipe(pFile,nModelIdx,CamType_Transfer);
		m_pRefFindParm.ReadRecipe(pFile,nModelIdx,CamType_Reflex);
		return TRUE;
	}	
	BOOL WriteRecipe(CHMRegiConfig *pFile,int nModelIdx)
	{
		if(pFile == NULL)
			return FALSE;

		int			nValue;

		pFile->SetItemValue(nModelIdx, "MODEL_TCSNUM", m_bTCSNum);
		pFile->SetItemValue(nModelIdx, "MODEL_USEDPC", m_bUseDPC);
		pFile->SetItemValue(nModelIdx, "MODEL_REPORTRESULT", m_bReportResult);
		pFile->SetItemValue(nModelIdx, "MODEL_CELLPITCHCONV", m_dCellPitchConv);
		pFile->SetItemValue(nModelIdx, "MODEL_CELLPITCHSCAN", m_dCellPitchScan);
		pFile->SetItemValue(nModelIdx, "MODEL_PITCHCYCLECONV", m_nPitchCycleConv);
		pFile->SetItemValue(nModelIdx, "MODEL_PITCHCYCLESCAN", m_nPitchCycleScan);
		nValue = (int)m_DefectLoc;
		pFile->SetItemValue(nModelIdx, "MODEL_DEFECTLOC", nValue);		

		m_pCoordiParm.WriteRecipe(pFile,nModelIdx);
		m_pTransInsParm.WriteRecipe(pFile,nModelIdx,CamType_Transfer);
		m_pRefInsParm.WriteRecipe(pFile,nModelIdx,CamType_Reflex);
		m_pTransFindParm.WriteRecipe(pFile,nModelIdx,CamType_Transfer);
		m_pRefFindParm.WriteRecipe(pFile,nModelIdx,CamType_Reflex);
		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{			
		int			nValue;
		pPacket->ReadValue(m_bTCSNum);	
		pPacket->ReadValue(m_bUseDPC);
		pPacket->ReadValue(m_bReportResult);
		pPacket->ReadValue(m_dCellPitchConv);
		pPacket->ReadValue(m_dCellPitchScan);
		pPacket->ReadValue(m_nPitchCycleConv);
		pPacket->ReadValue(m_nPitchCycleScan);
		pPacket->ReadValue(nValue);
		m_DefectLoc = (DefectLocation)nValue;
		
		m_pCoordiParm.ReadRecipe(pPacket);
		m_pTransInsParm.ReadRecipe(pPacket);
		m_pRefInsParm.ReadRecipe(pPacket);
		m_pTransFindParm.ReadRecipe(pPacket);
		m_pRefFindParm.ReadRecipe(pPacket);
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		int			nValue;
		pPacket->WriteValue(m_bTCSNum);	
		pPacket->WriteValue(m_bUseDPC);
		pPacket->WriteValue(m_bReportResult);
		pPacket->WriteValue(m_dCellPitchConv);
		pPacket->WriteValue(m_dCellPitchScan);
		pPacket->WriteValue(m_nPitchCycleConv);
		pPacket->WriteValue(m_nPitchCycleScan);
		nValue = (int)m_DefectLoc;
		pPacket->WriteValue(nValue);		

		m_pCoordiParm.WriteRecipe(pPacket);
		m_pTransInsParm.WriteRecipe(pPacket);
		m_pRefInsParm.WriteRecipe(pPacket);
		m_pTransFindParm.WriteRecipe(pPacket);
		m_pRefFindParm.WriteRecipe(pPacket);
		return TRUE;
	}
};

#endif