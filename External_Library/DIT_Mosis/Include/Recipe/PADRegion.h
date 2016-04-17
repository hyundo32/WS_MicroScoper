// PADRegion.h: interface for the CPADRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PADREGION_H__3BA9C7BC_907E_4E3E_8EE1_44CE00B3D434__INCLUDED_)
#define AFX_PADREGION_H__3BA9C7BC_907E_4E3E_8EE1_44CE00B3D434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>
#include "PADRegionParm.h"

const int PAD_MAX_POLYGON_POINT = 5;
enum	emRegionType{PADREGION_RECT=0,PADREGION_POLY,PADREGION_NONEAREA,PADREGION_NONE};

class AFX_EXT_CLASS CPADRegionNoneArea : public CArray<CRect, CRect>
{	
public:	
	
	CPADRegionNoneArea()
	{
	
	}	
	CPADRegionNoneArea& operator=(CPADRegionNoneArea& rhs)
	{
		Reset();

		int			iLoop;
		for(iLoop=0;iLoop<rhs.GetCount();iLoop++)
		{
			Add(rhs.GetAt(iLoop));
		}
		return *this;
	}
	void Reset()
	{
		RemoveAll();
	}

	BOOL	ReadRegionNoneArea(int iModel,CHMRegiConfig* pFile)
	{
		int		i,nValue,nCount=0;
		CRect	rect;
		CString	strSection;
		
		Reset();

		strSection.Format("PADREGION_NONEAREA_COUNT");
		pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection,nCount);

		for(i=0;i<nCount;i++)
		{
			strSection.Format("PADREGION_NONEAREA%d_LEFT",i);
			pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection,nValue,0);
			rect.left = nValue;
			strSection.Format("PADREGION_NONEAREA%d_TOP",i);
			pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection,nValue,0);
			rect.top = nValue;
			strSection.Format("PADREGION_NONEAREA%d_RIGHT",i);
			pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection,nValue,0);
			rect.right = nValue;
			strSection.Format("PADREGION_NONEAREA%d_BOTTOM",i);
			pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection,nValue,0);
			rect.bottom = nValue;

			Add(rect);
		}		

		return TRUE;
	}

	BOOL	WriteRegionNoneArea(int iModel,CHMRegiConfig* pFile)
	{
		int		i;
		CRect	rect;
		CString	strSection;		
		int		nCount;

		nCount = static_cast<int>(GetSize());
		strSection.Format("PADREGION_NONEAREA_COUNT");
		pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection,nCount);
		
		for(i=0;i<GetSize();i++)
		{
			rect = GetAt(i);
			strSection.Format("PADREGION_NONEAREA%d_LEFT",i);
			pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection,rect.left);			
			strSection.Format("PADREGION_NONEAREA%d_TOP",i);
			pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection,rect.top);
			strSection.Format("PADREGION_NONEAREA%d_RIGHT",i);
			pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection,rect.right);
			strSection.Format("PADREGION_NONEAREA%d_BOTTOM",i);
			pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection,rect.bottom);			
		}		
		
		return TRUE;
	}
	BOOL		ReadRecipe(CMosisPacket *pPacket)
	{
		int			nCount,i;		
		CRect		rect;

		pPacket->ReadValue(nCount);

		for(i=0;i<nCount;i++)
		{			
			pPacket->ReadValue(rect.left);
			pPacket->ReadValue(rect.top);
			pPacket->ReadValue(rect.right);
			pPacket->ReadValue(rect.bottom);
			Add(rect);
		}	
		return TRUE;
	}
	BOOL		WriteRecipe(CMosisPacket *pPacket)
	{
		int			i,nValue;		
		CRect		rect;

		nValue = (int)GetSize();
		pPacket->WriteValue(nValue);

		for(i=0;i<GetSize();i++)
		{			
			rect = GetAt(i);
			pPacket->WriteValue(rect.left);
			pPacket->WriteValue(rect.top);
			pPacket->WriteValue(rect.right);
			pPacket->WriteValue(rect.bottom);			
		}	
		return TRUE;
	}
};

