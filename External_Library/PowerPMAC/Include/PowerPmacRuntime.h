
#ifndef __POWERPMACRUNTIME_H__
#define __POWERPMACRUNTIME_H__

#include "PowerPmacDef.h"

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	typedef UINT	(CALLBACK *DTKPOWERPMACOPEN)(DWORD dwIPAddress, UINT uMode);
	typedef UINT	(CALLBACK *DTKPOWERPMACCLOSE)(UINT uDeviceID);
	typedef UINT	(CALLBACK *DTKCONNECT)(UINT uDeviceID);
	typedef UINT	(CALLBACK *DTKDISCONNECT)(UINT uDeviceID);
	typedef UINT	(CALLBACK *DTKISCONNECTED)(UINT uDeviceID, PBOOL pConnected);
	typedef UINT	(CALLBACK *DTKGETRESPONSEA)(UINT uDeviceID, LPSTR lpCommand, LPSTR lpResponse, INT nLength);
	typedef UINT	(CALLBACK *DTKGETRESPONSEW)(UINT uDeviceID, LPWSTR lpwCommand, LPWSTR lpwResponse, INT nLength);
	typedef UINT	(CALLBACK *DTKSENDCOMMANDA)(UINT uDeviceID, LPSTR lpCommand);
	typedef UINT	(CALLBACK *DTKSENDCOMMANDW)(UINT uDeviceID, LPWSTR lpwCommand);
	typedef UINT	(CALLBACK *DTKABORT)(UINT uDeviceID);
	typedef UINT	(CALLBACK *DTKDOWNLOADA)(UINT uDeviceID, LPSTR lpDownload, BOOL bDowoload, PDOWNLOAD_PROGRESS lpDownloadProgress, PDOWNLOAD_MESSAGE_A lpDownloadMessage);
	typedef UINT	(CALLBACK *DTKDOWNLOADW)(UINT uDeviceID, LPWSTR lpwDownload, BOOL bDowoload, PDOWNLOAD_PROGRESS lpDownloadProgress, PDOWNLOAD_MESSAGE_W lpwDownloadMessage);
	typedef UINT	(CALLBACK *DTKSETRECEIVEA)(UINT uDeviceID, PRECEIVE_PROC_A lpReveiveProc);
	typedef UINT	(CALLBACK *DTKSETRECEIVEW)(UINT uDeviceID, PRECEIVE_PROC_W lpwReveiveProc);

	extern DTKPOWERPMACOPEN		DTKPowerPmacOpen;
	extern DTKPOWERPMACCLOSE	DTKPowerPmacClose;
	extern DTKCONNECT			DTKConnect;
	extern DTKDISCONNECT		DTKDisconnect;
	extern DTKISCONNECTED		DTKIsConnected;
	extern DTKGETRESPONSEA		DTKGetResponseA;
	extern DTKGETRESPONSEW		DTKGetResponseW;
	extern DTKSENDCOMMANDA		DTKSendCommandA;
	extern DTKSENDCOMMANDW		DTKSendCommandW;
	extern DTKABORT				DTKAbort;
	extern DTKDOWNLOADA			DTKDownloadA;
	extern DTKDOWNLOADW			DTKDownloadW;
	extern DTKSETRECEIVEA		DTKSetReceiveA;
	extern DTKSETRECEIVEW		DTKSetReceiveW;
	
	HINSTANCE	OpenRuntimeLink();
	VOID		CloseRuntimeLink();

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__POWERPMACRUNTIME_H__