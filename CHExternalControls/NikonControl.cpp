#include "StdAfx.h"
#include "NikonControl.h"

CNikonControl::CNikonControl(int nIndex, int nControlType) : CRevolverControl(nIndex, nControlType)
{
}

CNikonControl::~CNikonControl(void)
{
}

BOOL CNikonControl::Connect(const CString& strPort, int nBaud)			
{ 
	int nPort = _ttoi(strPort);

	CString strProt;
	strProt.Format(_T("COM%d"), nPort);

	if(Open(strProt, nBaud) != ERROR_SUCCESS)
	{
		m_bConnected = FALSE;
		return FALSE;
	}

	m_bConnected = TRUE;
	
	return m_bConnected;
// 	m_bConnected = Open(nPort, nBaud); 
// 
// 	return m_bConnected;
}

void CNikonControl::Disconnect()				
{ 
	Close(); 
}

BOOL CNikonControl::Backward()
{ 
	if (!m_bConnected) return FALSE;

	char strData[10];
	sprintf_s(strData, "cRCC%c", ASCII_CR);

	Write(strData, strlen(strData));
	//return SendData(strData, 5); 
	if(Write(strData, strlen(strData)) != ERROR_SUCCESS) return FALSE;

	return TRUE;
}

BOOL CNikonControl::Forward()
{ 
	if (!m_bConnected) return FALSE;

	char strData[10];
	sprintf_s(strData, "cRCW%c", ASCII_CR);

	//return SendData(strData, 5);

	if(Write(strData, strlen(strData)) != ERROR_SUCCESS) return FALSE;

	return TRUE;
}

BOOL CNikonControl::SetPosition(int nPos)
{
	if (!m_bConnected) return FALSE;

	char strData[10];
	sprintf_s(strData, "cRDC%d%c", nPos+1, ASCII_CR);

	//return SendData(strData, 6); 

	if(Write(strData, strlen(strData)) != ERROR_SUCCESS) return FALSE;

	m_nCurrentValue = nPos;

	return TRUE;
}

BOOL CNikonControl::GetPosition(int& nPos)				
{ 
	if (!m_bConnected) return FALSE;

	char strData[10], strRecevie[10];
	sprintf_s(strData, "rRAR%c", ASCII_CR);

	//if (!SendData(strData, 5)) return FALSE;
	if(Write(strData, strlen(strData)) != ERROR_SUCCESS) return FALSE;

	::Sleep(50);

	//ReadDataWaiting();

// 	int nValue = ReadData(strData, 5 );
// 
// 	if (nValue<5) 
// 	{
// 		return FALSE;
// 	}

	if(Read(strData, strlen(strData)) != ERROR_SUCCESS)  return FALSE;

	sprintf_s(strRecevie, "%c", strData[4]);

	nPos = atoi(strRecevie);

	return TRUE;
}