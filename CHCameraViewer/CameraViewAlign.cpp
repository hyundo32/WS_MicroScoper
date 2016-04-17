﻿// CameraViewAlign.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CameraViewAlign.h"
#include "CHImageData.h"
#include "CHBufferDC.h"
#include <math.h>

using namespace CHImageControls;

// CCameraViewAlign
IMPLEMENT_DYNAMIC(CCameraViewAlign, CCameraView)

CCameraViewAlign::CCameraViewAlign(int nViewIndex, CWnd* pParentWnd) : CCameraView(nViewIndex, pParentWnd)
{
	m_bDrawROIRect			= TRUE;
	m_rtROIRect				= CRect(0,0,0,0);

	m_rectTracker.m_rect	= CRect(0,0,0,0);
	m_rectTracker.m_nStyle	= CRectTracker::dottedLine | CRectTracker::resizeOutside;
	m_rectTracker.m_sizeMin = CSize(8,8);
	m_bDrawTracker = FALSE;
}

CCameraViewAlign::~CCameraViewAlign()
{

}

BEGIN_MESSAGE_MAP(CCameraViewAlign, CCameraView)
	ON_COMMAND(IDR_ROI_RECT,&CCameraViewAlign::OnROIRect)
	ON_COMMAND(IDR_CLEAR_ROI_RECT,&CCameraViewAlign::OnClearROIRect)
	ON_COMMAND(IDR_SAVE_ROI_RECT,&CCameraViewAlign::OnSaveROIRect)
	ON_COMMAND(IDR_CHANGE_VIEW, &CCameraViewAlign::OnChangeView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CCameraViewAlign 메시지 처리기입니다.
void CCameraViewAlign::SetDrawROIRect(BOOL bDraw)
{
	m_bDrawROIRect = bDraw;
	Invalidate(TRUE);
}

void CCameraViewAlign::SetTrackerRect(const CRect& rtRect)
{
	m_rectTracker.m_rect = rtRect;
	Invalidate(TRUE);
}

void CCameraViewAlign::ClearTrackerRect()
{
	m_rectTracker.m_rect = CRect(0,0,0,0);
	Invalidate(TRUE);
}

BOOL CCameraViewAlign::GetTrackerRect(CRect& rtRect)
{
	if (m_rectTracker.m_rect.Width()<0 || m_rectTracker.m_rect.Height()<0)
	{
		return FALSE;
	}

	rtRect = m_rectTracker.m_rect;

	return TRUE;
}

void CCameraViewAlign::OnPaint()
{
	CCHBufferDC *pDC = new CCHBufferDC(this); // device context for painting

	UpdateView(pDC);

	if (m_bDrawROIRect) DrawROIRect(pDC);

	m_rectTracker.m_rect.NormalizeRect();
	if (m_rectTracker.m_rect.left>0 && m_rectTracker.m_rect.right<GetScaleWidth())
	{
		if (m_rectTracker.m_rect.top>0 && m_rectTracker.m_rect.bottom<GetScaleHeight())
		{
			m_rectTracker.Draw(pDC);
		}
	}

	delete pDC;
}

void CCameraViewAlign::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GetImageExist()) 
	{
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
	}

	CCameraView::OnLButtonDown(nFlags, point);
}

void CCameraViewAlign::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetTrackerRect(m_rtROIRect))
	{
// 		m_sROIInfo.nWidthPixel = m_rtROIRect.Width();
// 		m_sROIInfo.nHeightPixel = m_rtROIRect.Height();
// 		m_sROIInfo.dDiagonalPixel = sqrt(double(m_sROIInfo.nWidthPixel*m_sROIInfo.nWidthPixel + m_sROIInfo.nHeightPixel*m_sROIInfo.nHeightPixel));
// 
// 		m_sROIInfo.dWidthRealUM = m_sROIInfo.nWidthPixel * (m_dResolution / m_dWidthScale) ;
// 		m_sROIInfo.dHeightRealUM = m_sROIInfo.nHeightPixel * (m_dResolution / m_dHeightScale);
// 		m_sROIInfo.dDiagonalRealUM = sqrt((m_sROIInfo.dWidthRealUM*m_sROIInfo.dWidthRealUM)+(m_sROIInfo.dHeightRealUM*m_sROIInfo.dHeightRealUM));

		Invalidate(TRUE);
	}

	CCameraView::OnLButtonUp(nFlags, point);
}

void CCameraViewAlign::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CCameraView::OnLButtonDblClk(nFlags, point);
}

