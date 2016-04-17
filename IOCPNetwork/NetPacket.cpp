#include "StdAfx.h"
#include "NetPacket.h"
#include "IOCPDefinition.h"

#define		STEP_INCREASE		5

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetPacket::CNetPacket()
{
	m_bUsing = FALSE;

	m_nShortArraySize = STEP_INCREASE;
	m_psShort = new short[m_nShortArraySize];

	m_nIntArraySize = STEP_INCREASE;
	m_pnInt = new int[m_nIntArraySize];

	m_nDoubleArraySize = STEP_INCREASE;
	m_pdDouble = new double[m_nDoubleArraySize];

	m_nStringArraySize = STEP_INCREASE;
	m_pstrString = new CString[m_nStringArraySize];
	for (int i = 0; i < m_nStringArraySize; i++)
		m_pstrString[i] = _T("");

	m_nBufferSize = 0;
	m_pBuffer = NULL;

	ResetValues();
}

CNetPacket::~CNetPacket()
{
	m_nShortCount = 0;
	m_nShortArraySize = 0;
	if (m_psShort)
		delete [] m_psShort;
	m_psShort = NULL;

	m_nIntCount = 0;
	m_nIntArraySize = 0;
	if (m_pnInt)
		delete [] m_pnInt;
	m_pnInt = NULL;

	m_nDoubleCount = 0;
	m_nDoubleArraySize = 0;
	if (m_pdDouble)
		delete [] m_pdDouble;
	m_pdDouble = NULL;

	m_nStringCount = 0;
	m_nStringArraySize = 0;
	if (m_pstrString)
		delete [] m_pstrString;
	m_pstrString = NULL;

	m_nBufferSize = 0;
	if (m_pBuffer)
		delete [] m_pBuffer;
	m_pBuffer = NULL;
}

void CNetPacket::SetVersion(SHORT sNetworkCode, SHORT sVersion)
{
	m_sSetNetworkCode = sNetworkCode;
	m_sSetVersion = sVersion;

	m_sNetworkCode = m_sSetNetworkCode;
	m_sVersion = m_sSetVersion;
}

int CNetPacket::Read_PacketHeader(WSABUF* pWsaBuf)
{
	if (!pWsaBuf)
		return 0;

	// 읽기 버퍼 설정.
	int nReadSize = 0;

	try
	{
		memcpy(&m_nSize, pWsaBuf->buf + nReadSize, sizeof(int));
		nReadSize += sizeof(int);
		memcpy(&m_sNetworkCode, pWsaBuf->buf + nReadSize, sizeof(SHORT));
		nReadSize += sizeof(SHORT);
		memcpy(&m_sVersion, pWsaBuf->buf + nReadSize, sizeof(SHORT));
		nReadSize += sizeof(SHORT);
		memcpy(&m_nPacketCode, pWsaBuf->buf + nReadSize, sizeof(int));
		nReadSize += sizeof(int);
		memcpy(&m_sPacketIndex, pWsaBuf->buf + nReadSize, sizeof(SHORT));
		nReadSize += sizeof(SHORT);
		memcpy(&m_sResult, pWsaBuf->buf + nReadSize, sizeof(SHORT));
		nReadSize += sizeof(SHORT);
		memcpy(&m_nModuleNo, pWsaBuf->buf + nReadSize, sizeof(int));
		nReadSize += sizeof(int);
	}
	catch (...)
	{
		return 0;		
	}

	return nReadSize;
}

