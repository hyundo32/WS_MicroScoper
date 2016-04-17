#pragma once


#define	 UM_PLC_MESSAGE (WM_USER + 3000)
#define  UM_PLC_PIOMSG	(WM_USER + 3300)
#define  ALIVESIGNAL_DEFINE_CNT		12

enum PLC_COMMAND_MESSAGE{ PLCCOMMAND_RECONNECT = 0, 
						  PLCCOMMAND_BYPASS_MODE, PLCCOMMAND_AUTO_MODE, PLCCOMMAND_MANUAL_MODE, PLCCOMMAND_SIMULATE_MODE,
						  PLCCOMMAND_GLASS_LOADING, PLCCOMMAND_GLASS_LOADING_END, PLCCOMMAND_ALIGN_START, PLCCOMMAND_ALIGN_END,
						  PLCCOMMAND_REVIEW_START, PLCCOMMAND_REVIEW_END, PLCCOMMAND_GLASS_UNLOADING, PLCCOMMAND_OPERATION_REVIEW_MODE };

enum PLC_SEND_MESSAGE{	PLCSEND_LOAD_SUCCESS = 0,	PLCSEND_LOAD_FAIL, 
						PLCSEND_ALIGN_SUCCESS,		PLCSEND_ALIGN_FAIL, 
						PLCSEND_REVIEW_SUCCESS,		PLCSEND_REVIEW_FAIL,
						PLCSEND_LIGHT_FAIL,			PLCSEND_AF_FAIL,
						PLCSEND_RESERVED_1,			PLCSEND_RESERVED_2,
						PLCSEND_LIGHT_ON, 			PLCSEND_LIGHT_OFF};

enum MAINPLC_ALARM_MESSAGE{ ALARM1 = 0, ALARM2, ALARM3, ALARM4, ALARM5, ALARM6, ALARM7,
							ALARM8, ALARM9, ALARM10, ALARM11, ALARM12, ALARM13, ALARM14 };

enum { PLC_SEND_REVIEW_END_SIGNAL = 1};

#define	ALIVE_BIT(nValue, add, bit) 	nValue= nValue | ((add)<<(bit));
#define IS_ALIVE_BIT(nValue, bit) 		((nValue & (0x01<<(bit))) >>(bit));

typedef struct _ST_PLC_RECIVE_A3_01
{
	BOOL					bBypassMode;
	BOOL					bAutoMode;
	BOOL					bManualMode;
	BOOL					bReviewMode;

	BOOL					bGlassLoading;
	BOOL					bAlignStart;
	BOOL					bAlignEnd;
	BOOL					bReviewStart;		
	BOOL					bReviewEnd;		
	BOOL					bGlassUnloading;

	BOOL					bReadSignal_05;
	BOOL					bReadSignal_04;
	BOOL					bReadSignal_03;
	BOOL					bReadSignal_02;
	BOOL					bReadSignal_01;
	BOOL					bReadSignal_00;


	_ST_PLC_RECIVE_A3_01()
	{
		Reset();
	}

	_ST_PLC_RECIVE_A3_01(int nPLCData)
	{	
		/*
		(nPLCData & 0x01)	?bBypassMode=TRUE:bBypassMode=FALSE;
		(nPLCData & 0x02)	?bAutoMode=TRUE:bAutoMode=FALSE;
		(nPLCData & 0x04)	?bManualMode=TRUE:bManualMode=FALSE;
		(nPLCData & 0x08)	?bGlassLoading=TRUE:bGlassLoading=FALSE;
		(nPLCData & 0x10)	?bGlassLoadingEnd=TRUE:bGlassLoadingEnd=FALSE;
		(nPLCData & 0x20)	?bReviewStart=TRUE:bReviewStart=FALSE;
		(nPLCData & 0x40)	?bReviewEnd=TRUE:bReviewEnd=FALSE;
		(nPLCData & 0x80)	?bGlassUnloading=TRUE:bGlassUnloading=FALSE;
		(nPLCData & 0x100)	?bOperationReviewMode=TRUE:bOperationReviewMode=FALSE;
		*/

		bBypassMode		= nPLCData & 0x01;
		bAutoMode		= nPLCData & 0x02;
		bManualMode		= nPLCData & 0x04;
		bReviewMode		= nPLCData & 0x08;

		bGlassLoading	= nPLCData & 0x10;
		bAlignStart		= nPLCData & 0x20;
		bAlignEnd		= nPLCData & 0x40;
		bReviewStart	= nPLCData & 0x80;
		bReviewEnd		= nPLCData & 0x100;
		bGlassUnloading = nPLCData & 0x200;
	}

	void Reset()
	{
		memset(this, 0, sizeof(_ST_PLC_RECIVE_A3_01));
	}		

}ST_PLC_RECIVE_A3_01,*pST_PLC_RECIVE_A3_01;

