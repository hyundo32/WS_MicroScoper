#pragma once

#include <vector>

#define MAX_MOTOR_ADDRESS_SIZE	100
#define	LIMIT_NONE				-999999
#define ADDRESS_NONE			-1

// CMotorControleXcom
typedef std::vector<long>				VectorLong;
typedef std::vector<long>::iterator		VectorLongIt;
typedef std::vector<int>				VectorInteger;
typedef std::vector<int>::iterator		VectorIntegerIt;
typedef std::vector<double>				VectorDouble;
typedef std::vector<double>::iterator	VectorDoubleIt;

class AFX_EXT_CLASS CMotorAxis
{
public:
	CMotorAxis()			{ Reset(); }
	virtual ~CMotorAxis()	{ Reset(); }

	void Reset()
	{
		nAxisIdx		= -1;
		nGantryIdx		= -1;
		
		lStatus			= 0;
		dPosition		= 0;
		dLimitPlus		= LIMIT_NONE;
		dLimitMinus		= LIMIT_NONE;
		dJogSpeedMin	= 0.0;
		dJogSpeedMax	= 10.0;
		
		nPositionAddr	= ADDRESS_NONE;
		nStatusAddr		= ADDRESS_NONE;
		nGoPositionAddr	= ADDRESS_NONE;
		nJogCommandAddr	= ADDRESS_NONE;

		nGoSpeedAddr	= ADDRESS_NONE;
		nGoAccelAddr	= ADDRESS_NONE;
		nJogSpeedAddr	= ADDRESS_NONE;
		nJogAccelAddr	= ADDRESS_NONE;
	}

	// Axis Info
	int			nAxisIdx;
	int			nGantryIdx;

	long 		lStatus;
	double		dPosition;
	double		dLimitPlus;
	double		dLimitMinus;
	double		dJogSpeedMin;
	double		dJogSpeedMax;
	
	// Axis Addr
	int			nPositionAddr;
	int			nStatusAddr;
	int			nGoPositionAddr;
	int			nJogCommandAddr;

	int			nGoSpeedAddr;
	int			nGoAccelAddr;
	int			nJogSpeedAddr;
	int			nJogAccelAddr;
};

typedef std::vector<CMotorAxis>						VectorMotorAxis;
typedef std::vector<CMotorAxis>::iterator			VectorMotorAxisIt;

class AFX_EXT_CLASS CMotorGantry
{
public:
	CMotorGantry()			{ Reset(); }
	virtual ~CMotorGantry()	{ Reset(); }

	void Reset()
	{
		nGantryIdx = -1;
		nManualGoAddr				= ADDRESS_NONE;

		nAutoGoAddr					= ADDRESS_NONE;
		nMoveCountAddr				= ADDRESS_NONE;
		nTriggerCountAddr			= ADDRESS_NONE;

		nMeasureAutoGoAddr			= ADDRESS_NONE;
		nMeasureMoveCountAddr		= ADDRESS_NONE;
		nMeasureTriggerCountAddr	= ADDRESS_NONE;

		nOtherAutoGoAddr			= ADDRESS_NONE;
		nOtherMoveCountAddr			= ADDRESS_NONE;
		nOtherTriggerCountAddr		= ADDRESS_NONE;

		vectorMotorAxis.clear();
	}

	int			nGantryIdx;
	int			nManualGoAddr;
	
	int			nAutoGoAddr;
	int			nMoveCountAddr;
	int			nTriggerCountAddr;

	int			nMeasureAutoGoAddr;
	int			nMeasureMoveCountAddr;
	int			nMeasureTriggerCountAddr;

	int			nOtherAutoGoAddr;
	int			nOtherMoveCountAddr;
	int			nOtherTriggerCountAddr;

	VectorMotorAxis vectorMotorAxis;
};
typedef std::vector<CMotorGantry>			VectorMotorGantry;
typedef std::vector<CMotorGantry>::iterator	VectorMotorGantryIt;

class AFX_EXT_CLASS CMotorCommand
{
public:
	CMotorCommand()				{ Reset(); }
	virtual ~CMotorCommand()	{ Reset(); }

