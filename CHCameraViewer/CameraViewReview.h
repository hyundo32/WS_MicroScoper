#pragma once

#include "CHRectTracker.h"
#include "CameraView.h"
// CCameraViewReview

class AFX_EXT_CLASS CCameraViewReview : public CCameraView
{
	DECLARE_DYNAMIC(CCameraViewReview)

public:
	CCameraViewReview(int nViewIndex=0, CWnd* pParentWnd=NULL);
	virtual ~CCameraViewReview();

	void	SetDrawROIRect(BOOL bDraw);
	void	SetDrawDefectPos(BOOL bDraw);
	void	SetDrawAoiDefect(BOOL bDraw);
	
	void	SetTrackerRect(const CRect& rtRect);
	BOOL	GetTrackerRect(CRect& rtRect);
	void	ClearTrackerRect();
	void	SetDefectPos(const CPoint& ptPoint);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	
	afx_msg void OnSaveAsImage();
	afx_msg void OnChangeView();
	afx_msg void OnROIRect();
	afx_msg void OnAoiDefect();
	afx_msg void OnDefectPos();
	afx_msg void OnClearROIRect();
	afx_msg void OnSaveROIRect();

protected:
	void	DrawROIRect(CDC *pDC);
	void	DrawDefectPos(CDC *pDC);
	void	DrawAoiDefect(CDC *pDC);
	virtual void PopUpCommandMenu(const CPoint& point);
	
protected:
	BOOL			m_bDrawROIRect;
	CRect			m_rtROIRect;
	SPixelInfo		m_sROIInfo;

	BOOL			m_bDrawDefectPos;
	CPoint			m_ptDefectPos;

	BOOL			m_bDrawAoiDefect;

	// tracker
	BOOL			m_bDrawTracker;
	CPoint			m_ptTrackerStart;
	CCHRectTracker	m_rectTracker;

	CCHImageData	m_AoiImageData;
};


