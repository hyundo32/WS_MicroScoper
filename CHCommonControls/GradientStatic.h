#pragma once


// CGradientStatic
typedef UINT (CALLBACK* LPFNDLLFUNC1)(HDC,CONST PTRIVERTEX,DWORD,CONST PVOID,DWORD,DWORD);

class AFX_EXT_CLASS CGradientStatic : public CStatic
{
	DECLARE_DYNAMIC(CGradientStatic)

public:
	CGradientStatic();
	virtual ~CGradientStatic();
	void SetWindowText(LPCTSTR a_lpstr);
	void SetColor(long cl) {clLeft=cl;};
	void SetGradientColor(long cl) {clRight=cl;};
	void SetTextColor(long cl) {clText=cl;};
	void SetReverseGradient();
	void SetLeftSpacing(int iNoOfPixels) { m_iLeftSpacing = iNoOfPixels; };
	void SetTextAlign(int iAlign ) { m_iAlign = iAlign; }; //0 - left, 1 - center, 2 -right
	void SetVerticalGradient(BOOL a_bVertical = TRUE) { m_bVertical = a_bVertical; };

	static void DrawGradRect(CDC *pDC, CRect r, COLORREF cLeft, COLORREF cRight, BOOL a_bVertical);

protected:
	CString m_sTEXT;
	int m_iLeftSpacing;
	long clLeft;
	long clRight;
	long clText;
	int m_iAlign;
	HINSTANCE hinst_msimg32;
	BOOL m_bCanDoGradientFill;
	BOOL m_bVertical;
	LPFNDLLFUNC1 dllfunc_GradientFill;

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
public:
};


