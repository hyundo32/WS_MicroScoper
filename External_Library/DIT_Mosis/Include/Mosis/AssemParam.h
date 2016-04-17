#pragma once

#include "Pad.h"
#include "AssemDefect.h"


enum ConvMode		{ ConvMode_Float = 0, ConvMode_Pixel, ConvMode_Float_3By2 };
enum ConvDir		{ ConvDir_Hori= 0, ConvDir_Vert= 1, ConvDir_Double= 2};


class AFX_EXT_CLASS CConvParam
{
public:
	// 1.1. 검사 방법 관련
	ConvMode	m_ConvMode;		// 0 = ConvFloat(2*2), 1 = ConvPixel(1*1)
	int			m_ConvDir;		// 0 = Horizontal, 1= Vertical, 2, Hori and Vert
	int			m_ReinspectCount;		// Frame Overflow시 재검사 회수
	BOOL		m_bZone2B2;				// Zone 2 By 2 검사

	// 1.2. 검사 이미지 정보
	LPBYTE		s_lpBuffer;				// * 검사할 버퍼
	int			s_nFrameWidth;			// * 검사 이미지 넓이
	int			s_nFrameHeight;			// * 검사 이미지 높이.

	// 1.3. 검사 영역 정보.
	CTrapezium	m_Trapezium;			// 사다리꼴 검사 영역
	CRect		s_RectConv;				// * 버퍼에서 연산할 사각형
	CRect		s_RectFilter;			// * 버퍼에서 필터링에 쓰일 수 있는 사각형(원래 셀크기 등 가능)


	// 1.5. Convolution 정보.
	int			s_nConvWidth;			// * 검사 단위 넓이
	int			s_nConvHeight;			// * 검사 단위 높이

	double		s_dPitchConvReal;		// * 검사 피치
	int			s_nPitchCycleConv;		// 한 셀의 피치
	double		s_dPitchScanReal;		// * 검사 피치
	int			s_nPitchCycleScan;		// 한 셀의 피치

	float		m_fTilt;				// tilt 값. 

	// 1.6. 문턱값 정보.
	int			s_nThreshold;			// * 메인 문턱값
	int			s_nDThSlide;			// Dynamic Threshold 기울기
	int			s_nDThMin;				// Dynamic Threshold 적용 하한... 현재 안씀.
	int			s_nThresholdSupress;	// Suppress 문턱값.
	int			s_nSideThreshold;		// 좌/우/Unpair 검사 문턱값
	int			s_nBWB_Main;		// Black 결함 어셈에서의 필터링 DTh(ConvPixel VertSBW, VTDSBW만 구현), 0이면 안씀.
	int			s_nBWB_Slide;		// Black 결함 어셈에서의 필터링 DTh(ConvPixel VertSBW, VTDSBW만 구현)
	int			m_EchoThreshold;		// Echo 필터링에서 주변 노이즈 적용 최소값.

	// 1.7. 검사 Process 정보.
	BOOL		m_bEchoConvolution;	// 연산 속도 부족으로 어셈 내부에서 Thread 분할 검사 여부.
	BOOL		m_bVertSpeedup;		// Pixel과 Zone 속도 부족으로 검사 루틴의 메모리 재해석 여부
	BOOL		m_bSelectiveDir;	// 연산 방향에서 3피치 부족시 연산방향을 cross 방향으로 변경후 검사 할지 여부.
	BOOL		m_bUseUnpair;			// Left와 Right의 Unpair 결함을 Assem의 Pairing 단계에서 뽑아 냄(TRUE) or 버림(FALSE). 기본 TRUE;
	BOOL		s_bReverseFilter;		// ReverseFilter 사용 여부
	int			m_EdgeFilterTh;			// 셀 Edge 영역에서 단주기 필터 적용할 수 없는 구간을 위한 추가 Th

