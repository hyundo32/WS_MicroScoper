// LightTransmitPlustech.cpp: implementation of the CLightTransmitPlustech class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LightTransmitPlustech.h"
//#include "HMConfigFile.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLightTransmitPlustech::CLightTransmitPlustech()
{
	int		drv_idx,sub_idx,i;
	for(i=0;i<MAX_TRANSMIT_ID*MAX_TRANSMIT_CH;i++)
	{
		drv_idx = i / MAX_TRANSMIT_ID;
		sub_idx = i & 0x07;

		m_StuLightValue.drv[drv_idx].sub[sub_idx].volt = (unsigned short)0;
		m_StuLightValue.drv[drv_idx].sub[sub_idx].time = STROBE_DEFAULT_ONTIME;
		
		m_StuDrvConfigType.drv[drv_idx].sub[sub_idx].mode = 1;
		m_StuDrvConfigType.drv[drv_idx].sub[sub_idx].onoff = 0;
		m_StuDrvConfigType.drv[drv_idx].sub[sub_idx].pol = 0;
		m_StuDrvConfigType.drv[drv_idx].sub[sub_idx].trg_sel = 0;
		m_StuDrvConfigType.drv[drv_idx].sub[sub_idx].volt_min = 500;
		m_StuDrvConfigType.drv[drv_idx].sub[sub_idx].volt_max = 700;
	}

	m_bConnected = FALSE;
	m_dwPort = 0;
	m_dwBaud = 0;
}

CLightTransmitPlustech::~CLightTransmitPlustech()
{
	CloseControl();
}

BOOL CLightTransmitPlustech::OpenControl(void)
{
	return OpenControl(m_dwPort, m_dwBaud);
}

