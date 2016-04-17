// PADRecipe.h: interface for the CPADRecipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PADRECIPE_H__928151D6_F446_4542_A3C5_F70BC2C255B2__INCLUDED_)
#define AFX_PADRECIPE_H__928151D6_F446_4542_A3C5_F70BC2C255B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PADRecipeModel.h"

const int PAD_LIGHTSETTING_MAXCOUNT = 4;

typedef struct _PADLightSetting
{
	enum emLightEvalMode{MODE_AVERAGE=0,MODE_MIN,MODE_MAX,MODE_NONE};

	emLightEvalMode emLightMode;
	int				nSetValue;
	int				nMinValue;
	int				nMaxValue;

	_PADLightSetting()
	{
		Reset();
	}	
	void Reset()
	{
		emLightMode = MODE_NONE;
		nSetValue = 0;
		nMinValue = 0;
		nMaxValue = 0;
	}
	_PADLightSetting& operator=(const _PADLightSetting &rh)
	{		
		emLightMode = rh.emLightMode;
		nSetValue = rh.nSetValue;
		nMinValue = rh.nMinValue;
		nMaxValue = rh.nMaxValue;
		return *this;
	}
} PADLightSetting, *pPADLightSetting;

typedef struct _PADGENPARM
{
	BOOL					m_bUseInspect;
	BOOL					m_bSavePADImage;
	BOOL					m_bUsePRNU;
	int						m_nSmarLEDVolume;
	int						m_nCaliCoef;
	double					m_dCameraGain;	
	BOOL					m_bUsePADResultFile;	

	int						m_nInspectLeft_T_Margin;
	int						m_nInspectLeft_R_Margin;
	int						m_nInspectLeft_B_Margin;
	int						m_nInspectLeft_Width;	
	int						m_nInspectTop_L_Margin;
	int						m_nInspectTop_B_Margin;
	int						m_nInspectTop_R_Margin;
	int						m_nInspectTop_Height;
	int						m_nInspectRight_T_Margin;
	int						m_nInspectRight_R_Margin;
	int						m_nInspectRight_B_Margin;
	int						m_nInspectRight_Width;
	int						m_nInspectBottom_L_Margin;
	int						m_nInspectBottom_B_Margin;
	int						m_nInspectBottom_R_Margin;
	int						m_nInspectBottom_Height;
	
	_PADGENPARM()
	{
		Reset();
	}
	void Reset()
	{
		m_bUseInspect = FALSE;
		m_bSavePADImage = FALSE;
		m_bUsePRNU	= FALSE;		
		m_nSmarLEDVolume = 0;
		m_nCaliCoef = 0;
		m_dCameraGain = 0;
		m_bUsePADResultFile = FALSE;
		m_nInspectLeft_T_Margin = m_nInspectLeft_R_Margin = m_nInspectLeft_B_Margin = m_nInspectLeft_Width = 0;		
		m_nInspectRight_T_Margin = m_nInspectRight_R_Margin = m_nInspectRight_B_Margin = m_nInspectRight_Width = 0;		
		m_nInspectTop_L_Margin = m_nInspectTop_B_Margin = m_nInspectTop_R_Margin = m_nInspectTop_Height = 0;		
		m_nInspectBottom_L_Margin = m_nInspectBottom_B_Margin = m_nInspectBottom_R_Margin = m_nInspectBottom_Height = 0;		
	}
	_PADGENPARM& operator=(const _PADGENPARM &rh)
	{
		m_bUseInspect = rh.m_bUseInspect;	
		m_bSavePADImage = rh.m_bSavePADImage;	
		m_bUsePRNU = rh.m_bUsePRNU;	
		m_nSmarLEDVolume = rh.m_nSmarLEDVolume;	
		m_nCaliCoef = rh.m_nCaliCoef;	
		m_dCameraGain = rh.m_dCameraGain;	
		m_bUsePADResultFile = rh.m_bUsePADResultFile;			

		m_nInspectLeft_T_Margin = rh.m_nInspectLeft_T_Margin;	
		m_nInspectLeft_R_Margin = rh.m_nInspectLeft_R_Margin;	
		m_nInspectLeft_B_Margin = rh.m_nInspectLeft_B_Margin;	
		m_nInspectLeft_Width = rh.m_nInspectLeft_Width;	
		m_nInspectRight_T_Margin = rh.m_nInspectRight_T_Margin;	
		m_nInspectRight_R_Margin = rh.m_nInspectRight_R_Margin;	
		m_nInspectRight_B_Margin = rh.m_nInspectRight_B_Margin;	
		m_nInspectRight_Width = rh.m_nInspectRight_Width;	
		m_nInspectTop_L_Margin = rh.m_nInspectTop_L_Margin;	
		m_nInspectTop_B_Margin = rh.m_nInspectTop_B_Margin;	
		m_nInspectTop_R_Margin = rh.m_nInspectTop_R_Margin;	
		m_nInspectTop_Height = rh.m_nInspectTop_Height;	
		m_nInspectBottom_L_Margin = rh.m_nInspectBottom_L_Margin;	
		m_nInspectBottom_B_Margin = rh.m_nInspectBottom_B_Margin;	
		m_nInspectBottom_R_Margin = rh.m_nInspectBottom_R_Margin;	
		m_nInspectBottom_Height = rh.m_nInspectBottom_Height;			
		return *this;
	}
} PADGENPARM, *pPADGENPARM;

#define PAD_FILENAME_STRING "PAD"
#define PAD_RECIPE_PATH "C:\\검사설비\\RecipeNew\\"

class AFX_EXT_CLASS CPADRecipe  
{
public:
	CPADRecipe();
	virtual ~CPADRecipe();
	CPADRecipe(CPADRecipe& rhs);
	CPADRecipe& operator=(CPADRecipe& rhs);

public:
	void					SetRecipeName(CString strRecipeName);
	BOOL					ReadRecipeFile();
	BOOL					WriteRecipeFile(BOOL bCheck=TRUE);
	BOOL					ReadModelRecipeFile();
	BOOL					ReadRecipe(CMosisPacket *pPacket);
	BOOL					WriteRecipe(CMosisPacket *pPacket);

	void					ResetPADRecipe();	
	BOOL					SetPADModel(int iModel,CPADRecipeModel *pModel);
	CPADRecipeModel*		GetPADModel(int iModel);

	int						GetPADModelCount(){return m_nPADModelCount;}
	void					SetModelCount(int nCount);

	pPADLightSetting		GetLightSetting(int iSide);
	BOOL					SetLightSetting(int iSide,PADLightSetting stLight);

	pPADGENPARM				GetGenParm(){return &m_pPADGenParm;}
	void					SetGenParm(PADGENPARM pParm){m_pPADGenParm=pParm;}

protected:
	void					ResetModelList();

protected:	
	CHMRegiConfig			m_PADRecipeFile;
	CString					m_strPADRecipeFileName;
	CPADRecipeModel			*m_pPADModel;	
	int						m_nPADModelCount;
	PADLightSetting			m_pPADLightSetting[PAD_LIGHTSETTING_MAXCOUNT];
	PADGENPARM				m_pPADGenParm;

};

#endif // !defined(AFX_PADRECIPE_H__928151D6_F446_4542_A3C5_F70BC2C255B2__INCLUDED_)
