#pragma once

enum DefectKind {DK_BM, DK_PATTERN};
enum DefectType {DEFTYPE_BLACK = 0, DEFTYPE_WHITE, DEFTYPE_MIXED, DEFTYPE_NODEFECT, DEFTYPE_DELETE};
enum DefectPos	{DEFPOS_NONE = 0, DEFPOS_LEFT, DEFPOS_CENTER, DEFPOS_RIGHT, CRACKPOS_EDGELINE,CRACKPOS_SPOTLINE};
enum DefectPair	{DEFPAIR_NOTDEFINE = 0, DEFPAIR_PPAIR, DEFPAIR_MPAIR, DEFPAIR_SIDE, DEFPAIR_LARGE, DEFPAIR_UNPAIR };


typedef		int		DIT_RESULT;
#define		DIT_CONV_SUCCESS			0		// 정상 수행.
#define		DIT_CONV_CALLINGVIOLATION	1		// 함수 호출 절차가 잘못되었다.
#define		DIT_CONV_PARAMMODEFAIL		2
#define		DIT_CONV_INVALIDPARAM		3		// 올바르지 않은 함수 Parameter.
#define		DIT_CONV_FRAMEOVERFLOW		4		// 한 Frame에 결함이 10000 Pixel 이상이다.
#define		DIT_CONV_GLASSOVERFLOW		5		// 한 Thread가 Blob한 결함이 10000 개 이상이다.
#define		DIT_CONV_RSLT_RECT_SMALL	6		// 검사영역(Rect)가 피치 3배보다 작다.양방향모두
#define		DIT_CONV_USER				7		// Not_Defined...
#define		DIT_CONV_RESULT_COUNT		15		// 결과 종류의 개수, 항상 최대 수가 되어야 한다.

//////////////////////////////////////////////////////////////////////////
// Processing Option (선택된 항목에 따라 Processing 여부를 체크한다.)
#define		DIT_CONV_CONVOLUTION		1
#define		DIT_CONV_PPAIRING			2
#define		DIT_CONV_MPAIRING			4
#define		DIT_CONV_BLOBING			8
#define		DIT_CONV_CLASSIFY			16
#define		DIT_CONV_MERGE_DIVISION		32
#define		DIT_CONV_MERGE_MIX			64
#define		DIT_CONV_UNPAIRBLOB			128

#define		DIT_CONV_ALL_SKIPMERGE		31
#define		DIT_CONV_ALL_MERGEDIVISION	63
#define		DIT_CONV_ALL_MERGEMIX		95

//////////////////////////////////////////////////////////////////////////
// 최대 허용 결함 수.
#define		MAX_DEFECTPIX_NUM			10000
#define		MAX_DEFECTBLOB_NUM			10000
#define		MAX_ZONE_NUM				16

#define		VERT_CELLCENTER			0
#define		VERT_CELLSTART			1
#define		VERT_CELLEND			2




class AFX_EXT_CLASS CConvResult
{
public:
	int			m_nThreadDefectPixelNum;
	int			m_nThreadDefectNum;
	int			m_nSkipUnpaired;
	int			m_nSkipClassify;
	int			m_nMoveClassify;
	int			m_nSkipMerged;

	CConvResult()
	{
		Reset();
	}
	void Reset()
	{
		m_nThreadDefectPixelNum	= 0;
		m_nThreadDefectNum		= 0;
		m_nSkipUnpaired			= 0;
		m_nSkipClassify			= 0;
		m_nMoveClassify			= 0;
		m_nSkipMerged			= 0;
	}
};

class AFX_EXT_CLASS CDefectPair
{
public:
	DefectPos	s_DefectPos;
	DefectPair	s_DefectPair;
	DefectType	s_DefectType;
	int			s_fDefectPeak;
	int			s_nDefectX;
	int			s_nDefectY;
	int			s_nGraySrc;
	int			s_nGrayRef;
	int			s_nZone;
	int			s_nThresold;