BOOL CLightTransmitPlustech::OpenControl(int nPort, int nBaud)
{
	m_dwPort = nPort;
	m_dwBaud = nBaud;

	if(nBaud == 0)
		nBaud = 115200;

	CString		strProt;

	strProt.Format(_T("COM%d"), m_dwPort);

	if(Open(strProt,NULL) != ERROR_SUCCESS)
	{
		m_bConnected = FALSE;
		//g_pLog->DisplayMessage(TRUE,"연결 실패 : Port %s, Baud %d",strProt,m_dwPort);
		return FALSE;
	}

	Setup((EBaudrate)m_dwBaud,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	SetupHandshaking(CSerial::EHandshakeOff);

	m_bConnected = TRUE;

	return TRUE;
}

BOOL CLightTransmitPlustech::CloseControl(void)
{	
	Close();
	return TRUE;
}

BOOL CLightTransmitPlustech::SetTransmitLightValue(int iDrv,int iCh,unsigned short volt,unsigned short time)
{
	if(iDrv >= MAX_TRANSMIT_ID || iCh >= MAX_TRANSMIT_CH)
		return FALSE;

	m_StuLightValue.drv[iDrv].sub[iCh].volt = volt;
	m_StuLightValue.drv[iDrv].sub[iCh].time = time;

	return TRUE;
}

DATA_SubControl_type *CLightTransmitPlustech::GetTransmitLightValue(int iDrv,int iCh)
{
	if(iDrv >= MAX_TRANSMIT_ID || iCh >= MAX_TRANSMIT_CH)
		return NULL;

	return &m_StuLightValue.drv[iDrv].sub[iCh];
}

DATA_SubConfig_type *CLightTransmitPlustech::GetTransmitLightConfig(int iDrv,int iCh)
{
	if(iDrv >= MAX_TRANSMIT_ID || iCh >= MAX_TRANSMIT_CH)
		return NULL;

	return &m_StuDrvConfigType.drv[iDrv].sub[iCh];
}

BOOL CLightTransmitPlustech::SetLightControlValue()
{
	if(IsOpened() == FALSE)
		return FALSE;	
	
	return MakePacket(TRANSMIT_CONTROLLER_ID, 'I',(char*) &m_StuLightValue, sizeof(m_StuLightValue));
}

BOOL CLightTransmitPlustech::IsTurnOnLamp(BOOL bOn)
{
	if(IsOpened() == FALSE)
		return FALSE;

	CString			strData;	
	int             i;
	int             drv_idx, sub_idx;

	for(i=0;i<MAX_TRANSMIT_ID*MAX_TRANSMIT_CH;i++)
	{
		drv_idx = i / MAX_TRANSMIT_ID;
		sub_idx = i & 0x07;

		m_StuDrvConfigType.drv[drv_idx].sub[sub_idx].onoff = bOn;		
	}
	
	return MakePacket(TRANSMIT_CONTROLLER_ID, 'D',(char*) &m_StuDrvConfigType, sizeof(m_StuDrvConfigType));
}

BOOL CLightTransmitPlustech::SetReset()
{
	if(IsOpened() == FALSE)
		return FALSE;	

	return MakePacket(TRANSMIT_CONTROLLER_ID, 'R',(char*)NULL, 0);
}

BOOL CLightTransmitPlustech::MakePacket(unsigned char did, unsigned char cmd, char *pdat, unsigned short dat_len)
{
	STRB_BasicPacket_type       bas_pkt;
	char                        *_tm, tmp;
	int                         i;
	unsigned short              crc;

	bas_pkt.pre = STDP_PREAMBLE;
	bas_pkt.typ = 1;
	bas_pkt.cmd = cmd;

	bas_pkt.o_id = 0x0C; // I am Normal GUI
	bas_pkt.d_id = did;

	bas_pkt.len = PACKET_LENGTH(dat_len);

	unsigned char *pPacket = new unsigned char[bas_pkt.len];
	ZeroMemory(pPacket,sizeof(unsigned char)*bas_pkt.len);
	int			iPacket = 0;

	_tm = (char *) &bas_pkt;
	for(i=0; i < 10;i++)
	{
		pPacket[iPacket++] = *_tm++;
	}

	if( dat_len > 0 && pdat != NULL )
	{
		_tm = (char *) pdat;
		for(i=0;i < dat_len;i++)
		{
			pPacket[iPacket++] = *_tm++;
		}
	}
	
	crc = GetCRC16T(pPacket, iPacket);

	tmp = (unsigned char) (crc >> 8);
	pPacket[iPacket++] = tmp;
	tmp = (unsigned char) crc;
	pPacket[iPacket++] = tmp;

	BOOL		bRet = TRUE;

	if(Write(pPacket, bas_pkt.len) != ERROR_SUCCESS)
	{
		//g_pLog->DisplayMessage(TRUE,"Send Light Fail");
		bRet = FALSE;
	}

	if(pPacket != NULL)
		delete[] pPacket, pPacket = NULL;

	return bRet;
}

const unsigned short CRC_16_Table[] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

unsigned short CLightTransmitPlustech::GetCRC16T(unsigned char *pStart, int len)
{
	unsigned short	crc16 = 0;

	while(len--)		crc16 = CRC_16_Table[(crc16 >> 8) ^ *pStart++] ^ (crc16 << 8);

	//    *pStart++ = crc16 >> 8;
	//    *pStart   = crc16;

	return(crc16);
}

/*
BOOL CLightTransmitPlustech::LoadParameter(CString strPPID)
{
	CHMConfigFile		ConfigFile;

	CString strFileName = _T("");	

	//strFileName.Format("C:\\AOIServer\\LightFile\\TransmitConfig_%s.cfg",strPPID);
	strFileName.Format("C:\\AOIServer\\LightFile\\TransmitConfig.cfg");

	ConfigFile.SetFileName((LPSTR)(LPCTSTR)strFileName);

	int			iDrv,iCh;
	DATA_SubConfig_type	*pCf;
	DATA_SubControl_type	*pLV;
	CString		strCon,strVal;

	for(iDrv=0;iDrv<MAX_TRANSMIT_ID;iDrv++)
	{
		for(iCh=0;iCh<MAX_TRANSMIT_CH;iCh++)
		{
			pCf = &m_StuDrvConfigType.drv[iDrv].sub[iCh];
			pLV = &m_StuLightValue.drv[iDrv].sub[iCh];

			strCon.Format("CONFIG_MODE_%d_%d",iDrv,iCh);			
			ConfigFile.GetItemValue((LPSTR)(LPCTSTR)strCon, strVal);		
			pCf->mode = atoi(strVal);
			strCon.Format("CONFIG_POLAR_%d_%d",iDrv,iCh);			
			ConfigFile.GetItemValue((LPSTR)(LPCTSTR)strCon, strVal);
			pCf->pol = atoi(strVal);			
			strCon.Format("CONFIG_TRIGGER_%d_%d",iDrv,iCh);			
			ConfigFile.GetItemValue((LPSTR)(LPCTSTR)strCon, strVal);
			pCf->trg_sel = atoi(strVal);
			strCon.Format("CONFIG_VOLT_MIN_%d_%d",iDrv,iCh);			
			ConfigFile.GetItemValue((LPSTR)(LPCTSTR)strCon, strVal);
			pCf->volt_min = atoi(strVal);
			strCon.Format("CONFIG_VOLT_MAX_%d_%d",iDrv,iCh);			
			ConfigFile.GetItemValue((LPSTR)(LPCTSTR)strCon, strVal);
			pCf->volt_max = atoi(strVal);

			strCon.Format("LIGHT_VALUE_%d_%d",iDrv,iCh);			
			ConfigFile.GetItemValue((LPSTR)(LPCTSTR)strCon, strVal);
			pLV->volt = atoi(strVal);
			strCon.Format("LIGHT_TIME_%d_%d",iDrv,iCh);			
			ConfigFile.GetItemValue((LPSTR)(LPCTSTR)strCon, strVal);
			pLV->time = atoi(strVal);

			if(pLV->volt > TRANSMIT_MAX_VOLT_VALUE)
				pLV->volt = TRANSMIT_MAX_VOLT_VALUE;
			if(pLV->time > STROBE_DEFAULT_ONTIME)
				pLV->time = STROBE_DEFAULT_ONTIME;
		}
	}

	return TRUE;
}

BOOL CLightTransmitPlustech::SaveParameter(CString strPPID)
{
	CHMConfigFile		ConfigFile;

	CString strFileName = _T("");	

	if(strPPID == "*")
		strPPID = "";
	//strFileName.Format("C:\\AOIServer\\LightFile\\TransmitConfig_%s.cfg",strPPID);

	strFileName.Format("C:\\AOIServer\\LightFile\\TransmitConfig.cfg");

	ConfigFile.SetFileName((LPSTR)(LPCTSTR)strFileName);

	int			iDrv,iCh;
	DATA_SubConfig_type	*pCf;
	DATA_SubControl_type	*pLV;
	CString		strCon,strVal;

	for(iDrv=0;iDrv<MAX_TRANSMIT_ID;iDrv++)
	{
		for(iCh=0;iCh<MAX_TRANSMIT_CH;iCh++)
		{
			pCf = &m_StuDrvConfigType.drv[iDrv].sub[iCh];
			pLV = &m_StuLightValue.drv[iDrv].sub[iCh];

			strCon.Format("CONFIG_MODE_%d_%d",iDrv,iCh);
			strVal.Format("%d",pCf->mode);
			ConfigFile.SetItemValue((LPSTR)(LPCTSTR)strCon, (LPSTR)(LPCTSTR)strVal);			
			strCon.Format("CONFIG_POLAR_%d_%d",iDrv,iCh);
			strVal.Format("%d",pCf->pol);
			ConfigFile.SetItemValue((LPSTR)(LPCTSTR)strCon, (LPSTR)(LPCTSTR)strVal);				
			strCon.Format("CONFIG_TRIGGER_%d_%d",iDrv,iCh);
			strVal.Format("%d",pCf->trg_sel);
			ConfigFile.SetItemValue((LPSTR)(LPCTSTR)strCon, (LPSTR)(LPCTSTR)strVal);	
			strCon.Format("CONFIG_VOLT_MIN_%d_%d",iDrv,iCh);
			strVal.Format("%d",pCf->volt_min);
			ConfigFile.SetItemValue((LPSTR)(LPCTSTR)strCon, (LPSTR)(LPCTSTR)strVal);				
			strCon.Format("CONFIG_VOLT_MAX_%d_%d",iDrv,iCh);
			strVal.Format("%d",pCf->volt_max);
			ConfigFile.SetItemValue((LPSTR)(LPCTSTR)strCon, (LPSTR)(LPCTSTR)strVal);	

			if(pLV->volt > TRANSMIT_MAX_VOLT_VALUE)
				pLV->volt = TRANSMIT_MAX_VOLT_VALUE;
			if(pLV->time > STROBE_DEFAULT_ONTIME)
				pLV->time = STROBE_DEFAULT_ONTIME;

			strCon.Format("LIGHT_VALUE_%d_%d",iDrv,iCh);
			strVal.Format("%d",pLV->volt);
			ConfigFile.SetItemValue((LPSTR)(LPCTSTR)strCon, (LPSTR)(LPCTSTR)strVal);			
			strCon.Format("LIGHT_TIME_%d_%d",iDrv,iCh);
			strVal.Format("%d",pLV->time);
			ConfigFile.SetItemValue((LPSTR)(LPCTSTR)strCon, (LPSTR)(LPCTSTR)strVal);	
		}
	}	

	return ConfigFile.WriteToFile();
}
*/