	// 1.8. Dynamic Pitch 정보
	int			*m_pPitch16;	// pitch를 16배 해서 subpixel까지 표현, Trapezium이외의 Rect에서는 Vertical 방향이다.
	short		*m_pPitch;		// Dynamic Pitch. Horizontal 방향이고
	short		*m_pfSPR;		// Dynamic Pitch. Horizontal 방향이고, Sub Pixel
	short		*m_psSPR;		// Dynamic Pitch. Horizontal 방향이고, Sub Pixel
	BOOL		m_bUseDPC;
	int			m_PitchWidth;	// Pitch Table 이 2차원일 경우 Table width를 설정한다.(2차원 검사에 대한 flag 역할까지 겸함, 0일 경우 1차원)

	// 1.7. Zone Threshold 검사 정보
	BOOL		m_bZoneMulti;			// Zone Multi Threshold 검사 적용 선택.
	short*		s_psZoneTable;				// Zone별 Thresholding을 위한 Index Table
	short*		s_psZoneThTable;			// Zone별 Thresholding을 위한 Threshold Table
	short*		s_psZoneAddTable;			// Zone별 Thresholding을 위한 DynamicThreshold Table
	short*		s_psZoneMThTable;		// 2중 ZoneThreshold의 두번째 Base Threshold
	short*		s_psZoneMAddTable;		// 2중 ZoneThreshold의 두번째 Threshold Slope
	short*		s_psZoneMMinTable;			// 2중 ZoneThreshold의 2번째 직선을 위한 임계치(픽셀 밝기), 2중 ZoneThreshold적용 여부의 구분자 역할도 겸함.
	int			s_nThresTableWidth;		// Threshold Table 넓이
	int			s_nThresTableHeight;	// Threshold Table 높이
	int			s_nThresTableStartX;	// X축 시작 Offset
	int			s_nThresTableStartY;	// Y축 시작 Offset
	void*		m_pGammaTable;			// Zone Threshold 두번째 버전으로 매칭 테이블 정보 보관.
	int			m_nGammaXCount;			// Gamma Table의 X, Y 배열의 X축 매칭포이트 카운트(개수)
	int			s_nZoneCycleConv;		// Zone Reference 이미지의 기본주기 배수
	int			s_nZoneCycleScan;		// Zone Reference 이미지의 기본주기 배수
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
	// Assem 이 끝난 후처리와 관련된 정보.

	// 2.1.  결함 관리 정보.
	int			s_nFrameDefPixLimit;	// 한 Frame에서의 최대 결함 픽셀 수
	int			s_nFrameDefBlobLimit;	// 한 Frame에서의 최대 결함 수

	// 2.2. 픽셀 단계 필터링 정보.
	BOOL		m_bUsePixelFilter;		// Pixel단계에서 필터링을 할 것인가 말 것인가?
	BOOL		m_bOnlyJumpFilter;		// 검사 축의 Cross 방향으로 필터링 없이 JumpFiltering만 할 것인가 .or. CrossFilter를 할 것인가.
	BOOL		m_bPairFilter;			// Pair 결함에도 Cross&Jump 필터 적용.

	// 2.2 Zone Measure Filter // 특정 존에 대해 미세 영역 재 매칭후 다시 검사 필터 적용.
	BOOL		m_bTiltFilter;
 
	// 2.3. 후처리 정보.
	int			m_nDefectMergePixel;	// 디펙 머지 사이즈

	// 2.4. Zone Sorting, Thresholding
	BYTE*		s_pZoneData;			// Zone Sort Table
	BYTE*		s_pZoneThreshold;		// Zone Threshold Table
	int			s_nZoneDataWidth;		// Zone Sort Table 넓이
	int			s_nZoneBufferWidth;		// Zone Data 버퍼의 이미지 버퍼 크기.
	int			s_nZoneDataHeight;		// Zone Sort Table 높이

	// 2.5. 결함 정보 가공 정보.
	double		s_dScanResolution;		// Scan Resolution
	double		s_dConvResolution;		// Conv Resolution



