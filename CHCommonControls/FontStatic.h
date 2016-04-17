#pragma once

// Defines for the font style
#define FS_NORMAL			0x00
#define FS_BOLD				0x01
#define FS_ITALIC			0x02
#define FS_UNDERLINED		0x04
#define FS_STRIKETHROUGH	0x08
#define FS_ANTIALIAS		0x10

// Defines for horizontal alignment
#define FS_CENTER			0x20
#define FS_LEFT				0x40
#define FS_RIGHT			0x80

// CFontStatic

class AFX_EXT_CLASS CFontStatic : 
	public CStatic
{
	DECLARE_DYNAMIC(CFontStatic)

public:
	CFontStatic();

	// Attributes
public:
	bool m_bBold;
	bool m_bItalic;
	bool m_bUnderlined;
	bool m_bStrikethrough;
	bool m_bAntialias;
	bool m_bBgColor;

	bool m_bCenter;
	bool m_bLeft;
	bool m_bRight;

	CString m_szText;
	CString m_szFont;
	DWORD m_dwColor;
	DWORD m_dwBgColor;
	int m_nSize;

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontStatic)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Implementation
public:
	void SetWindowText(LPCTSTR a_lpstr);
	void SetFontStyle(DWORD dwStyle);
	void SetBackground(DWORD dwBgColor);
	void SetFontStatic(CString szFont, int nSize, DWORD dwColor, DWORD dwStyle);
	virtual ~CFontStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


