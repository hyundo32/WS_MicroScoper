#pragma once

namespace CHImageControls
{
	enum ImageConvertType	{ ConvertTypeGray2RGB=0, ConvertTypeRGB2Gray };
	enum ImageFlipType		{ FlipTypeBoth=-1, FlipTypeVert=1, FlipTypeHoriz=2 };
	enum ImageRotateType	{ RotateTypeRight=0, RotateTypeLeft };
	enum ProcessResultType	{ ResultSuccess=1, ResultImageNULL=0, ResultImageNoExist=-1, ResultFail=-2 };
}

#include <list>
#include <vector>
#include <algorithm>
#include "CHImageData.h"

struct _IplConvKernel;


class AFX_EXT_CLASS CLineData
{
public:
	CLineData()
	{
		dPoint1X = 0.;
		dPoint1Y = 0.;
		dPoint2X = 0.;
		dPoint2Y = 0.;
		dRatio = 0.;
		dPeak = 0.;
		nCount = 0;
	}

	CLineData(double dFx, double dFy, double dSx, double dSy, double dCount=0)
	{
		dPoint1X = dFx;
		dPoint1Y = dFy;
		dPoint2X = dSx;
		dPoint2Y = dSy;
		dLength = sqrt((dPoint1X - dPoint2X) * (dPoint1X - dPoint2X) + (dPoint1Y - dPoint2Y) * (dPoint1Y - dPoint2Y));
		dRatio = 0.;
		dPeak = 0.;
		nCount = 0;
	}

	double dPoint1X;
	double dPoint1Y;
	double dPoint2X;
	double dPoint2Y;
	double dLength;
	double	dRatio;
	double	dPeak;
	int		nCount;

};
typedef std::vector<CLineData>						VectorLineData;
typedef std::vector<CLineData>::iterator			VectorLineDataIt;

class AFX_EXT_CLASS CCircleData
{
public:
	CCircleData()
	{
		dCenterX	= 0.;
		dCenterY	= 0.;
		dRadius		= 0.;
	}

	double dCenterX;
	double dCenterY;
	double dRadius;
};
typedef std::vector<CCircleData>					VectorCircleData;
typedef std::vector<CCircleData>::iterator			VectorCircleDataIt;

struct SDefectBlob
{
	SDefectBlob()	
	{ 
		Reset(); 
	}

	SDefectBlob(int left, int top, int right, int bottom)
	{
		nLeft	= left;
		nTop	= top;
		nRight	= right;
		nBottom = bottom;
		nPeak	= 0;
		nCount	= 0;
		//	vectorPoint.clear();
	}

	void Reset()
	{
		nLeft	= INT_MAX;
		nTop	= INT_MAX;
		nRight	= INT_MIN;
		nBottom = INT_MIN;
		nPeak	= 0;
		nCount	= 0;
		//	vectorPoint.clear();
	}

	int		GetSquareSize()	const	{ return (nRight-nLeft+1) * (nBottom-nTop+1); }
	int		GetWidth() const		{ return (nRight-nLeft+1); }
	int		GetHeight() const		{ return (nBottom-nTop+1); }
	int		GetCenterX() const		{ return nLeft + (nRight - nLeft + 1) / 2; }
	int		GetCenterY() const		{ return nTop + (nBottom - nTop + 1) / 2; }
	int		GetMaxSize() const		{ return (GetWidth() < GetHeight()) ? GetHeight(): GetWidth(); }
	int		GetMinSize() const		{ return (GetWidth() > GetHeight()) ? GetHeight(): GetWidth(); }
	size_t	GetPixelCount()	const	{ return nCount; }
	int		GetAvgPeak() const		{ return (nCount==0) ? 0 : nPeak / nCount; }

	int					nLeft;
	int					nTop;
	int					nRight;
	int					nBottom;
	int					nPeak;
	int					nCount;

	//std::vector<CPoint> vectorPoint;
};
typedef std::vector<SDefectBlob>			VectorDefectBlob;
typedef std::vector<SDefectBlob>::iterator	VectorDefectBlobIt;


