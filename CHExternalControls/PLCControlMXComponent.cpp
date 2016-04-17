#include "StdAfx.h"
#include "PLCControlMXComponent.h"


CPLCControlMXComponent::CPLCControlMXComponent(DWORD dwPeriod) :
	CPLCControl(dwPeriod),
	m_ReadEvent(FALSE,FALSE,_T("DIT_PLCConnector_Event_1")), 
	m_WriteCompleteEvent(FALSE,FALSE,_T("DIT_PLCConnector_Write_Complete_Event_1"))
{
	m_hMap			= NULL;
	m_pSharedMemory = NULL;	
	m_aliveTimer	= 0;

	InitializeCriticalSection(&m_csExit);
	InitializeCriticalSection(&m_csPLCProcess);
}

CPLCControlMXComponent::~CPLCControlMXComponent(void)
{
	DeleteCriticalSection(&m_csExit);
	DeleteCriticalSection(&m_csPLCProcess);
}

// CMotorControlPmac 메시지 처리기입니다.
int CPLCControlMXComponent::StartThread()
{
	return CreateTimerThread(this);
}

void CPLCControlMXComponent::StopThread()
{
	CTimerThreadPools::StopThread();
}

BOOL CPLCControlMXComponent::ConnectToPLC()
{
	m_bPLCConnected = FALSE;

	//4k 크기의 공유 메모리 파일 생성
	m_hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE, 0, SHARED_MEMORY_SIZE, _T("DIT_MX_COMPONENT_SHARED_MEMORY"));

	//만일 이미 생성된 객체가 있다면 이미 Connector가 작동 중..
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//이미 생성된 객체가 있다면 생성이 아니라 오픈으로 실행
		m_hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,_T("DIT_MX_COMPONENT_SHARED_MEMORY"));
	}
	else//만일 생성된 객체가 없다면 정상 동작하지 않는다.
	{
		if (m_hMap == NULL)
		{
			return FALSE;
		}
	}

	//공유메모리 맵핑
	m_pSharedMemory = (char*)::MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, SHARED_MEMORY_SIZE);

	if (m_pSharedMemory == NULL)
	{
		//		g_pLog->DisplayMessage(TRUE, "ERROR: Failed to get shared memory!");			
		return FALSE;
	}

	//0으로 초기화
	ZeroMemory(m_pSharedMemory,SHARED_MEMORY_SIZE);	

	m_bPLCConnected = TRUE;

	m_aliveTimer = GetTickCount();

	return TRUE;
}

BOOL CPLCControlMXComponent::ReconnectPLC()
{
	//	g_pLog->DisplayMessage(_T("PLCConnector를 다시 시작 합니다."));
	if(m_pPLC2P != NULL)
		m_pPLC2P->PLC2P_SetPLCSignal(PLCCOMMAND_RECONNECT);

	Sleep(1000);

	return TRUE;
}

BOOL CPLCControlMXComponent::DisconnectToPLC()
{
	if (m_pSharedMemory!=NULL)
	{
		::UnmapViewOfFile(m_pSharedMemory);
		m_pSharedMemory = NULL;
	}
	if (m_hMap != NULL)
	{
		::CloseHandle(m_hMap);
		m_hMap = NULL;
	}

	return TRUE;
}

BOOL CPLCControlMXComponent::IsConnectToPLC()
{
	return m_bPLCConnected;
}

// CTimerThreadPools
void CPLCControlMXComponent::TimerThreadProcess(PVOID pParameter)
{
	if (m_pPLC2P == NULL || IsConnectToPLC() == FALSE)
	{
		Sleep(50);
		return;
	}	

	// signal
	if (IsConnectToPLC() == FALSE || ReadPLCSignal_01() == FALSE || ReadPLCSignal_02() == FALSE )
	{
		ReconnectPLC();
		return;
	}
}

