
// MicroScoperView.cpp : CMicroScoperView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MicroScoper.h"
#endif

#include "MicroScoperDoc.h"
#include "MicroScoperView.h"
#include "MainFrm.h"

#include "DlgCameraReview.h"
#include "DlgCameraAlign.h"
#include "DlgModuleList.h"
#include "DlgModuleControl.h"
#include "DlgTabControl.h"
#include "DlgHistoryList.h"
#include "DlgLogMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMicroScoperView

IMPLEMENT_DYNCREATE(CMicroScoperView, CFormView)

BEGIN_MESSAGE_MAP(CMicroScoperView, CFormView)
END_MESSAGE_MAP()

// CMicroScoperView 생성/소멸

CMicroScoperView::CMicroScoperView()
	: CFormView(CMicroScoperView::IDD)
{
	m_pDlgCameraReview		= NULL;
	m_pDlgCameraAlign		= NULL;
	m_pDlgModuleList		= NULL;
	m_pDlgModuleControl		= NULL;
	m_pDlgTabControl		= NULL;
	m_pDlgHistoryList		= NULL;
	m_pDlgLogMessage		= NULL;

}

CMicroScoperView::~CMicroScoperView()
{
	DeleteAllDialogs();
}

void CMicroScoperView::ChangeCameraView(int nViewType)
{
	switch(nViewType)
	{
	case 0:
		if (m_pDlgCameraAlign) m_pDlgCameraAlign->ShowWindow(SW_HIDE);
		if (m_pDlgCameraReview) m_pDlgCameraReview->ShowWindow(SW_SHOW);
		break;

	case 1:
		if (m_pDlgCameraReview) m_pDlgCameraReview->ShowWindow(SW_HIDE);
		if (m_pDlgCameraAlign) m_pDlgCameraAlign->ShowWindow(SW_SHOW);
		break;
	}
}

void CMicroScoperView::DisplayMessage(const CString& strMessage)
{
	if(m_pDlgLogMessage)
	{
		m_pDlgLogMessage->DisplayMessage(strMessage);
	}
}

void CMicroScoperView::DisplayMessage(const TCHAR* lpstrFormat, ...)
{
	if(m_pDlgLogMessage==NULL) return;

	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrFormat);
	_vstprintf_s(strText, lpstrFormat, list);
	va_end(list);

	m_pDlgLogMessage->DisplayMessage(strText);
}

void CMicroScoperView::CreateCommonDialogs()
{
	CMainFrame *pMainFrame=(CMainFrame *)AfxGetMainWnd();  //View 2 MainFrm 
	if (pMainFrame==NULL) return;

	CRect rect;
	WINDOWPLACEMENT placement;

	// DlgCameraReview
	m_pDlgCameraReview = new CDlgCameraReview(this, RGB(220,220,220));
	if (m_pDlgCameraReview)
	{
		m_pDlgCameraReview->SetIDCR2P(static_cast<IDlgCameraReview2Parent*>(pMainFrame));

		rect = DlgCameraViewRect;
		m_pDlgCameraReview->Create(CDlgCameraReview::IDD, this);
		placement.rcNormalPosition.left		= rect.left;
		placement.rcNormalPosition.top		= rect.top;
		placement.rcNormalPosition.right	= rect.right;
		placement.rcNormalPosition.bottom	= rect.bottom;
		m_pDlgCameraReview->SetWindowPlacement(&placement);
		m_pDlgCameraReview->ShowWindow(SW_HIDE);
	}


	// DlgModuleList
	m_pDlgModuleList = new CDlgModuleList(this, RGB(220,220,220));
	if (m_pDlgModuleList)
	{
		m_pDlgModuleList->SetIDML2P(static_cast<IDlgModuleList2Parent*>(pMainFrame));

		rect = DlgModuleListRect;
		m_pDlgModuleList->Create(CDlgModuleList::IDD, this);
		placement.rcNormalPosition.left		= rect.left;
		placement.rcNormalPosition.top		= rect.top;
		placement.rcNormalPosition.right	= rect.right;
		placement.rcNormalPosition.bottom	= rect.bottom;
		m_pDlgModuleList->SetWindowPlacement(&placement);
		m_pDlgModuleList->ShowWindow(SW_SHOW);
	}

	
	// DlgModuleControl
	m_pDlgModuleControl = new CDlgModuleControl(this, RGB(220,220,220));
	if (m_pDlgModuleControl)
	{
		m_pDlgModuleControl->SetIDMC2P(static_cast<IDlgModuleControl2Parent*>(pMainFrame));

		rect = DlgModuleControlRect;
		m_pDlgModuleControl->Create(CDlgModuleControl::IDD, this);
		placement.rcNormalPosition.left		= rect.left;
		placement.rcNormalPosition.top		= rect.top;
		placement.rcNormalPosition.right	= rect.right;
		placement.rcNormalPosition.bottom	= rect.bottom;
		m_pDlgModuleControl->SetWindowPlacement(&placement);
		m_pDlgModuleControl->ShowWindow(SW_SHOW);
	}

	
	// DlgTabControl
 	m_pDlgTabControl = new CDlgTabControl(this, RGB(220,220,220));
 	if (m_pDlgTabControl)
 	{
		m_pDlgTabControl->SetIDTC2P(static_cast<IDlgTabControl2Parent*>(pMainFrame));

 		rect = DlgTabControlRect;
 		m_pDlgTabControl->Create(CDlgTabControl::IDD, this);
 		placement.rcNormalPosition.left		= rect.left;
 		placement.rcNormalPosition.top		= rect.top;
 		placement.rcNormalPosition.right	= rect.right;
 		placement.rcNormalPosition.bottom	= rect.bottom;
 		m_pDlgTabControl->SetWindowPlacement(&placement);
 		m_pDlgTabControl->ShowWindow(SW_SHOW);

		// DlgHistoryList
		m_pDlgHistoryList = new CDlgHistoryList(this, RGB(220,220,220));
		if (m_pDlgHistoryList)
		{
			m_pDlgHistoryList->SetIDHL2P(static_cast<IDlgHistoryList2Parent*>(pMainFrame));

			rect = DlgHistoryListRect;
			m_pDlgHistoryList->Create(CDlgHistoryList::IDD, this);
			placement.rcNormalPosition.left		= rect.left;
			placement.rcNormalPosition.top		= rect.top;
			placement.rcNormalPosition.right	= rect.right;
			placement.rcNormalPosition.bottom	= rect.bottom;
			m_pDlgHistoryList->SetWindowPlacement(&placement);
			m_pDlgHistoryList->ShowWindow(SW_SHOW);

			m_pDlgTabControl->AddTabWndData(m_pDlgHistoryList, _T("History List"));
		}

		// DlgLogMessage
		m_pDlgLogMessage = new CDlgLogMessage(this, RGB(220,220,220));
		if (m_pDlgLogMessage)
		{
			m_pDlgLogMessage->SetIDLM2P(static_cast<IDlgLogMessage2Parent*>(pMainFrame));

			rect = DlgLogMessageRect;
			m_pDlgLogMessage->Create(CDlgLogMessage::IDD, this);
			placement.rcNormalPosition.left		= rect.left;
			placement.rcNormalPosition.top		= rect.top;
			placement.rcNormalPosition.right	= rect.right;
			placement.rcNormalPosition.bottom	= rect.bottom;
			m_pDlgLogMessage->SetWindowPlacement(&placement);
			m_pDlgLogMessage->ShowWindow(SW_SHOW);

			m_pDlgTabControl->AddTabWndData(m_pDlgLogMessage, _T("Log Message"));
		}

 	}

}