class AFX_EXT_CLASS CPADRegion
{	
public:
	emRegionType	m_emRegType;
	int				m_nGroupIdx;	
	CRect			m_rectangle;
	int				m_nPolyPointCnt;
	CPoint			m_pPolyPoint[PAD_MAX_POLYGON_POINT];

	CPADRegion()
	{
		Reset();
	}	
	CPADRegion& operator=(CPADRegion& rhs)
	{
		m_emRegType = rhs.m_emRegType;
		m_nGroupIdx = rhs.m_nGroupIdx;
		m_rectangle = rhs.m_rectangle;
		m_nPolyPointCnt = rhs.m_nPolyPointCnt;
		CopyMemory(m_pPolyPoint,rhs.m_pPolyPoint,sizeof(CPoint)*PAD_MAX_POLYGON_POINT);

		return *this;
	}
	void Reset()
	{
		int			i;		
		
		m_emRegType = PADREGION_NONE;
		m_nGroupIdx = -1;
		m_rectangle.SetRect(0,0,0,0);		
		m_nPolyPointCnt = 0;
		for(i=0;i<PAD_MAX_POLYGON_POINT;i++)
			m_pPolyPoint[i] = CPoint(0,0);
	}
	CRect GetOutRect()
	{
		if(m_emRegType != PADREGION_POLY)
			return CRect(0,0,0,0);
		
		CRect	rect = CRect(LONG_MAX,LONG_MAX,LONG_MIN,LONG_MIN);
		
		if(m_nPolyPointCnt > 0)
		{
			int		i;		
			for(i=0;i<m_nPolyPointCnt;i++)
			{
				rect.left = min(m_pPolyPoint[i].x,rect.left);
				rect.top = min(m_pPolyPoint[i].y,rect.top);
				rect.right = max(m_pPolyPoint[i].x,rect.right);
				rect.bottom = max(m_pPolyPoint[i].y,rect.bottom);
			}
		}
		else
			rect = CRect(0,0,0,0);
				
		return rect;
	}