	void Reset()
	{
		nAutoStop			= ADDRESS_NONE;
		nAllGoAddr			= ADDRESS_NONE;
		nAllMeasureGoAddr	= ADDRESS_NONE;
		nAllWsiGoAddr		= ADDRESS_NONE;

		nPrevDelayTimeAddr	= ADDRESS_NONE;
		nPostDelayTimeAddr	= ADDRESS_NONE;
		nOtherDelayTimeAddr = ADDRESS_NONE;

		nPrevDelayTime		= 150;
		nPostDelayTime		= 50;
		nOtherDelayTime		= 2000;

		bUseThetaAxis		= 0;
		nAxisThetaManualGoAddr = ADDRESS_NONE;
		axisThetaAddr.Reset();

		vectorMotorGantry.clear();
	}

	int					nAutoStop;
	int					nAllGoAddr;
	int					nAllMeasureGoAddr;
	int					nAllWsiGoAddr;

	int					nPrevDelayTimeAddr;
	int					nPostDelayTimeAddr;
	int					nOtherDelayTimeAddr;

	int					nPrevDelayTime;
	int					nPostDelayTime;
	int					nOtherDelayTime;

	// theta motor
	int					bUseThetaAxis;
	int					nAxisThetaManualGoAddr;
	CMotorAxis			axisThetaAddr;
	
	VectorMotorGantry	vectorMotorGantry;
};

interface IMotorControl2Parent
{
	virtual void MC2P_DisplayMessage(const TCHAR* lpstrFormat, ...) = 0;
	virtual void MC2P_AxisMotionEnd(int nAxisIdx, BOOL bMotionEnd) = 0;
	virtual void MC2P_GantryMotionEnd(int nGantryIdx, BOOL bMotionEnd) = 0;
	virtual void MC2P_UpdatePosition(int nIndex, int nAxisIdx, double dPosition) = 0;	
	virtual void MC2P_UpdateStatus(int nIndex, int nAxisIdx, long lStatus) = 0;
	virtual void MC2P_UpdateThetaPosition(int nIndex, double dPosition) = 0;	
	virtual void MC2P_UpdateThetaStatus(int nIndex, long lStatus) = 0;	
};

class CMotionEndThreadData
{
public:
	CMotionEndThreadData() 
	{
		bRunning = false;
		nDelayTime = 100;
		nWaitTime = 5000; //ms
		nAxisIndex = -1;
		nGantryIndex = -1;
		pThisPtr = NULL;
	}

	bool	bRunning;
	DWORD	nWaitTime;
	DWORD	nDelayTime;
	int		nAxisIndex;
	int		nGantryIndex;
	void*	pThisPtr;
};

#include "TimerThreadPools.h"

class AFX_EXT_CLASS CMotorControl : public CTimerThreadPools
{
public:
	CMotorControl(int nIndex=0, DWORD dwPeriod=100);
	virtual ~CMotorControl(void);

public:
	// CMotorControl
	virtual int		Connect(const CString& strPort) = 0;
	virtual void	Disconnect() = 0;
	virtual BOOL	ReadAddressValue(int nAddress, long &nValue) = 0;
	virtual BOOL	WriteAddressValue(int nAddress, long nValue) = 0;
	virtual BOOL	ReadAddressValue(int nAddress, float &fValue) = 0;
	virtual BOOL	WriteAddressValue(int nAddress, float fValue) = 0;
	virtual BOOL	ReadAddressValue(int nAddress, double &dValue) = 0;
	virtual BOOL	WriteAddressValue(int nAddress, double dValue) = 0;
	

	// CTimerThreadPool
	virtual BOOL	StartThread() = 0;
	virtual void	StopThread();

	void SetMC2P(IMotorControl2Parent* pMC2P)						{ m_pMC2P = pMC2P; }
	BOOL GetConnected()												{ return m_bConnected; }
	CMotorCommand GetMotorCommandAddr()								{ return m_motorCommand; }
	void SetMotorCommandAddr(const CMotorCommand& motorCommand)		{ m_motorCommand = motorCommand; }
			
