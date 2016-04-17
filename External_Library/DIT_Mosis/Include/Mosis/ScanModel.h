// ScanModel.h: interface for the CScanModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCANMODEL_H__4496825C_3AC9_4CB6_885C_EFC54FB0C3F1__INCLUDED_)
#define AFX_SCANMODEL_H__4496825C_3AC9_4CB6_885C_EFC54FB0C3F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MosisHive.h"
#include "SystemRecipe.h"
#include "GlassRecipe.h"



#define FRAME_HEIGHT_TEMP 1024

class	CScanCellMosis
{
public:
	CPoint	LeftTop, RightTop;
	CPoint	LeftBottom, RightBottom;
	int		m_StartFrame, m_EndFrame;
	int		m_FrameHeight;

	double	a, bLeft, bRight;

public:
	CScanCellMosis()
	{
		ResetScanCell();
	}

	void SetCellData(int frameHeight, CPoint &lt, CPoint &lb, CPoint &rt, CPoint &rb)
	{
		m_FrameHeight= frameHeight;
		LeftTop= lt;
		LeftBottom= lb;
		RightTop= rt;
		RightBottom= rb;

		double dx= LeftBottom.x- LeftTop.x;
		double dy= LeftBottom.y- LeftTop.y;

		if(dx == 0)
		{
			a= 0;
			bLeft= LeftTop.x;
		}else
		{
			a= dx/dy;
			bLeft= -a*LeftTop.y+ LeftTop.x;
			bRight= -a*RightTop.y+ RightTop.x;
		}

		m_StartFrame= lt.y/frameHeight;
		m_EndFrame= lb.y/frameHeight;
	}

	void	ResetScanCell()
	{
		m_StartFrame= -1;// StartFrame�� ���� ��꿡 Ư���� �ǹ̸� ���´�. ���� �Ǵܰ����� ���.
	}

	// �۶� ƿƮ, Cell Start, End, Left, Right���� ����Ҷ�.. Start & End Frame�� �������� ���� Ÿ���ϰڴ�.
	// y�����δ� Cell Top �� Bottom ���̿� ����������, Left & Right ������ �ִٰ� ������ ���� �ִ�(ƿƮ ����)
	// Left, Right, Top, Bottom���� ���� ���꺸�� Frame Index ������ ������.
	BOOL	GetActiveScanRect(int iFrame, CRect &rectFrame, CRect &rectCal)
	{
		if(iFrame < m_StartFrame)	return FALSE;
		if(iFrame > m_EndFrame)	return FALSE;

		rectCal= rectFrame;
		rectCal.left= a*rectFrame.top+ bLeft;
		rectCal.right= a*rectFrame.top+ bRight;

		if(rectCal.left < rectFrame.left)
			rectCal.left= rectFrame.left;
		if(rectCal.right > rectFrame.right)
			rectCal.right= rectFrame.right;
		if(rectFrame.top < LeftTop.y)
			rectCal.top= LeftTop.y;
		if(rectFrame.bottom > LeftBottom.y)
			rectCal.bottom= LeftBottom.y;
		return TRUE;
	}
	void Test(CRect rect, int tilt)
	{
		LeftTop.x= rect.left;
		LeftTop.y= rect.top;
		RightTop.x= rect.right;
		RightTop.y= rect.top;
		LeftBottom.x= rect.left- tilt;
		LeftBottom.y= rect.bottom;
		RightBottom.x= rect.right- tilt;
		RightBottom.y= rect.bottom;
		SetCellData(FRAME_HEIGHT_TEMP, LeftTop, LeftBottom, RightTop, RightBottom);
	}
};


class CScanData2;

// �𵨺� �˻� ���, ����
class AFX_EXT_CLASS CScanModel : public CModelData
{
protected:
	CCellHive2		m_CellHive2;

public:
	BOOL			MakeScanModel(CGlassModel *pModel, CScanData2 &ScanData, int FrameWidth);
	void			SetGlassStart(int y);
	void			SetCellStart(int iy, int offset);

public:
	BOOL			SetScanCellCnt(int nx, int ny)			{return m_CellHive2.SetSize(nx, ny);}
	int				GetScanCellWidth()						{return m_CellHive2.GetWidth();}
	int				GetScanCellHeight()						{return m_CellHive2.GetHeight();}
	CCell			*GetScanCell(int x, int y)				{return m_CellHive2.GetData(x, y);}

};
typedef				CMosisHive1<CScanModel>		CScanModelHive;





#endif // !defined(AFX_SCANMODEL_H__4496825C_3AC9_4CB6_885C_EFC54FB0C3F1__INCLUDED_)