	CDefectPair()
	{
		Reset();
	}
	void Reset()
	{
		s_DefectPos = DEFPOS_NONE;
		s_DefectPair = DEFPAIR_NOTDEFINE;
		s_DefectType = DEFTYPE_NODEFECT;
		s_fDefectPeak = 0;
		s_nDefectX = 0;
		s_nDefectY = 0;
		s_nGraySrc = 0;
		s_nGrayRef = 0;
		s_nZone= 0;
		s_nThresold= 0;
	}
	void SetPeak(int nPeak, int nKernelThres, int nCalcPixel)
	{
		s_fDefectPeak= (abs(nPeak));
		s_fDefectPeak -= (nKernelThres);
		s_fDefectPeak /= (nCalcPixel);
	}

	// 복사 생성자.
	CDefectPair(const CDefectPair& rhs)
	{
		if(this != &rhs)
		{
			s_DefectPos			= rhs.s_DefectPos;
			s_DefectPair		= rhs.s_DefectPair;
			s_DefectType		= rhs.s_DefectType;
			s_fDefectPeak		= rhs.s_fDefectPeak;
			s_nDefectX			= rhs.s_nDefectX;
			s_nDefectY			= rhs.s_nDefectY;
			s_nGraySrc			= rhs.s_nGraySrc;
			s_nGrayRef			= rhs.s_nGrayRef;
			s_nZone				= rhs.s_nZone;
			s_nThresold			= rhs.s_nThresold;
		}
	}

	// 대입연산자 오버로딩.
	CDefectPair& operator=(const CDefectPair& rhs)
	{
		if(this != &rhs)
		{
			s_DefectPos			= rhs.s_DefectPos;
			s_DefectPair		= rhs.s_DefectPair;
			s_DefectType		= rhs.s_DefectType;
			s_fDefectPeak		= rhs.s_fDefectPeak;
			s_nDefectX			= rhs.s_nDefectX;
			s_nDefectY			= rhs.s_nDefectY;
			s_nGraySrc			= rhs.s_nGraySrc;
			s_nGrayRef			= rhs.s_nGrayRef;
			s_nZone				= rhs.s_nZone;
			s_nThresold			= rhs.s_nThresold;
		}
		return *this;
	}

};

class AFX_EXT_CLASS CDefectBlob
{
public:
	DefectPos	s_DefectPos;
	DefectPair	s_DefectPair;
	DefectType	s_DefectType;
	int			s_nIndex;
	int			s_nDefectArea;
	int			s_nPadRegionIdx;
	int			s_nDefectX;
	int			s_nDefectY;
	int			s_xMaxPeak;
	int			s_yMaxPeak;
	int			s_xLevelSrcMax;
	int			s_yLevelSrcMax;
	int			s_nDefectRScale;
	CRect		s_DefectRect;
	short		s_sDefectPeak;

	short		s_sLevelSrcMin;
	short		s_sLevelSrcMax;
	short		s_sLevelSrcAvg;
	short		s_sLevelRefMin;
	short		s_sLevelRefMax;
	short		s_sLevelRefAvg;
	short		s_sLevelDiffMin;
	short		s_sLevelDiffMax;
	short		s_sLevelDiffAvg;
	int			s_nLevelSrcSum;
	int			s_nLevelRefSum;
	int			s_nLevelDiffSum;
	//무게중심의 Gray 정보 -윤정민-
	short		s_sCMSrcMin;
	short		s_sCMSrcMax;
	short		s_sCMSrcAvg;
	short		s_sCMRefMin;
	short		s_sCMRefMax;
	short		s_sCMRefAvg;

	short		s_sThreshold;

	short		s_sZonePixelCount[MAX_ZONE_NUM];
	short		s_sZoneValueMin[MAX_ZONE_NUM];
	BYTE		s_sZoneValueMax[MAX_ZONE_NUM];
	int			s_sZoneValueSum[MAX_ZONE_NUM];

	int			s_nRegionType;
	BOOL		s_bRemoved;


	CPoint		s_ptVertex[8];