	// common
	BOOL	CommonSetAFMDelayTime(long nTime = 0);
	BOOL	CommonSetSnapDelayTime(long nTime = 0);
	BOOL	CommonSetWSIDelayTime(long nTime = 0);
	BOOL	CommonSetAutoGoAll();
	BOOL	CommonSetAutoMeasureGoAll();
	BOOL	CommonSetAutoWsiGoAll();

	BOOL	CommonGetAFMDelayTime(long& nTime);
	BOOL	CommonGetSnapDelayTime(long& nTime);
	BOOL	CommonGetWSIDealyTime(long& nTime);
	BOOL	CommonSetAutoStop();

	// axis 
	BOOL	AxisManualGoEnd(int nAxisIdx, double dPosition);
	BOOL	AxisManualGo(int nAxisIdx, double dPosition);
	BOOL	AxisManualGo(const VectorInteger& vectorAxis, const VectorDouble& vectorPosition);
	BOOL	AxisJogPlus(int nAxisIdx);
	BOOL	AxisJogMinus(int nAxisIdx);
	BOOL	AxisJogStop(int nAxisIdx);
	BOOL	AxisManualGoSpeed(int nAxisIdx, double dSpeed);
	BOOL	AxisJogSpeed(int nAxisIdx, double dSpeed);
	BOOL	AxisManualGoAccel(int nAxisIdx, int nAccel);
	BOOL	AxisJogAccel(int nAxisIdx, int nAccel);
	BOOL	IsAxisMotionEnd(int nAxisIdx);

	// axis theta
	BOOL	AxisThetaManualGoEnd(double dPosition);
	BOOL	AxisThetaManualGo(double dPosition);
	BOOL	AxisThetaJogPlus();
	BOOL	AxisThetaJogMinus();
	BOOL	AxisThetaJogStop();
	BOOL	AxisThetaManualGoSpeed(double dSpeed);
	BOOL	AxisThetaJogSpeed(double dSpeed);
	BOOL	AxisThetaManualGoAccel(int nAccel);
	BOOL	AxisThetaJogAccel(int nAccel);
	BOOL	IsAxisThetaMotionEnd();

	// gantry
	BOOL	GantryManualGoEnd(int nGantryIdx, const VectorDouble& vectorPos);
	BOOL	GantryManualGoEnd(int nGantryIdx, const VectorDouble& vectorPos, int nMoveType);
	BOOL	GantryManualGoEnd(int nGantryIdx, double dPosX, double dPosY);
	BOOL	GantryManualGo(int nGantryIdx, const VectorDouble& vectorPos);
	BOOL	GantryManualGo(int nGantryIdx, const VectorDouble& vectorPos, int nMoveType);
	BOOL	GantryManualGo(int nGantryIdx, double dPosX, double dPosY);

	// review 
	BOOL	GantryAutoGo(int nGantryIdx, const VectorDouble& vectorPosX, const VectorDouble& vectorPosY, BOOL bCommand=FALSE);
	BOOL	GantryAutoGo(int nGantryIdx);
	BOOL	GantrySetMoveCount(int nGantryIdx, long nCount);
	BOOL	GantryGetMoveCount(int nGantryIdx, long& nCount);
	BOOL	GantryGetTriggerCount(int nGantryIdx, long& nCount);

	// measure
	BOOL	GantryMeasureAutoGo(int nGantryIdx, const VectorDouble& vectorPosData, BOOL bCommand=FALSE);
	BOOL	GantryMeasureAutoGo(int nGantryIdx);
	BOOL	GantrySetMeasureMoveCount(int nGantryIdx, long nCount);
	BOOL	GantryGetMeasureMoveCount(int nGantryIdx, long& nCount);
	BOOL	GantryGetMeasureTriggerCount(int nGantryIdx, long& nCount);

	// wsi 
	BOOL	GantryOtherAutoGo(int nGantryIdx, const VectorDouble& vectorPosX, const VectorDouble& vectorPosY, BOOL bCommand=FALSE);
	BOOL	GantryOtherAutoGo(int nGantryIdx);
	BOOL	GantrySetOtherMoveCount(int nGantryIdx, long nCount);
	BOOL	GantryGetOtherMoveCount(int nGantryIdx, long& nCount);
	BOOL	GantryGetOtherTriggerCount(int nGantryIdx, long& nCount);

