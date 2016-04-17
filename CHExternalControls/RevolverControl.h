#pragma once

#include <vector>

interface IRevolverControl2Parent
{
public:
	virtual void RC2P_ChangePosition(int nIndex, int nPos) = 0;
};

class AFX_EXT_CLASS CRevolverControl
{
public:
	CRevolverControl(int nIndex, int nControlType=-1)
	{
		m_nIndex		= nIndex;
		m_pRC2P			= NULL;
		m_nControlType	= nControlType;
		m_nPortIndex	= 1;
		m_nBaudRate		= CBR_9600;
		m_bConnected	= FALSE;
	}

	virtual ~CRevolverControl(void)
	{
	}

	void	SetRC2P(IRevolverControl2Parent* pRC2P)			{ m_pRC2P = pRC2P; }

	int		GetControlType()	{ return m_nControlType; }
	int		GetIndex()			{ return m_nIndex; }
	int		GetPortIndex()		{ return m_nPortIndex; }
	int		GetBaudRate()		{ return m_nBaudRate; }
	BOOL	GetConnected()		{ return m_bConnected; }

	virtual BOOL	Connect(const CString& strPort, int nBaud = CBR_9600) = 0;
	virtual void	Disconnect() = 0;
	virtual BOOL	Backward() = 0;	
	virtual BOOL	Forward() = 0;
	virtual BOOL	GetPosition(int& nPos) = 0;
	virtual BOOL	SetPosition(int nPos) = 0;

protected:
	IRevolverControl2Parent		*m_pRC2P;
	int							m_nIndex;
	int							m_nControlType;
	int							m_nPortIndex;
	int							m_nBaudRate;
	BOOL						m_bConnected;

};
typedef std::vector<CRevolverControl*>				VectorRevolverControl;
typedef std::vector<CRevolverControl*>::iterator	VectorRevolverControlIt;