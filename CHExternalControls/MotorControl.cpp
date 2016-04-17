#include "StdAfx.h"
#include "MotorControl.h"

CMotorControl::CMotorControl(int nIndex, DWORD dwPeriod) : CTimerThreadPools(dwPeriod)
{
	m_nIndex		= nIndex;
	m_pMC2P			= NULL;
	m_bConnected	= FALSE;
	m_motorCommand.Reset();
	
	// var
	m_nAxisCount		= 0;
	m_pAxisStatus		= NULL;
	m_pAxisPosition		= NULL;
	m_pAxisStatusPrev	= NULL;
	m_pAxisPositionPrev	= NULL;
	m_pAxisStatusAddr	= NULL;
	m_pAxisPositionAddr	= NULL;

	m_axisThetaStatus			= 0;
	m_axisThetaStatusPrev		= 0;
	m_axisThetaPosition			= 0.f;
	m_axisThetaPositionPrev		= -999.0;

	m_strPort					= "";
	m_nDelayCount				= 0;

	m_MotionEndThreadData.pThisPtr = this;
}

CMotorControl::~CMotorControl(void)
{
	m_nAxisCount = 0;

	if (m_pAxisStatus)
	{
		delete [] m_pAxisStatus;		
		m_pAxisStatus = NULL;
	}

	if (m_pAxisPosition)
	{
		delete [] m_pAxisPosition;		
		m_pAxisPosition = NULL;
	}

	if (m_pAxisStatusPrev)
	{
		delete [] m_pAxisStatusPrev;	
		m_pAxisStatusPrev = NULL;
	}

	if (m_pAxisPositionPrev)
	{
		delete [] m_pAxisPositionPrev;	
		m_pAxisPositionPrev = NULL;
	}

	if (m_pAxisStatusAddr)
	{
		delete [] m_pAxisStatusAddr;	
		m_pAxisStatusAddr = NULL;
	}

	if (m_pAxisPositionAddr)
	{
		delete [] m_pAxisPositionAddr;	
		m_pAxisPositionAddr = NULL;
	}
}




BOOL CMotorControl::TransformIP_CStringToDWORD(const CString& strServerIP, DWORD& dwServerIP)
{
	BOOL bReturn = TRUE;
	int nStartIdx = 0, nFieldIdx = 0;
	int nIPField0, nIPField1, nIPField2, nIPField3;
	CString strToken, strIPField0, strIPField1, strIPField2, strIPField3;

	if(FALSE == strServerIP.IsEmpty())
	{
		do 
		{
			strToken = strServerIP.Tokenize(_T("."), nStartIdx);

			switch(nFieldIdx)
			{
			case 0:
				strIPField0 = strToken;
				break;
			case 1:
				strIPField1 = strToken;
				break;
			case 2:
				strIPField2 = strToken;
				break;
			case 3:
				strIPField3 = strToken;
				break;
			default:
				break;
			}  

			nFieldIdx++;
		} while (strToken != _T(""));

		nIPField0 = _ttoi(strIPField0);
		nIPField1 = _ttoi(strIPField1);
		nIPField2 = _ttoi(strIPField2);
		nIPField3 = _ttoi(strIPField3);
		dwServerIP = (nIPField0 << 24) | (nIPField1 << 16) | (nIPField2 << 8) | (nIPField3);
	}
	else
	{
		bReturn =  FALSE;
	}

	return bReturn;
}


UINT CMotorControl::MotionEndThread_Func(LPVOID pParam)
{
	CMotionEndThreadData *pData = static_cast<CMotionEndThreadData*>(pParam);
	if (pData==NULL) return 0;


	CMotorControl *pThis = static_cast<CMotorControl*>(pData->pThisPtr);
	if (pThis==NULL) return 0;

	pThis->MotionEndThread_Func(pData);

	return 1;
}

void CMotorControl::MotionEndThread_Func(CMotionEndThreadData *pData)
{
	if (m_pMC2P==NULL || pData==NULL) return;

	::Sleep(pData->nDelayTime);

	BOOL bResult = FALSE;
	DWORD nStartTick = GetTickCount();

	if (pData->nAxisIndex==-1 && pData->nGantryIndex!=-1)
	{
		do 
		{
			bResult = IsGantryMotionEnd(pData->nGantryIndex);
			::Sleep(10);

			if ( (GetTickCount()-nStartTick) > pData->nWaitTime)
			{
				break;
			}
		} while (!bResult);

		m_pMC2P->MC2P_GantryMotionEnd(pData->nGantryIndex, bResult);
	}
	else if (pData->nAxisIndex!=-1 && pData->nGantryIndex==-1)
	{
		do 
		{
			bResult = IsAxisMotionEnd(pData->nAxisIndex);
			::Sleep(10);

			if ( (GetTickCount()-nStartTick) >pData->nWaitTime)
			{
				break;
			}
		} while (!bResult);

		m_pMC2P->MC2P_AxisMotionEnd(pData->nAxisIndex, bResult);
	}

	pData->bRunning = false;

	return;
}

void CMotorControl::StopThread()
{
	CTimerThreadPools::StopThread();
}

void CMotorControl::TimerThreadProcess(PVOID pParameter)
{
	if (!m_bConnected)
	{
		if(m_nDelayCount == 300)
		{
			if(Connect(m_strPort))
			{
				m_bConnected = TRUE;
			}
			m_nDelayCount = 0;
		}
		else
		{
			m_nDelayCount++;
		}
		return;
	}

	if (m_pMC2P==NULL) return;

	if (m_nAxisCount<1) return;

	// axis position+-3
	if (ReadAddressValue(m_pAxisPositionAddr[0], m_pAxisPosition, m_nAxisCount))
	{
		for (int i=0; i<m_nAxisCount; i++)
		{
			if (int(m_pAxisPositionPrev[i]*1000)!=int(m_pAxisPosition[i]*1000))
			{
				m_pMC2P->MC2P_UpdatePosition(m_nIndex, i, m_pAxisPosition[i]);
			}
			m_pAxisPositionPrev[i] = m_pAxisPosition[i];
		}
	}
	else
	{
		m_bConnected = FALSE;
		if(Connect(m_strPort))
		{
			m_bConnected = TRUE;
		}
	}

	// axis status
	if (ReadAddressValue(m_pAxisStatusAddr[0], m_pAxisStatus, m_nAxisCount))
	{
		for (int i=0; i<m_nAxisCount; i++)
		{
			if (m_pAxisStatusPrev[i]!=m_pAxisStatus[i])
			{
				m_pMC2P->MC2P_UpdateStatus(m_nIndex, i, m_pAxisStatus[i]);
			}
			m_pAxisStatusPrev[i] = m_pAxisStatus[i];
		}
	}
	
	// theta axis
	if (m_motorCommand.bUseThetaAxis)
	{
		// theta position
		if (ReadAddressValue(m_motorCommand.axisThetaAddr.nPositionAddr, m_axisThetaPosition))
		{
			if (int(m_axisThetaPositionPrev*1000000)!=int(m_axisThetaPosition*1000000))
			{
				m_pMC2P->MC2P_UpdateThetaPosition(m_nIndex, m_axisThetaPosition);
			}
			m_axisThetaPositionPrev = m_axisThetaPosition;
		}

		// theta status
		if (ReadAddressValue(m_motorCommand.axisThetaAddr.nStatusAddr, m_axisThetaStatus))
		{
			if (m_axisThetaStatusPrev!=m_axisThetaStatus)
			{
				m_pMC2P->MC2P_UpdateThetaStatus(m_nIndex, m_axisThetaStatus);
			}
			m_axisThetaStatusPrev = m_axisThetaStatus;
		}
	}
}

