#include "StdAfx.h"
#include "PlusTek_STB64C.h"
#include "LightTransmitPlustech.h"

CPlusTek_STB64C::CPlusTek_STB64C(int nIndex, int nControlType) : CLightControl(nIndex, nControlType)
{
	m_nLightLevel = 0;
	m_nLightStatus = 0;
	m_pLightControl = new CLightTransmitPlustech();
}

CPlusTek_STB64C::~CPlusTek_STB64C(void)
{
	Disconnect();

	if (m_pLightControl)
	{
		delete m_pLightControl;
		m_pLightControl = NULL;
	}
}

BOOL CPlusTek_STB64C::Connect(const CString& strPort, int nBaud)
{
	if (NULL == m_pLightControl) return FALSE;

	if (TRUE == m_pLightControl->IsOpened()) 
	{
		return FALSE;
	}

	int nPort = _ttoi(strPort);

	nBaud = 115200;
	if(FALSE == m_pLightControl->OpenControl(nPort, nBaud))
	{
		return FALSE;
	}	

	return m_pLightControl->IsTurnOnLamp(FALSE);
}

void CPlusTek_STB64C::Disconnect()
{
	if (NULL == m_pLightControl) return;

	if (TRUE == m_pLightControl->IsOpened()) return;

	m_pLightControl->CloseControl();
}

BOOL CPlusTek_STB64C::SetLightOn()
{
	if (NULL == m_pLightControl) return FALSE;

	if (FALSE == m_pLightControl->IsOpened()) return FALSE;

	return m_pLightControl->IsTurnOnLamp(TRUE);
}

BOOL CPlusTek_STB64C::SetLightOff()
{
	if (NULL == m_pLightControl) return FALSE;

	if (FALSE == m_pLightControl->IsOpened()) return FALSE;

	return m_pLightControl->IsTurnOnLamp(FALSE);
}

BOOL CPlusTek_STB64C::SetLightStatus(int nValue, int nChannel)
{
	if (NULL == m_pLightControl) return FALSE;

	if (FALSE == m_pLightControl->IsOpened()) return FALSE;

	if (nChannel<0 || nChannel>=MAX_TRANSMIT_CH) return FALSE;

	m_nLightStatus = nValue;

	return TRUE;
}

BOOL CPlusTek_STB64C::GetLightStatus(int& nValue, int nChannel)
{
	if (NULL == m_pLightControl) return FALSE;

	if (FALSE == m_pLightControl->IsOpened()) return FALSE;

	if (nChannel<0 || nChannel>=MAX_TRANSMIT_CH) return FALSE;

	nValue = m_nLightStatus;

	return TRUE;
}

BOOL CPlusTek_STB64C::SetLightLevel(int nValue, int nChannel)
{
	if (NULL == m_pLightControl) return FALSE;

	if (FALSE == m_pLightControl->IsOpened()) return FALSE;

	if (nChannel<0 || nChannel>=MAX_TRANSMIT_CH) return FALSE;

	int nOnTime = STROBE_DEFAULT_ONTIME;

	DATA_SubConfig_type *pCF;
	DATA_SubControl_type *pLV;

	for(int iDrv=0; iDrv<MAX_TRANSMIT_ID; iDrv++)
	{
		for(int iCh=0; iCh<MAX_TRANSMIT_CH; iCh++)
		{

			pCF = m_pLightControl->GetTransmitLightConfig(iDrv, iCh);
			pLV = m_pLightControl->GetTransmitLightValue(iDrv, iCh);
			if(pCF == NULL || pLV == NULL)
				continue;

			pLV->volt = nValue;
			pLV->time = nOnTime;

			if(pLV->volt > TRANSMIT_MAX_VOLT_VALUE)
				pLV->volt = TRANSMIT_MAX_VOLT_VALUE;
			if(pLV->time > STROBE_DEFAULT_ONTIME)
				pLV->time = STROBE_DEFAULT_ONTIME;
		}
	}
	
	if (FALSE==m_pLightControl->SetLightControlValue())
	{
		return FALSE;
	}

	m_nLightLevel = nValue;

	return TRUE;
	
}

BOOL CPlusTek_STB64C::SetLightLevel(double dValue, int nChannel)
{
	int nValue = int(dValue+0.5);

	return SetLightLevel(nValue, nChannel);
}

BOOL CPlusTek_STB64C::GetLightLevel(int &nValue, int Channel)
{
	if (NULL == m_pLightControl) return FALSE;

	if (FALSE == m_pLightControl->IsOpened()) return FALSE;

	nValue = m_nLightLevel;

	return TRUE;	
}

BOOL CPlusTek_STB64C::GetLightLevel(double &dValue, int Channel)
{
	int nValue = 0;
	
	if (FALSE == GetLightLevel(nValue))
	{
		return FALSE;
	}

	dValue = nValue;

	return TRUE;
}