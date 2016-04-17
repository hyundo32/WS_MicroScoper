/******************************************************************************
 *	
 *	(C) Copyright WDI 2010
 *	
 ******************************************************************************
 *
 *	FILE:		llc.h
 *
 *	PROJECT:	AFT Sensor
 *
 *	SUBPROJECT:	.
 *
 *	Description: LLC interface
 *	
 ******************************************************************************
 *	
 *	Change Activity
 *	Defect  Date       Developer        Description
 *	Number  DD/MM/YYYY Name
 *	======= ========== ================ =======================================
 *          28/01/2011 Chris O.         Initial version
 *
 ******************************************************************************/

#pragma once

#ifdef __cplusplus

#include "epp.h"
#include "logger.h"

// number of pulses per mm in LLC encoder
#define ENC_PULSE_PER_MM 10000	
// max number of objectives
#define MAX_LLC_OBJ_NUM 8

// LLC error codes
enum LlcErrCode
{
	LlcErrOK = 0,					// 0
// LLC not initilaized error
	LlcErrNotInitialized,			// 1
// LLC bad argument error
	LlcErrArgRange,					// 2
// communication errors
	LlcErrStatus,					// 3
	LlcErrSendFailure,				// 4
	LlcErrSensorCom,				// 5
	LlcErrCmndSyntax,				// 6
	LlcErrRecvLen,					// 7
	LlcErrRecvSyntax,				// 8
	LlcErrWhistleEC,				// 9
//	number of objectives error
	LlcErrObjectiveNum,				// 10
// offset file error
	LlcErrOffsetFile,				// 11
// homing failure error
	LlcErrHoming,					// 12
// move to target error
	LlcErrTargetPos,				// 13
// movement timeout
	LlcErrTimeout,					// 14
// hardware error
	LlcErrRecoveryFailed,			// 15
	LlcErrLimitSwitch,				// 16
	LlcErrMotorDisabled,			// 17
// hardware failure
	LlcErrPeakCurrExceeded,			// 18
	LlcErrInhibit,					// 19
	LlcErrSpeedTracking,			// 20
	LlcErrPositionTracking,			// 21
	LlcErrUnderVoltage,				// 22
	LlcErrOverVoltage,				// 23
	LlcErrShortCircuit,				// 24
	LlcErrOverheating,				// 25
	LlcErrElectricalZero,			// 26
	LlcErrSpeedLimit,				// 27
	LlcErrMotorStuck,				// 28
	LlcErrPositionlimit,			// 29
	LlcErrStartMotor,				// 30
// MFC firmware failure
	LlcErrStackOverflow,			// 31
	LlcErrCpuException				// 32
};

// SR - status register flags (received with "SR;" command)
#define LLC_SR_DR_FLAG		0x00000001	// drive error    (the same as in MF register shown below) - is ON when any of four shown below present
#define LLC_SR_DRUV_FLAG	0x00000003	// under voltage  (the same as in MF register shown below)
#define LLC_SR DROV_FLAG	0x00000005	// over voltage   (the same as in MF register shown below)
#define LLC_SR_DRSC_FLAG	0x0000000B	// short circuit  (the same as in MF register shown below)
#define LLC_SR_DROH_FLAG	0x0000000D	// overheating    (the same as in MF register shown below)
#define LLC_SR_MO_FLAG		0x00000010	// motor enable / disable status - can be obtain with "MO;" command too
#define LLC_SR_MF_FLAG		0x00000040	// motor failure - can be obtain with "MF;" command too
#define LLC_SR_MS_FLAG		0x0000C000	// motion status [00 = In Position; 01 = Off Position; 10 = In motion] - can be obtained with "MS;" command too
#define LLC_SR_PR_FLAG		0x00001000	// program running
#define LLC_SR_SL_FLAG		0x10000000	// limit switches
// MF - motor failure register flags (received with "MF;" command)
#define LLC_MF_PC_FLAG		0x00000008	// peak current exceeded
#define LLC_MF_IN_FLAG		0x00000010	// inhibit
#define LLC_MF_ST_FLAG		0x00000080	// speed tracking error
#define LLC_MF_PS_FLAG		0x00000100	// position tracking error
#define LLC_MF_DR_FLAG		0x00001000	// drive error    (the same as in SR register shown above) - is ON when any of four shown below present
#define LLC_MF_DRUV_FLAG	0x00003000	// under voltage  (the same as in SR register shown above)
#define LLC_MF_DROV_FLAG	0x00005000	// over voltage   (the same as in SR register shown above)
#define LLC_MF_DRSC_FLAG	0x0000B000	// short circuit  (the same as in SR register shown above)
#define LLC_MF_DROH_FLAG	0x0000D000	// overheating    (the same as in SR register shown above)
#define LLC_MF_EZ_FLAG		0x00010000	// electrical zero failure
#define LLC_MF_SL_FLAG		0x00020000	// speed limit failure
#define LLC_MF_SO_FLAG		0x00040000	// stack overflow
#define LLC_MF_CE_FLAG		0x00080000	// CPU exception
#define LLC_MF_MS_FLAG		0x00200000	// motor stuck
#define LLC_MF_PL_FLAG		0x00400000	// position limit exceeded
#define LLC_MF_SM_FLAG		0x20000000	// motor start failure


