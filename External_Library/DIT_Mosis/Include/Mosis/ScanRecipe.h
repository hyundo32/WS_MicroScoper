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


// ��ĵ ����,
class AFX_EXT_CLASS CScanData2 : public stScanData
{
public:
	double			m_ScanResolution;	// ���� �̵��� �ػ�. CSystemData���� ���� �޴´�. �ν����Ϳ��� ����Ҷ�..

public:
	CScanData2&	operator=(stScanData& System)		{SelfScanData()= System;		return *this;}
	double			GetLeft()							{return m_AccumCoord;}
	double			GetRight(int frameWidth)			{return GetLeft()+ (frameWidth- m_LeftMargin- m_RightMargin)*m_ConvResolution;}

	friend class CScanModel;
};


// ��ĵ����, ��ĵ�� �˻� ���+ ���� ����.
class AFX_EXT_CLASS CScanRecipe
{
public:
	int				m_iCamera;			// ī�޶� index
	int				m_iScan;			// ���� index

	stSystemData	m_SystemData;		// ī�޶� ����.
	CScanData2		m_ScanData;			// ���� ����.
protected:
	CScanModelHive	m_ScanModelHive;	// �˻� ���� ����.

public:
	BOOL			SetScanModelCnt(int nModel, BOOL bAppend)		{return m_ScanModelHive.SetSize(nModel, bAppend);}
	int				GetScanModelCnt()								{return m_ScanModelHive.GetSize();}
	CScanModel		*GetScanModel(int iModel)						{return m_ScanModelHive.GetData(iModel);}

public:
	BOOL			MakeScanRecipe(int iCamera, int iScan, CGlassRecipe &GlassRecipe, CSystemRecipe &SystemRecipe);
};
typedef				CMosisHive2<CScanRecipe>		CScanRecipeHive2;



#endif // !defined(AFX_SCANRECIPE_H__EAC0D9E3_4298_453A_90FD_0E4FC7C53682__INCLUDED_)
