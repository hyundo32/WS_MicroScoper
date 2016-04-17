#if !defined(__GLASS_RECIPE_HEADER_INCLUDED__)
#define __GLASS_RECIPE_HEADER_INCLUDED__

#include "HMRegiConfig.h"
#include "ModelRecipeHeader.h"

#define MAX_ZONE_NUM			16
#define MAX_MASKDATA			20
#define MAX_STRING_LENGTH		100

class AFX_EXT_CLASS CMaskShot
{
public:
	int			m_nStartX;
	int			m_nStartY;
	int			m_nEndX;
	int			m_nEndY;

	CMaskShot()
	{
		m_nStartX	= 0;
		m_nStartY	= 0;
		m_nEndX	= 0;
		m_nEndY	= 0;
	}
	BOOL		InsideOfX(int nX)
	{
		if (m_nStartX < m_nEndX)
			return (nX >= m_nStartX && nX < m_nEndX) ? TRUE : FALSE;
		else
			return (nX >= m_nEndX && nX < m_nStartX) ? TRUE : FALSE;
	}
	BOOL		InsideOfY(int nY)
	{
		if (m_nStartY < m_nEndY)
			return (nY >= m_nStartY && nY < m_nEndY) ? TRUE : FALSE;
		else
			return (nY >= m_nEndY && nY < m_nStartY) ? TRUE : FALSE;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile,int iIndex)
	{
		CString			strSection;

		strSection.Format("GLASS_MASKSHOT_SHOTX_%d",iIndex);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nStartX);
		strSection.Format("GLASS_MASKSHOT_SHOTY_%d",iIndex);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nStartY);
		strSection.Format("GLASS_MASKSHOT_ACTIVEX_%d",iIndex);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nEndX);
		strSection.Format("GLASS_MASKSHOT_ACTIVEY_%d",iIndex);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nEndY);
		
		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile,int iIndex)
	{
		CString			strSection;

		strSection.Format("GLASS_MASKSHOT_SHOTX_%d",iIndex);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nStartX);
		strSection.Format("GLASS_MASKSHOT_SHOTY_%d",iIndex);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nStartY);
		strSection.Format("GLASS_MASKSHOT_ACTIVEX_%d",iIndex);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nEndX);
		strSection.Format("GLASS_MASKSHOT_ACTIVEY_%d",iIndex);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nEndY);

		return TRUE;
	}	
};

class AFX_EXT_CLASS CGLASS_MASKING_CAMTYPE_PARM
{
public:
	BOOL				m_bCDB;
	BOOL				m_bCDW;
	BOOL				m_bMDB;
	BOOL				m_bMDW;
	int					m_nCDSizeB;
	int					m_nCDSizeW;
	int					m_nCDAlarmB;
	int					m_nCDAlarmW;
	int					m_nMDSizeB;
	int					m_nMDSizeW;
	int					m_nMDAlarmB;
	int					m_nMDAlarmW;

