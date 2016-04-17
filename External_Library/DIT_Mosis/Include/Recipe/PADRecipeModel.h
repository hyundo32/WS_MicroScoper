// PADRecipeModel.h: interface for the CPADRecipeModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PADRECIPEMODEL_H__C5F01CA9_8FD3_4EC1_89FF_A0935BA7CB06__INCLUDED_)
#define AFX_PADRECIPEMODEL_H__C5F01CA9_8FD3_4EC1_89FF_A0935BA7CB06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PADRegion.h"

typedef struct _PADMODELGENPARM
{	
	CPoint	m_nImgCutCellidx;	
	int		m_nImgCutSizeLeft;
	int		m_nImgCutSizeTop;
	int		m_nImgCutSizeRight;
	int		m_nImgCutSizeBottom;
	int		m_nImgCutOption;		// 0x01:Left, 0x02:Top,0x04:Right,0x08:Bottom	

	_PADMODELGENPARM()
	{
		Reset();
	}
	void Reset()
	{
		m_nImgCutCellidx = CPoint(0,0);
		m_nImgCutSizeLeft = 0;
		m_nImgCutSizeTop = 0;
		m_nImgCutSizeRight = 0;
		m_nImgCutSizeBottom = 0;
		m_nImgCutOption = -1;	
	}
	_PADMODELGENPARM& operator=(const _PADMODELGENPARM &rh)
	{	
		m_nImgCutCellidx = rh.m_nImgCutCellidx;
		m_nImgCutSizeLeft = rh.m_nImgCutSizeLeft;
		m_nImgCutSizeTop = rh.m_nImgCutSizeTop;
		m_nImgCutSizeRight = rh.m_nImgCutSizeRight;
		m_nImgCutSizeBottom = rh.m_nImgCutSizeBottom;
		m_nImgCutOption = rh.m_nImgCutOption;

		return *this;
	}
} PADMODELGENPARM, *pPADMODELGENPARM;

class AFX_EXT_CLASS CPADRecipeModel
{
public:
	CPADRecipeModel();
	virtual ~CPADRecipeModel();
	CPADRecipeModel(CPADRecipeModel& rhs);
	CPADRecipeModel& operator=(CPADRecipeModel& rhs);
	
	
public:
	BOOL	ReadPADModelRecipe(int iModel,CHMRegiConfig* pFile);
	BOOL	WritePADModelRecipe(int iModel,CHMRegiConfig* pFile);
	BOOL	ReadRecipe(CMosisPacket *pPacket);
	BOOL	WriteRecipe(CMosisPacket *pPacket);

	void	ResetModel();
	
	pPADMODELGENPARM	GetModelGenParm(){return &m_pPADModelGenParm;}
	void	SetModelGenParm(PADMODELGENPARM pParm){m_pPADModelGenParm=pParm;}

	CPADRegionInfo	*GetRegionInfo(){return &m_pPADRegionInfo;}
	CPADParmC2C		*GetC2CInfo(){return &m_pPADC2CParm;}
	
protected:
	CPADRegionInfo	m_pPADRegionInfo;
	PADMODELGENPARM	m_pPADModelGenParm;

	CPADParmC2C		m_pPADC2CParm;
	
};

#endif // !defined(AFX_PADRECIPEMODEL_H__C5F01CA9_8FD3_4EC1_89FF_A0935BA7CB06__INCLUDED_)
