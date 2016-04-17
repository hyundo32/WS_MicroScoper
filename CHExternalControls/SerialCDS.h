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
	int m_head;//data�� ���� ��ġ
	int m_tail;//data��   �� ��ġ,  head<tail
public:
	CdsCirBuf();
	~CdsCirBuf();
	int Alloc(int _size);
	bool Put(unsigned char* data, int size);//false: buffer full( if tail+1==head )
	int Get(unsigned char* data, int size);//false: buffer empty( if head==tail )
	bool Check(unsigned char* data, int size);//false: buffer empty( if head==tail )
	int RoomSize();//���� ���ۼ��� ����
	int DataSize();//������ ���ۼ��� ����
};

typedef struct _CdsPacket
{
	unsigned char Prefix; // Ŀ�ǵ� 0xA5  ȸ�� 0x5A
	unsigned short Length;// 2byte, �ִ� 0~32768
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
	int m_SendTimer;//��� ���� ������ timer index
	int m_ReceiveTimer;//��� ���� ������ timer index
	CdsSerialState m_SendState;// �۽� ��� ����
	CdsSerialState m_ReceiveState;// ���� ��� ����
	int m_ReceiveDataCounter;// data�ΰ� �󸶳� ���� �Ǿ��� �� ��Ÿ��
	int m_SendDataCounter;// �ش� ��Ŷ�� data�ΰ� �󸶳� �۽� �Ǿ����� ��Ÿ��
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
