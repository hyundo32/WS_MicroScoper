// LogListBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogListBox.h"

// CLogListBox
#define UM_DISPLAYMESSAGE		WM_USER + 8378

IMPLEMENT_DYNAMIC(CLogListBox, CListBox)

CLogListBox::CLogListBox()
{
	m_pFileLog = NULL;
	m_strLogPath = _T("");
	m_strLogFile = _T("");

	for (int i = 0; i < STRINGBUFFER_COUNT; i++)
	{
		m_strArray[i] = _T("");
		m_bMustDisplay[i] = FALSE;
	}

	m_nAddIndex = 0;
	m_nReadIndex = 0;

	InitializeCriticalSection(&m_csLog);
}

CLogListBox::~CLogListBox()
{
	for (int i = 0; i < STRINGBUFFER_COUNT; i++)
	{
		m_strArray[i] = _T("");
		m_bMustDisplay[i] = FALSE;
	}

	if (m_pFileLog!=NULL)
	{
		delete m_pFileLog;
		m_pFileLog = NULL;
	}

	DeleteCriticalSection(&m_csLog);
}


BEGIN_MESSAGE_MAP(CLogListBox, CListBox)
	ON_MESSAGE(UM_DISPLAYMESSAGE, OnDisplayMessage)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CLogListBox 메시지 처리기입니다.


void CLogListBox::SetPath(CString strPath, CString strName)
{
	strPath.Replace(_T("/"), _T("\\"));

	CString str = _T("");
	str = strPath.Right(1);

	if (str == "\\")
		m_strLogPath = strPath;
	else
		m_strLogPath = strPath + _T("\\");

	m_strLogPath += strName;

	MakeLogFile();

	m_dwThreadID = GetCurrentThreadId();
}

BOOL CLogListBox::MakeLogFile()
{
	// Log 처리
	if (m_strLogPath.IsEmpty())
		return FALSE;

	m_TimeLogFile = CTime::GetCurrentTime();
	m_strLogFile.Format(_T("%s_%02d_%02d.log"), m_strLogPath, m_TimeLogFile.GetMonth(), m_TimeLogFile.GetDay());

	if (m_pFileLog!=NULL)
	{
		delete m_pFileLog;
		m_pFileLog = NULL;
	}

	m_pFileLog = new CFile();

	return TRUE;
}


void CLogListBox::DisplayMessage(BOOL bAddMsgBox, const CString& str)
{
	try
	{
		EnterCriticalSection(&m_csLog);

		// 입력할 문자열 만들기.
		CTime	time = CTime::GetCurrentTime();

		CString strTemp;
		strTemp.Format(_T("%02d-%02d-%02d : "), time.GetHour(), time.GetMinute(), time.GetSecond());
		strTemp += str;

		WriteToFile(time, strTemp);

		// 화면에 출력하는 메시지라면...
		if (bAddMsgBox)
		{
			m_strArray[m_nAddIndex] = _T("");
			m_strArray[m_nAddIndex] = strTemp;
			m_bMustDisplay[m_nAddIndex] = TRUE;
			m_nAddIndex = (m_nAddIndex + 1) % STRINGBUFFER_COUNT;

			if (GetSafeHwnd())
			{
				if (m_dwThreadID == GetCurrentThreadId())
					OnDisplayMessage(1, 0);
				else
				{
					PostMessage(UM_DISPLAYMESSAGE, 0, 0);
					Sleep(0);
				}
			}
		}

		LeaveCriticalSection(&m_csLog);
	}
	catch (...)
	{
		LeaveCriticalSection(&m_csLog);
		return;
	}
}

void CLogListBox::DisplayMessage(BOOL bAddMsgBox, const TCHAR* lpstrFormat, ...)
{
	try
	{
		EnterCriticalSection(&m_csLog);

		va_list list;
		TCHAR strText[2000] = {0};

		va_start(list, lpstrFormat);
		_vstprintf_s(strText, lpstrFormat, list);
		va_end(list);

		// 입력할 문자열 만들기.
		CTime	time = CTime::GetCurrentTime();

		CString strTemp;
		strTemp.Format(_T("%02d-%02d-%02d : "), time.GetHour(), time.GetMinute(), time.GetSecond());
		strTemp += strText;

		WriteToFile(time, strTemp);

		// 화면에 출력하는 메시지라면...
		if (bAddMsgBox)
		{
			m_strArray[m_nAddIndex] = _T("");
			m_strArray[m_nAddIndex] = strTemp;
			m_bMustDisplay[m_nAddIndex] = TRUE;
			m_nAddIndex = (m_nAddIndex + 1) % STRINGBUFFER_COUNT;

			if (GetSafeHwnd())
			{
				if (m_dwThreadID == GetCurrentThreadId())
					OnDisplayMessage(1, 0);
				else
				{
					PostMessage(UM_DISPLAYMESSAGE, 0, 0);
					Sleep(0);
				}
			}
		}

		LeaveCriticalSection(&m_csLog);
	}
	catch (...)
	{
		LeaveCriticalSection(&m_csLog);
		return;
	}
}

LRESULT CLogListBox::OnDisplayMessage(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
		EnterCriticalSection(&m_csLog);

	int count = 0;
	int nInsertCount = 0;
	while (m_bMustDisplay[m_nReadIndex] && nInsertCount < STRINGBUFFER_COUNT)
	{
		// 1000개일때 리스트를 클리어.
		count = GetCount();
		if(count > 1000)
		{
			ResetContent();
			count = GetCount();
		}

		InsertString(count, (LPCTSTR)m_strArray[m_nReadIndex]);

		m_strArray[m_nReadIndex] = _T("");
		m_bMustDisplay[m_nReadIndex] = FALSE;

		m_nReadIndex = (m_nReadIndex + 1) % STRINGBUFFER_COUNT;

		nInsertCount++;
	}
	SetCaretIndex(count + 1);
	SetFocus();
	SendMessage(WM_KEYDOWN, VK_END);

	if (wParam == 0)
		LeaveCriticalSection(&m_csLog);

	return 0;
}

// Log File에 쓰기.
BOOL CLogListBox::WriteToFile(const CTime& time, CString strContents)
{
	// 날짜가 바뀌면 파일명을 갱신해서 쓴다.
	if ((time.GetMonth() != m_TimeLogFile.GetMonth()) || (time.GetDay() != m_TimeLogFile.GetDay()) || !m_pFileLog)
		MakeLogFile();

	if (!m_pFileLog->Open(m_strLogFile, CFile::modeWrite))
	{
		if (!m_pFileLog->Open(m_strLogFile, CFile::modeCreate | CFile::modeWrite))
		{
			delete m_pFileLog;
			m_pFileLog = NULL;
		}
	}

	if (m_pFileLog)
	{
		m_pFileLog->SeekToEnd();

		CString strLine = strContents + _T("\r\n");
		char * str_value = NULL;

#ifdef _UNICODE
		int length = WideCharToMultiByte(CP_ACP, 0, strLine.GetBuffer(), -1, NULL, 0, NULL,NULL); 
		str_value = new char[length]; 
		WideCharToMultiByte(CP_ACP, 0, strLine.GetBuffer(), -1, str_value, length, NULL, NULL); 
		m_pFileLog->Write(str_value, length-1);
#else
		int length = strLine.GetLength();
		str_value = new char[length+1]; 
		memcpy(str_value, strLine.GetBuffer(), sizeof(char)*length);
		str_value[length] = NULL;
		m_pFileLog->Write(str_value, length);
#endif

		delete [] str_value;

		m_pFileLog->Close();


	}

	return TRUE;
}



int CLogListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.


	return 0;
}
