#pragma once

#include "Pad.h"
#include "AssemDefect.h"


enum ConvMode		{ ConvMode_Float = 0, ConvMode_Pixel, ConvMode_Float_3By2 };
enum ConvDir		{ ConvDir_Hori= 0, ConvDir_Vert= 1, ConvDir_Double= 2};


class AFX_EXT_CLASS CConvParam
{
public:
	// 1.1. �˻� ��� ����
	ConvMode	m_ConvMode;		// 0 = ConvFloat(2*2), 1 = ConvPixel(1*1)
	int			m_ConvDir;		// 0 = Horizontal, 1= Vertical, 2, Hori and Vert
	int			m_ReinspectCount;		// Frame Overflow�� ��˻� ȸ��
	BOOL		m_bZone2B2;				// Zone 2 By 2 �˻�

	// 1.2. �˻� �̹��� ����
	LPBYTE		s_lpBuffer;				// * �˻��� ����
	int			s_nFrameWidth;			// * �˻� �̹��� ����
	int			s_nFrameHeight;			// * �˻� �̹��� ����.

	// 1.3. �˻� ���� ����.
	CTrapezium	m_Trapezium;			// ��ٸ��� �˻� ����
	CRect		s_RectConv;				// * ���ۿ��� ������ �簢��
	CRect		s_RectFilter;			// * ���ۿ��� ���͸��� ���� �� �ִ� �簢��(���� ��ũ�� �� ����)


	// 1.5. Convolution ����.
	int			s_nConvWidth;			// * �˻� ���� ����
	int			s_nConvHeight;			// * �˻� ���� ����

	double		s_dPitchConvReal;		// * �˻� ��ġ
	int			s_nPitchCycleConv;		// �� ���� ��ġ
	double		s_dPitchScanReal;		// * �˻� ��ġ
	int			s_nPitchCycleScan;		// �� ���� ��ġ

	float		m_fTilt;				// tilt ��. 

	// 1.6. ���ΰ� ����.
	int			s_nThreshold;			// * ���� ���ΰ�
	int			s_nDThSlide;			// Dynamic Threshold ����
	int			s_nDThMin;				// Dynamic Threshold ���� ����... ���� �Ⱦ�.
	int			s_nThresholdSupress;	// Suppress ���ΰ�.
	int			s_nSideThreshold;		// ��/��/Unpair �˻� ���ΰ�
	int			s_nBWB_Main;		// Black ���� ��������� ���͸� DTh(ConvPixel VertSBW, VTDSBW�� ����), 0�̸� �Ⱦ�.
	int			s_nBWB_Slide;		// Black ���� ��������� ���͸� DTh(ConvPixel VertSBW, VTDSBW�� ����)
	int			m_EchoThreshold;		// Echo ���͸����� �ֺ� ������ ���� �ּҰ�.

	// 1.7. �˻� Process ����.
	BOOL		m_bEchoConvolution;	// ���� �ӵ� �������� ��� ���ο��� Thread ���� �˻� ����.
	BOOL		m_bVertSpeedup;		// Pixel�� Zone �ӵ� �������� �˻� ��ƾ�� �޸� ���ؼ� ����
	BOOL		m_bSelectiveDir;	// ���� ���⿡�� 3��ġ ������ ��������� cross �������� ������ �˻� ���� ����.
	BOOL		m_bUseUnpair;			// Left�� Right�� Unpair ������ Assem�� Pairing �ܰ迡�� �̾� ��(TRUE) or ����(FALSE). �⺻ TRUE;
	BOOL		s_bReverseFilter;		// ReverseFilter ��� ����
	int			m_EdgeFilterTh;			// �� Edge �������� ���ֱ� ���� ������ �� ���� ������ ���� �߰� Th

	// 1.8. Dynamic Pitch ����
	int			*m_pPitch16;	// pitch�� 16�� �ؼ� subpixel���� ǥ��, Trapezium�̿��� Rect������ Vertical �����̴�.
	short		*m_pPitch;		// Dynamic Pitch. Horizontal �����̰�
	short		*m_pfSPR;		// Dynamic Pitch. Horizontal �����̰�, Sub Pixel
	short		*m_psSPR;		// Dynamic Pitch. Horizontal �����̰�, Sub Pixel
	BOOL		m_bUseDPC;
	int			m_PitchWidth;	// Pitch Table �� 2������ ��� Table width�� �����Ѵ�.(2���� �˻翡 ���� flag ���ұ��� ����, 0�� ��� 1����)

