#include "StdAfx.h"
#include "PSD1010Control.h"

CPSD1010Control::CPSD1010Control(int nIndex, int nControlType) : CLightControl(nIndex, nControlType)
{
	m_nPortIndex	= 1;
	m_nBaudRate		= 9600;
	m_bConnected	= FALSE;

	m_nCurrentValue = 0;
	m_nCurrentStatus = 0;
}

CPSD1010Control::~CPSD1010Control(void)
{
	Disconnect();
}


BOOL CPSD1010Control::Connect(const CString& strPort, int nBaud)
{
	if(m_nPortIndex < 1) return FALSE;

	int nPort = _ttoi(strPort);

	CString strProt;
	strProt.Format(_T("COM%d"), nPort);

	if(Open(strProt, nBaud) != ERROR_SUCCESS)
	{
		m_bConnected = FALSE;
		return FALSE;
	}

	m_bConnected = TRUE;
	m_nPortIndex = nPort;
	m_nBaudRate = nBaud;

	return TRUE;
}

void CPSD1010Control::Disconnect()
{
	if (!m_bConnected) return;

	m_bConnected = FALSE;

	Close();
}

BOOL CPSD1010Control::SetLightOn()
{
	if (!m_bConnected) return FALSE;

	return TRUE;
}

BOOL CPSD1010Control::SetLightOff()
{
	if (!m_bConnected) return FALSE;

	return TRUE;
}

BOOL CPSD1010Control::GetLightStatus(int &nValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	nValue = m_nCurrentStatus;

	return TRUE;
}

BOOL CPSD1010Control::SetLightStatus(int nValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	m_nCurrentStatus = nValue;

	return TRUE;
}

BOOL CPSD1010Control::GetLightLevel(int &nValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	nValue = m_nCurrentValue;

	return TRUE;
}

BOOL CPSD1010Control::GetLightLevel(double &dValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	dValue = (double)m_nCurrentValue;

	return TRUE;
}


BOOL CPSD1010Control::SetLightLevel(int nValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	char Data[10];

	sprintf_s(Data, "w%03d%c", nValue, ASCII_CR);

	if(Write(Data, strlen(Data)) != ERROR_SUCCESS) return FALSE;

	m_nCurrentValue = atoi(Data);

	return TRUE;
// 	if(SendData(Data, 5))
// 	{
// 		m_nCurrentValue = atoi(Data);
// 		return TRUE;
// 	}
// 	return FALSE;
}

BOOL CPSD1010Control::SetLightLevel(double dValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	char Data[10];

	sprintf_s(Data, "w%03d%c", (int)dValue, ASCII_CR);

	if(Write(Data, strlen(Data)) != ERROR_SUCCESS) return FALSE;

	m_nCurrentValue = atoi(Data);

	return TRUE;
// 	if(SendData(Data, 5))
// 	{
// 		m_nCurrentValue = atoi(Data);
// 		return TRUE;
// 	}
// 	return FALSE;
}