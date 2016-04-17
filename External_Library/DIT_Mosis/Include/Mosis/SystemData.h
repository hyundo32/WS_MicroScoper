// SystemData.h: interface for the CSystemData struct.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMDATA_H__95388BE6_CC9C_451C_A590_E5F190FAD6DA__INCLUDED_)
#define AFX_SYSTEMDATA_H__95388BE6_CC9C_451C_A590_E5F190FAD6DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MosisHive.h"
#include "MosisPacket.h"


// 시스템 고유 데이타.
struct AFX_EXT_CLASS stSystemData
{
	int			m_StartIPAdress;
	int			m_nCamera;
	int			m_nScan;
	
	int			m_Suppress;
	double		m_ScanResolution;
	
	BOOL		m_bCameraFlip;

	int			m_MaxGlassDefect;
	int			m_MaxFrameDefect;
	int			m_GlassTact;
	int			m_ScanTact;

	int			m_FrameWidth;
	int			m_FrameHeight;

	BOOL		m_bUseShrink;
	
	stSystemData	&SelfSystemData(){return *this;}
};


// 스캔별 고유 데이타.
struct AFX_EXT_CLASS stScanData
{
	int			m_LeftMargin;			// 카메라 마진 left
	int			m_RightMargin;			// 카메라 마진 right
double		m_AccumCoord;			// 카메라 누적 좌표.(um)
	double		m_ConvResolution;		// 카메라 해상도.

	int			m_GlassStartLine;		// 글라스 스타트 라인
	int			m_GlassStarrOffset;		// 글라스 스타트 라인 카메라별 offset
//	int			m_Crystal;// 수정봉???


	stScanData	&SelfScanData()	{return *this;}
};
typedef			CMosisHive2<stScanData>		CScanDataHive2;



#endif // !defined(AFX_SYSTEMDATA_H__95388BE6_CC9C_451C_A590_E5F190FAD6DA__INCLUDED_)
