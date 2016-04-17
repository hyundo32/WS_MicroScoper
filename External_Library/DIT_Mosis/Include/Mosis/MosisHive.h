// MosisHive.h: interface for the CMosisHive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOSISHIVE_H__EAC0D9E3_4298_453A_90FD_0E4FC7C53682__INCLUDED_)
#define AFX_MOSISHIVE_H__EAC0D9E3_4298_453A_90FD_0E4FC7C53682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <vector>




// type T�� ���� ����� ����(hive=����)
// �迭 �������� instance T�� �����ϸ� SetSize�� ���� Size�� �ø� �� �ִ�.
// ���� �޸� �۾��� �ּ�ȭ �� Mem free �������� Ȯ�� �ϱ� ���� ����Ѵ�.
// �����͸� ���� Class�� T�� �����ؼ� ����� ���� ����. �ڰ������� �ʿ� ���� ���� Data�� ���ؼ��� ������ �����Ѵ�.
template<typename T>
class AFX_EXT_CLASS CMosisHive1
{
protected:
	int		m_Size;
	int		m_Space;
	T*		m_Pointers;
public:
	CMosisHive1()
	{
		m_Pointers= NULL;
		m_Space= m_Size= 0;
	}
	~CMosisHive1()
	{
		if(m_Pointers)
			delete[] m_Pointers;
	}

	CMosisHive1& operator =(CMosisHive1& Hive)
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

// type T�� ���� ����� ����(hive=����)�� 2�������� Ȯ���Ѵ�.
// �迭 �������� instance T�� �����ϸ� SetSize�� ���� Size�� �ø� �� �ִ�.
// ���� �޸� �۾��� �ּ�ȭ �� Mem free �������� Ȯ�� �ϱ� ���� ����Ѵ�.
// �����͸� ���� Class�� T�� �����ؼ� ����� ���� ����. �ڰ������� �ʿ� ���� ���� Data�� ���ؼ��� ������ �����Ѵ�.
template<typename T>
class AFX_EXT_CLASS CMosisHive2
{
protected:
	int		m_Width, m_Height;
	int		m_Space;
	T*		m_Pointers;

public:
	CMosisHive2()		{m_Pointers= NULL; m_Space= m_Width= m_Height= 0;}
	~CMosisHive2()	{if(m_Pointers) delete[] m_Pointers;}

public:
	T*		GetData(int x, int y)	{ return GetData(y*m_Width+ x);	}
	T*		GetData(int i)
	{
		if(i < 0 || i > GetSize()- 1)
			return NULL;
		return m_Pointers+ i;
	}

	CMosisHive2& operator =(CMosisHive2& Hive)
	{
		if(SetSize(Hive.GetWidth(), Hive.GetHeight()))
			memcpy(m_Pointers, Hive.m_Pointers, sizeof(T)*Hive.GetSize());
		return *this;
	}

	int		GetWidth(){return m_Width;}
	int		GetHeight(){return m_Height;}
	int		GetSize(){return m_Width*m_Height;}
	BOOL	SetSize(int width, int height)
	{
		int size= width*height;

		ASSERT(size > 0);

		if(m_Pointers == NULL)
		{
			m_Pointers= new T[size];
			if(m_Pointers == NULL)
				return FALSE;
			m_Space= size;
			m_Width= width;
			m_Height= height;
			return TRUE;
		}
		if(m_Space < size)
		{
			T *pOld= m_Pointers;
			m_Pointers= new T[size];
			if(m_Pointers == NULL)
			{
				m_Pointers= pOld;
				m_Width= m_Height= 0;
				return FALSE;
			}

			delete[] pOld;

			m_Space= size;
			m_Width= width;
			m_Height= height;
			return TRUE;
		}
		m_Width= width;
		m_Height= height;
		return TRUE;
	}
};

/*

template<typename T>
class CPointerHive
{
protected:
	std::vector<T*> m_Pointers;
	
public:
	~CPointerHive()
	{
		Reset();
	}
	
public:
	void Reset()
	{
		for(int iPointer= 0; iPointer< m_Pointers.size(); iPointer++)
		{
			delete m_Pointers[iPointer];
		}
		m_Pointers.clear();
	}
	void Add(T *pData)// You should not add a pointer to local instance. It only holds the pointers.
	{
		m_Pointers.push_back(pData);
	}
	void Delete(int iPointer)
	{
		if(iPointer >= 0 && iPointer < m_Pointers.size())
		{
			delete m_Pointers[iPointer];
			m_Pointers.erase(m_Pointers.begin()+ iPointer);
		}
	}
	int	GetHiveSize(){return m_Pointers.size();}
	T*	GetPointer(int iPointer)
	{
		if(iPointer >= 0 && iPointer < GetHiveSize())
		{
			return m_Pointers[iPointer];
		}
		return NULL;
	}
};
template<typename T>
class CStaticPointerHive
{
protected:
	int	m_Size;
	int m_Space;
	T* m_Pointers;
	
public:
	CStaticPointerHive()
	{
		m_Space= m_Size= 0;
		m_Pointers= NULL;
	}
	~CStaticPointerHive()
	{
		Reset();
	}
	
public:
	BOOL SetSize(int size)
	{
		if(m_Pointers == NULL)
		{
			m_Pointers= new T[size];
			if(m_Pointers == NULL)
				return FALSE;
			m_Space= m_Size= size;
			return TRUE;
		}
		if(size > m_Space)
		{
			Reset();
			m_Pointers= new T[size];
			if(m_Pointers == NULL)
			{
				return FALSE;
			}
			m_Space= m_Size= size;
		}
		m_Size= size;
		return TRUE;
	}
	int	GetHiveSize(){return m_Size;}
	T*	GetPointer(int iPointer)
	{
		if(iPointer >= 0 && iPointer < GetHiveSize())
		{
			return m_Pointers+ iPointer;
		}
		return NULL;
	}
	
protected:
	void Reset()
	{
		m_Size= m_Space= 0;
		if(m_Pointers)
			delete[] m_Pointers;
		m_Pointers= NULL;
	}
};
*/
#endif // !defined(AFX_MOSISHIVE_H__EAC0D9E3_4298_453A_90FD_0E4FC7C53682__INCLUDED_)
