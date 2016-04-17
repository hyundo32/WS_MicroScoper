
#ifndef __POWERPMAC_H__
#define __POWERPMAC_H__

#include "PowerPmacDef.h"


#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

	// ���̺귯�� ����
	// ���ڸ� NULL�� �� ��� DTKDeviceSelect �Լ��� ����Ͽ� ��ġ�� �����ؾ� �Ѵ�.
	UINT	WINAPI DTKPowerPmacOpen(DWORD dwIPAddress, UINT uMode);

	// ���̺긮�� Ŭ����
	UINT	WINAPI DTKPowerPmacClose(UINT uDeviceID);

	// ��ϵ� ����̽� ����
	UINT	WINAPI DTKGetDeviceCount(PINT nDeviceCount);

	// IP Address Ȯ��
	UINT	WINAPI DTKGetIPAddress(UINT uDeviceID, PDWORD pdwIPAddress);
	
	// ��ġ�� ����
	UINT	WINAPI DTKConnect(UINT uDeviceID);

	// ��ġ�� ����
	UINT	WINAPI DTKDisconnect(UINT uDeviceID);

	// ��ġ�� ����Ǿ����� Ȯ��
	UINT	WINAPI DTKIsConnected(UINT uDeviceID, PBOOL pbConnected);

	// Single Character �������� ����
	UINT	WINAPI DTKGetResponseA(UINT uDeviceID, LPSTR lpCommand, LPSTR lpResponse, INT nLength);

	// Wild Character �������� ����
	UINT	WINAPI DTKGetResponseW(UINT uDeviceID, LPWSTR lpwCommand, LPWSTR lpwResponse, INT nLength);

	// Single Character �������� ����
	UINT	WINAPI DTKSendCommandA(UINT uDeviceID, LPSTR lpCommand);

	// Wild Character �������� ����
	UINT	WINAPI DTKSendCommandW(UINT uDeviceID, LPWSTR lpwCommand);

	UINT	WINAPI DTKAbort(UINT uDeviceID);

	// Single Character �������� Download
	UINT	WINAPI DTKDownloadA(UINT uDeviceID, LPSTR lpDownload, BOOL bDowoload, PDOWNLOAD_PROGRESS lpDownloadProgress, PDOWNLOAD_MESSAGE_A lpDownloadMessage);

	// Wild Character �������� Download
	UINT	WINAPI DTKDownloadW(UINT uDeviceID, LPWSTR lpwDownload, BOOL bDowoload, PDOWNLOAD_PROGRESS lpDownloadProgress, PDOWNLOAD_MESSAGE_W lpwDownloadMessage);

	// Single Character �������� ����
	UINT	WINAPI DTKSetReceiveA(UINT uDeviceID, PRECEIVE_PROC_A lpReveiveProc);

	// Wild Character �������� ����
	UINT	WINAPI DTKSetReceiveW(UINT uDeviceID, PRECEIVE_PROC_W lpwReveiveProc);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__POWERPMAC_H__