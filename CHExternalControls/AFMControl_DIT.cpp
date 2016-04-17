#include "StdAfx.h"
#include "AFMControl_DIT.h"

#include "NetPacket.h"

enum SEND_BIT_TYPE { SB_SERVO_ON=0, SB_ALARM, SB_IN_MOTION, SB_IN_POS, SB_HOME, SB_LIMIT_PLUS, SB_LIMIT_MINUS, SB_AF_ENABLE };
enum RECV_BIT_TYPE { RB_ZOOM_INDEX=0, RB_JOG_SPEED, RB_NONE_2, RB_NONE_3, RB_HOME, RB_JOG_COMMAND, RB_NONE_4, RB_AF_ENABLE};

CAFMControl_DIT::CAFMControl_DIT(int nIndex, int nControlType) : CAFMControl(nIndex, nControlType)
{
	m_pClientSocket = new CNetClient();
}

CAFMControl_DIT::~CAFMControl_DIT(void)
{
	if (m_pClientSocket!=NULL)
	{
		delete m_pClientSocket;
	}
	m_pClientSocket = NULL;
}

int	CAFMControl_DIT::InitNetwork(const CString& strIPAddress)
{
	if (m_pClientSocket==NULL) return 0;

	m_pClientSocket->SetIN2P(static_cast<IIOCPNetwork2Parent*>(this));
	m_pClientSocket->StartUp();
	m_pClientSocket->InitNetwork(ClientMode, 4, NETPORT_AFM_SERVER_ALL, strIPAddress);
	m_pClientSocket->SetServerInfo(Socket_AFM_Server, NETCODE_AFM_SERVER_ALL, NETVER_AFM_SERVER_ALL);

	return m_pClientSocket->Connect(TRUE);
}

void CAFMControl_DIT::DeinitNetwork()
{
	if (m_pClientSocket==NULL) return;

	m_pClientSocket->DeinitNetwork();
	m_pClientSocket->CleanUp();
}

void CAFMControl_DIT::IOCPNet2P_Connected(int nType)
{
	if (nType!=Socket_AFM_Server) return;
	
	if (m_pAFC2P)
	{
		m_pAFC2P->AFC2P_DisplayMessage(_T("AFM_Control_DIT NetConnected! => %d Client"), m_nIndex);
	}

	m_pClientSocket->SendToServer(CF_MODULE_INDEX, NET_RESULT_SUCCESS, m_nIndex);

	m_bConnected = TRUE;
}

void CAFMControl_DIT::IOCPNet2P_Disconnected(int nType, int nModuleNo)
{
	if (nType!=Socket_AFM_Server || nModuleNo!=m_nIndex) return;

	if (m_pAFC2P)
	{
		m_pAFC2P->AFC2P_DisplayMessage(_T("AFM_Control_DIT NetDisconnected! => %d Client"), m_nIndex);
	}

	m_bConnected = FALSE;
}

BOOL CAFMControl_DIT::IOCPNet2P_Received(int nType, CNetPacket* pPacket, int nContext)
{
	if (nType!=Socket_AFM_Server) return FALSE;

	if(!pPacket) return FALSE;

	switch(pPacket->GetPacketCode())
	{
	case FC_MODULE_INDEX:	
		NetProcess_FC_MODULE_INDEX(pPacket);
		break;

	case FC_SEND_RECIPE:
		NetProcess_FC_SEND_RECIPE(pPacket);
		break;

	case FC_RECV_RECIPE:
		NetProcess_FC_RECV_RECIPE(pPacket);
		break;

	case FC_RECIPE_COMP:
		NetProcess_FC_RECIPE_COMP(pPacket);
		break;

	case FC_RECIPE_COUNT:
		NetProcess_FC_RECIPE_COUNT(pPacket);
		break;

	case FC_RECIPE_INFO:
		NetProcess_FC_RECIPE_INFO(pPacket);
		break;

	case FC_SEND_BIT:
		NetProcess_FC_SEND_BIT(pPacket);
		break;

	case FC_RECV_BIT:
		NetProcess_FC_RECV_BIT(pPacket);
		break;


	default:
		break;
	}

	return TRUE;
}