class AFX_EXT_CLASS CCHImageProcess
{
public:
	class CPolynomial
	{
	public:

		CPolynomial()		{ Reset(); }

		CPolynomial(int degree, double coef)
		{
			nDegree = degree;
			dCoef	= coef;
		}

		~CPolynomial()	{ Reset(); }

		void Reset()
		{
			nDegree = 0;
			dCoef	= 0.0;
		}

		int		nDegree;
		double	dCoef;
	};

	typedef std::list<CPolynomial*>				ListPolynomial;
	typedef std::list<CPolynomial*>::iterator	ListPolynomialIt;

	typedef std::vector<double>						VectorDouble;
	typedef std::vector<double>::iterator			VectorDoubleIt;

	static BOOL	CalculateLagrange(VectorDouble& vectorX, VectorDouble& vectorY, ListPolynomial& listPolynomial);

public:
	CCHImageProcess(void);
	virtual ~CCHImageProcess(void);

	static int ImageConvert(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nConvertCode);
	static int ImageAverage(CCHImageData* pSourceImage, double& dValue);

	static int ImageResize(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nNewWidth, int nNewHeight, int nInterpolation=1);
	static int ImageResize(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dScale, int nInterpolation=1);
	static int ImageFlip(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nFlipMode);

	static int ImageErode(CCHImageData* pSourceImage, CCHImageData* pResultImage, _IplConvKernel* pElement=NULL, int nIterations=1);
	static int ImageDilate(CCHImageData* pSourceImage, CCHImageData* pResultImage, _IplConvKernel* pElement=NULL, int nIterations=1);
	static int ImageOpening(CCHImageData* pSourceImage, CCHImageData* pResultImage, _IplConvKernel* pElement=NULL, int nIterations=1);
	static int ImageClosing(CCHImageData* pSourceImage, CCHImageData* pResultImage, _IplConvKernel* pElement=NULL, int nIterations=1);

	static int ImageRotate(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dAngle, int nFlag=9, UCHAR nValue=0);
	static int ImageRotate(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nRotateType);

	static int ImageSmooth(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nSmoothType=2, int nSize1=3, int nSize2=0, double dSigma1=1.0, double dSigma2=1.0);
	static int ImageNormalize(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dMin, double dMax, int nType=32);

	static int ImageAnd(CCHImageData* pSourceImage, CCHImageData* pMaskImage, CCHImageData* pResultImage);
	static int ImageEqualizeHist(CCHImageData* pSourceImage, CCHImageData* pResultImage);
	static int ImagePreCornerDetect(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nApertureSize=3);
	static int ImageSobel(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nXOrder, int nYOrder, int nApertureSize=3);
	static int ImageSobel(CCHImageData* pSourceImage, CCHImageData* pXImage, CCHImageData* pYImage, int nApertureSize=0);
	static int ImageCanny(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dThreshold1, double dThreshold2, int nApertureSize=3);
	static int ImageAdaptiveThreshold(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dMaxValue, int nMethodType=0, int nThresholdType=0, int nBlockSize=3, double dParam1=5);
	static int ImageThreshold(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dThresholdValue, double dMaxValue, int nThresholdType);

	static int ImageFFT(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nFlags, CCHImageData* pNormalImage=NULL);
	static int ImageInverseFFT(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nWidth, int nHeight);

	static int ImageLowPassFilter(CCHImageData* pSourceImage, CCHImageData* pResultImage, int D0, int N, int nFlags=1, CCHImageData *pNormalImage=NULL);
	static int ImageHighPassFilter(CCHImageData* pSourceImage, CCHImageData* pResultImage, int D0, int N, int nFlags=1, CCHImageData *pNormalImage=NULL);

	static int ImageHoughLines2(CCHImageData* pSourceImage, int nMethod, double dRho, double dTheta, int nThreshold, double dParam1, double dParam2, VectorLineData& vectorLineData);
	static int ImageHoughCircles(CCHImageData* pSourceImage, int nMethod, double dDp, double dMinDist, double dParam1, double dParam2, int nMinRadius, int nMaxRadius, VectorCircleData& vectorCircleData);

