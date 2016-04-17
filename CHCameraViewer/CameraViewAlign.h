#pragma once

#include "CHRectTracker.h"
#include "CameraView.h"
// CCameraViewAlign

class AFX_EXT_CLASS CCameraViewAlign : public CCameraView
{
	DECLARE_DYNAMIC(CCameraViewAlign)

public:
	CCameraViewAlign(int nViewIndex=0, CWnd* pParentWnd=NULL);
	virtual ~CCameraViewAlign();

	void	SetDrawROIRect(BOOL bDraw);
	void	SetTrackerRect(const CRect& rtRect);
	BOOL	GetTrackerRect(CRect& rtRect);
	void	ClearTrackerRect();


protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnROIRect();
	afx_msg void OnClearROIRect();
	afx_msg void OnSaveROIRect();
	afx_msg void OnChangeView();
	
protected:
	void	DrawROIRect(CDC *pDC);
	virtual void PopUpCommandMenu(const CPoint& point);


protected:	
	BOOL			m_bDrawROIRect;
	CRect			m_rtROIRect;

	// tracker
	BOOL			m_bDrawTracker;
	CPoint			m_ptTrackerStart;
	CCHRectTracker	m_rectTracker;
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


