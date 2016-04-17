// EquipmentRecipe.h: interface for the CEquipmentRecipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQUIPMENTRECIPE_H__8696E880_9A6E_48E2_9D51_D756B352C8B1__INCLUDED_)
#define AFX_EQUIPMENTRECIPE_H__8696E880_9A6E_48E2_9D51_D756B352C8B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EquipmentData.h"


// 장비 설정... (설비 기본 설정)
class AFX_EXT_CLASS CEquipmentRecipe
{
protected:
	// 검사기 구분
	// 작업자가 본 스캔방향, Scan축의 좌표방향.
	// 글라스 종류.
	// 글라스 두께.
	CGlassDataHive		m_GlassDataHive;
	int					m_iGlassData;
	CGlassData			m_GlassData;

public:
	CEquipmentRecipe();
	virtual ~CEquipmentRecipe();

};

#endif // !defined(AFX_EQUIPMENTRECIPE_H__8696E880_9A6E_48E2_9D51_D756B352C8B1__INCLUDED_)
