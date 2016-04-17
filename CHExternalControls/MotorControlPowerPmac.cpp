#include "StdAfx.h"
#include "MotorControlPowerPmac.h"

#include "PPmacDprRuntime.h"

CMotorControlPowerPmac::CMotorControlPowerPmac(int nIndex, DWORD dwPeriod) : CMotorControl(nIndex, dwPeriod)
{
	m_uDeviceID = 0;
}

CMotorControlPowerPmac::~CMotorControlPowerPmac(void)
{
	Disconnect();
}

int CMotorControlPowerPmac::StartThread()
{
	return CreateTimerThread(this);
}

int	CMotorControlPowerPmac::Connect(const CString& strPort)
{
	ClosePPmacDprRuntimeLink();

	if (OpenPPmacDprRuntimeLink()==NULL)
	{
		return 0;
	}

	PPmacDprDisconnect(m_uDeviceID);
	PPmacDprClose(m_uDeviceID);

	m_bConnected = FALSE;

	DWORD dwipAddress = 0;

	TransformIP_CStringToDWORD(strPort, dwipAddress);

	int nPortNo = 1025;

	m_uDeviceID = PPmacDprOpen(dwipAddress, nPortNo);
	if ( PPmacDprConnect(m_uDeviceID)!=PS_Ok )
	{
		m_bConnected = FALSE;
		return -1;
	}

	m_bConnected = TRUE;

	// alloc 
	m_nAxisCount = 0;
	for(VectorMotorGantryIt it = m_motorCommand.vectorMotorGantry.begin(); it != m_motorCommand.vectorMotorGantry.end(); it++)
	{
		m_nAxisCount += (int)it->vectorMotorAxis.size();
	}

	if (m_nAxisCount<1) 
	{
		return -2;
	}

	if (m_pAxisStatus) delete  [] m_pAxisStatus;
	m_pAxisStatus = new long[m_nAxisCount];
	memset(m_pAxisStatus, 0, sizeof(long)*m_nAxisCount);

	if (m_pAxisPosition) delete  [] m_pAxisPosition;
	m_pAxisPosition = new float[m_nAxisCount];
	memset(m_pAxisPosition, -1, sizeof(float)*m_nAxisCount);

	if (m_pAxisStatusPrev) delete  [] m_pAxisStatusPrev;
	m_pAxisStatusPrev = new long[m_nAxisCount];
	memset(m_pAxisStatusPrev, 0, sizeof(long)*m_nAxisCount);

	if (m_pAxisPositionPrev) delete  [] m_pAxisPositionPrev;
	m_pAxisPositionPrev = new float[m_nAxisCount];
	memset(m_pAxisPositionPrev, -1, sizeof(float)*m_nAxisCount);

	if (m_pAxisStatusAddr) delete  [] m_pAxisStatusAddr;
	m_pAxisStatusAddr = new long[m_nAxisCount];

	if (m_pAxisPositionAddr) delete  [] m_pAxisPositionAddr;
	m_pAxisPositionAddr = new long[m_nAxisCount];

	int nAxisIndex = 0;
	for(VectorMotorGantryIt it = m_motorCommand.vectorMotorGantry.begin(); it != m_motorCommand.vectorMotorGantry.end(); it++)
	{
		for(VectorMotorAxisIt it2 = it->vectorMotorAxis.begin(); it2 != it->vectorMotorAxis.end(); it2++)
		{
			m_pAxisPositionAddr[nAxisIndex] = it2->nPositionAddr;
			m_pAxisStatusAddr[nAxisIndex] = it2->nStatusAddr;
			nAxisIndex++;
		}
	}

	return 1;
}

void CMotorControlPowerPmac::Disconnect()
{
	if (m_bConnected)
	{
		PPmacDprDisconnect(m_uDeviceID);

		PPmacDprClose(m_uDeviceID);

		m_bConnected = FALSE;
	}

	ClosePPmacDprRuntimeLink();
}

inline void CMotorControlPowerPmac::SwapBytes(int nSizeType, void *pSource, int nSourceSize)
{
	if (nSizeType==0 || (nSizeType%2)!=0) return;

	unsigned char temp;
	unsigned char* pSrcData = (unsigned char*)pSource;
	unsigned char* pSrc = NULL;
	unsigned char* pDst = NULL;
	
	int nLoopCount = nSizeType / 2;
	int nJumpSize = nSizeType - 1;
	
	for (int i=0; i<nSourceSize; i++)
	{		
		pSrc = pSrcData;
		pDst = pSrc + nJumpSize;
		for (int j=0; j<nLoopCount; j++)
		{
			// swap data
			temp = *pSrc;
			*pSrc = *pDst;
			*pDst = temp;

			pSrc++;
			pDst--;
		}
		pSrcData += nSizeType;
	}
}

