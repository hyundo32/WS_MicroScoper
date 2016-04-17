#include "StdAfx.h"
#include "MacroFile.h"

BOOL CMacroFile::Read(const TCHAR* lpstrFilePath)
{
	CStdioFile file;
	CFileException ex;

	if ( !file.Open(lpstrFilePath, CFile::modeRead, &ex))
	{

#ifdef _DEBUG
		TCHAR   szCause[255] = {0};
		ex.GetErrorMessage(szCause, 255);
		TRACE(szCause);
#endif
		return FALSE;
	}

	CString strLine;
	CString strTiltle, strValue;
	int p = 0;

	while(file.ReadString(strLine))
	{
		strTiltle = _T("");
		strValue = _T("");


		if ( -1 == (p = strLine.Find(_T('='), 0))) continue;

		strTiltle = strLine.Left(p);
		strValue = strLine.Right(strLine.GetLength() - p - 1);		

		m_arTiltleLine.Add(strTiltle);
		m_arValueLine.Add(strValue);


	}

	file.Close();

	return TRUE;
}

BOOL CMacroFile::Write(const TCHAR* lpstrFilePath)
{
	CStdioFile file;
	CFileException ex;

	if ( !file.Open(lpstrFilePath, CFile::modeCreate | CFile::modeWrite, &ex))
	{
#ifdef _DEBUG
		TCHAR   szCause[255] = {0};
		ex.GetErrorMessage(szCause, 255);
		TRACE(szCause);
#endif
		return FALSE;
	}

	CString strLine = _T("");
	CString strTiltle, strValue;

	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strTiltle = _T("");
		strValue = _T("");

		strTiltle = m_arTiltleLine.GetAt(nIndex);
		strValue = m_arValueLine.GetAt(nIndex);

		strLine.Format(_T("%s=%s"), strTiltle, strValue);

		file.WriteString(CString(strLine)+_T("\n"));

	}

	file.Close();

	return TRUE;
}

void CMacroFile::GetItem(const TCHAR* lpstrTitle, short& sValue, short sDefault)
{
	CString strText = _T("");
	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strText = m_arTiltleLine.GetAt(nIndex);
		if(strText.Compare(lpstrTitle)==0)
		{
			strText = m_arValueLine.GetAt(nIndex);
			sValue = ::_ttoi((LPCTSTR)strText);
			return;
		}
	}
	sValue = sDefault;
}

void CMacroFile::GetItem(const TCHAR* lpstrTitle, int& nValue, int nDefault)
{
	CString strText = _T("");
	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strText = m_arTiltleLine.GetAt(nIndex);
		if(strText.Compare(lpstrTitle)==0)
		{
			strText = m_arValueLine.GetAt(nIndex);
			strText.Remove('`');
			nValue = ::_ttoi((LPCTSTR)strText);
			return;
		}
	}
	nValue = nDefault;
}

void CMacroFile::GetItem(const TCHAR* lpstrTitle, UINT& unValue, UINT unDefault)
{
	CString strText = _T("");
	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strText = m_arTiltleLine.GetAt(nIndex);
		if(strText.Compare(lpstrTitle)==0)
		{
			strText = m_arValueLine.GetAt(nIndex);
			unValue = ::_ttoi((LPCTSTR)strText);
			return;
		}
	}
	unValue = unDefault;
}

void CMacroFile::GetItem(const TCHAR* lpstrTitle, long& lValue, long lDefault)
{
	CString strText = _T("");
	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strText = m_arTiltleLine.GetAt(nIndex);
		if(strText.Compare(lpstrTitle) == 0)
		{
			strText = m_arValueLine.GetAt(nIndex);
			lValue = ::_ttoi((LPCTSTR)strText);
			return;
		}
	}
	lValue = lDefault;
}

