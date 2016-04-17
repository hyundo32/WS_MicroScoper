#include "StdAfx.h"
#include "CHImageProcess.h"
#include "CHDataFitting.h"
#include "NCC.h"
#include <vector>

#include "cv.h"
#include "highgui.h"


using namespace CHImageControls;

inline bool CompareAvgPeak(SDefectBlob& a, SDefectBlob& b)
{
	return (a.GetAvgPeak() > b.GetAvgPeak());
}

CCHImageProcess::CCHImageProcess(void)
{

}

CCHImageProcess::~CCHImageProcess(void)
{

}

int CCHImageProcess::ImageAverage(CCHImageData* pSourceImage, double& dValue)
{
	if (pSourceImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (pSourceImage->GetChannels()!=1) return ResultImageNoExist;

	CvScalar sValue = cvAvg(pSourceImage->m_pImageData);

	dValue = sValue.val[0];

	return ResultSuccess;
}

int CCHImageProcess::ImageConvert(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nConvertCode)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	switch(nConvertCode)
	{
	case ConvertTypeGray2RGB:

		if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), 3)) return FALSE;

		cvCvtColor(pSourceImage->m_pImageData, pResultImage->m_pImageData, CV_GRAY2BGR);

		return ResultSuccess;

	case ConvertTypeRGB2Gray:
		if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), 1)) return FALSE;

		cvCvtColor(pSourceImage->m_pImageData, pResultImage->m_pImageData, CV_BGR2GRAY);

		return ResultSuccess;
	}

	return ResultImageNoExist;
}

int CCHImageProcess::ImageThreshold(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dThresholdValue, double dMaxValue, int nThresholdType)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), 8, 1)) return ResultFail;

	cvThreshold(pSourceImage->m_pImageData, pResultImage->m_pImageData, dThresholdValue, dMaxValue, nThresholdType);

	return ResultSuccess;
}

int CCHImageProcess::ImageResize(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nNewWidth, int nNewHeight, int nInterpolation)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(nNewWidth, nNewHeight, pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultFail;

	cvResize(pSourceImage->m_pImageData, pResultImage->m_pImageData, nInterpolation);

	return ResultSuccess;
}

int CCHImageProcess::ImageResize(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dScale, int nInterpolation)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	int nNewWidth = int(double(pSourceImage->GetWidth()) * dScale);
	int nNewHeight = int(double(pSourceImage->GetHeight()) * dScale);

	if (!pResultImage->CreateImage(nNewWidth, nNewHeight, pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultFail;

	cvResize(pSourceImage->m_pImageData, pResultImage->m_pImageData, nInterpolation);

	return ResultSuccess;
}

int CCHImageProcess::ImageFlip(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nFlipMode)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultImageNoExist;

	cvFlip(pSourceImage->m_pImageData, pResultImage->m_pImageData, nFlipMode);

	return ResultSuccess;
}

int CCHImageProcess::ImageErode(CCHImageData* pSourceImage, CCHImageData* pResultImage, IplConvKernel* pElement, int nIterations)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultImageNoExist;

	cvErode(pSourceImage->m_pImageData, pResultImage->m_pImageData, pElement, nIterations);

	return ResultSuccess;
}

int CCHImageProcess::ImageDilate(CCHImageData* pSourceImage, CCHImageData* pResultImage, IplConvKernel* pElement, int nIterations)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultImageNoExist;

	cvDilate(pSourceImage->m_pImageData, pResultImage->m_pImageData, pElement, nIterations);

	return ResultSuccess;
}


int CCHImageProcess::ImageOpening(CCHImageData* pSourceImage, CCHImageData* pResultImage, IplConvKernel* pElement, int nIterations)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultImageNoExist;

	cvMorphologyEx(pSourceImage->m_pImageData, pResultImage->m_pImageData, pElement, pElement, CV_MOP_OPEN, nIterations);

	return ResultSuccess;
}

int CCHImageProcess::ImageClosing(CCHImageData* pSourceImage, CCHImageData* pResultImage, IplConvKernel* pElement, int nIterations)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultImageNoExist;

	cvMorphologyEx(pSourceImage->m_pImageData, pResultImage->m_pImageData, pElement, pElement, CV_MOP_CLOSE, nIterations);

	return ResultSuccess;
}

int CCHImageProcess::ImageRotate(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dAngle, int nFlag, UCHAR nValue )
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultImageNoExist;

	CvMat *pRotationMatrix = cvCreateMat(2, 3, CV_32FC1);
	CvPoint2D32f centerPos;
	centerPos.x = float(pSourceImage->GetWidth())/2.f;
	centerPos.y = float(pSourceImage->GetHeight())/2.f;

	cv2DRotationMatrix(centerPos, dAngle, 1.0, pRotationMatrix);

	CvScalar fillval=cvScalarAll(nValue);

	cvWarpAffine(pSourceImage->m_pImageData, pResultImage->m_pImageData, pRotationMatrix, nFlag, fillval);

	cvReleaseMat(&pRotationMatrix);

	

	return ResultSuccess;
}

int CCHImageProcess::ImageRotate(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nRotateType)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	int nSrcWidth = pSourceImage->GetWidth();
	int nSrcHeight = pSourceImage->GetHeight();

	if (!pResultImage->CreateImage(nSrcHeight, nSrcWidth, pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultImageNoExist;
	cvTranspose(pSourceImage->m_pImageData, pResultImage->m_pImageData);

	switch(nRotateType)
	{
	case RotateTypeRight:
		cvFlip(pResultImage->m_pImageData, NULL, 1);
		break;

	case RotateTypeLeft:
		cvFlip(pResultImage->m_pImageData, NULL, 0);
		break;

	default:
		return ResultImageNoExist;
	}

	return ResultSuccess;
}

int CCHImageProcess::ImageSmooth(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nSmoothType, int nSize1, int nSize2, double dSigma1, double dSigma2)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultImageNoExist;

	cvSmooth(pSourceImage->m_pImageData, pResultImage->m_pImageData, nSmoothType, nSize1, nSize2, dSigma1, dSigma2);

	return ResultSuccess;
}

int CCHImageProcess::ImageNormalize(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dMin, double dMax, int nType)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	CCHImageData tmpImage;
	if (!tmpImage.CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), IPL_DEPTH_8U, pSourceImage->GetChannels())) return ResultImageNoExist;

	cvNormalize(pSourceImage->m_pImageData, tmpImage.m_pImageData, dMin, dMax, nType);

	if (!pResultImage->CreateImage(tmpImage.GetWidth(), tmpImage.GetHeight(), IPL_DEPTH_8U, tmpImage.GetChannels())) return ResultImageNoExist;

	cvCvtScaleAbs(tmpImage.m_pImageData, pResultImage->m_pImageData);

	return ResultSuccess;
}


int CCHImageProcess::ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CCHImageData* pResultImage, int nMethod)
{
	if (pSourceImage==NULL || pTemplateImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist() || !pTemplateImage->GetImageExist()) return ResultImageNoExist;

	if (pResultImage==NULL) return ResultImageNoExist;

	int nSrcWidth = pSourceImage->GetWidth();
	int nSrcHeight = pSourceImage->GetHeight();
	int nTmpWidth = pTemplateImage->GetWidth();
	int nTmpHeight = pTemplateImage->GetHeight();

	int nWidth = nSrcWidth - nTmpWidth + 1;
	int nHeight = nSrcHeight - nTmpHeight + 1;

	if (nWidth<1 || nHeight<1) return FALSE;

	if (!pResultImage->CreateImage(nWidth, nHeight, IPL_DEPTH_32F, 1)) return ResultImageNoExist;

	cvMatchTemplate(pSourceImage->m_pImageData, pTemplateImage->m_pImageData, pResultImage->m_pImageData, nMethod);

	return ResultSuccess;
}

int CCHImageProcess::ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CRect& rtROIRegion, CCHImageData* pResultImage, int nMethod)
{
	if (pSourceImage==NULL || pTemplateImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist() || !pTemplateImage->GetImageExist()) return ResultImageNoExist;

	int nTmpWidth = pTemplateImage->GetWidth() / 2;
	int nTmpHeight = pTemplateImage->GetHeight() / 2;

	rtROIRegion.left	= (rtROIRegion.left<0)								? 0:							rtROIRegion.left;
	rtROIRegion.top		= (rtROIRegion.top<0)								? 0:							rtROIRegion.top;
	rtROIRegion.right	= (rtROIRegion.right>pSourceImage->GetWidth())		? pSourceImage->GetWidth():		rtROIRegion.right;
	rtROIRegion.bottom	= (rtROIRegion.bottom>pSourceImage->GetHeight())	? pSourceImage->GetHeight():	rtROIRegion.bottom;

	if (rtROIRegion.Width() < pTemplateImage->GetWidth() || rtROIRegion.Height() < pTemplateImage->GetHeight()) 
	{
		return ResultImageNoExist;
	}

	CvRect orgROI = cvGetImageROI(pSourceImage->m_pImageData);
	cvSetImageROI(pSourceImage->m_pImageData, cvRect(rtROIRegion.left, rtROIRegion.top, rtROIRegion.Width(), rtROIRegion.Height()));

	int nSrcWidth		= rtROIRegion.Width();
	int nSrcHeight		= rtROIRegion.Height();
	nTmpWidth			= pTemplateImage->GetWidth();
	nTmpHeight			= pTemplateImage->GetHeight();

	int nWidth = nSrcWidth - nTmpWidth + 1;
	int nHeight = nSrcHeight - nTmpHeight + 1;

	if (nWidth<1 || nHeight<1) return ResultImageNoExist;

	if (!pResultImage->CreateImage(nWidth, nHeight, IPL_DEPTH_32F, 1)) return ResultImageNoExist;

	cvMatchTemplate(pSourceImage->m_pImageData, pTemplateImage->m_pImageData, pResultImage->m_pImageData, nMethod);

	cvSetImageROI(pSourceImage->m_pImageData, orgROI);

	rtROIRegion = rtROIRegion;

	return ResultSuccess;
}

void GetBestPosition(int nMethod, CCHImageData *pResult, int& nResultX, int& nResultY, double& dResultValue)
{
	double dMinValue = 0.0;
	double dMaxValue = 0.0;
	CvPoint ptMinPos;
	CvPoint ptMaxPos;

	cvMinMaxLoc(pResult->GetIplImage(), &dMinValue, &dMaxValue, &ptMinPos, &ptMaxPos, NULL);

	switch(nMethod)
	{
	case CV_TM_SQDIFF:
		nResultX = ptMinPos.x;
		nResultY = ptMinPos.y;
		dResultValue = dMinValue;
		break;

	case CV_TM_SQDIFF_NORMED:
		nResultX = ptMinPos.x;
		nResultY = ptMinPos.y;
		dResultValue = dMinValue;
		break;

	case CV_TM_CCORR:
		nResultX = ptMaxPos.x;
		nResultY = ptMaxPos.y;
		dResultValue = dMaxValue;
		break;

	case CV_TM_CCORR_NORMED:
		nResultX = ptMaxPos.x;
		nResultY = ptMaxPos.y;
		dResultValue = dMaxValue;
		break;

	case CV_TM_CCOEFF:
		nResultX = ptMaxPos.x;
		nResultY = ptMaxPos.y;
		dResultValue = dMaxValue;
		break;

	case CV_TM_CCOEFF_NORMED:
		nResultX = ptMaxPos.x;
		nResultY = ptMaxPos.y;
		dResultValue = dMaxValue;
		break;
	}
}

int CCHImageProcess::ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, int& nResultX, int& nResultY, double& dResultValue, int nMethod)
{
	CCHImageData resultImage;

	int nResultCode = ImageMatching(pSourceImage, pTemplateImage, &resultImage, nMethod);

	if (nResultCode!=ResultSuccess) return nResultCode;
	
	GetBestPosition(nMethod, &resultImage, nResultX, nResultY, dResultValue);

	resultImage.ReleaseImage();

	return ResultSuccess;
}

int CCHImageProcess::ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, int& nResultX, int& nResultY, double& dResultValue, CCHImageData* pResultImage, int nMethod)
{
	int nResultCode = ImageMatching(pSourceImage, pTemplateImage, pResultImage, nMethod);

	if (nResultCode!=ResultSuccess) return nResultCode;

	GetBestPosition(nMethod, pResultImage, nResultX, nResultY, dResultValue);

	return ResultSuccess;
}

int CCHImageProcess::ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CRect& rtROIRegion, int& nResultX, int& nResultY, double& dResultValue, int nMethod)
{
	CCHImageData resultImage;

	int nResultCode = ImageMatching(pSourceImage, pTemplateImage, rtROIRegion, &resultImage, nMethod);

	if (nResultCode!=ResultSuccess) return nResultCode;

	GetBestPosition(nMethod, &resultImage, nResultX, nResultY, dResultValue);

	resultImage.ReleaseImage();

	return ResultSuccess;
}

int CCHImageProcess::ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CRect& rtROIRegion, int& nResultX, int& nResultY, double& dResultValue, CCHImageData* pResultImage, int nMethod)
{
	int nResultCode = ImageMatching(pSourceImage, pTemplateImage, rtROIRegion, pResultImage, nMethod);

	if (nResultCode!=ResultSuccess) return nResultCode;

	GetBestPosition(nMethod, pResultImage, nResultX, nResultY, dResultValue);

	return ResultSuccess;
}