	// 1.7. Zone Threshold �˻� ����
	BOOL		m_bZoneMulti;			// Zone Multi Threshold �˻� ���� ����.
	short*		s_psZoneTable;				// Zone�� Thresholding�� ���� Index Table
	short*		s_psZoneThTable;			// Zone�� Thresholding�� ���� Threshold Table
	short*		s_psZoneAddTable;			// Zone�� Thresholding�� ���� DynamicThreshold Table
	short*		s_psZoneMThTable;		// 2�� ZoneThreshold�� �ι�° Base Threshold
	short*		s_psZoneMAddTable;		// 2�� ZoneThreshold�� �ι�° Threshold Slope
	short*		s_psZoneMMinTable;			// 2�� ZoneThreshold�� 2��° ������ ���� �Ӱ�ġ(�ȼ� ���), 2�� ZoneThreshold���� ������ ������ ���ҵ� ����.
	int			s_nThresTableWidth;		// Threshold Table ����
	int			s_nThresTableHeight;	// Threshold Table ����
	int			s_nThresTableStartX;	// X�� ���� Offset
	int			s_nThresTableStartY;	// Y�� ���� Offset
	void*		m_pGammaTable;			// Zone Threshold �ι�° �������� ��Ī ���̺� ���� ����.
	int			m_nGammaXCount;			// Gamma Table�� X, Y �迭�� X�� ��Ī����Ʈ ī��Ʈ(����)
	int			s_nZoneCycleConv;		// Zone Reference �̹����� �⺻�ֱ� ���
	int			s_nZoneCycleScan;		// Zone Reference �̹����� �⺻�ֱ� ���
	int*		m_pFilterThBlack;
	int*		m_pFilterThBlack2;
	int*		m_pFilterThWhite;
	int*		m_pFilterThWhite2;


	int			GetFilterThreshold(int iZone, int pixel, DefectType Type)
	{
		if(Type == DEFTYPE_WHITE)
			return ((m_pFilterThWhite2[iZone]*pixel)>>8)+ m_pFilterThWhite[iZone];

		return ((m_pFilterThBlack2[iZone]*pixel)>>8)+ m_pFilterThBlack[iZone];
	}


	//==================================
	// Assem �� ���� ��ó���� ���õ� ����.

	// 2.1.  ���� ���� ����.
	int			s_nFrameDefPixLimit;	// �� Frame������ �ִ� ���� �ȼ� ��
	int			s_nFrameDefBlobLimit;	// �� Frame������ �ִ� ���� ��

	// 2.2. �ȼ� �ܰ� ���͸� ����.
	BOOL		m_bUsePixelFilter;		// Pixel�ܰ迡�� ���͸��� �� ���ΰ� �� ���ΰ�?
	BOOL		m_bOnlyJumpFilter;		// �˻� ���� Cross �������� ���͸� ���� JumpFiltering�� �� ���ΰ� .or. CrossFilter�� �� ���ΰ�.
	BOOL		m_bPairFilter;			// Pair ���Կ��� Cross&Jump ���� ����.

	// 2.2 Zone Measure Filter // Ư�� ���� ���� �̼� ���� �� ��Ī�� �ٽ� �˻� ���� ����.
	BOOL		m_bTiltFilter;
 
	// 2.3. ��ó�� ����.
	int			m_nDefectMergePixel;	// ���� ���� ������

	// 2.4. Zone Sorting, Thresholding
	BYTE*		s_pZoneData;			// Zone Sort Table
	BYTE*		s_pZoneThreshold;		// Zone Threshold Table
	int			s_nZoneDataWidth;		// Zone Sort Table ����
	int			s_nZoneBufferWidth;		// Zone Data ������ �̹��� ���� ũ��.
	int			s_nZoneDataHeight;		// Zone Sort Table ����

	// 2.5. ���� ���� ���� ����.
	double		s_dScanResolution;		// Scan Resolution
	double		s_dConvResolution;		// Conv Resolution



	//======================================
	// 99. �� ������ ����� ������ ���� �͵�.
	short		s_sSkipSave;			// �˻� �� �̹��� ���� ����
	UINT		s_uiProcessing;			// * �˻� ���
	int			s_nRegionType;			// ���� - ������ ���� ���Կ� ���� ����

	// �Ⱦ��̴µ� ���� ���°���??? �ƴ� ��� ���ϼ����忡�� ��ȭ �ϱ�.
	int			s_nUseSize;
	int			s_nSizeThres;

	// Add by PJH
	int			s_nASGDynamicHi;
	int			s_nASGDynamicLow;

	// 99. ��Ÿ ��ٱ���
	// 200090612 size choigudal
	BOOL		s_bUseTrimmer;
	int			s_nTrimmerWidth;
	int			s_nTrimmerHeight;





	CConvParam()
	{
		Reset();
	}

	double	GetRealConvPitch(){return s_dPitchConvReal*s_nPitchCycleConv;}
	double	GetRealScanPitch(){return s_dPitchScanReal*s_nPitchCycleScan;}
	double	GetZoneConvPitch(){return s_dPitchConvReal*s_nZoneCycleConv;}
	double	GetZoneScanPitch(){return s_dPitchScanReal*s_nZoneCycleScan;}