void CMacroFile::GetItem(const TCHAR* lpstrTitle, float& fValue, float fDefault)
{
	CString strText = _T("");
	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strText = m_arTiltleLine.GetAt(nIndex);
		if(strText.Compare(lpstrTitle) == 0)
		{
			strText = m_arValueLine.GetAt(nIndex);
			//strText.Replace('\'','');
			strText.Remove('`');
			fValue =  float(_tstof((LPCTSTR)strText));
			return;
		}
	}
	fValue = fDefault;
}

void CMacroFile::GetItem(const TCHAR* lpstrTitle, double& dValue, double dDefault)
{
	CString strText = _T("");
	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strText = m_arTiltleLine.GetAt(nIndex);
		if(strText.Compare(lpstrTitle) == 0)
		{
			strText = m_arValueLine.GetAt(nIndex);
			dValue =  _tcstod((LPCTSTR)strText, NULL);
			return;
		}
	}
	dValue = dDefault;
}

void CMacroFile::GetItem(const TCHAR* lpstrTitle, TCHAR &cValue, TCHAR cDefault)
{
	CString strText = _T("");
	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strText = m_arTiltleLine.GetAt(nIndex);
		if(strText.Compare(lpstrTitle) == 0)
		{
			strText = m_arValueLine.GetAt(nIndex);
			cValue = (m_arValueLine.GetAt(nIndex)).GetAt(0);
			return;
		}
	}
	cValue = cDefault;
}

void CMacroFile::GetItem(const TCHAR* lpstrTitle, CString& strValue, CString strDefault)
{
	CString strText = _T("");

	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strText = m_arTiltleLine.GetAt(nIndex);

		if(strText.Compare(lpstrTitle) == 0)
		{
			strValue = m_arValueLine.GetAt(nIndex);
			return;
		}
	}
	strValue = strDefault;
}


void CMacroFile::SetItem(const TCHAR* lpstrTitle, const short& sValue)
{
	TCHAR szValue[256];
	_stprintf_s(szValue, _T("%d"), sValue);

	SetItem(lpstrTitle, szValue);
}

void CMacroFile::SetItem(const TCHAR* lpstrTitle,const int& nValue)
{
	TCHAR szValue[256];
	_stprintf_s(szValue, _T("%d"), nValue);

	SetItem(lpstrTitle, szValue);
}

void CMacroFile::SetItem(const TCHAR* lpstrTitle, const UINT& unValue)
{
	TCHAR szValue[256];
	_stprintf_s(szValue, _T("%d"), unValue);

	SetItem(lpstrTitle, szValue);
}

void CMacroFile::SetItem(const TCHAR* lpstrTitle, const long& lValue)
{
	TCHAR szValue[256];
	_stprintf_s(szValue, _T("%d"), lValue);

	SetItem(lpstrTitle, szValue);
}

void CMacroFile::SetItem(const TCHAR* lpstrTitle, const float& fValue)
{
	TCHAR szValue[256];
	_stprintf_s(szValue, _T("%.8f"), fValue);

	SetItem(lpstrTitle, szValue);
}

void CMacroFile::SetItem(const TCHAR* lpstrTitle, const double& dValue)
{
	TCHAR szValue[256];
	_stprintf_s(szValue, _T("%.8lf"), dValue);
	
	SetItem(lpstrTitle, szValue);
}

void CMacroFile::SetItem(const TCHAR* lpstrTitle, const TCHAR &cValue)
{
	TCHAR szValue[256];
	_stprintf_s(szValue, _T("%c"), cValue);

	SetItem(lpstrTitle, szValue);
}

void CMacroFile::SetItem(const TCHAR* lpstrTitle, const CString& strValue)
{
	SetItem(lpstrTitle, (LPCTSTR)strValue);
}