void CAFMControl_DIT::NetProcess_FC_MODULE_INDEX(CNetPacket* pPacket)
{
	// AFM 상태신호 요청 메세지 전송함.
// 	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
// 	if (pSendPacket)
// 	{
// 		pSendPacket->SetInt(m_nIndex);
// 		m_pClientSocket->SendToServer(CF_SEND_SIGNAL, NET_RESULT_SUCCESS, m_nIndex, pSendPacket);
// 	}
}

void CAFMControl_DIT::NetProcess_FC_SEND_SIGNAL(CNetPacket* pPacket)
{
	// 특정 모듈의 현재상태 신호 요청 결과
	int nModuleIndex = pPacket->GetInt(0);		// AFM 모듈의 번호		
	int nModuleStatus = pPacket->GetInt(1);		// 현재 상태 신호

	// 현재 모듈의 신호이면...
	if (m_nIndex == nModuleIndex)
	{
		if (m_pAFC2P && m_nStatus!=nModuleStatus)
		{
			m_pAFC2P->AFC2P_CurrentStatus(m_nIndex, nModuleStatus);
		}
		m_nStatus = nModuleStatus;
	}

	//g_pLog->DisplayMessage(_T("NetProcess_FC_SEND_SIGNAL %d %d"), nModuleIndex, nModuleStatus);

// 	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
// 	if (pSendPacket)
// 	{
// 		pSendPacket->SetInt(m_nIndex);
// 		pSendPacket->SetInt(64);
// 		m_pClientSocket->SendToServer(CF_RECV_SIGNAL, NET_RESULT_SUCCESS, 0, pSendPacket);
// 	}
}

void CAFMControl_DIT::NetProcess_FC_RECV_SIGNAL(CNetPacket* pPacket)
{
	// 특정 모듈의 상태변경 신호 전송 결과	
	int nModuleIndex = pPacket->GetInt(0);		// AFM 모듈의 번호		

	//g_pLog->DisplayMessage(_T("NetProcess_FC_RECV_SIGNAL %d"), nModuleIndex);

// 	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
// 	if (pSendPacket)
// 	{
// 		pSendPacket->SetInt(m_nIndex);
// 		m_pClientSocket->SendToServer(CF_SEND_RECIPE, NET_RESULT_SUCCESS, 0, pSendPacket);
// 	}
}

void CAFMControl_DIT::NetProcess_FC_SEND_RECIPE(CNetPacket* pPacket)
{
	// 특정 모듈의 현재 레시피 정보 요청 결과 
	int nModuleIndex		= pPacket->GetInt(0);		// AFM 모듈의 번호		
	int nRecipeIndex		= pPacket->GetInt(1);		// 레시피 번호	
	int nZoomCount			= pPacket->GetInt(2);		// 줌 갯수
	int nZoomIndex			= pPacket->GetInt(3);		// 줌 번호
	CString strRecipeName	= pPacket->GetString(0);	// 레시피 이름

// 	if (m_pAFC2P)
// 	{
// 		m_pAFC2P->AFC2P_DisplayMessage(_T("NetProcess_FC_SEND_RECIPE %d %d %d %d %s"), nModuleIndex, nZoomIndex, strRecipeName);
// 	}

// 	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
// 	if (pSendPacket)
// 	{
// 		pSendPacket->SetInt(m_nIndex);
// 		pSendPacket->SetInt(1);
// 
// 		CString strValue = _T("RECIPE_01");
// 		pSendPacket->SetString(strValue);
// 
// 		m_pClientSocket->SendToServer(CF_RECV_RECIPE, NET_RESULT_SUCCESS, 0, pSendPacket);
// 	}
}

void CAFMControl_DIT::NetProcess_FC_RECV_RECIPE(CNetPacket* pPacket)
{
// 	if (m_pAFC2P)
// 	{
// 		m_pAFC2P->AFC2P_DisplayMessage(_T("NetProcess_FC_RECV_RECIPE"));
// 	}
}