BOOL CPLCControlMXComponent::ReadPLCSignal_01()
{
	if(m_pSharedMemory == NULL) return FALSE;

	BOOL bSendStatus = FALSE;

	// read read signal
	CString	strValue = _T("");
	WORD	RetVal0 = 0, RetVal1 = 0;
	memcpy(&RetVal0,m_pSharedMemory+PLC_READ_SIGNAL0, 2);
	strValue.Format(_T("%x"), RetVal0);		
	strValue.MakeUpper();
	m_nReadPLCSignal_01 = Hex2Dec(strValue);

	// check prev signal
	if(m_nPrevPLCSignal_01 != m_nReadPLCSignal_01) 
	{
		// check bit signal
		int nCurValue = 0;
		int nBitValue = 1;
		for (int i=0; i<16; i++)
		{
			// compare read and prev;
			nCurValue = (nBitValue & m_nReadPLCSignal_01);

			if ( (nCurValue!=0) && (nBitValue & m_nPrevPLCSignal_01)!=(nBitValue & m_nReadPLCSignal_01))
			{
				m_pPLC2P->PLC2P_SetPLCSignal(i);
			}			

			nBitValue = nBitValue << 1;
		}

		// save read signal
		m_nPrevPLCSignal_01 = m_nReadPLCSignal_01;

		bSendStatus = TRUE;
	}

	// read write signal
	WORD	WRetVal0 = 0, WRetVal1 = 0;
	memcpy(&WRetVal0,m_pSharedMemory+PLC_WRITEWORD_VALUE, 2);
	strValue.Format(_T("%x"), WRetVal0);		
	strValue.MakeUpper();
	m_nWReadPLCSignal_01 = Hex2Dec(strValue);

	// check prev signal
	if(bSendStatus || (m_nWPrevPLCSignal_01 != m_nWReadPLCSignal_01)) 
	{
		m_pPLC2P->PLC2P_SetPLCStatus(m_nReadPLCSignal_01, m_nWReadPLCSignal_01);

		// save read signal
		m_nWPrevPLCSignal_01 = m_nWReadPLCSignal_01;
	}

	return TRUE;
}

BOOL CPLCControlMXComponent::SendPLCSignal_01(short BITOrder, DWORD msecTimer)
{
	TCHAR*	tAddr;
	CString	strAddr;

	strAddr.Format(_T("D%d"), m_nSendAddress);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;

	return WriteWORDOneBitTimer(tAddr, BITOrder, msecTimer);
}

BOOL CPLCControlMXComponent::SendPLCSignal_01()
{
	TCHAR*	tAddr;
	CString	strAddr;

	if((GetTickCount() - m_aliveTimer) > 2000)
	{
		strAddr.Format(_T("D%d"), m_nSendAddress);
		tAddr = (TCHAR *)(LPCTSTR)strAddr;
		WriteWORDOneBitTimer(tAddr, 0, 1000);
		m_aliveTimer = GetTickCount();
	}

	/*m_nAliveSignalCnt++;
	if(m_nAliveSignalCnt >= ALIVESIGNAL_DEFINE_CNT)
	{
	m_pSend01.bProgramAlive = !m_pSend01.bProgramAlive;
	m_nAliveSignalCnt = 0;

	int	nSendData = 0;

	m_pSend01.MakeSend(nSendData);

	m_pSentBack01.Reset(TRUE);

	TCHAR*	tAddr;
	CString	strAddr;

	strAddr.Format(_T("D%d"), m_nPLCSendAddress);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;

	short sSendData = (short)nSendData;

	if(WriteLongPLC(tAddr,&sSendData, 1))
	{
	m_pSend01.ChangeSignal(m_pSentBack01);
	}
	}*/

	/*TCHAR*	tAddr;
	CString	strAddr;

	if((GetTickCount() - m_aliveTimer) > 150)
	{
	int	nSendData = 0;
	short sSendData = 0;

	m_pSend01.bProgramAlive = TRUE;
	m_pSend01.MakeSend(nSendData);
	strAddr.Format(_T("D%d"), m_nPLCSendAddress);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;
	sSendData = (short)nSendData;
	WriteLongPLC(tAddr,&sSendData, 1);
	}*/

	return TRUE;
}