typedef std::vector<double>				VectorDouble;
typedef std::vector<double>::iterator	VectorDoubleIt;

BOOL CalculatePosition(int nSubPixel, CCHImageData *pResult, int nResultX, int nResultY, double& dResultX, double& dResultY, double& dResultValue)
{
	int nWidth = pResult->GetWidth();
	int nHeight = pResult->GetHeight();
	
	// check result
	if (nResultX<1 || nResultX>(nWidth-2))
	{
		dResultX = nResultX;
		dResultY = nResultY;
		return TRUE;
	}

	if (nResultY<1 || nResultY>(nHeight-2))
	{
		dResultX = nResultX;
		dResultY = nResultY;
		return TRUE;
	}

	// result image pointer
	float *pResultBuffer = (float*) pResult->GetImageBuffer();
	int nWidthStep = pResult->GetWidthStep() / 4;

	VectorDouble vectorX;
	vectorX.push_back(-1.0);
	vectorX.push_back(0.0);
	vectorX.push_back(1.0);

	// x axis
	VectorDouble vectorY;
	vectorY.push_back(pResultBuffer[(nResultY*nWidthStep)+(nResultX-1)]);
	vectorY.push_back(dResultValue);
	vectorY.push_back(pResultBuffer[(nResultY*nWidthStep)+(nResultX+1)]);

	VectorDouble vectorResultX;
	if (CCHDataFitting::PolynomialFitting(vectorX, vectorY, vectorResultX, 2)!=1)
	{
		return FALSE;
	}

	// y axis
	vectorY.clear();
	vectorY.push_back(pResultBuffer[((nResultY-1)*nWidthStep)+nResultX]);
	vectorY.push_back(dResultValue);
	vectorY.push_back(pResultBuffer[((nResultY+1)*nWidthStep)+nResultX]);

	VectorDouble vectorResultY;
	if (CCHDataFitting::PolynomialFitting(vectorX, vectorY, vectorResultY, 2)<1)
	{
		return FALSE;
	}

	dResultX	= (nResultX-1) + (-1.0 * vectorResultX[1]) / (2.0 * vectorResultX[2]);
	dResultY	= (nResultY-1) + (-1.0 * vectorResultY[1]) / (2.0 * vectorResultY[2]);

	dResultValue  = -1.0 * ( ( (vectorResultX[1]*vectorResultX[1]) - (4.0 * vectorResultX[2] * vectorResultX[0]) ) / (4.0 * vectorResultX[2]) );
	dResultValue += -1.0 * ( ( (vectorResultY[1]*vectorResultY[1]) - (4.0 * vectorResultY[2] * vectorResultY[0]) ) / (4.0 * vectorResultY[2]) );
	dResultValue /= 2.0;

	return TRUE;
}

int CCHImageProcess::ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, double& dResultX, double& dResultY, double& dResultValue, int nMethod, int nSubPixel)
{
	CCHImageData resultImage;

	int nResultCode = ImageMatching(pSourceImage, pTemplateImage, &resultImage, nMethod);

	if (nResultCode!=ResultSuccess) return nResultCode;

	int nResultX, nResultY;

	GetBestPosition(nMethod, &resultImage, nResultX, nResultY, dResultValue);

	if (!CalculatePosition(nSubPixel, &resultImage, nResultX, nResultY, dResultX, dResultY, dResultValue)) return ResultFail;

	return ResultSuccess;
}

int CCHImageProcess::ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, double& dResultX, double& dResultY, double& dResultValue, CCHImageData* pResultImage, int nMethod, int nSubPixel)
{
	int nResultCode = ImageMatching(pSourceImage, pTemplateImage, pResultImage, nMethod);

	if (nResultCode!=ResultSuccess) return nResultCode;

	int nResultX, nResultY;

	GetBestPosition(nMethod, pResultImage, nResultX, nResultY, dResultValue);

	if (!CalculatePosition(nSubPixel, pResultImage, nResultX, nResultY, dResultX, dResultY, dResultValue)) return ResultFail;

	return ResultSuccess;
}

int CCHImageProcess::ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CRect& rtROIRegion, double& dResultX, double& dResultY, double& dResultValue, int nMethod, int nSubPixel)
{
	CCHImageData resultImage;

	int nResultCode = ImageMatching(pSourceImage, pTemplateImage, rtROIRegion, &resultImage, nMethod);

	if (nResultCode!=ResultSuccess) return nResultCode;

	int nResultX, nResultY;

	GetBestPosition(nMethod, &resultImage, nResultX, nResultY, dResultValue);

	if (!CalculatePosition(nSubPixel, &resultImage, nResultX, nResultY, dResultX, dResultY, dResultValue)) return ResultFail;

	return ResultSuccess;
}

int CCHImageProcess::ImageMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, CRect& rtROIRegion, double& dResultX, double& dResultY, double& dResultValue, CCHImageData* pResultImage, int nMethod, int nSubPixel)
{
	int nResultCode = ImageMatching(pSourceImage, pTemplateImage, rtROIRegion, pResultImage, nMethod);

	if (nResultCode!=ResultSuccess) return nResultCode;

	int nResultX, nResultY;

	GetBestPosition(nMethod, pResultImage, nResultX, nResultY, dResultValue);

	if (!CalculatePosition(nSubPixel, pResultImage, nResultX, nResultY, dResultX, dResultY, dResultValue)) return ResultFail;

	return ResultSuccess;
}

int CCHImageProcess::ImageLocalConvolution(CCHImageData* pSourceImage, CCHImageData* pVertImage, CCHImageData* pHoriImage, int nPitchX, int nPitchY, int& nPosX, int& nPosY)
{	
	nPosX = nPosY = -1;
	if (pSourceImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	CvRect orgROI = cvGetImageROI(pSourceImage->m_pImageData);

	CCHImageData bandImage, *pProcImage = pSourceImage;
	if (pSourceImage->GetChannels()!=1)
	{
		if (!pSourceImage->GetBandImage(BandTypeGray, &bandImage)) return ResultImageNoExist;
		pProcImage = &bandImage;
	}

	int nWidth = pProcImage->GetWidth();
	int nHeight = pProcImage->GetHeight();
	int nHalfWidth = nWidth / 2;
	int nHalfHeight = nHeight / 2;
	
	CCHImageData tempResult;
	double dMinValue;
	CvScalar scalar;

	if (pVertImage!=NULL && nPitchX>0)
	{
		nPosX = -1;
		CCHImageData vertImg;
		double dMinValue = DBL_MAX;

		pProcImage->GetSubImage(nHalfWidth, 0, nHalfWidth, nHeight, &vertImg);
		tempResult.ReleaseImage();
		tempResult.CreateImage(nHalfWidth, nHeight);
		for (int i=0; i<nPitchX; i++)
		{
			// set roi
			cvSetImageROI(pProcImage->m_pImageData, cvRect(i, 0, nHalfWidth, nHeight));
			cvAbsDiff(pProcImage->m_pImageData, vertImg.m_pImageData, tempResult.m_pImageData);
			scalar = cvAvg(tempResult.m_pImageData);

			if (dMinValue>scalar.val[0])
			{
				nPosX = i;
				dMinValue = scalar.val[0];
				tempResult.CopyImageTo(pVertImage);
			}
		}
	}

	if (pHoriImage!=NULL && nPitchY>0)
	{
		nPosY = -1;
		CCHImageData horiImg;
		dMinValue = DBL_MAX;

		pProcImage->GetSubImage(0, nHalfHeight, nWidth, nHalfHeight, &horiImg);
		tempResult.ReleaseImage();
		tempResult.CreateImage(nWidth, nHalfHeight);
		for (int i=0; i<nPitchY; i++)
		{
			// set roi
			cvSetImageROI(pProcImage->m_pImageData, cvRect(0, i, nWidth, nHalfHeight));
			cvAbsDiff(pProcImage->m_pImageData, horiImg.m_pImageData, tempResult.m_pImageData);
			scalar = cvAvg(tempResult.m_pImageData);

			if (dMinValue>scalar.val[0])
			{
				nPosY = i;
				dMinValue = scalar.val[0];
				tempResult.CopyImageTo(pHoriImage);
			}
		}
	}

	cvSetImageROI(pSourceImage->m_pImageData, orgROI);

	if (pVertImage==NULL && pHoriImage==NULL) ResultImageNoExist;

	return ResultSuccess;

//	CCHImageProcess::ImageThreshold(&vertResult, &vertThres, nThres, 255, CV_THRESH_BINARY);
//	CCHImageProcess::ImageThreshold(&horiResult, &horiThres, nThres, 255, CV_THRESH_BINARY);

// 	// left top
// 	cvSetImageROI(vertResult.m_pImageData, cvRect(0, 0, nHalfWidth, nHalfHeight)); // top
// 	cvSetImageROI(horiResult.m_pImageData, cvRect(0, 0, nHalfWidth, nHalfHeight)); // left
// 	cvSetImageROI(pResultImage->m_pImageData, cvRect(0, 0, nHalfWidth, nHalfHeight));
// 	cvAnd(vertResult.m_pImageData, horiResult.m_pImageData, pResultImage->m_pImageData);
// 
// 	// left bottom
// 	cvSetImageROI(vertResult.m_pImageData, cvRect(0, nHalfHeight, nHalfWidth, nHalfHeight)); // bottom
// 	cvSetImageROI(horiResult.m_pImageData, cvRect(0, 0, nHalfWidth, nHalfHeight)); // left
// 	cvSetImageROI(pResultImage->m_pImageData, cvRect(0, nHalfHeight, nHalfWidth, nHalfHeight));
// 	cvAnd(vertResult.m_pImageData, horiResult.m_pImageData, pResultImage->m_pImageData);
// 
// 	// right top
// 	cvSetImageROI(vertResult.m_pImageData, cvRect(0, 0, nHalfWidth, nHalfHeight)); // top
// 	cvSetImageROI(horiResult.m_pImageData, cvRect(nHalfWidth, 0, nHalfWidth, nHalfHeight)); // right
// 	cvSetImageROI(pResultImage->m_pImageData, cvRect(nHalfWidth, 0, nHalfWidth, nHalfHeight));
// 	cvAnd(vertResult.m_pImageData, horiResult.m_pImageData, pResultImage->m_pImageData);
// 
// 	// right bottom
// 	cvSetImageROI(vertResult.m_pImageData, cvRect(0, nHalfHeight, nHalfWidth, nHalfHeight)); // bottom
// 	cvSetImageROI(horiResult.m_pImageData, cvRect(nHalfWidth, 0, nHalfWidth, nHalfHeight)); // right
// 	cvSetImageROI(pResultImage->m_pImageData, cvRect(nHalfWidth, nHalfHeight, nHalfWidth, nHalfHeight));
// 	cvAnd(vertResult.m_pImageData, horiResult.m_pImageData, pResultImage->m_pImageData);
// 
// 	cvSetImageROI(vertResult.m_pImageData, cvRect(0, 0, nHalfWidth, nHeight));
// 	cvSetImageROI(horiResult.m_pImageData, cvRect(0, 0, nWidth, nHalfHeight));
// 	cvSetImageROI(pResultImage->m_pImageData, cvRect(0, 0, nWidth, nHeight));
// 	cvSetImageROI(pSourceImage->m_pImageData, orgROI);

//	vertThres.SaveImage(_T("d:\\vertImage.bmp"));
//	horiThres.SaveImage(_T("d:\\horiImage.bmp"));
	//pResultImage->SaveImage(_T("d:\\resultImage.bmp"));


	return ResultSuccess;
}

int  CCHImageProcess::ImageIntegral(CCHImageData* pSourceImage, CCHImageData* pResultImage)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (pSourceImage->GetChannels()!=1) return ResultImageNoExist;

	int nWidth = pSourceImage->GetWidth();
	int nHeight = pSourceImage->GetHeight();
	
	if (!pResultImage->CreateImage(nWidth, nHeight, IPL_DEPTH_32S, 1)) return ResultImageNoExist;

	BYTE* image = (BYTE*)pSourceImage->GetImageBuffer();
	int* integral = (int*)pResultImage->GetImageBuffer();

	int nSum = 0;
	for(int xs=0; xs<nWidth; xs++)
	{
		nSum += *image++;		//sum += pi[xs];
		*integral++ = nSum;    //im[xs] = sum;
	}

	for(int ys=1; ys<nHeight; ys++)
	{
		nSum = 0;
		for(int xs=0; xs<nWidth; xs++, integral++)
		{
			nSum += *image++;                            //sum += pi[xs];
			*integral = *(integral-nWidth) + nSum;       //im[xs] = im[xs-XRES] +sum;
		}
	}

	return ResultSuccess;
}

void Get_Sum_Map(BYTE *pImage, int *pIntegral, int nWidth, int nHeight)
{
	BYTE* image = pImage;
	int* integral = pIntegral;

	int nSum = 0;
	for(int xs=0; xs<nWidth; xs++)
	{
		nSum += *image++;		//sum += pi[xs];
		*integral++ = nSum;    //im[xs] = sum;
	}

	for(int ys=1; ys<nHeight; ys++)
	{
		nSum = 0;
		for(int xs=0; xs<nWidth; xs++, integral++)
		{
			nSum += *image++;                            //sum += pi[xs];
			*integral = *(integral-nWidth) + nSum;       //im[xs] = im[xs-XRES] +sum;
		}
	}

}

