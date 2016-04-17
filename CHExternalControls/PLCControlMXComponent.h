#pragma once

#include <afxmt.h>
#include "PLCControl.h"
#include "PLCExternalIO_Def.h"
#include "SharedMemoryDefine.h"
#include <vector>

class AFX_EXT_CLASS CPLCControlMXComponent : public CPLCControl
{
public:
	CPLCControlMXComponent(DWORD dwPeriod=100);
	virtual ~CPLCControlMXComponent(void);

public:
	// CTimerThreadPools
	virtual int		StartThread();
	virtual void	StopThread();

	// CPLCControl
	virtual	BOOL	ConnectToPLC();
	virtual	BOOL	ReconnectPLC();
	virtual	BOOL	DisconnectToPLC();
	virtual	BOOL	IsConnectToPLC();

	virtual	BOOL	ReadPLCSignal_01();
	virtual	BOOL	SendPLCSignal_01();

	virtual BOOL	SendPLCSignal_01(short BITOrder, DWORD msecTimer=1000);

	virtual	BOOL	ReadPLCSignal_02();
	virtual	BOOL	SendPLCSignal_02();

	virtual BOOL	SendPLCSignal_02(short BITOrder, DWORD msecTimer=1000);
	
	virtual	BOOL	ResetPLCData();

	virtual	void	SetPLCAlarm(int nMsg, int nData = 0);
	virtual void	SendReviewEnd();
	virtual	BOOL	ReadPLCData(int nAddr, int& nData);
	virtual	BOOL	ReadPLCData(int nAddr, int &nData, int nLen);
	virtual	BOOL	ReadPLCData(int nAddr, CString& strData, int nLen);
	virtual	BOOL	ReadPLCData(char cMemory, int nAddr, CString& strData, int nLen);	
	virtual	BOOL	SendPLCData(int nAddr, int nData);
	virtual	BOOL	SendPLCData(char cDevice, int nAddr, CString& strData, int nLen);
	virtual BOOL	ReadPLCDataFlowHistory(int nAddr, unsigned short nRetData[]);

	BOOL			WriteCStringPLC(TCHAR* strAddress, CString strData);
	BOOL			WriteLongPLC(TCHAR* strAddress, short* WriteData, unsigned int nWORDSize);
	BOOL			WriteWORDPLCTimer(TCHAR* strAddress, short WriteData, DWORD msecTimer);
	BOOL			WriteWORDOneBitTimer(TCHAR* strAddress, short BITOrder, DWORD msecTimer);
	BOOL			WriteWORDManyBitsTimer(TCHAR* strAddress, char* BitFlag, DWORD msecTimer);
	BOOL			ReadLongData(char* pData, TCHAR* strAddress, unsigned short nSize);
	BOOL			ReadLongData(unsigned short* pData, TCHAR* strAddress, unsigned short nWORDSize);

protected:
	// CTimerThreadPools
	virtual void	TimerThreadProcess(PVOID pParameter);

protected:
	HANDLE					m_hMap;
	char*					m_pSharedMemory;
	CEvent					m_ReadEvent;
	CEvent					m_WriteCompleteEvent;

// 	ST_PLC_RECIVE_A2E_01	m_pRcv01;
// 	ST_PLC_SEND_A2E_01		m_pSend01;
// 	ST_PLC_SEND_A2E_01		m_pSentBack01;

	DWORD					m_aliveTimer;

	CRITICAL_SECTION		m_csExit;
	CRITICAL_SECTION		m_csPLCProcess;
};
