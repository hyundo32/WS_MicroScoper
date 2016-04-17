

/**
  ******************************************************************************
  * @file    /Project/Source/STRB_Protocol.hpp
  * @author  Electronic Develope Team
  * @version V1.0.0
  * @date    2013-11-27
  * @brief   This file contains the Project Protocol Control.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */


#ifndef __STRB_PROTOCOL_H
#define __STRB_PROTOCOL_H

//#include "STRB_ProjectInit.h"
#include "LightTransmitPlustech_DataType.h"

//#include "NString.hpp"

//#include "STM32_FlashControl.hpp"

#define MAX_DRV_SUB_CHANNEL             8
#define MAX_DRV_CHANNEL                 8


/*
class TStrobeProtocol : public NString
{
		int 					GetLength(void);
		STRB_BasicPacket_type	*m_Pkt;
	public:
		TStrobeProtocol(unsigned int size) : NString(size) { m_Pkt = (STRB_BasicPacket_type *) data; }
// ----------------------------------------------------------
//		Make Packet
// ----------------------------------------------------------
	void SetPacket(STRB_BasicPacket_type *imp) { memcpy(data, imp, imp->len); datalen = imp->len; }

	void AddHeader(char org, char dest, unsigned char cmd);
	void AddRawData(void *add_dat, int add_len);
	void AddData(void *dat, int len); // Finishing Data
	void AddTail(void); // Finishing Packet
// ----------------------------------------------------------
//		Complate Packet
// ----------------------------------------------------------
		bool 			IsComplate(void);
		unsigned char	GetOrigin(void) { return m_Pkt->o_id; }
		unsigned char	GetDestination(void) { return m_Pkt->d_id; }
//		unsigned char	GetOrigin(void) { return data[PI_OID]; }
//		unsigned char	GetDestination(void) { return data[PI_DID]; }
};
*/


enum PROTOCOL_STROBE_COMMAND
{
	PSC_MONITOR,   // S : Status Monitor
	PSC_ALARMRESET,// a : Driver No.
	PSC_GETCONFIG, // C, c
	PSC_SETCONFIG, // D, d
	PSC_GETCONTROL, // H, h
	PSC_SETCONTROL, // I, i

	// Standard Command
	PSC_TIMESET,  // T
	PSC_VERSION,  // V
	PSC_FWSIGN,   // W
	PSC_FWCODE,   // w
	PSC_HWRESET,  // X
	PSC_NONE
};

enum STRB_MAIN_PROTOCOL_GROUP
{
	SMPG_MAINPROC, // Main's Drv Response
	SMPG_MAIN2GUI, // GUI Requset.
	SMPG_PASS2GUI,
	SMPG_PASS2DRV,
	SMPG_NONE
};

enum STRB_DRIVER_PROTOCOL_GROUP
{
	SDPG_RET2DBG, // Dbg Return.
	SDPG_RET2LINK, // Link Return.
	SDPG_BROADCAST, // Broadcast.
	SDPG_NONE
};

//#pragma push

//#pragma pack(1)
#pragma pack(push, 1)

//////////////////////////////////////////////////////////////////////////

//						Strobe Control Structure						//

//////////////////////////////////////////////////////////////////////////
union _trg_cfg { // Trig Control
	struct {
	unsigned short onoff:1;  // 0: Off 1: On
	unsigned short mode:1;   // 0:Strobe 1:Continuous
	unsigned short pol:1;    // 0:Rising 1: Falling
	unsigned short trgsel:3; // 0 <= n <= 7
	unsigned short dummy:10;
	};
	unsigned short trg;
};

// Control Address : (struct TPTS_CS_type *) 0x60000100
struct TPTS_CS_type { // Driver-Base Board Control
	unsigned short		pwm[8]; 	// 12bit duration x8 (Contol)
	union _trg_cfg		tc[8];		// Trigger Config x8 (Config)
	unsigned short		err_clr;	// Error & Clear (Status)
	unsigned short		test;		// Test Mode (Config)
//	unsigned short		err; // .0~.7(OVP), .8~.15(OCP) ==> 0:OK, 1:ERR
};





//////////////////////////////////////////////////////////////////////////

//						Strobe Protocol Data Types						//

//////////////////////////////////////////////////////////////////////////

struct DATA_SubConfig_type {
	unsigned char		onoff;
	unsigned char		mode;		// Strobe / Continuous
	unsigned char		pol;
	unsigned char		trg_sel;

	unsigned short		volt_min;
	unsigned short		volt_max;
};

// Memory Store
struct DATA_DrvConfig_type {
	DATA_SubConfig_type		sub[MAX_DRV_SUB_CHANNEL];

//	unsigned char					test;
};

struct DATA_Config_type {
	DATA_DrvConfig_type		drv[MAX_DRV_CHANNEL];
};



struct DATA_SubControl_type {
	unsigned short			volt; // Set Voltage(0 ~ 10000)
	unsigned short			time; // Strobe duration time
};

struct DATA_DrvControl_type {
	DATA_SubControl_type	sub[MAX_DRV_SUB_CHANNEL];
};

struct DATA_Control_type {
	DATA_DrvControl_type	drv[MAX_DRV_CHANNEL];
};



// Status Structure

struct DATA_SubStatus_type {
	unsigned short			volt;

	unsigned char			ovp;
	unsigned char			ocp;
};

struct DATA_DrvStatus_type {
	DATA_SubStatus_type		sub[MAX_DRV_SUB_CHANNEL];
	unsigned char			mode; // Remote:0, Test:1
	unsigned char			alarm; // Sub Total alarm
};

struct DATA_Status_type {
	struct DATA_DateTime_type		datetime;

	DATA_DrvStatus_type				drv[MAX_DRV_CHANNEL];

	// System Status
	unsigned char					presents;
	unsigned char					alarm; // Total alarm
//	unsigned char					sys_mode;
	unsigned char					DIN_stat;
	unsigned char					DOUT_stat;
	unsigned short					power_volt;
	unsigned short					bat_volt;
};
//#pragma pop
#pragma pack(pop)

#endif	// __STRB_PROTOCOL_HPP