int CCHImageProcess::ImageKernelMean(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nKernelSizeW, int nKernelSizeH)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;
	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;
	if (pSourceImage->GetChannels()!=1) return ResultImageNoExist;

	int nWidth = pSourceImage->GetWidth();
	int nHeight = pSourceImage->GetHeight();
	int nWidthStep = pSourceImage->GetWidthStep();

	if (!pResultImage->CreateImage(nWidth, nHeight, 8, 1)) return ResultImageNoExist;
	pResultImage->ClearImage(0);

	int nKernelW = nKernelSizeW / 2;
	int nKernelH = nKernelSizeH / 2;
	int nDivValue = nKernelSizeW * nKernelSizeH;

	int *pIntImage = new int[nWidth*nHeight];

	BYTE *pSrc = (BYTE*) pSourceImage->GetImageBuffer();
	BYTE *pRes = (BYTE*) pResultImage->GetImageBuffer();
	int *pInt = pIntImage;

	Get_Sum_Map(pSrc, pInt, nWidth, nHeight);

	pSrc += ((nKernelH+1)*nWidthStep) + (nKernelW+1);
	pRes += ((nKernelH+1)*nWidthStep) + (nKernelW+1);

	int nKernelWLength = (nKernelW*2)+1;
	int nKernelHLength = ((nKernelH*2+1)*nWidth);

	int nYIdx=0;
	int nValue;
	BYTE *pResult, *pSource;
	int *pLeftTop, *pLeftBottom, *pRightTop, *pRightBottom;

	for (int i=(nKernelH+1); i<nHeight-(nKernelH+1); i++)
	{
		pLeftTop		= pInt + (nYIdx++*nWidth);
		pLeftBottom		= pLeftTop + nKernelHLength;
		pRightTop		= pLeftTop + nKernelWLength;
		pRightBottom	= pRightTop + nKernelHLength;
		pResult			= pRes;
		pSource			= pSrc;

		for (int j=(nKernelW+1); j<nWidth-(nKernelW+1); j++)
		{

			nValue = (*pRightBottom - *pLeftBottom - *pRightTop + *pLeftTop);
			*pResult = (nValue/nDivValue);

			pSource++;
			pResult++;
			pLeftTop++;
			pLeftBottom++;
			pRightTop++;
			pRightBottom++;
		}

		pSrc += nWidthStep;
		pRes += nWidthStep;
	}

	delete [] pIntImage;

	return ResultSuccess;

}


int CCHImageProcess::ImageFindDefect(CCHImageData *pImageData, int nPitchX, int nPitchY, int nThreshold, int nBlobMargin, POINT& ptResult, int nLocalCount, int nKernelX, int nKernelY)
{
	if (pImageData==NULL) 
	{
		ptResult = CPoint(-1, -1);
		return 0;
	}

	int nWidth = pImageData->GetWidth();
	int nHeight = pImageData->GetHeight();
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;

	ptResult = CPoint(nCenterX, nCenterY);

	// vertical x, horizontal y
	int nPosX, nPosY;
	CCHImageData vertImage, horiImage;
	CCHImageData *pVertImage = (nPitchX<1) ? NULL: &vertImage;
	CCHImageData *pHoriImage = (nPitchY<1) ? NULL: &horiImage;
	if (CCHImageProcess::ImageLocalConvolution(pImageData, pVertImage, pHoriImage, nPitchX, nPitchY, nPosX, nPosY)!=1)
	{
		return 0;
	}

	VectorDefectBlob resultDefectBlob;

	// vertical x (peak 값이 제일큰 blob n개)
	if (pVertImage && nPosX>-1)
	{
		VectorDefectBlob vertDefectBlob;
		int nLocalWidth = pVertImage->GetWidth();
		int nLocalHeight = pVertImage->GetHeight();
		int nDefectCount = ImageBlobAnalysis4FindDefect((BYTE*)pVertImage->GetImageBuffer(), nLocalWidth, nLocalHeight, nThreshold, nBlobMargin, nPosX, 0, vertDefectBlob);

		for (int i=0; i<nDefectCount; i++)
		{
			if (i==nLocalCount) break;
			resultDefectBlob.push_back(vertDefectBlob[i]);
		}
	}

	// horizontal y (peak 값이 제일큰 blob n개)
	if (pHoriImage && nPosY>-1)
	{
		VectorDefectBlob horiDefectBlob;
		int nLocalWidth = pHoriImage->GetWidth();
		int nLocalHeight = pHoriImage->GetHeight();
		int nDefectCount = ImageBlobAnalysis4FindDefect((BYTE*)pHoriImage->GetImageBuffer(), nLocalWidth, nLocalHeight, nThreshold, nBlobMargin, 0, nPosY, horiDefectBlob);

		for (int i=0; i<nDefectCount; i++)
		{
			if (i==nLocalCount) break;
			resultDefectBlob.push_back(horiDefectBlob[i]);
		}
	}

	// result size check
	if (resultDefectBlob.size()<1)
	{
		return 0;
	}

	// distance check (이미지 센터에서 가장 가까운 결함)
	double dTempDistance;
	double dMinDistance = DBL_MAX;
	for (VectorDefectBlobIt it=resultDefectBlob.begin(); it!=resultDefectBlob.end(); it++)
	{
		dTempDistance = (it->GetCenterX()-nCenterX)*(it->GetCenterX()-nCenterX) + (it->GetCenterY()-nCenterY)*(it->GetCenterY()-nCenterY);

		if (dMinDistance > dTempDistance)
		{
			ptResult.x = it->GetCenterX();
			ptResult.y = it->GetCenterY();
			dMinDistance = dTempDistance;
		}
	}

	return 1;
}

int	CCHImageProcess::ImageBlobAnalysis4FindDefect(BYTE *pImage, int nWidth, int nHeight, int nThreshold, int nBlobMargin, int nStartX, int nStartY, VectorDefectBlob& vectorBlob)
{
	if (pImage==NULL) return 0;

	int i, j, y, x;

	// top & bottom margin
	BYTE *pBuf1 = pImage;
	BYTE *pBuf2 = pImage + (nHeight-1)*nWidth;
	for (i=0; i<nBlobMargin; i++)
	{
		memset(pBuf1, 0, sizeof(BYTE)*nWidth);
		memset(pBuf2, 0, sizeof(BYTE)*nWidth);
		pBuf1 += nWidth;
		pBuf2 -= nWidth;
	}

	// left & right margin
	pBuf1 = pImage;
	for (i=0; i<nHeight; i++)
	{
		memset(pBuf1, 0, sizeof(BYTE)*nBlobMargin);
		memset(pBuf1+(nWidth-nBlobMargin-1), 0, sizeof(BYTE)*nBlobMargin);
		pBuf1 += nWidth;
	}


	size_t nSize;
	std::vector<CPoint> vectorPixel;
	SDefectBlob pixelBlob;
	register BYTE *pSrcImgY = pImage + (nBlobMargin*nWidth) + nBlobMargin;
	register BYTE *pSrcImgX, *pSubX, *pSubY;

	int nBlobMin = -nBlobMargin;
	int nBlobMax = nBlobMargin+1;

	for (i=nBlobMargin; i<nHeight-nBlobMargin; i++)
	{
		pSrcImgX = pSrcImgY;
		for (j=nBlobMargin; j<nWidth-nBlobMargin; j++)
		{
			if (*pSrcImgX>=nThreshold && *pSrcImgX!=255)
			{
				// new pixel blob
				pixelBlob.Reset();
				pixelBlob.nPeak += *pSrcImgX; // add peak
				pixelBlob.nCount++; 

				*pSrcImgX = 255;
				vectorPixel.push_back(CPoint(j, i));

				while ((nSize=vectorPixel.size())>0)
				{
					x = vectorPixel[nSize-1].x;
					y = vectorPixel[nSize-1].y;

					// delete end point
					vectorPixel.pop_back();

					// add point to blob
					//pixelBlob.vectorPoint.push_back(CPoint(x, y));

					// get current point
					pSubY = pImage + ((y+nBlobMin)*nWidth) + (x+nBlobMin);

					for (int sy=nBlobMin; sy<nBlobMax; sy++)
					{
						pSubX = pSubY;
						for (int sx=nBlobMin; sx<nBlobMax; sx++)
						{
							if (*pSubX >= nThreshold && *pSubX!=255)
							{
								pixelBlob.nPeak += *pSubX; // add peak
								pixelBlob.nCount++; 

								*pSubX = 255;
								vectorPixel.push_back(CPoint(x+sx, y+sy));
							}
							pSubX++;
						}
						pSubY += nWidth;
					}

					if (pixelBlob.nLeft>x)		pixelBlob.nLeft	= x;
					if (pixelBlob.nRight<x)		pixelBlob.nRight = x;
					if (pixelBlob.nTop>y)		pixelBlob.nTop	= y;
					if (pixelBlob.nBottom<y)	pixelBlob.nBottom = y;
				}

				vectorPixel.clear();

				pixelBlob.nLeft		+= nStartX;
				pixelBlob.nRight	+= nStartX;
				pixelBlob.nTop		+= nStartY;
				pixelBlob.nBottom	+= nStartY;

				vectorBlob.push_back(pixelBlob);

			} // end if
			pSrcImgX++;
		} //end for j
		pSrcImgY += nWidth;
	}// end for i

	if (vectorBlob.size()==0) return 0;

	// sort blob max size
	std::sort(vectorBlob.begin(), vectorBlob.end(), CompareAvgPeak);

	return (int) vectorBlob.size();
}

int CCHImageProcess::ImageConvolution(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nLeft, int nTop, int nRight, int nBottom, int nKernelX, int nKernelY, int nPitch, int nThres)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	CCHImageData bandImage, *pProcImage = pSourceImage;
	if (pSourceImage->GetChannels()!=1)
	{
		if (!pSourceImage->GetBandImage(BandTypeGray, &bandImage)) return ResultImageNoExist;
		pProcImage = &bandImage;
	}
	
	int nWidth = pProcImage->GetWidth();
	int nHeight = pProcImage->GetHeight();

	if (!pResultImage->CreateImage(nWidth, nHeight, IPL_DEPTH_16S, 1)) return ResultImageNoExist;

	int nSubStartX, nSubStartY;

	switch(nKernelX)
	{
	case 2:
		nSubStartX = 0;
		break;
	case 3:
		nSubStartX = 1;	
		break;
	case 4:
		nSubStartX = 2;	
		break;
	case 5:
		nSubStartX = 2;
		break;
	default:
		return -1;
	}

	switch(nKernelY)
	{
	case 2:
		nSubStartY = 0;
		break;
	case 3:
		nSubStartY = 1;	
		break;
	case 4:
		nSubStartY = 2;
		break;
	case 5:
		nSubStartY = 2;
		break;
	default:
		return -1;
	}

	BYTE *pImage = (BYTE*) pProcImage->GetImageBuffer();
	short *pResult = (short*) pResultImage->GetImageBuffer();
	int nStep = pResultImage->GetWidthStep() / 2;

	BYTE *pImageY = pImage + (nTop*nWidth) + (nLeft);
	short *pResultY = pResult + (nTop*nWidth) + nLeft;
	BYTE *pImageX, *pSubY, *pSubX;
	short *pResultX;

	static int nStartPos = (nSubStartY*nWidth) + nSubStartX;

	__m128i MinThresData		= _mm_set1_epi16(-(nThres*nKernelX*nKernelY));
	__m128i MaxThresData		= _mm_set1_epi16((nThres*nKernelX*nKernelY));
	__m128i ZeroData			= _mm_setzero_si128();
	__m128i CurrentData			= _mm_setzero_si128();
	__m128i DirMaskLow			= _mm_setzero_si128();
	__m128i DirMaskHigh			= _mm_setzero_si128();
	__m128i ResultLow			= _mm_setzero_si128();
	__m128i ResultHigh			= _mm_setzero_si128();
	__m128i LeftResultLow		= _mm_setzero_si128();
	__m128i LeftResultHigh		= _mm_setzero_si128();
	__m128i RightResultLow		= _mm_setzero_si128();
	__m128i RightResultHigh		= _mm_setzero_si128();
	__m128i TempData			= _mm_setzero_si128();

	for (int y=nTop; y<nBottom; y++)
	{
		pImageX = pImageY;
		pResultX = pResultY;

		for (int x=nLeft; x<nRight; x+=16)
		{
			// result set to zero
			ResultLow		= _mm_setzero_si128();
			ResultHigh		= _mm_setzero_si128();
			LeftResultLow	= _mm_setzero_si128();
			LeftResultHigh	= _mm_setzero_si128();
			RightResultLow	= _mm_setzero_si128();
			RightResultHigh	= _mm_setzero_si128();

			// set start pos
			pSubY = pImageX - nStartPos;
			for (int ky=0; ky<nKernelY; ky++)
			{
				pSubX = pSubY;
				for (int kx=0; kx<nKernelX; kx++)
				{
					// get current image [8bit x 16ea]
					CurrentData		= _mm_loadu_si128((__m128i*)(pSubX));
					// unpack and add[8bit => 16bit]
					TempData		= _mm_unpackhi_epi8(CurrentData, ZeroData);		// high
					ResultHigh		= _mm_add_epi16(ResultHigh, TempData);			// high
					TempData		= _mm_unpacklo_epi8(CurrentData, ZeroData);		// low
					ResultLow		= _mm_add_epi16(ResultLow, TempData);			// low

					// get left pitch image [8bit x 16ea]
					CurrentData		= _mm_loadu_si128((__m128i*)(pSubX-nPitch));
					// unpack and add[8bit => 16bit]
					TempData		= _mm_unpackhi_epi8(CurrentData, ZeroData);		// high
					LeftResultHigh	= _mm_add_epi16(LeftResultHigh, TempData);		// high
					TempData		= _mm_unpacklo_epi8(CurrentData, ZeroData);		// low
					LeftResultLow	= _mm_add_epi16(LeftResultLow, TempData);		// low

					// get right pitch image [8bit x 16ea]
					CurrentData		= _mm_loadu_si128((__m128i*)(pSubX+nPitch));
					// unpack and add[8bit => 16bit]
					TempData		= _mm_unpackhi_epi8(CurrentData, ZeroData);		// high
					RightResultHigh	= _mm_add_epi16(RightResultHigh, TempData);		// high
					TempData		= _mm_unpacklo_epi8(CurrentData, ZeroData);		// low
					RightResultLow	= _mm_add_epi16(RightResultLow, TempData);		// low

					pSubX++;
				}
				pSubY += nWidth;
			}


			// (current - left)
			LeftResultHigh	= _mm_sub_epi16(ResultHigh, LeftResultHigh);
			LeftResultLow	= _mm_sub_epi16(ResultLow, LeftResultLow);

			// (current - right)
			RightResultHigh	= _mm_sub_epi16(ResultHigh, RightResultHigh);
			RightResultLow	= _mm_sub_epi16(ResultLow, RightResultLow);

			// compare left
			LeftResultHigh	= _mm_or_si128(_mm_cmplt_epi16(LeftResultHigh, MinThresData), _mm_cmpgt_epi16(LeftResultHigh, MaxThresData));
			LeftResultLow	= _mm_or_si128(_mm_cmplt_epi16(LeftResultLow, MinThresData), _mm_cmpgt_epi16(LeftResultLow, MaxThresData));

			// compare right
			RightResultHigh	= _mm_or_si128(_mm_cmplt_epi16(RightResultHigh, MinThresData), _mm_cmpgt_epi16(RightResultHigh, MaxThresData));
			RightResultLow	= _mm_or_si128(_mm_cmplt_epi16(RightResultLow, MinThresData), _mm_cmpgt_epi16(RightResultLow, MaxThresData));

			// left & right (pair)
			ResultHigh		= _mm_and_si128(RightResultHigh, LeftResultHigh);
			ResultLow		= _mm_and_si128(RightResultLow, LeftResultLow);

			// result save
			_mm_storeu_si128((__m128i*)(pResultX),		ResultLow);
			_mm_storeu_si128((__m128i*)(pResultX+8),	ResultHigh);

			pImageX		+= 16;
			pResultX	+= 16;
		}

		pImageY		+= nWidth;
		pResultY	+= nStep;
	}

	return ResultSuccess;
}