	CDefectBlob()
	{
		Reset();
	}
	void ResetReuse()
	{
		s_DefectRect.SetRect(INT_MAX, INT_MAX, 0, 0);
		for (int i = 0; i < MAX_ZONE_NUM; i++)
		{
			s_sZonePixelCount[i] = 0;
			s_sZoneValueMin[i] = 256;
			s_sZoneValueMax[i] = 0;
			s_sZoneValueSum[i] = 0;
		}
		s_ptVertex[0]		= MAKELONG(32760, 32760);
		s_ptVertex[1]		= MAKELONG(32760, 0);
		s_ptVertex[2]		= MAKELONG(32760, 32760);
		s_ptVertex[3]		= MAKELONG(0, 32760);
		s_ptVertex[4]		= MAKELONG(0, 32760);
		s_ptVertex[5]		= MAKELONG(0, 0);
		s_ptVertex[6]		= MAKELONG(32760, 0);
		s_ptVertex[7]		= MAKELONG(0, 0);

		s_bRemoved			= FALSE;
	}
	void Reset()
	{
		s_DefectPos			= DEFPOS_NONE;
		s_DefectPair		= DEFPAIR_NOTDEFINE;
		s_DefectType		= DEFTYPE_NODEFECT;
		s_nIndex			= 0;
		s_nDefectArea		= 0;
		s_nPadRegionIdx		= -1;
		s_nDefectX			= 0;
		s_nDefectY			= 0;
		s_nDefectRScale		= -1;
		// 20080525 이준희
		s_DefectRect.SetRect(INT_MAX, INT_MAX, 0, 0);
		s_sDefectPeak		= 0;

		s_sLevelSrcMin		= 2048;
		s_xMaxPeak			= -1;
		s_yMaxPeak			= -1;
		s_sLevelSrcMax		= 0;
		s_xLevelSrcMax		= 0;
		s_yLevelSrcMax		= 0;
		s_sLevelSrcAvg		= 0;
		s_sLevelRefMin		= 2048;
		s_sLevelRefMax		= 0;
		s_sLevelRefAvg		= 0;
		s_sLevelDiffMin		= 2048;
		s_sLevelDiffMax		= 0;
		s_sLevelDiffAvg		= 0;
		s_nLevelSrcSum		= 0;
		s_nLevelRefSum		= 0;
		s_nLevelDiffSum		= 0;

		s_sThreshold		= 0;

		for (int i = 0; i < MAX_ZONE_NUM; i++)
		{
			s_sZonePixelCount[i] = 0;
			s_sZoneValueMin[i] = 256;
			s_sZoneValueMax[i] = 0;
			s_sZoneValueSum[i] = 0;
		}

		s_nRegionType		= -1;

		// 20080525 이준희
		s_ptVertex[0]		= MAKELONG(32760, 32760);
		s_ptVertex[1]		= MAKELONG(32760, 0);
		s_ptVertex[2]		= MAKELONG(32760, 32760);
		s_ptVertex[3]		= MAKELONG(0, 32760);
		s_ptVertex[4]		= MAKELONG(0, 32760);
		s_ptVertex[5]		= MAKELONG(0, 0);
		s_ptVertex[6]		= MAKELONG(32760, 0);
		s_ptVertex[7]		= MAKELONG(0, 0);

		s_bRemoved			= FALSE;
	}
	void SetDefectPos(DefectPos	DP)
	{
		// 2012.1.26 : Center 결함 우선. 
		if (s_DefectPos == DEFPOS_NONE)
			s_DefectPos = DP;
		else if (s_DefectPos == DEFPOS_CENTER)//DP == DEFPOS_LEFT || DP == DEFPOS_RIGHT)
			return;
		s_DefectPos = DP;
	}

	void SetDefectPair(DefectPair DP)
	{
		// 2012.1.26 : Pair 결함 우선으로 수정. mosis
		if (s_DefectPair > DP)
			s_DefectPair = DP;
	}


