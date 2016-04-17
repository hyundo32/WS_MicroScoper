// NoiseLevel.h: interface for the CNoiseLevel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOISELEVEL_H__5D3B57CE_14F1_4CD6_8D25_25471A55B874__INCLUDED_)
#define AFX_NOISELEVEL_H__5D3B57CE_14F1_4CD6_8D25_25471A55B874__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMosisBuffer;
typedef unsigned char BYTE;

// 1. FindXNoiseLevel 계열로 Noise를 데이터 확보.
// 2. RecommendThreshold(1, 2, 3) 3가지 방법으로 Threshold 도출.


class AFX_EXT_CLASS CNoiseLevel  
{
public:
	int m_TotalCount;
	int	m_OverCount;
	double	m_Ratio;
	int	m_LimitCount;
	int m_pnNoisePixel[256];
	int m_FoundThreshold;
	int	m_Width, m_Height;

	void ResetCount()
	{
		for(int i= 0; i< 256; i++)
		{
			m_pnNoisePixel[i]= 0;
		}
		m_TotalCount= 0;
		m_FoundThreshold= -1;
	}
	int GetNoiseCount(int level)
	{
		if(level < 0)
			level= -level;
		ASSERT(level < 256);
		return m_pnNoisePixel[level];
	}
	int CountNoise(int level)
	{
		if(level < 0)
			level= -level;
		ASSERT(level < 256);
		m_pnNoisePixel[level]++;
		m_TotalCount++;
		return m_pnNoisePixel[level];
	}


public:
	// 1. 노이즈가 큰 것부터 탐색해 limitCount 보다 높은 점이 발생하면 Threshold로 추천.. 깨끗한 이미지에 적합.
	int RecommendThreshold1(double fNoiseLimit= 0.001)
	{
		m_LimitCount= (int)(m_TotalCount*fNoiseLimit);
		if(m_LimitCount < 1)
			m_LimitCount= 1;
		return RecommendThreshold(m_LimitCount);
	}
	int RecommendThreshold(int nLimitCount)
	{
		m_OverCount= 0;
		for(int i= 255; i>= 0; i--)
		{
			m_OverCount+= m_pnNoisePixel[i];
			if(m_pnNoisePixel[i] >= nLimitCount)
			{
				m_OverCount-= m_pnNoisePixel[i];
				m_FoundThreshold= i;
				return m_FoundThreshold;
			}
		}
		m_FoundThreshold= 0;
		return 0;
	}

	// 2. 노이즈가 작은 것부터 시작해 limitcount이하로 떨어지기 시작하는 점을 찾는다... 노이즈 많은 이미지에 적합.
	int RecommendThreshold2(double fNoiseLimit= 0.001)
	{
		m_OverCount= m_TotalCount;
		m_LimitCount= (int)(m_TotalCount*fNoiseLimit);
		if(m_LimitCount < 1)
			m_LimitCount= 1;
		for(int i= 0; i< 256; i++)
		{
			m_OverCount-= m_pnNoisePixel[i];
			if(m_pnNoisePixel[i] <= m_LimitCount)
			{
				m_FoundThreshold= i;
				return i;
			}
		}
		m_FoundThreshold= 0;
		return 0;
	}

	// 3. 노이즈가 높은 쪽에서 부터 누적적으루다가 계산해서 limitcount를 넘는 놈을 Threshold로 추천. 안전빵에 적합.
	int RecommendThreshold3(double fNoiseLimit= 0.001)
	{
		m_LimitCount= (int)(m_TotalCount*fNoiseLimit);
		if(m_LimitCount < 1)
			m_LimitCount= 1;
		return RecommendThresholdAccumulative(m_LimitCount);
	}
	// Threshold를 낮춰가면 nNoiseLimet 개수 이상의 결함 픽셀이 발생하면 그 선을 Threshold로 한다.
	int RecommendThresholdAccumulative(int nLimitCount)
	{
		int nCount= 0;
		for(int i= 255; i>= 0; i--)
		{
			nCount+= m_pnNoisePixel[i];
			if(nCount >= nLimitCount)
			{
				m_OverCount= nCount-  m_pnNoisePixel[i];
				m_FoundThreshold= i;
				return i;
			}
		}
		m_FoundThreshold= 0;
		return 0;
	}

public:
	CNoiseLevel()			{ResetCount();}

	BOOL FindXNoiseLevel(CMosisBuffer &buffer, double xPitch, BOOL b32= TRUE);
	BOOL FindYNoiseLevel(CMosisBuffer &buffer, double yPitch, BOOL b23= TRUE);

	BOOL DrawNoisePixel2Left(CMosisBuffer &buffer, CMosisBuffer &buffer2, double pitch, int threshold, BYTE color, BOOL b32= TRUE);
	BOOL DrawNoisePixel2Right(CMosisBuffer &buffer, CMosisBuffer &buffer2, double pitch, int threshold, BYTE color, BOOL b32= TRUE);
	BOOL DrawNoisePixel2Top(CMosisBuffer &buffer, CMosisBuffer &buffer2, double pitch, int threshold, BYTE color, BOOL b23= TRUE);
	BOOL DrawNoisePixel2Bottom(CMosisBuffer &buffer, CMosisBuffer &buffer2, double pitch, int threshold, BYTE color, BOOL b23= TRUE);
	void DrawNoiseLevel(CDC *pDC);

	//////////////////////////////////////////////////////////////////////////
	// Focus Value
	int	CalFocusValue(CMosisBuffer &buffer);

};

#endif // !defined(AFX_NOISELEVEL_H__5D3B57CE_14F1_4CD6_8D25_25471A55B874__INCLUDED_)
