#include "StdAfx.h"
#include "LinearTurretControl.h"
#include "CommThread.h"

#define		X20_POS		3
#define		X10_POS		4
#define		X05_POS		2
#define		X02_POS		1

CLinearTurretControl::CLinearTurretControl(int nIndex, int nControlType) : CRevolverControl(nIndex, nControlType)
{
	m_nCurrentValue = -1;
}

CLinearTurretControl::~CLinearTurretControl(void)
{

}

BOOL CLinearTurretControl::Connect(const CString& strPort, int nBaud)			
{ 
	int nPort = _ttoi(strPort);

	CString strProt = _T("");
	strProt.Format(_T("COM%d"), nPort);

	if (Open(strProt,NULL) != ERROR_SUCCESS)
	{
		m_bConnected = FALSE;
		return false;
	}
	//	Setup(CSerial::EBaud9600,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	Setup(CSerial::EBaud19200,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	SetupHandshaking(CSerial::EHandshakeOff);

	char strData[10];
	m_bConnected = TRUE;
	sprintf_s(strData, "mo%c", ASCII_CR);

	//if (!SendData(strData, 6)) return FALSE;
	if (Write(strData, strlen(strData)) != ERROR_SUCCESS) return FALSE;

	::Sleep(50);

	if (strData[3] == '0')					// Servo Off ÀÌ¸י,
	{
		GoHoming();
		Sleep(10000);
	}

	return m_bConnected;
}

void CLinearTurretControl::Disconnect()				
{ 
	Close(); 
}

BOOL CLinearTurretControl::SetPosition(int nPos)
{
	if (!m_bConnected) return FALSE;

	int nRealPos = nPos + 1;

	//return SendData(strData, 7); 
	char strData[10];
	sprintf_s(strData, "xq##t%d%c", nRealPos, ASCII_CR);

	if(Write(strData, strlen(strData)) != ERROR_SUCCESS) return FALSE;

	m_nCurrentValue = nPos;

	return TRUE;
}

BOOL CLinearTurretControl::GetPosition(int& nPos)				
{ 
	if (!m_bConnected) return FALSE;

	nPos = m_nCurrentValue;

	return TRUE;
}

BOOL CLinearTurretControl::GoHoming()
{
	if (!m_bConnected) return FALSE;

	char strData[10];
	
	sprintf_s(strData, "kl%c", ASCII_CR);
	//BOOL bResult = SendData(strData, 3);
	//if(!bResult) return FALSE;
	if(Write(strData, strlen(strData)) != ERROR_SUCCESS) return FALSE;

	sprintf_s(strData, "xq##gh%c", ASCII_CR);

	//return SendData(strData, 7);
	if(Write(strData, strlen(strData)) != ERROR_SUCCESS) return FALSE;

	return TRUE;
}

BOOL CLinearTurretControl::Backward()
{
	return FALSE;
}

BOOL CLinearTurretControl::Forward()
{
	return FALSE;
}