	CGLASS_MASKING_CAMTYPE_PARM()
	{		
		Reset();
	}
	void Reset()
	{		
		m_bCDB = m_bCDW = m_bMDB = m_bMDW = FALSE;
		m_nCDSizeB = m_nCDSizeW = m_nCDAlarmB = m_nCDAlarmW = m_nMDSizeB = 0;
		m_nMDSizeW = m_nMDAlarmB = m_nMDAlarmW = 0;
	}	
	CGLASS_MASKING_CAMTYPE_PARM& operator=(const CGLASS_MASKING_CAMTYPE_PARM &rh)
	{
		m_bCDB = rh.m_bCDB;
		m_bCDW = rh.m_bCDW;
		m_bMDB = rh.m_bMDB;
		m_bMDW = rh.m_bMDW;
		m_nCDSizeB = rh.m_nCDSizeB;		
		m_nCDSizeW = rh.m_nCDSizeW;	
		m_nCDAlarmB = rh.m_nCDAlarmB;
		m_nCDAlarmW = rh.m_nCDAlarmW;
		m_nMDSizeB = rh.m_nMDSizeB;
		m_nMDSizeW = rh.m_nMDSizeW;
		m_nMDAlarmB = rh.m_nMDAlarmB;
		m_nMDAlarmW = rh.m_nMDAlarmW;
		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile,CameraType CamType)
	{
		CString			strSection,strTmp;

		switch(CamType)
		{
		case CamType_Reflex: strTmp = "R"; break;
		case CamType_Transfer: strTmp = "T"; break;
		default:
			return FALSE;
		}		
		strSection.Format("GLASS_CDB_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_bCDB);
		strSection.Format("GLASS_CDW_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_bCDW);
		strSection.Format("GLASS_MDB_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_bMDB);
		strSection.Format("GLASS_MDW_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_bMDW);
		strSection.Format("GLASS_CDSizeB_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nCDSizeB);
		strSection.Format("GLASS_CDSizeW_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nCDSizeW);
		strSection.Format("GLASS_CDAlarmB_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nCDAlarmB);
		strSection.Format("GLASS_CDAlarmW_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nCDAlarmW);
		strSection.Format("GLASS_MDSizeB_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nMDSizeB);
		strSection.Format("GLASS_MDSizeW_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nMDSizeW);
		strSection.Format("GLASS_MDAlarmB_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nMDAlarmB);
		strSection.Format("GLASS_MDAlarmW_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nMDAlarmW);

		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile,CameraType CamType)
	{
		CString			strSection,strTmp;

		switch(CamType)
		{
		case CamType_Reflex: strTmp = "R"; break;
		case CamType_Transfer: strTmp = "T"; break;
		default:
			return FALSE;
		}		
		strSection.Format("GLASS_CDB_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_bCDB);
		strSection.Format("GLASS_CDW_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_bCDW);
		strSection.Format("GLASS_MDB_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_bMDB);
		strSection.Format("GLASS_MDW_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_bMDW);
		strSection.Format("GLASS_CDSizeB_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nCDSizeB);
		strSection.Format("GLASS_CDSizeW_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nCDSizeW);
		strSection.Format("GLASS_CDAlarmB_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nCDAlarmB);
		strSection.Format("GLASS_CDAlarmW_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nCDAlarmW);
		strSection.Format("GLASS_MDSizeB_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nMDSizeB);
		strSection.Format("GLASS_MDSizeW_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nMDSizeW);
		strSection.Format("GLASS_MDAlarmB_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nMDAlarmB);
		strSection.Format("GLASS_MDAlarmW_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nMDAlarmW);
		return TRUE;
	}	
};

class AFX_EXT_CLASS CGLASS_MASKING_PARM
{
public:
	BOOL				m_bFindCommonRP;
	BOOL				m_bFindMaskRP;
	int					m_nMaskRPCount;
	int					m_nCommonRPGlass;
	int					m_nCommonRPCount;
	int					m_nMaskShotCount;
	CMaskShot			*m_MaskShot;

	BOOL				m_bUseCDFilter;	
	BOOL				m_bUseMDFilter;	
	int					m_nCDFilter;	// 0 : Pixel, 1 : RScale
	int					m_nMDFilter;	// 0 : Pixel, 1 : RScale

	CGLASS_MASKING_CAMTYPE_PARM	m_pRefMaskParm;
	CGLASS_MASKING_CAMTYPE_PARM	m_pTransMaskParm;

	CGLASS_MASKING_PARM()
	{
		m_MaskShot = NULL;
		Reset();
	}
	~CGLASS_MASKING_PARM()
	{
		if(m_MaskShot != NULL)
			delete[] m_MaskShot, m_MaskShot=NULL;
	}
	void SetMaskShot(int nShotCnt)
	{
		if(m_MaskShot != NULL)
			delete[] m_MaskShot, m_MaskShot=NULL;
		if(nShotCnt <= 0)
			return;
		m_nMaskShotCount = nShotCnt;
		m_MaskShot = new CMaskShot[nShotCnt];
	}
	void Reset()
	{		
		m_bFindCommonRP = m_bFindMaskRP = m_bUseCDFilter = m_bUseMDFilter = FALSE;
		m_nMaskRPCount = m_nCommonRPGlass = m_nCommonRPCount = m_nMaskShotCount = m_nCDFilter = m_nMDFilter = 0;
		if(m_MaskShot != NULL)
			delete[] m_MaskShot, m_MaskShot=NULL;
		m_pRefMaskParm.Reset();
		m_pTransMaskParm.Reset();
	}	
	CGLASS_MASKING_PARM& operator=(const CGLASS_MASKING_PARM &rh)
	{
		m_bFindCommonRP = rh.m_bFindCommonRP;
		m_bFindMaskRP = rh.m_bFindMaskRP;
		m_bUseCDFilter = rh.m_bUseCDFilter;
		m_bUseMDFilter = rh.m_bUseMDFilter;
		m_nMaskRPCount = rh.m_nMaskRPCount;		
		m_nCommonRPGlass = rh.m_nCommonRPGlass;	
		m_nCommonRPCount = rh.m_nCommonRPCount;		
		m_nCDFilter = rh.m_nCDFilter;
		m_nMDFilter = rh.m_nMDFilter;

		SetMaskShot(rh.m_nMaskShotCount);
		int		iLoop;
		for(iLoop=0;iLoop<m_nMaskShotCount;iLoop++)
		{
			m_MaskShot[iLoop] = rh.m_MaskShot[iLoop];
		}

		m_pRefMaskParm = m_pRefMaskParm;
		m_pTransMaskParm = m_pTransMaskParm;

		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile)
	{
		pFile->GetItemValue("GLASS_FINDCOMMONRP", m_bFindCommonRP);
		pFile->GetItemValue("GLASS_FINDMASKRP", m_bFindMaskRP);
		pFile->GetItemValue("GLASS_CDFLTUSE", m_bUseCDFilter);
		pFile->GetItemValue("GLASS_MDFLTUSE", m_bUseMDFilter);
		pFile->GetItemValue("GLASS_MASKRPCOUNT", m_nMaskRPCount);		
		pFile->GetItemValue("GLASS_COMMONRPGLASS", m_nCommonRPGlass);
		pFile->GetItemValue("GLASS_COMMONRPCOUNT", m_nCommonRPCount);
		pFile->GetItemValue("GLASS_CDFLT", m_nCDFilter);
		pFile->GetItemValue("GLASS_MDFLT", m_nMDFilter);
		pFile->GetItemValue("GLASS_MASKSHOTCOUNT", m_nMaskShotCount);

		SetMaskShot(m_nMaskShotCount);
		int		iLoop;
		for(iLoop=0;iLoop<m_nMaskShotCount;iLoop++)
		{
			m_MaskShot[iLoop].ReadRecipe(pFile,iLoop);
		}
		m_pRefMaskParm.ReadRecipe(pFile,CamType_Reflex);
		m_pTransMaskParm.ReadRecipe(pFile,CamType_Transfer);
		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile)
	{
		pFile->SetItemValue("GLASS_FINDCOMMONRP", m_bFindCommonRP);
		pFile->SetItemValue("GLASS_FINDMASKRP", m_bFindMaskRP);
		pFile->SetItemValue("GLASS_CDFLTUSE", m_bUseCDFilter);
		pFile->SetItemValue("GLASS_MDFLTUSE", m_bUseMDFilter);
		pFile->SetItemValue("GLASS_MASKRPCOUNT", m_nMaskRPCount);		
		pFile->SetItemValue("GLASS_COMMONRPGLASS", m_nCommonRPGlass);
		pFile->SetItemValue("GLASS_COMMONRPCOUNT", m_nCommonRPCount);
		pFile->SetItemValue("GLASS_CDFLT", m_nCDFilter);
		pFile->SetItemValue("GLASS_MDFLT", m_nMDFilter);	
		pFile->SetItemValue("GLASS_MASKSHOTCOUNT", m_nMaskShotCount);	

		int		iLoop;
		for(iLoop=0;iLoop<m_nMaskShotCount;iLoop++)
		{
			m_MaskShot[iLoop].WriteRecipe(pFile,iLoop);
		}
		m_pRefMaskParm.WriteRecipe(pFile,CamType_Reflex);
		m_pTransMaskParm.WriteRecipe(pFile,CamType_Transfer);
		return TRUE;
	}	
};

//////////////////////////////////////////////////////////////////////////
// Edge Crack 
class AFX_EXT_CLASS CEDGECRACK_RECIPE_PARM
{
public:
	BOOL				m_bEdgeCrack;
	int					m_nEdgeCrackInspRange;		// 검사 폭
	int					m_nEdgeCrackInspThreshold;	// 검사 문턱값
	int					m_nEdgeCrackMarginX;		// X축 마진
	int					m_nEdgeCrackMarginY;		// Y축 마진
	int					m_nEdgeCrackInspThresPixel;
	int					m_bEdgeCrackUseConv;

	CEDGECRACK_RECIPE_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_bEdgeCrack = FALSE;
		m_nEdgeCrackInspRange = 0;
		m_nEdgeCrackInspThreshold = 0;
		m_nEdgeCrackMarginX = 0;
		m_nEdgeCrackMarginY = 0;
		m_nEdgeCrackInspThresPixel = 0;
		m_bEdgeCrackUseConv = 0;
	}
	CEDGECRACK_RECIPE_PARM& operator=(const CEDGECRACK_RECIPE_PARM &rh)
	{
		m_bEdgeCrack = rh.m_bEdgeCrack;
		m_nEdgeCrackInspRange = rh.m_nEdgeCrackInspRange;
		m_nEdgeCrackInspThreshold = rh.m_nEdgeCrackInspThreshold;
		m_nEdgeCrackMarginX = rh.m_nEdgeCrackMarginX;
		m_nEdgeCrackMarginY = rh.m_nEdgeCrackMarginY;
		m_nEdgeCrackInspThresPixel = rh.m_nEdgeCrackInspThresPixel;
		m_bEdgeCrackUseConv = rh.m_bEdgeCrackUseConv;
		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile)
	{
		pFile->GetItemValue("GLASS_EDGECRACK", m_bEdgeCrack);
		pFile->GetItemValue("GLASS_EDGECRACKINSPRANGE", m_nEdgeCrackInspRange, 1000);
		pFile->GetItemValue("GLASS_EDGECRACKINSPTHRESHOLD", m_nEdgeCrackInspThreshold, 30);
		pFile->GetItemValue("GLASS_EDGECRACKMARGINX", m_nEdgeCrackMarginX, 5000);
		pFile->GetItemValue("GLASS_EDGECRACKMARGINY", m_nEdgeCrackMarginY, 5000);
		pFile->GetItemValue("GLASS_EDGECRACKTHRESPIXEL", m_nEdgeCrackInspThresPixel, 20);
		pFile->GetItemValue("GLASS_EDGECRACKUSECONV", m_bEdgeCrackUseConv, FALSE);

		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile)
	{
		pFile->SetItemValue("GLASS_EDGECRACK", m_bEdgeCrack);
		pFile->SetItemValue("GLASS_EDGECRACKINSPRANGE", m_nEdgeCrackInspRange);
		pFile->SetItemValue("GLASS_EDGECRACKINSPTHRESHOLD", m_nEdgeCrackInspThreshold);
		pFile->SetItemValue("GLASS_EDGECRACKMARGINX", m_nEdgeCrackMarginX);
		pFile->SetItemValue("GLASS_EDGECRACKMARGINY", m_nEdgeCrackMarginY);
		pFile->SetItemValue("GLASS_EDGECRACKTHRESPIXEL", m_nEdgeCrackInspThresPixel);
		pFile->SetItemValue("GLASS_EDGECRACKUSECONV", m_bEdgeCrackUseConv);
		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{			
		pPacket->ReadValue(m_bEdgeCrack);	
		pPacket->ReadValue(m_nEdgeCrackInspRange);
		pPacket->ReadValue(m_nEdgeCrackInspThreshold);
		pPacket->ReadValue(m_nEdgeCrackMarginX);
		pPacket->ReadValue(m_nEdgeCrackMarginY);
		pPacket->ReadValue(m_nEdgeCrackInspThresPixel);
		pPacket->ReadValue(m_bEdgeCrackUseConv);
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		pPacket->WriteValue(m_bEdgeCrack);	
		pPacket->WriteValue(m_nEdgeCrackInspRange);
		pPacket->WriteValue(m_nEdgeCrackInspThreshold);
		pPacket->WriteValue(m_nEdgeCrackMarginX);
		pPacket->WriteValue(m_nEdgeCrackMarginY);
		pPacket->WriteValue(m_nEdgeCrackInspThresPixel);
		pPacket->WriteValue(m_bEdgeCrackUseConv);
		return TRUE;
	}
};

class AFX_EXT_CLASS CRECIPE_INFO_PARM
{	
public:
	char				m_strRecipeName[MAX_STRING_LENGTH];			// 레시피 이름.
	char				m_strDelegateRecipe[MAX_STRING_LENGTH];		// 레시피 이름.
	char				m_strRecipeFileName[MAX_STRING_LENGTH];		// 레피시 파일명
	char				m_strRecipeComment[MAX_STRING_LENGTH];			// Recipe 설명
	int					m_nSoftRevision;			

	CRECIPE_INFO_PARM()
	{
		Reset();
	}
	void Reset()
	{
		ZeroMemory(m_strRecipeName,sizeof(char)*MAX_STRING_LENGTH);
		ZeroMemory(m_strDelegateRecipe,sizeof(char)*MAX_STRING_LENGTH);
		ZeroMemory(m_strRecipeFileName,sizeof(char)*MAX_STRING_LENGTH);
		ZeroMemory(m_strRecipeComment,sizeof(char)*MAX_STRING_LENGTH);		
		m_nSoftRevision = 0;
	}
	CRECIPE_INFO_PARM& operator=(const CRECIPE_INFO_PARM &rh)
	{
		CopyMemory(m_strRecipeName,rh.m_strRecipeName,sizeof(char)*MAX_STRING_LENGTH);
		CopyMemory(m_strDelegateRecipe,rh.m_strDelegateRecipe,sizeof(char)*MAX_STRING_LENGTH);
		CopyMemory(m_strRecipeFileName,rh.m_strRecipeFileName,sizeof(char)*MAX_STRING_LENGTH);
		CopyMemory(m_strRecipeComment,rh.m_strRecipeComment,sizeof(char)*MAX_STRING_LENGTH);
		m_nSoftRevision = rh.m_nSoftRevision;
		return *this;
	}
	void SetRecipeName(CString strRecipe)
	{
		CopyMemory(m_strRecipeName,strRecipe.GetBuffer(0),strRecipe.GetLength());
	}
	void SetRecipeFileName(CString strFile)
	{
		CopyMemory(m_strRecipeFileName,strFile.GetBuffer(0),strFile.GetLength());
	}
	void SetDelegateRecipe(CString strDele)
	{
		CopyMemory(m_strDelegateRecipe,strDele.GetBuffer(0),strDele.GetLength());
	}
	void SetRecipeComment(CString strComment)
	{
		CopyMemory(m_strRecipeComment,strComment.GetBuffer(0),strComment.GetLength());
	}
	CString GetRecipeName()
	{
		CString		strRecipe;		
		strRecipe.Format("%s",m_strRecipeName);
		return strRecipe;
	}
	CString GetRecipeFileName()
	{
		CString		strFile;		
		strFile.Format("%s",m_strRecipeFileName);
		return strFile;
	}
	CString GetDelegateRecipe()
	{
		CString		str;		
		str.Format("%s",m_strDelegateRecipe);
		return str;
	}
	CString GetRecipeComment()
	{
		CString		str;		
		str.Format("%s",m_strRecipeComment);
		return str;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile)
	{
		CString			str;
		pFile->GetItemValue("SOFTREV", m_nSoftRevision);
		pFile->GetItemValue("GLASS_DELEGATERECIPE", str);	// 위임된 Recipe
		CopyMemory(m_strDelegateRecipe,str.GetBuffer(0),str.GetLength());		
		pFile->GetItemValue("GLASS_RECIPECOMMENT", str);	// Glass 모델명.
		if (str.IsEmpty())
			pFile->GetItemValue("GLASS_MODELNAME", str);	// Glass 모델명.
		CopyMemory(m_strRecipeComment,str.GetBuffer(0),str.GetLength());
		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile)
	{
		pFile->SetItemValue("SOFTREV", m_nSoftRevision);
		pFile->SetItemValue("GLASS_DELEGATERECIPE", m_strDelegateRecipe);	// 위임된 Recipe
		pFile->SetItemValue("GLASS_RECIPECOMMENT", m_strRecipeComment);	// Glass 모델명.
		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{			
		pPacket->ReadArray(m_strRecipeName,MAX_STRING_LENGTH);
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		pPacket->WriteArray(m_strRecipeName,MAX_STRING_LENGTH);
		return TRUE;
	}
};

class AFX_EXT_CLASS CLIGHT_RECIPE_PARM
{		
public:
	int					m_nLightMax;	// 알람 상한
	int					m_nLightMin;	// 알람 하한
	int					m_nLightAdjMax;	// 조절 상한
	int					m_nLightAdjMin;	// 조절 하한
	int					m_nLightSet;	// 설정
	int					m_nLightAlarm;	// 알람 발생 조건
	int					m_nLightDefault; // 기본 볼륨

	CLIGHT_RECIPE_PARM()
	{	
		Reset();
	}	
	void Reset()
	{		
		m_nLightMax = m_nLightMin = m_nLightAdjMax = m_nLightAdjMin = 0;
		m_nLightSet = m_nLightAlarm = m_nLightDefault = 0;				
	}
	
	CLIGHT_RECIPE_PARM& operator=(const CLIGHT_RECIPE_PARM &rh)
	{		
		m_nLightMax = rh.m_nLightMax;
		m_nLightMin = rh.m_nLightMin;
		m_nLightAdjMax = rh.m_nLightAdjMax;
		m_nLightAdjMin = rh.m_nLightAdjMin;
		m_nLightSet = rh.m_nLightSet;
		m_nLightAlarm = rh.m_nLightAlarm;
		m_nLightDefault = rh.m_nLightDefault;
		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile,CameraType Camtype)
	{
		CString			strSection,strTmp;
		
		switch (Camtype)
		{
		case CamType_Reflex: strTmp = "R"; break;
		case CamType_Transfer: strTmp = "T"; break;
		default:
			return FALSE;
		}
		strSection.Format("GLASS_LIGHTMAX_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightMax);
		strSection.Format("GLASS_LIGHTMIN_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightMin);
		strSection.Format("GLASS_LIGHTADJMAX_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightAdjMax);
		strSection.Format("GLASS_LIGHTADJMIN_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightAdjMin);
		strSection.Format("GLASS_LIGHTSET_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightSet);
		strSection.Format("GLASS_LIGHTALARM_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightAlarm,2);
		strSection.Format("GLASS_LIGHTDEFAULT_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightDefault,50);		
		
		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile,CameraType Camtype)
	{
		CString			strSection,strTmp;

		switch (Camtype)
		{
		case CamType_Reflex: strTmp = "R"; break;
		case CamType_Transfer: strTmp = "T"; break;
		default:
			return FALSE;
		}
		strSection.Format("GLASS_LIGHTMAX_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightMax);
		strSection.Format("GLASS_LIGHTMIN_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightMin);
		strSection.Format("GLASS_LIGHTADJMAX_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightAdjMax);
		strSection.Format("GLASS_LIGHTADJMIN_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightAdjMin);
		strSection.Format("GLASS_LIGHTSET_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightSet);
		strSection.Format("GLASS_LIGHTALARM_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightAlarm);
		strSection.Format("GLASS_LIGHTDEFAULT_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nLightDefault);

		return TRUE;
	}
};

class AFX_EXT_CLASS CCAMERA_INSPECT_PARM
{
public:
	int			m_nCamNum;
	int			m_nConvMode;
	int			m_nThresFindGlass;
	BOOL		m_bIsZone;
	BOOL		m_bUseZoneData;		
	BOOL		m_bZoneFailUsePich;
	int			m_nZonFailPitchCycleConv;
	int			m_nZonFailPitchCycleScan;
	int			m_nZoneFailConvMode;
	
	CCAMERA_INSPECT_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_nConvMode = 0;
		m_nCamNum = 0;
		m_nThresFindGlass = 0;
		m_bIsZone = m_bUseZoneData = m_bZoneFailUsePich = FALSE;
		m_nZonFailPitchCycleConv = m_nZonFailPitchCycleScan = m_nZonFailPitchCycleScan = m_nZoneFailConvMode = 0;
	}
	CCAMERA_INSPECT_PARM& operator=(const CCAMERA_INSPECT_PARM &rh)
	{
		m_nCamNum = rh.m_nCamNum;
		m_nConvMode = rh.m_nConvMode;
		m_nThresFindGlass = rh.m_nThresFindGlass;
		m_bIsZone = rh.m_bIsZone;
		m_bUseZoneData = rh.m_bUseZoneData;
		m_bZoneFailUsePich = rh.m_bZoneFailUsePich;
		m_nZonFailPitchCycleConv = rh.m_nZonFailPitchCycleConv;
		m_nZonFailPitchCycleScan = rh.m_nZonFailPitchCycleScan;
		m_nZonFailPitchCycleScan = rh.m_nZonFailPitchCycleScan;
		m_nZoneFailConvMode = rh.m_nZoneFailConvMode;
		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile,CameraType Camtype)
	{
		CString			strSection,strTmp;

		switch (Camtype)
		{
		case CamType_Reflex: strTmp = "R"; break;
		case CamType_Transfer: strTmp = "T"; break;
		default:
			return FALSE;
		}

		strSection.Format("INSPECT_MODE_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nConvMode);
		strSection.Format("INSPECT_FINDGLASS_THRES_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_nThresFindGlass);		
		strSection.Format("INSPECT_GLASS_ZONEDATA_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection, m_bUseZoneData);	
		strSection.Format("INSPECT_GLASS_INSPECTORZONEFAIL_USE_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection,m_bZoneFailUsePich, FALSE);
		strSection.Format("INSPECT_GLASS_INSPECTORZONEFAIL_CYCLECONV_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection,m_nZonFailPitchCycleConv, 2);
		strSection.Format("INSPECT_GLASS_INSPECTORZONEFAIL_CYCLESCAN_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection,m_nZonFailPitchCycleScan, 2);
		strSection.Format("INSPECT_GLASS_INSPECTORZONEFAIL_CONVMODE_%s",strTmp);
		pFile->GetItemValue((LPSTR)(LPCTSTR)strSection,m_nZoneFailConvMode, 0);
		return TRUE;			
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile,CameraType Camtype)
	{
		CString			strSection,strTmp;

		switch (Camtype)
		{
		case CamType_Reflex: strTmp = "R"; break;
		case CamType_Transfer: strTmp = "T"; break;
		default:
			return FALSE;
		}
		strSection.Format("INSPECT_MODE_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nConvMode);
		strSection.Format("INSPECT_FINDGLASS_THRES_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_nThresFindGlass);
		strSection.Format("INSPECT_GLASS_ZONEDATA_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection, m_bUseZoneData);
		strSection.Format("INSPECT_GLASS_INSPECTORZONEFAIL_USE_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection,m_bZoneFailUsePich);
		strSection.Format("INSPECT_GLASS_INSPECTORZONEFAIL_CYCLECONV_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection,m_nZonFailPitchCycleConv);
		strSection.Format("INSPECT_GLASS_INSPECTORZONEFAIL_CYCLESCAN_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection,m_nZonFailPitchCycleScan);
		strSection.Format("INSPECT_GLASS_INSPECTORZONEFAIL_CONVMODE_%s",strTmp);
		pFile->SetItemValue((LPSTR)(LPCTSTR)strSection,m_nZoneFailConvMode);
		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{			
		pPacket->ReadValue(m_nCamNum);
		pPacket->ReadValue(m_nConvMode);
		pPacket->ReadValue(m_nThresFindGlass);
		pPacket->ReadValue(m_bUseZoneData);
		pPacket->ReadValue(m_bZoneFailUsePich);
		pPacket->ReadValue(m_nZonFailPitchCycleConv);
		pPacket->ReadValue(m_nZonFailPitchCycleScan);
		pPacket->ReadValue(m_nZoneFailConvMode);
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		pPacket->WriteValue(m_nCamNum);
		pPacket->WriteValue(m_nConvMode);
		pPacket->WriteValue(m_nThresFindGlass);
		pPacket->WriteValue(m_bUseZoneData);
		pPacket->WriteValue(m_bZoneFailUsePich);
		pPacket->WriteValue(m_nZonFailPitchCycleConv);
		pPacket->WriteValue(m_nZonFailPitchCycleScan);
		pPacket->WriteValue(m_nZoneFailConvMode);
		return TRUE;
	}
};

class AFX_EXT_CLASS CGLASS_ALIGN_PARM
{
public:
	BOOL				m_bUsePreAlign;//도석
	BOOL				m_bDetectAlign;	
	BOOL				m_bAlignUseTM;
	BOOL				m_bAlignTempAutoSave;
	int					m_nAlignTH;
	int					m_nAlignMergeRange;
	int					m_nAlignMatchRate;

	BOOL				m_bEdgeAlign;
	int					m_nEdgeAlignTH;
	int					m_nEdgeAlignFindLine;

	CGLASS_ALIGN_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_nAlignTH = m_nAlignMergeRange = m_nAlignMatchRate = m_nEdgeAlignTH = m_nEdgeAlignFindLine = 0;
		m_bDetectAlign = m_bAlignUseTM = m_bAlignTempAutoSave = m_bEdgeAlign = FALSE;
	}	
	CGLASS_ALIGN_PARM& operator=(const CGLASS_ALIGN_PARM &rh)
	{
		m_bUsePreAlign = rh.m_bUsePreAlign;
		m_nAlignTH = rh.m_nAlignTH;
		m_nAlignMergeRange = rh.m_nAlignMergeRange;
		m_nAlignMatchRate = rh.m_nAlignMatchRate;
		m_bDetectAlign = rh.m_bDetectAlign;
		m_bAlignUseTM = rh.m_bAlignUseTM;		
		m_bAlignTempAutoSave = rh.m_bAlignTempAutoSave;	
		m_bEdgeAlign = rh.m_bEdgeAlign;
		m_nEdgeAlignTH = rh.m_nEdgeAlignTH;
		m_nEdgeAlignFindLine = rh.m_nEdgeAlignFindLine;
		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile)
	{
		pFile->GetItemValue("GLASS_PRE_ALIGN", m_bUsePreAlign);
		pFile->GetItemValue("GLASS_ALIGNTH", m_nAlignTH);
		pFile->GetItemValue("GLASS_ALIGNMERGERANGE", m_nAlignMergeRange);
		pFile->GetItemValue("GLASS_ALIGNMATCHRATE", m_nAlignMatchRate);
		pFile->GetItemValue("GLASS_DETECTALIGN", m_bDetectAlign);
		pFile->GetItemValue("GLASS_ALIGNTM", m_bAlignUseTM);		
		pFile->GetItemValue("GLASS_ALIGNTEMP_AUTOSAVE", m_bAlignTempAutoSave);
		pFile->GetItemValue("GLASS_EDGEALIGN", m_bEdgeAlign);
		pFile->GetItemValue("GLASS_EDGEALIGNTH", m_nEdgeAlignTH);
		pFile->GetItemValue("GLASS_EDGEALIGNFINDLINE", m_nEdgeAlignFindLine);
		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile)
	{
		pFile->SetItemValue("GLASS_PRE_ALIGN", m_bUsePreAlign);
		pFile->SetItemValue("GLASS_ALIGNTH", m_nAlignTH);
		pFile->SetItemValue("GLASS_ALIGNMERGERANGE", m_nAlignMergeRange);
		pFile->SetItemValue("GLASS_ALIGNMATCHRATE", m_nAlignMatchRate);
		pFile->SetItemValue("GLASS_DETECTALIGN", m_bDetectAlign);
		pFile->SetItemValue("GLASS_ALIGNTM", m_bAlignUseTM);		
		pFile->SetItemValue("GLASS_ALIGNTEMP_AUTOSAVE", m_bAlignTempAutoSave);
		pFile->SetItemValue("GLASS_EDGEALIGN", m_bEdgeAlign);
		pFile->SetItemValue("GLASS_EDGEALIGNTH", m_nEdgeAlignTH);
		pFile->SetItemValue("GLASS_EDGEALIGNFINDLINE", m_nEdgeAlignFindLine);	
		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{	
		pPacket->ReadValue(m_bUsePreAlign);
		pPacket->ReadValue(m_nAlignTH);
		pPacket->ReadValue(m_nAlignMergeRange);
		pPacket->ReadValue(m_nAlignMatchRate);
		pPacket->ReadValue(m_bDetectAlign);
		pPacket->ReadValue(m_bAlignUseTM);
		pPacket->ReadValue(m_bAlignTempAutoSave);
		pPacket->ReadValue(m_bEdgeAlign);
		pPacket->ReadValue(m_nEdgeAlignTH);
		pPacket->ReadValue(m_nEdgeAlignFindLine);
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		pPacket->WriteValue(m_bUsePreAlign);
		pPacket->WriteValue(m_nAlignTH);
		pPacket->WriteValue(m_nAlignMergeRange);
		pPacket->WriteValue(m_nAlignMatchRate);
		pPacket->WriteValue(m_bDetectAlign);
		pPacket->WriteValue(m_bAlignUseTM);
		pPacket->WriteValue(m_bAlignTempAutoSave);
		pPacket->WriteValue(m_bEdgeAlign);
		pPacket->WriteValue(m_nEdgeAlignTH);
		pPacket->WriteValue(m_nEdgeAlignFindLine);
		return TRUE;
	}
};

class AFX_EXT_CLASS CGLASS_RECIPE_ETC_PARM
{	
public:
	int					m_nCellNumDir;      //원점 대비 cell index가 어느 방향으로 증가하느냐.	
	BOOL				m_bDontStopMask;
	BOOL				m_bUseTrimmer;
	int					m_nTrimmerWidth;
	CRect				m_rectMasking[2];
	int					m_nUseSize;
	BOOL				m_bUseInspectorSizeTh;
	int					m_nInspectorMergeSize;
	int					m_nInspectorSizeThOffset;
	int					m_nOffsetType;
	BOOL				m_bSamsungJudge;
	int					m_nLightMethod;
	BOOL				m_bSaveZoneMatchingImg;

	CGLASS_RECIPE_ETC_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_nCellNumDir = m_nTrimmerWidth = 0;
		m_bDontStopMask = m_bUseTrimmer = m_nUseSize = m_bUseInspectorSizeTh = m_bSamsungJudge = FALSE;
		m_rectMasking[0] = m_rectMasking[1] = CRect(0,0,0,0);
		m_nInspectorMergeSize = m_nInspectorSizeThOffset = m_nOffsetType = m_nLightMethod = 0;
		m_bSaveZoneMatchingImg = FALSE;
	}	
	CGLASS_RECIPE_ETC_PARM& operator=(const CGLASS_RECIPE_ETC_PARM &rh)
	{
		m_nCellNumDir = rh.m_nCellNumDir;
		m_nTrimmerWidth = rh.m_nTrimmerWidth;		
		m_bDontStopMask = rh.m_bDontStopMask;
		m_bUseTrimmer = rh.m_bUseTrimmer;		
		m_rectMasking[0] = rh.m_rectMasking[0];
		m_rectMasking[1] = rh.m_rectMasking[1];
		m_nUseSize = rh.m_nUseSize;
		m_bUseInspectorSizeTh = rh.m_bUseInspectorSizeTh;
		m_nInspectorMergeSize = rh.m_nInspectorMergeSize;
		m_nInspectorSizeThOffset = rh.m_nInspectorSizeThOffset;
		m_nOffsetType = rh.m_nOffsetType;
		m_bSamsungJudge = rh.m_bSamsungJudge;
		m_nLightMethod = rh.m_nLightMethod;
		m_bSaveZoneMatchingImg = rh.m_bSaveZoneMatchingImg;
		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile)
	{
		pFile->GetItemValue("GLASS_CELLNUMDIR", m_nCellNumDir);
		pFile->GetItemValue("GLASS_TRIMMER_WIDTH", m_nTrimmerWidth);		
		pFile->GetItemValue("GLASS_DONTSTOPMASK", m_bDontStopMask);
		pFile->GetItemValue("GLASS_TRIMMER", m_bUseTrimmer);		

		int			nValue;		
		pFile->GetItemValue("GLASS_MASKING_LEFT_FIRST", nValue);
		m_rectMasking[0].left = nValue;		
		pFile->GetItemValue("GLASS_MASKING_TOP_FIRST", nValue);
		m_rectMasking[0].top = nValue;		
		pFile->GetItemValue("GLASS_MASKING_RIGHT_FIRST", nValue);
		m_rectMasking[0].right = nValue;		
		pFile->GetItemValue("GLASS_MASKING_BOTTOM_FIRST", nValue);
		m_rectMasking[0].bottom = nValue;		
		pFile->GetItemValue("GLASS_MASKING_LEFT_SECOND", nValue);
		m_rectMasking[1].left = nValue;		
		pFile->GetItemValue("GLASS_MASKING_TOP_SECOND", nValue);
		m_rectMasking[1].top = nValue;		
		pFile->GetItemValue("GLASS_MASKING_RIGHT_SECOND", nValue);
		m_rectMasking[1].right = nValue;		
		pFile->GetItemValue("GLASS_MASKING_BOTTOM_SECOND", nValue);
		m_rectMasking[1].bottom = nValue;

		pFile->GetItemValue("GLASS_INSPECTORSIZE_USE",m_nUseSize,FALSE);
		pFile->GetItemValue("GLASS_INSPECTORSIZETH_USE",m_bUseInspectorSizeTh,FALSE);
		pFile->GetItemValue("GLASS_INSPECTORMERGE_SIZE",m_nInspectorMergeSize,10);
		pFile->GetItemValue("GLASS_INSPECTORSIZETH_OFFSET",m_nInspectorSizeThOffset,3);
		pFile->GetItemValue("GLASS_INSPECTOROFFSET_TYPE",m_nOffsetType,3);
		pFile->GetItemValue("GLASS_SAMSUNG_JUDGE", m_bSamsungJudge, 0);
		pFile->GetItemValue("GLASS_LIGHTMETHOD", m_nLightMethod, 2);
		pFile->GetItemValue("GLASS_SAVEZONEMATCHING_IMG", m_bSaveZoneMatchingImg, FALSE);		

		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile)
	{
		pFile->SetItemValue("GLASS_CELLNUMDIR", m_nCellNumDir);
		pFile->SetItemValue("GLASS_TRIMMER_WIDTH", m_nTrimmerWidth);		
		pFile->SetItemValue("GLASS_DONTSTOPMASK", m_bDontStopMask);
		pFile->SetItemValue("GLASS_TRIMMER", m_bUseTrimmer);		

		int			nValue;
		nValue = m_rectMasking[0].left;
		pFile->SetItemValue("GLASS_MASKING_LEFT_FIRST", nValue);
		nValue = m_rectMasking[0].top;
		pFile->SetItemValue("GLASS_MASKING_TOP_FIRST", nValue);
		nValue = m_rectMasking[0].right;
		pFile->SetItemValue("GLASS_MASKING_RIGHT_FIRST", nValue);
		nValue = m_rectMasking[0].bottom;
		pFile->SetItemValue("GLASS_MASKING_BOTTOM_FIRST", nValue);
		nValue = m_rectMasking[1].left;
		pFile->SetItemValue("GLASS_MASKING_LEFT_SECOND", nValue);
		nValue = m_rectMasking[1].top;
		pFile->SetItemValue("GLASS_MASKING_TOP_SECOND", nValue);
		nValue = m_rectMasking[1].right;
		pFile->SetItemValue("GLASS_MASKING_RIGHT_SECOND", nValue);
		nValue = m_rectMasking[1].bottom;
		pFile->SetItemValue("GLASS_MASKING_BOTTOM_SECOND", nValue);

		pFile->SetItemValue("GLASS_INSPECTORSIZE_USE",m_nUseSize);
		pFile->SetItemValue("GLASS_INSPECTORSIZETH_USE",m_bUseInspectorSizeTh);
		pFile->SetItemValue("GLASS_INSPECTORMERGE_SIZE",m_nInspectorMergeSize);
		pFile->SetItemValue("GLASS_INSPECTORSIZETH_OFFSET",m_nInspectorSizeThOffset);
		pFile->SetItemValue("GLASS_INSPECTOROFFSET_TYPE",m_nOffsetType);
		pFile->SetItemValue("GLASS_SAMSUNG_JUDGE", m_bSamsungJudge);
		pFile->SetItemValue("GLASS_LIGHTMETHOD", m_nLightMethod);
		pFile->SetItemValue("GLASS_SAVEZONEMATCHING_IMG", m_bSaveZoneMatchingImg);

		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{			
		pPacket->ReadValue(m_rectMasking[0].left);
		pPacket->ReadValue(m_rectMasking[0].top);
		pPacket->ReadValue(m_rectMasking[0].right);
		pPacket->ReadValue(m_rectMasking[0].bottom);
		pPacket->ReadValue(m_rectMasking[1].left);
		pPacket->ReadValue(m_rectMasking[1].top);
		pPacket->ReadValue(m_rectMasking[1].right);
		pPacket->ReadValue(m_rectMasking[1].bottom);
		pPacket->ReadValue(m_nUseSize);
		pPacket->ReadValue(m_bUseInspectorSizeTh);
		pPacket->ReadValue(m_nInspectorMergeSize);
		pPacket->ReadValue(m_nInspectorSizeThOffset);
		pPacket->ReadValue(m_nOffsetType);
		pPacket->ReadValue(m_bSaveZoneMatchingImg);
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{		
		pPacket->WriteValue(m_rectMasking[0].left);
		pPacket->WriteValue(m_rectMasking[0].top);
		pPacket->WriteValue(m_rectMasking[0].right);
		pPacket->WriteValue(m_rectMasking[0].bottom);
		pPacket->WriteValue(m_rectMasking[1].left);
		pPacket->WriteValue(m_rectMasking[1].top);
		pPacket->WriteValue(m_rectMasking[1].right);
		pPacket->WriteValue(m_rectMasking[1].bottom);
		pPacket->WriteValue(m_nUseSize);
		pPacket->WriteValue(m_bUseInspectorSizeTh);
		pPacket->WriteValue(m_nInspectorMergeSize);
		pPacket->WriteValue(m_nInspectorSizeThOffset);
		pPacket->WriteValue(m_nOffsetType);
		pPacket->WriteValue(m_bSaveZoneMatchingImg);
		return TRUE;
	}
};

class AFX_EXT_CLASS CGLASS_STACK_PARM
{
public:
	BOOL		m_bStackFileMergeOnOff;	
	int			m_nStackFileMergeSize;
	int			m_nStackFileMergeCount;	
	BOOL		m_bCheckCameraType;
	BOOL		m_bCheckJudeType;
	BOOL		m_bCheckJugeOK;
	BOOL		m_bCheckJugePR;
	BOOL		m_bCheckCameraTypeRB;
	BOOL		m_bCheckCameraTypeRW;
	BOOL		m_bCheckStackStep;
	BOOL		m_bCheckCameraTypeTB;
	BOOL		m_bCheckJudeTR;
	BOOL		m_bCheckCameraTypeTW;	
	int			m_nEditRBCutSize;
	int			m_nEditRWCutSize;
	int			m_nEditTBCutSize;
	int			m_nEditTWCutSize;
	CString		m_strStackStep[CFDEFECT_STACKCOUNT];
	int			m_nStackStepCount;

	CGLASS_STACK_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_bStackFileMergeOnOff = m_bCheckCameraType = m_bCheckJudeType = m_bCheckJugeOK = FALSE;
		m_bCheckJugePR = m_bCheckCameraTypeRB = m_bCheckCameraTypeRW = m_bCheckStackStep = m_bCheckCameraTypeTB = FALSE;
		m_bCheckJudeTR = m_bCheckCameraTypeTW = FALSE;
		m_nStackFileMergeSize =m_nStackFileMergeCount =m_nEditRBCutSize =  m_nEditRWCutSize = m_nEditTBCutSize = 0;
		m_nEditTWCutSize =  m_nStackStepCount = 0;
		int		iLoop;
		for(iLoop=0;iLoop<CFDEFECT_STACKCOUNT;iLoop++)
			m_strStackStep[iLoop].Empty();
	}	
	CGLASS_STACK_PARM& operator=(const CGLASS_STACK_PARM &rh)
	{
		m_bStackFileMergeOnOff = rh.m_bStackFileMergeOnOff;
		m_bCheckCameraType = rh.m_bCheckCameraType;
		m_bCheckJudeType = rh.m_bCheckJudeType;
		m_bCheckJugeOK = rh.m_bCheckJugeOK;
		m_bCheckJugePR = rh.m_bCheckJugePR;
		m_bCheckCameraTypeRB = rh.m_bCheckCameraTypeRB;
		m_bCheckCameraTypeRW = rh.m_bCheckCameraTypeRW;
		m_bCheckStackStep = rh.m_bCheckStackStep;
		m_bCheckCameraTypeTB = rh.m_bCheckCameraTypeTB;
		m_bCheckJudeTR = rh.m_bCheckJudeTR;
		m_bCheckCameraTypeTW = rh.m_bCheckCameraTypeTW;
		m_nStackFileMergeSize = rh.m_nStackFileMergeSize;
		m_nStackFileMergeCount = rh.m_nStackFileMergeCount;
		m_nEditRBCutSize = rh.m_nEditRBCutSize;
		m_nEditRWCutSize = rh.m_nEditRWCutSize;
		m_nEditTBCutSize = rh.m_nEditTBCutSize;
		m_nEditTWCutSize = rh.m_nEditTWCutSize;
		m_nStackStepCount = rh.m_nStackStepCount;
		int		iLoop;
		for(iLoop=0;iLoop<CFDEFECT_STACKCOUNT;iLoop++)
			m_strStackStep[iLoop] = rh.m_strStackStep[iLoop];
		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile)
	{
		pFile->GetItemValue("GLASS_STACKFILEMERGE_ONOFF", m_bStackFileMergeOnOff,FALSE);
		pFile->GetItemValue("GLASS_STACKFILEMERGE_SIZE", m_nStackFileMergeSize,0);
		pFile->GetItemValue("GALSS_STACKFILEMERGE_COUNT",m_nStackFileMergeCount,2000);	

		pFile->GetItemValue("GLASS_STACK_MERGE_CAMERA_TYPE",m_bCheckCameraType);
		pFile->GetItemValue("GLASS_STACK_MERGE_CAMERA_TYPE_TB",m_bCheckCameraTypeTB);
		pFile->GetItemValue("GLASS_STACK_MERGE_CAMERA_TYPE_TW",m_bCheckCameraTypeTW);
		pFile->GetItemValue("GLASS_STACK_MERGE_CAMERA_TYPE_RB",m_bCheckCameraTypeRB);
		pFile->GetItemValue("GLASS_STACK_MERGE_CAMERA_TYPE_RW",m_bCheckCameraTypeRW);


		pFile->GetItemValue("GLASS_STACK_MERGE_JUDE_TYPE",m_bCheckJudeType);
		pFile->GetItemValue("GLASS_STACK_MERGE_JUDE_TYPE_OK",m_bCheckJugeOK);
		pFile->GetItemValue("GLASS_STACK_MERGE_JUDE_TYPE_PR",m_bCheckJugePR);
		pFile->GetItemValue("GLASS_STACK_MERGE_JUDE_TYPE_TR",m_bCheckJudeTR);

		pFile->GetItemValue("GLASS_STACK_MERGE_STACK_STEP",m_bCheckStackStep);
		pFile->GetItemValue("GLASS_STACK_MERGE_STACK_STEP_COUNT",m_nStackStepCount);
		//20100506 Stack File Cut Off Size 설정
		pFile->GetItemValue("GLASS_STACK_MERGE_CUTOFF_RB",m_nEditRBCutSize,0);
		pFile->GetItemValue("GLASS_STACK_MERGE_CUTOFF_RW",m_nEditRWCutSize,0);
		pFile->GetItemValue("GLASS_STACK_MERGE_CUTOFF_TB",m_nEditTBCutSize,0);
		pFile->GetItemValue("GLASS_STACK_MERGE_CUTOFF_TW",m_nEditTWCutSize,0);

		int		i;
		for(i = 0; i <CFDEFECT_STACKCOUNT; i++ )
		{
			pFile->GetItemValue(i,"GLASS_STACK_MERGE_STACK_STEP_NAME",m_strStackStep[i]);
		}
		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile)
	{
		pFile->SetItemValue("GLASS_STACKFILEMERGE_ONOFF", m_bStackFileMergeOnOff);
		pFile->SetItemValue("GLASS_STACKFILEMERGE_SIZE", m_nStackFileMergeSize);
		pFile->SetItemValue("GALSS_STACKFILEMERGE_COUNT",m_nStackFileMergeCount);

		pFile->SetItemValue("GLASS_STACK_MERGE_CAMERA_TYPE",m_bCheckCameraType);
		pFile->SetItemValue("GLASS_STACK_MERGE_CAMERA_TYPE_TB",m_bCheckCameraTypeTB);
		pFile->SetItemValue("GLASS_STACK_MERGE_CAMERA_TYPE_TW",m_bCheckCameraTypeTW);
		pFile->SetItemValue("GLASS_STACK_MERGE_CAMERA_TYPE_RB",m_bCheckCameraTypeRB);
		pFile->SetItemValue("GLASS_STACK_MERGE_CAMERA_TYPE_RW",m_bCheckCameraTypeRW);

		pFile->SetItemValue("GLASS_STACK_MERGE_JUDE_TYPE",m_bCheckJudeType);
		pFile->SetItemValue("GLASS_STACK_MERGE_JUDE_TYPE_OK",m_bCheckJugeOK);
		pFile->SetItemValue("GLASS_STACK_MERGE_JUDE_TYPE_PR",m_bCheckJugePR);
		pFile->SetItemValue("GLASS_STACK_MERGE_JUDE_TYPE_TR",m_bCheckJudeTR);

		pFile->SetItemValue("GLASS_STACK_MERGE_STACK_STEP",m_bCheckStackStep);
		pFile->SetItemValue("GLASS_STACK_MERGE_STACK_STEP_COUNT",m_nStackStepCount);
		//20100506 Stack File Cut Off Size 설정
		pFile->SetItemValue("GLASS_STACK_MERGE_CUTOFF_RB",m_nEditRBCutSize);
		pFile->SetItemValue("GLASS_STACK_MERGE_CUTOFF_RW",m_nEditRWCutSize);
		pFile->SetItemValue("GLASS_STACK_MERGE_CUTOFF_TB",m_nEditTBCutSize);
		pFile->SetItemValue("GLASS_STACK_MERGE_CUTOFF_TW",m_nEditTWCutSize);

		int		i;
		for(i = 0; i <CFDEFECT_STACKCOUNT; i++ )
		{
			pFile->SetItemValue(i,"GLASS_STACK_MERGE_STACK_STEP_NAME",(LPSTR)(LPCTSTR)m_strStackStep[i]);
		}
		return TRUE;
	}
};

class AFX_EXT_CLASS CGLASS_DEFECT_JUDG_PARM
{
public:
	int					m_nSizeJudgeS;
	int					m_nSizeJudgeM;
	int					m_nSizeJudgeL;
	int					m_nPeakJudgeS;
	int					m_nPeakJudgeM;

	CGLASS_DEFECT_JUDG_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_nSizeJudgeS = 0;
		m_nSizeJudgeM = 0;		
		m_nSizeJudgeL = 0;
		m_nPeakJudgeS = 0;
		m_nPeakJudgeM = 0;
	}	
	CGLASS_DEFECT_JUDG_PARM& operator=(const CGLASS_DEFECT_JUDG_PARM &rh)
	{
		m_nSizeJudgeS = rh.m_nSizeJudgeS;
		m_nSizeJudgeM = rh.m_nSizeJudgeM;
		m_nSizeJudgeL = rh.m_nSizeJudgeL;
		m_nPeakJudgeS = rh.m_nPeakJudgeS;
		m_nPeakJudgeM = rh.m_nPeakJudgeM;
		return *this;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile)
	{
		pFile->GetItemValue("GLASS_SSIZE_JUDGE", m_nSizeJudgeS, 5);
		pFile->GetItemValue("GLASS_MSIZE_JUDGE", m_nSizeJudgeM, 10);
		pFile->GetItemValue("GLASS_LSIZE_JUDGE", m_nSizeJudgeL, 15);
		pFile->GetItemValue("GLASS_SHEIGHT_JUDGE", m_nPeakJudgeS, 10);
		pFile->GetItemValue("GLASS_MHEIGHT_JUDGE", m_nPeakJudgeM, 20);	
		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile)
	{
		pFile->SetItemValue("GLASS_SSIZE_JUDGE", m_nSizeJudgeS);
		pFile->SetItemValue("GLASS_MSIZE_JUDGE", m_nSizeJudgeM);
		pFile->SetItemValue("GLASS_LSIZE_JUDGE", m_nSizeJudgeL);
		pFile->SetItemValue("GLASS_SHEIGHT_JUDGE", m_nPeakJudgeS);
		pFile->SetItemValue("GLASS_MHEIGHT_JUDGE", m_nPeakJudgeM);
		return TRUE;
	}
};

class AFX_EXT_CLASS CGLASS_RECIPE_PARM
{	
public:
	int					m_nCameraNum;	
	int					m_nScanNum;	
	GlassType			m_GlassType;				// 현재 Glass의 종류
	double				m_dGlassThickness;			// Glass 두께.
	int					m_nSelectCellOri;		
	
	CRECIPE_INFO_PARM			m_pRcpInfoPrm;
	CCAMERA_INSPECT_PARM		m_pRefInspectPrm;
	CCAMERA_INSPECT_PARM		m_pTransInspectPrm;
	CEDGECRACK_RECIPE_PARM		m_pEdgeCrack;			// Edge Crack
	CLIGHT_RECIPE_PARM			m_pTransLightParm;		// 투과 광량 Parm
	CLIGHT_RECIPE_PARM			m_pRefLightParm;		// 반사 광량 Parm
	CGLASS_ALIGN_PARM			m_pGlassAlignParm;	
	CGLASS_MASKING_PARM			m_pGlassMaskParm;
	CGLASS_DEFECT_JUDG_PARM		m_pDefectJudge;
	CGLASS_STACK_PARM			m_pStackParm;
	CGLASS_RECIPE_ETC_PARM		m_pGlassEtcParm;

	CGLASS_RECIPE_PARM()
	{
		Reset();
	}
	void Reset()
	{
		m_nCameraNum = 0;
		m_nScanNum = 0;		
		m_GlassType.Reset();
		m_dGlassThickness = 0.;
		m_nSelectCellOri = 0;
		m_pRcpInfoPrm.Reset();
		m_pRefInspectPrm.Reset();
		m_pTransInspectPrm.Reset();
		m_pEdgeCrack.Reset();
		m_pTransLightParm.Reset();
		m_pRefLightParm.Reset();
		m_pGlassEtcParm.Reset();
		m_pGlassAlignParm.Reset();
		m_pGlassMaskParm.Reset();
		m_pDefectJudge.Reset();
		m_pStackParm.Reset();
	}
	void SetCamNum(int nTransCnt,int nRefCnt)
	{
		m_nCameraNum = nTransCnt+nRefCnt;
		m_pRefInspectPrm.m_nCamNum = nRefCnt;
		m_pTransInspectPrm.m_nCamNum = nTransCnt;
	}
	CGLASS_RECIPE_PARM& operator=(const CGLASS_RECIPE_PARM &rh)
	{
		m_nCameraNum = rh.m_nCameraNum;
		m_nScanNum = rh.m_nScanNum;
		m_GlassType = rh.m_GlassType;
		m_dGlassThickness = rh.m_dGlassThickness;
		m_nSelectCellOri = rh.m_nSelectCellOri;

		m_pRcpInfoPrm = rh.m_pRcpInfoPrm;
		m_pRefInspectPrm = rh.m_pRefInspectPrm;
		m_pTransInspectPrm = rh.m_pTransInspectPrm;
		m_pEdgeCrack = rh.m_pEdgeCrack;
		m_pTransLightParm = rh.m_pTransLightParm;
		m_pRefLightParm = rh.m_pRefLightParm;
		m_pGlassEtcParm = rh.m_pGlassEtcParm;
		m_pGlassAlignParm = rh.m_pGlassAlignParm;
		m_pGlassMaskParm = rh.m_pGlassMaskParm;
		m_pDefectJudge = rh.m_pDefectJudge;
		m_pStackParm = rh.m_pStackParm;
		return *this;
	}
	BOOL ReadRecipeAlong(CHMRegiConfig *pFile)
	{
		if(pFile == NULL)
			return FALSE;

		int			nValue;

		pFile->GetItemValue("GLASS_TYPEX", m_GlassType.m_nGlassSizeX);	// 현재 Glass의 X 크기
		pFile->GetItemValue("GLASS_TYPEY", m_GlassType.m_nGlassSizeY);	// 현재 Glass의 Y 크기
		pFile->GetItemValue("GLASS_ORIGINDIR", nValue);					// 현재 Glass의 Y 크기
		m_GlassType.m_OriginDirection = (GlassDirection)nValue;					// 현재 Glass의 원점 방향
		pFile->GetItemValue("GLASS_CCDIR", nValue);
		m_GlassType.m_CCutDirection = (GlassDirection)nValue;					// 현재 Glass의 코너컷 방향
		pFile->GetItemValue("GLASS_THICKNESS", m_dGlassThickness);		// Glass 두께.
		pFile->GetItemValue("GLASS_CELL_ORI", m_nSelectCellOri);	

		m_pRcpInfoPrm.ReadRecipe(pFile);

		return TRUE;
	}
	BOOL ReadRecipe(CHMRegiConfig *pFile)
	{
		if(pFile == NULL)
			return FALSE;

		int			nValue;

		pFile->GetItemValue("GLASS_TYPEX", m_GlassType.m_nGlassSizeX);	// 현재 Glass의 X 크기
		pFile->GetItemValue("GLASS_TYPEY", m_GlassType.m_nGlassSizeY);	// 현재 Glass의 Y 크기
		pFile->GetItemValue("GLASS_ORIGINDIR", nValue);					// 현재 Glass의 Y 크기
		m_GlassType.m_OriginDirection = (GlassDirection)nValue;					// 현재 Glass의 원점 방향
		pFile->GetItemValue("GLASS_CCDIR", nValue);
		m_GlassType.m_CCutDirection = (GlassDirection)nValue;					// 현재 Glass의 코너컷 방향
		pFile->GetItemValue("GLASS_THICKNESS", m_dGlassThickness);		// Glass 두께.
		pFile->GetItemValue("GLASS_CELL_ORI", m_nSelectCellOri);		

		m_pRcpInfoPrm.ReadRecipe(pFile);
		m_pRefInspectPrm.ReadRecipe(pFile,CamType_Reflex);
		m_pTransInspectPrm.ReadRecipe(pFile,CamType_Transfer);
		m_pEdgeCrack.ReadRecipe(pFile);
		m_pRefLightParm.ReadRecipe(pFile,CamType_Reflex);
		m_pTransLightParm.ReadRecipe(pFile,CamType_Transfer);
		m_pGlassAlignParm.ReadRecipe(pFile);
		m_pGlassEtcParm.ReadRecipe(pFile);
		m_pGlassMaskParm.ReadRecipe(pFile);
		m_pDefectJudge.ReadRecipe(pFile);
		m_pStackParm.ReadRecipe(pFile);
		return TRUE;
	}
	BOOL WriteRecipeAlong(CHMRegiConfig *pFile)
	{
		if(pFile == NULL)
			return FALSE;

		int			nValue;

		pFile->SetItemValue("GLASS_TYPEX", m_GlassType.m_nGlassSizeX);		// 현재 Glass의 X 크기
		pFile->SetItemValue("GLASS_TYPEY", m_GlassType.m_nGlassSizeY);		// 현재 Glass의 Y 크기
		nValue = (int)m_GlassType.m_OriginDirection;								// 현재 Glass의 원점 방향
		pFile->SetItemValue("GLASS_ORIGINDIR", nValue);
		nValue = (int)m_GlassType.m_CCutDirection;									// 현재 Glass의 코너컷 방향
		pFile->SetItemValue("GLASS_CCDIR", nValue);
		pFile->SetItemValue("GLASS_THICKNESS", m_dGlassThickness);			// Glass 두께.
		pFile->SetItemValue("GLASS_CELL_ORI", m_nSelectCellOri);

		return TRUE;
	}
	BOOL WriteRecipe(CHMRegiConfig *pFile)
	{
		int			nValue;

		pFile->SetItemValue("GLASS_TYPEX", m_GlassType.m_nGlassSizeX);		// 현재 Glass의 X 크기
		pFile->SetItemValue("GLASS_TYPEY", m_GlassType.m_nGlassSizeY);		// 현재 Glass의 Y 크기
		nValue = (int)m_GlassType.m_OriginDirection;								// 현재 Glass의 원점 방향
		pFile->SetItemValue("GLASS_ORIGINDIR", nValue);
		nValue = (int)m_GlassType.m_CCutDirection;									// 현재 Glass의 코너컷 방향
		pFile->SetItemValue("GLASS_CCDIR", nValue);
		pFile->SetItemValue("GLASS_THICKNESS", m_dGlassThickness);			// Glass 두께.
		pFile->SetItemValue("GLASS_CELL_ORI", m_nSelectCellOri);

		m_pRcpInfoPrm.WriteRecipe(pFile);
		m_pRefInspectPrm.WriteRecipe(pFile,CamType_Reflex);
		m_pTransInspectPrm.WriteRecipe(pFile,CamType_Transfer);
		m_pEdgeCrack.WriteRecipe(pFile);
		m_pRefLightParm.WriteRecipe(pFile,CamType_Reflex);
		m_pTransLightParm.WriteRecipe(pFile,CamType_Transfer);
		m_pGlassAlignParm.WriteRecipe(pFile);
		m_pGlassEtcParm.WriteRecipe(pFile);
		m_pGlassMaskParm.WriteRecipe(pFile);
		m_pDefectJudge.WriteRecipe(pFile);
		m_pStackParm.WriteRecipe(pFile);
		return TRUE;
	}
	BOOL ReadRecipe(CMosisPacket *pPacket)
	{			
		int			nValue;
		pPacket->ReadValue(m_GlassType.m_nGlassSizeX);
		pPacket->ReadValue(m_GlassType.m_nGlassSizeY);
		pPacket->ReadValue(nValue);
		m_GlassType.m_OriginDirection = (GlassDirection)nValue;
		pPacket->ReadValue(nValue);
		m_GlassType.m_CCutDirection = (GlassDirection)nValue;
		pPacket->ReadValue(m_dGlassThickness);
		pPacket->ReadValue(m_nSelectCellOri);

		m_pRcpInfoPrm.ReadRecipe(pPacket);
		m_pRefInspectPrm.ReadRecipe(pPacket);
		m_pTransInspectPrm.ReadRecipe(pPacket);
		m_pEdgeCrack.ReadRecipe(pPacket);
		m_pGlassAlignParm.ReadRecipe(pPacket);
		m_pGlassEtcParm.ReadRecipe(pPacket);
		return TRUE;
	}
	BOOL WriteRecipe(CMosisPacket *pPacket)
	{
		int			nValue;
		pPacket->WriteValue(m_GlassType.m_nGlassSizeX);
		pPacket->WriteValue(m_GlassType.m_nGlassSizeY);
		nValue = (int)m_GlassType.m_OriginDirection;
		pPacket->WriteValue(nValue);
		nValue = (int)m_GlassType.m_CCutDirection;
		pPacket->WriteValue(nValue);		
		pPacket->WriteValue(m_dGlassThickness);
		pPacket->WriteValue(m_nSelectCellOri);

		m_pRcpInfoPrm.WriteRecipe(pPacket);
		m_pRefInspectPrm.WriteRecipe(pPacket);
		m_pTransInspectPrm.WriteRecipe(pPacket);
		m_pEdgeCrack.WriteRecipe(pPacket);
		m_pGlassAlignParm.WriteRecipe(pPacket);
		m_pGlassEtcParm.WriteRecipe(pPacket);
		return TRUE;
	}
};

#endif