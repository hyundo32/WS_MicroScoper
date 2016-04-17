/******************************************************************************
 *	
 *	(C) Copyright WDI 2010
 *	
 ******************************************************************************
 *
 *	FILE:		atf_lib.h
 *
 *	PROJECT:	AFT Sensors
 *
 *	SUBPROJECT:	.
 *
 *	Description: ATF sensor communication library
 *	
 ******************************************************************************
 *	
 *	Change Activity
 *	Defect  Date       Developer        Description
 *	Number  DD/MM/YYYY Name
 *	======= ========== ================ =======================================
 *          21/09/2009 Andrew L.        Initial version 
 *          03/11/2010 Chris O.          
 *          15/11/2010 Chris O.         Added synchronization objects for
 *                                      multithreaded applications
 *                                      
 *****************************************************************************/

// atf_lib.h

#pragma once

#include "atf_def.h"
#include "epp.h"
#include "logger.h"
#include "llc.h"

#ifdef __cplusplus
	extern "C" {
#endif

// ---------------------------------------------------------------------------------
// error codes returned by application

// returns text name of most recent error
const char* atf_GetErrorInfo(int iErr);

// establishing connection. Return 0 if OK, or something else in case of error
int atf_OpenConnection (char *port, int speed);
void atf_CloseConnection (void);
// if multiple atf sensors are opend atf_OpenConnection allocates a new 
// communication object. This index can be read and set in case switching 
// between objects is needed. Conm index is a thread local, therefore if multiple
// threads call atf_OpenConnection each by default is using different com object.
// Using atf_GetComIdx/atf_SetComIdx is only necessary if a single thread 
// communicates with multiple objects. in this case com index needs to be set when
// object is being changed
int atf_GetComIdx();	// returns index
int atf_SetComIdx(int idx);		// returns ErrOK if index is valid or error otherwise

// serial Com port baudrate
int atf_ChangeCommBaudrate(DWORD dwBaudRate);
int atf_GetCommBaudrate(DWORD *dwBaudrate);
bool atf_isSerialConnection();

// read current sensor baudrate setting
int atf_ReadBaud (int &br);
// writes new baudrate setting (need to follow up with change of port setting if communication is to continue) 
int atf_WriteBaud (int br);

// attempts to recover in case of communication loss
// verify what is running. Returns:
// 0 - no communnication could be established
// 1 - application found
// 2 - bootloader found, it was restarted back to application
int atf_RecoverSensor();
// check if "A" is returned which indicate that software is working
int atf_PingAck();
// resets sensor
int atf_Reset();

// direct access to read abd write
int atf_ReadArray  (void *to,   int bid, int pid, EppDataType tag, int num, int offset);
int atf_WriteArray (void *from, int bid, int pid, EppDataType tag, int num, int offset);

// part identification -------------------------------------------------------------------
//
// returns text corresponding to 
const char* atf_PartInfo();

int atf_ReadSerialNumber(unsigned int* SensorSN);
int atf_ReadFirmwareVer(int* ver);

// one time initialization ---------------------------------------------------------------
// read all possible magnifications to choose from. Up to 16 elements are provided. On call
// 'pnum' defines size of the array. On return 'pnum' lists actual number of elements 
// defined in the sensor ('pnum'<16)
int atf_ReadAllAvailableMagnification (short* pMag, int* pnum); 
// generate default name for the given objective magnification
const char *atf_DecodeMagnification (int mag) ;
// read magnification assigned to particular objective
int atf_ReadMagnification (int obj, short* pMag) ;
// extract magnification and description portion
#define GetMag(mag) ((mag)&0xff)
#define GetDesc(mag) (((mag)>>8)&0xff)
// assignses magnification to the objective. This will cause to reinitialize all the parameters 
// associated with objective
int atf_WriteMagnification (int obj, short uMag) ;

// objective configuration ----------------------------------------------------------------
// assign current z position to be best in focus
int atf_Make0();

// save all parameters into the eprom memory
int atf_SaveAll();
// read and set current objective number
int atf_ReadObjNum (int *pObj);
int atf_WriteObjNum(int iObj);

// read peak offset
enum PeakOffsetWhich
{
	/*
	type			reading offset	Number of reading elements
	----------------------------------------------------------
	RPOW_VFar	(1)	13				1
	RPOW_VNear1	(2)	12				1
	RPOW_VNear7	(3)	16				7
	RPOW_VAll	(4)	12				12
	RPOW_VHAll	(8)	0				24	
	*/
	RPOW_VFar	= 1,	// only peak for analog ATF4 (number 1, offset 13) 
	RPOW_VNear1	= 2,	// near mode peak (number 1, offset 12)
	RPOW_VNear7	= 3,	// All 7 near peaks (number 7, offset 12+4=16)
	RPOW_VAll	= 4,	// All 12 peaks V (number 12, offset 12)
	RPOW_VHAll	= 8,	// All 24 peaks V and H (number 24, offset 0)
};
int atf_ReadPeakOffset (int obj, PeakOffsetWhich pow, int* peakArray); 
int atf_WritePeakOffset(int obj, PeakOffsetWhich pow, int* peakArray); 

// read acceleration in mm/ss
int atf_ReadAccel (int obj, float *pAccel);
int atf_WriteAccel (int obj, float fAccel);
// read speed in mm/s
int atf_ReadSpeed (int obj, float* pSpeed);
int atf_WriteSpeed (int obj, float fSpeed);
// read proportional gain factor
int atf_ReadP (int obj, float* pP) ;
int atf_WriteP (int obj, float fP) ;


// read number of steps correponding to 1 mm of motion
// Note: with firmware 7.1.41.19 and up atf_ReadStepPerMmConversion is relevent to currently
//       selected objective
int atf_ReadStepPerMmConversion (u_short* puStepMm);
// write number of steps correponding to 1 mm of motion
// Note: with firmware 7.1.41.19 and up atf_WriteStepPerMmConversion affects only currently
//       selected objective
int atf_WriteStepPerMmConversion (u_short uStepMm);

// read number of microsteps assigned per each stepper motor step
// Note: with firmware 7.1.41.19 and up atf_ReadMicrostep is relevent to currently
//       selected objective
int atf_ReadMicrostep (u_short* puUstep);
// change number os microsteps 
// write number of steps correponding to 1 mm of motion
// Note: with firmware 7.1.41.19 and up atf_WriteMicrostep affects only currently
//       selected objective
int atf_WriteMicrostep (u_short uUstep);

// read number of micrometers corresponding to unit on sensor output
int atf_ReadSlopeUmPerOut (int obj, float* pfumout);
int atf_WriteSlopeUmPerOut (int obj, float fumout);

// DUV jump dist
int atf_WriteDuvJump (int obj, short sUStep);
int atf_ReadDuvJump (int obj, short* pUStep);

// write number of units as reported by sensor position defining infocus range
int atf_WriteInfocusRange (int obj, int iRange) ;
int atf_ReadInfocusRange (int obj, int* pRange) ;

// prevent sensor from changing current window mode to the different one
int atf_DisableAutoWindowTransit (void) ;
// alows automatic window selection. TYpically: near mode (7 segments) at focus and far mode if distance is
// greater then linear range
int atf_EnableAutoWindowTransit (void) ;

// read/write number of imager pixels assigned to represent linear range
int atf_ReadLinearRange (int obj, int *pLinearRange);
int atf_WriteLinearRange (int obj, int iLinearRange);

// read and write surface for currently selected objective
int atf_ReadSurface (short *piSurf);
int atf_WriteSurface (short iSurf);

// reads/writes row delay parameter for specified objective and specified window
int atf_ReadWindowRowDelay  (int obj, WindowEnm which, short* prd);
int atf_WriteWindowRowDelay (int obj, WindowEnm which, short rd);

// auto focus tracking setup ----------------------------------------------------------------
// sets upper and lower limits (in abs position units) for motion executed during AF cycle.
// if adaptive_lim!=0 then upper and lower limits are adjusted up or down around 0 position if number
// of samples in a row falls within InFocusRange
int atf_WriteMotionLimits (int upper_limit, int lower_limit, int adaptive_lim);

// sets mode in which laser is going to be turned off once focus is reached. Next
// AF command id going to enable laser
int atf_EnableAutoOff(void);
int atf_DisableAutoOff(void);

// set far moed
int atf_ForceFarMode () ;
// set near moed
int atf_ForceNearMode () ;
// set center of the imager
int atf_CenterFarWindow ();
int atf_ClearFarWindowCentering ();


// commands and tracking --------------------------------------------------------------------
// executes motion of move microsteps
int atf_MoveZ (int move);
// executes motion of move micrometers
int atf_MoveZum (int iMoveUm);
// reads most recent motion distance
int atf_ReadLastMoveZ (int &zMove);
// executes stop motor motion
int atf_StopZMotor();
// executes motion of move microsteps followed by clear abs position
int atf_MoveZ_ClearAbs (int move);

// reads position in microstepps updated by the sensor with every z motion
int atf_ReadAbsZPos (int* absZ) ;
int atf_WriteAbsZPos (int absZ) ;

// stops AF tracking
int atf_AfStop ();
// starts AF tracking
int atf_AFTrack ();
// start AF tracking first, once at focus continue on AOI tracking
int atf_AFAoiTrack ();
// start Video tracking
int atf_AFVideoTrack();
// execute focus and disables, if DUV jump is defined will be taken
int atf_AfDuvJump ();
// start AOI tracking
int atf_AFAoiOnlyTrack ();

// status and results ------------------------------------------------------------------------
// reads sensor position, typically in range +-512
int atf_ReadPosition (float &fpos);

// read position packed into single short along with some extra flags
int atf_ReadPositionPacked (RsData *prs);

// reads hardware status of HwStatusFlagsEnm type (see description above)
// Note: in fw 7.1.41.19 and up hw status relates to ZAA not PZ device
int atf_ReadHwStat (int *HwStat);

// read status flags of StatusFlagsEnm type (see description above)
int atf_ReadStatus (short *pstatus);

// reads scanline.
// mode: one of the ...WINIDOW defined above
// h_scanline: points to a buffer of MAX_SCANLINE_LENGTH where horizontal scanline is to be copied to
// v_scanline: points to a buffer of MAX_SCANLINE_LENGTH where vertical scanline is to be copied to
// h_len: number of elements of h_scanline
// v_len: returns number of elements of v_scanline
// width: returns width of the v_scanline peak as detected by sensor
int atf_ReadScanline (u_short* mode, u_short* h_scanline, u_short* v_scanline, u_short* h_len, u_short* v_len, u_short* width);

// read individual dot.line segment position
int atf_Read7DotPosition (short *p7dots, int iLinearRange);

// read value reported on analog output
int atf_ReadAnalogOut (short &analog);

int atf_ReadErrorCode (AtfCodesEnm *pcode);

// laser control ------------------------------------------------------------------------------
// turns laser ON
int atf_EnableLaser ();
// turns laser OFF
int atf_DisableLaser ();
// enable automated laser power control
int atf_LaserTrackOn() ;
// enable manual laser power control
int atf_LaserTrackOff() ;
// read current laser power. Value from 0-1023.
// ATF5
// dotIdx:	0-2 - write laser power to current window
//          4 - far mode laser power (3,5 unused) 
//          6-8 - near mode left window laser power
//          9-11 - near mode center window laser power (10 is central dot)
//          12-14 - near mode right window laser power 
// ATF6
// dotIdx:	0 - write laser power to current window (1,2 - unused)
//          4 - far mode laser power (3,5 unused) 
//			7-13 - near mode 7 dot laser power (typically they are the same, 10 is central dot)
//          6,14 - unused
int atf_ReadLaserPower(u_short *plaser, int iByteNumToRead = 1, int dotIdx = 0);
// set current laser power
int atf_WriteLaserPower(u_short laser, int iByteNumToRead = 1, int dotIdx = 0);


// equivalent to enabling sync signal on input DIO
int atf_EnableSync ();
// equivalent to disabling sync signal on input DIO
int atf_DisableSync ();

// >>> --------------- CM and Retardation Control -------------------------------------------------
// return true if CM is available and atf_...Cm... could be called
BOOL atf_IsCmAvailable();
// Glass thickness is expressed in microsteps. if >0 then cm is top, if <0 then cm is bottom. if 0 then cm is disabled
int atf_ReadCmGlassThickness (int* piThicknessUstep);
int atf_WriteCmGlassThickness (int iThicknessUstep);
// retarder status. Only if 2,3 other atf_...CmRetarder... could be called
CmRetarderEnum atf_ReadCmRetarderStatus ();
// read WASP retardation setting for a surface. Same retardation apply to all objectives
int atf_ReadCmRetarder (float *pfRetardation_percent, int iSurface);
// write WASP retardation 
// check that atf_ReadCmRetarderStatus is 2 or 3
int atf_WriteCmRetarder (float fRetardation_percent, int iSurface);
// configuration setup saved with all parameters
// if bDoAutoDetectOnMake0 - runs autodetection before make0
// if bDoWaitAutoDetectToAf - runs autodetection before AF
// by default both are not used
int atf_ReadCmRetarderAutoDetectConfigOptions (bool &bDoAutoDetectOnMake0, bool &bDoWaitAutoDetectToAf);
int atf_WriteCmRetarderAutoDetectConfigOptions(bool bDoAutoDetectOnMake0,  bool  bDoWaitAutoDetectToAf);
 // request to do autodetection before next AF cycle
int atf_ScheduleCmRetarderAutoDetectForAF();
// requests retardation autodetect 
int atf_StartCmRetarderAutoDetect();
// interrupts ongoing retardation autodetect
int atf_StopCmRetarderAutoDetect();
// discovers current autodetection status
CmAutoDetectEnum atf_ReadCmRetarderAutoDetectStatus();

// <<< --------------- CM and Retardation Control ----------------------------------------------

// >>> --------------- Strobing -------------------------------------------------
// To set strobe pulse is length of strobe signal and needs only to be read from config file.
// Led is 1 or 2
int atf_ReadStrobePulseTime(int iLed, int *pStrobeTime); 
int atf_WriteStrobePulseTime(int iLed, int iStrobeTime); 

// To set strobe minimum timer :
int atf_ReadStrobeIntervalTime(int *pStrobeIntervalUs); 
int atf_WriteStrobeIntervalTime(int iStrobeIntervalUs); 
// This parameter specifies a minimum time between 2 strobes in microseconds. In case of multi 
// image strobing this will be a delay between image frames
// To generate a frame, illuminator needs to be set in external trigger mode at current value of choice.
int atf_Strobe(int iLed, int iNum, int iMaxError);
// iLed is 1 or 2 or 3
// iNum is number of frames to be strobed. 0 to stop current illuminator strobing stream. 
//		-1 to request infinite strobing count
// iMaxError is maximum position error sensor should assure at the time of generating strobe. 
//      Set to 0 if ignored. If set it can be in the range of 1 to 511. Sensor is guarantee 
//      to wait and trigger strobe only if position error is within this range. Wait will terminate 
//      with either completing a triggered series, or when triggering is stopped with atf_Strobe(iLed,0,0) 
//      or new series is started with atf_Strobe(…) command or when objective is changed.
// The atf_Strobe will return immediately. 
// You can read number of strobes executed since last atf_Strobe command with
int atf_StrobeCount(int *pNumStb, int *pNumStbRequested, bool *pbStrobeActive);
// *pbStrobeActive is true is series is still ongoing or false if is terminated.
// <<< --------------- Strobing -------------------------------------------------

// >>> --------------- Hybrid hardware support -------------------------------------------------
// read or writes bitwise flag combination of ConfigExtEnum
int atf_ReadConfigExt (int iObj, int *pcfg);
// only: CeePZ1_Activate,CeePZ2_Activate,CeeZAA_Move,CeePZ_Move could be set. Other bits
// are going to be left intact. mask selects bits to change, cfg is the update
int atf_SetConfigExt (int iObj, int mask, int cfg);

// reads/writes objectives that governs system motion. It may be different then objective set with 
// atf_WriteObjNum which determine data acquisition conditions.
int atf_ReadMoveObjNum (int *pObj, int *pMoveObj);
int atf_WriteMoveObjNum(int iObj, int iMoveObj);

// reads position in microsteps updated by the sensor with every z motion. Stage selection
// allows to read most recent ZAA or PZ position, even if this stage is deactivated
// pMoveObj if != 0 returns objective in context which abs z was calculated
int atf_ReadAbsZPosStage (enum StageSelectorEnum ess, int* pAbsZ, int *pMoveObj) ;

// There is going to be added mapping maintained by sensor to indicate iObjMove that needs to be 
// use for the AOI AF stage that follows up after regular AF when atf_AFAoiTrack is issued.
//
int atf_ReadAoiFollowUp (int iObj, int *pObjFollowUp);
int atf_WriteAoiFollowUp (int iObj, int iObjFollowUp);
// <<< --------------- Hybrid hardware support -------------------------------------------------

// LED control ------------------------------------------------------------------------------
// there are up to 3 led (0,1,2)
int atf_ReadLedCurrent (int channel, u_short* pCurrent);
int atf_WriteLedCurrent (int channel, u_short uCurrent);
int atf_ReadLedPwm (int channel, u_short* pPWM);
int atf_WriteLedPwm (int channel, u_short uPWM);
int atf_ReadLedPwmStartupVal (int iChannel, u_short *puPWM);
int atf_WriteLedPwmStartupVal (int iChannel, u_short uPWM);

// MFC --------------------------------------------------------------------------------------
int atf_ReadMfcConfig (int* mfc_config);
// read 5 bytes used to configure motor.
int atf_ReadMfcMotorParm (u_char* motor_parm);
int atf_WriteMfcMotorParm (u_char* motor_parm);

// logger
bool atf_openLogFile(const char *pLogFilePath, const char *pMode);
bool atf_closeLogFile(void);
void atf_setLogLevel(int iLogLevel);
FILE *atf_getLogFileHandle();
void atf_saveToLog(char *pLogMsg, int iLogLevel);
void atf_saveToLog2( int iLogLevel, char *pFormat, ...);

// LLC --------------------------------------------------------------------------------------
bool atf_initializeLlc(char *pOffsetFileDir, int *piErrCode);
int atf_sendLlc(char *toSend, char *toRecv);
bool atf_getLlcStatus(int *piErrCode, const char **ppErrMsg, int *piStatReg, int *piMtrFailureReg);
bool atf_recoverLlc(int *piErrCode);
bool atf_recoverLlcSensorConnection(int *piErrCode);
bool atf_recoverLlcHdwr(int *piErrCode);
bool atf_resetLlcController(int *piErrCode);
bool atf_homeLlc(bool bWaitForEnd, int *piErrCode);
bool atf_isLlcInHomePos(int *piErrCode);
void atf_setObjectiveNum(int iObjNum);
int atf_getObjectiveNum();
int atf_getLlcForcerSerNum();
bool atf_saveLlcObjectivePositionOffset(int iObjectiveNumber, int iOffset);
bool atf_getLlcObjectivePositionOffset(int iObjectiveNumber, int *piPositionOffset);
bool atf_moveLlcToObjective(int iObjectiveNumber, bool bWaitForEnd, int *piErrCode);
bool atf_moveLlcToPosition(int iPosition, bool bWaitForEnd, int *piErrCode);
bool atf_moveLLcRelativeDistance(int iDistance, bool bWaitForEnd, int *piErrCode);
bool atf_isLlcInTargetPosition(int *piErrCode);
bool atf_enableLlcMotor(int *piErrCode);
bool atf_disableLlcMotor(int *piErrCode);
bool atf_stopLlcMotion(int *piErrCode);
bool atf_setLlcSpeed(int iLlcSpeed, int *piErrCode);
bool atf_getLlcSpeed(int *piLlcSpeed, int *piErrCode);
bool atf_setLlcAcceleration(int iLlcAcceleration, int *piErrCode);
bool atf_getLlcAcceleration(int *piLlcAcceleration, int *piErrCode);
bool atf_setLlcDeceleration(int iLlcDeceleration, int *piErrCode);
bool atf_getLlcDeceleration(int *piLlcDeceleration, int *piErrCode);
bool atf_setLlcSmoothFactor(int iLlcSmoothFactor, int *piErrCode);
bool atf_getLlcSmoothFactor(int *piLlcSmoothFactor, int *piErrCode);
bool atf_getLlcVelocity(float *pfLlcVelocity, int *piErrCode);
bool atf_getLlcPosition(float *pfLlcPosition, int *piErrCode);
bool atf_getLlcMotorCurrent(float *pfLlcMotorCurrent, int *piErrCode);
int atf_CommToLlc (char* to_send, char* to_read);

// Homing Z
int atf_ReadHomingZ_Parameters(int *pHomingZ_Parameters);
int atf_WriteHomingZ_Parameters(int *pHomingZ_Parameters);
int atf_RunHomingZ(int *pHomingZ_Parameters);
int atf_IsInHomePosition(bool *pbInHome);

// Jogging Z
int atf_StartJoggingZ(int iSpeed);

#ifdef __cplusplus
	}
#endif