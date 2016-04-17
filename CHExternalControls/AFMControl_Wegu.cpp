#include "StdAfx.h"
#include "AFMControl_Wegu.h"
#include "atf_lib.h"
#include <tchar.h>

CAFMControl_Wegu::CAFMControl_Wegu(int nIndex, int nControlType) : CAFMControl(nIndex, nControlType)
{
	m_dSpeed = 1.0;
	m_hMutex = CreateMutex(NULL, FALSE, _T("AFMMutex"));
}

CAFMControl_Wegu::~CAFMControl_Wegu(void)
{		
	/*sSharedData SendData;
	strcpy(SendData.szCharName, m_strPort);
	SendData.nCmd = AFM_WEGU_END_PROCESS;
	CommunicationAFM(&SendData);*/

	/*if(GetConnected())
		atf_CloseConnection();*/
	//CopyMemory((PVOID)m_pBuf, &SendData , sizeof(sSharedData));
	CloseHandle(m_hMutex);
}

void WCharToChar( const wchar_t* pwstrSrc, char pstrDest[] )
{
	int nLen = ( int )wcslen( pwstrSrc );
	wcstombs( pstrDest, pwstrSrc, nLen + 1 );
}

void CharToWChar( const char* pstrSrc, wchar_t pwstrDest[] )
{
	int nLen = ( int )strlen( pstrSrc ) + 1;
	mbstowcs( pwstrDest, pstrSrc, nLen );
}



int	CAFMControl_Wegu::Connect(const SAFMConnParam& connParam)
{
	WaitForSingleObject(m_hMutex, INFINITE);
	
	m_bConnected = FALSE;

	CString strPort = _T("");
	strPort.Format(_T("COM%d"), connParam.nPortNum);

#ifdef UNICODE
	int Ret = 0;
	Ret = WideCharToMultiByte(CP_ACP, 0, strPort, (int)_tcslen(strPort), m_strPort, 15, NULL, NULL);
	m_strPort[Ret] = 0;
#endif
	
	//CWinThread* thread = AfxBeginThread(MyThreadProc, m_strPort);	
	m_nBaudrate_start = 9600;		// upon powercycle sensor always starts with 9600
	int baudrate_run = 9600;
	int point_count = -1;
	
	// open port, return if failed
	int ret = atf_OpenConnection(m_strPort, m_nBaudrate_start);
	if (ret)
	{
		ReleaseMutex(m_hMutex);	
		return 0;
	}

	Sleep(100);

	if(m_bConnected  = atf_isSerialConnection())
	{
		atf_SetComIdx(m_nIndex);

		Sleep(100);

		atf_ReadMicrostep(&m_nMicroSteps);

		Sleep(100);

		atf_ReadStepPerMmConversion(&m_nFullsteps);

		Sleep(100);

		atf_ReadHomingZ_Parameters(m_pAFMHomingZParam);		
	}

	if (m_bConnected==0) 
	{
		ReleaseMutex(m_hMutex);	
		return 0;
	}

	m_nRecipeIndex = 0;
	m_strRecipeName = _T("Default");
	int nZoomIndex = m_nZoomIndex = -1;
	
	ret = atf_ReadObjNum(&nZoomIndex);
	if (ret==0)
	{
		m_nZoomIndex = nZoomIndex;
	}

	RecipeZoom(1);

	ReleaseMutex(m_hMutex);	

	return 1;
}

void CAFMControl_Wegu::Disconnect()
{
	int ret;
	if(atf_GetComIdx() != m_nIndex)
	{
		ret = atf_SetComIdx(m_nIndex);		
	}
	atf_CloseConnection();
}

int	CAFMControl_Wegu::RecipeJogSpeed(double dSpeed)
{
	m_dSpeed = dSpeed;
	return 1;
}

int	CAFMControl_Wegu::RecipeJogCommand(int nCmd)
{
	int ret;

//	if(!atf_isSerialConnection()) return 0;
	if(!m_bConnected) return 0;

	if(atf_GetComIdx() != m_nIndex)
	{
		ret = atf_SetComIdx(m_nIndex);
		if(ret) return 0;
	}

	switch(nCmd)
	{
	case 0:
		ret = 0;
		break;
	case 1:
		ret = atf_MoveZ(((double)m_dSpeed * -m_nMicroSteps * m_nFullsteps)/1000);
		break;
	case -1:
		ret = atf_MoveZ(((double)-m_dSpeed * -m_nMicroSteps * m_nFullsteps)/1000);
		break;
	}
	if(ret) return 0;

	return 1;	
}


