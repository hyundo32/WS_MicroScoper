#ifndef __PPMACDPR_RUNTIME_H__
#define __PPMACDPR_RUNTIME_H__

#include"PPmacDprLibDef.h"
extern HINSTANCE hPPmacDprLib;	//

#ifdef WIN64
	#ifdef _DEBUG
		#define PPMACDPR_DRIVERNAME	TEXT("PPMACDPRLib_64D.dll")
	#else
		#define PPMACDPR_DRIVERNAME	TEXT("PPMACDPRLib_64.dll")
	#endif
#else
	#ifdef _DEBUG
		#define PPMACDPR_DRIVERNAME	TEXT("PPMACDPRLibD.dll")
	#else
		#define PPMACDPR_DRIVERNAME	TEXT("PPMACDPRLib.dll")
	#endif
#endif

//****************************************************
// Function Type Definitions
//****************************************************

typedef UINT (CALLBACK *PPMACDPROPEN) (DWORD dwIPAddress, INT nPortNo);
typedef UINT (CALLBACK *PPMACDPRCLOSE) (UINT uDeviceID);
typedef UINT (CALLBACK *PPMACDPRCONNECT) (UINT uDeviceID);
typedef UINT (CALLBACK *PPMACDPRDISCONNECT) (UINT uDeviceID);
typedef UINT (CALLBACK *PPMACDPRISCONNECTED) (UINT uDeviceID);
typedef UINT (CALLBACK *PPMACDPRGETDPRMEM) (UINT uDeviceID, long lStartAddr, long lLength, PVOID val);
typedef UINT (CALLBACK *PPMACDPRSETDPRMEM) (UINT uDeviceID, long lStartAddr, long lLength, PVOID val);


//***************************************************
// Functions

#ifdef NO_EXTERN
	#define EXTRN
#else
	#define EXTRN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

	HINSTANCE	OpenPPmacDprRuntimeLink();
	void		ClosePPmacDprRuntimeLink();

	EXTRN	PPMACDPROPEN			  PPmacDprOpen;
	EXTRN	PPMACDPRCLOSE		    PPmacDprClose;
	EXTRN	PPMACDPRCONNECT		  PPmacDprConnect;
	EXTRN	PPMACDPRDISCONNECT	PPmacDprDisconnect;
	EXTRN	PPMACDPRISCONNECTED	PPmacDprIsConnected;
	EXTRN	PPMACDPRGETDPRMEM	  PPmacDprGetDPRMem;
	EXTRN	PPMACDPRSETDPRMEM   PPmacDprSetDPRMem;

#ifdef __cplusplus
}
#endif

#endif
