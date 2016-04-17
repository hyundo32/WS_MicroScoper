#pragma once

#include <list>
#include <vector>
#include <algorithm>

class CPathInfo
{
public:
	CPathInfo()		{ Reset(); }
	~CPathInfo()	{ Reset(); }
	
	void Reset()
	{
		strFilePath	= _T("");
		nCreateDate	= 0;
		nDepth = 0;
	}
	
	CString		strFilePath;
	__int64		nCreateDate;
	int			nDepth;
};

typedef std::vector<CPathInfo>				VectorPathInfo;
typedef std::vector<CPathInfo>::iterator	VectorPathInfoIt;

typedef std::list<CPathInfo*>				ListPathInfo;
typedef std::list<CPathInfo*>::iterator		ListPathInfoIt;

class AFX_EXT_CLASS CFolderManager  
{
public:
	CFolderManager();
	CFolderManager(const CString& strMainPath, UINT nRemainCount);
	~CFolderManager();
	void DeleteOldFiles();
	void SetFolderInfo(const CString& strMainPath, UINT nRemainCount);

private:
	void Reset();
	void DeleteFileProcess();
	void DeleteFolderProcess();
	BOOL SearchPathInfo(const CString& strParentPath, int nDepth, int& nFileCount);

	CString			m_strMainPath;
	UINT			m_nRemainCount;

	VectorPathInfo	m_vectorFileInfo;
	VectorPathInfo	m_vectorFolderInfo;
//	ListPathInfo	m_listFolderInfo;
	__int64			m_nLastCreateDate;
};
