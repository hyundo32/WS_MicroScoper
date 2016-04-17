// DlgModuleList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MicroScoper.h"
#include "DlgModuleList.h"
#include "afxdialogex.h"


// CDlgModuleList 대화 상자입니다.
#define MODULE_INFO_GRID_ROW_COUNT		5
#define MODULE_INFO_GRID_COL_COUNT		10


IMPLEMENT_DYNAMIC(CDlgModuleList, CDialog)

CDlgModuleList::CDlgModuleList(CWnd* pParent, COLORREF nDialogColor)
	: CDialog(CDlgModuleList::IDD, pParent), CDialogInterface(nDialogColor)
{
	m_pDlgModuleList2Parent		= NULL;
	m_nSelectedModuleIndex		= -1;
}

CDlgModuleList::~CDlgModuleList()
{
}

void CDlgModuleList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_MODULE_LIST, m_ctrlGridModuleList);
}


BEGIN_MESSAGE_MAP(CDlgModuleList, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgModuleList::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgModuleList::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_GRID_MODULE_LIST, &CDlgModuleList::OnGridDoubleClick)
END_MESSAGE_MAP()


// CDlgModuleList 메시지 처리기입니다.


void CDlgModuleList::OnBnClickedOk()
{

}

void CDlgModuleList::OnBnClickedCancel()
{

}

void CDlgModuleList::OnGridDoubleClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
 	CCellID FocusedID = m_ctrlGridModuleList.GetFocusCell();
	int nIdx = FocusedID.row-1;
	if (nIdx<0 || nIdx==m_nSelectedModuleIndex || nIdx>=(MODULE_INFO_GRID_ROW_COUNT-1))
	{
		return;
	}
	
	if (m_nSelectedModuleIndex>-1)
	{
		for(int nColIdx = 3; nColIdx < MODULE_INFO_GRID_COL_COUNT; nColIdx++)
		{
			m_ctrlGridModuleList.SetItemBkColour(m_nSelectedModuleIndex+1,nColIdx,RGB(255,255,255));
		}
	}

	
	m_nSelectedModuleIndex = nIdx;

	if (m_pDlgModuleList2Parent)
		m_pDlgModuleList2Parent->IDMI2P_SetSelectedModuleIndex(m_nSelectedModuleIndex);

	for(int nColIdx = 3; nColIdx < 10; nColIdx++)
	{
		m_ctrlGridModuleList.SetItemBkColour(m_nSelectedModuleIndex+1, nColIdx,RGB(255,255,0));
	//	m_ctrlGridModuleList.SetItemBkColour(0,nColIdx,RGB(180, 210, 247));
	}

	m_ctrlGridModuleList.Invalidate(FALSE);
}

HBRUSH CDlgModuleList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

// 	if (nCtlColor == CTLCOLOR_DLG)
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		return (HBRUSH) m_hControlColor[nCtlColor];
// 	}

	return hbr;
}


void CDlgModuleList::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	
	return;

	if (m_ctrlGridModuleList.GetSafeHwnd())
	{
		m_ctrlGridModuleList.SetWindowPos(NULL, 1, 1, cx-2, cy-2, SWP_NOZORDER);
	}
}


BOOL CDlgModuleList::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitGridModuleList();

	SetGridModuleList();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgModuleList::SetGridModuleList()
{

}

void CDlgModuleList::InitGridModuleList()
{
	int nRows = MODULE_INFO_GRID_ROW_COUNT;
	int nCols = MODULE_INFO_GRID_COL_COUNT;
	int nFixRows = 1;
	int nFixCols = 3;
	int nRowIdx = 0;
	int nColIdx = 0;

	m_ctrlGridModuleList.GetDefaultCell(TRUE, FALSE)->SetBackClr(RGB(144, 200, 246));
	m_ctrlGridModuleList.GetDefaultCell(FALSE, TRUE)->SetBackClr(RGB(144, 200, 246));;
	m_ctrlGridModuleList.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));

	m_ctrlGridModuleList.SetRowCount(nRows);
	m_ctrlGridModuleList.SetColumnCount(nCols);
	m_ctrlGridModuleList.SetFixedRowCount(nFixRows);
	m_ctrlGridModuleList.SetFixedColumnCount(nFixCols);

	GV_ITEM Item;
	CString strTemp = _T("");

	m_ctrlGridModuleList.SetColumnWidth(nColIdx, 50);
	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx;
	Item.col = nColIdx++;
	strTemp.Format(_T("Module"));
	Item.strText = strTemp;
	m_ctrlGridModuleList.SetItem(&Item);

	m_ctrlGridModuleList.SetColumnWidth(nColIdx, 40);
	Item.col = nColIdx++;
	strTemp.Format(_T("G_Idx"));
	Item.strText = strTemp;
	m_ctrlGridModuleList.SetItem(&Item);

	m_ctrlGridModuleList.SetColumnWidth(nColIdx, 40);
	Item.col = nColIdx++;
	strTemp.Format(_T("H_Idx"));
	Item.strText = strTemp;
	m_ctrlGridModuleList.SetItem(&Item);

	m_ctrlGridModuleList.SetColumnWidth(nColIdx, 85);
	Item.col = nColIdx++;
	strTemp.Format(_T("Motor X(mm)"));
	Item.strText = strTemp;
	m_ctrlGridModuleList.SetItem(&Item);

	m_ctrlGridModuleList.SetColumnWidth(nColIdx, 85);
	Item.col = nColIdx++;
	strTemp.Format(_T("Motor Y(mm)"));
	Item.strText = strTemp;
	m_ctrlGridModuleList.SetItem(&Item);

	m_ctrlGridModuleList.SetColumnWidth(nColIdx, 85);
	Item.col = nColIdx++;
	strTemp.Format(_T("Glass X(mm)"));
	Item.strText = strTemp;
	m_ctrlGridModuleList.SetItem(&Item);

	m_ctrlGridModuleList.SetColumnWidth(nColIdx, 85);
	Item.col = nColIdx++;
	strTemp.Format(_T("Glass Y(mm)"));
	Item.strText = strTemp;
	m_ctrlGridModuleList.SetItem(&Item);

	m_ctrlGridModuleList.SetColumnWidth(nColIdx, 68);
	Item.col = nColIdx++;
	strTemp.Format(_T("Mag Level"));
	Item.strText = strTemp;
	m_ctrlGridModuleList.SetItem(&Item);


	m_ctrlGridModuleList.SetColumnWidth(nColIdx, 68);
	Item.col = nColIdx++;
	strTemp.Format(_T("Light Level"));
	Item.strText = strTemp;
	m_ctrlGridModuleList.SetItem(&Item);

	m_ctrlGridModuleList.SetColumnWidth(nColIdx, 68);
	Item.col = nColIdx++;
	strTemp.Format(_T("AFM Level"));
	Item.strText = strTemp;
	m_ctrlGridModuleList.SetItem(&Item);

	m_ctrlGridModuleList.SetEditable(FALSE);	
}


