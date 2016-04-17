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

	// Client ��ü�� ����.
	BOOL			InvokeToClient(int nType, SHORT sResult);
	BOOL			InvokeToClient(int nType, SHORT sResult, CNetPacket* pPacket);
	BOOL			InvokeToClient(int nType, SHORT sResult, SHORT sPacketIdx, CNetPacket* pPacket);

	// �� Client�� ����.
	BOOL			SendToClient(int nModuleNo, int nType, SHORT sResult);
	BOOL			SendToClient(int nModuleNo, int nType, SHORT sResult, CNetPacket* pPacket);
	BOOL			SendToClient(int nModuleNo, int nType, SHORT sResult, SHORT sPacketIdx, CNetPacket* pPacket);

	// ���� �ʱ�ȭ.
	BOOL			DisconnectClient(int nModuleNo);

protected:
	// Server Socket�� ������ ������� �� ȣ���ϴ� �Լ�.
	virtual int AcceptedSocket(pPerSocketContext pSocketCtx, ULONG ulIP, int nPort);

	// Client Socket�� ���ӵǾ��� �� ȣ���ϴ� �Լ�
	virtual BOOL ConnectedSocket(pPerSocketContext pSocketCtx);

	// Socket���� Receive�� �Ϸ�Ǿ��� �� CIOCPController���� ȣ���ϴ� �Լ� - Data Parseing
	virtual BOOL RecvCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes);

	// Socket�� ���� Send�� �Ϸ�Ǿ��� �� CIOCPController���� ȣ���ϴ� �Լ�. - ��ó��
	virtual BOOL SendCompleted(pPerSocketContext pSocketCtx, DWORD dwBytes);

	// Socket�� ���������� �˷��ֱ� ���� �Լ�
	virtual void SocketClosed(pPerSocketContext pSocketCtx);

private:
	IIOCPNetwork2Parent*	m_pIN2P;

	// Packet ����ü
	CNetPacket*				m_pPacket;
	int						m_nTotalPacketCount;
	CRITICAL_SECTION		m_csPacket;
	int						m_nPacketIdx;

	// Client List
	CNetJoiner*				m_pClientJoiner;
	int						m_nClientCount;			// ������ Client �� (Client�� Max ��)
	int						m_Clientint;

	// Network Information
	short					m_sNetworkCode;
	short					m_sVersion;
	int						m_nListenPort;
};	

