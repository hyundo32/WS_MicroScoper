#pragma once

namespace CHImageControls
{
	enum ColorBandType	{ BandTypeRed=0, BandTypeGreen, BandTypeBlue, BandTypeGray, BandTypeColor };
}

struct _IplImage;

class AFX_EXT_CLASS CCHImageData
{
public:
	CCHImageData(void);
	CCHImageData(int nWidth, int nHeight, int nDepth=8, int nChannels=1);
	virtual ~CCHImageData(void);

	// virtual func
	virtual BOOL	CreateImage(int nWidth, int nHeight, int nDepth=8, int nChannels=1);
	virtual void	ReleaseImage(void);
	virtual int		GetHeight(void) const;
	virtual int		GetWidth(void) const;
	virtual int		GetWidthStep(void) const;
	virtual int		GetDepth(void) const;
	virtual int		GetChannels(void) const;
	virtual int		GetBpp(void) const;
	virtual int		GetImageSize(void) const;
	virtual BOOL	GetImageExist(void) const;
	virtual char*	GetImageBuffer(void);
	virtual char*	GetImageBufferOrigin(void);
	virtual int		GetPixelValue(int x, int y) const;
	virtual int		SetPixelValue(int x, int y, int nR, int nG, int nB);

	virtual BOOL	LoadImage(const CString& strFilename);
	virtual BOOL	SaveImage(const CString& strFilename, int nQiality=95);

	// clear
	BOOL	ClearImage(int nValue);

	// copy func
	BOOL	CopyImageTo(CCHImageData *pToImage);
	BOOL	CopyImageTo(_IplImage *pToImage);
	BOOL	CopyImageFrom(const CCHImageData *pFromImage);
	BOOL	CopyImageFrom(const _IplImage *pFromImage);

	// show func
	void	ShowImage(HDC hDC, const CRect& dstRect);
	void	ShowImage(HDC hDC, const CRect& srcRect, const CRect& dstRect);
	void	ShowImage(HDC hDC, int nSx, int nSy, int nWidth, int nHeight, int nFromX, int nFromY);

	// get image data
	BOOL	GetSubImage(int nSx, int nSy, int nWidth, int nHeight, CCHImageData *pGetImage);
	BOOL	GetSubImage(int nSx, int nSy, int nWidth, int nHeight, int nChannels, BYTE *pGetBuffer);
	BOOL	GetSubImage(const CRect& rtRect, CCHImageData *pGetImage);
	BOOL	GetSubImage(const CRect& rtRect, int nChannels, BYTE *pGetBuffer);

	BOOL	GetBandImage(int nColorBand, CCHImageData* pGetImage);
	BOOL	GetBandImage(int nColorBand, int nSx, int nSy, int nWidth, int nHeight, CCHImageData* pGetImage);
	BOOL	GetBandImage(int nColorBand, int nSx, int nSy, int nWidth, int nHeight, BYTE* pGetBuffer);
	BOOL	GetBandImage(int nColorBand, const CRect& rtRect, CCHImageData* pGetImage);
	BOOL	GetBandImage(int nColorBand, const CRect& rtRect, BYTE* pGetBuffer);

	// set image data
	BOOL	SetSubImage(int nSx, int nSy, int nWidth, int nHeight, CCHImageData* pSetImage);
	BOOL	SetSubImage(int nSx, int nSy, int nWidth, int nHeight, int nBpp, BYTE *pSetBuffer);
	BOOL	SetSubImage(const CRect& rtRect, CCHImageData *pSetImage);
	BOOL	SetSubImage(const CRect& rtRect, int nChannels, BYTE *pSetBuffer);

	// draw function
	BOOL	DrawLine(CPoint ptPos1, CPoint ptPos2, DWORD nColor, int nThickness=1, int nLineType=8, int nShift=0 );
	BOOL	DrawRectangle(CPoint ptPos1, CPoint ptPos2, DWORD nColor, int nThickness=1, int nLineType=8, int nShift=0 );
	BOOL	DrawCircle(CPoint ptCenter, int nRadius, DWORD nColor, int nThickness=1, int nLineType=8, int nShift=0 );
	BOOL	DrawEllipse(CPoint ptCenter, CPoint ptSize, double dAngle, DWORD nColor, int nThickness=1, int nLineType=8, int nShift=0);
	BOOL	DrawText(CPoint ptPoint, DWORD nColor, const CString& strText);

	// buffer access
	_IplImage*		GetIplImage(void);
	BOOL DCtoIplImage(CDC* pDC, CRect rect);

	friend class CCHImageProcess;

protected:
	void		FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
	_IplImage	*m_pImageData;
};