BOOL CPLCControlMXComponent::ReadPLCSignal_02()
{
	if(m_pSharedMemory == NULL) return FALSE;

	BOOL bSendStatus = FALSE;

	// read read signal
	CString	strValue = _T("");
	WORD	RetVal0 = 0, RetVal1 = 0;
	memcpy(&RetVal0,m_pSharedMemory+PLC_READ_SIGNAL1, 2);
	strValue.Format(_T("%x"), RetVal0);		
	strValue.MakeUpper();
	m_nReadPLCSignal_02 = Hex2Dec(strValue);

	// check prev signal
	if(m_nPrevPLCSignal_02 != m_nReadPLCSignal_02) 
	{
		// check bit signal
		int nCurValue = 0;
		int nBitValue = 1;
		for (int i=0; i<16; i++)
		{
			// compare read and prev;
			nCurValue = (nBitValue & m_nReadPLCSignal_02);

			if ( (nCurValue!=0) && (nBitValue & m_nPrevPLCSignal_02)!=(nBitValue & m_nReadPLCSignal_02))
			{
				m_pPLC2P->PLC2P_SetPLCSubSignal(i);
			}
			nBitValue = nBitValue << 1;
		}

		// save read signal
		m_nPrevPLCSignal_02 = m_nReadPLCSignal_02;

		bSendStatus = TRUE;
	}

	// read write signal
/*	WORD	WRetVal0 = 0, WRetVal1 = 0;
	memcpy(&WRetVal0,m_pSharedMemory+PLC_WRITEWORD_VALUE, 2);
	strValue.Format(_T("%x"), WRetVal0);		
	strValue.MakeUpper();
	m_nWReadPLCSignal_02 = Hex2Dec(strValue);

	// check prev signal
	if(bSendStatus || (m_nWPrevPLCSignal_01 != m_nWReadPLCSignal_01)) 
	{
		m_pPLC2P->PLC2P_SetPLCStatus(m_nReadPLCSignal_01, m_nWReadPLCSignal_01);

		// save read signal
		m_nWPrevPLCSignal_01 = m_nWReadPLCSignal_01;
	}*/

	return TRUE;
}
BOOL CPLCControlMXComponent::SendPLCSignal_02()
{
	TCHAR*	tAddr;
	CString	strAddr;

	if((GetTickCount() - m_aliveTimer) > 2000)
	{
		strAddr.Format(_T("D%d"), m_nSendAddress);
		tAddr = (TCHAR *)(LPCTSTR)strAddr;
		WriteWORDOneBitTimer(tAddr, 0, 1000);
		m_aliveTimer = GetTickCount();
	}
	return TRUE;
}

BOOL CPLCControlMXComponent::SendPLCSignal_02(short BITOrder, DWORD msecTimer)
{
	
	TCHAR*	tAddr;
	CString	strAddr;

	strAddr.Format(_T("D%d"), m_nSendAddress);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;

	return WriteWORDOneBitTimer(tAddr, BITOrder, msecTimer);
}


BOOL CPLCControlMXComponent::ResetPLCData()
{
	//m_pRcv01.Reset();

	return TRUE;
}