int	 CMotorControl::GetAxisGantryIndex(int nAxisIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		for (VectorMotorAxisIt aIt=gIt->vectorMotorAxis.begin(); aIt!=gIt->vectorMotorAxis.end(); aIt++)
		{
			if (aIt->nAxisIdx==nAxisIdx)
			{
				return aIt->nGantryIdx;
			}
		}
	}

	return -1;
}

int CMotorControl::GetAxisPositionAddr(int nAxisIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		for (VectorMotorAxisIt aIt=gIt->vectorMotorAxis.begin(); aIt!=gIt->vectorMotorAxis.end(); aIt++)
		{
			if (aIt->nAxisIdx==nAxisIdx)
			{
				return aIt->nPositionAddr;
			}
		}
	}

	return ADDRESS_NONE;
}

int CMotorControl::GetAxisStatusAddr(int nAxisIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		for (VectorMotorAxisIt aIt=gIt->vectorMotorAxis.begin(); aIt!=gIt->vectorMotorAxis.end(); aIt++)
		{
			if (aIt->nAxisIdx==nAxisIdx)
			{
				return aIt->nStatusAddr;
			}
		}
	}

	return ADDRESS_NONE;
}

int CMotorControl::GetAxisGoPositionAddr(int nAxisIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		for (VectorMotorAxisIt aIt=gIt->vectorMotorAxis.begin(); aIt!=gIt->vectorMotorAxis.end(); aIt++)
		{
			if (aIt->nAxisIdx==nAxisIdx)
			{
				return aIt->nGoPositionAddr;
			}
		}
	}

	return ADDRESS_NONE;
}

int CMotorControl::GetAxisJogCommandAddr(int nAxisIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		for (VectorMotorAxisIt aIt=gIt->vectorMotorAxis.begin(); aIt!=gIt->vectorMotorAxis.end(); aIt++)
		{
			if (aIt->nAxisIdx==nAxisIdx)
			{
				return aIt->nJogCommandAddr;
			}
		}
	}

	return ADDRESS_NONE;
}

int	CMotorControl::GetAxisManualGoSpeedAddr(int nAxisIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		for (VectorMotorAxisIt aIt=gIt->vectorMotorAxis.begin(); aIt!=gIt->vectorMotorAxis.end(); aIt++)
		{
			if (aIt->nAxisIdx==nAxisIdx)
			{
				return aIt->nGoSpeedAddr;
			}
		}
	}
	return ADDRESS_NONE;
}

int	CMotorControl::GetAxisManualGoAccelAddr(int nAxisIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		for (VectorMotorAxisIt aIt=gIt->vectorMotorAxis.begin(); aIt!=gIt->vectorMotorAxis.end(); aIt++)
		{
			if (aIt->nAxisIdx==nAxisIdx)
			{
				return aIt->nGoAccelAddr;
			}
		}
	}
	return ADDRESS_NONE;
}

int	CMotorControl::GetAxisJogSpeedAddr(int nAxisIdx)
{
// 	if (nAxisIdx==0 || nAxisIdx==1) 
// 	{
// 		return 180;
// 	}
// 	else
// 	{
// 		return 181;
// 	}

	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		for (VectorMotorAxisIt aIt=gIt->vectorMotorAxis.begin(); aIt!=gIt->vectorMotorAxis.end(); aIt++)
		{
			if (aIt->nAxisIdx==nAxisIdx)
			{
				return aIt->nJogSpeedAddr;
			}
		}
	}
	return ADDRESS_NONE;
}

int	CMotorControl::GetAxisJogAccelAddr(int nAxisIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		for (VectorMotorAxisIt aIt=gIt->vectorMotorAxis.begin(); aIt!=gIt->vectorMotorAxis.end(); aIt++)
		{
			if (aIt->nAxisIdx==nAxisIdx)
			{
				return aIt->nJogAccelAddr;
			}
		}
	}
	return ADDRESS_NONE;
}

int	CMotorControl::GetAxisThetaPositionAddr()
{
	if (m_motorCommand.bUseThetaAxis!=1) return ADDRESS_NONE;
	return m_motorCommand.axisThetaAddr.nPositionAddr;
}

int	CMotorControl::GetAxisThetaStatusAddr()
{
	if (m_motorCommand.bUseThetaAxis!=1) return ADDRESS_NONE;
	return m_motorCommand.axisThetaAddr.nStatusAddr;
}
int	CMotorControl::GetAxisThetaGoPositionAddr()
{
	if (m_motorCommand.bUseThetaAxis!=1) return ADDRESS_NONE;
	return m_motorCommand.axisThetaAddr.nGoPositionAddr;
}

int	CMotorControl::GetAxisThetaJogCommandAddr()
{
	if (m_motorCommand.bUseThetaAxis!=1) return ADDRESS_NONE;
	return m_motorCommand.axisThetaAddr.nJogCommandAddr;
}

int	CMotorControl::GetAxisThetaManualGoAddr()
{
	if (m_motorCommand.bUseThetaAxis!=1) return ADDRESS_NONE;
	return m_motorCommand.nAxisThetaManualGoAddr;
}

int	CMotorControl::GetAxisThetaManualGoSpeedAddr()
{
	if (m_motorCommand.bUseThetaAxis!=1) return ADDRESS_NONE;
	return m_motorCommand.axisThetaAddr.nGoSpeedAddr;
}

