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


struct AFX_EXT_CLASS stCellInfo// �˻� ��ǥ ( lengths )
{
	int			m_nCell;
	
	int			m_Edge_Bm;
	int			m_Bm_Pattern;
	int			m_PatternLength;
	int			m_Pattern_Bm;
	int			m_Bm_Edge;
};

class AFX_EXT_CLASS CCellCoord //: public stCellCoord// �˻� ��ǥ 
{
public:
	stCellInfo		m_CellInfo;
#define MAX_CELL 50
	
public:
	int			m_Positions[MAX_CELL];// �� ���� start position.
	int			m_Distances[MAX_CELL];

	void		TestCellCoord();
	
	int			GetStartDistance(int i);// �� ��ġ�� ���� �Ÿ� ������(���� �� ��ŸƮ Ȥ�� �۶� ��ŸƮ�������κ����� �Ÿ�)�� �����Ѵ�.
	void		MinePositionData();//// ���Ÿ� �����͸� �� ���� ��ġ ������ ��ȯ�Ѵ�.
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
