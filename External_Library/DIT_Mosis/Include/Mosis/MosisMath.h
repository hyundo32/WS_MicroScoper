// MosisMath.h: interface for the CMosisMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOSISMATH_H__00204741_878C_42AB_8757_3EE86C292EFD__INCLUDED_)
#define AFX_MOSISMATH_H__00204741_878C_42AB_8757_3EE86C292EFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MosisBuffer.h"

typedef __int64 int64;
typedef unsigned char uchar;
typedef unsigned short ushort;


/*

// defect count ������� cutoff point �� ã�� �ش�.
// AddCountData�� ���� ����� FindCutoff_Continue�� ���� ��� �ΰ����� �ִ�.
class AFX_EXT_CLASS CCutoffFind
{
	int m_nContinuousCutoff;
	int *m_pCountData;
public:
	int m_nCountData;
	CCutoffFind()
	{
		m_nCountData= 0;
		m_pCountData= NULL;
	}
	~CCutoffFind()
	{
		if(m_pCountData)
			delete m_pCountData;
	}
	BOOL SetSize(int nData)
	{
		m_nCountData= nData;
		if(m_pCountData)
			delete m_pCountData;
		m_pCountData= new int[nData];
		return TRUE;
	}
	
	// ���������� ȣ��Ǹ� cutoff������ �Ǻ��� ���� cutoff count�� ������ �ش�.
	int AddCountData(int data, int nCutoff)
	{
		if(data < nCutoff)
		{
			m_nContinuousCutoff++;
			return m_nContinuousCutoff;
		}
		m_nContinuousCutoff= 0;
		return 0;
	}
	
	
	void SetCountData(int iData, int data)
	{
		m_pCountData[iData]= data;
	}
	// ��,���������� line by line �˻��ϸ� Cutoff_Continuous�� ã�� ����.  Cutoff_Continuous�� ������ false, Cutoff ��ġ�� ������ ������ index�̴�.
	// nContinue���� �̻� ���������� cutoff�� ���ܾ� Cutoff_Continuous�� �����Ѵ�.
	BOOL FindCutoff_Continuous(BOOL bForwardScan, int nCutoff, int nContinue, int &iForwardResult)
	{
		int i;
		int nOccur= 0;
		
		if(bForwardScan)
		{
			for(i= 0; i< m_nCountData; i++)
			{
				if(m_pCountData[i] > nCutoff)
				{
					nOccur++;
					if(nOccur >= nContinue)
					{
						iForwardResult= i- nContinue+ 1;
						return TRUE;
					}
				}else
				{
					nOccur= 0;
				}
			}
			return FALSE;
		}
		
		for(i= m_nCountData-1; i >= 0; i--)
		{
			if(m_pCountData[i] > nCutoff)
			{
				nOccur++;
				if(nOccur >= nContinue)
				{
					iForwardResult= i+ nContinue- 1;
					return TRUE;
				}else
				{
					nOccur= 0;
				}
			}
		}
		return FALSE;
	}
};
*/

// 1. Sum of Absolute Differences
// 2. Cross-Correlation Values.
// 3. Minimum or Maximum on a certain curve.
class AFX_EXT_CLASS CMosisMath
{
public:

	// 1. Get Sum of Absolute Differences on the specific area(Rect).
	static INT64	GetAbsDiff(CMosisBuffer buffer, CRect rect, CMosisBuffer buffer2, int x2, int y2)
	{
		return GetAbsDiff(buffer, rect.left, rect.top, rect.Width(), rect.Height(), buffer2, x2, y2);
	}
	static INT64	GetAbsDiff(CMosisBuffer buffer, int x1, int y1, int width, int height, CMosisBuffer buffer2, int x2, int y2);
	static INT64	GetAbsDiff(BYTE* pData, BYTE* pData2, int len);

	// 2. Get Cross Correlation Value on the specific area(Rect).
	static int64	Get_CCU8( CMosisBuffer buffer1, int x1, int y1, int width, int height, CMosisBuffer buffer2, int x2, int y2);
	static int64	Get_CCU8_A( const uchar * vec1, int len );
	static int64	Get_CCU8_AUA( const uchar * vec1, const uchar * vec2, int len );
	static int64	Get_CCU8_UA( const uchar * vec1, const uchar * vec2, int len );
	static int64	Get_CCU8_UA( const uchar * vec1, int len );
	static int64	Get_CCU16_A( const ushort * vec1, int len );
	static int64	Get_CCU16_AUA( const ushort * vec1, const ushort * vec2, int len );
	static int64	Get_CCU16_UA( const ushort * vec1, const ushort * vec2, int len );
	static int64	Get_CCU16_UA( const ushort * vec1, int len );

	static int64	SumPixels_8u( const uchar * vec, int len );
	static int64	SumPixels_16u( const ushort * vec, int len );


	// 3. y= -a(x+ b)^2+ c => �ּҰ��� ������ ���������� �ּҰ��� ã�´�.
	static double	SearchMinimum(int64 *pData, int nData);
	// 3. y= a(x+ b)^2+ c => �ִ밪�� ������ ���������� �ִ밪�� ã�´�.
	static double	SearchMaximum(int64 *pData, int nData);

	// 4. 
	static double	GetAverage(CMosisBuffer buffer);
	static INT64	GetSum(CMosisBuffer buffer);

	// 5.
//	static void		CountLineDefect(CMosisBuffer bufer, double xPitch, double yPitch, CCutoffFind &find);

public: // useless
	static int64	Get_CCU16_A( const ushort * vec1, const ushort * vec2, int len );
	static int64	Get_CCU8_A( const uchar * vec1, const uchar * vec2, int len );

public: // for omp
	static int64	OMP_CCU8_A(const uchar * vec1, int len);
	static int64	OMP_CCU8_UA(const uchar *vec1, int len);
	static int64	OMP_CCU8_AUA( const uchar * vec1, const uchar * vec2, int len );

	
	static int64	OMP_CCU16_A( const ushort * vec1, int len );
	static int64	OMP_CCU16_UA( const ushort * vec1, int len );
	static int64	OMP_CCU16_AUA( const ushort * vec1, const ushort * vec2, int len );
};

#endif // !defined(AFX_MOSISMATH_H__00204741_878C_42AB_8757_3EE86C292EFD__INCLUDED_)