int	CMotorControl::GetAxisThetaManualGoAccelAddr()
{
	if (m_motorCommand.bUseThetaAxis!=1) return ADDRESS_NONE;
	return m_motorCommand.axisThetaAddr.nGoAccelAddr;
}

int	CMotorControl::GetAxisThetaJogSpeedAddr()
{
	if (m_motorCommand.bUseThetaAxis!=1) return ADDRESS_NONE;
	return m_motorCommand.axisThetaAddr.nJogSpeedAddr;
}

int	CMotorControl::GetAxisThetaJogAccelAddr()
{
	if (m_motorCommand.bUseThetaAxis!=1) return ADDRESS_NONE;
	return m_motorCommand.axisThetaAddr.nJogAccelAddr;
}

int CMotorControl::GetGantryAxisCount(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return (int)gIt->vectorMotorAxis.size();
		}
	}

	return 0;
}

int CMotorControl::GetGantryManualGoAddr(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return gIt->nManualGoAddr;
		}
	}

	return ADDRESS_NONE;
}

int CMotorControl::GetGantryAutoGoAddr(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return gIt->nAutoGoAddr;
		}
	}

	return ADDRESS_NONE;
}


int	 CMotorControl::GetGantryMeasureAutoGoAddr(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return gIt->nMeasureAutoGoAddr;
		}
	}

	return ADDRESS_NONE;
}

int CMotorControl::GetGantryOtherAutoGoAddr(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return gIt->nOtherAutoGoAddr;
		}
	}

	return ADDRESS_NONE;
}

int CMotorControl::GetGantryMoveCountAddr(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return gIt->nMoveCountAddr;
		}
	}

	return ADDRESS_NONE;
}

int	CMotorControl::GetGantryMeasureMoveCountAddr(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return gIt->nMeasureMoveCountAddr;
		}
	}

	return ADDRESS_NONE;
}

int CMotorControl::GetGantryOtherMoveCountAddr(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return gIt->nOtherMoveCountAddr;
		}
	}

	return ADDRESS_NONE;
}

int CMotorControl::GetGantryTriggerCountAddr(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return gIt->nTriggerCountAddr;
		}
	}

	return ADDRESS_NONE;
}

int	CMotorControl::GetGantryMeasureTriggerCountAddr(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return gIt->nMeasureTriggerCountAddr;
		}
	}

	return ADDRESS_NONE;
}

int CMotorControl::GetGantryOtherTriggerCountAddr(int nGantryIdx)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		if (gIt->nGantryIdx==nGantryIdx)
		{
			return gIt->nOtherTriggerCountAddr;
		}
	}

	return ADDRESS_NONE;
}

BOOL CMotorControl::GetAxisLimitPosition(int nAxisIdx, double& dPlusLimit, double& dMinusLimit)
{
	for (VectorMotorGantryIt gIt=m_motorCommand.vectorMotorGantry.begin(); gIt!=m_motorCommand.vectorMotorGantry.end(); gIt++)
	{
		for (VectorMotorAxisIt aIt=gIt->vectorMotorAxis.begin(); aIt!=gIt->vectorMotorAxis.end(); aIt++)
		{
			if (aIt->nAxisIdx==nAxisIdx)
			{
				dPlusLimit =  aIt->dLimitPlus;
				dMinusLimit =  aIt->dLimitMinus;
				return TRUE;
			}
		}
	}

	return FALSE;
}




// axis
BOOL CMotorControl::AxisManualGo(int nAxisIndex, double dPosition)
{
	int nGantryIdx = GetAxisGantryIndex(nAxisIndex);

	if (nGantryIdx<0) return FALSE;

	UINT nManualGoAddr = GetGantryManualGoAddr(nGantryIdx);
	if (nManualGoAddr==ADDRESS_NONE) return FALSE;

	UINT nGoPosAddr = GetAxisGoPositionAddr(nAxisIndex);
	if (nGoPosAddr==ADDRESS_NONE) return FALSE;

	if (WriteAddressValue(nGoPosAddr, dPosition)==FALSE)
	{
		return FALSE;
	}

	int nAxisCount = 0;
	for (int i=0; i<nGantryIdx; i++)
	{
		nAxisCount += GetGantryAxisCount(i);
	}

	// 해당 비트만 set
	long nValue = 1 << (nAxisIndex-nAxisCount);

	return WriteAddressValue(nManualGoAddr, nValue);
}


BOOL CMotorControl::AxisManualGoEnd(int nAxisIdx, double dPosition)
{
	if (AxisManualGo(nAxisIdx, dPosition)==FALSE) return FALSE;

	if (m_MotionEndThreadData.bRunning==TRUE) return FALSE;

	m_MotionEndThreadData.bRunning		= true;
	m_MotionEndThreadData.nWaitTime		= 5000; //ms
	m_MotionEndThreadData.nAxisIndex	= nAxisIdx;
	m_MotionEndThreadData.nGantryIndex	= -1;
	m_MotionEndThreadData.pThisPtr		= this;

	AfxBeginThread(MotionEndThread_Func, static_cast<void*>(&m_MotionEndThreadData));

	return TRUE;
}

BOOL CMotorControl::AxisManualGo(const VectorInteger& vectorAxis, const VectorDouble& vectorPosition)
{
	if (vectorAxis.size()<1 || vectorPosition.size()<1) return FALSE;

	if (vectorAxis.size() != vectorPosition.size()) return FALSE;

	// 모든 축의 겐트리 인덱스가 같은가 확인
	int nGantryIdx = GetAxisGantryIndex(vectorAxis[0]);
	if (nGantryIdx<0) return FALSE;
	for (int i=1; i<(int)vectorAxis.size(); i++)
	{
		if (nGantryIdx!=GetAxisGantryIndex(vectorAxis[i]))
		{
			return FALSE;
		}
	}

	// 겐트리 수동이동 주소 확인
	UINT nManualGoAddr = GetGantryManualGoAddr(nGantryIdx);
	if (nManualGoAddr==ADDRESS_NONE) return FALSE;

	// 축별 수동이동 좌표 쓰기
	for (int i=0; i<(int)vectorAxis.size(); i++)
	{
		UINT nGoPosAddr = GetAxisGoPositionAddr(vectorAxis[i]);
		if (nGoPosAddr==ADDRESS_NONE) return FALSE;

		if (WriteAddressValue(nGoPosAddr, vectorPosition[i])==FALSE)
		{
			return FALSE;
		}
	}

	int nAxisCount = 0;
	for (int i=0; i<nGantryIdx; i++)
	{
		nAxisCount += GetGantryAxisCount(i);
	}

	// 해당 축의 비트만 set
	long nValue = 0;
	for (int i=0; i<(int)vectorAxis.size(); i++)
	{
		long nTemp = 1 << (vectorAxis[i]-nAxisCount);
		nValue = nValue | nTemp;
	}

	return WriteAddressValue(nManualGoAddr, nValue);
}

