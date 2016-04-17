// DlgLogMessage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MicroScoper.h"
#include "DlgLogMessage.h"
#include "afxdialogex.h"


// CDlgLogMessage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgLogMessage, CDialog)

CDlgLogMessage::CDlgLogMessage(CWnd* pParent, COLORREF nDialogColor)
	: CDialog(CDlgLogMessage::IDD, pParent), CDialogInterface(nDialogColor)
{
	m_pDlgLogMessage2Parent = NULL;
}

CDlgLogMessage::~CDlgLogMessage()
{
}

void CDlgLogMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG_MESSAGE, m_ctrlLogMessage);
}


BEGIN_MESSAGE_MAP(CDlgLogMessage, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLogMessage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgLogMessage::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgLogMessage 메시지 처리기입니다.


void CDlgLogMessage::OnBnClickedOk()
{
}

void CDlgLogMessage::OnBnClickedCancel()
{

}

void CDlgLogMessage::DisplayMessage(const CString& strMessage)
{
	int nCount;
	CTime time;
	CString strStamp, strLog;

	nCount = m_ctrlLogMessage.GetCount();

	if (nCount % MAX_PRINT_COUNT == 0)
	{
		m_ctrlLogMessage.ResetContent();
	}

	time = CTime::GetCurrentTime();
	strStamp.Format(_T("[%02d:%02d:%02d]"), time.GetHour(), time.GetMinute(), time.GetSecond());
	strLog.Format(_T("%s %s"), strStamp, strMessage);
	m_ctrlLogMessage.AddString(strLog);
	m_ctrlLogMessage.SetCurSel(m_ctrlLogMessage.GetCount()-1);
}