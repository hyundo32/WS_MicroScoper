// CHImageView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CHImageView.h"
#include "CHBufferDC.h"
#include <math.h>

using namespace CHImageControls;

// CCHImageView
IMPLEMENT_DYNAMIC(CCHImageView, CWnd)

CCHImageView::CCHImageView()
{
	Reset();
}

CCHImageView::CCHImageView(CWnd *pParentWnd)
{
	Reset();
	m_pParentWnd	= pParentWnd;
}

void CCHImageView::Reset()
{
	m_strViewName	= _T("");
	m_pParentWnd	= NULL;

	m_nHScroll		= 0;
	m_nVScroll		= 0;

	m_nMaxHScroll	= 0;
	m_nMaxVScroll	= 0;
	m_bDrawObject	= TRUE;

	m_dWidthScale	= 1.0; 
	m_dHeightScale	= 1.0;
	m_nScaleWidth	= 0;
	m_nScaleHeight	= 0;

	m_nDrawType		= DrawOrigin;
	m_rtClientRect	= CRect(0,0,0,0);
}

CCHImageView::~CCHImageView()
{
	ReleaseImage();
}

BEGIN_MESSAGE_MAP(CCHImageView, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
//	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()



// CCHImageView 메시지 처리기입니다.

void CCHImageView::UpdateImageView(CDC *pDC)
{
	CRect rtCurrentRect , rtSourceRect;
	GetClientRect(rtCurrentRect);

	if (GetImageExist())
	{
		if (m_rtClientRect!=rtCurrentRect) SetDrawType(m_nDrawType);

		int nSrcWidth = GetWidth();
		int nSrcHeight = GetHeight();

		int nWidth = int(double(nSrcWidth) * m_dWidthScale + 0.5);
		int nHeight = int(double(nSrcHeight) * m_dHeightScale + 0.5);

		if (m_nScaleWidth!=nWidth || m_nScaleHeight!=nHeight) SetDrawType(m_nDrawType);

		switch(m_nDrawType)
		{
		case DrawOrigin:
			pDC->SelectStockObject(GRAY_BRUSH);
			pDC->Rectangle(rtCurrentRect);
			ShowImage(pDC->m_hDC, 0, 0, rtCurrentRect.Width(), rtCurrentRect.Height(), m_nHScroll, m_nVScroll);
			break;

		case DrawScale:
			pDC->SelectStockObject(GRAY_BRUSH);
			pDC->Rectangle(rtCurrentRect);

			if (rtCurrentRect.Width() < nWidth)
			{
				rtSourceRect.left	= int(double(m_nHScroll) / m_dWidthScale);
				rtSourceRect.right	= rtSourceRect.left + int(double(rtCurrentRect.Width()) / m_dWidthScale);
			}
			else
			{
				rtSourceRect.left = 0;
				rtSourceRect.right = nSrcWidth;
				rtCurrentRect.right	= nWidth;
			}

			if (rtCurrentRect.Height() < nHeight)
			{
				rtSourceRect.top	= int(double(m_nMaxVScroll-m_nVScroll) / m_dHeightScale);
				rtSourceRect.bottom	= rtSourceRect.top + int(double(rtCurrentRect.Height()) / m_dHeightScale);
			}
			else
			{
				rtSourceRect.top	= 0;
				rtSourceRect.bottom	= nSrcHeight;
				rtCurrentRect.bottom = nHeight;
			}
			ShowImage(pDC->m_hDC, rtSourceRect, rtCurrentRect);
			break;

		case DrawFit:
			ShowImage(pDC->m_hDC, rtCurrentRect);
			break;
		}

		if (m_bDrawObject) DrawObjectList(pDC);
	}
	else
	{
		pDC->SelectStockObject(GRAY_BRUSH);
		pDC->Rectangle(rtCurrentRect);

		CFont font;
		VERIFY(font.CreateFont(
			35,                        // nHeight
			15,                         // nWidth
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

		int strValue = m_strViewName.GetLength() * 15 / 2;

		pDC->TextOut(rtCurrentRect.Width()/2 - strValue, rtCurrentRect.Height()/2 - 20, m_strViewName);
		pDC->SelectObject(def_font);
	}
}

void CCHImageView::OnPaint()
{

	CCHBufferDC *pDC = new CCHBufferDC(this); // device context for painting

	UpdateImageView(pDC);

	delete pDC;
}

void CCHImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pParentWnd==NULL || !GetImageExist()) return;

	if (point.x >= -1 && point.x < GetScaleWidth() &&
		point.y >= -1 && point.y < GetScaleHeight() )
	{
		m_pParentWnd->SendMessage(WM_MOUSEMOVE, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnMouseMove(nFlags, point);
}

int CCHImageView::PointInDrawObject(CPoint point, CRect& rtResult)
{
	int nMinIndex = -1;
	long nMinDist = INT_MAX;
	long nCurDist = 0;

	int nIndex = 0;
	for (ListDrawObjectIt it = m_ListDrawObject.begin(); it != m_ListDrawObject.end(); it++)
	{
		CRect rtRect(it->nLeft, it->nTop, it->nRight, it->nBottom);
		if (rtRect.PtInRect(point))
		{
			nCurDist = abs(it->GetCenterX() - point.x) + abs(it->GetCenterY() - point.y);
			if (nMinDist > nCurDist)
			{
				nMinIndex = nIndex;
				nMinDist = nCurDist;
				rtResult = rtRect;
			}
		}
		nIndex++;
	}

	return nMinIndex;
}

void CCHImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pParentWnd==NULL || !GetImageExist()) return;

	if (point.x > -1 && point.x < GetScaleWidth() &&
		point.y > -1 && point.y < GetScaleHeight() )
	{
		m_pParentWnd->SendMessage(WM_LBUTTONDOWN, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnLButtonDown(nFlags, point);
}

void CCHImageView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pParentWnd==NULL || !GetImageExist()) return;

	if (point.x > -1 && point.x < GetScaleWidth() &&
		point.y > -1 && point.y < GetScaleHeight() )
	{
		m_pParentWnd->SendMessage(WM_LBUTTONUP, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnLButtonUp(nFlags, point);
}

void CCHImageView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pParentWnd==NULL || !GetImageExist()) return;

	if (point.x > -1 && point.x < GetScaleWidth() &&
		point.y > -1 && point.y < GetScaleHeight() )
	{
		m_pParentWnd->SendMessage(WM_LBUTTONDBLCLK, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));
	}

	__super::OnLButtonDblClk(nFlags, point);
}

