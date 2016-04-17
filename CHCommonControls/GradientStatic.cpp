// GradientStatic.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GradientStatic.h"


// CGradientStatic

IMPLEMENT_DYNAMIC(CGradientStatic, CStatic)

CGradientStatic::CGradientStatic()
{
	m_bVertical = FALSE;
	m_iLeftSpacing = 10;
	clLeft = GetSysColor(COLOR_ACTIVECAPTION);
	clRight = GetSysColor(COLOR_BTNFACE);
	clText = GetSysColor(COLOR_CAPTIONTEXT);
	
	m_iAlign = 0;

	hinst_msimg32 = LoadLibrary( _T("msimg32.dll") );

	m_bCanDoGradientFill = FALSE;
		
	if(hinst_msimg32)
	{
		m_bCanDoGradientFill = TRUE;		
		dllfunc_GradientFill = ((LPFNDLLFUNC1) GetProcAddress( hinst_msimg32, "GradientFill" ));
	}
	
	m_sTEXT = _T("");
}

CGradientStatic::~CGradientStatic()
{
	FreeLibrary( hinst_msimg32 );
}


BEGIN_MESSAGE_MAP(CGradientStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CGradientStatic 메시지 처리기입니다
//this function will be used only if msimg32.dll library is not available
void CGradientStatic::DrawGradRect(CDC *pDC, CRect r, COLORREF cLeft, COLORREF cRight, BOOL a_bVertical)
{
	CRect stepR;					// rectangle for color's band
	COLORREF color;				// color for the bands
	float fStep;
	
	if(a_bVertical)
		fStep = ((float)r.Height())/255.0f;	
	else
		fStep = ((float)r.Width())/255.0f;	// width of color's band
	
	for (int iOnBand = 0; iOnBand < 255; iOnBand++) 
	{
		// set current band
		if(a_bVertical)
		{
			SetRect(&stepR,
				r.left, 
				r.top+(int)(iOnBand * fStep),
				r.right, 
				r.top+(int)((iOnBand+1)* fStep));	
		}
		else
		{
			SetRect(&stepR,
				r.left+(int)(iOnBand * fStep), 
				r.top,
				r.left+(int)((iOnBand+1)* fStep), 
				r.bottom);	
		}

		// set current color
		color = RGB((GetRValue(cRight)-GetRValue(cLeft))*((float)iOnBand)/255.0f+GetRValue(cLeft),
			(GetGValue(cRight)-GetGValue(cLeft))*((float)iOnBand)/255.0f+GetGValue(cLeft),
			(GetBValue(cRight)-GetBValue(cLeft))*((float)iOnBand)/255.0f+GetBValue(cLeft));
		// fill current band
		pDC->FillSolidRect(stepR,color);
	}
}


void CGradientStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	
	if(m_bCanDoGradientFill) //msimg32.dll library is loaded
	{
	
		TRIVERTEX rcVertex[2];
		rcVertex[0].x=rect.left;
		rcVertex[0].y=rect.top;
		rcVertex[0].Red=GetRValue(clLeft)<<8;	// color values from 0x0000 to 0xff00 !!!!
		rcVertex[0].Green=GetGValue(clLeft)<<8;
		rcVertex[0].Blue=GetBValue(clLeft)<<8;
		rcVertex[0].Alpha=0x0000;
		rcVertex[1].x=rect.right; 
		rcVertex[1].y=rect.bottom;
		rcVertex[1].Red=GetRValue(clRight)<<8;
		rcVertex[1].Green=GetGValue(clRight)<<8;
		rcVertex[1].Blue=GetBValue(clRight)<<8;
		rcVertex[1].Alpha=0;
		
		GRADIENT_RECT rect;
		rect.UpperLeft=0;
		rect.LowerRight=1;
		
		// fill the area 
		dllfunc_GradientFill( dc,rcVertex,2,&rect,1, m_bVertical ? GRADIENT_FILL_RECT_V : GRADIENT_FILL_RECT_H);
		
	}
	else
	{
		//msimg32.dll is not available. Let's use our own code to display gradient background.
		//This code is very simple and produces worse gradient that function from the library - but works!
		DrawGradRect(&dc,rect,clLeft,clRight,m_bVertical);
	}
	
	//let's set color defined by user
	::SetTextColor(dc,clText);

	HFONT hfontOld;

	CFont font;
	VERIFY(font.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BLACK,					// nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("MS Sans Serif")));                 // lpszFacename

	hfontOld = (HFONT)SelectObject(dc.m_hDC, font.m_hObject);

	::SetBkMode(dc, TRANSPARENT);
	GetClientRect(&rect);

	if(m_iAlign == 1) // center
		::DrawText(dc, m_sTEXT, -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	else if(m_iAlign == 0) // left
	{
		rect.left+=m_iLeftSpacing;
		::DrawText(dc, m_sTEXT, -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	}
	else //right
	{
		rect.right-=m_iLeftSpacing;
		::DrawText(dc, m_sTEXT, -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
	}

	::SelectObject(dc.m_hDC, hfontOld);
}


void CGradientStatic::SetReverseGradient()
{
	COLORREF cTemp = clLeft;
	clLeft = clRight;
	clRight = cTemp;
}

void CGradientStatic::SetWindowText(LPCTSTR a_lpstr)
{
//	CStatic::SetWindowText(a_lpstr);

	m_sTEXT = a_lpstr;

	Invalidate(TRUE);
}