void CPLCControlMXComponent::SetPLCAlarm(int nMsg, int nData)
{
	//MAINPLC_ALARM_MESSAGE emMsg = (MAINPLC_ALARM_MESSAGE)nMsg;
	int nSendBit, nDefaultSendBitCount;
	TCHAR*	tAddr;
	CString	strAddr;

	nSendBit = 0;
	nDefaultSendBitCount = 2; // Alive, ReviewEnd Signal
	strAddr.Format(_T("D%d"), m_nSendAddress);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;

	nSendBit = nMsg + nDefaultSendBitCount;
	WriteWORDOneBitTimer(tAddr, nSendBit, 1000);

	/*switch(emMsg)
	{
	case ALARM1: 
		m_pSend01.bAlarm1 = TRUE;
		m_pSentBack01.bAlarm1 = FALSE;
		break;
	case ALARM2: 
		m_pSend01.bAlarm2 = TRUE;
		m_pSentBack01.bAlarm2 = FALSE;
		break;
	case ALARM3:		
		m_pSend01.bAlarm3 = TRUE;
		m_pSentBack01.bAlarm3 = FALSE;
		break;
	case ALARM4:
		m_pSend01.bAlarm4 = TRUE;
		m_pSentBack01.bAlarm4 = FALSE;
		break;
	case ALARM5:
		m_pSend01.bAlarm5 = TRUE;
		m_pSentBack01.bAlarm5 = FALSE;
		break;
	case ALARM6:
		m_pSend01.bAlarm6 = TRUE;
		m_pSentBack01.bAlarm6 = FALSE;
		break;
	case ALARM7:
		m_pSend01.bAlarm7 = TRUE;
		m_pSentBack01.bAlarm7 = FALSE;
		break;
	case ALARM8:
		m_pSend01.bAlarm8 = TRUE;
		m_pSentBack01.bAlarm8 = FALSE;
		break;
	case ALARM9:
		m_pSend01.bAlarm9 = TRUE;
		m_pSentBack01.bAlarm9 = FALSE;
		break;
	case ALARM10:
		m_pSend01.bAlarm10 = TRUE;
		m_pSentBack01.bAlarm10 = FALSE;
		break;
	case  ALARM11:
		m_pSend01.bAlarm11 = TRUE;
		m_pSentBack01.bAlarm11 = FALSE;
		break;
	case ALARM12:
		m_pSend01.bAlarm12 = TRUE;
		m_pSentBack01.bAlarm12 = FALSE;
		break;
	case ALARM13:
		m_pSend01.bAlarm13 = TRUE;
		m_pSentBack01.bAlarm13 = FALSE;
		break;
	case ALARM14:
		m_pSend01.bAlarm14 = TRUE;
		m_pSentBack01.bAlarm14 = FALSE;
		break;
	}*/
}

void CPLCControlMXComponent::SendReviewEnd()
{
	int nSendBit;
	TCHAR*	tAddr;
	CString	strAddr;

	nSendBit = 1;	//ReviewEnd Signal
	strAddr.Format(_T("D%d"), m_nSendAddress);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;

	WriteWORDOneBitTimer(tAddr, nSendBit, 1000);
}

BOOL CPLCControlMXComponent::ReadPLCData(int nAddr, int& nData)
{
	if (IsConnectToPLC() == FALSE)
		return FALSE;

	TCHAR*	tAddr;
	CString	strAddr;

	strAddr.Format(_T("D%d"), nAddr);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;	

	char RetData[2048] = {0};

	BOOL bResult = ReadLongData(RetData,tAddr,1);		

	nData = Hex2Dec((CString)RetData);

	return TRUE;
}

BOOL CPLCControlMXComponent::ReadPLCDataFlowHistory(int nAddr, unsigned short nRetData[])
{
	if (IsConnectToPLC() == FALSE)
		return FALSE;

	TCHAR*	tAddr;
	CString	strAddr;

	strAddr.Format(_T("D%d"), nAddr);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;	

	unsigned short RetData[2048] = {0};

	BOOL bResult = ReadLongData(RetData,tAddr,20);		

	//nData = Hex2Dec((CString)RetData);
	
	for(int i = 0; i < 40; i++)
	{
		nRetData[i] = RetData[i];
	}	

	return TRUE;
}