void CCameraViewAlign::OnRButtonDown(UINT nFlags, CPoint point)
{
	CCameraView::OnRButtonDown(nFlags, point);
}

void CCameraViewAlign::OnRButtonUp(UINT nFlags, CPoint point)
{
	CCameraView::OnRButtonUp(nFlags, point);
}


void CCameraViewAlign::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	CCameraView::OnRButtonDblClk(nFlags, point);
}

void CCameraViewAlign::OnMouseMove(UINT nFlags, CPoint point)
{
	CCameraView::OnMouseMove(nFlags, point);
}

void CCameraViewAlign::OnROIRect()
{
	m_bDrawROIRect = !m_bDrawROIRect;
	Invalidate(TRUE);
}

void CCameraViewAlign::OnClearROIRect()
{
	ClearTrackerRect();

	m_rtROIRect = CRect(0,0,0,0);

	Invalidate(TRUE);
}

void CCameraViewAlign::OnChangeView()
{
	if (m_pParentWnd==NULL) return;

	m_pParentWnd->SendMessage(IDR_CHANGE_VIEW);
}

void CCameraViewAlign::OnSaveROIRect()
{
	if (m_rtROIRect.Width()<1 || m_rtROIRect.Height()<1) return;

	CString szFilter = _T("All Files(*.*)|*.*||");

	CString strPath;
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	dlg.m_ofn.lpstrTitle = _T("Save Image");

	if(dlg.DoModal() == IDCANCEL) return;

	CRect rtRect;
	rtRect.left = int(double(m_rtROIRect.left) / m_dWidthScale + 0.5);
	rtRect.top = int(double(m_rtROIRect.top) / m_dHeightScale + 0.5);
	rtRect.right = int(double(m_rtROIRect.right) / m_dWidthScale + 0.5);
	rtRect.bottom = int(double(m_rtROIRect.bottom) / m_dHeightScale + 0.5);

	CCHImageData subImage;
	if (GetSubImage(rtRect, &subImage)==FALSE)
	{
		return;
	}

	if (subImage.SaveImage(dlg.GetPathName())==FALSE) return;

	return;
}


