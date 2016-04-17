#pragma once

#include "DialogInterface.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "GridCtrl.h"

class CDlgHistoryList : public CDialog, public CDialogInterface
{
	DECLARE_DYNAMIC(CDlgHistoryList)

public:
	CDlgHistoryList(CWnd* pParent = NULL, COLORREF nDialogColor=RGB(255,255,255));   // 표준 생성자입니다.
	virtual ~CDlgHistoryList();
	void SetIDHL2P(IDlgHistoryList2Parent* pIDHL2P) { m_pDlgHistoryList2Parent = pIDHL2P; }

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_HISTORY_LIST };

protected:
	void InitGridHistoryList();
	void SetGridHistoryList();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

protected:
	IDlgHistoryList2Parent*	m_pDlgHistoryList2Parent;


private:
	CGridCtrl		m_ctrlHistoryList;
	
public:
	virtual BOOL OnInitDialog();
};
