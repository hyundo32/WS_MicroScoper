// DlgTabControl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MicroScoper.h"
#include "DlgTabControl.h"
#include "afxdialogex.h"


// CDlgTabControl 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTabControl, CDialog)

CDlgTabControl::CDlgTabControl(CWnd* pParent, COLORREF nDialogColor)
	: CDialog(CDlgTabControl::IDD, pParent), CDialogInterface(nDialogColor)
{
	m_pDlgTabControl2Parent		= NULL;
}

CDlgTabControl::~CDlgTabControl()
{
}

void CDlgTabControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CONTROL, m_ctrlTabControl);
}


BEGIN_MESSAGE_MAP(CDlgTabControl, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgTabControl::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgTabControl::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CONTROL, &CDlgTabControl::OnTcnSelchangeTabControl)
END_MESSAGE_MAP()


// CDlgTabControl 메시지 처리기입니다.


void CDlgTabControl::OnBnClickedOk()
{

}


void CDlgTabControl::OnBnClickedCancel()
{

}


BOOL CDlgTabControl::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


HBRUSH CDlgTabControl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

	return (HBRUSH) m_hControlColor[nCtlColor];

	if (nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH) m_hControlColor[nCtlColor];
	}
	return hbr;
}


void CDlgTabControl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if (m_ctrlTabControl.GetSafeHwnd())
	{
		m_ctrlTabControl.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}

/*	for (VectorTabWndDataIt it=m_vecTabWndData.begin(); it!=m_vecTabWndData.end(); it++)
	{
		if (it->pWndPtr->GetSafeHwnd())
		{
			it->pWndPtr->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		}
	}
	*/
}

void CDlgTabControl::AddTabWndData(CWnd *pWnd, const CString& strName)
{
	if (pWnd==NULL) return;
	pWnd->SetParent(this);

	m_vecTabWndData.push_back(STabWndData(pWnd, strName));

	InitTabControl();
}

void CDlgTabControl::InitTabControl()
{
	m_ctrlTabControl.DeleteAllItems();

	int nIdx = 0;
	for (VectorTabWndDataIt it=m_vecTabWndData.begin(); it!=m_vecTabWndData.end(); it++)
	{
		if (it->pWndPtr==NULL) continue;

		if (nIdx==0)
		{
			it->pWndPtr->ShowWindow(SW_SHOW);
		}
		else
		{
			it->pWndPtr->ShowWindow(SW_HIDE);
		}

		m_ctrlTabControl.InsertItem(nIdx, it->strWndName);

		nIdx++;
	}

	m_ctrlTabControl.SetCurSel(0);
}


void CDlgTabControl::OnTcnSelchangeTabControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nTabIdx = m_ctrlTabControl.GetCurSel();

	if (nTabIdx<0 || nTabIdx>=(int)m_vecTabWndData.size()) return;

	int nIdx = 0;
	for (VectorTabWndDataIt it=m_vecTabWndData.begin(); it!=m_vecTabWndData.end(); it++)
	{
		if (it->pWndPtr) 
		{
			if (nTabIdx==nIdx)
			{
				it->pWndPtr->ShowWindow(SW_SHOW);
			}
			else
			{
				it->pWndPtr->ShowWindow(SW_HIDE);
			}
		}
		nIdx++;
	}

	*pResult = 0;
}
