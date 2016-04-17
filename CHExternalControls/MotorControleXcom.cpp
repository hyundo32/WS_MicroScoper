// MotorControlExcom.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "eXcomDevice.h"
#include "MotorControleXcom.h"

// CMotorControleXcom

#define MAX_WRITE_MULTI_DATA_COUNT	100

CMotorControleXcom::CMotorControleXcom(int nIndex, DWORD dwPeriod) : CMotorControl(nIndex, dwPeriod)
{
	m_peXcomDevice			= NULL;
	m_bConnected			= FALSE;
	
	m_peXcomDevice			= new CeXcomDevice(nIndex);
	m_MotionEndThreadData.pThisPtr = this;
}

CMotorControleXcom::~CMotorControleXcom()
{
	Disconnect();

	if (m_peXcomDevice) 
	{
		delete m_peXcomDevice;
		m_peXcomDevice = NULL;
	}
}

int CMotorControleXcom::StartThread()
{
	return CreateTimerThread(this);
}

int	CMotorControleXcom::Connect(const CString& strPort)
{
	if(m_peXcomDevice == NULL) return 0;

	int nPort;
	CString strIPAddress;
	DWORD dwIPAddress;

	nPort = 1500;
	strIPAddress = strPort; 

	TransformIP_CStringToDWORD(strIPAddress, dwIPAddress);

	m_bConnected = m_peXcomDevice->Connect(dwIPAddress, nPort);

	if (m_bConnected==FALSE) return 0;

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

	m_axisThetaStatus			= 0;
	m_axisThetaPosition			= 0.f;
	m_axisThetaStatusPrev		= 0;
	m_axisThetaPositionPrev		= -999.f;

	return 1;
}

void CMotorControleXcom::Disconnect()
{
	if(m_peXcomDevice == NULL) return;

	if (m_bConnected)
	{
		m_peXcomDevice->Close();
		m_bConnected = FALSE;

		m_nAxisCount = 0;

		delete [] m_pAxisStatus;		
		m_pAxisStatus = NULL;

		delete [] m_pAxisPosition;		
		m_pAxisPosition = NULL;

		delete [] m_pAxisStatusPrev;	
		m_pAxisStatusPrev = NULL;

		delete [] m_pAxisPositionPrev;	
		m_pAxisPositionPrev = NULL;

		delete [] m_pAxisStatusAddr;	
		m_pAxisStatusAddr = NULL;

		delete [] m_pAxisPositionAddr;	
		m_pAxisPositionAddr = NULL;
	}
}

BOOL CMotorControleXcom::ReadAddressValue(int nAddress, double &dValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;
	if (m_peXcomDevice==NULL) return FALSE;
	
	float fValue = 0.0f;
	if (!m_peXcomDevice->ReadOneData(nAddress, fValue))
	{
		return FALSE;
	}

	dValue = fValue;

	return TRUE;
}

BOOL CMotorControleXcom::ReadAddressValue(int nAddress, long &nValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;
	if (m_peXcomDevice==NULL) return FALSE;

	return m_peXcomDevice->ReadOneData(nAddress, nValue);	
}

BOOL CMotorControleXcom::ReadAddressValue(int nAddress, float &fValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;
	if (m_peXcomDevice==NULL) return FALSE;

	return m_peXcomDevice->ReadOneData(nAddress, fValue);
}

BOOL CMotorControleXcom::WriteAddressValue(int nAddress, float fValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;
	if (m_peXcomDevice==NULL) return FALSE;

	return m_peXcomDevice->WriteOneData(nAddress, fValue);
}

BOOL CMotorControleXcom::WriteAddressValue(int nAddress, double dValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;
	if (m_peXcomDevice==NULL) return FALSE;;

	float fValue = (float)dValue;
	return m_peXcomDevice->WriteOneData(nAddress, fValue);
}

BOOL CMotorControleXcom::WriteAddressValue(int nAddress, long nValue)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;
	if (m_peXcomDevice==NULL) return FALSE;

	return m_peXcomDevice->WriteOneData(nAddress, nValue);
}

BOOL CMotorControleXcom::ReadAddressValue(long nAddress, long *pArrayData, int nArrayCount)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;
	if (m_peXcomDevice==NULL) return FALSE;

	return m_peXcomDevice->ReadMultyData(nAddress, pArrayData, nArrayCount);
}

BOOL CMotorControleXcom::WriteAddressValue(long nAddress, long *pArrayData, int nArrayCount)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;
	if (m_peXcomDevice==NULL) return FALSE;

	return m_peXcomDevice->WriteMultyData(nAddress, pArrayData, nArrayCount);
}

BOOL CMotorControleXcom::ReadAddressValue(long nAddress, float *pArrayData, int nArrayCount)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;
	if (m_peXcomDevice==NULL) return FALSE;

	return m_peXcomDevice->ReadMultyData(nAddress, pArrayData, nArrayCount);
}

BOOL CMotorControleXcom::WriteAddressValue(long nAddress, float *pArrayData, int nArrayCount)
{
	if (!m_bConnected) return FALSE;	
	if (nAddress<0) return FALSE;
	if (m_peXcomDevice==NULL) return FALSE;

	return m_peXcomDevice->WriteMultyData(nAddress, pArrayData, nArrayCount);
}
