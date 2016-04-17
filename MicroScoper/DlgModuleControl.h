#pragma once

#include "DialogInterface.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "GridCtrl.h"

class CDlgModuleControl : public CDialog, public CDialogInterface
{
	DECLARE_DYNAMIC(CDlgModuleControl)

public:
	CDlgModuleControl(CWnd* pParent = NULL, COLORREF nDialogColor=RGB(255,255,255));   // ǥ�� �������Դϴ�.
	virtual ~CDlgModuleControl();
	void SetIDMC2P(IDlgModuleControl2Parent* pIDMC2P) { m_pDlgModuleControl2Parent = pIDMC2P; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_MODULE_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	virtual BOOL OnInitDialog();

protected:
	void	InitGridControls();

	IDlgModuleControl2Parent*	m_pDlgModuleControl2Parent;

protected:
	CSliderCtrl		m_ctrlLightLevel;
	CSliderCtrl		m_ctrlJogSpeedLevel;

	int				m_nMagnificLevel;
	CButton			m_ctrlCameraControl;
	CButton			m_ctrlAFMTracking;

	CGridCtrl		m_ctrlMotoGoPos;
	CGridCtrl		m_ctrlGlassGoPos;
	CGridCtrl		m_ctrlRotateMotorGoPos;
public:
	afx_msg void OnNMReleasedcaptureSliderLightLevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderJogSpeedLevel(NMHDR *pNMHDR, LRESULT *pResult);
};