int CNetPacket::Read_PacketBody(char* pBuf)
{
	int nReadSize = 0, i, nCount;

	try
	{
		// short 잘라내기.
		memcpy(&nCount, pBuf + nReadSize, sizeof(int));
		nReadSize += sizeof(int);
		short sValue;
		for (i = 0; i < nCount; i++)
		{
			memcpy(&sValue, pBuf + nReadSize, sizeof(short));
			nReadSize += sizeof(short);
			if (nReadSize + PACKET_HEADERSIZE <= MAX_BUFFER_SIZE)
				SetShort(sValue);
			else
				return -1;
		}

		// int 잘라내기.
		memcpy(&nCount, pBuf + nReadSize, sizeof(int));
		nReadSize += sizeof(int);
		int nValue;
		for (i = 0; i < nCount; i++)
		{
			memcpy(&nValue, pBuf + nReadSize, sizeof(int));
			nReadSize += sizeof(int);
			if (nReadSize + PACKET_HEADERSIZE <= MAX_BUFFER_SIZE)
				SetInt(nValue);
			else
				return -1;
		}

		// double 잘라내기.
		memcpy(&nCount, pBuf + nReadSize, sizeof(int));
		nReadSize += sizeof(int);
		double dValue;
		for (i = 0; i < nCount; i++)
		{
			memcpy(&dValue, pBuf + nReadSize, sizeof(double));
			nReadSize += sizeof(double);
			if (nReadSize + PACKET_HEADERSIZE <= MAX_BUFFER_SIZE)
				SetDouble(dValue);
			else
				return -1;
		}

		// String 잘라내기.
		memcpy(&nCount, pBuf + nReadSize, sizeof(int));
		nReadSize += sizeof(int);

		int nStringSize;
		CString strValue = _T("");
		for (i = 0; i < nCount; i++)
		{
			memcpy(&nStringSize, pBuf + nReadSize, sizeof(int));
			nReadSize += sizeof(int);

			char* str = new char[nStringSize + 1];
			ZeroMemory(str, nStringSize + 1);
			memcpy(str, pBuf + nReadSize, nStringSize);
			nReadSize += nStringSize;
			if (nReadSize + PACKET_HEADERSIZE <= MAX_BUFFER_SIZE)
			{
				strValue = str;
				SetString(strValue);
			}
			else
			{
				delete [] str;
				return -1;
			}
			delete [] str;
		}

		// Buffer 잘라내기.
		memcpy(&nCount, pBuf + nReadSize, sizeof(int));
		nReadSize += sizeof(int);
		if (nCount > 0)
		{
			if (nReadSize + PACKET_HEADERSIZE <= MAX_BUFFER_SIZE)
				SetBuffer(nCount, pBuf + nReadSize);
			else
				return -1;
		}
	}
	catch (...)
	{
		return -2;
	}

	return nReadSize;
}

//////////////////////////////////////////////////////////////////////////
// For the Body
void CNetPacket::ResetValues()
{
	m_nSize				= 0;
	m_sNetworkCode		= 0;
	m_sVersion			= 0;
	m_nPacketCode		= 0;
	m_sPacketIndex		= 0;
	m_sResult			= 0;
	m_nModuleNo			= -1;

	m_nShortCount = 0;
	ZeroMemory(m_psShort, sizeof(short) * m_nShortArraySize);

	m_nIntCount = 0;
	ZeroMemory(m_pnInt, sizeof(int) * m_nIntArraySize);

	m_nDoubleCount = 0;
	ZeroMemory(m_pdDouble, sizeof(double) * m_nDoubleArraySize);

	m_nStringCount = 0;
	for (int i = 0; i < m_nStringCount; i++)
		m_pstrString[i] = _T("");

	if (m_nBufferSize > 0)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
		m_nBufferSize = 0;
	}
}

BOOL CNetPacket::SetShort(short sValue)
{
	if (m_nShortCount >= m_nShortArraySize)	// 배열을 키워야 한다.
	{
		int i;
		// Backup
		short* psTemp = new short[m_nShortArraySize];
		for (i = 0; i < m_nShortArraySize; i++)
			psTemp[i] = m_psShort[i];

		// Memory Reallocation
		delete [] m_psShort;
		m_nShortArraySize += STEP_INCREASE;
		m_psShort = new short[m_nShortArraySize];
		ZeroMemory(m_psShort, sizeof(short) * m_nShortArraySize);

		// 복원
		for (i = 0; i < m_nShortArraySize - STEP_INCREASE; i++)
			m_psShort[i] = psTemp[i];

		delete [] psTemp;
	}

	m_psShort[m_nShortCount] = sValue;
	m_nShortCount++;

	return TRUE;
}