// LLC class
class CLlc
{
public:
	// constructor & destructor
	CLlc(CLogger *pLogger = NULL)
	{
		// initialize
		m_pLogger = pLogger;
		m_bIsLlcInit = false;
		m_iObjNum = 0;
//		m_iObjPitch = 0;
		m_cOffsetFilePath[0] = '\0';
		m_cForcerSerNum[0] = '\0';
		for (int i = 0; i < MAX_LLC_OBJ_NUM; i++)
		{
			m_fObjPitch[i] = 0.0f;
		}
	}

	~CLlc() {}

private:
	CLogger *m_pLogger;					// pointer to logger object
	bool m_bIsLlcInit;					// is LLC initialized flag
	bool m_bIsHomeDetImpl;				// is ET[5] home detection implemented flag
	char m_cForcerSerNum[16];			// forcer serial number - ET[4]
	char m_cOffsetFilePath[MAX_PATH];	// objective movement offsets file name
	int m_iObjNum;						// number of available objectives
//	int m_iObjPitch;					// distance between objectives
	float m_fObjPitch[MAX_LLC_OBJ_NUM];	// distance between objectives

public:
	bool initializeLlc(char *pOffsetFileDir, int *piErrCode);
	void setObjectiveNum(int iObjNum);
	int getObjectiveNum();
	void setObjectivePitch(float fObjPitch[MAX_LLC_OBJ_NUM]);
	void getObjectivePitch(float fObjPitch[MAX_LLC_OBJ_NUM]);
	int getLlcForcerSerNum();
	int sendLlc(char *toSend, char *toRecv);
	bool getLlcStatus(int *piErrCode, const char **ppErrMsg, int *piStatReg, int *piMtrFailureReg);
	bool recoverLlc(int *piErrCode);
	bool recoverLlcSensorConnection(int *piErrCode);
	bool recoverLlcHdwr(int *piErrCode);
	bool resetLlcController(int *piErrCode);
	bool homeLlc(bool bWaitForEnd, int *piErrCode);
	bool isLlcInHomePos(int *piErrCode);
	bool saveLlcObjectivePositionOffset(int iObjectiveNumber, int iOffset);
	bool getLlcObjectivePositionOffset(int iObjectiveNumber, int *piPositionOffset);
	bool moveLlcToObjective(int iObjectiveNumber, bool bWaitForEnd, int *piErrCode);
	bool moveLlcToPosition(int iPosition, bool bWaitForEnd, int *piErrCode);
	bool moveLLcRelativeDistance(int iDistance, bool bWaitForEnd, int *piErrCode);
	bool isLlcInTargetPosition(int *piErrCode);
	bool enableLlcMotor(int *piErrCode);
	bool disableLlcMotor(int *piErrCode);
	bool stopLlcMotion(int *piErrCode);
	bool setLlcSpeed(int iLlcSpeed, int *piErrCode);
	bool getLlcSpeed(int *piLlcSpeed, int *piErrCode);
	bool setLlcAcceleration(int iLlcAcceleration, int *piErrCode);
	bool getLlcAcceleration(int *piLlcAcceleration, int *piErrCode);
	bool setLlcDeceleration(int iLlcDeceleration, int *piErrCode);
	bool getLlcDeceleration(int *piLlcDeceleration, int *piErrCode);
	bool setLlcSmoothFactor(int iLlcSmoothFactor, int *piErrCode);
	bool getLlcSmoothFactor(int *piLlcSmoothFactor, int *piErrCode);
	bool getLlcVelocity(float *pfLlcVelocity, int *piErrCode);
	bool getLlcPosition(float *pfLlcPosition, int *piErrCode);
	bool getLlcMotorCurrent(float *pfLlcMotorCurrent, int *piErrCode);
	inline void setLlcLogger(CLogger *pLogger) {m_pLogger = pLogger;}

private:
	bool createLlcOffsetFile(char *pOffsetFileDir);
	void saveToLog(const char *pLogMsg, CLogger::LogLevels iLogLevel);
	void saveToLog2(CLogger::LogLevels iLogLevel, char *pFormat, ...);

// logging macros
#define LLCENTERFUN(funname) saveToLog2(CLogger::LogLow, "%s%s", funname, " - entered\n");
#define LLCEXITFUN(funname) saveToLog2(CLogger::LogLow, "%s%s", funname, " - exited\n");
#define LLCLOGMSG(pLogMsg, LogLev) saveToLog(pLogMsg, LogLev);
#define LLCLOGMSG2(LogLev, format, ...) saveToLog2(LogLev, format, __VA_ARGS__);
};

#endif
