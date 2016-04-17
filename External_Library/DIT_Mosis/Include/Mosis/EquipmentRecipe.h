// EquipmentRecipe.h: interface for the CEquipmentRecipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQUIPMENTRECIPE_H__8696E880_9A6E_48E2_9D51_D756B352C8B1__INCLUDED_)
#define AFX_EQUIPMENTRECIPE_H__8696E880_9A6E_48E2_9D51_D756B352C8B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EquipmentData.h"


// ��� ����... (���� �⺻ ����)
class AFX_EXT_CLASS CEquipmentRecipe
{
protected:
	// �˻�� ����
	// �۾��ڰ� �� ��ĵ����, Scan���� ��ǥ����.
	// �۶� ����.
	// �۶� �β�.
	CGlassDataHive		m_GlassDataHive;
	int					m_iGlassData;
	CGlassData			m_GlassData;

public:
	CEquipmentRecipe();
	virtual ~CEquipmentRecipe();

};

#endif // !defined(AFX_EQUIPMENTRECIPE_H__8696E880_9A6E_48E2_9D51_D756B352C8B1__INCLUDED_)
