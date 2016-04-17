#include "StdAfx.h"
#include "MainInterface.h"


CMainInterface::CMainInterface(void)
{
	m_pDoc		= NULL;
	m_pView		= NULL;

	// init Log 
	new CDisplayMessage(_MICRO_SCOPER_LOG_PATH_, static_cast<IDisplayMessage2Parent*>(this));
}

CMainInterface::~CMainInterface(void)
{
	DeinitInterface();
}

void CMainInterface::InitInterface()
{
	// dialog
	if(m_pView) 
	{
		m_pView->CreateCommonDialogs();
		m_pView->CreateLineDialogs(0,0,1);
	}

	g_pLog->DisplayMessage(_T("MicroScoper is Started!"));
}

void CMainInterface::DeinitInterface()
{
	m_pDoc = NULL;
	m_pView = NULL;
}

// IDlgCameraReview2Parent
void CMainInterface::IDRC2P_DisplayMessage(const TCHAR* lpstrFormat, ...)
{
	if(m_pView==NULL) return;

	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrFormat);
	_vstprintf_s(strText, lpstrFormat, list);
	va_end(list);

	m_pView->DisplayMessage(strText);
}

void CMainInterface::IDRC2P_ChangeCameraView()
{
	if (m_pView)
	{
		m_pView->ChangeCameraView(1);
	}
}

// IDlgCameraAlign2Parent
void CMainInterface::IDAC2P_DisplayMessage(const TCHAR* lpstrFormat, ...)
{
	if(m_pView==NULL) return;

	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrFormat);
	_vstprintf_s(strText, lpstrFormat, list);
	va_end(list);

	m_pView->DisplayMessage(strText);
}

void CMainInterface::IDAC2P_ChangeCameraView()
{
	if (m_pView)
	{
		m_pView->ChangeCameraView(0);
	}
}

// IDlgModuleControl2Parent
void CMainInterface::IDMC2P_DisplayMessage(const TCHAR* lpstrFormat, ...)
{
	if(m_pView==NULL) return;

	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrFormat);
	_vstprintf_s(strText, lpstrFormat, list);
	va_end(list);

	m_pView->DisplayMessage(strText);
}

// IDlgModuleList2Parent
void CMainInterface::IDML2P_DisplayMessage(const TCHAR* lpstrFormat, ...)
{
	if(m_pView==NULL) return;

	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrFormat);
	_vstprintf_s(strText, lpstrFormat, list);
	va_end(list);

	m_pView->DisplayMessage(strText);
}

void CMainInterface::IDMI2P_SetSelectedModuleIndex(int nModuleIdx)
{
	m_pView->DisplayMessage(_T("[ModuleList] %d Module is Selected!"), nModuleIdx);
}

// IDlgTabControl2Parent
void CMainInterface::IDTC2P_DisplayMessage(const TCHAR* lpstrFormat, ...)
{
	if(m_pView==NULL) return;

	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrFormat);
	_vstprintf_s(strText, lpstrFormat, list);
	va_end(list);

	m_pView->DisplayMessage(strText);
}

// IDlgHistoryList2Parent
void CMainInterface::IDHL2P_DisplayMessage(const TCHAR* lpstrFormat, ...)
{
	if(m_pView==NULL) return;

	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrFormat);
	_vstprintf_s(strText, lpstrFormat, list);
	va_end(list);

	m_pView->DisplayMessage(strText);
}

// IDlgLogMessage2Parent
void CMainInterface::IDLM2P_DisplayMessage(const TCHAR* lpstrFormat, ...)
{
	if(m_pView==NULL) return;

	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrFormat);
	_vstprintf_s(strText, lpstrFormat, list);
	va_end(list);

	m_pView->DisplayMessage(strText);
}


void CMainInterface::DM2P_DisplayMessage(const CString& strMessage)
{
	if(m_pView==NULL) return;

	m_pView->DisplayMessage(strMessage);
}

void CMainInterface::DM2P_DisplayMessage(const TCHAR* lpstrFormat, ...)
{
	if(m_pView==NULL) return;

	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrFormat);
	_vstprintf_s(strText, lpstrFormat, list);
	va_end(list);

	m_pView->DisplayMessage(strText);
}