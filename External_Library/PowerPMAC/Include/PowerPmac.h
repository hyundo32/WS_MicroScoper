
#ifndef __POWERPMAC_H__
#define __POWERPMAC_H__

#include "PowerPmacDef.h"


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	// 라이브러리 오픈
	// 인자를 NULL로 할 경우 DTKDeviceSelect 함수를 사용하여 장치를 연결해야 한다.
	UINT	WINAPI DTKPowerPmacOpen(DWORD dwIPAddress, UINT uMode);

	// 라이브리리 클로즈
	UINT	WINAPI DTKPowerPmacClose(UINT uDeviceID);

	// 등록된 디바이스 갯수
	UINT	WINAPI DTKGetDeviceCount(PINT nDeviceCount);

	// IP Address 확인
	UINT	WINAPI DTKGetIPAddress(UINT uDeviceID, PDWORD pdwIPAddress);
	
	// 장치를 연결
	UINT	WINAPI DTKConnect(UINT uDeviceID);

	// 장치를 해제
	UINT	WINAPI DTKDisconnect(UINT uDeviceID);

	// 장치가 연결되었는지 확인
	UINT	WINAPI DTKIsConnected(UINT uDeviceID, PBOOL pbConnected);

	// Single Character 형식으로 제어
	UINT	WINAPI DTKGetResponseA(UINT uDeviceID, LPSTR lpCommand, LPSTR lpResponse, INT nLength);

	// Wild Character 형식으로 제어
	UINT	WINAPI DTKGetResponseW(UINT uDeviceID, LPWSTR lpwCommand, LPWSTR lpwResponse, INT nLength);

	// Single Character 형식으로 제어
	UINT	WINAPI DTKSendCommandA(UINT uDeviceID, LPSTR lpCommand);

	// Wild Character 형식으로 제어
	UINT	WINAPI DTKSendCommandW(UINT uDeviceID, LPWSTR lpwCommand);

	UINT	WINAPI DTKAbort(UINT uDeviceID);

	// Single Character 형식으로 Download
	UINT	WINAPI DTKDownloadA(UINT uDeviceID, LPSTR lpDownload, BOOL bDowoload, PDOWNLOAD_PROGRESS lpDownloadProgress, PDOWNLOAD_MESSAGE_A lpDownloadMessage);

	// Wild Character 형식으로 Download
	UINT	WINAPI DTKDownloadW(UINT uDeviceID, LPWSTR lpwDownload, BOOL bDowoload, PDOWNLOAD_PROGRESS lpDownloadProgress, PDOWNLOAD_MESSAGE_W lpwDownloadMessage);

	// Single Character 형식으로 제어
	UINT	WINAPI DTKSetReceiveA(UINT uDeviceID, PRECEIVE_PROC_A lpReveiveProc);

	// Wild Character 형식으로 제어
	UINT	WINAPI DTKSetReceiveW(UINT uDeviceID, PRECEIVE_PROC_W lpwReveiveProc);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__POWERPMAC_H__