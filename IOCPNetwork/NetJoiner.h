#pragma once

#include "NetPacket.h"
#include "IOCPController.h"

interface IIOCPNetwork2Parent
{
	virtual void IOCPNet2P_Connected(int Type) = 0;
	virtual void IOCPNet2P_Disconnected(int Type, int nModuleNo = -1) = 0;
	virtual BOOL IOCPNet2P_Received(int Type, CNetPacket* pPacket, int nContext = -1) = 0;
};

class CNetJoiner
{
public:
	CNetJoiner()
	{
		m_pCtx				= NULL;
		Reset();
	}

	virtual ~CNetJoiner()
	{
		Reset();
	}

	void					Reset()
	{
		m_nModuleNo			= -1;
		
		if (m_pCtx)
		{
			m_pCtx->Reset();
		}
		
		m_pCtx				= NULL;
		m_ulIP				= -1;
		m_bUseHeartbeat		= FALSE;
		m_dwRecvHeartbeat	= 0;
	}

	// getter
	WSABUF*					GetSendBuffer()				{ return m_pCtx ? m_pCtx->sendContext->GetBuffer() : NULL; }
	pPerSocketContext		GetSocketContext()			{ return m_pCtx; }
	ULONG					GetClientIP()				{ return m_ulIP; }
	int						GetModuleNo()				{ return m_nModuleNo; }
	DWORD					GetRecvHeartbeat()			{ return m_bUseHeartbeat ? m_dwRecvHeartbeat : 0; }

	// setter
	void					SetSocketContext(pPerSocketContext pCtx)	{ m_pCtx = pCtx; }
	void					SetClientIP(ULONG ulIP)						{ m_ulIP = ulIP; }
	void					SetModuleNo(int nModuleNo, BOOL bUseHeartbeat)
	{
		m_nModuleNo = nModuleNo;
		m_bUseHeartbeat = bUseHeartbeat;
	}
	void					SetUseHeartbeat(BOOL bUse)	{ m_bUseHeartbeat = bUse; }
	void					SetRecvHeartbeat()			{ m_dwRecvHeartbeat = GetTickCount(); }
	

protected:
	BOOL					m_bUseHeartbeat;
	DWORD					m_dwRecvHeartbeat;

	int						m_nModuleNo;
	pPerSocketContext		m_pCtx;
	ULONG					m_ulIP;
};

