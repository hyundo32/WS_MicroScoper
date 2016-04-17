#pragma once

#include <list>
#include "CHImageData.h"
#include "CHRectTracker.h"

namespace CHImageControls
{
	enum DrawType			{ DrawOrigin=0, DrawScale, DrawFit };
	enum ObjectType			{ ObjectLine=1, ObjectRectangle=2, ObjectCircle=4, ObjectText=8, ObjectTextRect=16 };
}

class AFX_EXT_CLASS CCHDrawObject
{
public:
	CCHDrawObject() { Reset(); }

	CCHDrawObject(int left, int top, int right, int bottom)
	{
		Reset();
		nLeft			= left;
		nTop			= top;
		nRight			= right;
		nBottom			= bottom;
	}

	void Reset()
	{
		nLeft			= 0;
		nTop			= 0;
		nRight			= 0;
		nBottom			= 0;
		nObjectType		= 1;	
		nForeColor		= RGB(255,0,0);
		nBackColor		= NULL_BRUSH;
		nSize			= 1;
		nTextWidth		= 8;
		nTextHeight		= 16;
		strTextString	= _T("");
	}

	int GetWidth()		{ return (nRight-nLeft)+1; }
	int GetHeight()		{ return (nBottom-nTop)+1; }
	int GetCenterX()	{ return nLeft + (GetWidth()/2); }
	int GetCenterY()	{ return nLeft + (GetHeight()/2); }

	int			nObjectType;
	int			nLeft;
	int			nTop;
	int			nRight;
	int			nBottom;
	DWORD		nForeColor;
	DWORD		nBackColor;
	int			nSize;

	int			nTextWidth;
	int			nTextHeight;
	CString		strTextString;

};
typedef std::list<CCHDrawObject>				ListDrawObject;
typedef std::list<CCHDrawObject>::iterator		ListDrawObjectIt;

class AFX_EXT_CLASS CCHImageView : public CWnd, public CCHImageData
{
	DECLARE_DYNAMIC(CCHImageView)
public:
	CCHImageView();
	CCHImageView(CWnd *pParentWnd);
	virtual ~CCHImageView();

	void	Reset();
	
	// getter
	double	GetWidthScale(void);
	double	GetHeightScale(void);
	int		GetScaleWidth(void);
	int		GetScaleHeight(void);
	int		GetHScrollPos(void);
	int		GetVScrollPos(void);

	// setter
	void	SetDrawObject(BOOL bDraw);
	void	SetDrawType(int nDrawType);
	void	SetViewName(const CString strViewName);
	void	SetParentWnd(CWnd* pParentWnd);
	void	SetDrawScale(double dScale);	

	BOOL	LoadImage(const CString& strFilename);

	void	AddDrawObject(const CCHDrawObject& drawObject);
	void	DeleteAllDrawObject(void);
	void	DeleteAllDrawObject(int nType);
	void	DeleteTailDrawObject(void);
	void	DeleteDrawObject(const CRect& rtRect);

	CCHDrawObject * GetDrawObject(UINT nIndex);
	
protected:
	DECLARE_MESSAGE_MAP()

protected:
	void	DrawObject(CDC *pDC, CCHDrawObject *pObject);
	void	DrawObjectList(CDC *pDC);
	void	DrawText(CDC *pDC, int nLeft, int nTop, int nRight, int nBottom, double dScaleX, double dScaleY, CCHDrawObject *pObject);
	int		PointInDrawObject(CPoint point, CRect& rtResult);
	void	UpdateImageView(CDC *pDC);

	CWnd		*m_pParentWnd;
	CString		m_strViewName;

	// pre draw info
	CRect		m_rtClientRect;
	int			m_nScaleWidth;
	int			m_nScaleHeight;

	// Scroll Pos
	int			m_nVScroll;
	int			m_nHScroll;

	// Max Scroll Pos
	int			m_nMaxVScroll;
	int			m_nMaxHScroll;


	// image draw
	double			m_dWidthScale; 
	double			m_dHeightScale;
	int				m_nDrawType;
	BOOL			m_bDrawObject;
	
	ListDrawObject	m_ListDrawObject;

public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


