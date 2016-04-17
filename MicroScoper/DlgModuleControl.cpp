// DlgModuleControl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MicroScoper.h"
#include "DlgModuleControl.h"
#include "afxdialogex.h"


// CDlgModuleControl 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgModuleControl, CDialog)

CDlgModuleControl::CDlgModuleControl(CWnd* pParent, COLORREF nDialogColor)
	: CDialog(CDlgModuleControl::IDD, pParent), CDialogInterface(nDialogColor)
{
	m_pDlgModuleControl2Parent	= NULL;
	m_nMagnificLevel = 0;
}

CDlgModuleControl::~CDlgModuleControl()
{
}

void CDlgModuleControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,	IDC_SLIDER_LIGHT_LEVEL,			m_ctrlLightLevel);
	DDX_Control(pDX,	IDC_SLIDER_JOG_SPEED_LEVEL,		m_ctrlJogSpeedLevel);
	DDX_Control(pDX,	IDC_CHECK_CAMERA_CONTROL,		m_ctrlCameraControl);
	DDX_Control(pDX,	IDC_CHECK_AFM_TRACK,			m_ctrlAFMTracking);
	DDX_Radio(pDX,		IDC_RADIO_MAG_LEVEL_0,			m_nMagnificLevel);
	DDX_Control(pDX, IDC_GRID_MOTOR_GO_POS, m_ctrlMotoGoPos);
	DDX_Control(pDX, IDC_GRID_GLASS_GO_POS, m_ctrlGlassGoPos);
	DDX_Control(pDX, IDC_GRID_ROTATE_MOTOR_POS, m_ctrlRotateMotorGoPos);
}


BEGIN_MESSAGE_MAP(CDlgModuleControl, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgModuleControl::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgModuleControl::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LIGHT_LEVEL, &CDlgModuleControl::OnNMReleasedcaptureSliderLightLevel)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_JOG_SPEED_LEVEL, &CDlgModuleControl::OnNMReleasedcaptureSliderJogSpeedLevel)
END_MESSAGE_MAP()


// CDlgModuleControl 메시지 처리기입니다.


void CDlgModuleControl::OnBnClickedOk()
{
}


void CDlgModuleControl::OnBnClickedCancel()
{
}

BOOL CDlgModuleControl::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitGridControls();

	CString tmp = _T("");

	// slider light level
	m_ctrlLightLevel.SetRange(0, 1000);
	m_ctrlLightLevel.SetRange(0, 1000);
	m_ctrlLightLevel.SetRangeMin(0);
	m_ctrlLightLevel.SetRangeMax(1000);
	m_ctrlLightLevel.SetTicFreq(1);
	m_ctrlLightLevel.SetLineSize(5);
	m_ctrlLightLevel.SetPageSize(50);
	m_ctrlLightLevel.SetPos(200);

	tmp.Format(_T("%.1lf%%"), m_ctrlLightLevel.GetPos() / 10.0);
	SetDlgItemText(IDC_STATIC_LIGHT_LEVEL, tmp);


	// slider jog speed
	m_ctrlJogSpeedLevel.SetRange(0, 1000);
	m_ctrlJogSpeedLevel.SetRangeMin(0);
	m_ctrlJogSpeedLevel.SetRangeMax(1000);
	m_ctrlJogSpeedLevel.SetTicFreq(1);
	m_ctrlJogSpeedLevel.SetLineSize(5);
	m_ctrlJogSpeedLevel.SetPageSize(50);
	m_ctrlJogSpeedLevel.SetPos(200);

	tmp.Format(_T("%.1lf%%"), m_ctrlJogSpeedLevel.GetPos() / 10.0);
	SetDlgItemText(IDC_STATIC_JOG_SPEED_LEVEL, tmp);

	return TRUE;  
}


HBRUSH CDlgModuleControl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

// 	pDC->SetBkMode(TRANSPARENT);
// 	return (HBRUSH) m_hControlColor[nCtlColor];
// 
// 	if (nCtlColor == CTLCOLOR_DLG)
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		return (HBRUSH) m_hControlColor[nCtlColor];
// 	}
	return hbr;
}


void  CDlgModuleControl::InitGridControls()
{
#define GO_POS_GRID_ROW_COUNT		1
#define GO_POS_GRID_COL_COUNT		2

	int nRows = GO_POS_GRID_ROW_COUNT;
	int nCols = GO_POS_GRID_COL_COUNT;

	// motor pos
	m_ctrlMotoGoPos.GetDefaultCell(TRUE, FALSE)->SetBackClr(RGB(144, 200, 246));
	m_ctrlMotoGoPos.GetDefaultCell(FALSE, TRUE)->SetBackClr(RGB(144, 200, 246));;
	m_ctrlMotoGoPos.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));

	m_ctrlMotoGoPos.SetRowCount(nRows);
	m_ctrlMotoGoPos.SetColumnCount(nCols);
	m_ctrlMotoGoPos.SetFixedRowCount(0);
	m_ctrlMotoGoPos.SetFixedColumnCount(0);

	m_ctrlMotoGoPos.ExpandToFit(TRUE);

	// glass pos
	m_ctrlGlassGoPos.GetDefaultCell(TRUE, FALSE)->SetBackClr(RGB(144, 200, 246));
	m_ctrlGlassGoPos.GetDefaultCell(FALSE, TRUE)->SetBackClr(RGB(144, 200, 246));;
	m_ctrlGlassGoPos.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));

	m_ctrlGlassGoPos.SetRowCount(nRows);
	m_ctrlGlassGoPos.SetColumnCount(nCols);
	m_ctrlGlassGoPos.SetFixedRowCount(0);
	m_ctrlGlassGoPos.SetFixedColumnCount(0);

	m_ctrlGlassGoPos.ExpandToFit(TRUE);

	// rotate motor pos
	m_ctrlRotateMotorGoPos.GetDefaultCell(TRUE, FALSE)->SetBackClr(RGB(144, 200, 246));
	m_ctrlRotateMotorGoPos.GetDefaultCell(FALSE, TRUE)->SetBackClr(RGB(144, 200, 246));;
	m_ctrlRotateMotorGoPos.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));

	m_ctrlRotateMotorGoPos.SetRowCount(nRows);
	m_ctrlRotateMotorGoPos.SetColumnCount(nCols);
	m_ctrlRotateMotorGoPos.SetFixedRowCount(0);
	m_ctrlRotateMotorGoPos.SetFixedColumnCount(1);

	m_ctrlRotateMotorGoPos.ExpandToFit(TRUE);
}

void CDlgModuleControl::OnNMReleasedcaptureSliderLightLevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pDlgModuleControl2Parent==NULL) return;

	double dValue = m_ctrlLightLevel.GetPos() / 1000.0;

	CString tmp = _T("");
	tmp.Format(_T("%.1lf%%"), m_ctrlLightLevel.GetPos() / 10.0);
	SetDlgItemText(IDC_STATIC_LIGHT_LEVEL, tmp);

	*pResult = 0;
}

void CDlgModuleControl::OnNMReleasedcaptureSliderJogSpeedLevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pDlgModuleControl2Parent==NULL) return;

	double dValue = m_ctrlJogSpeedLevel.GetPos() / 1000.0;
	
	CString tmp = _T("");
	tmp.Format(_T("%.1lf%%"), m_ctrlJogSpeedLevel.GetPos() / 10.0);
	SetDlgItemText(IDC_STATIC_JOG_SPEED_LEVEL, tmp);

	*pResult = 0;
}