void CMicroScoperView::CreateLineDialogs(int nLineType, int nMachineType, BOOL bUseVisionAlign)
{
	CMainFrame *pMainFrame=(CMainFrame *)AfxGetMainWnd();  //View 2 MainFrm 
	if (pMainFrame==NULL) return;

	CRect rect;
	WINDOWPLACEMENT placement;

	// DlgCameraAlign
	if (bUseVisionAlign)
	{
		m_pDlgCameraAlign = new CDlgCameraAlign(this, RGB(220,220,220));
		if (m_pDlgCameraAlign)
		{
			m_pDlgCameraAlign->SetIDCA2P(static_cast<IDlgCameraAlign2Parent*>(pMainFrame));

			rect = DlgCameraViewRect;
			m_pDlgCameraAlign->Create(CDlgCameraAlign::IDD, this);
			placement.rcNormalPosition.left		= rect.left;
			placement.rcNormalPosition.top		= rect.top;
			placement.rcNormalPosition.right	= rect.right;
			placement.rcNormalPosition.bottom	= rect.bottom;
			m_pDlgCameraAlign->SetWindowPlacement(&placement);
			m_pDlgCameraAlign->ShowWindow(SW_SHOW);
		}
	}

}

void CMicroScoperView::DeleteAllDialogs()
{
	if (m_pDlgHistoryList)
	{
		m_pDlgHistoryList->DestroyWindow();
		delete m_pDlgHistoryList;
		m_pDlgHistoryList = NULL;
	}

	if (m_pDlgTabControl)
	{
		m_pDlgTabControl->DestroyWindow();
		delete m_pDlgTabControl;
		m_pDlgTabControl = NULL;
	}

	if (m_pDlgModuleControl)
	{
		m_pDlgModuleControl->DestroyWindow();
		delete m_pDlgModuleControl;
		m_pDlgModuleControl = NULL;
	}

	if (m_pDlgModuleList)
	{
		m_pDlgModuleList->DestroyWindow();
		delete m_pDlgModuleList;
		m_pDlgModuleList = NULL;
	}


	if (m_pDlgCameraAlign)
	{
		m_pDlgCameraAlign->DestroyWindow();
		delete m_pDlgCameraAlign;
		m_pDlgCameraAlign = NULL;
	}

	if (m_pDlgCameraReview)
	{
		m_pDlgCameraReview->DestroyWindow();
		delete m_pDlgCameraReview;
		m_pDlgCameraReview = NULL;
	}
}

void CMicroScoperView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CMicroScoperView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CMicroScoperView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}


// CMicroScoperView 진단

#ifdef _DEBUG
void CMicroScoperView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMicroScoperView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMicroScoperDoc* CMicroScoperView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMicroScoperDoc)));
	return (CMicroScoperDoc*)m_pDocument;
}
#endif //_DEBUG


// CMicroScoperView 메시지 처리기
