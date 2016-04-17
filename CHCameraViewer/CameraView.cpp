// CameraView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CameraView.h"
#include "CHImageData.h"
#include "CHBufferDC.h"

using namespace CHImageControls;
// CCameraView

IMPLEMENT_DYNAMIC(CCameraView, CWnd)

CCameraView::CCameraView(int nViewIndex, CWnd* pParentWnd) : m_nViewIndex(nViewIndex), m_pParentWnd(pParentWnd)
{
	m_strViewName	= _T("");

	m_nHScroll		= 0;
	m_nVScroll		= 0;

	m_nMaxHScroll	= 0;
	m_nMaxVScroll	= 0;
	m_nViewMode		= 0;

	m_nViewBand		= BandTypeColor;
	m_rtViewRect	= CRect(0,0,0,0);

	m_nViewMode		= 1;

	m_bDrawViewName		= FALSE;
	m_bDrawCenterLine	= FALSE;
	m_bDrawCenterRect	= FALSE;
	m_bDrawRuler		= FALSE;

	m_dWidthScale	= 1.0; 
	m_dHeightScale	= 1.0;
	m_nScaleWidth	= 0;
	m_nScaleHeight	= 0;

	m_dResolution	= 1.0; // um
	m_dRulerGab		= 100.0; //um

	InitializeCriticalSection(&m_csImageData);
}

CCameraView::~CCameraView()
{
	DeleteCriticalSection(&m_csImageData);
}


BEGIN_MESSAGE_MAP(CCameraView, CWnd)

	ON_COMMAND(IDR_LOAD_IMAGE, &CCameraView::OnLoadImage)
	ON_COMMAND(IDR_SAVE_IMAGE, &CCameraView::OnSaveImage)
	ON_COMMAND(IDR_VIEW_NAME, &CCameraView::OnViewName)
	ON_COMMAND(IDR_CENTER_LINE, &CCameraView::OnCenterLine)
	ON_COMMAND(IDR_CENTER_RECT, &CCameraView::OnCenterRect)
	ON_COMMAND(IDR_RULER, &CCameraView::OnRuler)
	ON_COMMAND(IDR_VIEW_ORIGIN, &CCameraView::OnViewOrigin)
	ON_COMMAND(IDR_VIEW_FIT, &CCameraView::OnViewFit)
	ON_COMMAND(IDR_VIEW_COLOR, &CCameraView::OnViewColor)
	ON_COMMAND(IDR_VIEW_GRAY,&CCameraView::OnViewGray)
	ON_COMMAND(IDR_VIEW_RED, &CCameraView::OnViewRed)
	ON_COMMAND(IDR_VIEW_GREEN, &CCameraView::OnViewGreen)
	ON_COMMAND(IDR_VIEW_BLUE, &CCameraView::OnViewBlue)
	ON_COMMAND(IDR_VIEW_BLUE, &CCameraView::OnViewBlue)

	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CCameraView::PopUpCommandMenu(const CPoint& point )
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
	popMenu.AppendMenu(MF_SEPARATOR);
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_ORIGIN, _T("1:1 View"));
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_FIT, _T("Fit View"));  // 색상의 서브메뉴 추가
	popMenu.AppendMenu(MF_SEPARATOR);
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_COLOR, _T("Color"));
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_GRAY, _T("Gray"));
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_RED, _T("Red"));
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_GREEN, _T("Green"));
	popMenu.AppendMenu(MF_STRING, IDR_VIEW_BLUE, _T("Blue"));

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

// CCameraView 메시지 처리기입니다.
void CCameraView::OnPaint()
{
	CCHBufferDC *pDC = new CCHBufferDC(this); // device context for painting

	UpdateView(pDC);

	delete pDC;
}

double	CCameraView::GetWidthScale()		
{ 
	return m_dWidthScale; 
}

double	CCameraView::GetHeightScale()	
{ 
	return m_dHeightScale; 
}

