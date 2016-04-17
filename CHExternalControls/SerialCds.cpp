//	SerialCDS.cpp - Implementation of the CSerialCDS class


//////////////////////////////////////////////////////////////////////
// Include the standard header files
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Include module headerfile

#include "SerialCDS.h"


//////////////////////////////////////////////////////////////////////
// Disable warning C4127: conditional expression is constant, which
// is generated when using the _RPTF and _ASSERTE macros.

#pragma warning(disable: 4127)


//////////////////////////////////////////////////////////////////////
// Enable debug memory manager

#ifdef _DEBUG

#ifdef THIS_FILE
#undef THIS_FILE
#endif

static const char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW

#endif


//////////////////////////////////////////////////////////////////////
// Code


CdsCirBuf::CdsCirBuf()
{
	m_BufSize=0;
	m_head=0;
	m_tail=0;
}

int CdsCirBuf::Alloc(int size)
{
	if(m_BufData=(unsigned char*)calloc(size,1)) m_BufSize=size;
	else m_BufSize=0;
	return m_BufSize;
}

CdsCirBuf::~CdsCirBuf()
{
	if(m_BufData) delete(m_BufData);
}

bool CdsCirBuf::Put(unsigned char* data, int size)//false: buffer full( if tail+1==head )
{
	int i;

	for (i=0;i<size;i++)
	{
		m_BufData[m_tail]=data[i];
		if((m_tail+1)%m_BufSize==m_head) return false;
		m_tail=(m_tail+1)%m_BufSize;
	}
	return true;
}

int CdsCirBuf::Get(unsigned char* data, int size)//false: buffer empty( if head==tail )
{
	int i;

	if(m_BufSize==0) return false;
	for (i=0;i<size;i++)
	{
		if(m_tail==m_head) return i;// number of bytes
		data[i]=m_BufData[m_head];
		m_head=(m_head+1)%m_BufSize;
	}
	return i;
}

bool CdsCirBuf::Check(unsigned char* data, int size)//false: buffer empty( if head==tail )
{
	int i, j;

	if(m_BufSize==0) return false;
	for (i=0, j=m_head;i<size;i++)
	{
		if(m_tail==j) return false;//buffer empty
		data[i]=m_BufData[j];
		j=(j+1)%m_BufSize;
	}
	return true;
}


int CdsCirBuf::RoomSize()//남은 버퍼수를 리턴
{
	int size;
	size=m_BufSize-(m_tail-m_head);
	if(size<0) size+=m_BufSize;
	return size;
}

int CdsCirBuf::DataSize()//사용중인 버퍼수를 리턴
{
	int size;
	size=m_tail-m_head;
	if(size<0) size+=m_BufSize;
	return size;
}
//////////////////////////////////////////////////////////////////////
// Code of CdsSerial
//
// programmed by CoolbART on Aug. 29th, 2008
//
//////////////////////////////////////////////////////////////////////

CdsSerial::CdsSerial()
{
	m_ReceiveState=INIT;
	m_SendState=INIT;
	m_ReceiveBuffer.Alloc(3072);
	m_SendBuffer.Alloc(2048);
	m_SendTimer=0;//통신 상태 검지용 timer index
	m_ReceiveTimer=0;//통신 상태 검지용 timer index
	m_ReceiveDataCounter=0;// data부가 얼마나 수신 되었는 지 나타냄
	m_SendDataCounter=0;
	m_Packet.Length=0;
	m_Packet.pData=NULL;
	return;
}

CdsSerial::~CdsSerial()
{
	if (m_Packet.Length >0 && m_Packet.pData != NULL) delete m_Packet.pData;
	return;
}

CdsSerialState CdsSerial::InitReceiveState()
{
	m_ReceiveState=IDLE;
	return m_ReceiveState;
}

CdsSerialState CdsSerial::DoReceiveStateTransition()
{
	switch(m_ReceiveState)
	{
	case INIT:// 통신이 연결되어 있으면 버퍼를 초기화 하고 IDLE로 진행, 아니면 FAULT로
		break;
	case IDLE:// ReceiveBuffer가 비어있지 않으면 BUSY로
		break;
	case BUSY:// ReceiveBuffer에 하나 이상의 완전한 패킷이 있으면 IDLE로, 아니면 타이머를 초기화 하고 WAIT로
		break;
	case WAIT:// 시간초과면 FAULT로, 새로 수신된 데이터가 있으면 BUSY로
		break;
	case FAULT:// InitState()함수에 의해 INIT로
		break;
	}
	return m_ReceiveState;
}

