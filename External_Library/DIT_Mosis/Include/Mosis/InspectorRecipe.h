// InspectorRecipe.h: interface for the CInspectorRecipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTORRECIPE_H__4156C9C1_99FF_4285_8A50_6EABAA26F46A__INCLUDED_)
#define AFX_INSPECTORRECIPE_H__4156C9C1_99FF_4285_8A50_6EABAA26F46A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "GlassRecipe.h"
#include "SystemRecipe.h"
#include "ScanRecipe.h"




class AFX_EXT_CLASS IInspectorRecipe
{

protected:
	CSystemRecipe			m_SystemRecipe;
	CGlassRecipe			m_GlassRecipe;
	CScanRecipe				m_ScanRecipe;


	CMosisPacket				m_ReadPacket;
	CMosisPacket				m_WritePacket;


public:
	CSystemRecipe	*GetSystemRecipe()				{return &m_SystemRecipe;}
	CGlassRecipe	*GetGlassRecipe()				{return &m_GlassRecipe;}
	CScanRecipe		*GetScanRecipe()				{return &m_ScanRecipe;}

public:
	// pData에 있는 내용을 Packet에 copy하고 각 내용을 읽는다.
	BOOL			ReadInspectorRecipe(BYTE* pData, int size, int iPacket);
	BOOL			MakeInspectorRecipe(int iScan);
	BOOL			Test_InspectorRecipe();


public:
};

#endif // !defined(AFX_INSPECTORRECIPE_H__4156C9C1_99FF_4285_8A50_6EABAA26F46A__INCLUDED_)