int CCHImageProcess::ImageFastMatching(CCHImageData* pSourceImage, CCHImageData* pTemplateImage, int& nResultX, int& nResultY, double& dResultValue)
{
	if (pSourceImage==NULL || pTemplateImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist() || !pTemplateImage->GetImageExist()) return ResultImageNoExist;

	if (pSourceImage->GetChannels()!=1 || pTemplateImage->GetChannels()!=1) return ResultImageNoExist;

	unsigned char* pSrcBuffer = (unsigned char*) pSourceImage->GetImageBuffer();
	int nSrcWidth = pSourceImage->GetWidth();
	int nSrcHeight = pSourceImage->GetHeight();

	unsigned char* pTmpBuffer = (unsigned char*) pTemplateImage->GetImageBuffer();
	int nTmpWidth = pTemplateImage->GetWidth();
	int nTmpHeight = pTemplateImage->GetHeight();

	
	int xx,yy;
	double NCC_value = Adaptive_WUS(pSrcBuffer, nSrcWidth, nSrcHeight, pTmpBuffer, nTmpWidth, nTmpHeight, xx, yy);

	nResultX = xx;
	nResultY = yy;
	dResultValue = NCC_value;

	return ResultSuccess;
}

int CCHImageProcess::ImageDCT(CCHImageData* pSourceImage, CCHImageData* pResultImage)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultFail;

	IplImage *source = cvCreateImage(cvGetSize(pSourceImage->m_pImageData), IPL_DEPTH_32F, 1);
	IplImage *freq = cvCreateImage(cvGetSize(pSourceImage->m_pImageData), IPL_DEPTH_32F, 1);

	//cvConvertScale(pSourceImage->m_pImageData, source, 1, 0);
	cvNormalize(pSourceImage->m_pImageData, source, 255, 0);
	cvShowImage("src1", source);	
	
	cvDCT(source, freq, CV_DXT_FORWARD);

	cvNormalize(freq, pResultImage->m_pImageData, 0, 255, CV_MINMAX);

	cvReleaseImage(&source);
	cvReleaseImage(&freq);

	return ResultSuccess;

}

int CCHImageProcess::ImageInvertDCT(CCHImageData* pSourceImage, CCHImageData* pResultImage)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultFail;

	cvDCT(pSourceImage->m_pImageData, pResultImage->m_pImageData, CV_DXT_INVERSE);

	return ResultSuccess;

}


int CCHImageProcess::ImageEqualizeHist(CCHImageData* pSourceImage, CCHImageData* pResultImage)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultFail;

	cvEqualizeHist(pSourceImage->m_pImageData, pResultImage->m_pImageData);

	return ResultSuccess;

}

int CCHImageProcess::ImagePreCornerDetect(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nApertureSize)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), IPL_DEPTH_8U, 1)) return ResultFail;

	IplImage *tmpImage = cvCreateImage(cvSize(pSourceImage->GetWidth(), pSourceImage->GetHeight()), IPL_DEPTH_32F, 1);

	cvPreCornerDetect(pSourceImage->m_pImageData, tmpImage, nApertureSize);

	cvConvertScaleAbs(tmpImage, pResultImage->m_pImageData, 255, 0);

	cvReleaseImage(&tmpImage);
	
	return ResultSuccess;

}



int CCHImageProcess::ImageCanny(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dThreshold1, double dThreshold2, int nApertureSize)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (pSourceImage->GetChannels()!=1) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), IPL_DEPTH_8U, 1)) return ResultFail;

	cvCanny(pSourceImage->m_pImageData, pResultImage->m_pImageData, dThreshold1, dThreshold2, nApertureSize);

	return ResultSuccess;
}

int CCHImageProcess::ImageAnd(CCHImageData* pSourceImage, CCHImageData* pMaskImage, CCHImageData* pResultImage)
{
	if (pSourceImage==NULL || pResultImage==NULL || pMaskImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), IPL_DEPTH_8U, 1)) return ResultFail;

	cvAnd(pSourceImage->m_pImageData, pResultImage->m_pImageData, pMaskImage->m_pImageData);

	return ResultSuccess;
}


int CCHImageProcess::ImageHoughLines2(CCHImageData* pSourceImage, int nMethod, double dRho, double dTheta, int nThreshold, double dParam1, double dParam2, VectorLineData& vectorLineData)
{
	if (pSourceImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (pSourceImage->GetChannels()!=1) return ResultImageNoExist;

	CvMemStorage* storage = cvCreateMemStorage(0);

	CvSeq* lines = 0;
	lines = cvHoughLines2( pSourceImage->m_pImageData, storage, nMethod, dRho, dTheta, nThreshold, dParam1, dParam2);

	for (int i=0; i<lines->total; i++)
	{
		CvPoint *line = (CvPoint*) cvGetSeqElem(lines, i);

		CLineData lineData(line[0].x, line[0].y, line[1].x, line[1].y);

		vectorLineData.push_back(lineData);
	}

	cvReleaseMemStorage(&storage);

	return ResultSuccess;
}


int CCHImageProcess::ImageHoughCircles(CCHImageData* pSourceImage, int nMethod, double dDp, double dMinDist, double dParam1, double dParam2, int nMinRadius, int nMaxRadius, VectorCircleData& vectorCircleData)
{
	if (pSourceImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (pSourceImage->GetChannels()!=1) return ResultImageNoExist;

	CvMemStorage* storage = cvCreateMemStorage(0);

	CvSeq *circles = cvHoughCircles(pSourceImage->m_pImageData, storage, nMethod, dDp, dMinDist, dParam1, dParam2, nMinRadius, nMaxRadius);

	CCircleData circleData;
	for (int i= 0; i < circles->total; i++)
	{
		float *p = (float *) cvGetSeqElem (circles, i);

		circleData.dCenterX	= p[0];
		circleData.dCenterY	= p[1];
		circleData.dRadius	= p[2];

		vectorCircleData.push_back(circleData);
	}

	cvReleaseMemStorage(&storage);

	return ResultSuccess;
}

int CCHImageProcess::ImageSobel(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nXOrder, int nYOrder, int nApertureSize)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), IPL_DEPTH_8U, 1)) return ResultFail;

	if (nXOrder==1 && nYOrder!=1)
	{
		IplImage *tempImage = cvCreateImage(cvSize(pSourceImage->GetWidth(), pSourceImage->GetHeight()), IPL_DEPTH_16S, 1);
		cvSobel(pSourceImage->m_pImageData, tempImage, 1, 0, nApertureSize);
		cvConvertScaleAbs(tempImage, pResultImage->m_pImageData, 1, 0);
		cvReleaseImage(&tempImage);
		return ResultSuccess;
	}

	if (nXOrder!=1 && nYOrder==1)
	{
		IplImage *tempImage = cvCreateImage(cvSize(pSourceImage->GetWidth(), pSourceImage->GetHeight()), IPL_DEPTH_16S, 1);
		cvSobel(pSourceImage->m_pImageData, tempImage, 0, 1, nApertureSize);
		cvConvertScaleAbs(tempImage, pResultImage->m_pImageData, 1, 0);
		cvReleaseImage(&tempImage);
		return ResultSuccess;
	}

	IplImage *xImage = cvCreateImage(cvSize(pSourceImage->GetWidth(), pSourceImage->GetHeight()), IPL_DEPTH_8U, 1);
	IplImage *yImage = cvCreateImage(cvSize(pSourceImage->GetWidth(), pSourceImage->GetHeight()), IPL_DEPTH_8U, 1);
	IplImage *tempImage = cvCreateImage(cvSize(pSourceImage->GetWidth(), pSourceImage->GetHeight()), IPL_DEPTH_16S, 1);

	cvSobel(pSourceImage->m_pImageData, tempImage, 1, 0, nApertureSize);
	cvConvertScaleAbs(tempImage, xImage, 0.5, 0);

	cvSobel(pSourceImage->m_pImageData, tempImage, 0, 1, nApertureSize);
	cvConvertScaleAbs(tempImage, yImage, 0.5, 0);
	
	cvAdd(xImage, yImage, pResultImage->m_pImageData);
	
	cvReleaseImage(&xImage);
	cvReleaseImage(&yImage);
	cvReleaseImage(&tempImage);
	
	return ResultSuccess;

}

int CCHImageProcess::ImageSobel(CCHImageData* pSourceImage, CCHImageData* pXImage, CCHImageData* pYImage, int nApertureSize)
{
	if (pSourceImage==NULL || pXImage==NULL || pYImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pXImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), IPL_DEPTH_8U, 1)) return ResultFail;
	if (!pYImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), IPL_DEPTH_8U, 1)) return ResultFail;

	IplImage *tmpImage1 = cvCreateImage(cvSize(pSourceImage->GetWidth(), pSourceImage->GetHeight()), IPL_DEPTH_16S, 1);
	IplImage *tmpImage2 = cvCreateImage(cvSize(pSourceImage->GetWidth(), pSourceImage->GetHeight()), IPL_DEPTH_16S, 1);

	cvSobel(pSourceImage->m_pImageData, tmpImage2, 1, 0, nApertureSize);
	cvConvertScaleAbs(tmpImage2, pXImage->m_pImageData, 1, 0);

	cvSobel(pSourceImage->m_pImageData, tmpImage1, 0, 1, nApertureSize);
	cvConvertScaleAbs(tmpImage1, pYImage->m_pImageData, 1, 0);

	cvReleaseImage(&tmpImage1);
	cvReleaseImage(&tmpImage2);

	return ResultSuccess;

}


int CCHImageProcess::ImageAdaptiveThreshold(CCHImageData* pSourceImage, CCHImageData* pResultImage, double dMaxValue, int nMethodType, int nThresholdType, int nBlockSize, double dParam1)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), 8, 1)) return ResultFail;

	cvAdaptiveThreshold(pSourceImage->m_pImageData, pResultImage->m_pImageData, dMaxValue, nMethodType, nThresholdType, nBlockSize, dParam1);

	return ResultSuccess;
}

int GS_optimal_power(int num)
{
	int result = 0;
	int tmp_num = num;

	while(tmp_num >= 2)
	{
		tmp_num = tmp_num >> 1;
		result++;
	}

	int powers = (int) pow(2.0, result);

	return (num != powers) ? (powers<<1): num;
}

