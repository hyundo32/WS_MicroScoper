#pragma once

#include "Serial.h"
#include "RevolverControl.h"

class AFX_EXT_CLASS CLinearTurretControl : public CSerial, public CRevolverControl
{
public:
	CLinearTurretControl(int nIndex, int nControlType=-1);
	~CLinearTurretControl(void);

	virtual BOOL	Connect(const CString& strPort, int nBaud=CBR_9600);
	virtual void	Disconnect();
	virtual BOOL	Backward();	
	virtual BOOL	Forward();
	virtual BOOL	GetPosition(int& nPos);
	virtual BOOL	SetPosition(int nPos);
	BOOL			GoHoming();

protected:
	int				m_nCurrentValue;
	
};
