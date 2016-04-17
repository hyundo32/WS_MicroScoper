
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����


#include <afxsock.h>            // MFC ���� Ȯ��


#define TotalWindowPosX			1200
#define TotalWindowPosY			0	
#define TotalWindowWidth		720
#define TotalWindowHeight		1040	

#define DlgCameraViewRect		CRect(0		,0		,718	,538)
#define DlgModuleListRect		CRect(0		,539	,718	,638)
#define DlgModuleControlRect	CRect(0		,639	,718	,798)
#define DlgTabControlRect		CRect(0		,799	,718	,1038)
#define DlgHistoryListRect		CRect(0		,0		,692	,237)
#define DlgLogMessageRect		CRect(0		,0		,692	,237)


#define _MICRO_SCOPER_BASE_PATH_					_T("C:\\DIT_MicroScoper")
#define _MICRO_SCOPER_LOG_PATH_						_T("C:\\DIT_MicroScoper\\MicroScoperLog")
#define _MICRO_SCOPER_RECIPE_PATH_					_T("C:\\DIT_Review\\Recipe")
#define _REVIEW_REF_RECIPE_PATH_					_T("C:\\DIT_Review\\Recipe\\Reference")
#define _MICRO_SCOPER_CONFIG_PATH_					_T("C:\\DIT_Review\\ReviewServerConfig")
#define _MICRO_SCOPER_SYSTEM_SETTING_FILE_PATH_		_T("C:\\DIT_Review\\ReviewServerConfig\\SystemSetting.cfg")
#define _SYSTEM_SETTING_TITLE_						_T("ReviewSystemSetting")
#define _RECIPE_EDITOR_TITLE_						_T("ReviewRecipeEditor")

#define MAX_GLASS_RESULT_COUNT	5

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