void CAFMControl_DIT::NetProcess_FC_RECIPE_COMP(CNetPacket* pPacket)
{
	// 특정 모듈의 특정 레시피 변경 결과를 받음.
	int nModuleIndex		= pPacket->GetInt(0);		// AFM 모듈의 번호	
	int nResultCode			= pPacket->GetInt(1);		// 레시피 변경 결과 1:성공, 
	int nRecipeIndex		= pPacket->GetInt(2);		// 레시피 번호
	int nZoomIndex			= pPacket->GetInt(3);		// 줌 번호
	CString strRecipeName	= pPacket->GetString(0);	// 레시피 이름

	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
	if (pSendPacket)
	{
		pSendPacket->SetInt(m_nIndex);
		m_pClientSocket->SendToServer(CF_RECIPE_COMP, NET_RESULT_SUCCESS, m_nIndex, pSendPacket);
	}

// 	if (m_pAFC2P)
// 	{
// 		m_pAFC2P->AFC2P_DisplayMessage(_T("NetProcess_FC_RECIPE_COMP %d %d %d %d %s"), nModuleIndex, nResultCode, nRecipeIndex, nZoomIndex, strRecipeName);
// 	}
}

void CAFMControl_DIT::NetProcess_FC_RECIPE_COUNT(CNetPacket* pPacket)
{
	// 특정 모듈의 총 레시피 수를 받음.
	int nModuleIndex	= pPacket->GetInt(0);			// AFM 모듈의 번호
	int nRecipeCount	= pPacket->GetInt(1);			// 총 레시피의 수

	//g_pLog->DisplayMessage(_T("NetProcess_FC_RECIPE_COUNT %d %d"), nModuleIndex, nRecipeCount);

// 	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
// 	if (pSendPacket)
// 	{
// 		pSendPacket->SetInt(0);
// 		m_pClientSocket->SendToServer(CF_RECIPE_INFO, NET_RESULT_SUCCESS, 0, pSendPacket);
// 	}
}

void CAFMControl_DIT::NetProcess_FC_RECIPE_INFO(CNetPacket* pPacket)
{
	// 특정 모듈의 특정 레시피 정보를 받음.
	int nModuleIndex		= pPacket->GetInt(0);		// AFM 모듈의 번호
	int nRecipeIndex		= pPacket->GetInt(1);		// 요청한 Recipe 번호
	int nZoomCount			= pPacket->GetInt(2);		// 요청한 Recipe의 줌 갯수
	CString strRecipeName	= pPacket->GetString(0);	// 요청한 Recipe의 이름

	//g_pLog->DisplayMessage(_T("NetProcess_FC_RECIPE_INFO %d %d %d %s"), nModuleIndex, nRecipeIndex, nZoomCount, strRecipeName);

}

void CAFMControl_DIT::NetProcess_FC_SEND_BIT(CNetPacket* pPacket)
{
	int nIndex = pPacket->GetInt(0);
	int nSignalIndex = pPacket->GetInt(1);
	int nSignalData = pPacket->GetInt(2);

	switch(nSignalIndex)
	{
	case SB_SERVO_ON:
		break;

	case SB_ALARM:
		break;

	case SB_IN_MOTION:
		break;

	case SB_IN_POS:
		break;

	case SB_HOME:
		break;

	case SB_LIMIT_PLUS:
		break;

	case SB_LIMIT_MINUS:
		break;

	case SB_AF_ENABLE:
		m_nTracking = nSignalData;
		break;		
	}



}

void CAFMControl_DIT::NetProcess_FC_RECV_BIT(CNetPacket* pPacket)
{
	int nIndex = pPacket->GetInt(0);
	int nResultCode = pPacket->GetInt(1);
	int nSignalIndex = pPacket->GetInt(2);
	int nSignalData = pPacket->GetInt(3);

	switch(nSignalIndex)
	{
	case RB_ZOOM_INDEX:
		if (nSignalData!=-1) m_nZoomIndex = nSignalData;
		break;

	case RB_JOG_SPEED:
		break;

	case RB_HOME:
		break;

	case RB_JOG_COMMAND:
		break;

	case RB_AF_ENABLE:
		m_nTracking = nSignalData;
		break;		
	}

// 	if (m_pAFC2P)
// 	{
// 		m_pAFC2P->AFC2P_DisplayMessage(_T("NetProcess_FC_RECV_BIT %d %d %d %d"), nIndex, nResultCode, nSignalIndex, nSignalData);
// 	}
}

