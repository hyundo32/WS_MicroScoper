#pragma once

/////////////////////////////////////////////////////////////////////////////
enum ETabOrientation
{	
	e_tabTop=0,
	e_tabBottom,
	e_tabLeft,
	e_tabRight,
	e_tabNOF,
};
/////////////////////////////////////////////////////////////////////////////
// CXPTabCtrl

class CXPTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CXPTabCtrl)

public:
	CXPTabCtrl();
	virtual ~CXPTabCtrl();

	void SetExtended(BOOL bTabExtended)			{ m_bTabExtended=bTabExtended; }
	void InitImageList(UINT nBitmapID);

protected:
	afx_msg void OnPaint();
	afx_msg void OnTabSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTabSelChanged (NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	void DrawThemesXpTabItem(CDC* pDC, int ixItem, const CRect& rcItem, UINT uiFlag);
	void DrawTabItem(CDC* pDC, int ixItem, const CRect& rcItemC, UINT uiFlags);
	BOOL IsExtendedTabTheamedXP();

protected:
	ETabOrientation	m_eTabOrientation;	// current tab orientation
	CImageList		m_ilTabs; 			// possible tab image list
	BOOL			m_bTabExtended;		// Use extended Themes functionalities
	int				m_ixSelOld;			// used to redraw deselected tab background

};

/////////////////////////////////////////////////////////////////////////////
// Helper functions
HRESULT DrawThemesPart(HDC hDC, int iPartId, int iStateId, LPCSTR uiPartNameID, LPRECT prcRcBx);
BOOL    IsThemeActiveXP();
int	    DWordAlign(int n);
template <class T> void SwapVars(T& a,T& b) { T t=a; a=b; b=t; }
/////////////////////////////////////////////////////////////////////////////