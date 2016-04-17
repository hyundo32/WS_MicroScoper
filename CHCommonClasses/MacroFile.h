#pragma once

class AFX_EXT_CLASS CMacroFile  
{
public:
	CMacroFile()	{ Clear(); }
	~CMacroFile()	{ Clear(); }
	void Clear()
	{
		m_arTiltleLine.RemoveAll();
		m_arValueLine.RemoveAll();
	}

	BOOL Read(const TCHAR* lpstrFilePath);
	BOOL Write(const TCHAR* lpstrFilePath);
	
	void GetItem(const TCHAR* lpstrTitle, short& sValue, short sDefault=0);
	void SetItem(const TCHAR* lpstrTitle, const short& sValue);

	void GetItem(const TCHAR* lpstrTitle, int& nValue, int nDefault=0);
	void SetItem(const TCHAR* lpstrTitle, const int& nValue);

	void GetItem(const TCHAR* lpstrTitle, UINT& unValue, UINT unDefault=0);
	void SetItem(const TCHAR* lpstrTitle, const UINT& unValue);

	void GetItem(const TCHAR* lpstrTitle, long& lValue, long lDefault=0);
	void SetItem(const TCHAR* lpstrTitle, const long& lValue);
	
	void GetItem(const TCHAR* lpstrTitle, float& fValue, float fDefault=0.0f);
	void SetItem(const TCHAR* lpstrTitle, const float& fValue);
	
	void GetItem(const TCHAR* lpstrTitle, double& dValue, double dDefault=0.0);
	void SetItem(const TCHAR* lpstrTitle, const double& dValue);
	
	void GetItem(const TCHAR* lpstrTitle, TCHAR &cValue, TCHAR cDefault=_T(''));
	void SetItem(const TCHAR* lpstrTitle, const TCHAR& cValue);

	void GetItem(const TCHAR* lpstrTitle, TCHAR* lpBuffer, const int nMaxLength);
	void SetItem(const TCHAR* lpstrTitle, const LPCTSTR lpstrValue);

	void GetItem(const TCHAR* lpstrTitle, CString& strValue, CString strDefault=_T(""));
	void SetItem(const TCHAR* lpstrTitle, const CString& strValue);
			
	void GetItem(const TCHAR* lpstrStartChar, const TCHAR* lpstrEndChar, TCHAR* lpBuffer, const int nMaxLength);
	void SetItem(const TCHAR* lpstrStartChar, const TCHAR* lpstrEndChar, const TCHAR* lpstrMacroFormat, ...);

protected:
	CStringArray m_arTiltleLine;
	CStringArray m_arValueLine;
};
