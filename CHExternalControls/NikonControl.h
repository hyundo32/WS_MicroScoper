#pragma once

#include "Serial.h"
#include "RevolverControl.h"

class AFX_EXT_CLASS CNikonControl : public CSerial
					, public CRevolverControl	
{
public:
	CNikonControl(int nIndex, int nControlType=-1);
	virtual ~CNikonControl(void);

	virtual BOOL	Connect(const CString& strPort, int nBaud=CBR_9600);
	virtual void	Disconnect();

	virtual BOOL	GetPosition(int& nPos);
	virtual BOOL	SetPosition(int nPos);
	virtual BOOL	Backward();	
	virtual BOOL	Forward();

protected:
	int				m_nCurrentValue;

};