	BOOL ReadRegion(int iModel,int iRegion,CHMRegiConfig* pFile)
	{
		if(iModel < 0 || iRegion < 0 || pFile == NULL)
			return FALSE;
		
		CString		strSection;
		int			nValue;
		int			i;

		strSection.Format("PADREGION_REGION%d_REG_TYPE",iRegion);
		pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, nValue,0);
		m_emRegType = (emRegionType)nValue;
		strSection.Format("PADREGION_REGION%d_REG_GROUP",iRegion);
		pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, m_nGroupIdx,0);				

		switch(m_emRegType)
		{
		case PADREGION_RECT:
		case PADREGION_NONEAREA:
			strSection.Format("PADREGION_REGION%d_REG_LEFT",iRegion);
			pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, nValue,0);
			m_rectangle.left = nValue;
			strSection.Format("PADREGION_REGION%d_REG_TOP",iRegion);
			pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, nValue,0);
			m_rectangle.top = nValue;
			strSection.Format("PADREGION_REGION%d_REG_RIGHT",iRegion);
			pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, nValue,0);
			m_rectangle.right = nValue;
			strSection.Format("PADREGION_REGION%d_REG_BOTTOM",iRegion);
			pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, nValue,0);
			m_rectangle.bottom = nValue;
			break;
		case PADREGION_POLY:
			strSection.Format("PADREGION_REGION%d_REG_POLYCNT",iRegion);
			pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, m_nPolyPointCnt,0);
			if(m_nPolyPointCnt >= PAD_MAX_POLYGON_POINT) 
				m_nPolyPointCnt=PAD_MAX_POLYGON_POINT;
			if(m_nPolyPointCnt < 0) 
				m_nPolyPointCnt=0;
					
			for(i=0;i<m_nPolyPointCnt;i++)
			{
				strSection.Format("PADREGION_REGION%d_REG_POS%d_X",iRegion,i);
				pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, nValue,0);
				m_pPolyPoint[i].x = nValue;
				strSection.Format("PADREGION_REGION%d_REG_POS%d_Y",iRegion,i);
				pFile->GetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, nValue,0);
				m_pPolyPoint[i].y = nValue;
			}
			break;
		}
		
		return TRUE;
	}
	BOOL WriteRegion(int iModel,int iRegion,CHMRegiConfig* pFile)
	{
		if(iModel < 0 || iRegion < 0 || pFile == NULL)
			return FALSE;
		
		CString		strSection;		
		int			i;
				
		strSection.Format("PADREGION_REGION%d_REG_TYPE",iRegion);
		pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, (int)m_emRegType);	
		strSection.Format("PADREGION_REGION%d_REG_GROUP",iRegion);
		pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, m_nGroupIdx);			

		switch(m_emRegType)
		{
		case PADREGION_RECT:
		case PADREGION_NONEAREA:
			strSection.Format("PADREGION_REGION%d_REG_LEFT",iRegion);
			pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, (int)m_rectangle.left);
			strSection.Format("PADREGION_REGION%d_REG_TOP",iRegion);
			pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, (int)m_rectangle.top);
			strSection.Format("PADREGION_REGION%d_REG_RIGHT",iRegion);
			pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, (int)m_rectangle.right);
			strSection.Format("PADREGION_REGION%d_REG_BOTTOM",iRegion);
			pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, (int)m_rectangle.bottom);
			break;
		case PADREGION_POLY:
			if(m_nPolyPointCnt >= PAD_MAX_POLYGON_POINT) 
				m_nPolyPointCnt=PAD_MAX_POLYGON_POINT;
			if(m_nPolyPointCnt < 0) 
				m_nPolyPointCnt=0;

			strSection.Format("PADREGION_REGION%d_REG_POLYCNT",iRegion);
			pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, m_nPolyPointCnt);
			for(i=0;i<m_nPolyPointCnt;i++)
			{
				strSection.Format("PADREGION_REGION%d_REG_POS%d_X",iRegion,i);
				pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, (int)m_pPolyPoint[i].x);
				strSection.Format("PADREGION_REGION%d_REG_POS%d_Y",iRegion,i);
				pFile->SetItemValue(iModel,(LPSTR)(LPCTSTR)strSection, (int)m_pPolyPoint[i].y);				
			}
			break;
		}
		return TRUE;
	}
	BOOL		ReadRecipe(CMosisPacket *pPacket)
	{
		int			i;		
		int			nValue;
		
		pPacket->ReadValue(nValue);
		m_emRegType = (emRegionType)nValue;
		
		switch(m_emRegType)
		{
		case PADREGION_RECT:
		case PADREGION_NONEAREA:
			pPacket->ReadValue(m_rectangle.left);
			pPacket->ReadValue(m_rectangle.top);
			pPacket->ReadValue(m_rectangle.right);
			pPacket->ReadValue(m_rectangle.bottom);			
			break;
		case PADREGION_POLY:
			pPacket->ReadValue(m_nPolyPointCnt);
			
			if(m_nPolyPointCnt > PAD_MAX_POLYGON_POINT) 
				m_nPolyPointCnt=PAD_MAX_POLYGON_POINT;
			if(m_nPolyPointCnt < 0) 
				m_nPolyPointCnt=0;

			for(i=0;i<m_nPolyPointCnt;i++)
			{
				pPacket->ReadValue(m_pPolyPoint[i].x);
				pPacket->ReadValue(m_pPolyPoint[i].y);
			}
			break;
		}
		return TRUE;
	}
	BOOL		WriteRecipe(CMosisPacket *pPacket)
	{
		int			i;		
		int			nValue;
		
		nValue = (int)m_emRegType;
		pPacket->WriteValue(nValue);				

		switch(m_emRegType)
		{
		case PADREGION_RECT:
		case PADREGION_NONEAREA:
			pPacket->WriteValue(m_rectangle.left);
			pPacket->WriteValue(m_rectangle.top);
			pPacket->WriteValue(m_rectangle.right);
			pPacket->WriteValue(m_rectangle.bottom);			
			break;
		case PADREGION_POLY:
			if(m_nPolyPointCnt > PAD_MAX_POLYGON_POINT) 
				m_nPolyPointCnt=PAD_MAX_POLYGON_POINT;
			if(m_nPolyPointCnt < 0) 
				m_nPolyPointCnt=0;

			pPacket->WriteValue(m_nPolyPointCnt);			

			for(i=0;i<m_nPolyPointCnt;i++)
			{
				pPacket->WriteValue(m_pPolyPoint[i].x);
				pPacket->WriteValue(m_pPolyPoint[i].y);
			}
			break;
		}	
		return TRUE;
	}
};