	//======================================
	// 99. 안 쓰지만 개념상 지우지 않을 것들.
	short		s_sSkipSave;			// 검사 후 이미지 저장 여부
	UINT		s_uiProcessing;			// * 검사 방법
	int			s_nRegionType;			// 영역 - 설정된 값을 결함에 쓰기 위함

	// 안쓰이는데 누가 쓰는건지??? 아는 사람 신일섭차장에게 전화 하기.
	int			s_nUseSize;
	int			s_nSizeThres;

	// Add by PJH
	int			s_nASGDynamicHi;
	int			s_nASGDynamicLow;

	// 99. 기타 잡다구리
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
		// 1.1. 검사 방법 관련
		m_ConvMode					= ConvMode_Float;
		m_ConvDir					= 0;
		m_ReinspectCount			= 3;
		m_bZone2B2					= FALSE;

		// 1.2. 검사 이미지 정보
		s_lpBuffer					= NULL;
		s_nFrameWidth				= 0;
		s_nFrameHeight				= 0;

		// 1.3. 검사 영역 정보.
		s_RectConv.SetRect(0, 0, 0, 0);
		s_RectFilter= s_RectConv;

		// 1.5. Convolution 정보.
		s_nConvWidth				= 0;
		s_nConvHeight				= 0;

		s_dPitchConvReal			= 0.0;
		s_nPitchCycleConv			= 0;
		s_dPitchScanReal			= 0.0;
		s_nPitchCycleScan			= 0;

		m_fTilt						= 0.0;

		// 1.6. 문턱값 정보.
		s_nThreshold				= 10;
		s_nDThSlide					= 20;
		s_nDThMin					= 255;
		s_nThresholdSupress			= 255;
		s_nSideThreshold			= 0;
		s_nBWB_Main					= 0;
		s_nBWB_Slide				= 0;
		m_EchoThreshold				= 0;

		// 1.7. 검사 Process 정보.
		m_bEchoConvolution			= TRUE;
		m_bVertSpeedup				= TRUE;
		m_bSelectiveDir				= FALSE;
		m_bUseUnpair				= TRUE;
		s_bReverseFilter			= FALSE;
		m_EdgeFilterTh				= 0;

		// 1.8. Dynamic Pitch 정보
		m_pPitch16					= NULL;
		m_pPitch					= NULL;
		m_pfSPR						= NULL;
		m_psSPR						= NULL;
		m_bUseDPC					= TRUE;
		m_PitchWidth				= 0;// 0일 경우 1차원, width 값을 갖는 경우 2 차원 DPC


		// 1.9. Zone Threshold 검사 정보
		m_bZoneMulti				= FALSE;
		s_psZoneTable				= NULL;	//0617 추가
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
		// Assem 이 끝난 후처리와 관련된 정보.

		// 2.1.  결함 관리 정보.
		s_nFrameDefPixLimit			= 10000;//MAX_DEFECTPIX_NUM;
		s_nFrameDefBlobLimit		= 10000;//MAX_DEFECTBLOB_NUM;

		// 2.2. 필터링 정보.
		m_bUsePixelFilter			= TRUE;
		m_bOnlyJumpFilter			= FALSE;
		m_bPairFilter				= FALSE;

		// 2.2. Zone Measure Filter // 특정 존에 대해 미세 영역 재 매칭후 다시 검사 필터 적용.
		m_bTiltFilter				= FALSE;

		// 2.3. 후처리 정보.
		m_nDefectMergePixel			= 4;// 머지 4 픽셀

		// 2.4. Zone Sorting, Thresholding, Zone Thresholding일 때의 Zone 정보, RSCale 추출을 위한 값들
		s_pZoneData					= NULL;
		s_pZoneThreshold			= NULL;
		s_nZoneDataWidth			= 0;
		s_nZoneBufferWidth			= 0;
		s_nZoneDataHeight			= 0;

		// 2.5. 결함 정보 가공 정보.
		s_dScanResolution			= 0.0;
		s_dConvResolution			= 0.0;

		//======================================
		// 99. 쓰는지 안 쓰는 지 모르는 것.
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
