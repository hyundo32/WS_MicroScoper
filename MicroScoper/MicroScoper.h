
// MicroScoper.h : MicroScoper ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMicroScoperApp:
// �� Ŭ������ ������ ���ؼ��� MicroScoper.cpp�� �����Ͻʽÿ�.
//

class CMicroScoperApp : public CWinApp
{
public:
	CMicroScoperApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

protected:
	HANDLE	m_hMutex;
	BOOL	m_bExcute;
};

extern CMicroScoperApp theApp;
