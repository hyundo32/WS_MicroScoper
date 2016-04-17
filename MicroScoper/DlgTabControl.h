#pragma once

#include "DialogInterface.h"
#include "afxcmn.h"
#include <vector>
// CDlgTabControl ��ȭ �����Դϴ�.


struct STabWndData
{   
	STabWndData(CWnd* pPtr, const CString& strName)
	{
		pWndPtr = pPtr;
		strWndName = strName;
	}

	CWnd*	pWndPtr;
	CString	strWndName;
};

typedef std::vector<STabWndData>					VectorTabWndData;
typedef std::vector<STabWndData>::iterator			VectorTabWndDataIt;
typedef std::vector<STabWndData>::const_iterator	constVectorTabWndDataIt;

class CDlgTabControl : public CDialog, public CDialogInterface
{
	DECLARE_DYNAMIC(CDlgTabControl)

public:
	CDlgTabControl(CWnd* pParent = NULL, COLORREF nDialogColor=RGB(255,255,255));   // ǥ�� �������Դϴ�.
	virtual ~CDlgTabControl();
	void SetIDTC2P(IDlgTabControl2Parent* pIDTC2P) { m_pDlgTabControl2Parent = pIDTC2P; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_TAB_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	void AddTabWndData(CWnd *pWnd, const CString& strName);


protected:
	void InitTabControl();

	IDlgTabControl2Parent*		m_pDlgTabControl2Parent;
	VectorTabWndData			m_vecTabWndData;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CTabCtrl m_ctrlTabControl;
	afx_msg void OnTcnSelchangeTabControl(NMHDR *pNMHDR, LRESULT *pResult);
};
