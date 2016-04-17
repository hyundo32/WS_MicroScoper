#pragma once


#include "DialogInterface.h"
#include "afxcmn.h"
#include "afxwin.h"

#define MAX_PRINT_COUNT		1000

class CDlgLogMessage : public CDialog, public CDialogInterface
{
	DECLARE_DYNAMIC(CDlgLogMessage)

public:
	CDlgLogMessage(CWnd* pParent = NULL, COLORREF nDialogColor=RGB(255,255,255));   // 표준 생성자입니다.
	virtual ~CDlgLogMessage();
	void SetIDLM2P(IDlgLogMessage2Parent* pIDLM2P) { m_pDlgLogMessage2Parent = pIDLM2P; }

	void DisplayMessage(const CString& strMessage);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_LOG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

protected:
	IDlgLogMessage2Parent*	m_pDlgLogMessage2Parent;
public:
	CListBox m_ctrlLogMessage;
};