void CCHImageView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pParentWnd==NULL) return;

	CPoint ptScroll = point;
	point.x += m_nHScroll;
	point.y += m_nVScroll;

	m_pParentWnd->SendMessage(WM_RBUTTONDOWN, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));

	__super::OnRButtonDown(nFlags, point);
}

void CCHImageView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pParentWnd==NULL) return;

	m_pParentWnd->SendMessage(WM_RBUTTONUP, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));

	__super::OnRButtonUp(nFlags, point);
}

void CCHImageView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pParentWnd==NULL) return;

	m_pParentWnd->SendMessage(WM_RBUTTONDBLCLK, static_cast<WPARAM>(nFlags), MAKELPARAM(point.x, point.y));

	__super::OnRButtonDblClk(nFlags, point);
}

void CCHImageView::SetDrawObject(BOOL bDraw)
{ 
	m_bDrawObject = bDraw; 
}

void CCHImageView::SetDrawType(int nDrawType)
{ 
	m_nDrawType = nDrawType; 

	GetClientRect(m_rtClientRect);

	m_nScaleWidth = GetScaleWidth();
	m_nScaleHeight = GetScaleHeight();

	switch(m_nDrawType)
	{
	case DrawOrigin:
		if (m_rtClientRect.Width() >= m_nScaleWidth)
		{
			m_nMaxHScroll = 0;
		}
		else
		{
			m_nMaxHScroll = m_nScaleWidth - (m_rtClientRect.Width()+17);
			if (m_nMaxHScroll < 0) 
			{
				m_nMaxHScroll = 0;
			}
			else
			{
				m_nMaxHScroll += 17;
			}
		}

		if (m_rtClientRect.Height() >= m_nScaleHeight)
		{
			m_nMaxVScroll = 0;
		}
		else
		{
			m_nMaxVScroll = m_nScaleHeight - (m_rtClientRect.Height()+17);
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

	case DrawScale:
		m_nMaxHScroll = m_nScaleWidth - (m_rtClientRect.Width()+17);
		m_nMaxVScroll = m_nScaleHeight - (m_rtClientRect.Height()+17);

		if (m_nMaxVScroll < 0) 
		{
			m_nMaxVScroll = 0;
		}
		else
		{
			m_nMaxVScroll += 17;
		}

		if (m_nMaxHScroll < 0) 
		{
			m_nMaxHScroll = 0;
		}
		else
		{
			m_nMaxHScroll += 17;
		}
		break;

	case DrawFit:
		m_nMaxHScroll = m_nMaxVScroll = 0;	
		break;
	}

	m_nHScroll = m_nVScroll = 0;

	SetScrollRange(SB_HORZ, 0, m_nMaxHScroll);
	SetScrollRange(SB_VERT, 0, m_nMaxVScroll);	
	SetScrollPos(SB_HORZ, m_nHScroll);
	SetScrollPos(SB_VERT, m_nVScroll);
}

void CCHImageView::DrawObjectList(CDC *pDC)
{
	for (ListDrawObjectIt it = m_ListDrawObject.begin(); it != m_ListDrawObject.end(); it++)
	{
		DrawObject(pDC, &(*it));
	}
}


void CCHImageView::DrawText(CDC *pDC, int nLeft, int nTop, int nRight, int nBottom, double dScaleX, double dScaleY, CCHDrawObject *pObject)
{
	int nSizeWidth	= int(double(pObject->nTextWidth) * dScaleX);
	int nSizeHeight = int(double(pObject->nTextHeight) * dScaleY);

	// text
	CFont font;
	VERIFY(font.CreateFont(
		nSizeHeight,				// nHeight
		nSizeWidth,					// nWidth
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
		_T("Arial")));             // lpszFacename

	// Do something with the font just created...
	CFont* pFont = pDC->SelectObject(&font);
	pDC->SetTextColor(pObject->nForeColor);
	pDC->SetBkMode(TRANSPARENT);

//	int nStartX = nLeft + ((nRight - nLeft)/2) - ((pObject->strTextString.GetLength() * nSizeWidth) / 2);
//	int nStartY = nTop + ((nBottom - nTop)/2) - (nSizeHeight / 2);

	int nStartX = nLeft + 2;
	int nStartY = nTop + 2;


	pDC->TextOut(nStartX, nStartY, pObject->strTextString);	
	pDC->SelectObject(pFont);
	
}

void CCHImageView::DrawObject(CDC *pDC, CCHDrawObject *pObject)
{
	CRect rect;
	int nLeft		= int(double(pObject->nLeft) * m_dWidthScale + 0.5) - m_nHScroll;
	int nTop		= int(double(pObject->nTop) * m_dHeightScale + 0.5) - m_nVScroll;
	int nRight		= int(double(pObject->nRight) * m_dWidthScale + 0.5) - m_nHScroll;
	int nBottom		= int(double(pObject->nBottom) * m_dHeightScale + 0.5) - m_nVScroll;

	if (nLeft > GetScaleWidth())	nLeft	= GetScaleWidth();
	if (nTop > GetScaleHeight())	nTop	= GetScaleHeight();
	if (nRight > GetScaleWidth())	nRight	= GetScaleWidth();
	if (nBottom > GetScaleHeight()) nBottom = GetScaleHeight();

	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, pObject->nSize, pObject->nForeColor);
	pOldPen = (CPen*) pDC->SelectObject(&pen);

	CBrush brush, *pBrush;
	if (pObject->nBackColor==NULL_BRUSH)
	{
		pDC->SelectStockObject(pObject->nBackColor);
	}
	else
	{
		brush.CreateSolidBrush(pObject->nBackColor);
		pBrush = (CBrush*)pDC->SelectObject(&brush);
	}

	switch(pObject->nObjectType)
	{
	case ObjectLine:
		pDC->MoveTo(nLeft, nTop);
		pDC->LineTo(nRight, nBottom);
		break;

	case ObjectRectangle:
		pDC->Rectangle(nLeft, nTop, nRight, nBottom);
		break;

	case ObjectCircle:
		pDC->Ellipse(nLeft, nTop, nRight, nBottom);
		break;

	case ObjectText:
		DrawText(pDC, nLeft, nTop, nRight, nBottom, m_dWidthScale, m_dHeightScale, pObject);
		break;

	case ObjectTextRect:
		pDC->Rectangle(nLeft, nTop, nRight, nBottom);
		DrawText(pDC, nLeft, nTop, nRight, nBottom, m_dWidthScale, m_dHeightScale, pObject);
		break;
	}
}	

