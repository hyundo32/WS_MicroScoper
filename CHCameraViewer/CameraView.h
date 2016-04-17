#pragma once


// CCameraView
#include "CHImageData.h"

#define IDR_LOAD_IMAGE		9000
#define IDR_SAVE_IMAGE		9001
#define IDR_SAVE_AS_IMAGE	9002

#define IDR_VIEW_NAME		9100
#define IDR_CENTER_LINE		9101
#define IDR_CENTER_RECT		9102
#define IDR_RULER			9103
#define IDR_ROI_RECT		9104
#define IDR_CLEAR_ROI_RECT	9105
#define IDR_SAVE_ROI_RECT	9106
#define IDR_AOI_DEFECT		9107
#define IDR_DEFECT_POS		9108

#define IDR_VIEW_ORIGIN		9200
#define IDR_VIEW_FIT		9201

#define IDR_VIEW_COLOR		9300
#define IDR_VIEW_GRAY		9301
#define IDR_VIEW_RED		9302
#define IDR_VIEW_BLUE		9303
#define IDR_VIEW_GREEN		9304

#define IDR_CHANGE_VIEW		9400

struct SPixelInfo
{
	SPixelInfo() 
	{
		nWidthPixel = 0;
		nHeightPixel = 0;
		dDiagonalPixel = 0;
		dWidthRealUM = 0;
		dHeightRealUM = 0;
		dDiagonalRealUM = 0;
	}
	int		nWidthPixel;
	int		nHeightPixel;
	double	dDiagonalPixel;

	double	dWidthRealUM;
	double	dHeightRealUM;
	double	dDiagonalRealUM;

};

class AFX_EXT_CLASS CCameraView : public CWnd, public CCHImageData
{
	DECLARE_DYNAMIC(CCameraView)

public:
	CCameraView(int nViewIndex=0, CWnd* pParentWnd=NULL);
	virtual ~CCameraView();

	// getter 
	double	GetWidthScale();
	double	GetHeightScale();
	int		GetScaleWidth();
	int		GetScaleHeight();
	int		GetHScrollPos();
	int		GetVScrollPos();

	// setter
	void	SetViewMode(int nViewMode);
	void	SetViewName(const CString& strViewName);
	void	SetParentWnd(CWnd* pParentWnd);
	void	SetDrawCenterLine(BOOL bDraw);
	void	SetDrawRuler(BOOL bDraw);
	void	SetResolution(double dRes);
	void	SetRulerGab(double dGab);
	
	// loader
	BOOL	LoadImage(const CString& strFilename);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnLoadImage();
	afx_msg void OnSaveImage();
	afx_msg void OnViewName();
	afx_msg void OnCenterLine();
	afx_msg void OnCenterRect();
	afx_msg void OnRuler();
	afx_msg void OnViewOrigin();
	afx_msg void OnViewFit();
	afx_msg void OnViewColor();
	afx_msg void OnViewGray();
	afx_msg void OnViewRed();
	afx_msg void OnViewGreen();
	afx_msg void OnViewBlue();

protected:
	void	DrawViewName(CDC *pDC);
	void	DrawCenterRect(CDC *pDC);
	void	DrawCenterLine(CDC *pDC);
	void	DrawRuler(CDC *pDC);
	void	UpdateView(CDC *pDC);
	virtual void PopUpCommandMenu(const CPoint& point);
			
protected:
	CRITICAL_SECTION	m_csImageData;
	int					m_nViewIndex;
	CCHImageData		m_OriginImage;

	CWnd				*m_pParentWnd;

	int					m_nViewMode;
	int					m_nViewBand;
	CRect				m_rtViewRect;
	CString				m_strViewName;
		
	int					m_nScaleWidth;
	int					m_nScaleHeight;
	double				m_dWidthScale; 
	double				m_dHeightScale;

	// Scroll Pos
	int					m_nVScroll;
	int					m_nHScroll;
	// Max Scroll Pos
	int					m_nMaxVScroll;
	int					m_nMaxHScroll;

	BOOL				m_bDrawViewName;
	BOOL				m_bDrawCenterLine;
	BOOL				m_bDrawCenterRect;
	BOOL				m_bDrawRuler;
	double				m_dRulerGab;
	double				m_dResolution;

public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