int	CCameraView::GetHScrollPos(void)
{ 
	return m_nHScroll; 
}

int	CCameraView::GetVScrollPos(void)	
{ 
	return m_nVScroll; 
}

BOOL CCameraView::LoadImage(const CString& strFilename)
{
	if (!CCHImageData::LoadImage(strFilename)) 
	{
		m_nHScroll = m_nVScroll = 0;
		m_nMaxHScroll = m_nMaxVScroll = 0;
		SetScrollRange(SB_HORZ, 0, m_nMaxHScroll);
		SetScrollRange(SB_VERT, 0, m_nMaxVScroll);
		SetScrollPos(SB_HORZ, m_nHScroll);
		SetScrollPos(SB_VERT, m_nVScroll);
		return FALSE;
	}

	SetViewMode(m_nViewMode);

	return TRUE;
}

void CCameraView::UpdateView(CDC *pDC)
{
	CRect rtCurrentRect , rtSourceRect;
	GetClientRect(rtCurrentRect);

	if (GetImageExist())
	{
		if (m_rtViewRect!=rtCurrentRect) 
		{
			SetViewMode(m_nViewMode);
		}

		int nSrcWidth = GetWidth();
		int nSrcHeight = GetHeight();

		int nWidth = int(double(nSrcWidth) * m_dWidthScale + 0.5);
		int nHeight = int(double(nSrcHeight) * m_dHeightScale + 0.5);

		if (m_nScaleWidth!=nWidth || m_nScaleHeight!=nHeight) 
		{
			SetViewMode(m_nViewMode);
		}

		switch(m_nViewMode)
		{
		case 0: // fit
			pDC->SelectStockObject(GRAY_BRUSH);
			pDC->Rectangle(rtCurrentRect);
			ShowImage(pDC->m_hDC, 0, 0, rtCurrentRect.Width(), rtCurrentRect.Height(), m_nHScroll, m_nVScroll);
			break;

		case 1: // origin
			ShowImage(pDC->m_hDC, rtCurrentRect);
			break;
		}
	}
	else
	{
		pDC->SelectStockObject(GRAY_BRUSH);
		pDC->Rectangle(rtCurrentRect);
	}


	if (m_bDrawCenterLine) DrawCenterLine(pDC);

	if (m_bDrawCenterRect) DrawCenterRect(pDC);

	if (m_bDrawRuler) DrawRuler(pDC);

	if (m_bDrawViewName) DrawViewName(pDC);

}


