#pragma once

// CLogListBox
#define STRINGBUFFER_COUNT		200

class AFX_EXT_CLASS CLogListBox : 
	public CListBox
{
	DECLARE_DYNAMIC(CLogListBox)

public:
	CLogListBox();
	virtual ~CLogListBox();

	void SetPath(CString strPath, CString strName);
	void DisplayMessage(BOOL bAddMsgBox, const CString& str);
	void DisplayMessage(BOOL bAddMsgBox, const TCHAR* lpstrMacroFormat, ...);

private:
	BOOL				MakeLogFile();
	BOOL				WriteToFile(const CTime& time, CString strContents);

	CString				m_strLogPath;
	CString				m_strLogFile;
	CTime				m_TimeLogFile;
	CFile*				m_pFileLog;

	DWORD				m_dwThreadID;
	CString				m_strArray[STRINGBUFFER_COUNT];
	BOOL				m_bMustDisplay[STRINGBUFFER_COUNT];
	int					m_nAddIndex;
	int					m_nReadIndex;

	CRITICAL_SECTION	m_csLog;

protected:
	afx_msg LRESULT OnDisplayMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