int GS_check_power(int num)
{
	int optimal_num = GS_optimal_power( num );

	return (optimal_num == num) ? 0: -1;
}

int GS_check_log2N(int width, int height)
{
	int width_log2N = GS_check_power( height );
	int height_log2N = GS_check_power( height );

	if (height_log2N == -1) return -1;

	if (width_log2N == -1) return -1;

	return 0;
}


IplImage *GS_forward_FFT2D(IplImage *src_image)
{
	if (src_image==NULL) return NULL;

	int width	= src_image->width;
	int height	= src_image->height;

	if (src_image->nChannels!=1) return NULL;

	int checked = GS_check_log2N(width, height);

	if (checked==-1) return NULL;

	IplImage *spatial = cvCreateImage(cvGetSize(src_image), IPL_DEPTH_32F, 2);
	IplImage *real = cvCreateImage(cvGetSize(src_image), IPL_DEPTH_32F, 1);
	IplImage *imag = cvCreateImage(cvGetSize(src_image), IPL_DEPTH_32F, 1);

	IplImage *freq = cvCreateImage(cvGetSize(src_image), IPL_DEPTH_32F, 2);

	cvConvertScale(src_image, real, 1, 0);
	cvSetZero(imag);

	cvCvtPlaneToPix(real, imag, NULL, NULL, spatial);

	cvFFT(spatial, freq, CV_DXT_FORWARD);

	cvReleaseImage(&spatial);
	cvReleaseImage(&real);
	cvReleaseImage(&imag);

	return freq;
}

double GS_log(double x, double y)
{
	return log(1 + sqrt(x*x + y*y));
}

IplImage *GS_create_FFT_image(IplImage *freq, int method)
{
	int i, j;
	double tmp;

	if (freq==NULL) return NULL;
	if (freq->nChannels!=2) return NULL;

	int width = freq->width;
	int height = freq->height;

	int checked = GS_check_log2N(width, height);
	if (checked==-1) return NULL;

	IplImage *real = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);
	IplImage *imag = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);
	cvCvtPixToPlane(freq, real, imag, NULL, NULL);

	IplImage *norm = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);
	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++)
		{
			tmp = GS_log( cvGetReal2D(real, i, j), cvGetReal2D(imag, i, j));

			cvSetReal2D(norm, i, j, tmp);
		}
	}

	IplImage *test_normal = cvCreateImage(cvGetSize(freq), IPL_DEPTH_8U, 1);
	IplImage *test_save = cvCreateImage(cvGetSize(freq), IPL_DEPTH_8U, 1);

	double min_val, max_val;

	cvMinMaxLoc(real, &min_val, &max_val);
	cvNormalize(real, test_normal, 0.0, 255.0, CV_MINMAX);
	cvCvtScaleAbs(test_normal, test_save);
	float *pValue = (float*) real->imageData;

	cvMinMaxLoc(imag, &min_val, &max_val);
	cvNormalize(imag, test_normal, 0.0, 255.0, CV_MINMAX);
	cvCvtScaleAbs(test_normal, test_save);
	pValue = (float*) real->imageData;

	IplImage *dst_freq_image = cvCreateImage(cvGetSize(freq), IPL_DEPTH_8U, 1);
	IplImage *temp = cvCreateImage(cvGetSize(freq), IPL_DEPTH_8U, 1);

	cvNormalize(norm, temp, 0.0, 255.0, CV_MINMAX);

	cvCvtScaleAbs(temp, dst_freq_image);

	cvReleaseImage(&temp);

	if (method==0) return dst_freq_image;

	IplImage *dst_freq_center_image = cvCreateImage(cvGetSize(freq), IPL_DEPTH_8U, 1);

	for (i=0; i<height/2; i++)
	{
		for (j=0; j<width/2; j++)
		{
			// 1 => 4
			cvSetReal2D(dst_freq_center_image, 
				i+height/2 - 1,
				j+width/2 - 1,
				cvGetReal2D(dst_freq_image, i, j));

			// 2 => 3
			cvSetReal2D(dst_freq_center_image, 
				i+height/2 - 1,
				j,
				cvGetReal2D(dst_freq_image, i, j+width/2 -1));

			// 3 => 2
			cvSetReal2D(dst_freq_center_image, 
				i,
				j+width/2 - 1,
				cvGetReal2D(dst_freq_image, i+height/2-1, j));

			// 4 => 1
			cvSetReal2D(dst_freq_center_image, 
				i,
				j,
				cvGetReal2D(dst_freq_image, i+height/2-1, j+width/2-1));
		}
	}

	cvReleaseImage(&dst_freq_image);
	cvReleaseImage(&norm);
	cvReleaseImage(&real);
	cvReleaseImage(&imag);

	return dst_freq_center_image;

}

IplImage *GS_pre_zero_padding(IplImage *src_image)
{
	int width = src_image->width;
	int height = src_image->height;

	if (src_image->nChannels!=1) return NULL;

	int pz_width = GS_optimal_power(width);
	int pz_height = GS_optimal_power(height);

	if (pz_height < pz_width) 
		pz_height = pz_width;
	else
		pz_width = pz_height;

	CvSize pz_size = cvSize(pz_width, pz_height);
	IplImage *pz_src_image = cvCreateImage(pz_size, IPL_DEPTH_8U, 1);
	cvZero(pz_src_image);

	cvSetImageROI(pz_src_image, cvRect(0, 0, width, height));
	cvCopy(src_image, pz_src_image);
	cvResetImageROI(pz_src_image);

	return pz_src_image;
}

IplImage *GS_post_zero_padding(IplImage *pz_src_image, int width, int height)
{
	CvSize size = cvSize(width, height);
	IplImage *dst_image = cvCreateImage(size, IPL_DEPTH_8U, 1);

	cvSetImageROI(pz_src_image, cvRect(0, 0, width, height));
	cvCopy(pz_src_image, dst_image);
	cvResetImageROI(pz_src_image);

	return dst_image;
}


int CCHImageProcess::ImageFFT(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nFlags, CCHImageData* pNormalImage)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	pResultImage->m_pImageData = GS_forward_FFT2D(GS_pre_zero_padding(pSourceImage->m_pImageData));

	if (pResultImage->m_pImageData==NULL) return ResultImageNoExist;

	if (pNormalImage==NULL) return ResultSuccess;

	pNormalImage->m_pImageData = GS_create_FFT_image(pResultImage->m_pImageData, nFlags);

	if (pNormalImage->m_pImageData==NULL) return ResultImageNoExist;

	return ResultSuccess;
}

IplImage *GS_inverse_FFT2D(IplImage *freq)
{
	if (freq==NULL) return NULL;

	if (freq->nChannels!=2) return NULL;

	int width = freq->width;
	int height = freq->height;

	int checked = GS_check_log2N(width, height);

	if (checked==-1) return NULL;

	IplImage *spatial = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 2);
	IplImage *real = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);
	IplImage *imag = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);

	IplImage *dst_image = cvCreateImage(cvGetSize(freq), IPL_DEPTH_8U, 1);

	cvFFT(freq, spatial, CV_DXT_INVERSE_SCALE);

	cvCvtPixToPlane(spatial, real, imag, NULL, NULL);
	cvConvertScale(real, dst_image, 1, 0);

	cvReleaseImage(&spatial);
	cvReleaseImage(&real);
	cvReleaseImage(&imag);

	return dst_image;
}

int CCHImageProcess::ImageInverseFFT(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nWidth, int nHeight)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	pResultImage->m_pImageData = GS_post_zero_padding(GS_inverse_FFT2D(pSourceImage->m_pImageData), nWidth, nHeight);

	if (pResultImage->m_pImageData==NULL) return ResultImageNoExist;

	return ResultSuccess;
}

IplImage *BW_Lowpass_Filter(IplImage *freq, int D0, int N)
{
	int i, j;
	int temp_h, temp_w;
	double sqrt_value, h_2powers, w_2powers, norm;
	double var, real_var, imag_var;

	if (freq==NULL) return NULL;

	if (freq->nChannels!=2) return NULL;

	IplImage *lowpass_real = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);
	IplImage *lowpass_imag = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);

	IplImage *freq_real = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);
	IplImage *freq_imag = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);

	cvCvtPixToPlane(freq, freq_real, freq_imag, NULL, NULL);

	int width = freq->width;
	int height = freq->height;
	int half_width = width/2;
	int half_height = height/2;

	for (i=0; i<height; i++)
	{

		if (i>=half_height) temp_h = i - half_height;
		else temp_h = i + half_height;

		for (j=0; j<width; j++)
		{
			if (j>=half_width) temp_w = j - half_width;
			else temp_w = j + half_width;

			w_2powers = (temp_w-half_width) * (temp_w - half_width);
			h_2powers = (temp_h-half_height) * (temp_h - half_height);

			norm = (double) (h_2powers + w_2powers);
			sqrt_value = sqrt(norm);

			var = 1.0 / (1.0 + pow((sqrt_value/(double)D0), 2*N));

			real_var = cvGetReal2D(freq_real, i, j) * var;
			imag_var = cvGetReal2D(freq_imag, i, j) * var;

			cvSetReal2D(lowpass_real, i, j, real_var);
			cvSetReal2D(lowpass_imag, i, j, imag_var);

		}
	}

	IplImage *lowpass_freq = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 2);
	cvCvtPlaneToPix(lowpass_real, lowpass_imag, NULL, NULL, lowpass_freq);

	cvReleaseImage(&freq_real);
	cvReleaseImage(&freq_imag);
	cvReleaseImage(&lowpass_real);
	cvReleaseImage(&lowpass_imag);

	return lowpass_freq;
}

IplImage *BW_Highpass_Filter(IplImage *freq, int D0, int N)
{
	int i, j;
	int temp_h, temp_w;
	double sqrt_value, h_2powers, w_2powers, norm;
	double var, real_var, imag_var;

	if (freq==NULL) return NULL;

	if (freq->nChannels!=2) return NULL;

	IplImage *highpass_real = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);
	IplImage *highpass_imag = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);

	IplImage *freq_real = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);
	IplImage *freq_imag = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 1);

	cvCvtPixToPlane(freq, freq_real, freq_imag, NULL, NULL);

	int width = freq->width;
	int height = freq->height;
	int half_width = width/2;
	int half_height = height/2;

	for (i=0; i<height; i++)
	{

		if (i>=half_height) temp_h = i - half_height;
		else temp_h = i + half_height;

		for (j=0; j<width; j++)
		{
			if (j>=half_width) temp_w = j - half_width;
			else temp_w = j + half_width;

			w_2powers = (temp_w-half_width) * (temp_w - half_width);
			h_2powers = (temp_h-half_height) * (temp_h - half_height);

			norm = (double) (h_2powers + w_2powers);
			sqrt_value = sqrt(norm);

			var = 1.0 / (1.0 + pow(((double)D0/sqrt_value), 2*N));

			real_var = cvGetReal2D(freq_real, i, j) * var;
			imag_var = cvGetReal2D(freq_imag, i, j) * var;

			cvSetReal2D(highpass_real, i, j, real_var);
			cvSetReal2D(highpass_imag, i, j, imag_var);

		}
	}

	IplImage *highpass_freq = cvCreateImage(cvGetSize(freq), IPL_DEPTH_32F, 2);
	cvCvtPlaneToPix(highpass_real, highpass_imag, NULL, NULL, highpass_freq);

	cvReleaseImage(&freq_real);
	cvReleaseImage(&freq_imag);
	cvReleaseImage(&highpass_real);
	cvReleaseImage(&highpass_imag);

	return highpass_freq;
}



int CCHImageProcess::ImageHighPassFilter(CCHImageData* pSourceImage, CCHImageData* pResultImage, int D0, int N, int nFlags, CCHImageData *pNormalImage)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	pResultImage->m_pImageData = GS_forward_FFT2D(GS_pre_zero_padding(pSourceImage->m_pImageData));

	if (pResultImage->m_pImageData==NULL) return ResultImageNoExist;

	IplImage* high_pass = BW_Highpass_Filter(pResultImage->m_pImageData, D0, N);

	if (high_pass==NULL) return ResultImageNoExist;

	if (pNormalImage==NULL) 
	{
		cvReleaseImage(&high_pass);
		return ResultSuccess;
	}

	pNormalImage->m_pImageData = GS_create_FFT_image(high_pass, nFlags);

	if (pNormalImage->m_pImageData==NULL) return ResultImageNoExist;

	if (pNormalImage->m_pImageData==NULL) 
	{
		cvReleaseImage(&high_pass);
		return ResultImageNoExist;
	}

	cvReleaseImage(&high_pass);
	return ResultSuccess;
}

