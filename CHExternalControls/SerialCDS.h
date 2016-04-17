//	SerialCds.h - Definition of the CSerialMFC class
//
//	Copyright (C) 1999-2003 Ramon de Klein (Ramon.de.Klein@ict.nl)
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


#ifndef __SERIAL_CDS_H
#define __SERIAL_CDS_H


//////////////////////////////////////////////////////////////////////
// Include CSerialWnd base class

#include "SerialMFC.h"


//////////////////////////////////////////////////////////////////////
// Make sure that MFC is already loaded
//
// We don't include it here, because MFC based applications typically
// use a precompiled header file.

#ifndef __AFXWIN_H__
#error "CWnd class isn't included (include AFXWIN.H)"
#endif


class AFX_EXT_CLASS CdsCirBuf
{
	int m_BufSize;
	unsigned char *m_BufData;
	int m_head;//data를 읽을 위치
	int m_tail;//data를   쓸 위치,  head<tail
public:
	CdsCirBuf();
	~CdsCirBuf();
	int Alloc(int _size);
	bool Put(unsigned char* data, int size);//false: buffer full( if tail+1==head )
	int Get(unsigned char* data, int size);//false: buffer empty( if head==tail )
	bool Check(unsigned char* data, int size);//false: buffer empty( if head==tail )
	int RoomSize();//남은 버퍼수를 리턴
	int DataSize();//차지한 버퍼수를 리턴
};

typedef struct _CdsPacket
{
	unsigned char Prefix; // 커맨드 0xA5  회신 0x5A
	unsigned short Length;// 2byte, 최대 0~32768
	unsigned char Cmd;
	unsigned char *pData;
} CdsPacket;


enum CdsSerialState
{
	INIT,
	IDLE,
	BUSY,
	WAIT,
	FAULT
};

class AFX_EXT_CLASS CdsSerial : public CSerialMFC
{
public:
	CdsCirBuf m_ReceiveBuffer;//receive buffer
	CdsCirBuf m_SendBuffer;//send buffer
	int m_SendTimer;//통신 상태 검지용 timer index
	int m_ReceiveTimer;//통신 상태 검지용 timer index
	CdsSerialState m_SendState;// 송신 통신 상태
	CdsSerialState m_ReceiveState;// 수신 통신 상태
	int m_ReceiveDataCounter;// data부가 얼마나 수신 되었는 지 나타냄
	int m_SendDataCounter;// 해당 패킷의 data부가 얼마나 송신 되었는지 나타냄
	CdsPacket m_Packet;
	bool CheckReceivePacket();
	int TxData();
	int RxData();

public:
	CdsSerial();
	~CdsSerial();
	CdsSerialState DoSendStateTransition();
	CdsSerialState InitSendState();
	CdsSerialState DoReceiveStateTransition();
	CdsSerialState InitReceiveState();
	bool SendPacket(CdsPacket *pSendPacket);
	bool GetPacket(CdsPacket *pGetPacket);
	bool SendPacket2(unsigned char cmd, unsigned char *pData, int size);
	bool GetPacket2(unsigned char &cmd, unsigned char *pData, int &prog);
};


#endif	// __SERIAL_MFC_H
