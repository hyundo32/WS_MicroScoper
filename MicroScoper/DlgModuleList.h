#pragma once

#include "DialogInterface.h"
#include "afxwin.h"
#include "GridCtrl.h"

// CDlgModuleList ��ȭ �����Դϴ�.

class CDlgModuleList : public CDialog, public CDialogInterface
{
	DECLARE_DYNAMIC(CDlgModuleList)

public:
	CDlgModuleList(CWnd* pParent = NULL, COLORREF nDialogColor=RGB(255,255,255));   // ǥ�� �������Դϴ�.
	virtual ~CDlgModuleList();
	void SetIDML2P(IDlgModuleList2Parent* pIDML2P) { m_pDlgModuleList2Parent = pIDML2P; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_MODULE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