int CCHImageProcess::ImageLowPassFilter(CCHImageData* pSourceImage, CCHImageData* pResultImage, int D0, int N, int nFlags, CCHImageData *pNormalImage)
{
	if (pSourceImage==NULL || pResultImage==NULL) return ResultImageNULL;

	if (!pSourceImage->GetImageExist()) return ResultImageNoExist;

	pResultImage->m_pImageData = GS_forward_FFT2D(GS_pre_zero_padding(pSourceImage->m_pImageData));

	if (pResultImage->m_pImageData==NULL) return ResultImageNoExist;

	IplImage* low_pass = BW_Lowpass_Filter(pResultImage->m_pImageData, D0, N);

	if (low_pass==NULL) return ResultImageNoExist;

	if (pNormalImage==NULL) 
	{
		cvReleaseImage(&low_pass);
		return ResultSuccess;
	}

	pNormalImage->m_pImageData = GS_post_zero_padding(GS_inverse_FFT2D(low_pass), low_pass->width, low_pass->height);

	if (pNormalImage->m_pImageData==NULL) return ResultImageNoExist;

	if (pNormalImage->m_pImageData==NULL) 
	{
		cvReleaseImage(&low_pass);
		return ResultImageNoExist;
}

	cvReleaseImage(&low_pass);
	return ResultSuccess;
}

BOOL CCHImageProcess::CalculateLagrange(VectorDouble& vectorX, VectorDouble& vectorY, ListPolynomial& listPolynomial)
{
	if (vectorX.size()<2) return FALSE;

	if (vectorX.size()!=vectorY.size()) return FALSE;

	int n = (int)vectorX.size();

	int i, j;

	int sign;

	double coef, temp;

	for (i=0; i<n; i++)
	{
		if (!vectorY[i]) continue;

		coef = vectorY[i];

		for (j=0; j<n; j++)
		{
			if (i==j) continue;

			if (vectorX[i]==vectorX[j])
			{
				RemovePolynomial(listPolynomial);
				return FALSE;
			}

			coef /= vectorX[i] - vectorX[j];

		}

		temp = vectorX[i];
		vectorX[i] = 0;


		for (j=0; j<n; j++)
		{
			if (j%2)
			{
				sign = -1;
			}
			else
			{
				sign = 1;
			}

			InsertPolynomial(listPolynomial, int(n-j-1), sign*coef*Combi(vectorX, vectorY, n, j));
		}

		vectorX[i] = temp;
	}

	return TRUE;

}

double CCHImageProcess::Combi(VectorDouble& vectorX, VectorDouble& vectorY, int n, int r)
{
	int i;
	double result = 1;

	if (n<0 || r<0 || n<r) return 0.0;

	if (n==r)
	{
		for(i=0; i<n; i++)
		{
			result *= vectorX[i];
		}
		return result;
	}

	if (r!=0) 
	{
		return vectorX[n-1]*Combi(vectorX, vectorY, n-1, r-1)+Combi(vectorX, vectorY, n-1, r);
	}

	return 1;
}

void CCHImageProcess::RemovePolynomial(ListPolynomial& listPolynomial)
{
	for(ListPolynomialIt it=listPolynomial.begin(); it!= listPolynomial.end();)
	{
		CPolynomial *pNode = *it;
		delete pNode;
		it = listPolynomial.erase(it);
	}
	listPolynomial.clear();	
}

void CCHImageProcess::InsertPolynomial(ListPolynomial &listPolynomial, int degree, double coef)
{
	if (coef==0.0) return;

	CPolynomial *pNewPoly = new CPolynomial(degree, coef);

	BOOL bAdded = FALSE;

	for(ListPolynomialIt it=listPolynomial.begin(); it!= listPolynomial.end(); it++)
	{
		CPolynomial *pCurrent = *it;

		if (pCurrent->nDegree == pNewPoly->nDegree)
		{
			// 현재 노드와 차수 같으면 현재 노드에 계수를 더하고, 생성노드는 삭제한다.
			pCurrent->dCoef += pNewPoly->dCoef;
			delete pNewPoly;
			bAdded = TRUE;
			break;
		}
		else if (pCurrent->nDegree < pNewPoly->nDegree)
		{
			// 현재 앞에 삽입한다.
			listPolynomial.push_front(pNewPoly);
			bAdded = TRUE;
			break;
		}
	}

	// 앞에서 추가가 안되었으면, 맨뒤에 삽입한다.
	if (!bAdded)
	{
		listPolynomial.push_back(pNewPoly);
		return;
	}

}

int CCHImageProcess::MatrixSolve(double *A, double* B, double* X, int nSize, int nMethod)
{
	CvMat a = cvMat( nSize, nSize, CV_64FC1, A );
	CvMat b = cvMat( nSize, 1, CV_64FC1, B );
	CvMat x = cvMat( nSize, 1, CV_64FC1, X );

	int nValue = cvSolve(&a, &b, &x, nMethod);

	return nValue;
}


int CCHImageProcess::ImageFindLineInfo(CCHImageData *pSource, float rho, float theta, int threshold, int lineLength, int lineGap, VectorLineData& vectorLine, int linesMax)
{
	if (pSource==NULL) return ResultImageNULL;
	if (!pSource->GetImageExist()) return ResultImageNoExist;
	if (pSource->GetChannels()!=1) return ResultImageNoExist;

	int nWidth = pSource->GetWidth();
	int nHeight = pSource->GetHeight();
	int nWidthStep = pSource->GetWidthStep();

	// point list
	CvSeq* seq;
	CvSeqWriter writer;
	cv::MemStorage storage(cvCreateMemStorage(0));

	float ang;
	int r, n, count;
	float irho = 1 / rho;
	CvRNG rng = cvRNG(-1);

	int numangle = cvRound(CV_PI / theta);
	int numrho = cvRound(((nWidth + nHeight) * 2 + 1) / rho);

	// alloc memory
	BYTE *pMask = new BYTE[nWidth*nHeight];
	float *pAccum = new float[numangle*numrho];
	memset(pAccum, 0, sizeof(float)*numangle*numrho);
	float *pTrigtab = new float[numangle*2];

	// step 0. calculate angle lookup table
	for(ang=0, n=0; n<numangle; ang+=theta, n++)
	{
		pTrigtab[n*2] = (float)(cos(ang) * irho);
		pTrigtab[n*2+1] = (float)(sin(ang) * irho);
	}
	const float* ttab = &pTrigtab[0];

	// step 1. collect non-zero image points
	BYTE *pSourceY = (BYTE*) pSource->GetImageBuffer();
	BYTE *pMaskY = pMask;
	BYTE *pSourceX, *pMaskX;

	cvStartWriteSeq( CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), storage, &writer );

	CvPoint pt;
	for(pt.y=0, count=0; pt.y<nHeight; pt.y++)
	{
		pSourceX = pSourceY;
		pMaskX = pMaskY;
		for(pt.x=0; pt.x<nWidth; pt.x++)
		{
			if(*pSourceX++)
			{
				*pMaskX = 1;
				CV_WRITE_SEQ_ELEM( pt, writer );
			}
			else
			{
				*pMaskX = 0;
			}
			pMaskX++;
		}
		pSourceY += nWidthStep;
		pMaskY += nWidth;
	}

	seq = cvEndWriteSeq( &writer );
	count = seq->total;

	// step 2. process all the points in random order
	for( ; count>0; count--)
	{
		// choose random point out of the remaining ones
		int idx = cvRandInt(&rng) % count;
		float max_val = float(threshold-1);
		int max_n = 0;
		CvPoint* pt = (CvPoint*)cvGetSeqElem(seq, idx);
		CvPoint line_end[2] = {{0,0}, {0,0}};

		float a, b;
		int i, j, k, x0, y0, dx0, dy0, xflag;
		int good_line;
		const int shift = 16;
		float* adata = pAccum;

		// save pos
		i = pt->y;
		j = pt->x;

		// "remove" it by overriding it with the last element (save last element value to current element)
		*pt = *(CvPoint*)cvGetSeqElem( seq, count-1 );

		// check if it has been excluded already (i.e. belongs to some other line)
		if(!pMask[i*nWidth + j])
		{
			continue;
		}

		// update accumulator, find the most probable line
		for(n=0; n<numangle; n++, adata+=numrho)
		{
			r = cvRound( j * ttab[n*2] + i * ttab[n*2+1] );
			r += (numrho - 1) / 2;
			float val = ++adata[r];
			if(max_val<val)
			{
				max_val = val;
				max_n = n;
			}
		}

		// if it is too "weak" candidate, continue with another point
		if(max_val<threshold)
		{
			continue;
		}

		// from the current point walk in each direction
		// along the found line and extract the line segment
		a = -ttab[max_n*2+1];
		b = ttab[max_n*2];
		x0 = j;
		y0 = i;
		if(fabs(a)>fabs(b))
		{
			xflag = 1;
			dx0 = a > 0 ? 1 : -1;
			dy0 = cvRound( b*(1 << shift)/fabs(a) );
			y0 = (y0 << shift) + (1 << (shift-1));
		}
		else
		{
			xflag = 0;
			dy0 = b > 0 ? 1 : -1;
			dx0 = cvRound( a*(1 << shift)/fabs(b) );
			x0 = (x0 << shift) + (1 << (shift-1));
		}

		for(k=0; k<2; k++)
		{
			int gap = 0, x = x0, y = y0, dx = dx0, dy = dy0;

			if(k>0)
			{
				dx = -dx, dy = -dy;
			}

			// walk along the line using fixed-point arithmetics,
			// stop at the image border or in case of too big gap
			for( ;; x+=dx, y+=dy)
			{
				BYTE* pCurMask;
				int i1, j1;

				if(xflag)
				{
					j1 = x;
					i1 = y >> shift;
				}
				else
				{
					j1 = x >> shift;
					i1 = y;
				}

				if(j1<0 || j1>=nWidth || i1<0 || i1>=nHeight)
				{
					break;
				}

				pCurMask = pMask + i1*nWidth + j1;

				// for each non-zero point:
				//    update line end,
				//    clear the mask element
				//    reset the gap
				if(*pCurMask)
				{
					gap = 0;
					line_end[k].y = i1;
					line_end[k].x = j1;
				}
				else if(++gap>lineGap)
				{
					break;
				}
			}
		}

		good_line = abs(line_end[1].x - line_end[0].x) >= lineLength || abs(line_end[1].y - line_end[0].y) >= lineLength;

		for(k=0; k<2; k++)
		{
			int x = x0, y = y0, dx = dx0, dy = dy0;

			if( k > 0 )
			{
				dx = -dx, dy = -dy;
			}

			// walk along the line using fixed-point arithmetics,
			// stop at the image border or in case of too big gap
			for( ;; x+=dx, y+=dy)
			{
				BYTE* pCurMask;
				int i1, j1;

				if(xflag)
				{
					j1 = x;
					i1 = y >> shift;
				}
				else
				{
					j1 = x >> shift;
					i1 = y;
				}

				pCurMask = pMask + i1*nWidth + j1;

				// for each non-zero point:
				//    update line end,
				//    clear the mask element
				//    reset the gap
				if(*pCurMask)
				{
					if(good_line)
					{
						adata = pAccum;
						for( n = 0; n < numangle; n++, adata += numrho )
						{
							r = cvRound( j1 * ttab[n*2] + i1 * ttab[n*2+1] );
							r += (numrho - 1) / 2;
							adata[r]--;
						}
					}
					*pCurMask = 0;
				}

				if(i1 == line_end[k].y && j1 == line_end[k].x)
				{
					break;
				}
			}
		}

		if(good_line)
		{
			vectorLine.push_back(CLineData(line_end[0].x, line_end[0].y, line_end[1].x, line_end[1].y));
			if(int(vectorLine.size())>=linesMax)
			{
				break;
			}
		}
	} // end for 

	delete [] pMask;
	delete [] pAccum;
	delete [] pTrigtab;

	return ResultSuccess;

}

void MultipleMaxLoc( const IplImage& image, CvPoint** locations, int numMaxima)
{
	// initialize input variable locations
	*locations = new CvPoint[numMaxima];

	// create array for tracking maxima
	double *maxima = new double[numMaxima];
	memset(maxima, 0, sizeof(double)*numMaxima);

	// extract the raw data for analysis
	float* data;
	int step;
	CvSize size;

	cvGetRawData( &image, ( uchar** )&data, &step, &size );

	step /= sizeof( data[0] );

	for( int y = 0; y < size.height; y++, data += step )
	{
		for( int x = 0; x < size.width; x++ )
		{
			// insert the data value into the array if it is greater than any of the
			//  other array values, and bump the other values below it, down
			for( int j = 0; j < numMaxima; j++ )
			{
				if( data[x] > maxima[j] )
				{
					// move the maxima down
					for( int k = numMaxima - 1; k > j; k-- )
					{
						maxima[k] = maxima[k-1];
						( *locations )[k] = ( *locations )[k-1];
					}

					// insert the value
					maxima[j] = ( double )data[x];
					( *locations )[j].x = x;
					( *locations )[j].y = y;
					break;
				}
			}
		}
	}

	delete [] maxima;
}

