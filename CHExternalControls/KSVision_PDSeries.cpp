#include "StdAfx.h"
#include "KSVision_PDSeries.h"


CKSVision_PDSeries::CKSVision_PDSeries(int nIndex, int nControlType) : CLightControl(nIndex, nControlType)
{
	m_bConnected	= FALSE;
	m_nPortIndex	= 1;
	m_nBaudRate		= 9600;
	m_nCurrentValue = 100;
	m_nCurrentStatus = 0;
}

CKSVision_PDSeries::~CKSVision_PDSeries(void)
{
	Close();
}

BOOL CKSVision_PDSeries::Connect(const CString& strPort, int nBaud)
{
	int nPort = _ttoi(strPort);

	if(nPort < 1) return FALSE;

	if(IsOpen()) Close();
	CString strProt = _T("");

	if(nPort > 9)
	{
		strProt.Format(_T("\\\\.\\\\COM%d"), nPort);
	}
	else
	{
		strProt.Format(_T("COM%d"), nPort);
	}

	if (Open(strProt,NULL) != ERROR_SUCCESS)
	{
		m_bConnected = FALSE;
		return false;
	}
	Setup(CSerial::EBaud9600,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	SetupHandshaking(CSerial::EHandshakeOff);

	m_nPortIndex = nPort;
	m_nBaudRate = nBaud;
	m_bConnected = TRUE;

	//Turn On
	char Data[50];
	memset(&Data[0],0x00,sizeof(Data));
	sprintf_s(Data, "CTON%c%c", ASCII_CR, ASCII_LF);
	Write(Data, strlen(Data));

	return TRUE;
}

void CKSVision_PDSeries::Disconnect()
{
	if (!m_bConnected) return;

	m_bConnected = FALSE;

	Close();
}

BOOL CKSVision_PDSeries::SetLightOn()
{
	if (!m_bConnected) return FALSE;

	return TRUE;
}

BOOL CKSVision_PDSeries::SetLightOff()
{
	if (!m_bConnected) return FALSE;

	return TRUE;
}

BOOL CKSVision_PDSeries::SetLightLevel(int nValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	int nHigh = nValue / 16;
	int nLow = nValue % 16;

	CString hex("0123456789ABCDEF");

	char Data[50];
	memset(&Data[0],0x00,sizeof(Data));
	//sprintf_s(Data, "CT%02x%c%c", nValue, ASCII_CR, ASCII_LF);
	//sprintf_s(Data, "CT%c%c%c%c", hex.GetAt(nHigh), hex.GetAt(nLow), ASCII_CR, ASCII_LF);

	sprintf_s(Data, "C%d%c%c%c%c", (Channel+1), hex.GetAt(nHigh), hex.GetAt(nLow), ASCII_CR, ASCII_LF);

	Write(Data, strlen(Data));

	m_nCurrentValue = nValue;

	return TRUE;
}

BOOL CKSVision_PDSeries::SetLightLevel(double dValue, int Channel)
{
	int nValue = int(dValue+0.5);

	return SetLightLevel(nValue, Channel);
}

BOOL CKSVision_PDSeries::GetLightLevel(int &nValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	nValue = m_nCurrentValue;

	return TRUE;
}

BOOL CKSVision_PDSeries::GetLightLevel(double &dValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	dValue = (double)m_nCurrentValue;

	return TRUE;
}

BOOL CKSVision_PDSeries::SetLightStatus(int nValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	int nHigh = nValue / 16;
	int nLow = nValue % 16;

	CString hex("0123456789ABCDEF");

	char Data[50];
	memset(&Data[0],0x00,sizeof(Data));
	//sprintf_s(Data, "CT%02x%c%c", nValue, ASCII_CR, ASCII_LF);
	//sprintf_s(Data, "CT%c%c%c%c", hex.GetAt(nHigh), hex.GetAt(nLow), ASCII_CR, ASCII_LF);

	if (nValue==0)
		sprintf_s(Data, "H%dOF%c%c", (Channel+1), hex.GetAt(nHigh), hex.GetAt(nLow), ASCII_CR, ASCII_LF);
	else
		sprintf_s(Data, "H%dON%c%c", (Channel+1), hex.GetAt(nHigh), hex.GetAt(nLow), ASCII_CR, ASCII_LF);

	Write(Data, strlen(Data));

	m_nCurrentValue = nValue;

	return TRUE;
}

BOOL CKSVision_PDSeries::GetLightStatus(int &nValue, int Channel)
{
	if (!m_bConnected) return FALSE;

	nValue = m_nCurrentStatus;

	return TRUE;
}