void  CCameraView::DrawViewName(CDC *pDC)
{
	int nDiv		= 14;
	int nTextHeight = m_rtViewRect.Height() / nDiv;
	int nTextWidth	= nTextHeight / 3;

	CFont font;
	VERIFY(font.CreateFont(
		nTextHeight,               // nHeight
		nTextWidth,                // nWidth
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
	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetBkMode(TRANSPARENT);

	//int nStrLength = (m_strViewName.GetLength() * 10);

	//pDC->TextOut(m_rtClientRect.Width() - nStrLength - 30, 5, m_strViewName);
	CRect rtTextRect(10, 10, 200, 200);

	//m_strViewName = _T("Match: 000%\nJudge: OK");
	//pDC->TextOut(10, 10, m_strViewName);

	pDC->DrawText(m_strViewName, rtTextRect, DT_LEFT | DT_NOCLIP |DT_VCENTER | DT_WORDBREAK);


	pDC->SelectObject(def_font);

}

int	CCameraView::GetScaleWidth()
{
	if (GetImageExist()==FALSE) return 0;

	CRect rect;
	int nWidth = GetWidth();

	switch(m_nViewMode)
	{
	case 0:
		m_dWidthScale = 1.0;
		return nWidth;

	case 1:
		GetClientRect(rect);
		if (nWidth!=0)
			m_dWidthScale = double(rect.Width()) / double(nWidth);
		return int(double(nWidth) * m_dWidthScale + 0.5);
	}

	return 0;
}

int	CCameraView::GetScaleHeight()
{ 
	if (GetImageExist()==FALSE) return 0;

	CRect rect;
	int nHeight = GetHeight();

	switch(m_nViewMode)
	{
	case 0:
		m_dHeightScale = 1.0;
		return nHeight;

	case 1:
		GetClientRect(rect);
		if (nHeight!=0)
			m_dHeightScale = double(rect.Height()) / double(nHeight);
		return int(double(nHeight) * m_dHeightScale + 0.5);
	}

	return 0;
}

void CCameraView::SetViewMode(int nViewMode)
{ 
	m_nViewMode = nViewMode; 

	GetClientRect(m_rtViewRect);

	m_nScaleWidth = GetScaleWidth();
	m_nScaleHeight = GetScaleHeight();

	switch(m_nViewMode)
	{
	case 0:
		if (m_rtViewRect.Width() >= m_nScaleWidth)
		{
			m_nMaxHScroll = 0;
		}
		else
		{
			m_nMaxHScroll = m_nScaleWidth - (m_rtViewRect.Width()+17);
			if (m_nMaxHScroll < 0) 
			{
				m_nMaxHScroll = 0;
			}
			else
			{
				m_nMaxHScroll += 17;
			}
		}

		if (m_rtViewRect.Height() >= m_nScaleHeight)
		{
			m_nMaxVScroll = 0;
		}
		else
		{
			m_nMaxVScroll = m_nScaleHeight - (m_rtViewRect.Height()+17);
			if (m_nMaxVScroll < 0) 
			{
				m_nMaxVScroll = 0;
			}
			else
			{
				m_nMaxVScroll += 17;
			}

		}
		break;

	case 1:
		m_nMaxHScroll = m_nMaxVScroll = 0;	
		break;
	}

	m_nHScroll = m_nVScroll = 0;

	SetScrollRange(SB_HORZ, 0, m_nMaxHScroll);
	SetScrollRange(SB_VERT, 0, m_nMaxVScroll);	
	SetScrollPos(SB_HORZ, m_nHScroll);
	SetScrollPos(SB_VERT, m_nVScroll);
}

void CCameraView::OnLoadImage()
{
	CString szFilter = _T("All Files(*.*)|*.*||");

	CString strPath;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	dlg.m_ofn.lpstrTitle = _T("Load Image");

	if(dlg.DoModal() == IDOK)
	{
		if (m_OriginImage.LoadImage(dlg.GetPathName()))
		{
			if (m_nViewBand==4)
			{
				m_OriginImage.CopyImageTo(this);
			}
			else
			{
				m_OriginImage.GetBandImage(m_nViewBand, this);
			}

			Invalidate(TRUE);
		}
	}
}

void CCameraView::OnSaveImage()
{
	CString szFilter = _T("BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg| All Files(*.*)|*.*||");

	CString strPath;
	CFileDialog dlg(FALSE, szFilter, NULL, OFN_HIDEREADONLY, szFilter);
	dlg.m_ofn.lpstrTitle = _T("Save Image");

	if(dlg.DoModal() == IDOK)
	{
		if (SaveImage(dlg.GetPathName()))
		{

		}
	}
}

void CCameraView::OnViewName()
{
	m_bDrawViewName = !m_bDrawViewName;
	Invalidate(TRUE);
}

void CCameraView::OnCenterLine()
{
	m_bDrawCenterLine = !m_bDrawCenterLine;
	Invalidate(TRUE);
}

void CCameraView::OnCenterRect()
{
	m_bDrawCenterRect = !m_bDrawCenterRect;
	Invalidate(TRUE);
}

void CCameraView::OnRuler()
{
	m_bDrawRuler = !m_bDrawRuler;
	Invalidate(TRUE);
}

void CCameraView::OnViewOrigin()
{
	m_nViewMode = 0;

	SetViewMode(m_nViewMode);
	Invalidate(TRUE);
}

void CCameraView::OnViewFit()
{
	m_nViewMode = 1;

	SetViewMode(m_nViewMode);
	Invalidate(TRUE);
}

void CCameraView::OnViewColor()
{
	if (m_OriginImage.GetChannels()==1)
	{
		m_nViewBand = BandTypeGray;
		return;
	}

	if (m_nViewBand == BandTypeColor) return;

	if (m_OriginImage.CopyImageTo(this))
	{
		m_nViewBand = BandTypeColor;
		Invalidate(TRUE);
	}
}

void CCameraView::OnViewGray()
{
	if (m_nViewBand == BandTypeGray) return;

	if (m_OriginImage.GetBandImage(BandTypeGray, this))
	{
		m_nViewBand = BandTypeGray;
		Invalidate(TRUE);
	}
}

void CCameraView::OnViewRed()
{
	if (m_OriginImage.GetChannels()==1)
	{
		m_nViewBand = BandTypeGray;
		return;
	}

	if (m_nViewBand == BandTypeRed) return;

	if (m_OriginImage.GetBandImage(BandTypeRed, this))
	{
		m_nViewBand = BandTypeRed;
		Invalidate(TRUE);
	}
}

void CCameraView::OnViewGreen()
{
	if (m_OriginImage.GetChannels()==1)
	{
		m_nViewBand = BandTypeGray;
		return;
	}

	if (m_nViewBand == BandTypeGreen) return;

	if (m_OriginImage.GetBandImage(BandTypeGreen, this))
	{
		m_nViewBand = BandTypeGreen;
		Invalidate(TRUE);
	}
}

void CCameraView::OnViewBlue()
{
	if (m_OriginImage.GetChannels()==1)
	{
		m_nViewBand = BandTypeGray;
		return;
	}

	if (m_nViewBand == BandTypeBlue) return;

	if (m_OriginImage.GetBandImage(BandTypeBlue, this))
	{
		m_nViewBand = BandTypeBlue;
		Invalidate(TRUE);
	}
}


void CCameraView::SetViewName(const CString& strViewName)
{ 
	m_strViewName = strViewName; 
}

void CCameraView::SetParentWnd(CWnd* pParentWnd)			
{ 
	m_pParentWnd = pParentWnd; 
}

void CCameraView::SetDrawCenterLine(BOOL bDraw)
{
	m_bDrawCenterLine = bDraw; 
	Invalidate(TRUE);
}

void CCameraView::SetDrawRuler(BOOL bDraw)
{
	m_bDrawRuler = bDraw; 
	Invalidate(TRUE);
}

void CCameraView::SetResolution(double dRes)
{
	if (dRes<0.0) return;

	m_dResolution = dRes;
}

void CCameraView::SetRulerGab(double dGab)
{
	if (dGab<0.0) return;

	m_dRulerGab = dGab;
}

void CCameraView::DrawCenterLine(CDC *pDC)
{
	if (m_rtViewRect.Width()==0 || m_rtViewRect.Height()==0) return;

	CRect rtDraw = m_rtViewRect;
	if (m_nViewMode==0)
	{
		rtDraw.right = GetWidth();
		rtDraw.bottom = GetHeight();
	}

	rtDraw.left -= m_nHScroll;
	rtDraw.top -= m_nVScroll;
	rtDraw.right -= m_nHScroll;
	rtDraw.bottom -= m_nVScroll;

	CPen pen, *pPen=NULL;
	pen.CreatePen(PS_SOLID, 2, RGB(255,0,0));
	pDC->SelectObject(&pen);

	//pDC->SelectStockObject(WHITE_PEN);

	// center line
	pDC->MoveTo( (rtDraw.Width()/2)-m_nHScroll,	0);
	pDC->LineTo((rtDraw.Width()/2)-m_nHScroll,	rtDraw.Height());
	pDC->MoveTo(0,					(rtDraw.Height()/2)-m_nVScroll);
	pDC->LineTo(rtDraw.Width(),		(rtDraw.Height()/2)-m_nVScroll);
}

void CCameraView::DrawCenterRect(CDC *pDC)
{
	if (m_rtViewRect.Width()==0 || m_rtViewRect.Height()==0) return;

	CRect rtDraw = m_rtViewRect;
	if (m_nViewMode==0)
	{
		rtDraw.right = GetWidth();
		rtDraw.bottom = GetHeight();
	}

	rtDraw.left -= m_nHScroll;
	rtDraw.top -= m_nVScroll;
	rtDraw.right -= m_nHScroll;
	rtDraw.bottom -= m_nVScroll;

	CPen pen, *pPen=NULL;
	pen.CreatePen(PS_SOLID, 2, RGB(255,0,0));
	pDC->SelectObject(&pen);
	pDC->SelectStockObject(NULL_BRUSH);

	int nWidthBlockCount = 4;
	int nHeightBlockCount = 4;
	int nRectWidthBlockCount = 2;
	int nRectHeightBlockCount = 2;

	int nBlockWidth = (rtDraw.Width() / nWidthBlockCount) * nRectWidthBlockCount;
	int nBlockHeight = (rtDraw.Height() / nHeightBlockCount) * nRectHeightBlockCount;


	int nCenterWidth = (rtDraw.Width()/2)-m_nHScroll;
	int nCenterHeight = (rtDraw.Height()/2)-m_nVScroll;

	// center rect
	CRect rtRect;
	rtRect.left = nCenterWidth - (nBlockWidth/2);
	rtRect.right = nCenterWidth + (nBlockWidth/2);
	rtRect.top = nCenterHeight - (nBlockHeight/2);
	rtRect.bottom = nCenterHeight + (nBlockHeight/2);

	pDC->Rectangle(rtRect);
}


void CCameraView::DrawRuler(CDC *pDC)
{
	if (m_dRulerGab==0.0) return;

	if (m_rtViewRect.Width()==0 || m_rtViewRect.Height()==0) return;

	CRect rtDraw = m_rtViewRect;
	if (m_nViewMode==0)
	{
		rtDraw.right = GetWidth();
		rtDraw.bottom = GetHeight();
	}

	rtDraw.left -= m_nHScroll;
	rtDraw.top -= m_nVScroll;
	rtDraw.right -= m_nHScroll;
	rtDraw.bottom -= m_nVScroll;

	pDC->SelectStockObject(WHITE_PEN);

	CFont font;
	VERIFY(font.CreateFont(
		10,                        // nHeight
		5,                         // nWidth
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
	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetBkMode(TRANSPARENT);

	// ruler 
	double dResX = m_dResolution / m_dWidthScale;
	double dResY = m_dResolution / m_dHeightScale;
	double dGabX = m_dRulerGab / dResX; // um
	double dGabY = m_dRulerGab / dResY; // um

	if (dGabX<=0.0 || dGabY<=0.0) return;

	double dWidth = GetScaleWidth();
	double dHeight = GetScaleHeight();

	double dCenterX = rtDraw.Width()/2;
	double dCenterY = rtDraw.Height()/2;

	// x direction
	CString strValue = _T("");
	strValue.Format(_T("%.1lf um"), m_dRulerGab);
	pDC->TextOut(-m_nHScroll+5, rtDraw.Height()/2-m_nVScroll+5, strValue);
	for (double dPos=dCenterX; dPos>=0.0; dPos-=dGabX)
	{
		pDC->MoveTo((int)(dPos+0.5)-m_nHScroll, -m_nVScroll);
		pDC->LineTo((int)(dPos+0.5)-m_nHScroll, -m_nVScroll+6);
	}
	for (double dPos=dCenterX+dGabX; dPos<dWidth; dPos+=dGabX)
	{
		pDC->MoveTo((int)(dPos+0.5)-m_nHScroll, -m_nVScroll);
		pDC->LineTo((int)(dPos+0.5)-m_nHScroll, -m_nVScroll+6);
	}

	// y direction
	strValue.Format(_T("%.1lf um"), m_dRulerGab);
	pDC->TextOut(rtDraw.Width()/2-m_nHScroll+5, 5-m_nVScroll, strValue);
	for (double dPos=dCenterY; dPos>0.0; dPos-=dGabY)
	{
		pDC->MoveTo(-m_nHScroll, (int)(dPos+0.5)-m_nVScroll);
		pDC->LineTo(-m_nHScroll+6, (int)(dPos+0.5)-m_nVScroll);
	}
	for (double dPos=dCenterY+dGabY; dPos<dHeight; dPos+=dGabY)
	{
		pDC->MoveTo(-m_nHScroll, (int)(dPos+0.5)-m_nVScroll);
		pDC->LineTo(-m_nHScroll+6, (int)(dPos+0.5)-m_nVScroll);
	}
}

void CCameraView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_pParentWnd==NULL) return;

	CRect rect;
	this->GetClientRect(rect);

	point.x += m_nHScroll;
	point.y += m_nVScroll;
	if (point.x > 0 && point.x < this->GetScaleWidth()-1 &&
		point.y > 0 && point.y < this->GetScaleHeight()-1 )
	{
		m_pParentWnd->SendMessage(WM_RBUTTONDOWN, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnRButtonDown(nFlags, point);
}

void CCameraView::OnRButtonUp(UINT nFlags, CPoint point)
{
	PopUpCommandMenu(point);

	if (m_pParentWnd==NULL) return;

	CRect rect;
	this->GetClientRect(rect);

	point.x += m_nHScroll;
	point.y += m_nVScroll;

	if (point.x > 0 && point.x < this->GetScaleWidth()-1 &&
		point.y > 0 && point.y < this->GetScaleHeight()-1 )
	{
		m_pParentWnd->SendMessage(WM_RBUTTONUP, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnRButtonUp(nFlags, point);
}


void CCameraView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pParentWnd==NULL) return;

	CRect rect;
	this->GetClientRect(rect);

	point.x += m_nHScroll;
	point.y += m_nVScroll;
	if (point.x > 0 && point.x < this->GetScaleWidth()-1 &&
		point.y > 0 && point.y < this->GetScaleHeight()-1 )
	{
		m_pParentWnd->SendMessage(WM_RBUTTONDBLCLK, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnRButtonDblClk(nFlags, point);
}


void CCameraView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pParentWnd==NULL) return;

	CRect rect;
	this->GetClientRect(rect);

	point.x += m_nHScroll;
	point.y += m_nVScroll;

	if (point.x > 0 && point.x < this->GetScaleWidth()-1 &&
		point.y > 0 && point.y < this->GetScaleHeight()-1 )
	{
		m_pParentWnd->SendMessage(WM_LBUTTONDOWN, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnLButtonDown(nFlags, point);
}


void CCameraView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pParentWnd==NULL) return;

	CRect rect;
	this->GetClientRect(rect);

	point.x += m_nHScroll;
	point.y += m_nVScroll;

	if (point.x > 0 && point.x < this->GetScaleWidth()-1 &&
		point.y > 0 && point.y < this->GetScaleHeight()-1 )
	{
		m_pParentWnd->SendMessage(WM_LBUTTONUP, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnLButtonUp(nFlags, point);
}


void CCameraView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pParentWnd==NULL) return;

	CRect rect;
	this->GetClientRect(rect);

	point.x += m_nHScroll;
	point.y += m_nVScroll;
	if (point.x > 0 && point.x < this->GetScaleWidth()-1 &&
		point.y > 0 && point.y < this->GetScaleHeight()-1 )
	{
		m_pParentWnd->SendMessage(WM_LBUTTONDBLCLK, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnLButtonDblClk(nFlags, point);
}


void CCameraView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pParentWnd==NULL) return;

	CRect rect;
	this->GetClientRect(rect);

	point.x += m_nHScroll;
	point.y += m_nVScroll;

	if (point.x > 0 && point.x < this->GetScaleWidth()-1 &&
		point.y > 0 && point.y < this->GetScaleHeight()-1 )
	{
		m_pParentWnd->SendMessage(WM_MOUSEMOVE, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnMouseMove(nFlags, point);
}


void CCameraView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (nSBCode)
	{
	case SB_BOTTOM:			// Scroll to bottom. 
		break;
	case SB_ENDSCROLL:		// End scroll.
		break;
	case SB_LINEDOWN:		// Scroll one line down.
		if (m_nVScroll < m_nMaxVScroll)
			m_nVScroll++;
		break;
	case SB_LINEUP:			// Scroll one line up. 
		if (m_nVScroll > 0)
			m_nVScroll--;
		break;
	case SB_PAGEDOWN:		// Scroll one page down. 
		if (5 < m_nMaxVScroll / 256)
			m_nVScroll += 20;//m_nVScroll += 5;
		else
			m_nVScroll += 10;//m_nVScroll += (m_nMaxVScroll / 256);

		if (m_nVScroll > m_nMaxVScroll)
			m_nVScroll = m_nMaxVScroll;
		break;
	case SB_PAGEUP:			// Scroll one page up.
		if (5 < m_nMaxVScroll / 256)
			m_nVScroll -= 20;//m_nVScroll -= 5;
		else
			m_nVScroll -= 10;//m_nVScroll -= (m_nMaxVScroll / 256);

		if (m_nVScroll < 0)
			m_nVScroll = 0;
		break;
	case SB_THUMBPOSITION:	// Scroll to the absolute position. The current position is provided in nPos. 
		break;
	case SB_THUMBTRACK:		// Drag scroll box to specified position. The current position is provided in nPos. 
		m_nVScroll = nPos;
		break;
	case SB_TOP:			// Scroll to top. 
		break;
	}

	// Set the new position of the thumb (scroll box).
	SetScrollPos(SB_VERT, m_nVScroll);

	Invalidate(FALSE);

	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CCameraView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		break;
	case SB_RIGHT:      // Scroll to far right.
		break;
	case SB_ENDSCROLL:   // End scroll.
		break;
	case SB_LINELEFT:      // Scroll left.
		if (m_nHScroll > 0)
			m_nHScroll--;
		break;
	case SB_LINERIGHT:   // Scroll right.
		if (m_nHScroll < m_nMaxHScroll)
			m_nHScroll++;
		break;
	case SB_PAGELEFT:    // Scroll one page left.
		if (5 < m_nMaxHScroll / 256)
			m_nHScroll -= 20;//m_nHScroll -= 5;
		else
			m_nHScroll -= 10;//m_nHScroll -= m_nMaxHScroll / 256;

		if (m_nHScroll < 0)
			m_nHScroll = 0;
		break;
	case SB_PAGERIGHT:      // Scroll one page right.
		if (5 < m_nMaxHScroll / 256)
			m_nHScroll += 20;//m_nHScroll += 5;
		else
			m_nHScroll += 10;//m_nHScroll += m_nMaxHScroll / 256;

		if (m_nHScroll > m_nMaxHScroll)
			m_nHScroll = m_nMaxHScroll;
		break;
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		break;
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		m_nHScroll = nPos;
		break;
	}

	// Set the new position of the thumb (scroll box).
	SetScrollPos(SB_HORZ, m_nHScroll);

	Invalidate(FALSE);

	__super::OnHScroll(nSBCode, nPos, pScrollBar);
}
