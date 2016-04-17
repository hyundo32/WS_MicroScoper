#pragma once

#include "DialogInterface.h"
#include "afxwin.h"
#include "CameraViewReview.h"

class CDlgCameraReview : public CDialog, public CDialogInterface
{
	DECLARE_DYNAMIC(CDlgCameraReview)

public:
	CDlgCameraReview(CWnd* pParent = NULL, COLORREF nDialogColor=RGB(255,255,255));   // 표준 생성자입니다.
	virtual ~CDlgCameraReview();
	void SetIDCR2P(IDlgCameraReview2Parent* pIDRA2P) { m_pDlgCameraReview2Parent = pIDRA2P; }

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_CAMERA_REVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnChangeView(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:
	IDlgCameraReview2Parent*	m_pDlgCameraReview2Parent;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL DestroyWindow();

	CCameraViewReview			m_ctrlCameraView;
};
