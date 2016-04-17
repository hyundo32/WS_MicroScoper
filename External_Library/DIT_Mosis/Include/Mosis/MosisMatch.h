// MosisMatch.h: interface for the CMosisMatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOSISMATCH_H__CB8CE1CD_6F94_4C2E_858D_8182CEF9A453__INCLUDED_)
#define AFX_MOSISMATCH_H__CB8CE1CD_6F94_4C2E_858D_8182CEF9A453__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InspectionBuffer.h"



typedef struct stMosisMatchResult
{
	double m_X, m_Y, m_Score;
	stMosisMatchResult()	{	Invalidate();	}
	void Invalidate()		{ m_X= 0, m_Y= 0; m_Score= 0;}
	BOOL IsValid()			{	return m_Score != 0;	}
	BOOL MatchBetterThan(stMosisMatchResult &result)	{	return m_Score > result.m_Score;}

	void OffsetBig(CRect &rect)		{	m_X += rect.left;	m_Y	+= rect.top;}	// BigBuffer 좌표로 변환.
	void OffsetSmall(CRect &rect)	{	m_X -= rect.left;	m_Y -= rect.top;}	// SmallBuffer의 원점이 매칭된 점을 찾고자 한다.
	void Offset(double x, double y)	{	m_X+= x;	m_Y+= y;}

	void CenterOrigin(CRect &rect)
	{// width, height 가 홀수 일 경우.. 0.5가 중심점임을 명심 또 명심.
		double dx= (double)rect.Width()/2;
		double dy= (double)rect.Height()/2;
		Offset(-dx, -dy);
	}
}CMatchPt;


typedef struct stMosisMatchParam
{

	// 템플릿 메소드.
	enum{CV_TM_SQDIFF= 0, CV_TM_SQDIFF_NORMED= 1, CV_TM_CCORR= 2, CV_TM_CCORR_NORMED= 3, CV_TM_CCOEFF= 4, CV_TM_CCOEFF_NORMED= 5};

	int			m_Method;// 0, 1, 2, 3, 4, 5
	int			m_Pyramid;
	double		m_Acceptance;// 0~ 1
	BOOL		m_bInterpol;

	stMosisMatchParam(int pyramid= 2, double acceptance= 0.3, BOOL bUseInterpolate= TRUE, int method= CV_TM_CCOEFF_NORMED)
		: m_Pyramid(pyramid), m_Acceptance(acceptance), m_bInterpol(bUseInterpolate),  m_Method(method)
	{	}
	int			GetMulti(){return 1<<m_Pyramid;}
	void		ValidateParam()
	{
		if(m_Pyramid < 0)
			m_Pyramid= 0;
		if(m_Pyramid > 4)
			m_Pyramid= 4;

		if(m_Acceptance > 0.99)
			m_Acceptance= 0.99;
	}
}CMatchParam;


class AFX_EXT_CLASS CMosisMatch  
{
	CMosisBuffer	m_BigBuffer, m_SmallBuffer;// 원본 버퍼(대리버퍼)

	CRect m_BigRect, m_SmallRect;// 매칭 roi.
	stMosisMatchParam m_MatchParam;// 매칭 파라미터.
	class CMatchImpl *m_pMatchImpl;

/*
	struct _IplImage *m_pBigImage, *m_pSmallImage, *m_pResultImage;// 피라미드 매칭용 이미지.
 	struct _IplImage *m_pBigImage2, *m_pSmallImage2, *m_pResultImage2;// 상세 매칭용 이미지.
*/
public:
	CMosisMatch();
	virtual ~CMosisMatch();

public:
	BOOL DoTemplateMatch(stMosisMatchResult &result, stMosisMatchParam &matchParam);
private:
	BOOL DoTemplateMatch2(stMosisMatchResult &result);
	
public:
	BOOL SetBigImage(CMosisBuffer buffer, CRect &roi);// 패턴을 찾을 패턴 이미지.
	BOOL SetSmallImage(CMosisBuffer buffer, CRect &roi);// 매칭용 템플릿 이미지.
	BOOL SetBigImage2(CMosisBuffer buffer, CRect &roi);// 패턴을 찾을 패턴 이미지.
	BOOL SetSmallImage2(CMosisBuffer buffer, CRect &roi);// 매칭용 템플릿 이미지.
public:
	void NormalizeMatchRect(CRect &rect);// 매칭에 맞게 수직,수평 양방향 4byte 얼라인 사이즈를 맞춘다.
	BOOL SetImage32f(struct _IplImage **pImage, CMosisBuffer *buffer, CRect &roi);// 이미지를 32bit float형태의 summation버퍼로 생성한다.
	BOOL MakeImage(struct _IplImage **pImage, int width, int height);
	BOOL SetImage(struct _IplImage **pImage, CMosisBuffer *buffer, CRect &roi);// 이미지를 8bit(1byte) 이미지 그대로 매칭에 맞게 만든다.
	
};

#endif // !defined(AFX_MOSISMATCH_H__CB8CE1CD_6F94_4C2E_858D_8182CEF9A453__INCLUDED_)