BOOL CMotorControlPowerPmac::ReadAddressValue(int nAddress, double &dValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;

	float fValue = 0.0f;
	if (PPmacDprGetDPRMem(m_uDeviceID, (long)nAddress*4, sizeof(float), PVOID(&fValue))!=PS_Ok)
	{
		return FALSE;
	}

	// swap
	SwapBytes(sizeof(float), &fValue, 1);

	dValue = fValue;

	return TRUE;
}

BOOL CMotorControlPowerPmac::ReadAddressValue(int nAddress, long &lValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;

	lValue = 0;
	if (PPmacDprGetDPRMem(m_uDeviceID, (long)nAddress*4, sizeof(long), PVOID(&lValue))!=PS_Ok)
	{
		return FALSE;
	}

	// swap
	SwapBytes(sizeof(long), &lValue, 1);

	return TRUE;
}

BOOL CMotorControlPowerPmac::WriteAddressValue(int nAddress, double dValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;

	float fValue = float(dValue);

	// swap
	SwapBytes(sizeof(float), &fValue, 1);

	if (PPmacDprSetDPRMem(m_uDeviceID, (long)nAddress*4, sizeof(float), PVOID(&fValue))!=PS_Ok)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CMotorControlPowerPmac::WriteAddressValue(int nAddress, long lValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;

	// swap
	SwapBytes(sizeof(long), &lValue, 1);

	if (PPmacDprSetDPRMem(m_uDeviceID, (long)nAddress*4, sizeof(long), PVOID(&lValue))!=PS_Ok)
	{
		return FALSE;
	}
	
	return TRUE;
}


BOOL CMotorControlPowerPmac::ReadAddressValue(int nAddress, float &fValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;

	if (PPmacDprGetDPRMem(m_uDeviceID, (long)nAddress*4, sizeof(float), PVOID(&fValue))!=PS_Ok)
	{
		return FALSE;
	}

	// swap
	SwapBytes(sizeof(float), &fValue, 1);

	return TRUE;
}

BOOL CMotorControlPowerPmac::WriteAddressValue(int nAddress, float fValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;

	// swap
	SwapBytes(sizeof(float), &fValue, 1);

	if (PPmacDprSetDPRMem(m_uDeviceID, (long)nAddress*4, sizeof(float), PVOID(&fValue))!=PS_Ok)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CMotorControlPowerPmac::ReadAddressValue(long nAddress, long *pArrayData, int nArrayCount)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;

	if (PPmacDprGetDPRMem(m_uDeviceID, (long)nAddress*4, sizeof(long)*nArrayCount, PVOID(pArrayData))!=PS_Ok)
	{
		return FALSE;
	}

	// swap
	SwapBytes(sizeof(long), pArrayData, nArrayCount);

	return TRUE;
}

BOOL CMotorControlPowerPmac::WriteAddressValue(long nAddress, long *pArrayData, int nArrayCount)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;

	// swap
	SwapBytes(sizeof(long), pArrayData, nArrayCount);

	if (PPmacDprSetDPRMem(m_uDeviceID, (long)nAddress*4, sizeof(long)*nArrayCount, PVOID(pArrayData))!=PS_Ok)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CMotorControlPowerPmac::ReadAddressValue(long nAddress, float *pArrayData, int nArrayCount)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;

	if (PPmacDprGetDPRMem(m_uDeviceID, (long)nAddress*4, sizeof(float)*nArrayCount, PVOID(pArrayData))!=PS_Ok)
	{
		return FALSE;
	}

	// swap
	SwapBytes(sizeof(float), pArrayData, nArrayCount);

	return TRUE;
}

BOOL CMotorControlPowerPmac::WriteAddressValue(long nAddress, float *pArrayData, int nArrayCount)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;

	// swap
	SwapBytes(sizeof(float), pArrayData, nArrayCount);

	if (PPmacDprSetDPRMem(m_uDeviceID, (long)nAddress*4, sizeof(float)*nArrayCount, PVOID(pArrayData))!=PS_Ok)
	{
		return FALSE;
	}

	return TRUE;
}
