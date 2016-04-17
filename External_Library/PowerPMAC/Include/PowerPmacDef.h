
#ifndef __POWERPMACDEF_H__
#define __POWERPMACDEF_H__

typedef enum _DTK_MODE_TYPE
{
	DM_GPASCII			= 0,
	DM_GETSENDS_0		= 1,
	DM_GETSENDS_1		= 2,
	DM_GETSENDS_2		= 3,
	DM_GETSENDS_3		= 4,
	DM_GETSENDS_4		= 5
} DTK_MODE_TYPE;

typedef enum _DTK_STATUS
{
	DS_Ok				= 0,
	DS_Exception		= 1,
	DS_TimeOut			= 2,
	DS_Connected		= 3,
	DS_NotConnected		= 4,
	DS_Failed			= 5,
	DS_InvalidDevice	= 11,
	DS_LengthExceeds	= 21,
	DS_RunningDownload	= 22,
	DS_RunningRead		= 23
} DTK_STATUS;

typedef enum _DTK_RESET_TYPE
{
	DR_Reset			= 0,
	DR_FullReset		= 1
} DTK_RESET_TYPE;

typedef VOID (WINAPI *PDOWNLOAD_PROGRESS)(INT nPercent);
typedef VOID (WINAPI *PDOWNLOAD_MESSAGE_A)(LPSTR lpMessage);
typedef VOID (WINAPI *PDOWNLOAD_MESSAGE_W)(LPWSTR lpwMessage);

typedef VOID (WINAPI *PRECEIVE_PROC_A)(LPSTR lpReveive);
typedef VOID (WINAPI *PRECEIVE_PROC_W)(LPWSTR lpwReveive);


#endif	//__POWERPMAC_H__