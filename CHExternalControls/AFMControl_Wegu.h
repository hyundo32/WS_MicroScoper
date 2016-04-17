#pragma once

#include "afmcontrol.h"
#include "WorkThreadPools.h"

enum {AFM_WEGU_JOG_COMAND = 0, AFM_WEGU_WRITE_ZOOM, AFM_WEGU_AF_TRACKING, AFM_WEGU_AF_STOP, AFM_WEGU_IS_SERIAL_CONNECTION, AFM_WEGU_HOME, AFM_WEGU_END_PROCESS };

#pragma comment(lib, "user32.lib")

struct sSharedData
{
	int nCmd;
	int nArgument;
	char szCharName[100];

	sSharedData()
	{
		nCmd = -1;
		nArgument = -1;
		memset(szCharName, 0, sizeof(szCharName));
	}
};

class CAFMControlProcessData : public CWorkThreadData
{
public:
	CAFMControlProcessData(PVOID pPtr) : CWorkThreadData(pPtr)
	{
		nThreadIdx		= -1;		
	}
	int				nThreadIdx;
};


class AFX_EXT_CLASS CAFMControl_Wegu :	public CAFMControl,
										public CWorkThreadPools
{
public:
	CAFMControl_Wegu(int nIndex, int nControlType);
	virtual ~CAFMControl_Wegu(void);

	virtual int		Connect(const SAFMConnParam& connParam);
	virtual void	Disconnect();
	virtual BOOL	GetConnected();

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

	virtual void	WorkThreadProcess(PVOID pParameter);	

protected:
	void Wait(DWORD dwMillisecond);

	sSharedData CommunicationAFM(sSharedData* SendData);	

	char	m_strPort[16];
	int		m_nBaudrate_start;
	double  m_dSpeed;
	HANDLE	m_hMutex;

	int		m_count;

	HANDLE	m_hMapFile;
	LPCTSTR m_pBuf;

	u_short m_nMicroSteps;
	u_short m_nFullsteps;

	int m_pAFMHomingZParam[8];
};
