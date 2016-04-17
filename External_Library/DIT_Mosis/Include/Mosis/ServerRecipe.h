// ServerRecipe.h: interface for the CServerRecipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERRECIPE_H__80A07C8A_A529_4231_BF4E_B59A0FA9AD9E__INCLUDED_)
#define AFX_SERVERRECIPE_H__80A07C8A_A529_4231_BF4E_B59A0FA9AD9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EquipmentRecipe.h"
#include "UtilityRecipe.h"
#include "SystemRecipe.h"
#include "GlassRecipe.h"

// 검사 시스템 + 검사 방법, 검사영역 + 패킷.
class AFX_EXT_CLASS CServerRecipe
{
protected:
	int					m_SystemRecipeCnt;
	int					m_ModelCnt;
protected:
	CEquipmentRecipe	m_EquipmentRecipe;
	CUtilityRecipe		m_UtilityRecipe;

	CGlassRecipe		m_GlassRecipe;
	CSystemRecipeHive	m_SystemRecipeHive;		// 검사 시스템, 투과, 반사, 이면등....

	CMosisPacket		m_WritePacket;

public:
	BOOL				SetSystemRecipeCnt(int nSys, BOOL bAppend)			{return m_SystemRecipeHive.SetSize(nSys, bAppend);}
	int					GetSystemRecipeCnt()								{return m_SystemRecipeHive.GetSize();}
	CSystemRecipe		*GetSystemRecipe(int iSystem)						{return m_SystemRecipeHive.GetData(iSystem);};

public:
	CMosisPacket		*GetPacketHive()									{return &m_WritePacket;}
	BOOL				WriteInspectorRecipe(int iSystem, int iCamera);
};

#endif // !defined(AFX_SERVERRECIPE_H__80A07C8A_A529_4231_BF4E_B59A0FA9AD9E__INCLUDED_)
