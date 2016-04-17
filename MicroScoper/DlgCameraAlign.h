#pragma once

#include <vector>
#include "DialogInterface.h"
#include "GridCtrl.h"
#include "CameraViewAlign.h"

class CDlgCameraAlign : public CDialog, public CDialogInterface
{
	DECLARE_DYNAMIC(CDlgCameraAlign)

public:
	CDlgCameraAlign(CWnd* pParent = NULL, COLORREF nDialogColor=RGB(255,255,255));   // ǥ�� �������Դϴ�.
	virtual ~CDlgCameraAlign();
	void SetIDCA2P(IDlgCameraAlign2Parent* pIDCA2P) { m_pDlgCameraAlign2Parent = pIDCA2P; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_CAMERA_ALIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnChangeView(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:
	void	InitGridOriginInfo();
	void	InitGridAlignInfo();

	IDlgCameraAlign2Parent*		m_pDlgCameraAlign2Parent;

	CCameraView					m_ctrlTemplateView[2];
	CCameraViewAlign			m_ctrlCameraView[2];

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL DestroyWindow();
	CGridCtrl m_ctrlOriginInfo;
	CGridCtrl m_ctrlAlignInfo;
};
