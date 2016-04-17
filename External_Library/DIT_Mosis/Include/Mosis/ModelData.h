// ModelData.h: interface for the CModelData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELDATA_H__0F64EB90_305E_44B1_898C_9753FFBD0B45__INCLUDED_)
#define AFX_MODELDATA_H__0F64EB90_305E_44B1_898C_9753FFBD0B45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MosisHive.h"
#include "MosisPacket.h"

// ���� ���.
struct AFX_EXT_CLASS stPatternEdge
{
	BOOL		m_FindBoundary;
	// ���ã�� ���� (�˻� �̹��� ũ��)
	int			m_FindPatternWidth;
	int			m_FindPatternHeight;
	int			m_FindSideWidth;
	int			m_FindSideHeight;
	
	// ���ã�� ���� (�˻� ����)
	int			m_Threshold;
	double		m_DefectRatio;
	int			m_ContinueCnt;
	
	// ���� �˻� ���� (�˻� ���� ���� �ȼ� ũ��)
	int			m_PatternDummy;
	int			m_PatternDummySide;
};


// �˻� ����.
struct AFX_EXT_CLASS	stConvData
{
	int			m_InspMethodPat;	// �˻� ���.
	int			m_ConvDir;			// �˻� ����.
	double		m_ConvPitch;		// ������ ���� ��ġ(Pitch). ī�޶� ����.
	int			m_ConvPitchCycle;	// ������ ���� ��ġ ���.
	double		m_ScanPitch;		// Ⱦ���� ���� ��ġ. ī�޶� ����.
	int			m_ScanPitchCycle;	// Ⱦ���� ���� ��ġ ���.

	int			m_ConvThreshold;	// �˻� ���ΰ�.
	int			m_ConvThreshold2;	// 2�� ���ΰ� for Dynamic Threshold : 2*Bright/255+ 1

	int			m_ConvWidth;		// Convolution Width.
	int			m_ConvHeight;		// Convolution Height.

	int			m_Suppress;			// �˻� Suppress.

	double		GetInspectConvPitch(){return m_ConvPitch*m_ConvPitchCycle;}
	double		GetInspectScanPitch(){return m_ScanPitch*m_ScanPitchCycle;}
};



class AFX_EXT_CLASS CModelData
{
public:
	stPatternEdge	m_PatternEdge;
	stConvData		m_PatternConv;


public:
	CModelData	&SelfModelData(){return *this;}
	BOOL		ReadModelData(CMosisPacket *pPacket);
	BOOL		WriteModelData(CMosisPacket *pPacket);

};
typedef		CMosisHive1<CModelData>		CModelDataHive;



#endif // !defined(AFX_MODELDATA_H__0F64EB90_305E_44B1_898C_9753FFBD0B45__INCLUDED_)