int	CAFMControl_Wegu::RecipeTracking(int nTracking)
{
	return 1;
}

int	CAFMControl_Wegu::RecipeZoom(int nZoomIndex)
{
	int ret;

//	if(!atf_isSerialConnection()) return 0;
	if(!m_bConnected) return 0;

	if(atf_GetComIdx() != m_nIndex)
	{
		ret = atf_SetComIdx(m_nIndex);
		if(ret) return 0;
	}

	ret = atf_WriteObjNum(nZoomIndex);
	if(ret) return 0;

	return 1;
}

int	CAFMControl_Wegu::RecipeChange(int nRecipeIndex, int nZoomIndex)
{
	return RecipeZoom(nZoomIndex);
}

int	CAFMControl_Wegu::RecipeChange(const CString& strRecipeName, int nZoomIndex)
{
	return RecipeZoom(nZoomIndex);
}

BOOL CAFMControl_Wegu::SetTracking(int nTracking)
{
	int ret;

//	if(!atf_isSerialConnection()) return 0;
	if(!m_bConnected) return 0;

	if(atf_GetComIdx() != m_nIndex)
	{
		ret = atf_SetComIdx(m_nIndex);
		if(ret) return 0;
	}
	
	if(nTracking)
	{
		ret = atf_AFTrack();
	}
	else
	{
		ret = atf_AfStop();
	}	

	if(ret) return FALSE;

	return TRUE;
}

BOOL CAFMControl_Wegu::SetZoomIndex(int nZoomIndex)
{
	int ret;

//	if(!atf_isSerialConnection()) return 0;
	if(!m_bConnected) return 0;

	if(atf_GetComIdx() != m_nIndex)
	{
		ret = atf_SetComIdx(m_nIndex);
		if(ret) return 0;
	}

	ret = atf_WriteObjNum(nZoomIndex);

	if(ret) return FALSE;

	m_nZoomIndex = nZoomIndex;

	return TRUE;
}

BOOL CAFMControl_Wegu::SetRecipeIndex(int nRecipeIdnex, int nZoomIndex)
{
	int ret;
	if (nZoomIndex!=-1)
	{
		ret = SetZoomIndex(nZoomIndex);
	}
	return ret;
}

BOOL CAFMControl_Wegu::GetConnected()
{	
	int ret;

//	if(!atf_isSerialConnection()) return 0;
	if(!m_bConnected) return 0;

	if(atf_GetComIdx() != m_nIndex)
	{
		ret = atf_SetComIdx(m_nIndex);
		if(ret) return 0;
	}

	return atf_isSerialConnection();
}

BOOL CAFMControl_Wegu::SetRecipeName(const CString& strRecipeName, int nZoomIndex)
{
	int ret;

	if (nZoomIndex!=-1)
	{
		ret = SetZoomIndex(nZoomIndex);
	}
	
//	ReleaseMutex(m_hMutex);
	return ret;

}

BOOL CAFMControl_Wegu::GetTracking(int& nTracking)
{
	nTracking = m_nTracking;	
	return TRUE;
}

BOOL CAFMControl_Wegu::GetZoomIndex(int& nZoomIndex)
{
	nZoomIndex = m_nZoomIndex;	
	return TRUE;
}

BOOL CAFMControl_Wegu::GetRecipeIndex(int& nRecipeIdnex, int& nZoomIndex)
{
	nRecipeIdnex = m_nRecipeIndex;
	nZoomIndex = m_nZoomIndex;	
	return TRUE;
}

BOOL CAFMControl_Wegu::GetRecipeName(CString& strRecipeName, int& nZoomIndex)
{
	strRecipeName = strRecipeName;
	nZoomIndex = m_nZoomIndex;	
	return TRUE;
}

