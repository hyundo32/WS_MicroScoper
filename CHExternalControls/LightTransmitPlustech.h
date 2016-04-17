// LightTransmitPlustech.h: interface for the CLightTransmitPlustech class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTTRANSMITPLUSTECH_H_INCLUDED_)
#define AFX_LIGHTTRANSMITPLUSTECH_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serial.h"
#include "LightTransmitPlustech_Protocol.h"

class CLightTransmitPlustech : public CSerial
{
public:
	CLightTransmitPlustech();
	virtual ~CLightTransmitPlustech();

public:
	BOOL						OpenControl(void);
	BOOL						OpenControl(int nPort, int nBaud);
	BOOL						CloseControl(void);
	BOOL						IsOpened()						{ return m_bConnected; }
	BOOL						SetLightControlValue();	
	DATA_SubControl_type		*GetTransmitLightValue(int iDrv,int iCh);
	BOOL						SetTransmitLightValue(int iDrv,int iCh,unsigned short volt,unsigned short time);
	DATA_SubConfig_type			*GetTransmitLightConfig(int iDrv,int iCh);
	BOOL						IsTurnOnLamp(BOOL bOn);		
	BOOL						SetReset();
	//BOOL						LoadParameter(CString strPPID);
	//BOOL						SaveParameter(CString strPPID);

protected:
	unsigned short				GetCRC16T(unsigned char *pStart, int len);
	BOOL						MakePacket(unsigned char did, unsigned char cmd, char *pdat, unsigned short dat_len);

protected:
	DATA_Config_type			m_StuDrvConfigType;
	DATA_Control_type			m_StuLightValue;	
	BOOL						m_bConnected;
	int							m_dwPort,m_dwBaud;
};

#endif // !defined(AFX_EXTERNLIGHTCONTROLELPOSTECH_H__28B165BE_71B7_4AF2_A966_BD8694FDD522__INCLUDED_)
