#pragma once

#include "afmcontrol.h"

#include "NetDefinition.h"
#include "NetClient.h"

class AFX_EXT_CLASS CAFMControl_DIT : public CAFMControl, public IIOCPNetwork2Parent
{
public:
	CAFMControl_DIT(int nIndex, int nControlType);
	virtual ~CAFMControl_DIT(void);
	
	// net iocp 2 parent
	virtual void	IOCPNet2P_Connected(int Type);
	virtual void	IOCPNet2P_Disconnected(int Type, int nModuleNo = -1);
	virtual BOOL	IOCPNet2P_Received(int Type, CNetPacket* pPacket, int nContext = -1);

	virtual int		Connect(const SAFMConnParam& connParam);
	virtual void	Disconnect();

	virtual int		RecipeJogSpeed(double dSpeed);
	virtual int		RecipeJogCommand(int nCmd);
	virtual int		RecipeTracking(int nTracking);
	virtual int		RecipeZoom(int nZoomIndex);
	virtual int		RecipeChange(int nRecipeIndex, int nZoomIndex);
	virtual int		RecipeChange(const CString& strRecipeName, int nZoomIndex);

	virtual BOOL	SetTracking(int nTracking);
	virtual BOOL	SetZoomIndex(int nZoomIndex);
	virtual BOOL	SetRecipeIndex(int nRecipeIdnex, int nZoomIndex=-1);
	virtual BOOL	SetRecipeName(const CString& strRecipeName, int nZoomIndex=-1);

	virtual BOOL	GetTracking(int& nTracking);
	virtual BOOL	GetZoomIndex(int& nZoomIndex);
	virtual BOOL	GetRecipeIndex(int& nRecipeIdnex, int& nZoomIndex);
	virtual BOOL	GetRecipeName(CString& strRecipeName, int& nZoomIndex);

	virtual BOOL	MoveToLimitPlus();
	virtual BOOL	MoveToLimitMinus();
	virtual BOOL	MoveToBasePosition(int nZoomIndex);

protected:
	int		InitNetwork(const CString& strIPAddress);
	void	DeinitNetwork();

	void	NetProcess_FC_MODULE_INDEX(CNetPacket* pPacket);
	void	NetProcess_FC_SEND_SIGNAL(CNetPacket* pPacket);
	void	NetProcess_FC_RECV_SIGNAL(CNetPacket* pPacket);
	void	NetProcess_FC_SEND_RECIPE(CNetPacket* pPacket);
	void	NetProcess_FC_RECV_RECIPE(CNetPacket* pPacket);
	void	NetProcess_FC_RECIPE_COMP(CNetPacket* pPacket);
	void	NetProcess_FC_RECIPE_COUNT(CNetPacket* pPacket);
	void	NetProcess_FC_RECIPE_INFO(CNetPacket* pPacket);
	void	NetProcess_FC_SEND_BIT(CNetPacket* pPacket);
	void	NetProcess_FC_RECV_BIT(CNetPacket* pPacket);

	CNetClient*		m_pClientSocket;				// AFM Client Socket
};
