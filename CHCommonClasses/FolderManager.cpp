#include "StdAfx.h"
#include "FolderManager.h"


bool CompareCreateDate(CPathInfo& a, CPathInfo& b)
{
	return (a.nCreateDate > b.nCreateDate);
}

bool ComparePathDepth(CPathInfo& a, CPathInfo& b)
{
	return (a.nDepth > b.nDepth);
// 	int nIdx, nPathDepth1 = 0, nPathDepth2 = 0;
// 	CString strPath, strTemp;
// 
// 	strPath = a.strFilePath;
// 	
// 	while(TRUE)
// 	{
// 		nIdx = strPath.Find(_T("\\"));
// 		if(nIdx == -1) break;
// 		strPath = strPath.Mid(nIdx + 1);
// 		nPathDepth1++;
// 	}
// 	
// 	strPath = b.strFilePath;
// 
// 	while(TRUE)
// 	{
// 		nIdx = strPath.Find(_T("\\"));
// 		if(nIdx == -1) break;
// 		strPath = strPath.Mid(nIdx);
// 		nPathDepth2++;
// 	}
//
//	return (nPathDepth1 > nPathDepth2);
}

CFolderManager::CFolderManager()
{
	Reset();
}

CFolderManager::~CFolderManager()
{
	Reset();
}

CFolderManager::CFolderManager(const CString& strMainPath, UINT nRemainCount)
{
	Reset();

	m_strMainPath	= strMainPath;
	m_nRemainCount	= nRemainCount;
}

void CFolderManager::SetFolderInfo(const CString& strMainPath, UINT nRemainCount)
{
	m_strMainPath	= strMainPath;
	m_nRemainCount	= nRemainCount;
}

void CFolderManager::Reset()
{
	m_strMainPath		= _T("");
	m_nRemainCount		= 0;
	m_nLastCreateDate	= 0;
		
	m_vectorFileInfo.clear();
	m_vectorFolderInfo.clear();
// 	for (ListPathInfoIt it=m_listFolderInfo.begin(); it!=m_listFolderInfo.end(); it++)
// 	{
// 		CPathInfo *pPathInfo = (CPathInfo*) *it;
// 		delete pPathInfo;
// 	}
// 	m_listFolderInfo.clear();
}

void CFolderManager::DeleteOldFiles()
{
	m_vectorFileInfo.clear();
	m_vectorFolderInfo.clear();

	int nDepth = 0, nFileCount = 0;
	SearchPathInfo(m_strMainPath, nDepth, nFileCount);

	size_t f_cnt = m_vectorFileInfo.size();
	size_t p_cnt = m_vectorFolderInfo.size();
//	size_t p_cnt = m_listFolderInfo.size();

	if(m_vectorFolderInfo.size() > 0)
	{
		std::sort(m_vectorFolderInfo.begin(), m_vectorFolderInfo.end(), ComparePathDepth);

		DeleteFolderProcess();
	}

	if(m_vectorFileInfo.size() > 0)
	{
		std::sort(m_vectorFileInfo.begin(), m_vectorFileInfo.end(), CompareCreateDate);

		if (m_nRemainCount < m_vectorFileInfo.size())
		{
			DeleteFileProcess();
		}
	}

	f_cnt = m_vectorFileInfo.size();
	p_cnt = m_vectorFolderInfo.size();
//	p_cnt = m_listFolderInfo.size();

}

void CFolderManager::DeleteFileProcess()
{
	for (UINT i=m_nRemainCount; i<m_vectorFileInfo.size(); i++)
	{
		CPathInfo pathInfo;
		pathInfo = m_vectorFileInfo[i];
		DeleteFile(pathInfo.strFilePath);

// 		if (!DeleteFile(pathInfo.strFilePath))
// 		{
// 			SetFileAttributes(m_vectorFileInfo[i].strFilePath, FILE_ATTRIBUTE_NORMAL);	
// 			DeleteFile(m_vectorFileInfo[i].strFilePath);
// 		}
	}

//	m_vectorFileInfo.resize(m_nRemainCount);
}

void CFolderManager::DeleteFolderProcess()
{
	// ºó Æú´õ Á¦°Å
	int i, nCount;

	nCount = (int)m_vectorFolderInfo.size();

	for(i = 0; i < nCount; i++)
	{
		CPathInfo pathInfo;
		pathInfo = m_vectorFolderInfo[i];
		RemoveDirectory(pathInfo.strFilePath);
	}
// 	for(ListPathInfoIt iter= m_listFolderInfo.begin() ; iter != m_listFolderInfo.end() ; )
// 	{
// 		if (RemoveDirectory((*iter)->strFilePath))
// 		{
// 			CPathInfo *pPathInfo = (CPathInfo *) *iter;
// 			delete pPathInfo;
// 			iter = m_listFolderInfo.erase(iter);
// 			continue;
// 		}
// 		else
// 		{
// 			SetFileAttributes((*iter)->strFilePath, FILE_ATTRIBUTE_NORMAL);
// 			
// 			if (RemoveDirectory((*iter)->strFilePath))
// 			{
// 				CPathInfo *pPathInfo = (CPathInfo *) *iter;
// 				delete pPathInfo;
// 				iter = m_listFolderInfo.erase(iter);
// 				continue;
// 			}
// 		}
// 
// 		iter++;
// 	}
}

int CFolderManager::SearchPathInfo(const CString& strParentPath, int nDepth, int& nFileCount)
{
	if (strParentPath.GetLength()==0) 
		return FALSE;
	
	CString strPath = strParentPath;
	
	if(strPath.Right(1) != _T('\\')) 
		strPath += _T('\\');
	
	strPath += _T("*.*");
	
	CFileFind fileFind;
	BOOL bFind;	
	
	if(!(bFind = fileFind.FindFile(strPath)))
		return FALSE;
	
	CTime fileCreateTime;
	
	while(bFind)
	{
		bFind = fileFind.FindNextFile();
		
		if (fileFind.IsDots())
			continue;

		if (!fileFind.GetCreationTime(fileCreateTime)) 
			continue;
		
// 		if (m_nLastCreateDate > fileCreateTime.GetTime())
// 			continue;
		
		if (fileFind.IsDirectory())
		{
			int nFileCountInDirectory = 0;
			CPathInfo pathInfo;
 			pathInfo.strFilePath	= fileFind.GetFilePath();
 			pathInfo.nCreateDate	= fileCreateTime.GetTime();
 
 			SearchPathInfo(pathInfo.strFilePath, nDepth+1, nFileCountInDirectory);

			nFileCount += nFileCountInDirectory;
			pathInfo.nDepth = nDepth;

			if(nFileCountInDirectory == 0)
			{
				m_vectorFolderInfo.push_back(pathInfo);
			}

// 			CPathInfo *pPathInfo = new CPathInfo();
// 			pPathInfo->strFilePath	= fileFind.GetFilePath();
// 			pPathInfo->nCreateDate	= fileCreateTime.GetTime();
// 			m_listFolderInfo.push_back(pPathInfo);
// 
// 			SearchPathInfo(pPathInfo->strFilePath);
		}
		else
		{
			CPathInfo pathInfo;
			pathInfo.strFilePath	= fileFind.GetFilePath();
			pathInfo.nCreateDate	= fileCreateTime.GetTime();
			m_vectorFileInfo.push_back(pathInfo);
			nFileCount++;
		}
	}

	fileFind.Close();

	return TRUE;
}