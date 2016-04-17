#pragma once

#include "Serial.h"
#include "lightcontrol.h"

class CLightTransmitPlustech;

class AFX_EXT_CLASS CPlusTek_STB64C : public CLightControl
{
public:
	CPlusTek_STB64C(int nIndex, int nControlType=-1);
	virtual ~CPlusTek_STB64C(void);

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
	CLightTransmitPlustech* m_pLightControl;

	int		m_nLightLevel;
	int		m_nLightStatus;
};

