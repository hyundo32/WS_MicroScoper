// DlgCameraAlign.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MicroScoper.h"
#include "DlgCameraAlign.h"
#include "afxdialogex.h"
#include "CameraViewAlign.h"


// CDlgCameraAlign 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCameraAlign, CDialog)

CDlgCameraAlign::CDlgCameraAlign(CWnd* pParent, COLORREF nDialogColor)
	: CDialog(CDlgCameraAlign::IDD, pParent), CDialogInterface(nDialogColor)
{
	m_pDlgCameraAlign2Parent	= NULL;
}

CDlgCameraAlign::~CDlgCameraAlign()
{
}

void CDlgCameraAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_ORIGIN_INFO, m_ctrlOriginInfo);
	DDX_Control(pDX, IDC_GRID_ALIGN_INFO, m_ctrlAlignInfo);

	DDX_Control(pDX, IDC_STATIC_CAMERA_VIEW_0, m_ctrlCameraView[0]);
	DDX_Control(pDX, IDC_STATIC_CAMERA_VIEW_1, m_ctrlCameraView[1]);

	DDX_Control(pDX, IDC_STATIC_TEMPLATE_VIEW_0, m_ctrlTemplateView[0]);
	DDX_Control(pDX, IDC_STATIC_TEMPLATE_VIEW_1, m_ctrlTemplateView[1]);
}


BEGIN_MESSAGE_MAP(CDlgCameraAlign, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCameraAlign::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCameraAlign::OnBnClickedCancel)
	ON_MESSAGE(IDR_CHANGE_VIEW, &CDlgCameraAlign::OnChangeView)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgCameraAlign 메시지 처리기입니다.

LRESULT CDlgCameraAlign::OnChangeView(WPARAM wParam, LPARAM lParam)
{
	if (m_pDlgCameraAlign2Parent)
	{
		m_pDlgCameraAlign2Parent->IDAC2P_ChangeCameraView();
		return 1;
	}
	return 0;
}


void CDlgCameraAlign::OnBnClickedOk()
{

}

void CDlgCameraAlign::OnBnClickedCancel()
{

}

BOOL CDlgCameraAlign::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_ctrlCameraView[0].SetViewName(_T("1st"));
	m_ctrlCameraView[1].SetViewName(_T("2nd"));

	m_ctrlCameraView[0].SetParentWnd(this);
	m_ctrlCameraView[1].SetParentWnd(this);

	m_ctrlTemplateView[0].SetParentWnd(this);
	m_ctrlTemplateView[1].SetParentWnd(this);

// 
// 
// 	CRect rect;
// 	GetClientRect(rect);
// 
// 	int nViewWidth		= (rect.Width() - 3) / 2;
// 	int nViewHeight		= int(nViewWidth * (3.0 / 4.0));
// 
// 	CCameraViewAlign *pFirstView = new CCameraViewAlign(0);
// 	if (pFirstView)
// 	{
// 		rect.left		= 1;
// 		rect.top		= 1; 
// 		rect.right		= rect.left + nViewWidth;
// 		rect.bottom		= rect.top + nViewHeight;
// 		pFirstView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WM_NOTIFY , rect, this, 0);
// 		pFirstView->SetParentWnd(this);
// 		pFirstView->SetViewName(_T("1st"));
// 
// 		pFirstView->ShowWindow(SW_SHOW);
// 		m_vecCameraViewAlign.push_back(pFirstView);
// 	}
// 
// 	CCameraViewAlign *pSecondView = new CCameraViewAlign(1);
// 	if (pSecondView)
// 	{
// 		rect.left		= rect.right + 1;
// 		rect.top		= 1; 
// 		rect.right		= rect.left + nViewWidth;
// 		rect.bottom		= rect.top + nViewHeight;
// 		pSecondView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WM_NOTIFY , rect, this, 0);
// 		pSecondView->SetParentWnd(this);
// 		pSecondView->SetViewName(_T("2nd"));
// 
// 		pSecondView->ShowWindow(SW_SHOW);
// 		m_vecCameraViewAlign.push_back(pSecondView);
// 	}
// 
// 
// 	int nTempWidth		= 112;
// 	int nTempHeight		= 112;
// 	CCameraView *pFirstTemp = new CCameraView(0);
// 	if (pFirstView)
// 	{
// 		rect.left		= 1;
// 		rect.top		= 1; 
// 		rect.right		= rect.left + nTempWidth;
// 		rect.bottom		= rect.top + nTempHeight;
// 		pFirstTemp->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WM_NOTIFY , rect, this, 0);
// 		pFirstTemp->SetParentWnd(this);
// 		pFirstTemp->ShowWindow(SW_SHOW);
// 		m_vecTemplateView.push_back(pFirstTemp);
// 	}
// 
// 	CCameraView *pSecondTemp = new CCameraView(1);
// 	if (pFirstView)
// 	{
// 		rect.left		= 1;
// 		rect.top		= 1; 
// 		rect.right		= rect.left + nTempWidth;
// 		rect.bottom		= rect.top + nTempHeight;
// 		pSecondTemp->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WM_NOTIFY , rect, this, 0);
// 		pSecondTemp->SetParentWnd(this);
// 		pSecondTemp->ShowWindow(SW_SHOW);
// 		m_vecTemplateView.push_back(pSecondTemp);
// 	}


	InitGridOriginInfo();

	InitGridAlignInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
}


