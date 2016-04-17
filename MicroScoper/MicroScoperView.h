
// MicroScoperView.h : CMicroScoperView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"

class CDlgCameraAlign;
class CDlgCameraReview;
class CDlgModuleList;
class CDlgModuleControl;
class CDlgTabControl;
class CDlgHistoryList;
class CDlgLogMessage;

class CMicroScoperView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CMicroScoperView();
	DECLARE_DYNCREATE(CMicroScoperView)

public:
	enum{ IDD = IDD_MICROSCOPER_FORM };

// Ư���Դϴ�.
public:
	CMicroScoperDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	void ChangeCameraView(int nViewType);
	void DisplayMessage(const CString& strMessage);
	void DisplayMessage(const TCHAR* lpstrFormat, ...);
	void CreateCommonDialogs();
	void CreateLineDialogs(int nLineType, int nMachineType, BOOL bUseVisionAlign=TRUE);
	void DeleteAllDialogs();

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CMicroScoperView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


	CDlgCameraReview*		m_pDlgCameraReview;		
	CDlgCameraAlign*		m_pDlgCameraAlign;		
	CDlgModuleList*			m_pDlgModuleList;
	CDlgModuleControl*		m_pDlgModuleControl;
	CDlgTabControl*			m_pDlgTabControl;
	CDlgHistoryList*		m_pDlgHistoryList;
	CDlgLogMessage*			m_pDlgLogMessage;

	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MicroScoperView.cpp�� ����� ����
inline CMicroScoperDoc* CMicroScoperView::GetDocument() const
   { return reinterpret_cast<CMicroScoperDoc*>(m_pDocument); }
#endif