	// 대입연산자 오버로딩.
	CDefectBlob& operator=(const CDefectBlob& rhs)
	{
		if(this != &rhs)
		{
			s_DefectPos			= rhs.s_DefectPos;
			s_DefectPair		= rhs.s_DefectPair;
			s_DefectType		= rhs.s_DefectType;
			s_nIndex			= rhs.s_nIndex;
			s_nDefectArea		= rhs.s_nDefectArea;
			s_nPadRegionIdx		= rhs.s_nPadRegionIdx;
			s_nDefectX			= rhs.s_nDefectX;
			s_nDefectY			= rhs.s_nDefectY;
			s_nDefectRScale		= rhs.s_nDefectRScale;
			s_DefectRect		= rhs.s_DefectRect;
			s_sDefectPeak		= rhs.s_sDefectPeak;

			s_sLevelSrcMin		= rhs.s_sLevelSrcMin;
			s_sLevelSrcMax		= rhs.s_sLevelSrcMax;
			s_xMaxPeak			= rhs.s_xMaxPeak;
			s_yMaxPeak			= rhs.s_yMaxPeak;
			s_xLevelSrcMax		= rhs.s_xLevelSrcMax;
			s_yLevelSrcMax		= rhs.s_yLevelSrcMax;
			s_sLevelSrcAvg		= rhs.s_sLevelSrcAvg;
			s_sLevelRefMin		= rhs.s_sLevelRefMin;
			s_sLevelRefMax		= rhs.s_sLevelRefMax;
			s_sLevelRefAvg		= rhs.s_sLevelRefAvg;
			s_sLevelDiffMin		= rhs.s_sLevelDiffMin;
			s_sLevelDiffMax		= rhs.s_sLevelDiffMax;
			s_sLevelDiffAvg		= rhs.s_sLevelDiffAvg;
			s_nLevelSrcSum		= rhs.s_nLevelSrcSum;
			s_nLevelRefSum		= rhs.s_nLevelRefSum;
			s_nLevelDiffSum		= rhs.s_nLevelDiffSum;

			s_sThreshold		= rhs.s_sThreshold;

			int i;
#define MOSIS_EDIT_VERTEX
			for(i= 0; i< 8; i++)
			{
				s_ptVertex[i]= rhs.s_ptVertex[i];
			}

			for (i = 0; i < MAX_ZONE_NUM; i++)
			{
				s_sZonePixelCount[i] = rhs.s_sZonePixelCount[i];
				s_sZoneValueMin[i] = rhs.s_sZoneValueMin[i];
				s_sZoneValueMax[i] = rhs.s_sZoneValueMax[i];
				s_sZoneValueSum[i] = rhs.s_sZoneValueSum[i];
			}
			s_nRegionType		= rhs.s_nRegionType;
			s_bRemoved			= rhs.s_bRemoved;
		}
		return *this;
	}

