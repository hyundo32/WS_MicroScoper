// PADRegionParm.h: interface for the CPADRegionParm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PADREGIONPARM_H__10A29210_3AD6_4C7F_B5A2_C7B7FDE24BF7__INCLUDED_)
#define AFX_PADREGIONPARM_H__10A29210_3AD6_4C7F_B5A2_C7B7FDE24BF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
//#include "HMRegiConfig.h"
#include "MosisPacket.h"

class CHMRegiConfig;
enum	emPADSIDE{SIDE_LEFT=0,SIDE_TOP,SIDE_RIGHT,SIDE_BOTTOM,SIDE_NONE};
enum	emPADINSTYPE{PADINSTYPE_PITCH=0,PADINSTYPE_ASG,PADINSTYPE_C2C};
enum	emPADPITCHINSDIR{INSDIR_HORI=0,INSDIR_VERT};

class AFX_EXT_CLASS CPADRegionParm  
{
public:
	CPADRegionParm();
	virtual ~CPADRegionParm();

public:
	virtual void		Reset(){};	
	virtual BOOL		GetUseInspect(){return FALSE;}
	virtual BOOL		ReadRegionParm(int iModel,int iRegion,CHMRegiConfig* pFile){return FALSE;}
	virtual BOOL		WriteRegionParm(int iModel,int iRegion,CHMRegiConfig* pFile){return FALSE;}
	virtual void*		GetRegionParm(){return NULL;}
	virtual void		SetRegionParm(void *pParm){}
	virtual int			GetThreshold(){return 0;}
	virtual BOOL		ReadRecipe(CMosisPacket *pPacket){return FALSE;}
	virtual BOOL		WriteRecipe(CMosisPacket *pPacket){return FALSE;}

};

typedef struct _PADPARMPITCH
{
	BOOL				m_bUseInspect;
	BOOL				m_bUseFPC;
	emPADPITCHINSDIR	m_emInspectDir;
	double				m_dPitchMin;
	double				m_dPitchMax;
	int					m_nPitchCycle;
	int					m_nThreshold;
	int					m_nSuppress;
	
	_PADPARMPITCH()
	{
		Reset();
	}
	void Reset()
	{
		m_bUseInspect	= FALSE;
		m_bUseFPC	= FALSE;	
		m_emInspectDir = INSDIR_HORI;
		m_dPitchMin = m_dPitchMax = 0.;
		m_nPitchCycle = 1;
		m_nThreshold = 255;
		m_nSuppress = 255;
	}
	_PADPARMPITCH& operator=(const _PADPARMPITCH &rh)
	{
		m_bUseInspect = rh.m_bUseInspect;
		m_bUseFPC = rh.m_bUseFPC;
		m_emInspectDir = rh.m_emInspectDir;
		m_dPitchMin = rh.m_dPitchMin;
		m_dPitchMax = rh.m_dPitchMax;
		m_nPitchCycle = rh.m_nPitchCycle;
		m_nThreshold = rh.m_nThreshold;
		m_nSuppress = rh.m_nSuppress;		
		return *this;
	}
} PADPARMPITCH, *pPADPARMPITCH;

class AFX_EXT_CLASS CPADParmPitch : public CPADRegionParm
{
public:
	CPADParmPitch();
	virtual ~CPADParmPitch();
	CPADParmPitch& operator=(CPADParmPitch& rhs);
	
public:
	virtual void		Reset();	
	virtual BOOL		GetUseInspect(){return m_pPitchParm.m_bUseInspect;}
	virtual BOOL		ReadRegionParm(int iModel,int iRegion,CHMRegiConfig* pFile);
	virtual BOOL		WriteRegionParm(int iModel,int iRegion,CHMRegiConfig* pFile);
	virtual void*		GetRegionParm(){return static_cast<void*>(&m_pPitchParm);}	
	virtual void		SetRegionParm(void *pParm){ m_pPitchParm=*(static_cast<pPADPARMPITCH>(pParm));}
	virtual int			GetThreshold(){return m_pPitchParm.m_nThreshold;}
	virtual BOOL		ReadRecipe(CMosisPacket *pPacket);
	virtual BOOL		WriteRecipe(CMosisPacket *pPacket);
	
protected:
	PADPARMPITCH		m_pPitchParm;	
};

