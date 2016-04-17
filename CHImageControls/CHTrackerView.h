#pragma once


// CCHTrackerView
#include "CHImageView.h"

class AFX_EXT_CLASS CCHTrackerView : public CCHImageView
{
	DECLARE_DYNAMIC(CCHTrackerView)

public:
	CCHTrackerView();
	CCHTrackerView(CWnd *pParentWnd);
	virtual ~CCHTrackerView();

	BOOL GetTrackerRect(CRect& rtRect);
	void SetTrackerRect(const CRect& rtRect);
	void ClearTrackerRect();

protected:
	// tracker
	BOOL			m_bDrawTracker;
	CPoint			m_ptTrackerStart;
	CCHRectTracker	m_rectTracker;

protected:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