BOOL CMotorControl::AxisJogPlus(int nAxisIdx)
{
	int nJogAddr = (int)GetAxisJogCommandAddr(nAxisIdx);
	if (nJogAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nJogAddr, (long)1);
}

BOOL CMotorControl::AxisJogMinus(int nAxisIdx)
{
	int nJogAddr = (int)GetAxisJogCommandAddr(nAxisIdx);
	if (nJogAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nJogAddr, (long)2);
}

BOOL CMotorControl::AxisJogStop(int nAxisIdx)
{
	int nJogAddr = (int)GetAxisJogCommandAddr(nAxisIdx);
	if (nJogAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nJogAddr, (long)0);
}

BOOL CMotorControl::AxisManualGoSpeed(int nAxisIdx, double dSpeed)
{
	int nAddr = GetAxisManualGoSpeedAddr(nAxisIdx);

	if (nAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nAddr, dSpeed);
}

BOOL CMotorControl::AxisJogSpeed(int nAxisIdx, double dSpeed)
{
	int nAddr = GetAxisJogSpeedAddr(nAxisIdx);

	if (nAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nAddr, dSpeed);
	//return WriteAddressValue(nAddr, nSpeed);
}

BOOL CMotorControl::AxisManualGoAccel(int nAxisIdx, int nAccel)
{
	int nAddr = GetAxisManualGoAccelAddr(nAxisIdx);

	if (nAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nAddr, (long)nAccel);
}

BOOL CMotorControl::AxisJogAccel(int nAxisIdx, int nAccel)
{
	int nAddr = GetAxisJogAccelAddr(nAxisIdx);

	if (nAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nAddr, (long)nAccel);
}


// axis theta
BOOL CMotorControl::AxisThetaManualGo(double dPosition)
{
	if (m_motorCommand.bUseThetaAxis!=1) return FALSE;
	
	UINT nManualGoAddr = GetAxisThetaManualGoAddr();
	if (nManualGoAddr==ADDRESS_NONE) return FALSE;

	UINT nGoPosAddr = GetAxisThetaGoPositionAddr();
	if (nGoPosAddr==ADDRESS_NONE) return FALSE;

	if (WriteAddressValue(nGoPosAddr, dPosition)==FALSE)
	{
		return FALSE;
	}

	return WriteAddressValue(nManualGoAddr, (long)1);
}


BOOL CMotorControl::AxisThetaManualGoEnd(double dPosition)
{
	if (AxisThetaManualGo(dPosition)==FALSE) return FALSE;

	if (m_MotionEndThreadData.bRunning==TRUE) return FALSE;

	m_MotionEndThreadData.bRunning		= true;
	m_MotionEndThreadData.nWaitTime		= 5000; //ms
	m_MotionEndThreadData.nAxisIndex	= -1; // axis theta index
	m_MotionEndThreadData.nGantryIndex	= -1;
	m_MotionEndThreadData.pThisPtr		= this;

	AfxBeginThread(MotionEndThread_Func, static_cast<void*>(&m_MotionEndThreadData));

	return TRUE;
}

BOOL CMotorControl::AxisThetaJogPlus()
{
	int nJogAddr = (int)GetAxisThetaJogCommandAddr();
	if (nJogAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nJogAddr, (long)1);
}

BOOL CMotorControl::AxisThetaJogMinus()
{
	int nJogAddr = (int)GetAxisThetaJogCommandAddr();
	if (nJogAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nJogAddr, (long)2);
}

BOOL CMotorControl::AxisThetaJogStop()
{
	int nJogAddr = (int)GetAxisThetaJogCommandAddr();
	if (nJogAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nJogAddr, (long)0);
}

BOOL CMotorControl::AxisThetaManualGoSpeed(double dSpeed)
{
	int nAddr = GetAxisThetaManualGoSpeedAddr();

	if (nAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nAddr, dSpeed);
}

BOOL CMotorControl::AxisThetaJogSpeed(double dSpeed)
{
	int nAddr = GetAxisThetaJogSpeedAddr();

	if (nAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nAddr, dSpeed);
}

BOOL CMotorControl::AxisThetaManualGoAccel(int nAccel)
{
	int nAddr = GetAxisThetaManualGoAccelAddr();

	if (nAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nAddr, (long)nAccel);
}

BOOL CMotorControl::AxisThetaJogAccel(int nAccel)
{
	int nAddr = GetAxisThetaJogAccelAddr();

	if (nAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nAddr, (long)nAccel);
}


// gantry
BOOL CMotorControl::GantryManualGo(int nGantryIdx, const VectorDouble& vectorAxisPos)
{	
	UINT nManualAddr = GetGantryManualGoAddr(nGantryIdx);
	if (nManualAddr==ADDRESS_NONE) return FALSE;

	int nAxisCount = 0;
	for (int i=0; i<nGantryIdx; i++)
	{
		nAxisCount += GetGantryAxisCount(i);
	}

	long nValue = 0;
	for (int i=0; i<(int)vectorAxisPos.size(); i++)
	{
		UINT nGoPosAddr = GetAxisGoPositionAddr(nAxisCount+i);
		if (nGoPosAddr==ADDRESS_NONE) return FALSE;

		if (WriteAddressValue(nGoPosAddr, vectorAxisPos[i])==FALSE)
		{
			return FALSE;
		}

		nValue = nValue | (1 << i);
	}

	return WriteAddressValue(nManualAddr, nValue);
}

