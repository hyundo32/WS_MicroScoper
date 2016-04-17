/******************************************************************************
 *	
 *	(C) Copyright WDI 2012
 *	
 ******************************************************************************
 *
 *	FILE:		atf_def.h
 *
 *	PROJECT:	AFT Sensors
 *
 *	SUBPROJECT:	.
 *
 *	Description: ATF related definitions
 *	
 ******************************************************************************
 *	
 *	Change Activity
 *	Defect  Date       Developer        Description
 *	Number  DD/MM/YYYY Name
 *	======= ========== ================ =======================================
 *          19/04/2012 Chris O.        Initial version
 *                                     
 *****************************************************************************/

#pragma once

#ifdef __cplusplus
	extern "C" {
#endif

// data type
typedef enum 
{
	DtNoDataEnm					= 0x00,
	DtByteEnm					= 0x10,
	DtWordEnm					= 0x20,
	DtDwordEnm					= 0x30,
	DtDataType					= 0x30,
	DtArrayEnm					= 0x40,
	DtOffsetEnm					= 0x80,
	DtSignedEnm					= 0x80,
	DtLastDataEnm				= 0x80,
} EppDataType;

// sensor status packed data
typedef struct 
{
	unsigned short  miv : 1;
	unsigned short  in_focus : 1;
	unsigned short  sync : 1;
	unsigned short  laser_enabled : 1;
	unsigned short  invalid_data : 1;
			 short  position : 11; 
} RsData, *RsDataPtr;
// sensor position max value
#define MAX_RS_POS  511
// sensor position min value
#define MIN_RS_POS  -512

// fpga config flags
enum FpgaConfigFlagsEnm 
{
    FpgaConfig_Analog             = 0x00000001,
    FpgaConfig_MtrCtl             = 0x00000002,
    FpgaConfig_XY_MtrCtl          = 0x00000004,
    FpgaConfig_Std_Mcm            = 0x00000008,
    FpgaConfig_Maaf               = 0x00000010,
    FpgaConfig_Mfc_Mcm            = 0x00000020,
    FpgaConfig_StepDir_Mcm        = 0x00000040,
    FpgaConfig_HW_Link_Mcm        = 0x00000080,
    FpgaConfig_Any_Mcm            = 0x0000007e,

	FpgaConfigMfc_Led1            = 0x00000100,
    FpgaConfigMfc_Led2            = 0x00000200,
    FpgaConfigMfc_Led3            = 0x00000400,
    FpgaConfigMfc_12VReg          = 0x00000800,
    FpgaConfigMfc_Epld            = 0x00001000,
    FpgaConfigMfc_Whistle         = 0x00002000,
    FpgaConfigMfc_IntZDrive       = 0x00004000,
    FpgaConfigMfc_Reserved        = 0x00008000,
    FpgaConfigMfc_Flags           = 0x0000ff00,

    FpgaConfigMfcLinkDown         = 0x00010000,

    FpgaConfig_XY_Encoder         = 0x01000000,
};

// hardware status flags
enum HwStatusFlagsEnm 
{
    HwOK                    = 0x00000001,   // exec: hardware seems to be OK
    HwCasErr                = 0x00000002,   // exec: PPR_SR_CAS_TIME_ERR

	HwLaserDioDisabled      = 0x00000010,   // exec: PPR_SR_DIOR_LASER_ENABLE

    HwSyncDioAsserted       = 0x00000100,   // exec: ^PPR_SR_DIOR_SYNC
    HwSyncDioLastFrame      = 0x00000200,   // exec: PPR_SR_DIOR_SYNC_LASTFRAME
    HwSyncEnabled           = 0x00000400,   // exec: FCS_CR_SYNC_SENSE_ENABLE

    HwMotionZ               = 0x00001000,   // exec: FMP_CR_MOTION_BUSY
    HwMotionX               = 0x00002000,   // exec: FMP_CR_X_MOVE
    HwMotionY               = 0x00004000,   // exec: FMP_CR_Y_MOVE

    HwMotionCWLimit         = 0x00010000,   // exec: FMP_CR_CW_LIMIT_SWITCH
    HwMotionCWLimitSense    = 0x00020000,   // exec: FMP_CR_CW_SENSE    
    HwMotionCCWLimit        = 0x00040000,   // exec: FMP_CR_CCW_LIMIT_SWITCH
    HwMotionCCWLimitSense   = 0x00080000,   // exec: FMP_CR_CCW_SENSE

    HwMotionInhibit         = 0x00100000,   // exec: FMP_CR_MOTION_INHIBIT
    HwMotionInhibitSense    = 0x00200000,   // exec: FMP_CR_MOTION_INHIBIT_SENSE    
};

// sensor status flags
enum StatusFlagsEnm 
{
	MsHwOK                  = 0x00000001,   // exec: hardware seems to be OK
	MsSwOK                  = 0x00000002,   // exec: software finds all OK    
	MsXYMotion              = 0x00000004,   // exec: XY motion
	MsZMotion               = 0x00000008,   // exec: Z  motion

	MsEnableLaser           = 0x00000010,   // param: turn laser On, Off
	MsLaserDisabledMode     = 0x00000020,   // exec: entered laser disabled mode    
	MsEnableSyncSensitivity = 0x00000040,   // param: sync is enabled
	MsSyncMode              = 0x00000080,   // exec: entered sync mode

	MsExecuteAf             = 0x00000100,   // param    
	MsLaserTracking         = 0x00000200,   // param
	MsZTracking             = 0x00000400,   // param
	MsNearWindow            = 0x00000800,   // exec: set if near mode is used (7D or dominate)

	MsMiv                   = 0x00001000,   // exec
	MsInFocus               = 0x00002000,   // exec
	MsFocusRefining         = 0x00004000,   // exec: obsoleted
	MsCachedData            = 0x00008000,   // exec: signifies that data is generated from last frame, and temporary unavailable
};

// sensor window definitions
#define     FAR_WINDOW                  0       // far mode window
#define     NEAR_WINDOW                 1       // near mode window
#define     FAR3D_WINDOW                2       // indicate far window working in 3D mode
#define     SV_WINDOW                   3       // single frame window

#define MAX_SCANLINE_LENGTH				1400

// error codes return by sensor
enum AtfCodesEnm 
{
	AfStatusOK					= 0,
	AfStatusSaturated			= 1,
	AfStatusLowIntensity		= 2,
    AfStatusOutOfBound			= 3,
	AfStatusFailed				= 5,
	AfNotSupported				= 6,
	AfSyntaxError				= 7,
	AfNotAvailable				= 11,
};

#define AtfNamesTable					\
	static const char* g_AtfStatusName[] = {\
	"0-OK",								\
	"1-Saturated",						\
	"2-LowIntensity",					\
	"3-OutOfBound",						\
	0,									\
	"5-Failed",							\
	"6-NotSupported",					\
	"7-SyntaxError",					\
	0,									\
	0,									\
	0,									\
	"11-Not Available",					\
	};

typedef enum {
	WindowFarEnm	= 0,	// far mode window
	WindowAtf4Enm	= 0,	// atf4 window
	WindowNear1Enm	= 1,	// near mode single window
	WindowNear7Enm	= 2,	// near mode seven segment window
	WindowNear3Enm	= 2,	// near mode 3 segments window
} WindowEnm;

enum CmAutoDetectEnum
{
	CmAutoDetectStatusDone		= 0,	// ready to be run
	CmAutoDetectStop		    = 1,	// 
	CmAutoDetectStatusInitial	= 4,	// not ran yet, ready to be run
	CmAutoDetectStatusPending	= 11,	// will be in progress
	CmAutoDetectStatusFailed	= 10,	// failed due to some error
	CmAutoDetectStatusBusy		= 13,	// in progress
	// other codes indicate error
};
enum CmRetarderEnum
{
	CmRetarderStatusUnavailable = 0,	// no retardation installed (not a wasp)
	CmRetarderStatusCalibrating = 1,	// undergoing calibration
	CmRetarderStatusActive		= 2,	// ready for operation
	CmRetarderStatusInactive	= 3,	// turned off
};

enum ConfigExtEnum {
	CeePZ1_Activate     = 0x0400,             // activate PZ1 (turns on PZ1 and sets it to 0)
	CeePZ2_Activate     = 0x0800,             // activate PZ2 (turns on PZ2 and sets it to 0)
	CeeZAA_Move         = 0x0100,             // use Zaa for motion
	CeePZ_Move          = 0x0200,             // use Pz for motion
	CeeAO_Out           = 0x0000,             // use analog out for sensor position
} ;

enum StageSelectorEnum {
	SseZAA				= 1,
	SsePZ				= 2,
};

#ifdef __cplusplus
	}
#endif