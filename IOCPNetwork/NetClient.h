#pragma once

#include "NetJoiner.h"

class AFX_EXT_CLASS CNetClient : public CIOCPController
{
public:
	CNetClient(void);
	virtual ~CNetClient(void);

	void			SetIN2P(IIOCPNetwork2Parent* pIN2P)		{ m_pIN2P = pIN2P; }
	BOOL			SetServerInfo(int ST, short sNetworkCode, short sVersion, int nPacketCount = 100);

	CNetPacket*		GetPacket(int nMilSecond= 100);
	CNetJoiner*		GetJoiner()			{ return &m_SocketJoiner; }

	BOOL			SendToServer(int nType, SHORT sResult, int nModuleNo);
	BOOL			SendToServer(int nType, SHORT sResult, int nModuleNo, CNetPacket* pPacket);
	BOOL			SendToServer(int nType, SHORT sResult, int nModuleNo, SHORT sPacketIdx, CNetPacket* pPacket);

	// ���� �ʱ�ȭ.
	BOOL			DisconnectServer();

protected:
	// Server Socket�� ������ ������� �� ȣ���ϴ� �Լ�.
	virtual int		AcceptedSocket(pPerSocketContext pSocketCtx, ULONG ulIP, int nPort);

	// Client Socket�� ���ӵǾ��� �� ȣ���ϴ� �Լ�
	virtual BOOL	ConnectedSocket(pPerSocketContext pSocketCtx);

	// Socket���� Receive�� �Ϸ�Ǿ��� �� CIOCPController���� ȣ���ϴ� �Լ� - Data Parseing
	virtual BOOL	RecvCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes);

	// Socket�� ���� Send�� �Ϸ�Ǿ��� �� CIOCPController���� ȣ���ϴ� �Լ�. - ��ó��
	virtual BOOL	SendCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes);

	// Socket�� ���������� �˷��ֱ� ���� �Լ�
	virtual void	SocketClosed(pPerSocketContext pSocketCtx);

private:
	IIOCPNetwork2Parent*	m_pIN2P;

	// Packet ����ü
	int						m_nPacketCount;
	CNetPacket*				m_pPacket;
	CRITICAL_SECTION		m_csPacket;
	CRITICAL_SECTION		m_csSending;
	int						m_nPacketIdx;

	CNetJoiner				m_SocketJoiner;
	int						m_nServerSocketType;

	// Network Information
	short					m_sNetworkCode;
	short					m_sVersion;
};

