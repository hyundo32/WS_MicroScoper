#if !defined(__ASGINSPECTION_INCLUDED__)
#define __ASGINSPECTION_INCLUDED__

#include "AssemParam.h"
#include "AssemDefect.h"

class AFX_EXT_CLASS CASGInspection
{
	// Construction
public:
	CASGInspection();
	virtual ~CASGInspection();

public:
	void			SetPixelBuf(int *pPixelX,int *pPixelY,short *pPixelType,short *pPixelValue,int *nPixelCnt)
	{
		m_pPixelX = pPixelX,m_pPixelY = pPixelY,m_pPixelType = pPixelType,m_pPixelValue = pPixelValue,m_nPixelCnt = nPixelCnt;
	}
	void			SetParingBuf(CDefectPair *pDefectPair,int nParingCnt)
	{
		m_pDefectParing = pDefectPair,m_nDefectParingCnt=nParingCnt;
	}
	int				GetParingCnt(){return m_nDefectParingCnt;}

	void			VConvolutionConvC(CConvParam *pParam);

protected:
	BOOL			GetBufferCheck();
	void			SetPixelData(int x, int y, int sub, int threshold);
	BOOL			InsertPairing(int x, int y, int sub, int threshold, int graySrc, int grayRef);
	CRITICAL_SECTION	m_csDefect;
protected:
	int				*m_pPixelX;
	int				*m_pPixelY;
	short			*m_pPixelType;
	short			*m_pPixelValue;
	int				*m_nPixelCnt;
	CDefectPair		*m_pDefectParing;
	int				m_nDefectParingCnt;

};

#endif