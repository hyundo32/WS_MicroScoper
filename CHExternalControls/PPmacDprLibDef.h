#ifndef __PPMACLIBDEF_H__
#define __PPMACLIBDEF_H__

typedef enum _PPMAC_STATUS
{
	PS_Ok				= 0,
	PS_Exception		= 1,
	PS_TimeOut			= 2,
	PS_NotConnected		= 3,
	PS_Failed			= 4,
	PS_InvalidDevice	= 11,
	PS_LengthExceeds	= 21,
	PS_RunningThread	= 22
} PPMAC_STATUS;

#endif	//__PPMACLIBDEF_H__