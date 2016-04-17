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

	void OffsetBig(CRect &rect)		{	m_X += rect.left;	m_Y	+= rect.top;}	// BigBuffer ��ǥ�� ��ȯ.
	void OffsetSmall(CRect &rect)	{	m_X -= rect.left;	m_Y -= rect.top;}	// SmallBuffer�� ������ ��Ī�� ���� ã���� �Ѵ�.
	void Offset(double x, double y)	{	m_X+= x;	m_Y+= y;}

	void CenterOrigin(CRect &rect)
	{// width, height �� Ȧ�� �� ���.. 0.5�� �߽������� ��� �� ���.
		double dx= (double)rect.Width()/2;
		double dy= (double)rect.Height()/2;
		Offset(-dx, -dy);
	}
}CMatchPt;


typedef struct stMosisMatchParam
{

	// ���ø� �޼ҵ�.
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
	CMosisBuffer	m_BigBuffer, m_SmallBuffer;// ���� ����(�븮����)

	CRect m_BigRect, m_SmallRect;// ��Ī roi.
	stMosisMatchParam m_MatchParam;// ��Ī �Ķ����.
	class CMatchImpl *m_pMatchImpl;

/*
	struct _IplImage *m_pBigImage, *m_pSmallImage, *m_pResultImage;// �Ƕ�̵� ��Ī�� �̹���.
 	struct _IplImage *m_pBigImage2, *m_pSmallImage2, *m_pResultImage2;// �� ��Ī�� �̹���.
*/
public:
	CMosisMatch();
	virtual ~CMosisMatch();

public:
	BOOL DoTemplateMatch(stMosisMatchResult &result, stMosisMatchParam &matchParam);
private:
	BOOL DoTemplateMatch2(stMosisMatchResult &result);
	
public:
	BOOL SetBigImage(CMosisBuffer buffer, CRect &roi);// ������ ã�� ���� �̹���.
	BOOL SetSmallImage(CMosisBuffer buffer, CRect &roi);// ��Ī�� ���ø� �̹���.
	BOOL SetBigImage2(CMosisBuffer buffer, CRect &roi);// ������ ã�� ���� �̹���.
	BOOL SetSmallImage2(CMosisBuffer buffer, CRect &roi);// ��Ī�� ���ø� �̹���.
public:
	void NormalizeMatchRect(CRect &rect);// ��Ī�� �°� ����,���� ����� 4byte ����� ����� �����.
	BOOL SetImage32f(struct _IplImage **pImage, CMosisBuffer *buffer, CRect &roi);// �̹����� 32bit float������ summation���۷� �����Ѵ�.
	BOOL MakeImage(struct _IplImage **pImage, int width, int height);
	BOOL SetImage(struct _IplImage **pImage, CMosisBuffer *buffer, CRect &roi);// �̹����� 8bit(1byte) �̹��� �״�� ��Ī�� �°� �����.
	
};

#endif // !defined(AFX_MOSISMATCH_H__CB8CE1CD_6F94_4C2E_858D_8182CEF9A453__INCLUDED_)
