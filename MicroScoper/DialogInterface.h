#pragma once

interface IDlgCameraReview2Parent
{
	virtual void IDRC2P_DisplayMessage(const TCHAR* lpstrFormat, ...) = 0;
	virtual void IDRC2P_ChangeCameraView() = 0;
};

interface IDlgCameraAlign2Parent
{
	virtual void IDAC2P_DisplayMessage(const TCHAR* lpstrFormat, ...) = 0;
	virtual void IDAC2P_ChangeCameraView() = 0;
};

interface IDlgModuleControl2Parent
{
	virtual void IDMC2P_DisplayMessage(const TCHAR* lpstrFormat, ...) = 0;
};

interface IDlgModuleList2Parent
{
	virtual void IDML2P_DisplayMessage(const TCHAR* lpstrFormat, ...) = 0;
	virtual void IDMI2P_SetSelectedModuleIndex(int nModuleIdx) = 0;
};

interface IDlgTabControl2Parent
{
	virtual void IDTC2P_DisplayMessage(const TCHAR* lpstrFormat, ...) = 0;
};

interface IDlgHistoryList2Parent
{
	virtual void IDHL2P_DisplayMessage(const TCHAR* lpstrFormat, ...) = 0;
};

interface IDlgLogMessage2Parent
{
	virtual void IDLM2P_DisplayMessage(const TCHAR* lpstrFormat, ...) = 0;
};


/*
#define CTLCOLOR_MSGBOX         0
#define CTLCOLOR_EDIT           1
#define CTLCOLOR_LISTBOX        2
#define CTLCOLOR_BTN            3
#define CTLCOLOR_DLG            4
#define CTLCOLOR_SCROLLBAR      5
#define CTLCOLOR_STATIC         6
#define CTLCOLOR_MAX            7
*/
#define CTLCOLOR_COUNT			8

class CDialogInterface
{
public:
	CDialogInterface(COLORREF nControlColor=RGB(255,255,255)) 
	{
		for (int i=0; i<CTLCOLOR_COUNT; i++)
		{
			switch(i)
			{
			case CTLCOLOR_DLG:
				m_hControlColor[i].CreateSolidBrush(nControlColor);
				break;
			default:
				m_hControlColor[i].CreateStockObject(NULL_BRUSH);
				break;
			}
		}
	}

	virtual ~CDialogInterface(void)
	{
	}

protected:
	CBrush	m_hControlColor[CTLCOLOR_COUNT];

};