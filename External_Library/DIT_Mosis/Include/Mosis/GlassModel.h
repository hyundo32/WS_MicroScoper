// GlassModel.h: interface for the CGlassModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLASSMODEL_H__4BBC59B3_81D8_4E3E_A88A_46DB8EA97E64__INCLUDED_)
#define AFX_GLASSMODEL_H__4BBC59B3_81D8_4E3E_A88A_46DB8EA97E64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MosisHive.h"
#include "ModelCoord.h"
#include "ModelData.h"



// Cell Rect + Cam Rect
// CRect = Cam Rect
// Original(Left, Right) => Original Cell(Left, Right)
// Glass ������ Cell�� ��ġ�� ũ�⸦ ǥ��.
// Scan Cam�� ��ȿ������ ���ԵǴ� Cell�� ��ġ�� ũ�� ǥ��.
// Scan Cam�������� ��ǥ ��ȯ�� Original left�� right�� ������ Original������ ����.
// Frame �������� Cell�ε� ǥ�� ����.
class CCell : public CRect
{
public:
	int		m_OriginalLeft;		// cell�� ����. ī�޶� �������� ������ �ƴϴ�.
	int		m_OriginalRight;	// cell�� ������. ī�޶� �������� ������ �ƴϴ�.
public:
	void Change2PixelCoord(double xR, double yR)
	{
		left= (LONG) (left/xR);
		right= (LONG) (right/xR);// /= xR;
		top= (LONG) (top/yR);///= yR;
		bottom= (LONG) (bottom/yR);///= yR;
		m_OriginalLeft= (int)(m_OriginalLeft/xR);///= xR;
		m_OriginalRight= (int)(m_OriginalRight/xR);// /= xR;
	}
	BOOL Change2ScanRect(int camLeft, int camRight)
	{
		m_OriginalLeft= left;
		m_OriginalRight= right;
		
		return IntersectX(camLeft, camRight);
	}
	BOOL HasLeftEdge()
	{
		return m_OriginalLeft == left;
	}
	BOOL HasRightEdge()
	{
		return m_OriginalRight == right;
	}
	BOOL IsIntersectX(int xLeft, int xRight)
	{
		if(left >= xRight)
			return FALSE;
		if(right <= xLeft)
			return FALSE;
		return TRUE;
	}
	BOOL IsIntersectY(int yTop, int yBottom)
	{
		if(top >= yBottom)
			return FALSE;
		if(bottom <= yTop)
			return FALSE;
		return TRUE;
	}
	BOOL IntersectX(int xLeft, int xRight)
	{
		if(left >= xRight)
			return FALSE;
		if(right <= xLeft)
			return FALSE;

		if(left < xLeft)
			left= xLeft;

		if(right > xRight)
			right= xRight;

		return TRUE;
	}
	BOOL IntersectY(int yTop, int yBottom)
	{
		if(top >= yBottom)
			return FALSE;
		if(bottom <= yTop)
			return FALSE;

		if(top < yTop)
			top= yTop;

		if(bottom > yBottom)
			bottom= yBottom;

		return TRUE;
	}
	void OffsetX(int x)
	{
		left+= x;
		right+= x;
	}
	void OffsetY(int y)
	{
		top+= y;
		bottom+= y;
	}
};
typedef			CMosisHive2<CCell>		CCellHive2;



class AFX_EXT_CLASS CGlassModel		:	public CModelCoord, public CModelData
{
protected:
	CCellHive2		m_CellHive2;

public:

public:
	BOOL			SetGlassCellCnt(int nx, int ny)			{return m_CellHive2.SetSize(nx, ny);}
	int				GetGlassCellWidth()						{return m_CellHive2.GetWidth();}
	int				GetGlassCellHeight()					{return m_CellHive2.GetHeight();}
	CCell			*GetGlassCell(int x, int y)				{return m_CellHive2.GetData(x, y);}
public:
	BOOL			MakeGlassModel();
	void			Change2PixelCoord(double x, double y);

	BOOL			ReadGlassModel(CMosisPacket *pPacket);
	BOOL			WriteGlassModel(CMosisPacket *pPacket);

friend class CScanModel;
};
typedef			CMosisHive1<CGlassModel>		CGlassModelHive;


#endif // !defined(AFX_GLASSMODEL_H__4BBC59B3_81D8_4E3E_A88A_46DB8EA97E64__INCLUDED_)