	BOOL	IsGantryMotionEnd(int nGantryIdx);
	BOOL	IsMotionEnd();

	BOOL	RemoveAllAutoGo(int nGantryIdx);

	BOOL	GetAxisLimitPosition(int nAxisIdx, double& dPlusLimit, double& dMinusLimit);

	static	UINT MotionEndThread_Func(LPVOID pParam);
	void	MotionEndThread_Func(CMotionEndThreadData *pData);

protected:
	// CTimerThreadPool
	virtual void TimerThreadProcess(PVOID pParameter);

	virtual BOOL	ReadAddressValue(long nAddress, long *pArrayData, int nArrayCount) = 0;
	virtual BOOL	WriteAddressValue(long nAddress, long *pArrayData, int nArrayCount) = 0;
	virtual BOOL	ReadAddressValue(long nAddress, float *pArrayData, int nArrayCount) = 0;
	virtual BOOL	WriteAddressValue(long nAddress, float *pArrayData, int nArrayCount) = 0;

	// axis
	int		GetAxisGantryIndex(int nAxisIdx);
	int		GetAxisPositionAddr(int nAxisIdx);
	int		GetAxisStatusAddr(int nAxisIdx);
	int		GetAxisGoPositionAddr(int nAxisIdx);
	int		GetAxisJogCommandAddr(int nAxisIdx);
	int		GetAxisManualGoSpeedAddr(int nAxisIdx);
	int		GetAxisManualGoAccelAddr(int nAxisIdx);
	int		GetAxisJogSpeedAddr(int nAxisIdx);
	int		GetAxisJogAccelAddr(int nAxisIdx);

	// theta axis
	int		GetAxisThetaPositionAddr();
	int		GetAxisThetaStatusAddr();
	int		GetAxisThetaGoPositionAddr();
	int		GetAxisThetaJogCommandAddr();
	int		GetAxisThetaManualGoAddr();
	int		GetAxisThetaManualGoSpeedAddr();
	int		GetAxisThetaManualGoAccelAddr();
	int		GetAxisThetaJogSpeedAddr();
	int		GetAxisThetaJogAccelAddr();

	// gantry
	int		GetGantryAxisCount(int nGantryIdx);
	int		GetGantryManualGoAddr(int nGantryIdx);

	int		GetGantryAutoGoAddr(int nGantryIdx);
	int		GetGantryMoveCountAddr(int nGantryIdx);
	int		GetGantryTriggerCountAddr(int nGantryIdx);

	int		GetGantryMeasureAutoGoAddr(int nGantryIdx);
	int		GetGantryMeasureMoveCountAddr(int nGantryIdx);
	int		GetGantryMeasureTriggerCountAddr(int nGantryIdx);

	int		GetGantryOtherAutoGoAddr(int nGantryIdx);
	int		GetGantryOtherMoveCountAddr(int nGantryIdx);
	int		GetGantryOtherTriggerCountAddr(int nGantryIdx);

	static BOOL	TransformIP_CStringToDWORD(const CString& strServerIP, DWORD& dwServerIP);

protected:
	IMotorControl2Parent*	m_pMC2P;
	int						m_nIndex;
	BOOL					m_bConnected;
	CMotorCommand			m_motorCommand;

	// var
	int						m_nAxisCount;
	
	long					*m_pAxisStatus;
	float					*m_pAxisPosition;
	long					*m_pAxisStatusPrev;
	float					*m_pAxisPositionPrev;

	long					*m_pAxisStatusAddr;
	long					*m_pAxisPositionAddr;


	//  axis theta
	long					m_axisThetaStatus;
	long					m_axisThetaStatusPrev;
	float					m_axisThetaPosition;
	float					m_axisThetaPositionPrev;

	CString					m_strPort;
	int						m_nDelayCount;

	CMotionEndThreadData	m_MotionEndThreadData;
};
