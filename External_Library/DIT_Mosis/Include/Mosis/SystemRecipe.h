// SystemRecipe.h: interface for the CSystemRecipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMRECIPE_H__45412082_78E1_431A_85D2_7458A8265E70__INCLUDED_)
#define AFX_SYSTEMRECIPE_H__45412082_78E1_431A_85D2_7458A8265E70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SystemData.h"


// 검사 시스템 [투과, 반사, 이면등..]
// 카메라 정보, 스캔 정보, 조명 정보.
class AFX_EXT_CLASS CSystemRecipe
{
protected:
	stSystemData		m_SystemData;
	CScanDataHive2		m_ScanDataHive2;// m_nCamera * m_nScan

public:
	CSystemRecipe();
	virtual ~CSystemRecipe();

public:
	stSystemData	*GetSystemData()							{return &m_SystemData;}
public:
	BOOL			SetScanDataCnt(int nCam, int nScan)			{return m_ScanDataHive2.SetSize(nCam, nScan);}
	int				GetCameraCnt()								{return m_ScanDataHive2.GetWidth();}
	int				GetScanCnt()								{return m_ScanDataHive2.GetHeight();}
	stScanData		*GetScanData(int iCam, int iScan)			{return m_ScanDataHive2.GetData(iCam, iScan);}


public:
	BOOL			ReadSystemRecipe(CMosisPacket *pPacket, int iCamera);
	BOOL			WriteSystemRecipe(CMosisPacket *pPacket, int iCamera);

	void			Test_FillSystemRecipe();

friend				class IInspectorRecipe;
};
typedef				CMosisHive1<CSystemRecipe>		CSystemRecipeHive;



#endif // !defined(AFX_SYSTEMRECIPE_H__45412082_78E1_431A_85D2_7458A8265E70__INCLUDED_)
