#pragma once

#include "Singleton.h"

interface IDisplayMessage2Parent
{
public:
	virtual void	DM2P_DisplayMessage(const CString& strMessage) = 0;
	virtual void	DM2P_DisplayMessage(const TCHAR* lpstrFormat, ...) = 0;
};

class AFX_EXT_CLASS CDisplayMessage : public Singleton<CDisplayMessage>
{
public:
	CDisplayMessage(const CString& strPath, IDisplayMessage2Parent* pDM2P=NULL);
	virtual ~CDisplayMessage(void);

public:
	void	SetPath(const CString& strPath);
	BOOL	MakeLogFile();
	BOOL	WriteToFile(const CString& strMessage);
	void	DisplayMessage(const CString& strMessage);
	void	DisplayMessage(const TCHAR* lpstrFormat, ...);

public:
	CTime						m_TimeLogFile;
	CFile*						m_pFileLog;
	CString						m_strLogPath;
	CString						m_strLogFile;
	IDisplayMessage2Parent*		m_pDM2P;
	CRITICAL_SECTION			m_csLog;
};

#define g_pLog	CDisplayMessage::GetSingletonPtr()