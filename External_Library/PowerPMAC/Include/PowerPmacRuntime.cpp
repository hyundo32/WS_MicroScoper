#include <Windows.h>
#include <Tchar.h>
#include "PowerPmacRuntime.h"

DTKPOWERPMACOPEN	DTKPowerPmacOpen;
DTKPOWERPMACCLOSE	DTKPowerPmacClose;
DTKCONNECT			DTKConnect;
DTKDISCONNECT		DTKDisconnect;
DTKISCONNECTED		DTKIsConnected;
DTKGETRESPONSEA		DTKGetResponseA;
DTKGETRESPONSEW		DTKGetResponseW;
DTKSENDCOMMANDA		DTKSendCommandA;
DTKSENDCOMMANDW		DTKSendCommandW;
DTKABORT			DTKAbort;
DTKDOWNLOADA		DTKDownloadA;
DTKDOWNLOADW		DTKDownloadW;
DTKSETRECEIVEA		DTKSetReceiveA;
DTKSETRECEIVEW		DTKSetReceiveW;

HINSTANCE			g_hLib = NULL;

HINSTANCE OpenRuntimeLink()
{
	#ifdef WIN64
		g_hLib = LoadLibrary(_T("PowerPmac64.dll"));
	#else
		g_hLib = LoadLibrary(_T("PowerPmac32.dll"));
	#endif


	if (g_hLib)
	{
		INT			i;
		
		for (i = 0; i < 1; i++)
		{
			DTKPowerPmacOpen		= (DTKPOWERPMACOPEN)GetProcAddress(g_hLib, "DTKPowerPmacOpen");
			if (DTKPowerPmacOpen == NULL) break;
			
			DTKPowerPmacClose		= (DTKPOWERPMACCLOSE)GetProcAddress(g_hLib, "DTKPowerPmacClose");
			if (DTKPowerPmacClose == NULL) break;
			
			DTKConnect				= (DTKCONNECT)GetProcAddress(g_hLib, "DTKConnect");
			if (DTKConnect == NULL) break;
			
			DTKDisconnect			= (DTKDISCONNECT)GetProcAddress(g_hLib, "DTKDisconnect");
			if (DTKDisconnect == NULL) break;
			
			DTKIsConnected			= (DTKISCONNECTED)GetProcAddress(g_hLib, "DTKIsConnected");
			if (DTKIsConnected == NULL) break;
			
			DTKGetResponseA			= (DTKGETRESPONSEA)GetProcAddress(g_hLib, "DTKGetResponseA");
			if (DTKGetResponseA == NULL) break;
			
			DTKGetResponseW			= (DTKGETRESPONSEW)GetProcAddress(g_hLib, "DTKGetResponseW");
			if (DTKGetResponseW == NULL) break;
			
			DTKAbort				= (DTKABORT)GetProcAddress(g_hLib, "DTKAbort");
			if (DTKAbort == NULL) break;
			
			DTKDownloadA			= (DTKDOWNLOADA)GetProcAddress(g_hLib, "DTKDownloadA");
			if (DTKDownloadA == NULL) break;
			
			DTKDownloadW			= (DTKDOWNLOADW)GetProcAddress(g_hLib, "DTKDownloadW");
			if (DTKDownloadW == NULL) break;
			
			DTKSetReceiveA			= (DTKSETRECEIVEA)GetProcAddress(g_hLib, "DTKSetReceiveA");
			if (DTKSetReceiveA == NULL) break;
			
			DTKSetReceiveW			= (DTKSETRECEIVEW)GetProcAddress(g_hLib, "DTKSetReceiveW");
			if (DTKSetReceiveW == NULL) break;
		}
		
		if(i == 0)                         // Check validity of procedure addresses
			CloseRuntimeLink();
		
	}
	else
		MessageBox(NULL, _T("Error in loading Power Pmac library."), _T("Power Pmac"), MB_ICONSTOP | MB_OK | MB_TOPMOST);
	
	return g_hLib;
}

VOID CloseRuntimeLink()
{
	if (g_hLib)
	{
		FreeLibrary(g_hLib);
		g_hLib		= NULL;
	}
}