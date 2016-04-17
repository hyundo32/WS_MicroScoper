
// MosisPacket.h: interface for the CMosisPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOSISPACKET_H__EAC0D9E3_4298_453A_90FD_0E4FC7C53682__INCLUDED_)
#define AFX_MOSISPACKET_H__EAC0D9E3_4298_453A_90FD_0E4FC7C53682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




// Byte 형태의 메모리를 Array형태로 관리.
// 쓰면서 자동으로 메모리를 확장한다.
// type형태에 상관없이 메모리 단위로 I/O 가 가능하다.
class AFX_EXT_CLASS CMosisPacket// 버퍼( for 1 Dimensional Byte Data)
{
protected:
	BYTE	*m_pData;
	int		m_Size;
	int		m_Space;

	int		m_iRead;
	int		m_iWrite;

public:
	CMosisPacket()								{m_pData= NULL, m_Size= m_Space= 0; ResetPacket();}
	~CMosisPacket()								{if(m_pData) delete[] m_pData;}

	void	ResetPacket()						{m_iWrite= m_iRead= 0;}

	BYTE*	GetDataAddress()					{return m_pData;}
	BYTE*	GetDataAddress(int offset)			{return m_pData+ offset;}
	BOOL	IsValidHive()						{return m_pData != NULL && m_Size > 0 && m_Space > 0;}

	int		GetReadSize()						{return m_iRead;}
	int		GetWriteSize()						{return m_iWrite;}
	int		GetSize()							{return m_Size;}

	void	SetData(BYTE *pBuffer,int nBufSize) 
	{
		if(m_pData != NULL)
			delete[] m_pData, m_pData= NULL;
		m_pData=pBuffer,m_Size=nBufSize;
	}
	void	SetBufferNull(BYTE *pBuffer,int nBufSize) 
	{
		m_pData=pBuffer,m_Size=nBufSize;
	}

	BOOL	SetSize(int size, BOOL bAppend)
	{
		if(m_pData == NULL)
		{
			m_pData= new BYTE[size];
			if(m_pData == NULL)
				return FALSE;
			m_Size= m_Space= size;
		}
		else if(size > m_Space)
		{
			BYTE* pOld= m_pData;

			m_pData= new BYTE[size];
			if(m_pData == NULL)
			{
				m_pData= pOld;
				m_Size= 0;
				return FALSE;
			}

			if(bAppend)
				memcpy(m_pData, pOld, m_Size);

			delete[] pOld;

			m_Space= size;
		}
		m_Size= size;
		return TRUE;
	}

	template<typename VV>
	BOOL	ReadValue(VV &val)
	{
		if(m_iRead+ sizeof(VV) > m_Size)
			return FALSE;
		
		val= *((VV*) GetDataAddress(m_iRead));
		m_iRead+= sizeof(VV);
		return m_iRead;
	}
	
	template<typename VV>
	BOOL	WriteValue(VV &val)
	{
		if(SetSize(m_iWrite+ sizeof(VV), TRUE) < 1)
			return FALSE;
		
		*((VV*) GetDataAddress(m_iWrite))= val;
		m_iWrite+= sizeof(VV);
		return m_iWrite;
	}
	template<typename VV>
	BOOL	ReadArray(VV *pData, int nData)
	{// virtual table 등을 조심할 것. structure등 데이터만 선언되어 있는 것을 사용하는게 좋다.
		if(nData < 1)
			return FALSE;

		return Read((BYTE*)pData, nData);
	}
	
	template<typename VV>
	BOOL	WriteArray(VV *pData, int nData)
	{// virtual table 등을 조심할 것. structure등 데이터만 선언되어 있는 것을 사용하는게 좋다.
		if(nData < 1)
			return FALSE;
		return Write((BYTE*)pData, nData);
	}

	BOOL	Read(BYTE *pData, int size)
	{// virtual table 등을 조심할 것. structure등 데이터만 선언되어 있는 것을 사용하는게 좋다.
		if(m_iRead+ size > m_Size)
			return FALSE;

		memcpy(pData, GetDataAddress(m_iRead), size);
		m_iRead+= size;
		return m_iRead;
	}

	BOOL	Write(BYTE *pData, int size)
	{// virtual table 등을 조심할 것. structure등 데이터만 선언되어 있는 것을 사용하는게 좋다.
		if(SetSize(m_iWrite+ size, TRUE) < 1)
			return FALSE;
		memcpy(GetDataAddress(m_iWrite), pData, size);
		m_iWrite+= size;
		return m_iWrite;
	}
};

#endif // !defined(AFX_MOSISPACKET_H__EAC0D9E3_4298_453A_90FD_0E4FC7C53682__INCLUDED_)
