#pragma once

#include "RevolverControl.h"

class AFX_EXT_CLASS CDualTubeLens : public CRevolverControl
{
public:
	CDualTubeLens(int nIndex, int nControlType=-1);
	virtual ~CDualTubeLens(void);

	virtual BOOL	Connect(const CString& strPort, int nBaud = CBR_9600);
	virtual void	Disconnect();
	virtual BOOL	Backward();	
	virtual BOOL	Forward();
	virtual BOOL	GetPosition(int& nPos);
	virtual BOOL	SetPosition(int nPos);

protected:

};