void CCameraViewAlign::DrawROIRect(CDC *pDC)
{
	if (m_rtROIRect.Width()==0 || m_rtROIRect.Height()==0) return;

	// 	CPen pen, *pOldPen;
	// 	pen.CreatePen(PS_DOT, 1, RGB(255,255,255));
	// 	pOldPen = pDC->SelectObject(&pen);
	// 	pDC->SelectStockObject(NULL_BRUSH);
	// 
	// 	pDC->Rectangle(m_rtROIRect);

	CFont font;
	VERIFY(font.CreateFont(
		15,                        // nHeight
		6,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

	// Do something with the font just created...
	CFont* def_font = pDC->SelectObject(&font);
	// 	pDC->SetTextColor(RGB(255,255,255));
	// 	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,0));
	pDC->SetBkColor(RGB(0, 0, 0));
	pDC->SetBkMode(OPAQUE);

// 	CString strValue = _T("");
// 	strValue.Format(_T("[Region] %d, %d, %d, %d"), m_rtROIRect.left, m_rtROIRect.top, m_rtROIRect.right, m_rtROIRect.bottom);
// 	pDC->TextOut(m_rtROIRect.right+5, m_rtROIRect.top+5, strValue);
// 
// 	strValue.Format(_T("[Width] %d pix, %.3lf um"), m_sROIInfo.nWidthPixel, m_sROIInfo.dWidthRealUM);
// 	pDC->TextOut(m_rtROIRect.right+5, m_rtROIRect.top+25, strValue);
// 
// 	strValue.Format(_T("[Height] %d pix, %.3lf um"), m_sROIInfo.nHeightPixel, m_sROIInfo.dHeightRealUM);
// 	pDC->TextOut(m_rtROIRect.right+5, m_rtROIRect.top+45, strValue);
// 
// 	strValue.Format(_T("[Diagonal] %.3lf pix, %.3lf um"), m_sROIInfo.dDiagonalPixel, m_sROIInfo.dDiagonalRealUM);
// 	pDC->TextOut(m_rtROIRect.right+5, m_rtROIRect.top+65, strValue);
// 
// 	strValue.Format(_T("[Resol_X] %.6lf um/pixel"), (m_dResolution/m_dWidthScale));
// 	pDC->TextOut(m_rtROIRect.right+5, m_rtROIRect.top+85, strValue);
// 
// 	strValue.Format(_T("[Resol_Y] %.6lf um/pixel"), (m_dResolution/m_dHeightScale));
// 	pDC->TextOut(m_rtROIRect.right+5, m_rtROIRect.top+105, strValue);
// 
// 	pDC->SelectObject(def_font);

}

void CCameraViewAlign::PopUpCommandMenu(const CPoint& point)
{
	CMenu menu;
	CMenu popMenu;
	menu.CreateMenu();  // 최상위 뼈대 메뉴
	popMenu.CreatePopupMenu();  // 팝업메뉴
	BOOL bAddMenu = FALSE;

	popMenu.AppendMenu(MF_STRING, IDR_LOAD_IMAGE, _T("Load Image"));
	popMenu.AppendMenu(MF_STRING, IDR_SAVE_IMAGE, _T("Save Image"));  // 색상의 서브메뉴 추가
	popMenu.AppendMenu(MF_SEPARATOR);
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_NAME, _T("View Name"));
	popMenu.AppendMenu(MF_STRING, IDR_CENTER_LINE, _T("Center Line"));
	popMenu.AppendMenu(MF_STRING, IDR_CENTER_RECT, _T("Center Rect"));
	popMenu.AppendMenu(MF_STRING, IDR_RULER, _T("Ruler"));
	popMenu.AppendMenu(MF_STRING, IDR_ROI_RECT, _T("ROI Rect"));  // 색상의 서브메뉴 추가
	popMenu.AppendMenu(MF_STRING, IDR_CLEAR_ROI_RECT, _T("Clear ROI Rect"));  // 색상의 서브메뉴 추가
	popMenu.AppendMenu(MF_STRING, IDR_SAVE_ROI_RECT, _T("Save ROI Rect"));  // 색상의 서브메뉴 추가
	popMenu.AppendMenu(MF_SEPARATOR);
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_ORIGIN, _T("1:1 View"));
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_FIT, _T("Fit View"));  // 색상의 서브메뉴 추가
	popMenu.AppendMenu(MF_SEPARATOR);
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_COLOR, _T("Color"));
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_GRAY, _T("Gray"));
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_RED, _T("Red"));
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_GREEN, _T("Green"));
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_BLUE, _T("Blue"));
	popMenu.AppendMenu(MF_SEPARATOR);
	popMenu.AppendMenu(MF_STRING, IDR_CHANGE_VIEW, _T("Change VIew"));

	// 메뉴 상태 처리
	if (m_bDrawViewName)
	{
		popMenu.CheckMenuItem(IDR_VIEW_NAME, MF_CHECKED);
	}

	if (m_bDrawCenterLine)
	{
		popMenu.CheckMenuItem(IDR_CENTER_LINE, MF_CHECKED);
	}

	if (m_bDrawCenterRect)
	{
		popMenu.CheckMenuItem(IDR_CENTER_RECT, MF_CHECKED);

	}
	if (m_bDrawRuler)
	{
		popMenu.CheckMenuItem(IDR_RULER, MF_CHECKED);
	}

	if (m_bDrawROIRect)
	{
		popMenu.CheckMenuItem(IDR_ROI_RECT, MF_CHECKED);
	}

	switch(m_nViewMode)
	{
	case 0:
		popMenu.CheckMenuItem(IDR_VIEW_ORIGIN, MF_CHECKED);
		break;
	case 1:
		popMenu.CheckMenuItem(IDR_VIEW_FIT, MF_CHECKED);
		break;
	}

	// 메뉴 상태 처리
	switch(m_nViewBand)
	{
	case BandTypeRed:
		popMenu.CheckMenuItem(IDR_VIEW_RED, MF_CHECKED);
		break;
	case BandTypeGreen:
		popMenu.CheckMenuItem(IDR_VIEW_GREEN, MF_CHECKED);
		break;
	case BandTypeBlue:
		popMenu.CheckMenuItem(IDR_VIEW_BLUE, MF_CHECKED);
		break;
	case BandTypeGray:
		popMenu.CheckMenuItem(IDR_VIEW_GRAY, MF_CHECKED);
		break;
	default:
		popMenu.CheckMenuItem(IDR_VIEW_COLOR, MF_CHECKED);
		break;
	}

	// 컨텍스트 메뉴 호출
	CRect rect;
	GetWindowRect(rect);
	int nX = rect.left + point.x; 
	int nY = rect.top + point.y; 
	popMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, nX, nY, this);
}


void CCameraViewAlign::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CCameraView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CCameraViewAlign::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{

		AfxMessageBox(_T("TEST"));

	}
	
	return CCameraView::PreTranslateMessage(pMsg);
}
