#pragma once


#include "MotorControl.h"


class AFX_EXT_CLASS CMotorControlPowerPmacCmd  : public CMotorControl
{
public:
	CMotorControlPowerPmacCmd(int nIndex=0, DWORD dwPeriod=100);
	virtual ~CMotorControlPowerPmacCmd(void);

public:
	// CMotorControl
	virtual int		Connect(const CString& strPort);
	virtual void	Disconnect();
	virtual BOOL	ReadAddressValue(int nAddress, long &nValue);
	virtual BOOL	WriteAddressValue(int nAddress, long nValue);
	virtual BOOL	ReadAddressValue(int nAddress, float &fValue);
	virtual BOOL	WriteAddressValue(int nAddress, float fValue);
	virtual BOOL	ReadAddressValue(int nAddress, double &dValue);
	virtual BOOL	WriteAddressValue(int nAddress, double dValue);

	// CTimerThreadPools
	virtual int		StartThread();

protected:
	virtual BOOL	ReadAddressValue(long nAddress, long *pArrayData, int nArrayCount);
	virtual BOOL	WriteAddressValue(long nAddress, long *pArrayData, int nArrayCount);
	virtual BOOL	ReadAddressValue(long nAddress, float *pArrayData, int nArrayCount);
	virtual BOOL	WriteAddressValue(long nAddress, float *pArrayData, int nArrayCount);


protected:	
	UINT			m_uDeviceID;
};