BOOL CPLCControlMXComponent::ReadPLCData(int nAddr, int &nData, int nLen)
{
	if (IsConnectToPLC() == FALSE)
		return FALSE;

	TCHAR*	tAddr;
	CString	strAddr;
	CString	pString;

	strAddr.Format(_T("D%d"), nAddr);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;	

	char RetData[2048] = {0};

	ReadLongData(RetData,tAddr, nLen);	
	pString.Format(_T("%s"), RetData);

	if (pString.GetLength() <= 0)
		return FALSE;

	CString		strRead = pString;
	CString		strConvert;

	if(strRead.GetLength() < 8)
		return FALSE;

	strConvert = strRead.Right(4);
	strConvert += strRead.Left(4);

	nData = Hex2Dec(strConvert);

	return TRUE;
}

BOOL CPLCControlMXComponent::ReadPLCData(int nAddr, CString& strData, int nLen)
{
	if (IsConnectToPLC() == FALSE)
		return FALSE;

	TCHAR*	tAddr;
	CString	strAddr;
	CString	pString;

	strAddr.Format(_T("D%d"), nAddr);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;	

	char RetData[2048] = {0};

	ReadLongData(RetData,tAddr,nLen);	
	//	pString.Format(_T("%s"), RetData);
	pString = RetData;

	if (pString.GetLength() <= 0)
		return FALSE;

	strData = pString;

	return TRUE;
}

BOOL CPLCControlMXComponent::ReadPLCData(char cMemory, int nAddr, CString& strData, int nLen)
{
	if (IsConnectToPLC() == FALSE)
		return FALSE;

	TCHAR*	tAddr;
	CString	strAddr;
	CString	pString;

	strAddr.Format(_T("D%d"), nAddr);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;	

	char RetData[2048] = {0};

	ReadLongData(RetData,tAddr,nLen);	
	pString.Format(_T("%s"), RetData);

	if (pString.GetLength() <= 0)
		return FALSE;

	strData = pString;

	return TRUE;
}

BOOL CPLCControlMXComponent::SendPLCData(int nAddr, int nData)
{
	if (IsConnectToPLC() == FALSE)
		return FALSE;

	BOOL bRet = FALSE;
	TCHAR*	tAddr;
	CString	strAddr;

	strAddr.Format(_T("D%d"), nAddr);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;		

	bRet = WriteWORDPLCTimer(tAddr,nData,500);

	return bRet;
}

BOOL CPLCControlMXComponent::SendPLCData(char cDevice, int nAddr, CString& strData, int nLen)
{
	if (IsConnectToPLC() == FALSE)
		return FALSE;

	BOOL bRet = FALSE;
	char* op = new char[nLen * 4 + 1];
	ZeroMemory(op, nLen * 2 + 1);

	int nDataLen = strData.GetLength();
	MakePLCString(op, (unsigned char*)strData.GetBuffer(nLen), nDataLen);

	// 유효글자 이외에는 공백 표시.
	if (nDataLen % 2 == 1)
		nDataLen++;
	for (int i = nDataLen * 2; i < nLen * 2; i += 2)
	{
		op[i] = '2';
		op[i + 1] = '0';
	}

	TCHAR*	tAddr;
	CString	strAddr;

	strAddr.Format(_T("D%d"),nAddr);
	tAddr = (TCHAR *)(LPCTSTR)strAddr;

	bRet = WriteCStringPLC(tAddr,(CString)op);

	delete [] op;

	return bRet;
}

BOOL CPLCControlMXComponent::WriteCStringPLC(TCHAR* strAddress, CString strData)
{
	if(m_pSharedMemory == NULL)
		return FALSE;

	int strlength =  lstrlen(strAddress);
	if ( strlength > 6) strlength = 6;
	int	nWORDSize = strData.GetLength()/4;

	if( nWORDSize > 1000) nWORDSize = 1000;

	memset(m_pSharedMemory+PLC_WRITELONG_ADDRESS, 0x00, 7*sizeof(TCHAR));
	memcpy(m_pSharedMemory+PLC_WRITELONG_ADDRESS, strAddress,strlength*sizeof(TCHAR));
	memcpy(m_pSharedMemory+PLC_WRITELONG_WORDLENGTH, &nWORDSize, sizeof(unsigned int));
	memcpy(m_pSharedMemory+PLC_WRITELONG_DATA, strData.GetBuffer(0), nWORDSize*2);

	//모든 값을 채워 넣은 후 PLCConnector에게 PLC 주소에 값을 적도록 신호를 보낸다.
	m_pSharedMemory[PLC_WRITELONG_SIGNAL] = 1;
	return TRUE;
}

