#pragma once

#include "AssemParam.h"

class AFX_EXT_CLASS CPixelStorage
{
protected:
	CConvParam	m_Param;
public:
	int		m_nPixel;

	int		*m_pPixelX;
	int		*m_pPixelY;
	short	*m_pPixelType;
	short	*m_pPixelValue;

	int		*m_pGraySrc;
	int		*m_pGrayCmp;
	int		*m_pZoneID;
	int		*m_pZoneTh;

public:
	int		m_nPixelSpace;
	int		m_maxPixel;

public:
	CPixelStorage();
	virtual ~CPixelStorage();

public:
	int		InitPixelStorage(int maxPixel= 10000);
	void	SetConvParam(CConvParam *pParam);
	void	ResetPixelStorage();
	int		GetPixelCount(){return m_nPixel;}

	int		GetThreadDefectPixelNum()		{ return m_nPixel; }
	int		GetOverflowedPixelY()			{ return m_pPixelY[m_nPixel - 1]; }


	int	GetPixelX(int i){return m_pPixelX[i];}
	int	GetPixelY(int i){return m_pPixelY[i];}
	int	GetPixelType(int i){return m_pPixelType[i];}
	int	GetPixelValue(int i){return m_pPixelValue[i];}
	int	GetPeak(int i){return m_pPixelValue[i];}
	int	GetThreshold(int i){return m_pZoneTh[i];}
	int	GetSrcGray(int i){return m_pGraySrc[i];}
	int	GetCmpGray(int i){return m_pGrayCmp[i];}
};

