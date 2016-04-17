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


int CdsCirBuf::RoomSize()//���� ���ۼ��� ����
{
	int size;
	size=m_BufSize-(m_tail-m_head);
	if(size<0) size+=m_BufSize;
	return size;
}

int CdsCirBuf::DataSize()//������� ���ۼ��� ����
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
	m_SendTimer=0;//��� ���� ������ timer index
	m_ReceiveTimer=0;//��� ���� ������ timer index
	m_ReceiveDataCounter=0;// data�ΰ� �󸶳� ���� �Ǿ��� �� ��Ÿ��
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
	case INIT:// ����� ����Ǿ� ������ ���۸� �ʱ�ȭ �ϰ� IDLE�� ����, �ƴϸ� FAULT��
		break;
	case IDLE:// ReceiveBuffer�� ������� ������ BUSY��
		break;
	case BUSY:// ReceiveBuffer�� �ϳ� �̻��� ������ ��Ŷ�� ������ IDLE��, �ƴϸ� Ÿ�̸Ӹ� �ʱ�ȭ �ϰ� WAIT��
		break;
	case WAIT:// �ð��ʰ��� FAULT��, ���� ���ŵ� �����Ͱ� ������ BUSY��
		break;
	case FAULT:// InitState()�Լ��� ���� INIT��
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
	case INIT:// ����� ����Ǿ� ������ ���۸� �ʱ�ȭ �ϰ� IDLE�� ����, �ƴϸ� FAULT��
		break;
	case IDLE:// SendBuffer�� ������� ������ BUSY��
		break;
	case BUSY:// �ϳ��� ��Ŷ�� �����ϴ� ���� BUSY��, ���۰� ��� ���, ��Ŷ ���� �Ŀ� Ÿ�̸� �ʱ�ȭ �ϰ� WAIT�� ����
		      // ������ ���Žø��� ������ �ݿ�.
		break;
	case WAIT:// ���� ��Ŷ�� ���� ack�� �� ������ Ÿ�̸� �۵��ϸ鼭 ���. �ð����ѿ� �ɸ��� FAULT��, �ƴϸ� IDLE��
		break;
	case FAULT:// InitState()�Լ��� ���� INIT��
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
	if(! m_ReceiveBuffer.Check((unsigned char*)&Packet,4) ) return false; // ����� üũ�Ͽ� head�� ������ false
	if(m_ReceiveBuffer.DataSize() < Packet.Length + 4) return false;
	m_ReceiveBuffer.Get((unsigned char*)&pGetPacket, Packet.Length+4);
	return true;
}

int CdsSerial::TxData()// SendBuffer���� ������ ũ���� �����͸� �����Ѵ�.
{
	unsigned char pData[64];
	int i;
	int ByteSend=m_SendBuffer.Get(pData,64); //�ѹ��� �ִ� 16���� �����͸� �����ϵ��� �Ѵ�.

	for( i=0;i < ByteSend;i++)
	{
		WriteByte(&(pData[i]),1);// 1byte�� �ϴ� ��������.
		Sleep(10);
	}
	return i; //���۵� byte���� ����
}

int CdsSerial::RxData()// ���ŵ� �����͸� ReceiveBuffer�� ä���ִ´�.
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

bool CdsSerial::GetPacket2(unsigned char &cmd, unsigned char *pData, int &prog)// ��Ŷ�� ������ �������� true
{
//	bool result;

	int size=0;
	
	if(!m_ReceiveBuffer.Check(&cmd,1)) 
	{
		m_ReceiveState=IDLE;
		return false;
	}
	m_ReceiveState=BUSY;
	switch(cmd)//������ Ŀ�ǵ忡 ���� �ؼ�
	{
	case 0xff://ver
		size=8;// AVR 4byte IS 4byte
		break;
	case 0xac://progress
		size=2;
		break;
	case 'p'://�Ϲ� ����
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
		return false;// packet�� �� ���� ������ ���
	}
//	prog=0;
	m_ReceiveState=IDLE;
	m_ReceiveBuffer.Get(&cmd, 1);
	m_ReceiveBuffer.Get(pData, size);
	return true;
}