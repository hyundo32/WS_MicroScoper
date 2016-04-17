#include "stdafx.h"

#pragma hdrstop

#define NO_EXTERN
#include "PPmacDprRuntime.h"
#undef NO_EXTERN

HINSTANCE hPPmacDprLib = NULL;

//----------------------------------------------------------------------------
FARPROC GetPPmacDprFuncAddress(HMODULE hModule,LPCSTR lpProcName)
{
 //TCHAR tmp[256];
 FARPROC adr;

 adr = GetProcAddress(hModule,lpProcName);
 if (adr == NULL)
   {
   // sprintf(tmp, _T("Error in loading library function: %s"),lpProcName);
   // MessageBox(NULL,tmp, _T(""),MB_ICONSTOP | MB_OK);
   }
 return adr;
}

//----------------------------------------------------------------------------
void ClosePPmacDprRuntimeLink()
{
 if (hPPmacDprLib != NULL)
   {
    FreeLibrary(hPPmacDprLib);
    hPPmacDprLib = NULL;
   }
}

// Macro to make things consistant and easier
#define PPDPRDLL_LINK(var,type,name)  var=(type)GetPPmacDprFuncAddress(hPPmacDprLib,name); \
                                 if (var==NULL) goto load_err


//----------------------------------------------------------------------------
HINSTANCE OpenPPmacDprRuntimeLink()
{
	// Get handle to PComm32.DLL
	hPPmacDprLib = LoadLibrary(PPMACDPR_DRIVERNAME);

	if (hPPmacDprLib == NULL)
	{
		MessageBox(NULL, _T("Error in loading PPmacDpr library."), _T("PPmacDpr"),MB_ICONSTOP | MB_OK | MB_TOPMOST);
		return NULL;
	}
	else
	{
// Get all COMM the function handles required
		PPDPRDLL_LINK(PPmacDprOpen,			PPMACDPROPEN,			"PPmacDprOpen");
		PPDPRDLL_LINK(PPmacDprClose,		PPMACDPRCLOSE,			"PPmacDprClose");
		PPDPRDLL_LINK(PPmacDprConnect,		PPMACDPRCONNECT,		"PPmacDprConnect");
		PPDPRDLL_LINK(PPmacDprDisconnect,	PPMACDPRDISCONNECT,		"PPmacDprDisconnect");
		PPDPRDLL_LINK(PPmacDprIsConnected,	PPMACDPRISCONNECTED,	"PPmacDprIsConnected");
		PPDPRDLL_LINK(PPmacDprGetDPRMem,	PPMACDPRGETDPRMEM,		"PPmacDprGetDPRMem");
		PPDPRDLL_LINK(PPmacDprSetDPRMem,	PPMACDPRSETDPRMEM,		"PPmacDprSetDPRMem");

		return hPPmacDprLib;

load_err:
    FreeLibrary(hPPmacDprLib);
    hPPmacDprLib = NULL;
    return NULL;
  }
}