/*
int CCHImageProcess::FastMatchTemplate(const IplImage& source, const IplImage&  target, std::vector<CvPoint>* foundPointsList, std::vector<double>*  confidencesList, int matchPercentage, BOOL findMultipleTargets, int numMaxima, int numDownPyrs, int searchExpansion)
{
	// make sure that the template image is smaller than the source
	if( target.width > source.width || target.height > source.height )
	{
		return 0;
	}

	if( source.depth != target.depth )
	{
		return 0;
	}

	if( source.nChannels != target.nChannels )
	{
		return 0;
	}

	CvSize sourceSize = cvGetSize( &source );
	CvSize targetSize = cvGetSize( &target );

	int depth = source.depth;
	int numChannels = source.nChannels;

	// create copies of the images to modify
	IplImage* copyOfSource = cvCloneImage( &source );
	IplImage* copyOfTarget = cvCloneImage( &target );

	// down pyramid the images
	for( int ii = 0; ii < numDownPyrs; ii++ )
	{
		// start with the source image
		sourceSize.width /= 2;
		sourceSize.height /= 2;

		IplImage* smallSource = NULL;
		smallSource = cvCreateImage( sourceSize, depth, numChannels );
		cvPyrDown( copyOfSource, smallSource, CV_GAUSSIAN_5x5 );

		// prepare for next loop, if any
		cvReleaseImage( &copyOfSource );
		copyOfSource = cvCloneImage( smallSource );
		cvReleaseImage( &smallSource );

		// next, do the target    
		targetSize.width /= 2;
		targetSize.height /= 2;

		IplImage* smallTarget = NULL;
		smallTarget = cvCreateImage( targetSize, depth, numChannels );
		cvPyrDown( copyOfTarget, smallTarget, CV_GAUSSIAN_5x5 );

		// prepare for next loop, if any
		cvReleaseImage( &copyOfTarget );
		copyOfTarget = cvCloneImage( smallTarget );
		cvReleaseImage( &smallTarget );
	}

	// perform the match on the shrunken images
	CvSize smallTargetSize = cvGetSize( copyOfTarget );
	CvSize smallSourceSize = cvGetSize( copyOfSource );

	CvSize resultSize;
	resultSize.width = smallSourceSize.width - smallTargetSize.width + 1;
	resultSize.height = smallSourceSize.height - smallTargetSize.height + 1;

	IplImage* result = cvCreateImage(resultSize, IPL_DEPTH_32F, 1);

	cvMatchTemplate( copyOfSource, copyOfTarget, result, CV_TM_CCOEFF_NORMED);

	// release memory we don't need anymore
	cvReleaseImage( &copyOfSource );
	cvReleaseImage( &copyOfTarget );

	// find the top match locations
	CvPoint* locations = NULL;
	MultipleMaxLoc(*result, &locations, numMaxima);

	cvReleaseImage(&result);

	// search the large images at the returned locations
	sourceSize = cvGetSize( &source );
	targetSize = cvGetSize( &target );

	// create a copy of the source in order to adjust its ROI for searching
	IplImage* searchImage = cvCloneImage( &source );
	for( int currMax = 0; currMax < numMaxima; currMax++ )
	{ 
		// transform the point to its corresponding point in the larger image
		locations[currMax].x *= ( int )pow( double(2), double(numDownPyrs) );
		locations[currMax].y *= ( int )pow( double(2), double(numDownPyrs) );
		locations[currMax].x += targetSize.width / 2;
		locations[currMax].y += targetSize.height / 2;

		const CvPoint& searchPoint = locations[currMax];

		// if we are searching for multiple targets and we have found a target or 
		//  multiple targets, we don't want to search in the same location(s) again
		if( findMultipleTargets && !foundPointsList->empty() )
		{
			BOOL thisTargetFound = FALSE;

			int numPoints = (int)foundPointsList->size();
			for( int currPoint = 0; currPoint < numPoints; currPoint++ )
			{
				const CvPoint& foundPoint = ( *foundPointsList )[currPoint];
				if( abs( searchPoint.x - foundPoint.x ) <= searchExpansion * 2 && 
					abs( searchPoint.y - foundPoint.y ) <= searchExpansion * 2 )
				{
					thisTargetFound = TRUE;
					break;
				}
			}

			// if the current target has been found, continue onto the next point
			if( thisTargetFound )
			{
				continue;
			}
		}

		// set the source image's ROI to slightly larger than the target image, 
		//  centred at the current point
		CvRect searchRoi;
		searchRoi.x = searchPoint.x - ( target.width ) / 2 - searchExpansion;
		searchRoi.y = searchPoint.y - ( target.height ) / 2 - searchExpansion;
		searchRoi.width = target.width + searchExpansion * 2;
		searchRoi.height = target.height + searchExpansion * 2;

		// make sure ROI doesn't extend outside of image
		if( searchRoi.x < 0 )
		{
			searchRoi.x = 0;
		}
		if( searchRoi.y < 0 )
		{
			searchRoi.y = 0;
		}
		if( ( searchRoi.x + searchRoi.width ) > ( sourceSize.width - 1 ) )
		{
			int numPixelsOver 
				= ( searchRoi.x + searchRoi.width ) - ( sourceSize.width - 1 );

			searchRoi.width -= numPixelsOver;
		}
		if( ( searchRoi.y + searchRoi.height ) > ( sourceSize.height - 1 ) )
		{
			int numPixelsOver 
				= ( searchRoi.y + searchRoi.height ) - ( sourceSize.height - 1 );

			searchRoi.height -= numPixelsOver;
		}

		cvSetImageROI( searchImage, searchRoi );

		// perform the search on the large images
		resultSize.width = searchRoi.width - target.width + 1;
		resultSize.height = searchRoi.height - target.height + 1;

		result = cvCreateImage( resultSize, IPL_DEPTH_32F, 1 );

		cvMatchTemplate( searchImage, &target, result, CV_TM_CCOEFF_NORMED );
		cvResetImageROI( searchImage );

		// find the best match location
		double minValue, maxValue;
		CvPoint minLoc, maxLoc;
		cvMinMaxLoc( result, &minValue, &maxValue, &minLoc, &maxLoc );
		maxValue *= 100;

		// transform point back to original image
		maxLoc.x += searchRoi.x + target.width / 2;
		maxLoc.y += searchRoi.y + target.height / 2;

		cvReleaseImage( &result );

		if( maxValue >= matchPercentage )
		{
			// add the point to the list
			foundPointsList->push_back( maxLoc );
			confidencesList->push_back( maxValue );

			// if we are only looking for a single target, we have found it, so we
			//  can return
			if( !findMultipleTargets )
			{
				break;
			}
		}
	}

	if( foundPointsList->empty() )
	{
		printf( "\nTarget was not found to required confidence of %d.\n", matchPercentage);
	}

	delete [] locations;

	cvReleaseImage( &searchImage );

	return ResultSuccess;
}

*/
BOOL CCHImageProcess::ImageSmooth_HybridMedianFilter(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nFilterSize)
{
	if (pSourceImage==NULL || pResultImage==NULL) return FALSE;

	if (!pSourceImage->GetImageExist()) return FALSE;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultImageNoExist;
	pResultImage->ClearImage(0);
	pSourceImage->CopyImageTo(pResultImage);

	int *pMedianArray, *pTempArray;
	int nTempArrayIdx, nMedianArrayIdx;
	int x, y, nWidth, nHeight, nWidthStep, nMiddleOfFilter, nFilterRowIdx, nFilterColIdx, nValue, nMiddleIdx, nRow;

	pTempArray = new int[nFilterSize * 2];
	pMedianArray = new int[3];
	
	nWidth = pSourceImage->GetWidth();
	nHeight = pSourceImage->GetHeight();
	nWidthStep = pSourceImage->GetWidthStep();
	nMiddleOfFilter = nFilterSize / 2;

	BYTE* pSrc = (BYTE*) pSourceImage->GetImageBuffer();
	BYTE* pDst = (BYTE*) pResultImage->GetImageBuffer();
	BYTE* pTempLine = NULL;
	BYTE** pFilterLine = NULL;

	pFilterLine = new BYTE*[nFilterSize];

	for(nFilterColIdx = 0; nFilterColIdx < nFilterSize; nFilterColIdx++)
	{
		if(nFilterColIdx == 0)
		{
			pFilterLine[nFilterColIdx] = &pSrc[0];
		}
		else
		{
			pFilterLine[nFilterColIdx] = pFilterLine[nFilterColIdx - 1] + nWidthStep;
		}
	}
	
	for(y = nMiddleOfFilter; y < nHeight - nMiddleOfFilter; y++)
	{
		for(x = nMiddleOfFilter; x < nWidth - nMiddleOfFilter; x++)
		{
			//1. X방향
			nRow = x - nMiddleOfFilter;
			nTempArrayIdx = 0;
			nMedianArrayIdx = 0;
			ZeroMemory(pTempArray, sizeof(int) * nFilterSize * 2);
			ZeroMemory(pMedianArray, sizeof(int) * 3);

			for(nFilterColIdx = 0; nFilterColIdx < nFilterSize; nFilterColIdx++)
			{
				nValue = pFilterLine[nFilterColIdx][nRow + nFilterColIdx];
				pTempArray[nTempArrayIdx++] = nValue;

				nValue = pFilterLine[nFilterColIdx][nRow + nFilterSize - 1 - nFilterColIdx];
				pTempArray[nTempArrayIdx++] = nValue;
			}

			QuickSort(pTempArray, 0, nTempArrayIdx - 1);
			nMiddleIdx = nTempArrayIdx / 2;
			nValue = pTempArray[nMiddleIdx];
			pMedianArray[nMedianArrayIdx++] = nValue;

			//2. ＋방향
			nTempArrayIdx = 0;
			ZeroMemory(pTempArray, sizeof(int) * nFilterSize * 2);

			for(nFilterColIdx = 0; nFilterColIdx < nFilterSize; nFilterColIdx++)
			{
				if(nFilterColIdx == nMiddleOfFilter)
				{
					for(nFilterRowIdx = 0; nFilterRowIdx < nFilterSize; nFilterRowIdx++)
					{
						nValue = pFilterLine[nFilterColIdx][nRow + nFilterRowIdx];
						pTempArray[nTempArrayIdx++] = nValue;
					}
				}
				else
				{
					nValue = pFilterLine[nFilterColIdx][nRow + nMiddleOfFilter];
					pTempArray[nTempArrayIdx++] = nValue;
				}
			}

			QuickSort(pTempArray, 0, nTempArrayIdx - 1);
			nMiddleIdx = nTempArrayIdx / 2;
			nValue = pTempArray[nMiddleIdx];
			pMedianArray[nMedianArrayIdx++] = nValue;

			//3. 현재 포인트
			nValue = pFilterLine[nMiddleOfFilter][x];
			pMedianArray[nMedianArrayIdx++] = nValue;

			//4. 중간값 추출
			QuickSort(pMedianArray, 0, nMedianArrayIdx - 1);
			nMiddleIdx = nMedianArrayIdx / 2;
			nValue = pMedianArray[nMiddleIdx];

			pDst[y * nWidthStep + x] = nValue;
		}

		for(nFilterColIdx = 0; nFilterColIdx < nFilterSize; nFilterColIdx++)
		{
			if(nFilterColIdx == nFilterSize - 1)
			{
				pFilterLine[nFilterColIdx] = pFilterLine[nFilterColIdx] + nWidthStep;
			}
			else
			{
				pFilterLine[nFilterColIdx] = pFilterLine[nFilterColIdx + 1];
			}
		}
	}

	delete[] pTempArray;
	delete[] pMedianArray;
	delete[] pFilterLine;

	return TRUE;
}