	static int MatrixSolve(double *A, double* B, double* X, int nSize, int nMethod=0);

	static int ImageDCT(CCHImageData* pSourceImage, CCHImageData* pResultImage);
	static int ImageInvertDCT(CCHImageData* pSourceImage, CCHImageData* pResultImage);
	static void	RemovePolynomial(ListPolynomial& listPolynomial);

	static int ImageFindLineInfo(CCHImageData *pSource, float rho, float theta, int threshold, int lineLength, int lineGap, VectorLineData& vectorLine, int linesMax=INT_MAX);
	//static int FastMatchTemplate(const IplImage& source, const IplImage&  target, std::vector<CvPoint>* foundPointsList, std::vector<double>* confidencesList, int matchPercentage=70, BOOL findMultipleTargets=TRUE, int numMaxima=10, int numDownPyrs=2, int searchExpansion=15);

	static	BOOL	ImageSmooth_HybridMedianFilter(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nFilterSize);
	static	BOOL	ImageSmooth_HybridMedianFilter2(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nFilterSize);
	static	int		GetHybridMedianFilterValue(BYTE* pKernelValue, int nFilterSize);
	static	void	Swap(int* pA, int* pB);
	static	void	QuickSort(int* nArr, int nStart, int nEnd);
	static	void	ImageDiffusion(BYTE *pBuffer, int w, int h, int iter, float lambda=0.25f, float k=4.0f);

	// pixel
	static int ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CCHImageData* pResultImage, int nMethod=5);
	static int ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CRect& rtROIRegion,  CCHImageData* pResultImage, int nMethod=5);
	static int ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, int& nResultX, int& nResultY, double& dResultValue, int nMethod=5);
	static int ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, int& nResultX, int& nResultY, double& dResultValue, CCHImageData* pResultImage, int nMethod=5);
	static int ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CRect& rtROIRegion, int& nResultX, int& nResultY, double& dResultValue, int nMethod=5);
	static int ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CRect& rtROIRegion, int& nResultX, int& nResultY, double& dResultValue, CCHImageData* pResultImage, int nMethod=5);

	// sub pixel
	static int ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, double& dResultX, double& dResultY, double& dResultValue, int nMethod=5, int nSubPixel=0);
	static int ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, double& dResultX, double& dResultY, double& dResultValue, CCHImageData* pResultImage, int nMethod=5, int nSubPixel=0);
	static int ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CRect& rtROIRegion, double& dResultX, double& dResultY, double& dResultValue, int nMethod=5, int nSubPixel=0);
	static int ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CRect& rtROIRegion, double& dResultX, double& dResultY, double& dResultValue, CCHImageData* pResultImage, int nMethod=5, int nSubPixel=0);

	// pixel
	static int ImageFastMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, int& nResultX, int& nResultY, double& dResultValue);
	static int ImageConvolution(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nLeft, int nTop, int nRight, int nBottom, int nKernelX, int nKernelY, int nPitch, int nThres);
	static int ImageLocalConvolution(CCHImageData* pSourceImage, CCHImageData* pVertImage, CCHImageData* pHoriImage, int nPitchX, int nPitchY, int& nPosX, int& nPosY);
	static int ImageFindDefect(CCHImageData *pImageData, int nPitchX, int nPitchY, int nThreshold, int nBlobMargin, POINT& ptResult, int nLocalCount=2, int nKernelX=1, int nKernelY=1);
	static int ImageBlobAnalysis4FindDefect(BYTE *pImage, int nWidth, int nHeight, int nThreshold, int nBlobMargin, int nStartX, int nStartY, VectorDefectBlob& vectorBlob);

	static int ImageIntegral(CCHImageData* pSourceImage, CCHImageData* pResultImage);
	static int ImageKernelMean(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nKernelSizeW, int nKernelSizeH);
protected:
	

	static void	InsertPolynomial(ListPolynomial &listPolynomial, int degree, double coef);
	static double	Combi(VectorDouble& vectorX, VectorDouble& vectorY, int n, int r);

};