void CMacroFile::SetItem(const TCHAR* lpstrTitle, const TCHAR* lpstrValue)
{
	BOOL bNewItem = TRUE;
	CString strText = _T("");
	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strText = m_arTiltleLine.GetAt(nIndex);
		if(strText.Compare(lpstrTitle) == 0)
		{
			m_arValueLine.SetAt(nIndex, lpstrValue);
			bNewItem = FALSE;
		}
	}

	if(bNewItem)
	{
		m_arTiltleLine.Add(lpstrTitle);
		m_arValueLine.Add(lpstrValue);
	}
}

void CMacroFile::GetItem(const TCHAR* lpstrTitle, TCHAR* lpBuffer, const int nMaxLength)
{
	ASSERT(lpBuffer);
	ASSERT(nMaxLength > 0);

	ZeroMemory(lpBuffer, sizeof(char)*nMaxLength);

	CString strText = _T("");
	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strText = m_arTiltleLine.GetAt(nIndex);
		if(strText.Compare(lpstrTitle) == 0)
		{
			strText = m_arValueLine.GetAt(nIndex);
			_tcscpy_s(lpBuffer, strText.GetLength(), (LPCTSTR)strText);
			break;
		}
	}
}


void CMacroFile::GetItem(const TCHAR* lpstrStartChar, const TCHAR* lpstrEndChar, TCHAR* lpBuffer, const int nMaxLength)
{
	ASSERT(lpBuffer);
	ASSERT(nMaxLength > 0);

	ZeroMemory(lpBuffer, sizeof(char)*nMaxLength);

	CString strTitle , strValue ;

	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strTitle = _T("");
		strValue = _T("");

		strTitle = m_arTiltleLine.GetAt(nIndex);
		strValue = m_arValueLine.GetAt(nIndex);

		if (strTitle.IsEmpty()) continue;

		if( strTitle.GetAt(0) == _T('!') && strValue.Compare(lpstrStartChar) == 0)
		{
			for(int n2thIndex = nIndex+1; m_arTiltleLine.GetSize(); n2thIndex++)
			{
				strTitle = _T("");
				strValue = _T("");

				strTitle = m_arTiltleLine.GetAt(n2thIndex);
				strValue = m_arValueLine.GetAt(n2thIndex);

				if (strTitle.IsEmpty()) continue;

				if( strTitle.GetAt(0) == _T('!') && strValue.Compare(lpstrEndChar) == 0) 
					break;

				if ( int((_tcslen(lpBuffer) + strValue.GetLength()+1)) > nMaxLength)
					break;

				_tcscat_s(lpBuffer, strValue.GetLength(), (LPCTSTR)strValue);
				_tcscat_s(lpBuffer, 1, _T("\n"));
			}
		}
	}
}

void CMacroFile::SetItem(const TCHAR* lpstrStartChar, const TCHAR* lpstrEndChar, const TCHAR* lpstrMacroFormat, ...)
{
	va_list list;
	TCHAR strText[2000] = {0};

	va_start(list, lpstrMacroFormat);
	_vstprintf_s(strText, lpstrMacroFormat, list);
	va_end(list);

	CString strTitle , strValue ;

	BOOL bNewItem = TRUE;

	for(int nIndex=0; nIndex<m_arTiltleLine.GetSize(); nIndex++)
	{
		strTitle = m_arTiltleLine.GetAt(nIndex);
		strValue =	m_arValueLine.GetAt(nIndex);

		if (strTitle.IsEmpty()) continue;

		if( strTitle.GetAt(0) == _T('!') && strValue.Compare(lpstrEndChar) == 0)
		{
			m_arTiltleLine.InsertAt(nIndex, _T("!"), 1);
			m_arValueLine.InsertAt(nIndex, strText, 1);
			bNewItem = FALSE;
			break;
		}
	}

	if (bNewItem)
	{
		m_arTiltleLine.Add(_T("!"));
		m_arValueLine.Add(lpstrStartChar);

		m_arTiltleLine.Add(_T("!"));
		m_arValueLine.Add(strText);

		m_arTiltleLine.Add(_T("!"));
		m_arValueLine.Add(lpstrEndChar);

	}
}
