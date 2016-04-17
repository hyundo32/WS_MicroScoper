#include "StdAfx.h"
#include "DisplayMessage.h"

CDisplayMessage::CDisplayMessage(const CString& strPath, IDisplayMessage2Parent* pDM2P)
{
	m_pDM2P		= pDM2P;
	m_pFileLog	= NULL;

	SetPath(strPath);
	InitializeCriticalSection(&m_csLog);
}

CDisplayMessage::~CDisplayMessage(void)
{
	if (m_pFileLog)
	{
		delete m_pFileLog;
		m_pFileLog = NULL;
	}

	DeleteCriticalSection(&m_csLog);
}

void CDisplayMessage::DisplayMessage(const CString& strMessage)
{
	WriteToFile(strMessage);

	if(m_pDM2P)
	{
		m_pDM2P->DM2P_DisplayMessage(strMessage);
	}
}

void CDisplayMessage::DisplayMessage(const TCHAR* lpstrFormat, ...)
{
	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrFormat);
	_vstprintf_s(strText, lpstrFormat, list);
	va_end(list);

	WriteToFile(strText);

	if(m_pDM2P)
	{
		m_pDM2P->DM2P_DisplayMessage(strText);
	}
}

void CDisplayMessage::SetPath(const CString& strPath)
{
	m_strLogPath = strPath;

	CreateDirectory(m_strLogPath, NULL);

	MakeLogFile();
}

BOOL CDisplayMessage::MakeLogFile()
{
	// Log 처리
	if (m_strLogPath.IsEmpty())
		return FALSE;

	// 일정 수 이상 되면 오래된 항목을 지운다.
	//	m_FSLog.CommitSchedule();

	m_TimeLogFile = CTime::GetCurrentTime();
	m_strLogFile.Format(_T("%s\\ReviewSystem_%04d-%02d-%02d.log"), m_strLogPath, m_TimeLogFile.GetYear(), m_TimeLogFile.GetMonth(), m_TimeLogFile.GetDay());

	if (m_pFileLog)
	{
		delete m_pFileLog;
	}

	m_pFileLog = new CFile();

	return TRUE;
}

BOOL CDisplayMessage::WriteToFile(const CString& strMessage)
{
	if(m_pFileLog == NULL)	return FALSE;

	EnterCriticalSection(&m_csLog);

	CTime	time = CTime::GetCurrentTime();
	CString	strTimeStamp;
	// 날짜가 바뀌면 파일명을 갱신해서 쓴다.
	if ((time.GetMonth() != m_TimeLogFile.GetMonth()) || (time.GetDay() != m_TimeLogFile.GetDay()) || !m_pFileLog)
		MakeLogFile();

	if (m_pFileLog->Open(m_strLogFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone ))
	{
		// 유니코드 파일의 시작은 BOM(0xFEFF) 이어야 한다.
		TCHAR strCommand = 0xFEFF;
		CString strLog;
		strTimeStamp.Format(_T("[%02d:%02d:%02d]"), time.GetHour(), time.GetMinute(), time.GetSecond());
#ifdef UNICODE
		strLog.Format(_T("%c%s %s\r\n"), strCommand, strTimeStamp, strMessage);
#else
		strLog.Format(_T("%s %s\r\n"), strTimeStamp, strMessage);
#endif
		m_pFileLog->SeekToEnd();
		m_pFileLog->Write(strLog.GetBuffer(0), strLog.GetLength() * sizeof(TCHAR));
		strLog.ReleaseBuffer();
		m_pFileLog->Close();
	}

	LeaveCriticalSection(&m_csLog);

	return TRUE;
}