	void Reset()
	{
		// 1.1. �˻� ��� ����
		m_ConvMode					= ConvMode_Float;
		m_ConvDir					= 0;
		m_ReinspectCount			= 3;
		m_bZone2B2					= FALSE;

		// 1.2. �˻� �̹��� ����
		s_lpBuffer					= NULL;
		s_nFrameWidth				= 0;
		s_nFrameHeight				= 0;

		// 1.3. �˻� ���� ����.
		s_RectConv.SetRect(0, 0, 0, 0);
		s_RectFilter= s_RectConv;

		// 1.5. Convolution ����.
		s_nConvWidth				= 0;
		s_nConvHeight				= 0;

		s_dPitchConvReal			= 0.0;
		s_nPitchCycleConv			= 0;
		s_dPitchScanReal			= 0.0;
		s_nPitchCycleScan			= 0;

		m_fTilt						= 0.0;

		// 1.6. ���ΰ� ����.
		s_nThreshold				= 10;
		s_nDThSlide					= 20;
		s_nDThMin					= 255;
		s_nThresholdSupress			= 255;
		s_nSideThreshold			= 0;
		s_nBWB_Main					= 0;
		s_nBWB_Slide				= 0;
		m_EchoThreshold				= 0;

		// 1.7. �˻� Process ����.
		m_bEchoConvolution			= TRUE;
		m_bVertSpeedup				= TRUE;
		m_bSelectiveDir				= FALSE;
		m_bUseUnpair				= TRUE;
		s_bReverseFilter			= FALSE;
		m_EdgeFilterTh				= 0;

		// 1.8. Dynamic Pitch ����
		m_pPitch16					= NULL;
		m_pPitch					= NULL;
		m_pfSPR						= NULL;
		m_psSPR						= NULL;
		m_bUseDPC					= TRUE;
		m_PitchWidth				= 0;// 0�� ��� 1����, width ���� ���� ��� 2 ���� DPC


		// 1.9. Zone Threshold �˻� ����
		m_bZoneMulti				= FALSE;
		s_psZoneTable				= NULL;	//0617 �߰�
		s_psZoneThTable				= NULL;
		s_psZoneAddTable			= NULL;
		s_psZoneMThTable			= NULL;
		s_psZoneMAddTable			= NULL;
		s_psZoneMMinTable			= NULL;
		s_nThresTableWidth			= 12000;
		s_nThresTableHeight			= 1024;
		s_nThresTableStartX			= 0;
		s_nThresTableStartY			= 0;
		m_pGammaTable				= NULL;
		m_nGammaXCount				= 300;
		s_nZoneCycleConv			= 1;
		s_nZoneCycleScan			= 1;
		m_pFilterThBlack			= NULL;
		m_pFilterThBlack2			= NULL;
		m_pFilterThWhite			= NULL;
		m_pFilterThWhite2			= NULL;

		//==================================
		// Assem �� ���� ��ó���� ���õ� ����.

		// 2.1.  ���� ���� ����.
		s_nFrameDefPixLimit			= 10000;//MAX_DEFECTPIX_NUM;
		s_nFrameDefBlobLimit		= 10000;//MAX_DEFECTBLOB_NUM;

		// 2.2. ���͸� ����.
		m_bUsePixelFilter			= TRUE;
		m_bOnlyJumpFilter			= FALSE;
		m_bPairFilter				= FALSE;

		// 2.2. Zone Measure Filter // Ư�� ���� ���� �̼� ���� �� ��Ī�� �ٽ� �˻� ���� ����.
		m_bTiltFilter				= FALSE;

		// 2.3. ��ó�� ����.
		m_nDefectMergePixel			= 4;// ���� 4 �ȼ�

		// 2.4. Zone Sorting, Thresholding, Zone Thresholding�� ���� Zone ����, RSCale ������ ���� ����
		s_pZoneData					= NULL;
		s_pZoneThreshold			= NULL;
		s_nZoneDataWidth			= 0;
		s_nZoneBufferWidth			= 0;
		s_nZoneDataHeight			= 0;

		// 2.5. ���� ���� ���� ����.
		s_dScanResolution			= 0.0;
		s_dConvResolution			= 0.0;

		//======================================
		// 99. ������ �� ���� �� �𸣴� ��.
		s_sSkipSave					= 0;
		s_uiProcessing				= 0;
		s_nRegionType				= -1;

		s_nUseSize					= FALSE;
		s_nSizeThres				= 0;		  

		// Add by PJH
		s_nASGDynamicHi				= 0;
		s_nASGDynamicLow			= 0;

		// 200090612 size choigudal
		s_bUseTrimmer				= FALSE;
		s_nTrimmerWidth				= 0;
		s_nTrimmerHeight			= 0;
//.
	}

	BOOL IsAllInspected(int endLine)
	{
		if(m_ConvDir == ConvDir_Hori)
			return s_RectConv.bottom - endLine < 16;
		return s_RectConv.right - endLine < 16;
	}
};
