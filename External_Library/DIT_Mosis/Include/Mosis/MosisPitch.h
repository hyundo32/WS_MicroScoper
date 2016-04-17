// MosisPitch.h: interface for the CMosisPitch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOSISPITCH_H__CA80B4A7_F0BD_4AEA_A81B_CF86C09EC7C1__INCLUDED_)
#define AFX_MOSISPITCH_H__CA80B4A7_F0BD_4AEA_A81B_CF86C09EC7C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MosisBuffer.h"


// type T�� ���� ����� ����(hive=����) ���� : MosisRecipe �� ����.
// �迭 �������� instance T�� �����ϸ� SetSize�� ���� Size�� �ø� �� �ִ�.
// ���� �޸� �۾��� �ּ�ȭ �� Mem free �������� Ȯ�� �ϱ� ���� ����Ѵ�.
typedef __int64 int64;

template<typename T>
class CMosisHive1B
{
protected:
	int		m_Size;
	int		m_Space;
	T*		m_Pointers;
public:
	CMosisHive1B()
	{
		m_Pointers= NULL;
		m_Space= m_Size= 0;
	}
	~CMosisHive1B()
	{
		if(m_Pointers)
			delete[] m_Pointers;
	}

	CMosisHive1B& operator =(CMosisHive1B& Hive)
	{
		if( SetSize(Hive.m_Size, FALSE))
			memcpy(m_Pointers, Hive.m_Pointers, sizeof(T)*Hive.GetSize());
		return *this;
	}

	int		GetSize(){return m_Size;}
	BOOL	SetSize(int size, BOOL bAppend)
	{
		ASSERT(size > 0);
		
		if(m_Pointers == NULL)
		{
			m_Pointers= new T[size];
			if(m_Pointers == NULL)
				return FALSE;
			m_Space= m_Size= size;
			return TRUE;
		}
		if(m_Space < size)
		{
			T *pOld= m_Pointers;
			m_Pointers= new T[size];
			if(m_Pointers == NULL)
			{
				m_Pointers= pOld;
				m_Size= 0;
				return FALSE;
			}
			
			if(bAppend)
				memcpy(m_Pointers, pOld, sizeof(T)*m_Size);
			
			delete[] pOld;
			
			m_Space= m_Size= size;
			return TRUE;
		}
		m_Size= size;
		return TRUE;
	}

	void	Delete(int i)
	{
		T* pData= GetData(i);
		if(pData == NULL)
			return;
		int size= GetSize()- 1- i;
		if(size > 0)
			memcpy(GetData(i), GetData(i+1), sizeof(T)*size);
		SetSize(GetSize()- 1, FALSE);
	}

	T*		Add()
	{
		int iAdd= m_Size;
		SetSize(m_Size+ 1, TRUE);
		return GetData(iAdd);
	}

	BOOL	Swap(int a, int b)
	{
		T* pa= GetData(a);
		T* pb= GetData(b);
		if(pa == NULL || pb == NULL)
			return FALSE;

		T temp= *pa;
		*pa= *pb;
		*pb= temp;
		return TRUE;
	}

	T*		GetData(int i)
	{
		if(i < 0 || i > GetSize()- 1)
			return NULL;
		return m_Pointers+ i;
	}
};




class AFX_EXT_CLASS CMosisPitch
{
public:
	double	m_xPitch;			// ���� ��ġ.
	double	m_yPitch;

	CRect	m_Rect;				// �˻� ����.
	
	double	m_xPitchOrigin;		// ���� ��ġ - �־��� ��ġ. 
	double	m_yPitchOrigin;
	long	m_CheckWidth;		// ��ġ ���� ���� width
	long	m_CheckHeight;
	long	m_xSearchRange;		// ���� ��ġ +- Range ���� ��ŭ �˻�.
	long	m_ySearchRange;
	
//	CMosisHive1B<int64>	m_DifferenceHive;
	
	CMosisPitch(double xPitch, double yPitch, long CheckWidth, long CheckHeight, long xSearchRange= 4, long ySearchRange= 4)
	{
		m_xPitchOrigin= xPitch;
		m_yPitchOrigin= yPitch;
		m_CheckWidth= CheckWidth;
		m_CheckHeight= CheckHeight;
		m_xSearchRange= xSearchRange;
		m_ySearchRange= ySearchRange;
		m_xPitch= 0;
		m_yPitch= 0;
	}
	CMosisPitch()
	{
		m_xPitchOrigin= 100.0;// 100.0 += m_SearchRange ��ŭ ����.
		m_yPitchOrigin= 100.0;// 100.0 += m_SearchRange ��ŭ ����.
		m_CheckWidth= 100;// widht 100 �ʺ�� �� ����.
		m_CheckHeight= 100;// Height 100 �ʺ�� �� ����.
		m_xSearchRange= 4;// PITCH +- 4 ���� ����.
		m_ySearchRange= 4;// PITCH +- 4 ���� ����.
		m_xPitch= 0;
		m_yPitch= 0;
	}
	
	BOOL CheckPitch(CMosisBuffer buffer, CRect rect);
};
#endif // !defined(AFX_MOSISPITCH_H__CA80B4A7_F0BD_4AEA_A81B_CF86C09EC7C1__INCLUDED_)