	void MergeBlob(const CDefectBlob& rhs)
	{
		if (s_DefectPos == DEFPOS_NONE)
			s_DefectPos = rhs.s_DefectPos;
		else if (rhs.s_DefectPos == DEFPOS_LEFT || rhs.s_DefectPos == DEFPOS_RIGHT)
			s_DefectPos = rhs.s_DefectPos;

		if (s_DefectPair < rhs.s_DefectPair)
			s_DefectPair = rhs.s_DefectPair;

		s_nDefectArea += rhs.s_nDefectArea;
		s_nDefectX = (s_DefectRect.right + s_DefectRect.left) / 2;
		s_nDefectY = (s_DefectRect.bottom + s_DefectRect.top) / 2;

		if (s_ptVertex[0].x >= rhs.s_ptVertex[0].x)	// LT
			s_ptVertex[0] = rhs.s_ptVertex[0];
		if (s_ptVertex[1].x >= rhs.s_ptVertex[1].x)	// LB
			s_ptVertex[1] = rhs.s_ptVertex[1];
		if (s_ptVertex[2].y >= rhs.s_ptVertex[2].y)	// TL
			s_ptVertex[2] = rhs.s_ptVertex[2];
		if (s_ptVertex[3].y >= rhs.s_ptVertex[3].y)	// TR
			s_ptVertex[3] = rhs.s_ptVertex[3];
		if (s_ptVertex[4].x <= rhs.s_ptVertex[4].x)	// RT
			s_ptVertex[4] = rhs.s_ptVertex[4];
		if (s_ptVertex[5].x <= rhs.s_ptVertex[5].x)	// RB
			s_ptVertex[5] = rhs.s_ptVertex[5];
		if (s_ptVertex[6].y <= rhs.s_ptVertex[6].y)	// BL
			s_ptVertex[6] = rhs.s_ptVertex[6];
		if (s_ptVertex[7].y <= rhs.s_ptVertex[7].y)	// BR
			s_ptVertex[7] = rhs.s_ptVertex[7];

		s_nDefectRScale = (s_nDefectRScale + rhs.s_nDefectRScale);
		if (s_DefectRect.left > rhs.s_DefectRect.left)
			s_DefectRect.left = rhs.s_DefectRect.left;
		if (s_DefectRect.top > rhs.s_DefectRect.top)
			s_DefectRect.top = rhs.s_DefectRect.top;
		if (s_DefectRect.right < rhs.s_DefectRect.right)
			s_DefectRect.right = rhs.s_DefectRect.right;
		if (s_DefectRect.bottom < rhs.s_DefectRect.bottom)
			s_DefectRect.bottom = rhs.s_DefectRect.bottom;
		if (s_sDefectPeak < rhs.s_sDefectPeak)
		{
			s_sDefectPeak		= rhs.s_sDefectPeak;
			s_xMaxPeak			= rhs.s_xMaxPeak;
			s_yMaxPeak			= rhs.s_yMaxPeak;
		}

		if (s_sLevelSrcMin > rhs.s_sLevelSrcMin)
			s_sLevelSrcMin		= rhs.s_sLevelSrcMin;
		if (s_sLevelSrcMax < rhs.s_sLevelSrcMax)
		{
			s_sLevelSrcMax		= rhs.s_sLevelSrcMax;
			s_xLevelSrcMax		= rhs.s_xLevelSrcMax;
			s_yLevelSrcMax		= rhs.s_yLevelSrcMax;
		}
		s_sLevelSrcAvg			= (s_sLevelSrcAvg + rhs.s_sLevelSrcAvg) / 2;
		if (s_sLevelRefMin > rhs.s_sLevelRefMin)
			s_sLevelRefMin		= rhs.s_sLevelRefMin;
		if (s_sLevelRefMax < rhs.s_sLevelRefMax)
			s_sLevelRefMax		= rhs.s_sLevelRefMax;
		s_sLevelRefAvg			= (s_sLevelRefAvg + rhs.s_sLevelRefAvg) / 2;
		if (s_sLevelDiffMin > rhs.s_sLevelDiffMin)
			s_sLevelDiffMin		= rhs.s_sLevelDiffMin;
		if (s_sLevelDiffMax < rhs.s_sLevelDiffMax)
			s_sLevelDiffMax		= rhs.s_sLevelDiffMax;
		s_sLevelDiffAvg			= (s_sLevelDiffAvg + rhs.s_sLevelDiffAvg) / 2;
		s_nLevelSrcSum		+= rhs.s_nLevelSrcSum;
		s_nLevelRefSum		+= rhs.s_nLevelRefSum;
		s_nLevelDiffSum		+= rhs.s_nLevelDiffSum;

		if (s_sThreshold < rhs.s_sThreshold)
			s_sThreshold		= rhs.s_sThreshold;

		for (int i = 0; i < MAX_ZONE_NUM; i++)
		{
			s_sZonePixelCount[i] = s_sZonePixelCount[i] + rhs.s_sZonePixelCount[i];
			s_sZoneValueMin[i] = min(s_sZoneValueMin[i],rhs.s_sZoneValueMin[i]);
			s_sZoneValueMax[i] = max(s_sZoneValueMax[i],rhs.s_sZoneValueMax[i]);
			s_sZoneValueSum[i] = (s_sZoneValueSum[i]+rhs.s_sZoneValueSum[i]);
		}
	}
	void CreateBlob(CDefectPair *pPair, int id);
	void IncludePair(CDefectPair *pPair);
	void CheckDefectRect(int nX, int nY);
};