CdsSerialState CdsSerial::InitSendState()
{
	m_SendState=IDLE;
	return m_SendState;
}

CdsSerialState CdsSerial::DoSendStateTransition()
{
	switch(m_SendState)
	{
	case INIT:// 통신이 연결되어 있으면 버퍼를 초기화 하고 IDLE로 진행, 아니면 FAULT로
		break;
	case IDLE:// SendBuffer가 비어있지 않으면 BUSY로
		break;
	case BUSY:// 하나의 패킷을 전송하는 동안 BUSY로, 버퍼가 비면 대기, 패킷 전송 후에 타이머 초기화 하고 WAIT로 진행
		      // 데이터 수신시마다 수신율 반영.
		break;
	case WAIT:// 보낸 패킷에 대한 ack가 올 때까지 타이머 작동하면서 대기. 시간제한에 걸리면 FAULT로, 아니면 IDLE로
		break;
	case FAULT:// InitState()함수에 의해 INIT로
		break;
	}
	return m_SendState;
}

bool CdsSerial::SendPacket(CdsPacket *pSendPacket)
{
	if(m_SendBuffer.RoomSize() < pSendPacket->Length + 4) return false;
	m_SendBuffer.Put((unsigned char *)pSendPacket,4);			// head
	m_SendBuffer.Put(pSendPacket->pData,pSendPacket->Length);	// data
	return true;
}

bool CdsSerial::GetPacket(CdsPacket *pGetPacket)
{
	CdsPacket Packet;
	if(! m_ReceiveBuffer.Check((unsigned char*)&Packet,4) ) return false; // 헤더를 체크하여 head가 없으면 false
	if(m_ReceiveBuffer.DataSize() < Packet.Length + 4) return false;
	m_ReceiveBuffer.Get((unsigned char*)&pGetPacket, Packet.Length+4);
	return true;
}

int CdsSerial::TxData()// SendBuffer에서 정해진 크기의 데이터를 전송한다.
{
	unsigned char pData[64];
	int i;
	int ByteSend=m_SendBuffer.Get(pData,64); //한번에 최대 16개의 데이터를 전송하도록 한다.

	for( i=0;i < ByteSend;i++)
	{
		WriteByte(&(pData[i]),1);// 1byte씩 일단 전송하자.
		Sleep(10);
	}
	return i; //전송된 byte수를 리턴
}

int CdsSerial::RxData()// 수신된 데이터를 ReceiveBuffer에 채워넣는다.
{
	DWORD dwRead;
	unsigned char szData[128];
	Read(szData,128,&dwRead);
	if(m_ReceiveBuffer.RoomSize() < (int)dwRead) return -1;
	m_ReceiveBuffer.Put(szData, dwRead);
	return dwRead;
}

bool CdsSerial::SendPacket2(unsigned char cmd, unsigned char *pData, int size)
{
	if(m_SendBuffer.RoomSize() < size+1) return false;
	m_SendBuffer.Put(&cmd,1);			// head
	m_SendBuffer.Put(pData,size);	// data
	return true;
}

bool CdsSerial::GetPacket2(unsigned char &cmd, unsigned char *pData, int &prog)// 패킷이 온전히 들어왔으면 true
{
//	bool result;

	int size=0;
	
	if(!m_ReceiveBuffer.Check(&cmd,1)) 
	{
		m_ReceiveState=IDLE;
		return false;
	}
	m_ReceiveState=BUSY;
	switch(cmd)//들어오는 커맨드에 대한 해석
	{
	case 0xff://ver
		size=8;// AVR 4byte IS 4byte
		break;
	case 0xac://progress
		size=2;
		break;
	case 'p'://일반 정보
		size=20;
		break;
	case 0xa5://filter upload
		size=2048;
		break;
	case 0xa1://cap data upload
		size=2048;
		break;
	case 0xae://mem data upload
		size=6;
		break;
	}
	m_ReceiveDataCounter=m_ReceiveBuffer.DataSize();
	if(m_ReceiveDataCounter <= size) 
	{
		prog= m_ReceiveDataCounter;
		return false;// packet이 다 들어올 때까지 대기
	}
//	prog=0;
	m_ReceiveState=IDLE;
	m_ReceiveBuffer.Get(&cmd, 1);
	m_ReceiveBuffer.Get(pData, size);
	return true;
}