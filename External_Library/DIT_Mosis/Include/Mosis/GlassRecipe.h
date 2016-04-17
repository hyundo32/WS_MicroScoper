// GlassRecipe.h: interface for the CGlassRecipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLASSRECIPE_H__A4A5EE14_6B2E_45BD_9384_07F5AE5DFD2D__INCLUDED_)
#define AFX_GLASSRECIPE_H__A4A5EE14_6B2E_45BD_9384_07F5AE5DFD2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GlassModel.h"

// 검사 방법, 영역 정보
class AFX_EXT_CLASS CGlassRecipe
{
protected:
	int					m_ModelCnt;
	CGlassModelHive		m_GlassModelHive;

	CPoint				m_FirstAlign;
	CPoint				m_SecondAlign;

public:
	BOOL				SetGlassModelCnt(int nModel, BOOL bAppend)		{return m_GlassModelHive.SetSize(nModel, bAppend);}
	int					GetGlassModelCnt()								{return m_GlassModelHive.GetSize();}
	CGlassModel			*GetGlassModel(int iModel)						{return m_GlassModelHive.GetData(iModel);}
public:
	void				Change2PixelCoord(double x, double y);
	BOOL				MakeGlassRecipe();
	BOOL				Test_FillGlassRecipe();

	BOOL				WriteGlassRecipe(CMosisPacket *pPacket);
	BOOL				ReadGlassRecipe(CMosisPacket *pPacket);
};


#endif // !defined(AFX_GLASSRECIPE_H__A4A5EE14_6B2E_45BD_9384_07F5AE5DFD2D__INCLUDED_)