typedef struct _PADPARMC2C_PATTERNINFO
{
	BOOL			m_bUseFirst;
	BOOL			m_bUseSecond;
	int				m_nCam;
	int				m_nScan;
	CRect			m_FirstRect;
	CRect			m_SecondRect;

	_PADPARMC2C_PATTERNINFO()
	{
		Reset();
	}
	void Reset()
	{
		m_bUseFirst = m_bUseSecond = FALSE;
		m_FirstRect = m_SecondRect = CRect(0,0,0,0);
	}
	_PADPARMC2C_PATTERNINFO& operator=(const _PADPARMC2C_PATTERNINFO &rh)
	{
		m_bUseFirst = rh.m_bUseFirst;	
		m_bUseSecond = rh.m_bUseSecond;	
		m_nCam = rh.m_nCam;	
		m_nScan = rh.m_nScan;	
		m_FirstRect = rh.m_FirstRect;	
		m_SecondRect = rh.m_SecondRect;	
		return *this;
	}
}PADPARMC2C_PATTERNINFO,*pPADPARMC2C_PATTERNINFO;

typedef struct _PADPARMC2C
{
	BOOL			m_bUseInspect;
	int				m_nThreshold;
	int				m_nPatternCnt;
	pPADPARMC2C_PATTERNINFO	m_pPatternInfo;

	_PADPARMC2C()
	{		
		m_pPatternInfo = NULL;
		Reset();
	}
	~_PADPARMC2C()
	{
		Reset();
	}
	void Reset()
	{
		m_bUseInspect = FALSE;
		m_nThreshold = 0;				
		m_nPatternCnt = 0;
		if(m_pPatternInfo != NULL)
			delete[] m_pPatternInfo, m_pPatternInfo=NULL;
	}
	void SetPatternCnt(int nCnt)
	{
		m_nPatternCnt = nCnt;
		if(m_pPatternInfo != NULL)
			delete[] m_pPatternInfo, m_pPatternInfo=NULL;
		if(m_nPatternCnt > 0)
			m_pPatternInfo = new PADPARMC2C_PATTERNINFO[m_nPatternCnt];
	}
	void SetPatternData(int nIndex,PADPARMC2C_PATTERNINFO &pData)
	{
		if(nIndex >= m_nPatternCnt || m_pPatternInfo == NULL)
			return;

		m_pPatternInfo[nIndex] = pData;
	}
	PADPARMC2C_PATTERNINFO *GetPatternData(int nIndex)
	{
		if(nIndex >= m_nPatternCnt || m_pPatternInfo == NULL)
			return NULL;

		return &m_pPatternInfo[nIndex];
	}
	_PADPARMC2C& operator=(const _PADPARMC2C &rh)
	{
		int			iLoop;

		Reset();

		m_bUseInspect = rh.m_bUseInspect;	
		m_nThreshold = rh.m_nThreshold;	
		m_nPatternCnt = rh.m_nPatternCnt;

		if(m_nPatternCnt > 0)
			m_pPatternInfo = new PADPARMC2C_PATTERNINFO[m_nPatternCnt];

		for(iLoop=0;iLoop<m_nPatternCnt;iLoop++)
		{
			m_pPatternInfo[iLoop] = rh.m_pPatternInfo[iLoop];
		}

		return *this;
	}
} PADPARMC2C, *pPADPARMC2C;

class AFX_EXT_CLASS CPADParmC2C
{
public:
	CPADParmC2C();
	virtual ~CPADParmC2C();
	CPADParmC2C& operator=(CPADParmC2C& rhs);
	
public:
	void		Reset();	
	BOOL		GetUseInspect(){return m_pC2CParm.m_bUseInspect;}
	BOOL		ReadRegionParm(int iModel,CHMRegiConfig* pFile);
	BOOL		WriteRegionParm(int iModel,CHMRegiConfig* pFile);
	pPADPARMC2C	GetRegionParm(){return &m_pC2CParm;}
	void		SetRegionParm(pPADPARMC2C pParm){ m_pC2CParm=*pParm;}	
	int			GetThreshold(){return m_pC2CParm.m_nThreshold;}
	BOOL		ReadRecipe(CMosisPacket *pPacket);
	BOOL		WriteRecipe(CMosisPacket *pPacket);

protected:
	PADPARMC2C			m_pC2CParm;
	
};

#endif // !defined(AFX_PADREGIONPARM_H__10A29210_3AD6_4C7F_B5A2_C7B7FDE24BF7__INCLUDED_)
