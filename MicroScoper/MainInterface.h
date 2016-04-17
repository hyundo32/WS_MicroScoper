#pragma once

#include "MicroScoperDoc.h"
#include "MicroScoperView.h"
#include "ObserverInterface.h"
#include "DialogInterface.h"
#include "DisplayMessage.h"


class CMainInterface :	public CObserverInterface,
						public IDisplayMessage2Parent,
						public IDlgCameraAlign2Parent,
						public IDlgModuleControl2Parent,
						public IDlgModuleList2Parent,
						public IDlgCameraReview2Parent,
						public IDlgTabControl2Parent,
						public IDlgHistoryList2Parent,
						public IDlgLogMessage2Parent
{
public:
	CMainInterface(void);
	virtual ~CMainInterface(void);
	void SetDoc(CMicroScoperDoc* pDoc)				{ m_pDoc = pDoc; }
	void SetView(CMicroScoperView* pView)
	{ 
		m_pView = pView;
		InitInterface();
	} 

public: // Dialog Interface
	// IDlgCameraReview2Parent
	virtual void IDRC2P_DisplayMessage(const TCHAR* lpstrFormat, ...);
	virtual void IDRC2P_ChangeCameraView();

	// IDlgCameraAlign2Parent
	virtual void IDAC2P_DisplayMessage(const TCHAR* lpstrFormat, ...);
	virtual void IDAC2P_ChangeCameraView();

	// IDlgModuleControl2Parent
	virtual void IDMC2P_DisplayMessage(const TCHAR* lpstrFormat, ...);

	// IDlgModuleList2Parent
	virtual void IDML2P_DisplayMessage(const TCHAR* lpstrFormat, ...);
	virtual void IDMI2P_SetSelectedModuleIndex(int nModuleIdx);
	
	// IDlgTabControl2Parent
	virtual void IDTC2P_DisplayMessage(const TCHAR* lpstrFormat, ...);

	// IDlgHistoryList2Parent
	virtual void IDHL2P_DisplayMessage(const TCHAR* lpstrFormat, ...);

	// IDlgLogMessage2Parent
	virtual void IDLM2P_DisplayMessage(const TCHAR* lpstrFormat, ...);

	// display message 2 parent
	virtual void DM2P_DisplayMessage(const CString& strMessage);
	virtual void DM2P_DisplayMessage(const TCHAR* lpstrFormat, ...);

protected: // hardware module
	// interface
	void	InitInterface();
	void	DeinitInterface();

	// motor control
	int		InitMotorControl();
	void	DeinitMotorControl();

	// review camera
	int		InitReviewCamera();
	void	DeinitReviewCamera();

	// align camera
	int		InitAlignCamera();
	void	DeinitAlignCamera();

	// review light
	int		InitReviewLight();
	void	DeInitReviewLight();

	// align light
	int		InitAlignLight();
	void	DeinitAlignLight();

	// review revolver
	int		InitReviewRevolver();
	void	DeinitReviewRevolver();

	// review afm control
	int		InitReviewAfmControl();
	void	DeinitReviewAfmControl();

	// wsi control
	int		InitWsiControl();
	void	DeinitWsiControl();

protected: // software module
	// review processor
	int		InitReviewProcessor();
	void	DeinitReviewProcessor();

	// sequence processor
	int		InitSequenceProcessor();
	void	DeinitSequenceProcessor();
		
protected:
	CMicroScoperDoc*	m_pDoc;
	CMicroScoperView*	m_pView;

};

