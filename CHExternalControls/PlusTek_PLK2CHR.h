#pragma once
//////////////////////////////////////////////////////////
//Brightness range : 29~255
//PWM Frequency : 83Khz
//Communication
//- RS232
//- 38400bps
//- 8bit / none parity/1 stop bit/none flow control
//Channel number : 2
//////////////////////////////////////////////////////////

#include "Serial.h"
#include "LightControl.h"

class AFX_EXT_CLASS CPlusTek_PLK2CHR : public CLightControl, public CSerial
{
public:
	CPlusTek_PLK2CHR(int nIndex, int nControlType=-1);
	virtual ~CPlusTek_PLK2CHR(void);

	virtual BOOL Connect(const CString& strPort, int nBaud=CBR_38400);
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