typedef struct _ST_PLC_SEND_A2E_01
{
	BOOL	bAlarm14;
	BOOL	bAlarm13;
	BOOL	bAlarm12;
	BOOL	bAlarm11;
	BOOL	bAlarm10;
	BOOL	bAlarm9;
	BOOL	bAlarm8;
	BOOL	bAlarm7;
	BOOL	bAlarm6;
	BOOL	bAlarm5;
	BOOL	bAlarm4;
	BOOL	bAlarm3;
	BOOL	bAlarm2;
	BOOL	bAlarm1;
	BOOL	bReviewEnd;
	BOOL	bProgramAlive;

	_ST_PLC_SEND_A2E_01()
	{
		bProgramAlive = FALSE;
		Reset(FALSE);
	}	
	void Reset(BOOL bSet)
	{		
		bAlarm1 = bAlarm2 = bAlarm3 = bAlarm4 = bAlarm5 = bAlarm6 = bAlarm7 = bAlarm8 =bSet;				
		bAlarm9 = bAlarm10 = bAlarm11 = bAlarm12 = bAlarm13 = bAlarm14 = bReviewEnd = bSet;
	}		
	void ChangeSignal(_ST_PLC_SEND_A2E_01 &pSent)
	{
		if(pSent.bAlarm14) bAlarm14 = FALSE;
		if(pSent.bAlarm13) bAlarm13 = FALSE;
		if(pSent.bAlarm12) bAlarm12 = FALSE;
		if(pSent.bAlarm11) bAlarm11 = FALSE;
		if(pSent.bAlarm10) bAlarm10 = FALSE;
		if(pSent.bAlarm9) bAlarm9 = FALSE;
		if(pSent.bAlarm8) bAlarm8 = FALSE;
		if(pSent.bAlarm7) bAlarm7 = FALSE;
		if(pSent.bAlarm6) bAlarm6 = FALSE;
		if(pSent.bAlarm5) bAlarm5 = FALSE;
		if(pSent.bAlarm4) bAlarm4 = FALSE;
		if(pSent.bAlarm3) bAlarm3 = FALSE;
		if(pSent.bAlarm2) bAlarm2 = FALSE;
		if(pSent.bAlarm1) bAlarm1 = FALSE;
		if(pSent.bReviewEnd) bReviewEnd = FALSE;
	}
	void MakeSend(int &nSendData)
	{		
		nSendData = 0;
		ALIVE_BIT(nSendData, bAlarm14, 15);
		ALIVE_BIT(nSendData, bAlarm13, 14);
		ALIVE_BIT(nSendData, bAlarm12, 13);
		ALIVE_BIT(nSendData, bAlarm11, 12);
		ALIVE_BIT(nSendData, bAlarm10, 11);
		ALIVE_BIT(nSendData, bAlarm9, 10);
		ALIVE_BIT(nSendData, bAlarm8, 9);
		ALIVE_BIT(nSendData, bAlarm7, 8);
		ALIVE_BIT(nSendData, bAlarm6, 7);
		ALIVE_BIT(nSendData, bAlarm5, 6);
		ALIVE_BIT(nSendData, bAlarm4, 5);
		ALIVE_BIT(nSendData, bAlarm3, 4);
		ALIVE_BIT(nSendData, bAlarm2, 3);
		ALIVE_BIT(nSendData, bAlarm1, 2);
		ALIVE_BIT(nSendData, bReviewEnd, 1);
		ALIVE_BIT(nSendData, bProgramAlive, 0);	
	}
}ST_PLC_SEND_A2E_01,*pST_PLC_SEND_A2E_01;