int	CAFMControl_DIT::Connect(const SAFMConnParam& connParam)
{
	m_bConnected = FALSE;

	if (InitNetwork(connParam.strPortNum)!=1)
	{
		return 0;
	}

	return 1;
}

void CAFMControl_DIT::Disconnect()
{
	DeinitNetwork();

	m_bConnected = FALSE;
}

int	CAFMControl_DIT::RecipeJogSpeed(double dSpeed)
{
	if (GetConnected()==FALSE) return 0;
	
	if (m_pClientSocket==NULL) return 0;

	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
	if (pSendPacket)
	{
		int nSpeed = int(dSpeed * 10000.0);
		pSendPacket->SetInt(m_nIndex);			// ModuleIndex
		pSendPacket->SetInt(RB_JOG_SPEED);		// Signal Type
		pSendPacket->SetInt(nSpeed);			// Signal Data
		return m_pClientSocket->SendToServer(CF_RECV_BIT, NET_RESULT_SUCCESS, m_nIndex, pSendPacket);
	}

	return 0;
}

int	CAFMControl_DIT::RecipeJogCommand(int nCmd)
{
	if (GetConnected()==FALSE) return 0;

	if (m_pClientSocket==NULL) return 0;

	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
	if (pSendPacket)
	{
		pSendPacket->SetInt(m_nIndex);			// ModuleIndex
		pSendPacket->SetInt(RB_JOG_COMMAND);	// Signal Type
		pSendPacket->SetInt(nCmd);				// Signal Data
		return m_pClientSocket->SendToServer(CF_RECV_BIT, NET_RESULT_SUCCESS, m_nIndex, pSendPacket);
	}

	return 0;
}
	
int	CAFMControl_DIT::RecipeTracking(int nTracking)
{
	if (GetConnected()==FALSE) return 0;

	if (m_pClientSocket==NULL) return 0;

	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
	if (pSendPacket)
	{
		pSendPacket->SetInt(m_nIndex);			// ModuleIndex
		pSendPacket->SetInt(RB_AF_ENABLE);		// Signal Type
		pSendPacket->SetInt(nTracking);			// Signal Data
		return m_pClientSocket->SendToServer(CF_RECV_BIT, NET_RESULT_SUCCESS, m_nIndex, pSendPacket);
	}

	return 0;
}


int	CAFMControl_DIT::RecipeZoom(int nZoomIndex)
{
	if (GetConnected()==FALSE) return 0;

	if (m_pClientSocket==NULL) return 0;

	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
	if (pSendPacket)
	{
		pSendPacket->SetInt(m_nIndex);			// ModuleIndex
		pSendPacket->SetInt(RB_ZOOM_INDEX);		// Signal Type
		pSendPacket->SetInt(nZoomIndex);		// Signal Data
		return m_pClientSocket->SendToServer(CF_RECV_BIT, NET_RESULT_SUCCESS, m_nIndex, pSendPacket);
	}

	return 0;
}

int	CAFMControl_DIT::RecipeChange(int nRecipeIndex, int nZoomIndex)
{
	if (GetConnected()==FALSE) return 0;

	if (m_pClientSocket==NULL) return 0;

	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
	if (pSendPacket)
	{
		pSendPacket->SetInt(m_nIndex);				// ModuleIndex
		pSendPacket->SetInt(nRecipeIndex);			// recipe index
		pSendPacket->SetInt(nZoomIndex);			// zoom index
		pSendPacket->SetString(m_strRecipeName);	// recipe name
		return m_pClientSocket->SendToServer(CF_RECV_RECIPE, NET_RESULT_SUCCESS, m_nIndex, pSendPacket);
	}

	return 0;
}

