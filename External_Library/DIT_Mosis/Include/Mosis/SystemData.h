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


// �ý��� ���� ����Ÿ.
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


// ��ĵ�� ���� ����Ÿ.
struct AFX_EXT_CLASS stScanData
{
	int			m_LeftMargin;			// ī�޶� ���� left
	int			m_RightMargin;			// ī�޶� ���� right
double		m_AccumCoord;			// ī�޶� ���� ��ǥ.(um)
	double		m_ConvResolution;		// ī�޶� �ػ�.

	int			m_GlassStartLine;		// �۶� ��ŸƮ ����
	int			m_GlassStarrOffset;		// �۶� ��ŸƮ ���� ī�޶� offset
//	int			m_Crystal;// ������???


	stScanData	&SelfScanData()	{return *this;}
};
typedef			CMosisHive2<stScanData>		CScanDataHive2;



#endif // !defined(AFX_SYSTEMDATA_H__95388BE6_CC9C_451C_A590_E5F190FAD6DA__INCLUDED_)
