// ModelCoord.h: interface for the CModelCoord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELCOORD_H__C092E22E_AEBD_4C88_B85C_33828F16E54E__INCLUDED_)
#define AFX_MODELCOORD_H__C092E22E_AEBD_4C88_B85C_33828F16E54E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MosisHive.h"
#include "MosisPacket.h"


struct AFX_EXT_CLASS stCellInfo// 검사 좌표 ( lengths )
{
	int			m_nCell;
	
	int			m_Edge_Bm;
	int			m_Bm_Pattern;
	int			m_PatternLength;
	int			m_Pattern_Bm;
	int			m_Bm_Edge;
};

class AFX_EXT_CLASS CCellCoord //: public stCellCoord// 검사 좌표 
{
public:
	stCellInfo		m_CellInfo;
#define MAX_CELL 50
	
public:
	int			m_Positions[MAX_CELL];// 각 셀의 start position.
	int			m_Distances[MAX_CELL];

	void		TestCellCoord();
	
	int			GetStartDistance(int i);// 셀 위치에 따라 거리 데이터(이전 셀 스타트 혹은 글라스 스타트라인으로부터의 거리)를 리턴한다.
	void		MinePositionData();//// 셀거리 데이터를 셀 시작 위치 정보로 변환한다.
	void		Scale(double resolution);
	
	BOOL		ReadCellCoord(CMosisPacket *pPacket);
	BOOL		WriteCellCoord(CMosisPacket *pPacket);
};



class AFX_EXT_CLASS CModelCoord
{
protected:
	CCellCoord		m_xCellCoord;
	CCellCoord		m_yCellCoord;

public:
	CModelCoord		&SelfModelCoord()	{return *this;}
	void			TestModelCoord();
	BOOL			ReadModelCoord(CMosisPacket *pPacket);
	BOOL			WriteModelCoord(CMosisPacket *pPacket);
	
	void			ChangeXY();
	friend			class	CGlassModel;
	friend			class	CScanMdel;
};

typedef			CMosisHive1<CModelCoord>		CModelCoordHive;


#endif // !defined(AFX_MODELCOORD_H__C092E22E_AEBD_4C88_B85C_33828F16E54E__INCLUDED_)
