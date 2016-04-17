#pragma once

#include "Serial.h"
#include "lightcontrol.h"

class AFX_EXT_CLASS CKSVision_PDSeries : public CLightControl, public CSerial
{
public:
	CKSVision_PDSeries(int nIndex, int nControlType=-1);
	virtual ~CKSVision_PDSeries(void);

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
	int m_nCurrentValue;
	int	m_nCurrentStatus;
};