BOOL CMotorControl::GantryManualGo(int nGantryIdx, const VectorDouble& vectorAxisPos, int nMoveType)
{
	UINT nManualAddr = GetGantryManualGoAddr(nGantryIdx);
	if (nManualAddr==ADDRESS_NONE) return FALSE;

	int nAxisCount = 0;
	for (int i=0; i<nGantryIdx; i++)
	{
		nAxisCount += GetGantryAxisCount(i);
	}

	int nBitData = 1;
	for (int i=0; i<GetGantryAxisCount(nGantryIdx); i++)
	{
		if ( nBitData & nMoveType ) 
		{
			UINT nGoPosAddr = GetAxisGoPositionAddr(nAxisCount+i);
			if (nGoPosAddr!=ADDRESS_NONE) 
			{
				if (WriteAddressValue(nGoPosAddr, vectorAxisPos[i])==FALSE)
				{
					continue;
				}
			}
		}

		nBitData = nBitData << 1;
	}
	Sleep(700);
	BOOL bResult = WriteAddressValue(nManualAddr, (long) nMoveType);
	
	//return WriteAddressValue(nManualAddr, (long) nMoveType);
	return bResult;
}

BOOL CMotorControl::GantryManualGo(int nGantryIdx, double dPosX, double dPosY)
{
	UINT nManualAddr = GetGantryManualGoAddr(nGantryIdx);
	if (nManualAddr==ADDRESS_NONE) return FALSE;

	int nAxisCount = 0;
	for (int i=0; i<nGantryIdx; i++)
	{
		nAxisCount += GetGantryAxisCount(i);
	}

	UINT nXGoPosAddr = GetAxisGoPositionAddr(nAxisCount);
	if (nXGoPosAddr==ADDRESS_NONE) return FALSE;

	UINT nYGoPosAddr = GetAxisGoPositionAddr(nAxisCount+1);
	//UINT nYGoPosAddr = GetAxisGoPositionAddr(nAxisCount+2);	// lmk motor y2
	if (nYGoPosAddr==ADDRESS_NONE) return FALSE;

	if (WriteAddressValue(nXGoPosAddr, dPosX)==FALSE)
	{
		return FALSE;
	}

	if (WriteAddressValue(nYGoPosAddr, dPosY)==FALSE)
	{
		return FALSE;
	}

	return WriteAddressValue(nManualAddr, (long)3);
	//return WriteAddressValue(nManualAddr, (long)5);	// lmk motor y2
}


BOOL CMotorControl::GantryManualGoEnd(int nGantryIdx, const VectorDouble& vectorPos)
{
	if (GantryManualGo(nGantryIdx, vectorPos)==FALSE) return FALSE;

	if (m_MotionEndThreadData.bRunning==TRUE) return FALSE;

	m_MotionEndThreadData.bRunning		= true;
	m_MotionEndThreadData.nWaitTime		= 5000; //ms
	m_MotionEndThreadData.nAxisIndex	= -1;
	m_MotionEndThreadData.nGantryIndex	= nGantryIdx;
	m_MotionEndThreadData.pThisPtr		= this;

	AfxBeginThread(MotionEndThread_Func, static_cast<void*>(&m_MotionEndThreadData));

	return TRUE;
}

BOOL CMotorControl::GantryManualGoEnd(int nGantryIdx, const VectorDouble& vectorPos, int nMoveType)
{
	if (GantryManualGo(nGantryIdx, vectorPos, nMoveType)==FALSE) return FALSE;

	if (m_MotionEndThreadData.bRunning==TRUE) return FALSE;

	m_MotionEndThreadData.bRunning		= true;
	m_MotionEndThreadData.nWaitTime		= 5000; //ms
	m_MotionEndThreadData.nAxisIndex	= -1;
	m_MotionEndThreadData.nGantryIndex	= nGantryIdx;
	m_MotionEndThreadData.pThisPtr		= this;

	AfxBeginThread(MotionEndThread_Func, static_cast<void*>(&m_MotionEndThreadData));

	return TRUE;
}

BOOL CMotorControl::GantryManualGoEnd(int nGantryIdx, double dPosX, double dPosY)
{
	if (GantryManualGo(nGantryIdx, dPosX, dPosY)==FALSE) return FALSE;

	if (m_MotionEndThreadData.bRunning==TRUE) return FALSE;

	m_MotionEndThreadData.bRunning		= true;
	m_MotionEndThreadData.nWaitTime		= 5000; //ms
	m_MotionEndThreadData.nAxisIndex	= -1;
	m_MotionEndThreadData.nGantryIndex	= nGantryIdx;
	m_MotionEndThreadData.pThisPtr		= this;

	AfxBeginThread(MotionEndThread_Func, static_cast<void*>(&m_MotionEndThreadData));

	return TRUE;
}