class AFX_EXT_CLASS CPAD_ManData
{
public:		
	emPADINSTYPE	m_emInsType;
	LONG			m_nID;
	CPADRegion		m_pRegion;
	CPADRegionParm	*m_pParm;	
	
public:
	CPAD_ManData()
	{
		m_pParm = NULL;
		m_pRegion.Reset();			
		m_emInsType = PADINSTYPE_PITCH;
		m_nID = -1;
	}
	~CPAD_ManData()
	{
		if (m_pParm) delete m_pParm, m_pParm = NULL;				
	}
	CPAD_ManData& operator=(CPAD_ManData& rhs)
	{
		if (m_pParm) delete m_pParm, m_pParm = NULL;

		m_emInsType = rhs.m_emInsType;
		m_nID = rhs.m_nID;
		m_pRegion = rhs.m_pRegion;
		switch (m_emInsType)
		{
		case PADINSTYPE_PITCH:		
		case PADINSTYPE_ASG:
			m_pParm = new CPADParmPitch;	
			break;			
		default:			
			return *this;
		}
		*m_pParm = *rhs.m_pParm;

		return *this;
	}
};

class AFX_EXT_CLASS CPAD_ManDataArray : public CArray<CPAD_ManData *, CPAD_ManData *>
{
public:
	~CPAD_ManDataArray()
	{
		DeleteAll();
	}		
	LONG FindMaxID()
	{
		LONG	nID = 0;
		for (LONG i = 0; i < GetSize(); i++)
		{
			if(nID < GetAt(i)->m_nID) nID = GetAt(i)->m_nID;
		}
		
		return nID;
	}
	void DeleteAt(LONG nIndex)
	{
		if(nIndex >= GetSize()) return;

		for (int i = 0; i < GetSize(); i++)
		{
			if(nIndex == i)
			{					
				delete GetAt(i);
				RemoveAt(i);
				break;
			}
		}
		
	}
	void DeleteAll()
	{
		for (int i = 0; i < GetSize(); i++)
			delete GetAt(i);
		RemoveAll();
	}
};

class AFX_EXT_CLASS CPADRegionInfo
{
public:
	CPADRegionInfo();
	virtual ~CPADRegionInfo();
	CPADRegionInfo& operator=(CPADRegionInfo& rhs);

protected:	
	CPAD_ManDataArray	m_pPADDataManager;
	CPADRegionNoneArea	m_pPADNoneArea;

public:
	int			AddRegion(emPADINSTYPE emInsType);
	int			AddNoneArea(CRect rect);
	BOOL		ReadRegionInfo(int iModel,CHMRegiConfig* pFile);
	BOOL		WriteRegionInfo(int iModel,CHMRegiConfig* pFile);
	BOOL		ResetRegionInfo();
	int			GetDataCount(){return static_cast<int>(m_pPADDataManager.GetSize());}
	int			GetNoneAreaCount(){return static_cast<int>(m_pPADNoneArea.GetSize());}
	CPAD_ManData*	GetManData(int nIndex);
	CRect		GetNoneAreaData(int nIndex);
	BOOL		ReadRecipe(CMosisPacket *pPacket);
	BOOL		WriteRecipe(CMosisPacket *pPacket);

};

#endif // !defined(AFX_PADREGION_H__3BA9C7BC_907E_4E3E_8EE1_44CE00B3D434__INCLUDED_)
