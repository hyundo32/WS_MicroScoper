
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MicroScoper.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = cs.style & ~WS_THICKFRAME & ~WS_MINIMIZEBOX & ~WS_CAPTION & ~WS_SYSMENU & ~FWS_ADDTOTITLE;
	cs.lpszName = _T("MicroScoper");
	this->SetTitle(_T("MicroScoper"));

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(WS_CAPTION, NULL);

	SetMenu(NULL);

	return 0;
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//
	lpMMI->ptMinTrackSize.x = TotalWindowWidth;
	lpMMI->ptMinTrackSize.y = TotalWindowHeight;

	lpMMI->ptMaxTrackSize.x = TotalWindowWidth;
	lpMMI->ptMaxTrackSize.y = TotalWindowHeight;

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	static bool once = true;   

	if (bShow && !IsWindowVisible() && once)  
	{
		once = false;
		POINT min, max;
		min.x = TotalWindowPosX;
		min.y = TotalWindowPosY;
		max.x = TotalWindowPosX;
		max.y = TotalWindowPosY;

		WINDOWPLACEMENT lwp;
		lwp.length = sizeof(WINDOWPLACEMENT);
		lwp.flags = SW_SHOWMINIMIZED | WPF_SETMINPOSITION;
		lwp.showCmd = SW_SHOW;
		lwp.ptMinPosition = min;
		lwp.ptMaxPosition = max;
		lwp.rcNormalPosition = CRect(min, max);
		SetWindowPlacement(&lwp);
	}
}
