/**
  ******************************************************************************
  * @file    /Project/Source/STRB_DataType.h
  * @author  Electronic Develope Team
  * @version V1.0.0
  * @date    2013-11-27
  * @brief   This file contains the Project Protocol Control.
  ******************************************************************************
  * @attention
  * STRB Protocol Data Type
  *D:\Work\Project\5.Enhanced-Strobe\3.Firmware\STROBE-WORK-IOTEST\Source\STRB_DataType.h
  ******************************************************************************
  */ 

#ifndef __STRB_DATATYPE_H
#define __STRB_DATATYPE_H

// #include "stm32f2xx_rtc.h"


enum PDG_PACKETDESTINATIONGROUP {
	PDG_FAIL,
	PDG_PLUSTEK, // Power User Interface
	PDG_GUI,
	PDG_MAIN,
	PDG_MAIN_BROADCAST,
	PDG_DRIVER,
	PDG_DRIVER_BROADCAST
};



enum PROTOCOL_TYPE {
	PT_NONE,
	PT_STROBE,



	PT_TEST = 200
};

struct ProtocolTable {
	unsigned char		cmd;
	unsigned short		size;
	unsigned short		tmo;
};

//#pragma push
//#pragma pack(1)
#pragma pack(push, 1)

/*
enum _PACKET_INDEX {
	PI_STX,
	PI_OID,
	PI_DID,
	PI_CMD,
	PI_LEN,
	PI_DAT,
	PI_CRC,
	PI_ETX
};
struct OLED_BasePacket_type {
	unsigned char		stx; 		// 0x02
	unsigned char		o_id;		// 0x00 : GUI(Origin) or Main
	unsigned char		d_id;		// 0x10 ~ 0xFF(Destination)
	unsigned char		cmd;		// 'A' ~ 'Z'...
	unsigned char		len;		// Total Length(STX ~ ETX)
	unsigned char		dat[1];		// 0~n
	unsigned short		crc16;		// Big-endian
	unsigned char		etx;
};
*/

#define PACKET_LENGTH(datalen)  ((datalen)+12)
#define PACKET_ID(mid, did)	(unsigned char) ((mid & 0x0F)<<4 | (did&0x0F))

#define STDP_PREAMBLE		0x03AA5502

#define MAX_TRANSMIT_ID		8
#define MAX_TRANSMIT_CH		8

struct STRB_Packet_type {
	unsigned long		pre;		// PTSE
	unsigned short		len;
	unsigned char		typ;		//
	unsigned char		cmd;
	unsigned char		dat[1];
//	unsigned short		crc;
};

struct STRB_BasicPacket_type {
	unsigned long		pre;		// STDP_PREAMBLE
	unsigned short		len;
	unsigned char		typ;		// STROBE : 0x01,etc
	unsigned char		o_id;
	unsigned char		d_id;
	unsigned char		cmd;
	unsigned char		dat[1];
//	unsigned short		crc;
};

#define TRANSMIT_CONTROLLER_ID 0x10
#define STROBE_DEFAULT_ONTIME  30000
#define TRANSMIT_MAX_VOLT_VALUE 10000
struct STU_TRANSMIT_LIGHT_VALUE 
{
	unsigned short		volt;
	unsigned short		time;
};

struct STU_TRANSMIT_CONFIG 
{
	unsigned char		onoff;
	unsigned char		mode;
	unsigned char		polar;
	unsigned char		trg_sel;
	unsigned short		volt_min;
	unsigned short		volt_max;
};


//enum _PACKET_INDEX {
//	PI_STX,
//	PI_LEN = 4,
//	PI_OID = 7,
//	PI_DID,
//	PI_CMD,
//	PI_DAT,
//	PI_CRC
//};

// #define STDP_0x01_LENGTH(dat_len)		((dat_len) + 12)
//enum _PACKET_INDEX { PI_PRE1, PI_PRE2, PI_PRE3, PI_PRE4, PI_LEN1, PI_LEN2, PI_TYPE, PI_OID, PI_DID, PI_CMD, PI_DAT };


struct DATA_Version_type {
	unsigned short			hw;
	unsigned short			fw;
	unsigned long			index;
	char					str[32];
};

struct DATA_DateTime_type {
	// RTC_InitTypeDef init;
/*
	RTC_TimeTypeDef time; // H(0~23), M(0~59), S, dummy(0)
	RTC_DateTypeDef date; // W(1~7), M(1~12), D(1~31), Year(00~99)
*/
    unsigned char       time[4];
    unsigned char       date[4];
};

struct DATA_FW_Signature_type {
	unsigned short		code_len;
	unsigned short		chksum;
};

#define DATA_CODEPAGE_SIZE	128

struct DATA_FW_CodePage_type {
	unsigned short		page;
	unsigned char		data[DATA_CODEPAGE_SIZE];
};


//#pragma pop
#pragma pack(pop)


#endif // __STRB_DATATYPE_H


