#pragma once
#include "afxext.h"


enum CHStyleFlags
{
//	solidLine = 1, dottedLine = 2, hatchedBorder = 4,
//	resizeInside = 8, resizeOutside = 16, hatchInside = 32,

	crossLine = 64, xLine = 128,
};


class AFX_EXT_CLASS CCHRectTracker : public CRectTracker
{
public:
	CCHRectTracker();
	CCHRectTracker(LPCRECT lpSrcRect, UINT nStyle);
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo=NULL);
	BOOL TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert);
	void Draw(CDC* pDC) const;
	virtual ~CCHRectTracker(void);

protected:
	BOOL TrackHandle(int nHandle, CWnd* pWnd, CPoint point, CWnd* pWndClipTo);
};
