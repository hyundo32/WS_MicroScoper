#pragma once

#include "DialogInterface.h"
#include "afxwin.h"
#include "GridCtrl.h"

// CDlgModuleList 대화 상자입니다.

class CDlgModuleList : public CDialog, public CDialogInterface
{
	DECLARE_DYNAMIC(CDlgModuleList)

public:
	CDlgModuleList(CWnd* pParent = NULL, COLORREF nDialogColor=RGB(255,255,255));   // 표준 생성자입니다.
	virtual ~CDlgModuleList();
	void SetIDML2P(IDlgModuleList2Parent* pIDML2P) { m_pDlgModuleList2Parent = pIDML2P; }

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MODULE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	void InitGridModuleList();
	void SetGridModuleList();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGridDoubleClick(NMHDR *pNotifyStruct, LRESULT* pResult);

protected:
	IDlgModuleList2Parent*		m_pDlgModuleList2Parent;
	CGridCtrl					m_ctrlGridModuleList;
	int							m_nSelectedModuleIndex;

};
