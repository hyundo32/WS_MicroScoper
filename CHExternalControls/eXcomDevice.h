#pragma once

#include <AFXSOCK.H>

#define MAXDATA_LEN		1400
#define ARRAY_MAX		15360

typedef struct _EXCOM_ETH_
{
	BYTE  RequestType;
	BYTE  Request;
	WORD  wValue;
	WORD  wIndex;
	WORD  wLength;
	BYTE  bData[1492];
}  EXCOM_ETH, *PEXCOM_ETH;

class CeXcomDevice
{
public:
	CeXcomDevice(int nNo = 0);
	virtual ~CeXcomDevice(void);

	BOOL Connect(DWORD IPAddress, int nPort);
	void Close();
	BOOL IsConnected()	{ return m_bConnected; }

	BOOL ReadOneData(long nAddress, long &nValue);
	BOOL WriteOneData(long nAddress, long nValue);
	BOOL ReadMultyData(long nAddress, long *pValue, int nSize);
	BOOL WriteMultyData(long nAddress, long *pValue, int nSize);

	BOOL ReadOneData(long nAddress, float &fValue);
	BOOL WriteOneData(long nAddress, float fValue);
	BOOL ReadMultyData(long nAddress, float *pValue, int nSize);
	BOOL WriteMultyData(long nAddress, float *pValue, int nSize);

protected:
	BOOL SetMemory(DWORD dwStartAddr, DWORD dwLength, PVOID val);
	BOOL GetMemory(DWORD dwStartAddr, DWORD dwLength, PVOID val);

	void Lock();
	void Release();

protected:
	SOCKET *m_socket;
	BOOL m_bConnected;

	HANDLE m_hMutex;
};
