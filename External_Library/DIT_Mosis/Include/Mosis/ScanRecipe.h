// ScanRecipe.h: interface for the CScanRecipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCANRECIPE_H__EAC0D9E3_4298_453A_90FD_0E4FC7C53682__INCLUDED_)
#define AFX_SCANRECIPE_H__EAC0D9E3_4298_453A_90FD_0E4FC7C53682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ScanModel.h"
#include "SystemRecipe.h"


// 스캔 정보,
class AFX_EXT_CLASS CScanData2 : public stScanData
{
public:
	double			m_ScanResolution;	// 모터 이동축 해상도. CSystemData에서 물려 받는다. 인스펙터에서 사용할때..

public:
	CScanData2&	operator=(stScanData& System)		{SelfScanData()= System;		return *this;}
	double			GetLeft()							{return m_AccumCoord;}
	double			GetRight(int frameWidth)			{return GetLeft()+ (frameWidth- m_LeftMargin- m_RightMargin)*m_ConvResolution;}

	friend class CScanModel;
};


// 스캔정보, 스캔별 검사 방법+ 영역 정보.
class AFX_EXT_CLASS CScanRecipe
{
public:
	int				m_iCamera;			// 카메라 index
	int				m_iScan;			// 스켄 index

	stSystemData	m_SystemData;		// 카메라 정보.
	CScanData2		m_ScanData;			// 스켄 정보.
protected:
	CScanModelHive	m_ScanModelHive;	// 검사 영역 정보.

public:
	BOOL			SetScanModelCnt(int nModel, BOOL bAppend)		{return m_ScanModelHive.SetSize(nModel, bAppend);}
	int				GetScanModelCnt()								{return m_ScanModelHive.GetSize();}
	CScanModel		*GetScanModel(int iModel)						{return m_ScanModelHive.GetData(iModel);}

public:
	BOOL			MakeScanRecipe(int iCamera, int iScan, CGlassRecipe &GlassRecipe, CSystemRecipe &SystemRecipe);
};
typedef				CMosisHive2<CScanRecipe>		CScanRecipeHive2;



#endif // !defined(AFX_SCANRECIPE_H__EAC0D9E3_4298_453A_90FD_0E4FC7C53682__INCLUDED_)