BOOL CPLCControlMXComponent::WriteLongPLC(TCHAR* strAddress, short* WriteData, unsigned int nWORDSize)
{
	if(m_pSharedMemory == NULL)
		return FALSE;

	int strlength =  lstrlen(strAddress);
	if ( strlength > 6) strlength = 6;

	if( nWORDSize > 1000) nWORDSize = 1000;

	::WaitForSingleObject(m_WriteCompleteEvent, 1000);

	memset(m_pSharedMemory+PLC_WRITELONG_ADDRESS, 0x00, 7*sizeof(TCHAR));
	memcpy(m_pSharedMemory+PLC_WRITELONG_ADDRESS, strAddress,strlength*sizeof(TCHAR));
	memcpy(m_pSharedMemory+PLC_WRITELONG_WORDLENGTH, &nWORDSize, sizeof(unsigned int));
	memcpy(m_pSharedMemory+PLC_WRITELONG_DATA, WriteData, nWORDSize*2);

	//모든 값을 채워 넣은 후 PLCConnector에게 PLC 주소에 값을 적도록 신호를 보낸다.
	m_pSharedMemory[PLC_WRITELONG_SIGNAL] = 1;
	return TRUE;
}

//strAddress : 쓰고자 하는 PLC 메모리 주소
//WriteData : 쓰고자 하는 값
//ResetData : 타이머가 지난후 리셋할 값
//리셋 타이머 지속시간
BOOL CPLCControlMXComponent::WriteWORDPLCTimer(TCHAR* strAddress, short WriteData, DWORD msecTimer)
{
	if(m_pSharedMemory == NULL)
		return FALSE;

	int strlength =  lstrlen(strAddress);
	if ( strlength > 6) strlength = 6;

	memset(m_pSharedMemory+PLC_WRITEWORD_ADDRESS, 0x00, 7*sizeof(TCHAR));
	memcpy(m_pSharedMemory+PLC_WRITEWORD_ADDRESS, strAddress,strlength*sizeof(TCHAR));
	memcpy(m_pSharedMemory+PLC_WRITEWORD_VALUE, &WriteData, 2);
	memcpy(m_pSharedMemory+PLC_WRITEWORD_TIMER, &msecTimer, 4);

	//모든 값을 채워 넣은 후 PLCConnector에게 PLC 주소에 값을 적도록 신호를 보낸다.
	m_pSharedMemory[PLC_WRITEWORD_SIGNAL] = 1;
	return TRUE;
}

BOOL CPLCControlMXComponent::WriteWORDOneBitTimer(TCHAR* strAddress, short BITOrder, DWORD msecTimer)
{
	if(m_pSharedMemory == NULL)
		return FALSE;

	int strlength =  lstrlen(strAddress);
	if ( strlength > 6) strlength = 6;

	short WriteData = (1 << BITOrder);
	//WriteData= (1 << BITOrder);

	memset(m_pSharedMemory+PLC_WRITEWORD_ADDRESS,	0x00,		7*sizeof(TCHAR));
	memcpy(m_pSharedMemory+PLC_WRITEWORD_ADDRESS,	strAddress,	strlength*sizeof(TCHAR));
	memcpy(m_pSharedMemory+PLC_WRITEWORD_VALUE,		&WriteData, 2);
	memcpy(m_pSharedMemory+PLC_WRITEWORD_TIMER,		&msecTimer, 4);

	//모든 값을 채워 넣은 후 PLCConnector에게 PLC 주소에 값을 적도록 신호를 보낸다.
	m_pSharedMemory[PLC_WRITEWORD_SIGNAL] = 1;
	return TRUE;
}