HBRUSH CDlgCameraAlign::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
// 	if (nCtlColor == CTLCOLOR_DLG)
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		return (HBRUSH) m_hControlColor[nCtlColor];
// 	}
	return hbr;
}

void CDlgCameraAlign::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(rect);
	int nViewWidth		= (rect.Width() - 3) / 2;
	int nViewHeight		= int(nViewWidth * (3.0 / 4.0));

	int nStartX = 1;
	int nStartY = 269;
	for (int i=0; i<2; i++)
	{
		if (m_ctrlCameraView[i].GetSafeHwnd())
		{
			m_ctrlCameraView[i].SetWindowPos(NULL, nStartX, nStartY, nViewWidth, nViewHeight, SWP_NOZORDER);
		}
		nStartX += (nViewWidth + 1);
	}


	int nTempWidth		= 112;
	int nTempHeight		= 112;

	nStartX = 1 + nViewWidth - nTempWidth;
	nStartY = nStartY - 113;
	for (int i=0; i<2; i++)
	{
		if (m_ctrlTemplateView[i].GetSafeHwnd())
		{
			m_ctrlTemplateView[i].SetWindowPos(NULL, nStartX, nStartY, nTempWidth, nTempHeight, SWP_NOZORDER);
		}
		nStartX = rect.Width() - nTempWidth - 2;
	}
}


BOOL CDlgCameraAlign::DestroyWindow()
{
	return __super::DestroyWindow();
}

void CDlgCameraAlign::InitGridAlignInfo()
{
	int nRowIdx, nColIdx, nRows, nCols, nFixRows, nFixCols;
	CString strTemp;
	CRect rt;

	nRows = 6;
	nCols = 5;
	nFixRows = 1;
	nFixCols = 1;
	nRowIdx = 0;
	nColIdx = 0;
	
	// 	CFont font;
	// 	VERIFY(font.CreateFont(
	// 		15,                        // nHeight
	// 		4,                         // nWidth
	// 		0,                         // nEscapement
	// 		0,                         // nOrientation
	// 		FW_NORMAL,                 // nWeight
	// 		FALSE,                     // bItalic
	// 		FALSE,                     // bUnderline
	// 		0,                         // cStrikeOut
	// 		ANSI_CHARSET,              // nCharSet
	// 		OUT_DEFAULT_PRECIS,        // nOutPrecision
	// 		CLIP_DEFAULT_PRECIS,       // nClipPrecision
	// 		DEFAULT_QUALITY,           // nQuality
	// 		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	// 		_T("MS Shell Dlg")));      // lpszFacename
	// 
	// 	m_ctrlGlassInfo.SetFont(&font);


	m_ctrlAlignInfo.GetClientRect(&rt);
	m_ctrlAlignInfo.GetDefaultCell(TRUE, FALSE)->SetBackClr(RGB(144, 200, 246));
	m_ctrlAlignInfo.GetDefaultCell(FALSE, TRUE)->SetBackClr(RGB(144, 200, 246));
	m_ctrlAlignInfo.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));

	m_ctrlAlignInfo.SetRowCount(nRows);
	m_ctrlAlignInfo.SetColumnCount(nCols);
	m_ctrlAlignInfo.SetFixedRowCount(nFixRows);
	m_ctrlAlignInfo.SetFixedColumnCount(nFixCols);

	m_ctrlAlignInfo.SetColumnWidth(0, 92);
	m_ctrlAlignInfo.SetColumnWidth(1, 70);
	m_ctrlAlignInfo.SetColumnWidth(2, 70);
	m_ctrlAlignInfo.SetColumnWidth(3, 70);
	m_ctrlAlignInfo.SetColumnWidth(4, 70);

	for (int i=0; i<nCols; i++)
	{
		m_ctrlAlignInfo.SetRowHeight(i,20);
	}


	GV_ITEM Item;

	nRowIdx = 0;
	nColIdx = 0;
	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx++;
	Item.col = nColIdx;
	strTemp.Format(_T(""));
	Item.strText = strTemp;
	m_ctrlAlignInfo.SetItem(&Item);

	// row
	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx++;
	Item.col = nColIdx;
	strTemp.Format(_T("Glass (mm)"));
	Item.strText = strTemp;
	m_ctrlAlignInfo.SetItem(&Item);

	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx++;
	Item.col = nColIdx;
	strTemp.Format(_T("Camera (pxl)"));
	Item.strText = strTemp;
	m_ctrlAlignInfo.SetItem(&Item);

	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx++;
	Item.col = nColIdx;
	strTemp.Format(_T("R Camera (pxl)"));
	Item.strText = strTemp;
	m_ctrlAlignInfo.SetItem(&Item);

	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx++;
	Item.col = nColIdx;
	strTemp.Format(_T("Motor (mm)"));
	Item.strText = strTemp;
	m_ctrlAlignInfo.SetItem(&Item);

	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx++;
	Item.col = nColIdx;
	strTemp.Format(_T("R Motor (mm)"));
	Item.strText = strTemp;
	m_ctrlAlignInfo.SetItem(&Item);

	// col
	nRowIdx = 0;
	nColIdx = 1;
	Item.row = nRowIdx;
	Item.col = nColIdx++;
	strTemp.Format(_T("1st X_Pos"));
	Item.strText = strTemp;
	m_ctrlAlignInfo.SetItem(&Item);

	Item.col = nColIdx++;
	strTemp.Format(_T("1st Y_Pos"));
	Item.strText = strTemp;
	m_ctrlAlignInfo.SetItem(&Item);

	Item.row = nRowIdx;
	Item.col = nColIdx++;
	strTemp.Format(_T("2nd X_Pos"));
	Item.strText = strTemp;
	m_ctrlAlignInfo.SetItem(&Item);

	Item.col = nColIdx++;
	strTemp.Format(_T("2nd Y_Pos"));
	Item.strText = strTemp;
	m_ctrlAlignInfo.SetItem(&Item);

	m_ctrlAlignInfo.SetEditable(FALSE);
}

