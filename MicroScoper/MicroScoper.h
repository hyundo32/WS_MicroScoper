
// MicroScoper.h : MicroScoper 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CMicroScoperApp:
// 이 클래스의 구현에 대해서는 MicroScoper.cpp을 참조하십시오.
//

class CMicroScoperApp : public CWinApp
{
public:
	CMicroScoperApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

protected:
	HANDLE	m_hMutex;
	BOOL	m_bExcute;
};

extern CMicroScoperApp theApp;
