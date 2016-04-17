#pragma once

#include <vector>

interface ILightControl2Parent
{
public:
	virtual void LC2P_ChangeLightLevel(int nIndex, int nLevel) = 0;
};

class AFX_EXT_CLASS CLightControl
{
public:
	CLightControl(int nIndex, int nControlType=-1)
	{
		m_nIndex		= nIndex;
		m_pLC2P			= NULL;
		m_nControlType	= nControlType;
		m_nPortIndex	= 1;
		m_nBaudRate		= CBR_9600;
		m_bConnected	= FALSE;
	}

	virtual ~CLightControl(void)
	{
	}

	void	SetLC2P(ILightControl2Parent* pLC2P)			{ m_pLC2P = pLC2P; }

	int		GetControlType()	{ return m_nControlType; }
	int		GetPortIndex()		{ return m_nPortIndex; }
	int		GetBaudRate()		{ return m_nBaudRate; }
	BOOL	GetConnected()		{ return m_bConnected; }
	void	SetDefaultValue(int nValue)	{ m_nDefaultValue = nValue; }
	int		GetDefaultValue()			{ return m_nDefaultValue; }

	virtual BOOL Connect(const CString& strPort, int nBaud=CBR_9600) = 0;
	virtual void Disconnect() = 0;

	virtual BOOL SetLightLevel(int nValue, int Channel = 0) = 0;
	virtual BOOL SetLightLevel(double dValue, int Channel = 0) = 0;
	virtual BOOL GetLightLevel(int &nValue, int Channel = 0) = 0;
	virtual BOOL GetLightLevel(double &dValue, int Channel = 0) = 0;

	virtual BOOL SetLightStatus(int nValue, int Channel = 0) = 0;
	virtual BOOL GetLightStatus(int &nValue, int Channel = 0) = 0;

	virtual BOOL SetLightOn() = 0;
	virtual BOOL SetLightOff() = 0;

		

protected:
	ILightControl2Parent*	m_pLC2P;
	int						m_nIndex;
	int						m_nControlType;

	int						m_nPortIndex;
	int						m_nBaudRate;
	BOOL					m_bConnected;
	int						m_nDefaultValue;
};
typedef std::vector<CLightControl*>				VectorLightControl;
typedef std::vector<CLightControl*>::iterator	VectorLightControlIt;