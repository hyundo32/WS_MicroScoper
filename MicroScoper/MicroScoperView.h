
// MicroScoperView.h : CMicroScoperView 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CMicroScoperView();
	DECLARE_DYNCREATE(CMicroScoperView)

public:
	enum{ IDD = IDD_MICROSCOPER_FORM };

// 특성입니다.
public:
	CMicroScoperDoc* GetDocument() const;

// 작업입니다.
public:
	void ChangeCameraView(int nViewType);
	void DisplayMessage(const CString& strMessage);
	void DisplayMessage(const TCHAR* lpstrFormat, ...);
	void CreateCommonDialogs();
	void CreateLineDialogs(int nLineType, int nMachineType, BOOL bUseVisionAlign=TRUE);
	void DeleteAllDialogs();

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
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

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MicroScoperView.cpp의 디버그 버전
inline CMicroScoperDoc* CMicroScoperView::GetDocument() const
   { return reinterpret_cast<CMicroScoperDoc*>(m_pDocument); }
#endif

