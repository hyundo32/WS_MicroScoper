#pragma once

#include "Serial.h"
#include "LightControl.h"

class AFX_EXT_CLASS CShinHan_FLS2CHR : public CLightControl, public CSerial
{
public:
	CShinHan_FLS2CHR(int nIndex, int nControlType=-1);
	virtual ~CShinHan_FLS2CHR(void);

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
