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

// 패턴 경계.
struct AFX_EXT_CLASS stPatternEdge
{
	BOOL		m_FindBoundary;
	// 경계찾기 관련 (검사 이미지 크기)
	int			m_FindPatternWidth;
	int			m_FindPatternHeight;
	int			m_FindSideWidth;
	int			m_FindSideHeight;
	
	// 경계찾기 관련 (검사 조건)
	int			m_Threshold;
	double		m_DefectRatio;
	int			m_ContinueCnt;
	
	// 경계부 검사 더미 (검사 제외 영역 픽셀 크기)
	int			m_PatternDummy;
	int			m_PatternDummySide;
};


// 검사 정보.
struct AFX_EXT_CLASS	stConvData
{
	int			m_InspMethodPat;	// 검사 방식.
	int			m_ConvDir;			// 검사 방향.
	double		m_ConvPitch;		// 종방향 패턴 피치(Pitch). 카메라 관점.
	int			m_ConvPitchCycle;	// 종방향 패턴 피치 배수.
	double		m_ScanPitch;		// 횡방향 패턴 피치. 카메라 관점.
	int			m_ScanPitchCycle;	// 횡방향 패턴 피치 배수.

	int			m_ConvThreshold;	// 검사 문턱값.
	int			m_ConvThreshold2;	// 2차 문턱값 for Dynamic Threshold : 2*Bright/255+ 1

	int			m_ConvWidth;		// Convolution Width.
	int			m_ConvHeight;		// Convolution Height.

	int			m_Suppress;			// 검사 Suppress.

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