int	CAFMControl_DIT::RecipeChange(const CString& strRecipeName, int nZoomIndex)
{
	if (GetConnected()==FALSE) return 0;

	if (m_pClientSocket==NULL) return 0;

	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
	if (pSendPacket)
	{
		pSendPacket->SetInt(m_nIndex);				// ModuleIndex
		pSendPacket->SetInt(m_nRecipeIndex);		// recipe index
		pSendPacket->SetInt(nZoomIndex);			// zoom index
		pSendPacket->SetString(strRecipeName);		// recipe name
		return m_pClientSocket->SendToServer(CF_RECV_RECIPE, NET_RESULT_SUCCESS, m_nIndex, pSendPacket);
	}

	return 0;
}

BOOL CAFMControl_DIT::SetTracking(int nTracking)
{
	if (RecipeTracking(nTracking))
	{
		m_nTracking = nTracking;
		return TRUE;
	}

	return FALSE;
}

BOOL CAFMControl_DIT::SetZoomIndex(int nZoomIndex)
{
	if (RecipeZoom(nZoomIndex))
	{
		m_nZoomIndex = nZoomIndex;
		return TRUE;
	}

	return FALSE;
}

BOOL CAFMControl_DIT::SetRecipeIndex(int nRecipeIndex, int nZoomIndex)
{
	if (RecipeChange(nRecipeIndex, nZoomIndex))
	{
		m_nRecipeIndex = nRecipeIndex;
		m_nZoomIndex = nZoomIndex;
		return TRUE;
	}

	return FALSE;
}

BOOL CAFMControl_DIT::SetRecipeName(const CString& strRecipeName, int nZoomIndex)
{
	if (RecipeChange(strRecipeName, nZoomIndex))
	{
		m_strRecipeName = strRecipeName;
		m_nZoomIndex = nZoomIndex;
		return TRUE;
	}

	return FALSE;
}

BOOL CAFMControl_DIT::GetTracking(int& nTracking)
{
	if (GetConnected()==FALSE) return FALSE;

	nTracking = m_nTracking;

	return TRUE;
}

BOOL CAFMControl_DIT::GetZoomIndex(int& nZoomIndex)
{
	if (GetConnected()==FALSE) return FALSE;

	nZoomIndex = m_nZoomIndex;

	return TRUE;
}

BOOL CAFMControl_DIT::GetRecipeIndex(int& nRecipeIdnex, int& nZoomIndex)
{
	if (GetConnected()==FALSE) return FALSE;

	nRecipeIdnex = m_nRecipeIndex;

	nZoomIndex = m_nZoomIndex;

	return TRUE;
}

BOOL CAFMControl_DIT::GetRecipeName(CString& strRecipeName, int& nZoomIndex)
{
	if (GetConnected()==FALSE) return FALSE;

	strRecipeName = strRecipeName;

	nZoomIndex = m_nZoomIndex;

	return TRUE;
}

BOOL CAFMControl_DIT::MoveToLimitPlus()
{
	if (GetConnected()==FALSE) return 0;

	if (m_pClientSocket==NULL) return 0;

	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
	if (pSendPacket)
	{
		pSendPacket->SetInt(m_nIndex);				// ModuleIndex
		pSendPacket->SetInt(RB_HOME);			// Home Command		
		return m_pClientSocket->SendToServer(CF_RECV_BIT, NET_RESULT_SUCCESS, m_nIndex, pSendPacket);
	} 
	return TRUE;
}
BOOL CAFMControl_DIT::MoveToLimitMinus()
{
	return TRUE;
}
BOOL CAFMControl_DIT::MoveToBasePosition(int nZoomIndex)
{
	if (GetConnected()==FALSE) return 0;

	if (m_pClientSocket==NULL) return 0;

	CNetPacket *pSendPacket = m_pClientSocket->GetPacket();
	if (pSendPacket)
	{
		pSendPacket->SetInt(m_nIndex);				// ModuleIndex
		pSendPacket->SetInt(nZoomIndex);			// Home Command		
		return m_pClientSocket->SendToServer(CF_RECV_WSI_MODE, NET_RESULT_SUCCESS, m_nIndex, pSendPacket);
	} 

	return TRUE;
}