int	CCHImageView::GetScaleWidth()
{
	CRect rect;
	int nWidth = GetWidth();

	switch(m_nDrawType)
	{
	case DrawOrigin:
		m_dWidthScale = 1.0;
		return nWidth;

	case DrawScale:
		return int(double(nWidth) * m_dWidthScale + 0.5);

	case DrawFit:
		GetClientRect(rect);
		if (nWidth!=0)
			m_dWidthScale = double(rect.Width()) / double(nWidth);
		return int(double(nWidth) * m_dWidthScale + 0.5);
	}

	return 0;
}

double	CCHImageView::GetWidthScale()
{
	CRect rect;
	switch(m_nDrawType)
	{
	case DrawOrigin:
		return 1.0;

	case DrawScale:
		return m_dWidthScale;

	case DrawFit:
		GetClientRect(rect);
		return m_dWidthScale;
	}

	return 1.0;
}

int	CCHImageView::GetScaleHeight()
{ 
	CRect rect;
	int nHeight = GetHeight();

	switch(m_nDrawType)
	{
	case DrawOrigin:
		m_dHeightScale = 1.0;
		return nHeight;

	case DrawScale:
		return int(double(nHeight) * m_dHeightScale + 0.5);

	case DrawFit:
		GetClientRect(rect);
		if (nHeight!=0)
			m_dHeightScale = double(rect.Height()) / double(nHeight);
		return int(double(nHeight) * m_dHeightScale + 0.5);
	}

	return 0;
}