BOOL CAFMControl_Wegu::MoveToLimitPlus()
{
	int ret;

//	if(!atf_isSerialConnection()) return 0;
	if(!m_bConnected) return 0;

	if(atf_GetComIdx() != m_nIndex)
	{
		ret = atf_SetComIdx(m_nIndex);
		if(ret) return 0;
	}

	//ret = atf_MoveZ(((double)300000 * m_nMicroSteps * m_nFullsteps)/1000);

	int arrHomeParam[8];

	memcpy(arrHomeParam, m_pAFMHomingZParam, sizeof(int) * 8);

	arrHomeParam[3] = arrHomeParam[7] = 0;

	ret = atf_RunHomingZ(arrHomeParam);

	if(ret) return FALSE;

	return TRUE;
}
BOOL CAFMControl_Wegu::MoveToLimitMinus()
{
	int ret;

//	if(!atf_isSerialConnection()) return 0;
	if(!m_bConnected) return 0;

	if(atf_GetComIdx() != m_nIndex)
	{
		ret = atf_SetComIdx(m_nIndex);
		if(ret) return 0;
	}

	//ret = atf_MoveZ(((double)-300000 * m_nMicroSteps * m_nFullsteps)/1000);

	int arrHomeParam[8];

	memcpy(arrHomeParam, m_pAFMHomingZParam, sizeof(int) * 8);

	arrHomeParam[3] = arrHomeParam[7] = 0;

	arrHomeParam[0] *= -1;

	arrHomeParam[1] *= -1;

	arrHomeParam[2] *= -1;

	ret = atf_RunHomingZ(arrHomeParam);

	if(ret) return FALSE;

	return TRUE;
}
BOOL CAFMControl_Wegu::MoveToBasePosition(int nZoomIndex)
{
	int ret;

//	if(!atf_isSerialConnection()) return 0;
	if(!m_bConnected) return 0;

	if(atf_GetComIdx() != m_nIndex)
	{
		ret = atf_SetComIdx(m_nIndex);
		if(ret) return 0;
	}

	//ret = atf_MoveZ(((double)-100 * m_nMicroSteps * m_nFullsteps)/1000);

	//if(ret) return FALSE;

	//Sleep(100);

//	ret = atf_RunHomingZ(m_pAFMHomingZParam);
	
	ret = atf_MoveZ(((double)m_pAFMHomingZParam[3] * m_nMicroSteps * m_nFullsteps)/1000);

	if(ret) return FALSE;

	return TRUE;
}

void CAFMControl_Wegu::WorkThreadProcess(PVOID pParameter)
{
	bool m_bIsWorking = FALSE;
	while(!m_bIsWorking){
		WaitForSingleObject(m_hMutex, INFINITE);
		atf_CloseConnection();
		int ret = atf_OpenConnection(m_strPort, m_nBaudrate_start);
		if (ret)
		{
			ReleaseMutex(m_hMutex);
			return ;
		}

		atf_IsInHomePosition(&m_bIsWorking);
		ReleaseMutex(m_hMutex);
		Wait(100);
	}

	WaitForSingleObject(m_hMutex, INFINITE);
	atf_CloseConnection();
	int ret = atf_OpenConnection(m_strPort, m_nBaudrate_start);
	if (ret)
	{
		ReleaseMutex(m_hMutex);
		return ;
	}	
	atf_AFTrack();
	ReleaseMutex(m_hMutex);
}

void CAFMControl_Wegu::Wait(DWORD dwMillisecond)

{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while(GetTickCount() - dwStart < dwMillisecond)
	{

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

sSharedData CAFMControl_Wegu::CommunicationAFM(sSharedData* SendData)
{	
	CopyMemory((PVOID)m_pBuf, SendData , sizeof(sSharedData));	

	while(1)
	{
		Sleep(60);
		sSharedData* pRcvData = (sSharedData*)m_pBuf;
		
		if(!strcmp(pRcvData->szCharName, "main"))
		{
			sSharedData	RcvData;
			RcvData.nArgument = pRcvData->nArgument;
			RcvData.nCmd = pRcvData->nCmd;
			strcpy(RcvData.szCharName, pRcvData->szCharName);

//			WaitForSingleObject(m_hMutex, INFINITE);
			//CopyMemory((PVOID)m_pBuf, *pRcvData , sizeof(sSharedData));	
			*pRcvData = sSharedData();
//			ReleaseMutex(m_hMutex);

			return RcvData;
		}
	}
	return sSharedData();
}

