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

	// 접속 초기화.
	BOOL			DisconnectServer();

protected:
	// Server Socket이 접속을 허용했을 때 호출하는 함수.
	virtual int		AcceptedSocket(pPerSocketContext pSocketCtx, ULONG ulIP, int nPort);

	// Client Socket이 접속되었을 때 호출하는 함수
	virtual BOOL	ConnectedSocket(pPerSocketContext pSocketCtx);

	// Socket에서 Receive가 완료되었을 때 CIOCPController에서 호출하는 함수 - Data Parseing
	virtual BOOL	RecvCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes);

	// Socket을 통해 Send가 완료되었을 때 CIOCPController에서 호출하는 함수. - 후처리
	virtual BOOL	SendCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes);

	// Socket이 끊어졌음을 알려주기 위한 함수
	virtual void	SocketClosed(pPerSocketContext pSocketCtx);

private:
	IIOCPNetwork2Parent*	m_pIN2P;

	// Packet 구조체
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