BOOL CPLCControlMXComponent::WriteWORDManyBitsTimer(TCHAR* strAddress, char* BitFlag, DWORD msecTimer)
{
	short WriteData = 0;

	for( int i = 0; i < 16; i++)
		WriteData |= (BitFlag[i] << i);

	return WriteWORDPLCTimer( strAddress,  WriteData, msecTimer);
}

BOOL CPLCControlMXComponent::ReadLongData(char* pData, TCHAR* strAddress, unsigned short nWORDSize)
{
	if(m_pSharedMemory == NULL)
		return FALSE;

	int strlength =  lstrlen(strAddress);
	if ( strlength > 6) strlength = 6;

	if( nWORDSize > 1000) nWORDSize = 1000;

	//공유 메모리에 물류데이터를 읽도록 신호를 보낸다.`
	memset(m_pSharedMemory+PLC_READ_LONGDATA_SIGNAL_1, 0x01, 1);
	memcpy(m_pSharedMemory+PLC_READ_LONGDATA_ADDRESS_1, strAddress,strlength*sizeof(TCHAR));
	memcpy(m_pSharedMemory+PLC_READ_LONGDATA_LENGTH_1, &nWORDSize,sizeof(unsigned short));

	//데이터를 다 읽고 공유메모리 적을 때까지 기다린다.
	m_ReadEvent.ResetEvent();
	//TRACE(_T("[A2E]m_ReadEvent.ResetEvent()\n"));
	::WaitForSingleObject(m_ReadEvent, 2000);
	//TRACE(_T("[A2E]::WaitForSingleObject(m_ReadEvent, 2000)\n"));

	//완료가 되면 해당 값을 다시 읽어온다.
	//pData = m_pSharedMemory+PLC_READ_LONGDATA_VALUE;

	memcpy(pData, m_pSharedMemory+PLC_READ_LONGDATA_VALUE_1, nWORDSize*sizeof(TCHAR));

	return TRUE;
}

BOOL CPLCControlMXComponent::ReadLongData(unsigned short* pData, TCHAR* strAddress, unsigned short nWORDSize)
{
	if(m_pSharedMemory == NULL)
		return FALSE;

	int strlength =  lstrlen(strAddress);
	if ( strlength > 6) strlength = 6;

	if( nWORDSize > 1000) nWORDSize = 1000;

	//공유 메모리에 물류데이터를 읽도록 신호를 보낸다.`
	memset(m_pSharedMemory+PLC_READ_LONGDATA_SIGNAL_1, 0x01, 1);
	memcpy(m_pSharedMemory+PLC_READ_LONGDATA_ADDRESS_1, strAddress,strlength*sizeof(TCHAR));
	memcpy(m_pSharedMemory+PLC_READ_LONGDATA_LENGTH_1, &nWORDSize,sizeof(unsigned short));

	//데이터를 다 읽고 공유메모리 적을 때까지 기다린다.
	m_ReadEvent.ResetEvent();
	//TRACE(_T("[A2E]m_ReadEvent.ResetEvent()\n"));
	::WaitForSingleObject(m_ReadEvent, 2000);
	//TRACE(_T("[A2E]::WaitForSingleObject(m_ReadEvent, 2000)\n"));

	//완료가 되면 해당 값을 다시 읽어온다.
	//pData = m_pSharedMemory+PLC_READ_LONGDATA_VALUE;

	//memcpy(pData, m_pSharedMemory+PLC_READ_LONGDATA_VALUE_1, nWORDSize*sizeof(TCHAR));	

	for(int i = 0; i < nWORDSize*sizeof(TCHAR); i++)
	{
		memcpy(pData + i, m_pSharedMemory+PLC_READ_LONGDATA_VALUE_1 + i, 1);
	}

	return TRUE;
}