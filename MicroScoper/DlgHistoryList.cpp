// DlgHistoryList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MicroScoper.h"
#include "DlgHistoryList.h"
#include "afxdialogex.h"


// CDlgHistoryList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgHistoryList, CDialog)

CDlgHistoryList::CDlgHistoryList(CWnd* pParent, COLORREF nDialogColor)
	: CDialog(CDlgHistoryList::IDD, pParent), CDialogInterface(nDialogColor)
{
	m_pDlgHistoryList2Parent	= NULL;
}

CDlgHistoryList::~CDlgHistoryList()
{
}

void CDlgHistoryList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_HISTORY_LIST, m_ctrlHistoryList);
}


BEGIN_MESSAGE_MAP(CDlgHistoryList, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgHistoryList::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgHistoryList::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgHistoryList 메시지 처리기입니다.


void CDlgHistoryList::OnBnClickedOk()
{
}


void CDlgHistoryList::OnBnClickedCancel()
{
}



void CDlgHistoryList::SetGridHistoryList()
{

}

void CDlgHistoryList::InitGridHistoryList()
{
#define GLASS_HISTORY_GRID_ROW_COUNT		101
#define GLASS_HISTORY_GRID_COL_COUNT		11

	int nRows = GLASS_HISTORY_GRID_ROW_COUNT;
	int nCols = GLASS_HISTORY_GRID_COL_COUNT;
	int nFixRows = 1;
	int nFixCols = 1;
	int nRowIdx = 0;
	int nColIdx = 0;

	m_ctrlHistoryList.GetDefaultCell(TRUE, FALSE)->SetBackClr(RGB(144, 200, 246));
	m_ctrlHistoryList.GetDefaultCell(FALSE, TRUE)->SetBackClr(RGB(144, 200, 246));;
	m_ctrlHistoryList.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));

	m_ctrlHistoryList.SetRowCount(nRows);
	m_ctrlHistoryList.SetColumnCount(nCols);
	m_ctrlHistoryList.SetFixedRowCount(nFixRows);
	m_ctrlHistoryList.SetFixedColumnCount(nFixCols);

	GV_ITEM Item;
	CString strTemp = _T("");

	m_ctrlHistoryList.SetColumnWidth(nColIdx, 30);
	Item.mask = GVIF_TEXT;
	Item.row = nRowIdx;
	Item.col = nColIdx++;
	strTemp.Format(_T("Idx"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);

	m_ctrlHistoryList.SetColumnWidth(nColIdx, 60);
	Item.col = nColIdx++;
	strTemp.Format(_T("Date"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);

	m_ctrlHistoryList.SetColumnWidth(nColIdx, 60);
	Item.col = nColIdx++;
	strTemp.Format(_T("Time"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);

	m_ctrlHistoryList.SetColumnWidth(nColIdx, 90);
	Item.col = nColIdx++;
	strTemp.Format(_T("GlassID"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);

	m_ctrlHistoryList.SetColumnWidth(nColIdx, 80);
	Item.col = nColIdx++;
	strTemp.Format(_T("PPID"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);

	m_ctrlHistoryList.SetColumnWidth(nColIdx, 80);
	Item.col = nColIdx++;
	strTemp.Format(_T("StepID"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);

	m_ctrlHistoryList.SetColumnWidth(nColIdx, 50);
	Item.col = nColIdx++;
	strTemp.Format(_T("TD(ea)"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);


	m_ctrlHistoryList.SetColumnWidth(nColIdx, 50);
	Item.col = nColIdx++;
	strTemp.Format(_T("CD(ea)"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);


	m_ctrlHistoryList.SetColumnWidth(nColIdx, 50);
	Item.col = nColIdx++;
	strTemp.Format(_T("SD(ea)"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);

	m_ctrlHistoryList.SetColumnWidth(nColIdx, 50);
	Item.col = nColIdx++;
	strTemp.Format(_T("RV(ea)"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);

	m_ctrlHistoryList.SetColumnWidth(nColIdx, 55);
	Item.col = nColIdx++;
	strTemp.Format(_T("MES(ea)"));
	Item.strText = strTemp;
	m_ctrlHistoryList.SetItem(&Item);

	m_ctrlHistoryList.SetEditable(FALSE);	
}


BOOL CDlgHistoryList::OnInitDialog()
{
	__super::OnInitDialog();

	InitGridHistoryList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
