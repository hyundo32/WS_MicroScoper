#pragma once

#include <vector>

struct SAFMConnParam
{
	SAFMConnParam() 
	{
		nPortNum = -1;
		strPortNum = _T("");
	}
	int			nPortNum;
	CString		strPortNum;
};

struct SAFMRecipeInfo
{
	SAFMRecipeInfo() 
	{
		strRecipeName = _T("");
		nZoomCount = 0;
	}
	CString		strRecipeName;
	int			nZoomCount;
};

typedef std::vector<SAFMRecipeInfo>				VectorAFMRecipeInfo;
typedef std::vector<SAFMRecipeInfo>::iterator	VectorAFMRecipeInfoIt;

interface IAFMControl2Parent
{
public:
	virtual void AFC2P_CurrentStatus(int nIndex, int nStatusCode) = 0;
	virtual void AFC2P_RecipeChangeComplete(int nIndex, int nResultCode) = 0;
	virtual void AFC2P_DisplayMessage(const TCHAR* lpstrFormat, ...) = 0;
};

class AFX_EXT_CLASS CAFMControl
{
public:
	CAFMControl(int nIndex, int nControlType=-1);
	virtual ~CAFMControl(void);
	void Reset();
	void SetAFC2P(IAFMControl2Parent* pAFC2P)		{ m_pAFC2P = pAFC2P; }
	
	virtual int		Connect(const SAFMConnParam& connParam) = 0;
	virtual void	Disconnect() = 0;
	virtual BOOL	GetConnected();

	virtual int		RecipeJogSpeed(double dSpeed) = 0;
	virtual int		RecipeJogCommand(int nCmd) = 0;
	virtual int		RecipeTracking(int nTracking) = 0;
	virtual int		RecipeZoom(int nZoomIndex) = 0;
	virtual int		RecipeChange(int nRecipeIndex, int nZoomIndex) = 0;
	virtual int		RecipeChange(const CString& strRecipeName, int nZoomIndex) = 0;
	
	virtual BOOL	SetTracking(int nTracking) = 0;
	virtual BOOL	SetZoomIndex(int nZoomIndex) = 0;
	virtual BOOL	SetRecipeIndex(int nRecipeIdnex, int nZoomIndex=-1) = 0;
	virtual BOOL	SetRecipeName(const CString& strRecipeName, int nZoomIndex=-1) = 0;
	
	virtual BOOL	GetTracking(int& nTracking) = 0;
	virtual BOOL	GetZoomIndex(int& nZoomIndex) = 0;
	virtual BOOL	GetRecipeIndex(int& nRecipeIdnex, int& nZoomIndex) = 0;
	virtual BOOL	GetRecipeName(CString& strRecipeName, int& nZoomIndex) = 0;

	virtual BOOL	MoveToLimitPlus() = 0;
	virtual BOOL	MoveToLimitMinus() = 0;
	virtual BOOL	MoveToBasePosition(int nZoomIndex) = 0;
	
	int	GetControlType()		{ return m_nControlType; }
	int	GetTotalRecipeCount()	{ return (int)m_vecAFMRecipeInfo.size(); }
	int	GetStatus()				{ return m_nStatus; }
	int	GetRecipeStatus()		{ return m_nRecipeStatus; }

	void SetIndex(int nIndex)	{ m_nIndex = nIndex; }
	int GetIndex()				{ return m_nIndex; }
	
	SAFMRecipeInfo*			GetAFMRecipeInfo(int nIndex);
	const SAFMRecipeInfo*	GetAFMRecipeInfo(int nIndex) const;

protected:
	IAFMControl2Parent*		m_pAFC2P;
	int						m_nIndex;
	int						m_nControlType;
	BOOL					m_bConnected;

	int						m_nStatus;
	int						m_nRecipeStatus;
	
	CString					m_strRecipeName;
	int						m_nRecipeIndex;
	int						m_nZoomIndex;
	int						m_nTracking;

	VectorAFMRecipeInfo		m_vecAFMRecipeInfo;

};
typedef std::vector<CAFMControl*>				VectorAFMControl;
typedef std::vector<CAFMControl*>::iterator		VectorAFMControlIt;