// EquipmentData.h: interface for the CEquipmentData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQUIPMENTDATA_H__DD9AF0E9_213B_40AA_B2F3_47EA7312DF44__INCLUDED_)
#define AFX_EQUIPMENTDATA_H__DD9AF0E9_213B_40AA_B2F3_47EA7312DF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MosisHive.h"

class AFX_EXT_CLASS CGlassData
{
	int		m_GlassThick;
	int		m_GlassWidth;
	int		m_GlassHeight;
};
typedef CMosisHive1<CGlassData>	CGlassDataHive;

#endif // !defined(AFX_EQUIPMENTDATA_H__DD9AF0E9_213B_40AA_B2F3_47EA7312DF44__INCLUDED_)
