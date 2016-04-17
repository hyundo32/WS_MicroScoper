#include "StdAfx.h"
#include "MotorControlPowerPmacCmd.h"

#include "PowerPmac.h"
#include "PowerPmacDef.h"

CMotorControlPowerPmacCmd::CMotorControlPowerPmacCmd(int nIndex, DWORD dwPeriod) : CMotorControl(nIndex, dwPeriod)
{
	m_uDeviceID = 0;
}

CMotorControlPowerPmacCmd::~CMotorControlPowerPmacCmd(void)
{
	Disconnect();
}

// CMotorControl
int	CMotorControlPowerPmacCmd::Connect(const CString& strPort)
{
	DWORD dwIPAddr = 0;

	m_strPort = strPort;

	TransformIP_CStringToDWORD(strPort, dwIPAddr);

	int nPort = 0;

	// Power PMAC Open
	m_uDeviceID	= DTKPowerPmacOpen(dwIPAddr, nPort);

	// Device Connect
	if (DTKConnect(m_uDeviceID) != DS_Ok)
	{
		return 0;
	}
	
	// alloc 
	m_nAxisCount = 0;
	for(VectorMotorGantryIt it = m_motorCommand.vectorMotorGantry.begin(); it != m_motorCommand.vectorMotorGantry.end(); it++)
	{
		m_nAxisCount += (int)it->vectorMotorAxis.size();
	}

	if (m_nAxisCount<1) 
	{
		return 0;
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

	m_bConnected = TRUE;

	return 1;
}

void CMotorControlPowerPmacCmd::Disconnect()
{
	if (m_bConnected==FALSE) return;

	// Power PMAC Close
	DTKDisconnect(m_uDeviceID);

	DTKPowerPmacClose(m_uDeviceID);

	m_bConnected = FALSE;
}

BOOL CMotorControlPowerPmacCmd::ReadAddressValue(int nAddress, long &nValue)
{
	if (!m_bConnected) return FALSE;	

	TCHAR pchResponse[256];
	CString str;
	str.Format(_T("sys.idata[%d]"), nAddress);
	UINT nResult = DTKGetResponseW(m_uDeviceID, (LPWSTR)(LPCTSTR)str, pchResponse, 256);
	
	CString strResponse;
	strResponse.Format(_T("%s"), pchResponse);

	if (nResult == DS_Ok)
	{
		if (strResponse.GetLength()>0)
		{
			strResponse=strResponse.Mid(strResponse.Find(_T("="))+1);
			nValue = _wtoi(strResponse);
		}		
		return TRUE;
	}

	return FALSE;
}

BOOL CMotorControlPowerPmacCmd::WriteAddressValue(int nAddress, long nValue)
{
	if (!m_bConnected) return FALSE;	

	TCHAR pchResponse[256];

	CString	str;
	str.Format(_T("sys.idata[%d]=%d"), nAddress, nValue);
	UINT nResult = DTKGetResponseW(m_uDeviceID, (LPWSTR)(LPCTSTR)str, pchResponse, 256);

	if (nResult == DS_Ok)
		return TRUE;

	return FALSE;
}

BOOL CMotorControlPowerPmacCmd::ReadAddressValue(int nAddress, float &fValue)
{
	if (!m_bConnected) return FALSE;	

	TCHAR pchResponse[256];
	CString str;
	str.Format(_T("sys.fdata[%d]"), nAddress);
	UINT nResult = DTKGetResponseW(m_uDeviceID, (LPWSTR)(LPCTSTR)str, pchResponse, 256);

	CString strResponse;
	strResponse.Format(_T("%s"), pchResponse);

	if (nResult == DS_Ok)
	{
		if (strResponse.GetLength()>0)
		{
			strResponse=strResponse.Mid(strResponse.Find(_T("="))+1);
			fValue = (float)_wtof(strResponse);
		}		
		return TRUE;
	}

	return FALSE;
}

BOOL CMotorControlPowerPmacCmd::WriteAddressValue(int nAddress, float fValue)
{
	if (!m_bConnected) return FALSE;	

	TCHAR pchResponse[256];

	CString	str;
	str.Format(_T("sys.fdata[%d]=%f"), nAddress, fValue);
	UINT nResult = DTKGetResponseW(m_uDeviceID, (LPWSTR)(LPCTSTR)str, pchResponse, 256);

	if (nResult == DS_Ok)
		return TRUE;

	return FALSE;
}

BOOL CMotorControlPowerPmacCmd::ReadAddressValue(int nAddress, double &dValue)
{
	if (!m_bConnected) return FALSE;	

	BOOL bSuccess = TRUE;
	TCHAR pchResponse[256];

	CString str;
	str.Format(_T("sys.fdata[%d]"), nAddress);
	UINT nResult = DTKGetResponseW(m_uDeviceID, (LPWSTR)(LPCTSTR)str, pchResponse, 256);

	CString strResponse;
	strResponse.Format(_T("%s"), pchResponse);

	if (nResult == DS_Ok)
	{
		if (strResponse.GetLength()>0)
		{
			strResponse=strResponse.Mid(strResponse.Find(_T("="))+1);
			dValue = _wtof(strResponse);
		}		
		return TRUE;
	}

	return FALSE;
}

BOOL CMotorControlPowerPmacCmd::WriteAddressValue(int nAddress, double dValue)
{
	if (!m_bConnected) return FALSE;	

	TCHAR pchResponse[256];

	CString	str;
	str.Format(_T("sys.fdata[%d]=%lf"), nAddress, dValue);
	UINT nResult = DTKGetResponseW(m_uDeviceID, (LPWSTR)(LPCTSTR)str, pchResponse, 256);

	if (nResult == DS_Ok)
		return TRUE;

	return FALSE;
}

// CTimerThreadPools
int CMotorControlPowerPmacCmd::StartThread()
{
	return CreateTimerThread(this);
}

BOOL CMotorControlPowerPmacCmd::ReadAddressValue(long nAddress, long *pArrayData, int nArrayCount)
{
	BOOL bResult = TRUE;
	for (int i=0; i<nArrayCount; i++)
	{
		bResult = bResult && ReadAddressValue(nAddress+i, pArrayData[i]);
	}
	return bResult;
}

BOOL CMotorControlPowerPmacCmd::WriteAddressValue(long nAddress, long *pArrayData, int nArrayCount)
{
	BOOL bResult = TRUE;
	for (int i=0; i<nArrayCount; i++)
	{
		bResult = bResult && WriteAddressValue(nAddress+i, pArrayData[i]);
	}
	return bResult;
}

BOOL CMotorControlPowerPmacCmd::ReadAddressValue(long nAddress, float *pArrayData, int nArrayCount)
{
	BOOL bResult = TRUE;
	for (int i=0; i<nArrayCount; i++)
	{
		bResult = bResult && ReadAddressValue(nAddress+i, pArrayData[i]);
	}
	return bResult;
}

BOOL CMotorControlPowerPmacCmd::WriteAddressValue(long nAddress, float *pArrayData, int nArrayCount)
{
	BOOL bResult = TRUE;
	for (int i=0; i<nArrayCount; i++)
	{
		bResult = bResult && WriteAddressValue(nAddress+i, pArrayData[i]);
	}
	return bResult;
}
