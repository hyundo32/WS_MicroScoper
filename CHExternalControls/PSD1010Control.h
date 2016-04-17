#pragma once
#include "Serial.h"
#include "LightControl.h"

class AFX_EXT_CLASS CPSD1010Control :	public CLightControl, public CSerial
{
public:
	CPSD1010Control(int nIndex, int nControlType=-1);
	virtual ~CPSD1010Control(void);

	virtual BOOL Connect(const CString& strPort, int nBaud=CBR_9600);
	virtual void Disconnect();

	virtual BOOL SetLightLevel(int nValue, int Channel = 0);
	virtual BOOL SetLightLevel(double dValue, int Channel = 0);
	virtual BOOL SetLightOn();
	virtual BOOL SetLightOff();

	virtual BOOL GetLightLevel(int &nValue, int Channel = 0);
	virtual BOOL GetLightLevel(double &dValue, int Channel = 0);

	virtual BOOL SetLightStatus(int nValue, int Channel = 0);
	virtual BOOL GetLightStatus(int &nValue, int Channel = 0);


protected:
	int		m_nCurrentValue;
	int		m_nCurrentStatus;
};