BOOL CMotorControl::IsAxisMotionEnd(int nAxisIdx)
{
	// axis theta
	if (m_motorCommand.bUseThetaAxis && nAxisIdx==-1)
	{
		if( (m_axisThetaStatus & 0x01) == 0)
		{
			return FALSE;
		}
		return TRUE;
	}

	// axis
	if (nAxisIdx<0 || nAxisIdx>=m_nAxisCount) return FALSE;

	if( (m_pAxisStatus[nAxisIdx] & 0x01) == 0)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMotorControl::IsGantryMotionEnd(int nGantryIdx)
{
	int nAxisStart = 0;
	for (int i=0; i<nGantryIdx; i++)
	{
		nAxisStart += GetGantryAxisCount(i);
	}

	int nAxisEnd = nAxisStart + GetGantryAxisCount(nGantryIdx);

	for (int i=nAxisStart; i<nAxisEnd; i++)
	{
		if( (m_pAxisStatus[i] & 0x01) == 0)
		{
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CMotorControl::GantryAutoGo(int nGantryIdx, const VectorDouble& vectorPosX, const VectorDouble& vectorPosY, BOOL bCommand)
{
	double dPlusLimitX, dMinusLimitX, dPlusLimitY, dMinusLimitY;
	int nPointCount = (int)vectorPosX.size();

	if (nPointCount > MAX_MOTOR_ADDRESS_SIZE) return FALSE;

	if (nPointCount<1 || vectorPosY.size()<1) return FALSE;

	if (nPointCount!=(int)vectorPosY.size()) return FALSE;

	int nStartAxisIdx = 0;
	for (int i=0; i<nGantryIdx; i++)
	{
		nStartAxisIdx += GetGantryAxisCount(i);
	}

	int nGoPosAddrX = GetAxisGoPositionAddr(nStartAxisIdx);
	if (nGoPosAddrX==ADDRESS_NONE) return FALSE;

	int nGoPosAddrY = GetAxisGoPositionAddr(nStartAxisIdx+1);
	if (nGoPosAddrY==ADDRESS_NONE) return FALSE;

	GetAxisLimitPosition(nStartAxisIdx, dPlusLimitX, dMinusLimitX);
	GetAxisLimitPosition(nStartAxisIdx+1, dPlusLimitY, dMinusLimitY);

	BOOL bSuccess = TRUE;
	float fXPos[MAX_MOTOR_ADDRESS_SIZE], fYPos[MAX_MOTOR_ADDRESS_SIZE];
	ZeroMemory(fXPos, sizeof(float)*MAX_MOTOR_ADDRESS_SIZE);
	ZeroMemory(fYPos, sizeof(float)*MAX_MOTOR_ADDRESS_SIZE);

	for (int i=0; i<nPointCount; i++)
	{
		if(vectorPosX[i] > dMinusLimitX && vectorPosX[i] < dPlusLimitX)
		{
			fXPos[i] = float(vectorPosX[i]);
		}
		else
		{
			fXPos[i] = 0.0f;
			if (m_pMC2P)
			{
				m_pMC2P->MC2P_DisplayMessage(_T("X MotorPosition Idx[%d] Position[%.03lf] Limit"), i, vectorPosX[i]);
			}
		}

		if(vectorPosY[i] > dMinusLimitY && vectorPosY[i] < dPlusLimitY)
		{
			fYPos[i] = float(vectorPosY[i]);
		}
		else
		{
			fYPos[i] = 0.0f;
			if (m_pMC2P)
			{
				m_pMC2P->MC2P_DisplayMessage(_T("Y MotorPosition Idx[%d] Position[%.03lf] Limit"), i, vectorPosY[i]);
			}
		}
	}
	
	// write x pos
	if (WriteAddressValue(nGoPosAddrX, fXPos, nPointCount)==FALSE)
	{
		return FALSE;
	}

	// write y pos
	if (WriteAddressValue(nGoPosAddrY, fYPos, nPointCount)==FALSE)
	{
		return FALSE;
	}

	// count
	int nMoveCountAddr = (int)GetGantryMoveCountAddr(nGantryIdx);
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;
	if (WriteAddressValue(nMoveCountAddr, (long)nPointCount)==FALSE)
	{
		return FALSE;
	}
	
	if (bCommand==FALSE) 
	{
		return TRUE;
	}
	
	// command
	int nMoveCmdAddr = (int)GetGantryAutoGoAddr(nGantryIdx);

	return WriteAddressValue(nMoveCmdAddr, (long)1);
}

BOOL CMotorControl::GantryAutoGo(int nGantryIdx)
{
	int nGoCmdAddr = (int)GetGantryAutoGoAddr(nGantryIdx);

	if (nGoCmdAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nGoCmdAddr, (long)1);
}

BOOL CMotorControl::GantrySetMoveCount(int nGantryIdx, long nCount)
{
	UINT nMoveCountAddr = GetGantryMoveCountAddr(nGantryIdx);
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nMoveCountAddr, nCount);
}

BOOL CMotorControl::GantryGetMoveCount(int nGantryIdx, long& nCount)
{
	UINT nCountAddr = GetGantryMoveCountAddr(nGantryIdx);
	if (nCountAddr==ADDRESS_NONE) return FALSE;

	return ReadAddressValue(nCountAddr, nCount);
}

BOOL CMotorControl::GantryGetTriggerCount(int nGantryIdx, long& nCount)
{
	UINT nCountAddr = GetGantryTriggerCountAddr(nGantryIdx);
	if (nCountAddr==ADDRESS_NONE) return FALSE;

	return ReadAddressValue(nCountAddr, nCount);
}

BOOL CMotorControl::GantryMeasureAutoGo(int nGantryIdx, const VectorDouble& vectorPosData, BOOL bCommand)
{
	int nGantryAxisCount = GetGantryAxisCount(nGantryIdx);

	int nTotalPointCount = (int)vectorPosData.size();

	if ( (nTotalPointCount%nGantryAxisCount)!=0 ) return FALSE;

	int nPointCount = nTotalPointCount / nGantryAxisCount;

	if (nPointCount > MAX_MOTOR_ADDRESS_SIZE) return FALSE;

	int nStartAxisIdx = 0;
	for (int i=0; i<nGantryIdx; i++)
	{
		nStartAxisIdx += GetGantryAxisCount(i);
	}
	
	BOOL bSuccess = TRUE;
	double dPosData = 0.0;
	double dPlusLimit, dMinusLimit;
	float fPosData[MAX_MOTOR_ADDRESS_SIZE];
	memset(fPosData, 0, sizeof(float)*MAX_MOTOR_ADDRESS_SIZE);

	for (int nAxisIdx=0; nAxisIdx<nGantryAxisCount; nAxisIdx++)
	{
		int nGoPosAddr = GetAxisGoPositionAddr(nStartAxisIdx+nAxisIdx);
		if (nGoPosAddr==ADDRESS_NONE) 
		{
			return FALSE;
		}

		GetAxisLimitPosition(nStartAxisIdx+nAxisIdx, dPlusLimit, dMinusLimit);

		for (int nPosIdx=0; nPosIdx<nPointCount; nPosIdx++)
		{
			dPosData = vectorPosData[(nPosIdx*nGantryAxisCount)+nAxisIdx];

			if(dPosData > dMinusLimit && dPosData < dPlusLimit)
			{
				fPosData[nPosIdx] = float(dPosData);
			}
			else
			{
				fPosData[nPosIdx] = 0.0;
				if (m_pMC2P)
				{
					m_pMC2P->MC2P_DisplayMessage(_T("MotorPosition Idx[%d] Position[%.03lf] Limit"), nAxisIdx, dPosData);
				}
			}
		}

		// write y pos
		if (WriteAddressValue(nGoPosAddr, fPosData, nPointCount)==FALSE)
		{
			return FALSE;
		}
	}
	
	// count
	int nMoveCountAddr = (int)GetGantryMeasureMoveCountAddr(nGantryIdx);
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;
	if (WriteAddressValue(nMoveCountAddr, (long)nPointCount)==FALSE)
	{
		return FALSE;
	}

	if (bCommand==FALSE) 
	{
		return TRUE;
	}

	// command
	int nMoveCmdAddr = (int)GetGantryMeasureAutoGoAddr(nGantryIdx);

	return WriteAddressValue(nMoveCmdAddr, (long)1);
}

BOOL CMotorControl::GantryMeasureAutoGo(int nGantryIdx)
{
	int nGoCmdAddr = (int)GetGantryMeasureAutoGoAddr(nGantryIdx);

	if (nGoCmdAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nGoCmdAddr, (long)1);
}

BOOL CMotorControl::GantrySetMeasureMoveCount(int nGantryIdx, long nCount)
{
	UINT nMoveCountAddr = GetGantryMeasureMoveCountAddr(nGantryIdx);
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nMoveCountAddr, nCount);
}

BOOL CMotorControl::GantryGetMeasureMoveCount(int nGantryIdx, long& nCount)
{
	UINT nCountAddr = GetGantryMeasureMoveCountAddr(nGantryIdx);
	if (nCountAddr==ADDRESS_NONE) return FALSE;

	return ReadAddressValue(nCountAddr, nCount);
}

BOOL CMotorControl::GantryGetMeasureTriggerCount(int nGantryIdx, long& nCount)
{
	UINT nCountAddr = GetGantryMeasureTriggerCountAddr(nGantryIdx);
	if (nCountAddr==ADDRESS_NONE) return FALSE;

	return ReadAddressValue(nCountAddr, nCount);
}

BOOL CMotorControl::GantryOtherAutoGo(int nGantryIdx, const VectorDouble& vectorPosX, const VectorDouble& vectorPosY, BOOL bCommand)
{
	double dPlusLimitX, dMinusLimitX, dPlusLimitY, dMinusLimitY;
	int nPointCount = (int)vectorPosX.size();

	if (nPointCount > MAX_MOTOR_ADDRESS_SIZE) return FALSE;

	if (nPointCount<1 || vectorPosY.size()<1) return FALSE;

	if (nPointCount!=(int)vectorPosY.size()) return FALSE;

	int nStartAxisIdx = 0;
	for (int i=0; i<nGantryIdx; i++)
	{
		nStartAxisIdx += GetGantryAxisCount(i);
	}

	int nGoPosAddrX = GetAxisGoPositionAddr(nStartAxisIdx);
	if (nGoPosAddrX==ADDRESS_NONE) return FALSE;

	int nGoPosAddrY = GetAxisGoPositionAddr(nStartAxisIdx+1);
	if (nGoPosAddrY==ADDRESS_NONE) return FALSE;

	GetAxisLimitPosition(nStartAxisIdx, dPlusLimitX, dMinusLimitX);
	GetAxisLimitPosition(nStartAxisIdx+1, dPlusLimitY, dMinusLimitY);

	BOOL bSuccess = TRUE;
	float fXPos[MAX_MOTOR_ADDRESS_SIZE], fYPos[MAX_MOTOR_ADDRESS_SIZE];
	ZeroMemory(fXPos, sizeof(float)*MAX_MOTOR_ADDRESS_SIZE);
	ZeroMemory(fYPos, sizeof(float)*MAX_MOTOR_ADDRESS_SIZE);

	for (int i=0; i<nPointCount; i++)
	{
		if(vectorPosX[i] > dMinusLimitX && vectorPosX[i] < dPlusLimitX)
		{
			fXPos[i] = float(vectorPosX[i]);
		}
		else
		{
			fXPos[i] = 0.0f;
			if (m_pMC2P)
			{
				m_pMC2P->MC2P_DisplayMessage(_T("X MotorPosition Idx[%d] Position[%.03lf] Limit"), i, vectorPosX[i]);
			}
		}

		if(vectorPosY[i] > dMinusLimitY && vectorPosY[i] < dPlusLimitY)
		{
			fYPos[i] = float(vectorPosY[i]);
		}
		else
		{
			fYPos[i] = 0.0f;
			if (m_pMC2P)
			{
				m_pMC2P->MC2P_DisplayMessage(_T("Y MotorPosition Idx[%d] Position[%.03lf] Limit"), i, vectorPosY[i]);
			}
		}
	}

	// write x pos
	if (WriteAddressValue(nGoPosAddrX, fXPos, nPointCount)==FALSE)
	{
		return FALSE;
	}

	// write y pos
	if (WriteAddressValue(nGoPosAddrY, fYPos, nPointCount)==FALSE)
	{
		return FALSE;
	}

	if (bCommand==FALSE) return TRUE;

	
	int nMoveCountAddr = (int)GetGantryOtherMoveCountAddr(nGantryIdx);
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;
	if (WriteAddressValue(nMoveCountAddr, (long)nPointCount)==FALSE)
	{
		return FALSE;
	}

	if (bCommand==FALSE) 
	{
		return TRUE;
	}


	int nGoCmdAddr = (int)GetGantryOtherAutoGoAddr(nGantryIdx);
	//int nGoCmdAddr = (int)GetGantryOtherAutoGoAddr((long)1);
	if (nGoCmdAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nGoCmdAddr, (long)1);
}

BOOL CMotorControl::GantryOtherAutoGo(int nGantryIdx)
{
	int nGoCmdAddr = (int)GetGantryOtherAutoGoAddr(nGantryIdx);

	if (nGoCmdAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nGoCmdAddr, (long)1);
}

BOOL CMotorControl::GantrySetOtherMoveCount(int nGantryIdx, long nCount)
{
	UINT nMoveCountAddr = GetGantryOtherMoveCountAddr(nGantryIdx);
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(nMoveCountAddr, nCount);
}

BOOL CMotorControl::GantryGetOtherMoveCount(int nGantryIdx, long& nCount)
{
	UINT nCountAddr = GetGantryOtherMoveCountAddr(nGantryIdx);
	if (nCountAddr==ADDRESS_NONE) return FALSE;

	return ReadAddressValue(nCountAddr, nCount);
}

BOOL CMotorControl::GantryGetOtherTriggerCount(int nGantryIdx, long& nCount)
{
	UINT nCountAddr = GetGantryOtherTriggerCountAddr(nGantryIdx);
	if (nCountAddr==ADDRESS_NONE) return FALSE;

	return ReadAddressValue(nCountAddr, nCount);
}


// common
BOOL CMotorControl::CommonSetAFMDelayTime(long nTime)
{
	if (!m_bConnected) return FALSE;

	if (m_motorCommand.nPrevDelayTimeAddr==ADDRESS_NONE) return FALSE;

	if(nTime < 1)
	{
		return WriteAddressValue(m_motorCommand.nPrevDelayTimeAddr, (long)m_motorCommand.nPrevDelayTime);
	}

	return WriteAddressValue(m_motorCommand.nPrevDelayTimeAddr, nTime);
}

BOOL CMotorControl::CommonSetSnapDelayTime(long nTime)
{
	if (!m_bConnected) return FALSE;

	if (m_motorCommand.nPostDelayTimeAddr==ADDRESS_NONE) return FALSE;

	if(nTime < 1)
	{
		return WriteAddressValue(m_motorCommand.nPostDelayTimeAddr, (long)m_motorCommand.nPostDelayTime);
	}

	return WriteAddressValue(m_motorCommand.nPostDelayTimeAddr, nTime);
}


BOOL CMotorControl::CommonSetWSIDelayTime(long nTime)
{
	if (!m_bConnected) return FALSE;

	if (m_motorCommand.nOtherDelayTimeAddr==ADDRESS_NONE) return FALSE;

	if(nTime < 1)
	{
		return WriteAddressValue(m_motorCommand.nOtherDelayTimeAddr, (long)m_motorCommand.nOtherDelayTime);
	}

	return WriteAddressValue(m_motorCommand.nOtherDelayTimeAddr, nTime);
}

BOOL CMotorControl::CommonGetAFMDelayTime(long& nTime)
{
	if (!m_bConnected) return FALSE;

	if (m_motorCommand.nPrevDelayTimeAddr==ADDRESS_NONE) return FALSE;

	return ReadAddressValue(m_motorCommand.nPrevDelayTimeAddr, nTime);
}

BOOL CMotorControl::CommonGetSnapDelayTime(long& nTime)
{
	if (!m_bConnected) return FALSE;

	if (m_motorCommand.nPostDelayTimeAddr==ADDRESS_NONE) return FALSE;

	return ReadAddressValue(m_motorCommand.nPostDelayTimeAddr, nTime);
}

BOOL CMotorControl::CommonGetWSIDealyTime(long& nTime)
{
	if (!m_bConnected) return FALSE;

	if (m_motorCommand.nOtherDelayTimeAddr==ADDRESS_NONE) return FALSE;

	return ReadAddressValue(m_motorCommand.nOtherDelayTimeAddr, nTime);
}

BOOL CMotorControl::CommonSetAutoStop()
{
	if (!m_bConnected) return FALSE;

	if (m_motorCommand.nAutoStop==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(m_motorCommand.nAutoStop, (long)1);
}

BOOL CMotorControl::CommonSetAutoGoAll()
{
	if (!m_bConnected) return FALSE;

	if (m_motorCommand.nAllGoAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(m_motorCommand.nAllGoAddr, (long)1);
}

BOOL CMotorControl::CommonSetAutoMeasureGoAll()
{
	if (!m_bConnected) return FALSE;

	if (m_motorCommand.nAllMeasureGoAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(m_motorCommand.nAllMeasureGoAddr, (long)1);
}

BOOL CMotorControl::CommonSetAutoWsiGoAll()
{
	if (!m_bConnected) return FALSE;

	if (m_motorCommand.nAllWsiGoAddr==ADDRESS_NONE) return FALSE;

	return WriteAddressValue(m_motorCommand.nAllWsiGoAddr, (long)1);
}

BOOL CMotorControl::IsMotionEnd()
{
	BOOL bRet = TRUE;
	int i;

	for(i = 0; i < m_nAxisCount; i++)
	{
		if(m_pAxisStatus[i] == FALSE)
		{
			bRet = FALSE;
			break;
		}
	}

	return bRet;
}

BOOL CMotorControl::RemoveAllAutoGo(int nGantryIdx)
{	
	/*int nGantryAxisCount = GetGantryAxisCount(nGantryIdx);	

	int nPointCount = 99;

	if (nPointCount > MAX_MOTOR_ADDRESS_SIZE) return FALSE;

	int nStartAxisIdx = 0;
	for (int i=0; i<nGantryIdx; i++)
	{
		nStartAxisIdx += GetGantryAxisCount(i);
	}

	BOOL bSuccess = TRUE;
	double dPosData = 0.0;
	double dPlusLimit, dMinusLimit;
	float fPosData[MAX_MOTOR_ADDRESS_SIZE];
	memset(fPosData, 0, sizeof(float)*MAX_MOTOR_ADDRESS_SIZE);

	for (int nAxisIdx=0; nAxisIdx<nGantryAxisCount; nAxisIdx++)
	{
		int nGoPosAddr = GetAxisGoPositionAddr(nStartAxisIdx+nAxisIdx);
		if (nGoPosAddr==ADDRESS_NONE) 
		{
			return FALSE;
		}

		GetAxisLimitPosition(nStartAxisIdx+nAxisIdx, dPlusLimit, dMinusLimit);

		for (int nPosIdx=0; nPosIdx<nPointCount; nPosIdx++)
		{
			dPosData = 0.0;

			if(dPosData > dMinusLimit && dPosData < dPlusLimit)
			{
				fPosData[nPosIdx] = 0.0;
			}
			else
			{
				fPosData[nPosIdx] = 0.0;
				if (m_pMC2P)
				{
					m_pMC2P->MC2P_DisplayMessage(_T("MotorPosition Idx[%d] Position[%.03lf] Limit"), nAxisIdx, dPosData);
				}
			}
		}

		// write y pos
		if (WriteAddressValue(nGoPosAddr, fPosData, nPointCount)==FALSE)
		{
			return FALSE;
		}
	}*/

	// count
	int nMoveCountAddr = (int)GetGantryMeasureMoveCountAddr(nGantryIdx);
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;
	if (WriteAddressValue(nMoveCountAddr, (long)0)==FALSE)
	{
		return FALSE;
	}	

	nMoveCountAddr = (int)GetGantryMoveCountAddr(nGantryIdx); 
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;
	if (WriteAddressValue(nMoveCountAddr, (long)0)==FALSE)
	{
		return FALSE;
	}	

	nMoveCountAddr = (int)GetGantryOtherMoveCountAddr(nGantryIdx);
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;
	if (WriteAddressValue(nMoveCountAddr, (long)0)==FALSE)
	{
		return FALSE;
	}	

	//Trigger
	nMoveCountAddr = (int)GetGantryMeasureTriggerCountAddr(nGantryIdx);
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;
	if (WriteAddressValue(nMoveCountAddr, (long)0)==FALSE)
	{
		return FALSE;
	}	

	nMoveCountAddr = (int)GetGantryTriggerCountAddr(nGantryIdx); 
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;
	if (WriteAddressValue(nMoveCountAddr, (long)0)==FALSE)
	{
		return FALSE;
	}	

	nMoveCountAddr = (int)GetGantryOtherTriggerCountAddr(nGantryIdx);
	if (nMoveCountAddr==ADDRESS_NONE) return FALSE;
	if (WriteAddressValue(nMoveCountAddr, (long)0)==FALSE)
	{
		return FALSE;
	}	
}