double	CCHImageView::GetHeightScale()
{ 
	CRect rect;

	switch(m_nDrawType)
	{
	case DrawOrigin:
		return 1.0;

	case DrawScale:
		return m_dHeightScale;

	case DrawFit:
		GetClientRect(rect);
		return m_dHeightScale;
	}
	
	return 1.0;
}

int	CCHImageView::GetHScrollPos(void)
{ 
	return m_nHScroll; 
}

int	CCHImageView::GetVScrollPos(void)	
{ 
	return m_nVScroll; 
}

void CCHImageView::SetViewName(const CString strViewName)
{ 
	m_strViewName = strViewName; 
}

void CCHImageView::SetParentWnd(CWnd* pParentWnd)			
{ 
	m_pParentWnd = pParentWnd; 
}

void CCHImageView::SetDrawScale(double dScale)
{
	if (m_nDrawType!=DrawScale) return;

	CRect rect;
	GetClientRect(rect);

	m_dWidthScale = m_dHeightScale = dScale;
	m_nHScroll = m_nVScroll = 0;

	int nWidth = int(double(GetWidth()) * m_dWidthScale);
	int nHeight = int(double(GetHeight()) * m_dHeightScale);
	m_nMaxHScroll = nWidth - rect.Width();
	m_nMaxVScroll = nHeight - rect.Height();

	if (m_nMaxVScroll > 0)
		SetScrollRange(SB_VERT, 0, m_nMaxVScroll);
	else
		SetScrollRange(SB_VERT, 0, 0);

	if (m_nMaxHScroll > 0)
		SetScrollRange(SB_HORZ, 0, m_nMaxHScroll);
	else
		SetScrollRange(SB_HORZ, 0, 0);

	Invalidate(TRUE); 
}

BOOL CCHImageView::LoadImage(const CString& strFilename)
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

	SetDrawType(m_nDrawType);

	return TRUE;
}

void CCHImageView::AddDrawObject(const CCHDrawObject &drawObject)
{
	m_ListDrawObject.push_back(drawObject);
}

void CCHImageView::DeleteAllDrawObject(void)
{
	ListDrawObjectIt it = m_ListDrawObject.begin();
	while (it != m_ListDrawObject.end()) 
	{
		m_ListDrawObject.erase(it++);
	}
}

void CCHImageView::DeleteAllDrawObject(int nType)
{
	ListDrawObjectIt it = m_ListDrawObject.begin();
	while (it != m_ListDrawObject.end()) 
	{
		if (it->nObjectType==nType)
		{
			m_ListDrawObject.erase(it++);
			continue;
		}
		it++;
	}
}

void CCHImageView::DeleteDrawObject(const CRect& rtRect)
{
	ListDrawObjectIt it = m_ListDrawObject.begin();
	while (it != m_ListDrawObject.end()) 
	{
		CRect rtCur = CRect(it->nLeft, it->nTop, it->nRight, it->nBottom);
		if (rtCur==rtRect)
		{
			m_ListDrawObject.erase(it++);
			continue;
		}
		it++;
	}
}

CCHDrawObject* CCHImageView::GetDrawObject(UINT nIndex)
{
	if (nIndex >= m_ListDrawObject.size())
	{
		return NULL;
	}

	UINT nCount = 0;
	ListDrawObjectIt it = m_ListDrawObject.begin();
	while (it != m_ListDrawObject.end()) 
	{
		if (nIndex==nCount)
		{
			return &(*it);
		}
		
		nCount++;
		it++;
	}

	return NULL;
}