void CDlgCameraAlign::InitGridOriginInfo()
{
	int nRowIdx, nColIdx, nRows, nCols, nFixRows, nFixCols;
	CString strTemp;
	CRect rt;

	nRows = 3;
	nCols = 4;
	nFixRows = 1;
	nFixCols = 1;
	nRowIdx = 0;
	nColIdx = 0;


	// 	CFont font;
	// 	VERIFY(font.CreateFont(
	// 		15,                        // nHeight
	// 		4,                         // nWidth
	// 		0,                         // nEscapement
	// 		0,                         // nOrientation
	// 		FW_NORMAL,                 // nWeight
	// 		FALSE,                     // bItalic
	// 		FALSE,                     // bUnderline
	// 		0,                         // cStrikeOut
	// 		ANSI_CHARSET,              // nCharSet
	// 		OUT_DEFAULT_PRECIS,        // nOutPrecision
	// 		CLIP_DEFAULT_PRECIS,       // nClipPrecision
	// 		DEFAULT_QUALITY,           // nQuality
	// 		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	// 		_T("MS Shell Dlg")));      // lpszFacename
	// 
	// 	m_ctrlGlassInfo.SetFont(&font);


	m_ctrlOriginInfo.GetClientRect(&rt);
	m_ctrlOriginInfo.GetDefaultCell(TRUE, FALSE)->SetBackClr(RGB(144, 200, 246));
	m_ctrlOriginInfo.GetDefaultCell(FALSE, TRUE)->SetBackClr(RGB(144, 200, 246));
	m_ctrlOriginInfo.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));

	m_ctrlOriginInfo.SetRowCount(nRows);
	m_ctrlOriginInfo.SetColumnCount(nCols);
	m_ctrlOriginInfo.SetFixedRowCount(nFixRows);
	m_ctrlOriginInfo.SetFixedColumnCount(nFixCols);

	m_ctrlOriginInfo.SetColumnWidth(0, 60);
	m_ctrlOriginInfo.SetColumnWidth(1, 60);
	m_ctrlOriginInfo.SetColumnWidth(2, 60);
	m_ctrlOriginInfo.SetColumnWidth(3, 80);

	for (int i=0; i<nRows; i++)
	{
		m_ctrlOriginInfo.SetRowHeight(i, 25);
	}


	GV_ITEM Item;

	nRowIdx = 0;
	nColIdx = 0;
	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx++;
	Item.col = nColIdx;
	strTemp.Format(_T(""));
	Item.strText = strTemp;
	m_ctrlOriginInfo.SetItem(&Item);

	// row
	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx++;
	Item.col = nColIdx;
	strTemp.Format(_T("Origin"));
	Item.strText = strTemp;
	m_ctrlOriginInfo.SetItem(&Item);

	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx++;
	Item.col = nColIdx;
	strTemp.Format(_T("Result"));
	Item.strText = strTemp;
	m_ctrlOriginInfo.SetItem(&Item);

	// col
	nRowIdx = 0;
	nColIdx = 1;
	Item.row = nRowIdx;
	Item.col = nColIdx++;
	strTemp.Format(_T("X (mm)"));
	Item.strText = strTemp;
	m_ctrlOriginInfo.SetItem(&Item);

	Item.col = nColIdx++;
	strTemp.Format(_T("Y (mm)"));
	Item.strText = strTemp;
	m_ctrlOriginInfo.SetItem(&Item);

	Item.col = nColIdx++;
	strTemp.Format(_T("Degree"));
	Item.strText = strTemp;
	m_ctrlOriginInfo.SetItem(&Item);

	m_ctrlOriginInfo.SetEditable(FALSE);
}