BOOL CCHImageProcess::ImageSmooth_HybridMedianFilter2(CCHImageData* pSourceImage, CCHImageData* pResultImage, int nFilterSize)
{
	if (pSourceImage==NULL || pResultImage==NULL) return FALSE;

	if (!pSourceImage->GetImageExist()) return FALSE;

	if (!pResultImage->CreateImage(pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetDepth(), pSourceImage->GetChannels())) return ResultImageNoExist;
	pResultImage->ClearImage(0);

	int nMiddleIdx, nMedianArrayIdx;
	int i, x, y, nWidth, nHeight, nWidthStep, nMiddleOfFilter, nValue;
	int *pKernelX, *pKernelY, *pKernelDial1, *pKernelDial2, *pMedianArray, *pTempArray;
	
	nWidth = pSourceImage->GetWidth();
	nHeight = pSourceImage->GetHeight();
	nWidthStep = pSourceImage->GetWidthStep();
	nMiddleOfFilter = nFilterSize / 2;

	BYTE* pSrcY = (BYTE*) pSourceImage->GetImageBuffer();
	BYTE* pRetY = (BYTE*) pResultImage->GetImageBuffer() + (nMiddleOfFilter * nWidthStep) + nMiddleOfFilter;
	BYTE *pRetX, *pSrcX;

	pKernelX = new int[nFilterSize];
	pKernelY = new int[nFilterSize];
	pKernelDial1 = new int[nFilterSize];
	pKernelDial2 = new int[nFilterSize];
	pTempArray = new int[nFilterSize * 2];
	pMedianArray = new int[3];

	BYTE *pX, *pY, *pD1, *pD2;

	int nPitchX = nWidthStep * nMiddleOfFilter;
	int nPitchD1 = nWidthStep + 1;
	int nPitchD2 = nWidthStep - 1;

	BYTE* pKernelValue = new BYTE[nFilterSize * nFilterSize];

	for(y = nMiddleOfFilter; y < nHeight - nMiddleOfFilter; y++)
	{
 		pSrcX = pSrcY;
 		pRetX = pRetY;

		for(x = nMiddleOfFilter; x < nWidth - nMiddleOfFilter; x++)
		{
			pX = pSrcX + nPitchX;
			pY = pSrcX + nMiddleOfFilter;
			pD1 = pSrcX;
			pD2 = pSrcX + nFilterSize - 1;

			for(i = 0; i < nFilterSize; i++)
			{
				pKernelX[i] = *pX;
				pKernelY[i] = *pY;
				pKernelDial1[i] = *pD1;
				pKernelDial2[i] = *pD2;
				
				pX++;
				pY += nWidthStep;
				pD1 += nPitchD1;
				pD2 += nPitchD2;
			}

			//1. X방향
			nMedianArrayIdx = 0;

			ZeroMemory(pTempArray, sizeof(int) * nFilterSize * 2 - 1);
			ZeroMemory(pMedianArray, sizeof(int) * 3);
			memcpy(pTempArray, pKernelDial1, sizeof(int) * nFilterSize);
			memcpy(pTempArray + nFilterSize, pKernelDial2, sizeof(int) * nMiddleOfFilter);
			memcpy(pTempArray + nFilterSize + nMiddleOfFilter, pKernelDial2 + nMiddleOfFilter + 1, sizeof(int) * nMiddleOfFilter);

			QuickSort(pTempArray, 0, nFilterSize * 2 - 2);
			nMiddleIdx = nFilterSize - 1;
			nValue = pTempArray[nMiddleIdx];
			pMedianArray[nMedianArrayIdx++] = nValue;

			//2. ＋방향
			ZeroMemory(pTempArray, sizeof(int) * nFilterSize * 2 - 1);
			memcpy(pTempArray, pKernelX, sizeof(int) * nFilterSize);
			memcpy(pTempArray + nFilterSize, pKernelY, sizeof(int) * nMiddleOfFilter);
			memcpy(pTempArray + nFilterSize + nMiddleOfFilter, pKernelY + nMiddleOfFilter + 1, sizeof(int) * nMiddleOfFilter);

			QuickSort(pTempArray, 0, nFilterSize * 2 - 2);
			nMiddleIdx = nFilterSize - 1;
			nValue = pTempArray[nMiddleIdx];
			pMedianArray[nMedianArrayIdx++] = nValue;

			//3. 현재 포인트
			nValue = pKernelX[nMiddleOfFilter];
			pMedianArray[nMedianArrayIdx++] = nValue;

			//4. 중간값 추출
			QuickSort(pMedianArray, 0, nMedianArrayIdx - 1);
			nMiddleIdx = nMedianArrayIdx / 2;
			nValue = pMedianArray[nMiddleIdx];

			*pRetX = nValue;
		
			pSrcX++;
			pRetX++;
		}

		/*for(x = nMiddleOfFilter; x < nWidth - nMiddleOfFilter; x++)
		{
			nIdx = 0;
			pSubY = pSrcX;
			
			ZeroMemory(pKernelX, sizeof(int) * nFilterSize);
			ZeroMemory(pKernelY, sizeof(int) * nFilterSize);
			ZeroMemory(pKernelDial1, sizeof(int) * nFilterSize);
			ZeroMemory(pKernelDial2, sizeof(int) * nFilterSize);

 			for(i = 0; i < nFilterSize; i++)
 			{
 				pKernelX[i] = pSrcX[(nWidthStep * nMiddleOfFilter) + i];
 				pKernelY[i] = pSrcX[(nWidthStep * i) + nMiddleOfFilter];
 				pKernelDial1[i] = pSrcX[(nWidthStep * i) + i];
 				pKernelDial2[i] = pSrcX[(nWidthStep * i) + (nFilterSize - i - 1)];
 			}

			//1. X방향
			nMedianArrayIdx = 0;

			ZeroMemory(pTempArray, sizeof(int) * nFilterSize * 2 - 1);
			ZeroMemory(pMedianArray, sizeof(int) * 3);
			memcpy(pTempArray, pKernelDial1, sizeof(int) * nFilterSize);
			memcpy(pTempArray + nFilterSize, pKernelDial2, sizeof(int) * nMiddleOfFilter);
			memcpy(pTempArray + nFilterSize + nMiddleOfFilter, pKernelDial2 + nMiddleOfFilter + 1, sizeof(int) * nMiddleOfFilter);

			QuickSort(pTempArray, 0, nFilterSize * 2 - 2);
			nMiddleIdx = nFilterSize - 1;
			nValue = pTempArray[nMiddleIdx];
			pMedianArray[nMedianArrayIdx++] = nValue;

			//2. ＋방향
			ZeroMemory(pTempArray, sizeof(int) * nFilterSize * 2 - 1);
			memcpy(pTempArray, pKernelX, sizeof(int) * nFilterSize);
			memcpy(pTempArray + nFilterSize, pKernelY, sizeof(int) * nMiddleOfFilter);
			memcpy(pTempArray + nFilterSize + nMiddleOfFilter, pKernelY + nMiddleOfFilter + 1, sizeof(int) * nMiddleOfFilter);

			QuickSort(pTempArray, 0, nFilterSize * 2 - 2);
			nMiddleIdx = nFilterSize - 1;
			nValue = pTempArray[nMiddleIdx];
			pMedianArray[nMedianArrayIdx++] = nValue;

			//3. 현재 포인트
			nValue = pKernelX[nMiddleOfFilter];
			pMedianArray[nMedianArrayIdx++] = nValue;

			//4. 중간값 추출
			QuickSort(pMedianArray, 0, nMedianArrayIdx - 1);
			nMiddleIdx = nMedianArrayIdx / 2;
			nValue = pMedianArray[nMiddleIdx];

// 			for(i = 0; i < nFilterSize; i++)
// 			{
// 				pSubX = pSubY;
// 
// 				for(j = 0; j < nFilterSize; j++)
// 				{
// 					pKernelValue[nIdx++] = *pSubX++;
// 				}
// 
// 				pSubY += nWidthStep;
// 			}
// 
// 			nValue = GetHybridMedianFilterValue(pKernelValue, nFilterSize);
			*pRetX = nValue;
			pSrcX++;
			pRetX++;
		}*/

		pSrcY += nWidthStep;
		pRetY += nWidthStep;
	}

	delete[] pKernelX;
	delete[] pKernelY;
	delete[] pKernelDial1;
	delete[] pKernelDial2;
	delete[] pTempArray;
	delete[] pMedianArray;
	delete[] pKernelValue;

	return TRUE;
}

int CCHImageProcess::GetHybridMedianFilterValue(BYTE* pKernelValue, int nFilterSize)
{
	int *pMedianArray, *pTempArray;
	int i, j, nIdx, nValue, nMiddleOfFilter, nMiddleIdx, nTempArrayIdx, nMedianArrayIdx;

	pTempArray = new int[nFilterSize * 2];
	ZeroMemory(pTempArray, sizeof(int) * nFilterSize * 2);
	pMedianArray = new int[3];
	ZeroMemory(pMedianArray, sizeof(int) * 3);

	//1. X방향
	nTempArrayIdx = 0;
	nMedianArrayIdx = 0;
	nMiddleOfFilter = nFilterSize / 2;

	for(i = 0; i < nFilterSize; i++)
	{
		nIdx = i * nFilterSize + i;
		nValue = pKernelValue[nIdx];
		pTempArray[nTempArrayIdx++] = nValue;

		if(i != nMiddleOfFilter)
		{
			nIdx = (i + 1) * nFilterSize - (i + 1);
			nValue = pKernelValue[nIdx];
			pTempArray[nTempArrayIdx++] = nValue;
		}
	}

	QuickSort(pTempArray, 0, nTempArrayIdx - 1);
	nMiddleIdx = nTempArrayIdx / 2;
	nValue = pTempArray[nMiddleIdx];
	pMedianArray[nMedianArrayIdx++] = nValue;

	//2. ＋방향
	nTempArrayIdx = 0;
	ZeroMemory(pTempArray, sizeof(int) * nFilterSize * 2);

	for(i = 0; i < nFilterSize; i++)
	{
		if(i == nMiddleOfFilter)
		{
			nIdx = i * nFilterSize;

			for(j = 0; j < nFilterSize; j++)
			{
				nValue = pKernelValue[nIdx++];
				pTempArray[nTempArrayIdx++] = nValue;
			}
		}
		else
		{
			nIdx = i * nFilterSize + nMiddleOfFilter;
			nValue = pKernelValue[nIdx];
			pTempArray[nTempArrayIdx++] = nValue;
		}
	}

	QuickSort(pTempArray, 0, nTempArrayIdx - 1);
	nMiddleIdx = nTempArrayIdx / 2;
	nValue = pTempArray[nMiddleIdx];
	pMedianArray[nMedianArrayIdx++] = nValue;

	//3. 현재 포인트
	nIdx = nMiddleOfFilter * nFilterSize + nMiddleOfFilter;
	nValue = pKernelValue[nIdx];
	pMedianArray[nMedianArrayIdx++] = nValue;
	
	//4. 중간값 추출
	QuickSort(pMedianArray, 0, nMedianArrayIdx - 1);
	nMiddleIdx = nMedianArrayIdx / 2;
	nValue = pMedianArray[nMiddleIdx];

	delete[] pTempArray;
	delete[] pMedianArray;

	return nValue;
}

template<typename T>

inline T limit(const T& value)
{
	return ( (value > 255) ? 255 : ((value < 0) ? 0 : value) );
}


//비등방성 확산 필터
void CCHImageProcess::ImageDiffusion(BYTE *pBuffer, int w, int h, int iter, float lambda, float k)
{
	register int i, x, y;
	float k2 = k * k;
	float gradn, grads, grade, gradw;
	float gcn, gcs, gce, gcw;
	float tmp;

	// 연산을실수형으로하기위하여실수형2차원배열동적생성
	float** cpy = new float*[h];

	for( i = 0 ; i < h ; i++ )
	{
		cpy[i] = new float[w];
		memset(cpy[i], 0, sizeof(float)*w);
	}

	float** buf = new float*[h];

	for( i = 0 ; i < h ; i++ )
	{
		buf[i] = new float[w];
		memset(buf[i], 0, sizeof(float)*w);
	}

	// 입력영상의정보를복사
	BYTE *pImg = pBuffer;

	for( y = 0 ; y < h ; y++ )
		for( x = 0 ; x < w ; x++ )
		{
			cpy[y][x] = buf[y][x] = (float)*pImg++;
		}

		for( i = 0 ; i < iter ; i++ )
		{
			for( y = 1 ; y < h-1 ; y++ )
				for( x = 1 ; x < w-1 ; x++ )
				{
					tmp = cpy[y][x];
					gradn = cpy[y-1][x  ] - tmp;
					grads = cpy[y+1][x  ] - tmp;
					grade = cpy[y  ][x-1] - tmp;
					gradw = cpy[y  ][x+1] - tmp;

					gcn = gradn / (1.0f + gradn*gradn/k2);
					gcs = grads / (1.0f + grads*grads/k2);
					gce = grade / (1.0f + grade*grade/k2);
					gcw = gradw / (1.0f + gradw*gradw/k2);

					buf[y][x] = cpy[y][x] + lambda*(gcn + gcs + gce + gcw);
				}

				// 버퍼복사
				for( y = 0 ; y < h ; y++ )
					memcpy(cpy[y], buf[y], sizeof(float)*w);
		}

		// 입력영상의값갱신
		pImg = pBuffer;

		for( y = 0 ; y < h ; y++ )
			for( x = 0 ; x < w ; x++ )
			{
				*pImg++ = (BYTE)limit(buf[y][x] + 0.5f);
			}

			// 동적할당한메모리해제
			for( i = 0 ; i < h ; i++ )
			{
				delete [] buf[i];
				delete [] cpy[i];
			}

			delete [] buf;
			delete [] cpy;
}

void CCHImageProcess::Swap(int* pA, int* pB)
{
	int nTemp = *pA;
	*pA = *pB;
	*pB = nTemp;
}

void CCHImageProcess::QuickSort(int* nArr, int nStart, int nEnd)
{
	int nPivot = nArr[nStart];
	int nLeft = nStart + 1;
	int nRight = nEnd;

	// 개수가 적어서 찾을 것이 없는가?
	if( nEnd - nStart <= 0 )
	{
		return;
	}

	while(1)
	{
		// Left를 찾는다.
		for(; nLeft <= nEnd; nLeft++)
		{
			if(nPivot < nArr[nLeft])
			{
				break;
			}
		}

		// Right를 찾는다.
		for(; nRight > nStart; nRight--)
		{
			if(nPivot > nArr[nRight])
			{
				break;
			}
		}

		// 바꿔야할 상황이라면 바꾼다.
		if(nLeft <= nEnd && nRight > nStart && nLeft < nRight)
		{
			Swap(&nArr[nLeft], &nArr[nRight]);
			continue;
		}

		// Left, Right가 모두 범위 밖으로 나갔다면 배열 안의 숫자는 모두 같은 숫자다.
		if(nRight <= nStart && nLeft > nEnd)
		{
			return;
		}

		// 조건이 된다면 왼쪽 Array를 Recursive로 돌린다.
		if(nRight > nStart)
		{
			// Pivot과 Right를 바꾼다.
			Swap(&nArr[nStart], &nArr[nRight]);
			QuickSort(nArr, nStart, nRight);
		}

		// 오른쪽 Array는 지금의 함수를 재사용.
		nStart = nRight + 1;
		nPivot = nArr[nStart];
		nLeft = nStart + 1;
		nRight = nEnd;

		// 개수가 적어서 찾을 것이 없는가?
		if(nEnd - nStart <= 0)
		{
			return;
		}
	}
}