void CCHImageView::DeleteTailDrawObject(void)
{
	if (m_ListDrawObject.size()<1) return;

	UINT nSize = (UINT)(m_ListDrawObject.size()-1);
	ListDrawObjectIt it = m_ListDrawObject.begin();

	UINT nIndex = 0;
	while (it != m_ListDrawObject.end()) 
	{
		if ((nIndex++)==nSize)
		{
			m_ListDrawObject.erase(it++);
		}
		else
		{
			it++;
		}
	}

}

void CCHImageView::OnDestroy()
{
	__super::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DeleteAllDrawObject();
}

void CCHImageView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
			m_nHScroll -= 10; //m_nHScroll--;
		break;
	case SB_LINERIGHT:   // Scroll right.
		if (m_nHScroll < m_nMaxHScroll)
			m_nHScroll += 10; //m_nHScroll++;
		break;
	case SB_PAGELEFT:    // Scroll one page left.
		if (5 < m_nMaxHScroll / 256)
			m_nHScroll -= 40;//m_nHScroll -= 5;
		else
			m_nHScroll -= 20;//m_nHScroll -= m_nMaxHScroll / 256;

		if (m_nHScroll < 0)
			m_nHScroll = 0;
		break;
	case SB_PAGERIGHT:      // Scroll one page right.
		if (5 < m_nMaxHScroll / 256)
			m_nHScroll += 40;//m_nHScroll += 5;
		else
			m_nHScroll += 20;//m_nHScroll += m_nMaxHScroll / 256;

		if (m_nHScroll > m_nMaxHScroll)
			m_nHScroll = m_nMaxHScroll;
		break;
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		break;
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		m_nHScroll = nPos;
		TRACE(_T("Horz SB_THUMBTRACK : %d\n"), nPos);
		break;
	}

	// Set the new position of the thumb (scroll box).
	SetScrollPos(SB_HORZ, m_nHScroll);

	Invalidate(FALSE);

	__super::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCHImageView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nSBCode)
	{
	case SB_BOTTOM:			// Scroll to bottom. 
		break;
	case SB_ENDSCROLL:		// End scroll.
		break;
	case SB_LINEDOWN:		// Scroll one line down.
		if (m_nVScroll < m_nMaxVScroll)
			m_nVScroll += 10; //m_nVScroll++;
		break;
	case SB_LINEUP:			// Scroll one line up. 
		if (m_nVScroll > 0)
			m_nVScroll -= 10; //m_nVScroll--;
		break;
	case SB_PAGEDOWN:		// Scroll one page down. 
		if (5 < m_nMaxVScroll / 256)
			m_nVScroll += 40;//m_nVScroll += 5;
		else
			m_nVScroll += 20;//m_nVScroll += (m_nMaxVScroll / 256);

		if (m_nVScroll > m_nMaxVScroll)
			m_nVScroll = m_nMaxVScroll;
		break;
	case SB_PAGEUP:			// Scroll one page up.
		if (5 < m_nMaxVScroll / 256)
			m_nVScroll -= 40;//m_nVScroll -= 5;
		else
			m_nVScroll -= 20;//m_nVScroll -= (m_nMaxVScroll / 256);

		if (m_nVScroll < 0)
			m_nVScroll = 0;
		break;
	case SB_THUMBPOSITION:	// Scroll to the absolute position. The current position is provided in nPos. 
		break;
	case SB_THUMBTRACK:		// Drag scroll box to specified position. The current position is provided in nPos. 
		m_nVScroll = nPos;
		TRACE(_T("Vert SB_THUMBTRACK : %d\n"), nPos);
		break;
	case SB_TOP:			// Scroll to top. 
		break;
	}

	// Set the new position of the thumb (scroll box).
	SetScrollPos(SB_VERT, m_nVScroll);

	Invalidate(FALSE);

	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CCHImageView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;

//	return __super::OnEraseBkgnd(pDC);
}

BOOL CCHImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nFlags == MK_SHIFT)
	{
		if (zDelta<=0)
		{
			DWORD dw = SB_PAGERIGHT;
			SendMessage(WM_HSCROLL, dw, 0);
		}
		else
		{
			DWORD dw = SB_PAGELEFT;
			SendMessage(WM_HSCROLL, dw, 0);
		}
	}
	else
	{
		if (zDelta<=0)
		{
			DWORD dw = SB_PAGEDOWN;
			SendMessage(WM_VSCROLL, dw, 0);
		}
		else
		{
			DWORD dw = SB_PAGEUP;
			SendMessage(WM_VSCROLL, dw, 0);
		}
	}

	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