BOOL CNetPacket::SetInt(int nValue)
{
	if (m_nIntCount >= m_nIntArraySize)	// 배열을 키워야 한다.
	{
		int i;
		// Backup
		int* pnTemp = new int[m_nIntArraySize];
		for (i = 0; i < m_nIntArraySize; i++)
			pnTemp[i] = m_pnInt[i];

		// Memory Reallocation
		delete [] m_pnInt;
		m_nIntArraySize += STEP_INCREASE;
		m_pnInt = new int[m_nIntArraySize];
		ZeroMemory(m_pnInt, sizeof(int) * m_nIntArraySize);

		// 복원
		for (i = 0; i < m_nIntArraySize - STEP_INCREASE; i++)
			m_pnInt[i] = pnTemp[i];

		delete [] pnTemp;
	}

	m_pnInt[m_nIntCount] = nValue;
	m_nIntCount++;

	return TRUE;
}

BOOL CNetPacket::SetDouble(double dValue)
{
	if (m_nDoubleCount >= m_nDoubleArraySize)	// 배열을 키워야 한다.
	{
		int i;
		// Backup
		double* pdTemp = new double[m_nDoubleArraySize];
		for (i = 0; i < m_nDoubleArraySize; i++)
			pdTemp[i] = m_pdDouble[i];

		// Memory Reallocation
		delete [] m_pdDouble;
		m_nDoubleArraySize += STEP_INCREASE;
		m_pdDouble = new double[m_nDoubleArraySize];
		ZeroMemory(m_pdDouble, sizeof(double) * m_nDoubleArraySize);

		// 복원
		for (i = 0; i < m_nDoubleArraySize - STEP_INCREASE; i++)
			m_pdDouble[i] = pdTemp[i];

		delete [] pdTemp;
	}

	m_pdDouble[m_nDoubleCount] = dValue;
	m_nDoubleCount++;

	return TRUE;
}

BOOL CNetPacket::SetString(const CString& strString)
{
	if (m_nStringCount >= m_nStringArraySize)	// 배열을 키워야 한다.
	{
		int i;
		// Backup
		CString* pstrTemp = new CString[m_nStringArraySize];
		for (i = 0; i < m_nStringArraySize; i++)
			pstrTemp[i] = m_pstrString[i];

		// Memory Reallocation
		delete [] m_pstrString;
		m_nStringArraySize += STEP_INCREASE;
		m_pstrString = new CString[m_nStringArraySize];
		for (i = 0; i < m_nStringArraySize; i++)
			m_pstrString[i].Empty();

		// 복원
		for (i = 0; i < m_nStringArraySize - STEP_INCREASE; i++)
			m_pstrString[i] = pstrTemp[i];

		delete [] pstrTemp;
	}

	m_pstrString[m_nStringCount] = strString;
	m_nStringCount++;

	return TRUE;
}

BOOL CNetPacket::SetBuffer(int nSize, char* pBuffer)
{
	if (nSize <= 0)
		return FALSE;

	if (m_nBufferSize > 0)
		delete [] m_pBuffer;

	m_nBufferSize = nSize;
	m_pBuffer = new char[m_nBufferSize + 1];
	ZeroMemory(m_pBuffer, m_nBufferSize + 1);
	memcpy(m_pBuffer, pBuffer, m_nBufferSize);

	return TRUE;
}

short CNetPacket::GetShort(int nIdx)
{
	if (nIdx < m_nShortArraySize && nIdx >= 0)
		return m_psShort[nIdx];

	return 0;
}

int CNetPacket::GetInt(int nIdx)
{
	if (nIdx < m_nIntArraySize && nIdx >= 0)
		return m_pnInt[nIdx];

	return 0;
}

double CNetPacket::GetDouble(int nIdx)
{
	if (nIdx < m_nDoubleArraySize && nIdx >= 0)
		return m_pdDouble[nIdx];

	return 0;
}

CString CNetPacket::GetString(int nIdx)
{
	if (nIdx < m_nStringArraySize && nIdx >= 0)
		return m_pstrString[nIdx];

	CString str(_T(""));
	return str;
}

char* CNetPacket::GetBuffer()
{
	return m_pBuffer;
}

