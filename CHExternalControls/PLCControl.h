#pragma once

#include "TimerThreadPools.h"

interface IPLCControl2Parent
{
public:
	virtual void PLC2P_SetPLCSignal(int nCommandIdx) = 0;	
	virtual void PLC2P_SetPLCStatus(int nReadSignal, int nWriteSignal) = 0;	
	virtual void PLC2P_SetPLCSubSignal(int nCommandIdx) = 0;
};

class AFX_EXT_CLASS CPLCControl : public CTimerThreadPools
{
public:
	CPLCControl(DWORD dwPeriod=100);
	virtual ~CPLCControl(void);
	void SetParents(CWnd *pWnd)					{ m_pParents = pWnd; }
	void SetPLC2P(IPLCControl2Parent* pPLC2P)	{ m_pPLC2P = pPLC2P; }
	void SetDelayTime(int nDelayTime)			{ m_nDelayTime = nDelayTime; }
	void SetSendAddress(int nAddress)			{ m_nSendAddress = nAddress; }

public:
	// CTimerThreadPools
	virtual BOOL	StartThread() = 0;
	virtual void	StopThread() = 0;

	// CPlcControl
	virtual	BOOL	ReadPLCSignal_01() = 0;
	virtual	BOOL	SendPLCSignal_01() = 0;
	virtual BOOL	SendPLCSignal_01(short BITOrder, DWORD msecTimer=1000) = 0;

	virtual	BOOL	ReadPLCSignal_02() = 0;
	virtual	BOOL	SendPLCSignal_02() = 0;
	virtual BOOL	SendPLCSignal_02(short BITOrder, DWORD msecTimer=1000) = 0;
	
	virtual	BOOL	ResetPLCData() = 0;
	virtual	void	SetPLCAlarm(int nMsg,int nData=0) = 0;
	virtual void	SendReviewEnd() = 0;


	virtual	BOOL	ConnectToPLC() = 0;
	virtual	BOOL	ReconnectPLC() = 0;
	virtual	BOOL	DisconnectToPLC() = 0;
	virtual	BOOL	IsConnectToPLC() = 0;

	virtual	BOOL	ReadPLCData(int nAddr, int& nData) = 0;
	virtual	BOOL	ReadPLCData(int nAddr, int &nData, int nLen) = 0;
	virtual	BOOL	ReadPLCData(int nAddr, CString& strData, int nLen) = 0;
	virtual	BOOL	ReadPLCData(char cMemory, int nAddr, CString& strData, int nLen) = 0;	
	virtual BOOL	ReadPLCDataFlowHistory(int nAddr, unsigned short nRetData[]) = 0;

	virtual	BOOL	SendPLCData(int nAddr, int nData) = 0;
	virtual	BOOL	SendPLCData(char cDevice, int nAddr, CString& strData, int nLen) = 0;

protected:
	// CTimerThreadPools
	virtual void TimerThreadProcess(PVOID pParameter) = 0;

	int				MakePLCString(char *Destination, unsigned char *Source, int Size);
	CString			GetDataWord2Str(CString& value, int& addr, int length);
	int				Hex2Dec(const CString& hex_str);

protected:
	CWnd*				m_pParents;
	BOOL				m_bPLCConnected;
	
	int					m_nPrevPLCSignal_01;	
	int					m_nReadPLCSignal_01;

	int					m_nWPrevPLCSignal_01;	
	int					m_nWReadPLCSignal_01;

	int					m_nPrevPLCSignal_02;	
	int					m_nReadPLCSignal_02;

	int					m_nWPrevPLCSignal_02;	
	int					m_nWReadPLCSignal_02;

	int					m_nAliveSignalCnt;

	int					m_nDelayTime;
	int					m_nSendAddress;

	IPLCControl2Parent* m_pPLC2P;
};
