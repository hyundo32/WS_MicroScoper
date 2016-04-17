#pragma once

#include "NetJoiner.h"

class AFX_EXT_CLASS CNetServer : public CIOCPController
{
public:
	CNetServer(void);
	virtual ~CNetServer(void);

	void			SetIN2P(IIOCPNetwork2Parent* pIN2P)	{ m_pIN2P = pIN2P; }
	BOOL			InitNetwork(NetworkMode Mode, int nSocketCount, int nPort = 0, CString strIP = _T(""));

	int				GetClientCount()			{ return m_nClientCount; }
	BOOL			SetClientInfo(int ST, int nCount, short sNetworkCode, short sVersion, BOOL bUseHeartbeat, int nPacketCount = 100);
	short			GetNetworkCode()			{ return m_sNetworkCode; }
	short			GetVersion()				{ return m_sVersion; }

	CNetPacket*		GetPacket();
	CNetJoiner*		GetJoinerArray(int nIdx)	{ return m_pClientJoiner ? &m_pClientJoiner[nIdx] : NULL; }
	CNetJoiner*		GetJoinerUser(int nModuleNo);

	// Client 전체에 전송.
	BOOL			InvokeToClient(int nType, SHORT sResult);
	BOOL			InvokeToClient(int nType, SHORT sResult, CNetPacket* pPacket);
	BOOL			InvokeToClient(int nType, SHORT sResult, SHORT sPacketIdx, CNetPacket* pPacket);

	// 한 Client에 전송.
	BOOL			SendToClient(int nModuleNo, int nType, SHORT sResult);
	BOOL			SendToClient(int nModuleNo, int nType, SHORT sResult, CNetPacket* pPacket);
	BOOL			SendToClient(int nModuleNo, int nType, SHORT sResult, SHORT sPacketIdx, CNetPacket* pPacket);

	// 접속 초기화.
	BOOL			DisconnectClient(int nModuleNo);

protected:
	// Server Socket이 접속을 허용했을 때 호출하는 함수.
	virtual int AcceptedSocket(pPerSocketContext pSocketCtx, ULONG ulIP, int nPort);

	// Client Socket이 접속되었을 때 호출하는 함수
	virtual BOOL ConnectedSocket(pPerSocketContext pSocketCtx);

	// Socket에서 Receive가 완료되었을 때 CIOCPController에서 호출하는 함수 - Data Parseing
	virtual BOOL RecvCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes);

	// Socket을 통해 Send가 완료되었을 때 CIOCPController에서 호출하는 함수. - 후처리
	virtual BOOL SendCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes);

	// Socket이 끊어졌음을 알려주기 위한 함수
	virtual void SocketClosed(pPerSocketContext pSocketCtx);

private:
	IIOCPNetwork2Parent*	m_pIN2P;

	// Packet 구조체
	CNetPacket*				m_pPacket;
	int						m_nTotalPacketCount;
	CRITICAL_SECTION		m_csPacket;
	int						m_nPacketIdx;

	// Client List
	CNetJoiner*				m_pClientJoiner;
	int						m_nClientCount;			// 설정된 Client 수 (Client의 Max 수)
	int						m_Clientint;

	// Network Information
	short					m_sNetworkCode;
	short					m_sVersion;
	int						m_nListenPort;
};	

