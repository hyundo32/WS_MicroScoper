// CHTrackerView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CHTrackerView.h"
#include "CHBufferDC.h"

// CCHTrackerView

IMPLEMENT_DYNAMIC(CCHTrackerView, CCHImageView)

CCHTrackerView::CCHTrackerView() : CCHImageView()
{
	m_rectTracker.m_rect = CRect(0,0,0,0);
	m_rectTracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeOutside;
	m_rectTracker.m_sizeMin = CSize(8,8);
	m_bDrawTracker = FALSE;
}


CCHTrackerView::CCHTrackerView(CWnd *pParentWnd) : CCHImageView(pParentWnd)
{
	m_rectTracker.m_rect = CRect(0,0,0,0);
	m_rectTracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeOutside;
	m_rectTracker.m_sizeMin = CSize(8,8);
	m_bDrawTracker = FALSE;
}

CCHTrackerView::~CCHTrackerView()
{

}


BEGIN_MESSAGE_MAP(CCHTrackerView, CCHImageView)
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CCHTrackerView 메시지 처리기입니다.

BOOL CCHTrackerView::GetTrackerRect(CRect& rtRect)
{
	if (m_rectTracker.m_rect.Width()<0 || m_rectTracker.m_rect.Height()<0)
	{
		return FALSE;
	}

	rtRect = m_rectTracker.m_rect;

	return TRUE;
}

void CCHTrackerView::SetTrackerRect(const CRect& rtRect)
{
	m_rectTracker.m_rect = rtRect;
	Invalidate(TRUE);
}

void CCHTrackerView::ClearTrackerRect()
{
	m_rectTracker.m_rect = CRect(0,0,0,0);
	Invalidate(TRUE);
}

BOOL CCHTrackerView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_rectTracker.SetCursor(this, nHitTest))
		return TRUE;

	return CCHImageView::OnSetCursor(pWnd, nHitTest, message);
}

void CCHTrackerView::OnPaint()
{

	CCHBufferDC *pDC = new CCHBufferDC(this); // device context for painting

	UpdateImageView(pDC);

	m_rectTracker.m_rect.NormalizeRect();

	if (m_rectTracker.m_rect.left>0 && m_rectTracker.m_rect.right<GetWidth())
	{
		if (m_rectTracker.m_rect.top>0 && m_rectTracker.m_rect.bottom<GetHeight())
		{
			m_rectTracker.Draw(pDC);
		}
	}

	delete pDC;
	
}

void CCHTrackerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_pParentWnd==NULL || !GetImageExist()) return;

	if (point.x > -1 && point.x < GetScaleWidth() &&
		point.y > -1 && point.y < GetScaleHeight() )
	{
		if (m_rectTracker.HitTest(point) < 0)
		{
			// just to demonstrate CRectTracker::TrackRubberBand
			if (m_rectTracker.TrackRubberBand(this, point, TRUE))
			{
				Invalidate();
			}
		}
		else if (m_rectTracker.Track(this, point, TRUE))
		{
			Invalidate();
		}
	}

	CCHImageView::OnLButtonDown(nFlags, point);
}

