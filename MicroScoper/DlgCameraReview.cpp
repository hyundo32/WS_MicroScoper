// DlgCameraReview.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MicroScoper.h"
#include "DlgCameraReview.h"
#include "afxdialogex.h"

#include "CameraViewReview.h"


// CDlgCameraReview 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCameraReview, CDialog)

CDlgCameraReview::CDlgCameraReview(CWnd* pParent, COLORREF nDialogColor)
	: CDialog(CDlgCameraReview::IDD, pParent), CDialogInterface(nDialogColor)
{
	m_pDlgCameraReview2Parent	= NULL;
}

CDlgCameraReview::~CDlgCameraReview()
{
}

void CDlgCameraReview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CAMERA_VIEW, m_ctrlCameraView);
}


BEGIN_MESSAGE_MAP(CDlgCameraReview, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCameraReview::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCameraReview::OnBnClickedCancel)
	ON_MESSAGE(IDR_CHANGE_VIEW,&CDlgCameraReview::OnChangeView)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgCameraReview 메시지 처리기입니다.
LRESULT CDlgCameraReview::OnChangeView(WPARAM wParam, LPARAM lParam)
{
	if (m_pDlgCameraReview2Parent)
	{
		m_pDlgCameraReview2Parent->IDRC2P_ChangeCameraView();
		return 1;
	}

	return 0;
}


void CDlgCameraReview::OnBnClickedOk()
{

}


void CDlgCameraReview::OnBnClickedCancel()
{

}


BOOL CDlgCameraReview::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctrlCameraView.SetParentWnd(this);
	m_ctrlCameraView.SetViewName(_T("RV"));

	return TRUE;  // return TRUE unless you set the focus to a control
}


HBRUSH CDlgCameraReview::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH) m_hControlColor[nCtlColor];
	}
	return hbr;
}


void CDlgCameraReview::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(rect);

	if (m_ctrlCameraView.GetSafeHwnd())
	{
		m_ctrlCameraView.SetWindowPos(NULL, 0, 0, rect.Width()-2, rect.Height()-2, SWP_NOZORDER);
	}
}

BOOL CDlgCameraReview::DestroyWindow()
{
	return __super::DestroyWindow();
}
