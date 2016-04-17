#include "StdAfx.h"
#include "CHDataFitting.h"

#include <algorithm>
#include "cxcore.h"

CCHDataFitting::CCHDataFitting(void)
{

}

CCHDataFitting::~CCHDataFitting(void)
{

}

inline bool CompareValue(double& a, double& b)
{
	return (a < b);
}

inline double Median(double *pValue, int nSize)
{
	int odd = nSize % 2;
	int index = nSize / 2;

	// sort 
	double dTemp = 0.0;
	for(int i=0; i<index+1; i++)
	{
		for(int j=i+1; j<nSize; j++)
		{
			if(pValue[i]>pValue[j])
			{
				dTemp=pValue[i];
				pValue[i]=pValue[j];
				pValue[j]=dTemp;
			}
		}
	}

	if (odd==1)
	{
		return pValue[index];
	}

	return ((pValue[index-1]+pValue[index])/2.0);
}

inline double MedianAbsoluteDeviation(double *pValue, int nSize, double dMedian)
{
	for (int i=0; i<nSize; i++)
	{
		pValue[i] = fabs(pValue[i]-dMedian);
	}

	return Median(pValue, nSize);
}

inline double Median(VectorDouble &x)
{
	std::sort(x.begin(), x.end(), CompareValue);

	size_t odd = x.size() % 2;
	size_t index = x.size() / 2;

	if (odd==1)
	{
		return x[index];
	}

	return ((x[index-1]+x[index])/2.0);
}

inline double MedianAbsoluteDeviation(VectorDouble &x, double medianValue)
{
	VectorDouble mad;
	for (size_t i=0; i<x.size(); i++)
	{
		mad.push_back(fabs(x[i]-medianValue));
	}

	std::sort(mad.begin(), mad.end(), CompareValue);

	return Median(mad);
}

double CCHDataFitting::OneStepBiweightAlgorithm(double *pResidual, double *pWeight, int nSize, double dWeight, double dEpsilon)
{
	double *pTempValue = new double[nSize];
	memcpy(pTempValue, pResidual, sizeof(double)*nSize);

	double medianValue = Median(pTempValue, nSize);
	double MAD = MedianAbsoluteDeviation(pTempValue, nSize, medianValue) * dWeight + dEpsilon;

	double diff, u, uSquare, oneMinusUSqaure;
	double weightedSumNumer=0.0;
	double weightedSumDenom=0.0;

	for (int i=0; i<nSize; i++)
	{
		diff = pResidual[i] - medianValue;
		u = diff / MAD;
		uSquare = u * u;
		oneMinusUSqaure = 1.0 - uSquare;

		if (fabs(u) <= 1.0)
		{
			weightedSumNumer += diff * oneMinusUSqaure * oneMinusUSqaure;
			weightedSumDenom += oneMinusUSqaure * oneMinusUSqaure;
			pWeight[i] = oneMinusUSqaure * oneMinusUSqaure;
		}
		else
		{
			pWeight[i] = 0.0;
		}
	}

	double dValue=0.0;
	if (weightedSumDenom != 0.0)
	{
		dValue = medianValue + weightedSumNumer / weightedSumDenom;
	}

	delete [] pTempValue;

	return dValue;
}

double CCHDataFitting::OneStepBiweightAlgorithm(VectorDouble &vectorResidual, VectorDouble &vectorWeight, double dWeight, double dEpsilon)
{
	VectorDouble tempX = vectorResidual;

	double medianValue = Median(tempX);
	double MAD = MedianAbsoluteDeviation(tempX, medianValue) * dWeight + dEpsilon;

	double diff, u, uSquare, oneMinusUSqaure;
	double weightedSumNumer=0.0;
	double weightedSumDenom=0.0;

	size_t nSize = vectorResidual.size();
	for (size_t i=0; i<nSize; i++)
	{
		diff = vectorResidual[i] - medianValue;
		u = diff / MAD;
		uSquare = u * u;
		oneMinusUSqaure = 1.0 - uSquare;

		if (fabs(u) <= 1.0)
		{
			weightedSumNumer += diff * oneMinusUSqaure * oneMinusUSqaure;
			weightedSumDenom += oneMinusUSqaure * oneMinusUSqaure;

			vectorWeight[i] = oneMinusUSqaure * oneMinusUSqaure;
		}
		else
		{
			vectorWeight[i] = 0.0;
		}
	}

	double dValue=0.0;

	if (weightedSumDenom != 0.0)
	{
		dValue = medianValue + weightedSumNumer / weightedSumDenom;
	}

	return dValue;
}

int CCHDataFitting::SolveMatrix(double *pSrc, double *pSrc2Arg, double *pDst, int nSize, int nMethod)
{
	CvMat src = cvMat( nSize, nSize, CV_64FC1, pSrc );
	CvMat src2arg = cvMat( nSize, 1, CV_64FC1, pSrc2Arg );
	CvMat dst = cvMat( nSize, 1, CV_64FC1, pDst );

	return cvSolve(&src, &src2arg, &dst, nMethod);
}

double CCHDataFitting::InvertMatrix(double *pSrc, double*pDst, int nSize, int nMethod)
{
	CvMat src = cvMat( nSize, nSize, CV_64FC1, pSrc );
	CvMat dst = cvMat( nSize, nSize, CV_64FC1, pDst );

	return cvInvert(&src, &dst, nMethod);
}

int CCHDataFitting::EigenVectorValue(double *pSrcMatrix, double *pEignVector, double *pEignValue, int nSize)
{
	CvMat srcMat = cvMat( nSize, nSize, CV_64FC1, pSrcMatrix);
	CvMat eigenVector = cvMat( nSize, nSize, CV_64FC1, pEignVector);
	CvMat eigenValue = cvMat( nSize, 1, CV_64FC1, pEignValue);
	
	cvEigenVV(&srcMat, &eigenVector, &eigenValue);

	return 1;
}

int CCHDataFitting::MatrixMultiply(double *pSrcA, double *pSrcB, double *pDst, int nRows, int nCols)
{
	CvMat a = cvMat( nRows, nCols, CV_64FC1, pSrcA);
	CvMat b = cvMat( nCols, nRows, CV_64FC1, pSrcB);
	CvMat result = cvMat( nRows, nRows, CV_64FC1, pDst);

	cvMatMul(&a, &b, &result);

	return 1;
}

int CCHDataFitting::MatrixTranspose(double *pSource, double *pResult, int nRows, int nCols)
{
	CvMat src = cvMat( nRows, nCols, CV_64FC1, pSource);
	CvMat result = cvMat( nCols, nRows, CV_64FC1, pResult);

	cvTranspose(&src, &result);

	return 1;
}


int CCHDataFitting::PolynomialFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorR, int nOrder)
{
	vectorR.clear();

	int nSize = (int)vectorX.size();
	if (nOrder >= nSize) return 0;

	int nMatrixSize = nOrder + 1;
	int nMaxOrder = nOrder * 2 + 1;

	double *pX = new double[nMatrixSize * nMatrixSize];
	double *pA = new double[nMatrixSize];
	double *pY = new double[nMatrixSize];
	double *pOrderX = new double[nMaxOrder];

	int nTemp = 1;
	for (int i=0; i<nSize; i++)
	{
		//pOrderX[0] = (pOrderX[0]*i) / nTemp;
		pOrderX[1] = (pOrderX[1]*i + vectorX[i]) / nTemp;
		for (int j=2; j<nMaxOrder; j++)
		{
			pOrderX[j] = (pOrderX[j]*i + pow(vectorX[i],int(j))) / nTemp;
		}

		pY[0] = (pY[0]*i + vectorY[i]) / nTemp;
		pY[1] = (pY[1]*i + vectorX[i]*vectorY[i]) / nTemp;
		for (int j=2; j<nMatrixSize; j++)
		{
			pY[j] = (pY[j]*i + (pow(vectorX[i],int(j))*vectorY[i])) / nTemp;
		}
		nTemp++;
	}
	pOrderX[0] = 1.0;

	double *pSubX = pX;
	for (int i=0; i<nMatrixSize; i++)
	{
		for (int j=0; j<nMatrixSize; j++)
		{
			*pSubX++ = pOrderX[i+j];
		}
	}

	if (SolveMatrix(pX, pY, pA, nMatrixSize)!=1)
	{
		delete [] pX;
		delete [] pA;
		delete [] pY;
		delete [] pOrderX;

		return 0;
	}

	for (int i=0; i<nMatrixSize; i++)
	{
		vectorR.push_back(pA[i]);
	}

	delete [] pX;
	delete [] pA;
	delete [] pY;
	delete [] pOrderX;

	return 1;
}

int CCHDataFitting::PolynomialFitting(double *pXValue, double *pYValue, int nSize, double *pResult, int nOrder)
{
	int nMatrixSize = nOrder + 1;
	int nMaxOrder = nOrder * 2 + 1;

	memset(pResult, 0, sizeof(double)*nMatrixSize);

	if (nOrder >= nSize) return 0;

	double *pX = new double[nMatrixSize * nMatrixSize];
	double *pA = new double[nMatrixSize];
	double *pY = new double[nMatrixSize];
	double *pOrderX = new double[nMaxOrder];

	int nTemp = 1;
	for (int i=0; i<nSize; i++)
	{
		//pOrderX[0] = (pOrderX[0]*i) / nTemp;
		pOrderX[1] = (pOrderX[1]*i + pXValue[i]) / nTemp;
		for (int j=2; j<nMaxOrder; j++)
		{
			pOrderX[j] = (pOrderX[j]*i + pow(pXValue[i],j)) / nTemp;
		}

		pY[0] = (pY[0]*i + pYValue[i]) / nTemp;
		pY[1] = (pY[1]*i + pXValue[i]*pYValue[i]) / nTemp;
		for (int j=2; j<nMatrixSize; j++)
		{
			pY[j] = (pY[j]*i + (pow(pXValue[i],j)*pYValue[i])) / nTemp;
		}
		nTemp++;
	}
	pOrderX[0] = 1.0;

	double *pSubX = pX;
	for (int i=0; i<nMatrixSize; i++)
	{
		for (int j=0; j<nMatrixSize; j++)
		{
			*pSubX++ = pOrderX[i+j];
		}
	}

	if (SolveMatrix(pX, pY, pA, nMatrixSize)!=1)
	{
		delete [] pX;
		delete [] pA;
		delete [] pY;
		delete [] pOrderX;

		return 0;
	}

	// copy result
	memcpy(pResult, pA, sizeof(double)*nMatrixSize);

	delete [] pX;
	delete [] pA;
	delete [] pY;
	delete [] pOrderX;

	return 1;
}

int CCHDataFitting::GaussianFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorR)
{
	vectorR.clear();

	size_t nSize = vectorX.size();

	if (nSize < 3) return 0;

	VectorDouble vectorLY;
	vectorLY.resize(nSize);
	for (size_t i=0; i<nSize; i++)
	{
		vectorLY[i] = log(vectorY[i]);
	}

	VectorDouble vectorTemp;
	if (PolynomialFitting(vectorX, vectorLY, vectorTemp, 2)!=1)
	{
		return 0;
	}

	double dVariance = -1.0 * sqrt(fabs(1.0/vectorTemp[2]));
	double dMean = -1.0 * (vectorTemp[1] / (2.0*vectorTemp[2]));
	double dHeight = exp(vectorTemp[0] + (dMean*dMean)/(dVariance*dVariance));

	vectorR.push_back(dHeight);
	vectorR.push_back(dMean);
	vectorR.push_back(dVariance);

	return 1;
}

int CCHDataFitting::GaussianFitting(double *pXValue, double *pYValue, int nSize, double *pResult)
{
	memset(pResult, 0, sizeof(double)*3);

	if (nSize < 3) return 0;

	double *pYLogValue = new double[nSize];
	for (int i=0; i<nSize; i++)
	{
		pYLogValue[i] = log(pYValue[i]);
	}

	double *pResultTemp = new double[3];
	if (PolynomialFitting(pXValue, pYLogValue, nSize, pResultTemp, 2)!=1)
	{
		delete [] pYLogValue;
		delete [] pResultTemp;

		return 0;
	}

	double dVariance = -1.0 * sqrt(fabs(1.0/pResultTemp[2]));
	double dMean = -1.0 * (pResultTemp[1] / (2.0*pResultTemp[2]));
	double dHeight = exp(pResultTemp[0] + (dMean*dMean)/(dVariance*dVariance));

	pResult[0] = dHeight;
	pResult[1] = dMean;
	pResult[2] = dVariance;

	delete [] pYLogValue;
	delete [] pResultTemp;

	return 1;
}


int CCHDataFitting::IRLS_PolynomialFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorR, int nOrder, int nIteration, double dWeight, double dStopThres)
{
	vectorR.clear();

	int nSize = (int)vectorX.size();
	if (nOrder >= nSize) return 0;

	int nMatrixSize = nOrder + 1;
	int nMaxOrder = nOrder * 2 + 1;

	VectorDouble vectorWeight, vectorResidual;
	vectorWeight.resize(nSize);
	vectorResidual.resize(nSize);

	for (int i=0; i<nSize; i++)
	{
		vectorWeight[i] = 1.0;
	}

	double *pX = new double[nMatrixSize * nMatrixSize];
	double *pA = new double[nMatrixSize];
	double *pY = new double[nMatrixSize];
	double *pOrderX = new double[nMaxOrder];

	double dCurValue = 0.0;
	double dPrevValue = 0.0;

	int nReturnIteration = 0;
	for (nReturnIteration=0; nReturnIteration<nIteration; nReturnIteration++)
	{
		int nTemp = 1;
		for (int i=0; i<nSize; i++)
		{
			pOrderX[0] = (pOrderX[0]*i + vectorWeight[i]) / nTemp;
			pOrderX[1] = (pOrderX[1]*i + vectorWeight[i]*vectorX[i]) / nTemp;
			for (int j=2; j<nMaxOrder; j++)
			{
				pOrderX[j] = (pOrderX[j]*i + vectorWeight[i]*pow(vectorX[i],int(j))) / nTemp;
			}

			pY[0] = (pY[0]*i +  vectorWeight[i]*vectorY[i]) / nTemp;
			pY[1] = (pY[1]*i +  vectorWeight[i]*vectorX[i]*vectorY[i]) / nTemp;
			for (int j=2; j<nMatrixSize; j++)
			{
				pY[j] = (pY[j]*i + vectorWeight[i]*(pow(vectorX[i],int(j))*vectorY[i])) / nTemp;
			}
			nTemp++;
		}

		double *pSubX = pX;
		for (int i=0; i<nMatrixSize; i++)
		{
			for (int j=0; j<nMatrixSize; j++)
			{
				*pSubX++ = pOrderX[i+j];
			}
		}

		if (SolveMatrix(pX, pY, pA, nMatrixSize))
		{
			if (nIteration==1) break; 

			for (int i=0; i<nSize; i++)
			{
				vectorResidual[i] = pA[0] + (pA[1]*vectorX[i]);
				for (int j=2; j<nMatrixSize; j++)
				{
					vectorResidual[i] += pA[j] * pow(vectorX[i], int(j));					
				}
				vectorResidual[i] = vectorY[i] - vectorResidual[i];
			}

			dCurValue = OneStepBiweightAlgorithm(vectorResidual, vectorWeight, dWeight);
		}
		else
		{
			delete [] pX;
			delete [] pA;
			delete [] pY;
			delete [] pOrderX;

			return 0;
		}

		if ( fabs(dPrevValue-dCurValue) < dStopThres ) break;

		dPrevValue = dCurValue;
	}

	for (int i=0; i<nMatrixSize; i++)
	{
		vectorR.push_back(pA[i]);
	}

	delete [] pX;
	delete [] pA;
	delete [] pY;
	delete [] pOrderX;

	return nReturnIteration+1;
}

int CCHDataFitting::IRLS_PolynomialFitting(double *pXValue, double *pYValue, int nSize, double *pResult, int nOrder,  int nIteration, double dWeight, double dStopThres)
{
	int nMatrixSize = nOrder + 1;
	int nMaxOrder = nOrder * 2 + 1;

	memset(pResult, 0, sizeof(double)*nMatrixSize);

	if (nOrder >= nSize) return 0;

	double *pWeight = new double[nSize];
	double *pResidual = new double[nSize];
	for (int i=0; i<nSize; i++)
	{
		pWeight[i] = 1.0;
	}

	double *pX = new double[nMatrixSize * nMatrixSize];
	double *pA = new double[nMatrixSize];
	double *pY = new double[nMatrixSize];
	double *pOrderX = new double[nMaxOrder];

	double dCurValue = 0.0;
	double dPrevValue = 0.0;

	int nReturnIteration = 0;
	for (nReturnIteration=0; nReturnIteration<nIteration; nReturnIteration++)
	{
		int nTemp = 1;
		for (int i=0; i<nSize; i++)
		{
			pOrderX[0] = (pOrderX[0]*i + pWeight[i]) / nTemp;
			pOrderX[1] = (pOrderX[1]*i + pWeight[i]*pXValue[i]) / nTemp;
			for (int j=2; j<nMaxOrder; j++)
			{
				pOrderX[j] = (pOrderX[j]*i + pWeight[i]*pow(pXValue[i],j)) / nTemp;
			}

			pY[0] = (pY[0]*i +  pWeight[i]*pYValue[i]) / nTemp;
			pY[1] = (pY[1]*i +  pWeight[i]*pXValue[i]*pYValue[i]) / nTemp;
			for (int j=2; j<nMatrixSize; j++)
			{
				pY[j] = (pY[j]*i + pWeight[i]*(pow(pXValue[i],j)*pYValue[i])) / nTemp;
			}
			nTemp++;
		}

		double *pSubX = pX;
		for (int i=0; i<nMatrixSize; i++)
		{
			for (int j=0; j<nMatrixSize; j++)
			{
				*pSubX++ = pOrderX[i+j];
			}
		}

		if (SolveMatrix(pX, pY, pA, nMatrixSize))
		{
			if (nIteration==1) break;

			for (int i=0; i<nSize; i++)
			{
				pResidual[i] = pA[0] + (pA[1]*pXValue[i]);
				for (int j=2; j<nMatrixSize; j++)
				{
					pResidual[i] += pA[j]*pow(pXValue[i],j);					
				}
				pResidual[i] = pYValue[i]-pResidual[i];
			}

			dCurValue = OneStepBiweightAlgorithm(pResidual, pWeight, nSize, dWeight);
		}
		else
		{
			delete [] pX;
			delete [] pA;
			delete [] pY;
			delete [] pOrderX;
			delete [] pWeight;
			delete [] pResidual;

			return 0;
		}

		if ( fabs(dPrevValue-dCurValue) < dStopThres ) break;
		
		dPrevValue = dCurValue;
	}

	// copy result
	memcpy(pResult, pA, sizeof(double)*nMatrixSize);

	delete [] pX;
	delete [] pA;
	delete [] pY;
	delete [] pOrderX;
	delete [] pWeight;
	delete [] pResidual;

	return nReturnIteration+1;
}

int CCHDataFitting::IRLS_PlaneFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorZ, VectorDouble& vectorResult,  int nIteration, double dWeight, double dStopThres)
{
	vectorResult.clear();

	size_t nSize = vectorX.size();
	if (nSize<3) return 0;

	VectorDouble vectorWeight, vectorResidual;
	vectorWeight.resize(nSize);
	vectorResidual.resize(nSize);

	for (size_t i=0; i<nSize; i++)
	{
		vectorWeight[i] = 1.0;
	}

	double pX[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	double pA[3] = {0, 0, 0};
	double pY[3] = {0, 0, 0};

	double dCurValue = 0.0;
	double dPrevValue = 0.0;

	int nReturnIteration = 0;
	for (nReturnIteration=0; nReturnIteration<nIteration; nReturnIteration++)
	{
		int nTemp = 1;
		for (size_t i=0; i<nSize; i++)
		{
			pX[0] = (pX[0]*i + vectorWeight[i]) / nTemp; // 1
			pX[1] = (pX[1]*i + vectorWeight[i]*vectorY[i]) / nTemp; // y
			pX[2] = (pX[2]*i + vectorWeight[i]*vectorX[i]) / nTemp; // x
			pX[4] = (pX[4]*i + vectorWeight[i]*vectorY[i]*vectorY[i]) / nTemp; // yy
			pX[5] = (pX[5]*i + vectorWeight[i]*vectorX[i]*vectorY[i]) / nTemp; // xy
			pX[8] = (pX[8]*i + vectorWeight[i]*vectorX[i]*vectorX[i]) / nTemp; // xx

			pY[0] = (pY[0]*i +  vectorWeight[i]*vectorZ[i]) / nTemp;
			pY[1] = (pY[1]*i +  vectorWeight[i]*vectorY[i]*vectorZ[i]) / nTemp;
			pY[2] = (pY[2]*i +  vectorWeight[i]*vectorX[i]*vectorZ[i]) / nTemp;

			nTemp++;
		}

		pX[3] = pX[1];
		pX[6] = pX[2];
		pX[7] = pX[5];

		if (SolveMatrix(pX, pY, pA, 3))
		{
			if (nIteration==1) break;

			for (size_t i=0; i<nSize; i++)
			{
				vectorResidual[i] = vectorZ[i] - (pA[0] + (pA[1]*vectorY[i]) + (pA[2]*vectorX[i]));
			}

			dCurValue = OneStepBiweightAlgorithm(vectorResidual, vectorWeight, dWeight);
		}
		else
		{
			return 0;
		}

		if ( fabs(dPrevValue-dCurValue) < dStopThres ) break;
		
		dPrevValue = dCurValue;
	}

	for (size_t i=0; i<3; i++)
	{
		vectorResult.push_back(pA[i]);
	}

	return nReturnIteration+1;
}

int CCHDataFitting::IRLS_PlaneFitting(double *pXValue, double *pYValue, double *pZValue, int nSize, double *pResult,  int nIteration, double dWeight, double dStopThres)
{
	memset(pResult, 0, sizeof(double)*3);

	if (nSize<3) return 0;

	double *pWeight = new double[nSize];
	double *pResidual = new double[nSize];
	for (int i=0; i<nSize; i++)
	{
		pWeight[i] = 1.0;
	}

	double pX[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	double pA[3] = {0, 0, 0};
	double pY[3] = {0, 0, 0};

	double dCurValue = 0.0;
	double dPrevValue = 0.0;

	int nReturnIteration = 0;
	for (nReturnIteration=0; nReturnIteration<nIteration; nReturnIteration++)
	{
		int nTemp = 1;
		for (int i=0; i<nSize; i++)
		{
			pX[0] = (pX[0]*i + pWeight[i]) / nTemp; // 1
			pX[1] = (pX[1]*i + pWeight[i]*pYValue[i]) / nTemp; // y
			pX[2] = (pX[2]*i + pWeight[i]*pXValue[i]) / nTemp; // x
			pX[4] = (pX[4]*i + pWeight[i]*pYValue[i]*pYValue[i]) / nTemp; // yy
			pX[5] = (pX[5]*i + pWeight[i]*pXValue[i]*pYValue[i]) / nTemp; // xy
			pX[8] = (pX[8]*i + pWeight[i]*pXValue[i]*pXValue[i]) / nTemp; // xx

			pY[0] = (pY[0]*i +  pWeight[i]*pZValue[i]) / nTemp;	// z		
			pY[1] = (pY[1]*i +  pWeight[i]*pZValue[i]*pYValue[i]) / nTemp; // zy
			pY[2] = (pY[2]*i +  pWeight[i]*pZValue[i]*pXValue[i]) / nTemp; // zx
			
			nTemp++;
		}

		pX[3] = pX[1];
		pX[6] = pX[2];
		pX[7] = pX[5];

		if (SolveMatrix(pX, pY, pA, 3))
		{
			if (nIteration==1) break;

			for (int i=0; i<nSize; i++)
			{
				pResidual[i] = pZValue[i] - (pA[0] + (pA[1]*pYValue[i]) + (pA[2]*pXValue[i]));
			}

			dCurValue = OneStepBiweightAlgorithm(pResidual, pWeight, nSize, dWeight);
		}
		else
		{
			delete [] pWeight;
			delete [] pResidual;
			return 0;
		}

		if ( fabs(dPrevValue-dCurValue) > dStopThres ) break;

		dPrevValue = dCurValue;
	}

	// copy result
	memcpy(pResult, pA, sizeof(double)*3);

	delete [] pWeight;
	delete [] pResidual;

	return nReturnIteration+1;
}

int CCHDataFitting::IRLS_GaussianFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorR, int nIteration, double dWeight, double dStopThres)
{
	vectorR.clear();

	size_t nSize = vectorX.size();

	if (nSize < 3) return 0;

	VectorDouble vectorWeight, vectorResidual, vectorLY;
	vectorWeight.resize(nSize);
	vectorResidual.resize(nSize);
	vectorLY.resize(nSize);

	for (size_t i=0; i<nSize; i++)
	{
		vectorWeight[i] = 1.0;
		vectorLY[i] = log(vectorY[i]);
	}

	double pX[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	double pY[3] = {0, 0, 0};
	double pA[3] = {0, 0, 0};

	double dHeight, dMean, dVariance;
	double dCurValue = 0.0;
	double dPrevValue = 0.0;

	int nReturnIteration = 0;
	for (nReturnIteration=0; nReturnIteration<nIteration; nReturnIteration++)
	{
		size_t nTemp = 1;
		for (size_t i=0; i<nSize; i++)
		{
			pX[0] = (pX[0]*i + vectorWeight[i]) / nTemp;
			pX[1] = (pX[1]*i + vectorWeight[i]*vectorX[i]) / nTemp;
			pX[2] = (pX[2]*i + vectorWeight[i]*vectorX[i]*vectorX[i]) / nTemp;
			pX[5] = (pX[5]*i + vectorWeight[i]*vectorX[i]*vectorX[i]*vectorX[i]) / nTemp;
			pX[8] = (pX[8]*i + vectorWeight[i]*vectorX[i]*vectorX[i]*vectorX[i]*vectorX[i]) / nTemp;

			pY[0] = (pY[0]*i + vectorWeight[i]*vectorLY[i]) / nTemp;
			pY[1] = (pY[1]*i + vectorWeight[i]*vectorX[i]*vectorLY[i]) / nTemp;
			pY[2] = (pY[2]*i + vectorWeight[i]*vectorX[i]*vectorX[i]*vectorLY[i]) / nTemp;

			nTemp++;
		}

		pX[3] = pX[1];
		pX[4] = pX[2];
		pX[6] = pX[2];
		pX[7] = pX[5];

		if (SolveMatrix(pX, pY, pA, 3))
		{
			dVariance = -1.0 * sqrt(fabs(1.0/pA[2]));
			dMean = -1.0 * (pA[1] / (2.0*pA[2]));
			dHeight = exp(pA[0] + (dMean*dMean)/(dVariance*dVariance));

			if (nIteration==1) break;

			for (size_t i=0; i<nSize; i++)
			{
				vectorResidual[i] = -1.0 * ( (vectorX[i]-dMean)*(vectorX[i]-dMean) / (dVariance*dVariance) );
				vectorResidual[i] = vectorY[i] - (dHeight*exp(vectorResidual[i]));
			}

			dCurValue = OneStepBiweightAlgorithm(vectorResidual, vectorWeight, dWeight);
		}
		else
		{
			return 0;
		}

		if ( fabs(dPrevValue-dCurValue) < dStopThres ) break; 

		dPrevValue = dCurValue;
	}

	vectorR.push_back(dHeight);
	vectorR.push_back(dMean);
	vectorR.push_back(dVariance);

	return nReturnIteration+1;
}

int CCHDataFitting::IRLS_GaussianFitting(double *pXValue, double *pYValue, int nSize, double *pResult, int nIteration, double dWeight, double dStopThres)
{
	memset(pResult, 0, sizeof(double)*3);

	if (nSize < 3) return 0;

	double *pWeight = new double[nSize];
	double *pResidual = new double[nSize];
	double *pYLogValue = new double[nSize];

	for (int i=0; i<nSize; i++)
	{
		pWeight[i] = 1.0;
		pYLogValue[i] = log(pYValue[i]);
	}

	double pX[9], pY[3], pA[3];
	double dHeight, dMean, dVariance;
	double dCurValue = 0.0;
	double dPrevValue = 0.0;

	memset(pX, 0, sizeof(double)*9);
	memset(pY, 0, sizeof(double)*3);
	memset(pA, 0, sizeof(double)*3);

	int nReturnIteration = 0;

	for (nReturnIteration=0; nReturnIteration<nIteration; nReturnIteration++)
	{
		int nTemp = 1;
		for (int i=0; i<nSize; i++)
		{
			pX[0] = (pX[0]*i + pWeight[i]) / nTemp;
			pX[1] = (pX[1]*i + pWeight[i]*pXValue[i]) / nTemp;
			pX[2] = (pX[2]*i + pWeight[i]*pXValue[i]*pXValue[i]) / nTemp;
			pX[5] = (pX[5]*i + pWeight[i]*pXValue[i]*pXValue[i]*pXValue[i]) / nTemp;
			pX[8] = (pX[8]*i + pWeight[i]*pXValue[i]*pXValue[i]*pXValue[i]*pXValue[i]) / nTemp;

			pY[0] = (pY[0]*i + pWeight[i]*pYLogValue[i]) / nTemp;
			pY[1] = (pY[1]*i + pWeight[i]*pXValue[i]*pYLogValue[i]) / nTemp;
			pY[2] = (pY[2]*i + pWeight[i]*pXValue[i]*pXValue[i]*pYLogValue[i]) / nTemp;

			nTemp++;
		}

		pX[3] = pX[1];
		pX[4] = pX[2];
		pX[6] = pX[2];
		pX[7] = pX[5];

		if (SolveMatrix(pX, pY, pA, 3))
		{
			dVariance = -1.0 * sqrt(fabs(1.0/pA[2]));
			dMean = -1.0 * (pA[1] / (2.0*pA[2]));
			dHeight = exp(pA[0] + (dMean*dMean)/(dVariance*dVariance));

			if (nIteration==1) break;

			for (int i=0; i<nSize; i++)
			{
				pResidual[i] = -1.0 * ( (pXValue[i]-dMean)*(pXValue[i]-dMean) / (dVariance*dVariance) );
				pResidual[i] = pYValue[i] - (dHeight*exp(pResidual[i]));
			}

			dCurValue = OneStepBiweightAlgorithm(pResidual, pWeight, nSize, dWeight);
		}
		else
		{
			delete [] pWeight;
			delete [] pResidual;
			delete [] pYLogValue;
			return 0;
		}

		if ( fabs(dPrevValue-dCurValue) < dStopThres ) break;

		dPrevValue = dCurValue;

	}

	pResult[0] = dHeight;
	pResult[1] = dMean;
	pResult[2] = dVariance;

	delete [] pWeight;
	delete [] pResidual;
	delete [] pYLogValue;

	return nReturnIteration + 1;
}

int CCHDataFitting::CircleFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorR)
{
	vectorR.clear();

	size_t nSize = vectorX.size();
	if (nSize < 3) return 0;

	double avg_x = 0;
	double avg_y = 0;
	for (size_t i=0; i<nSize; i++)
	{
		avg_x += vectorX[i];
		avg_y += vectorY[i];
	}
	avg_x /= double(nSize);
	avg_y /= double(nSize);

	double a[4] = {0, 0, 0, 0};
	double b[2] = {0, 0};
	double dx[2] = {0, 0};

	double Suu = 0;
	double Suv = 0;
	double Svv = 0;
	double Suuu = 0;
	double Svvv = 0;
	double Suvv = 0;
	double Suuv = 0;

	double ui, vi;
		for (size_t i=0; i<nSize; i++)
		{
		ui = vectorX[i] - avg_x;
		vi = vectorY[i] - avg_y;

		Suu += (ui*ui);
		Suv += (ui*vi);
		Svv += (vi*vi);

		Suuu += (ui*ui*ui);
		Svvv += (vi*vi*vi);

		Suvv += (ui*vi*vi);
		Suuv += (ui*ui*vi);
		}

	a[0] = Suu;	// Suu
	a[1] = Suv;	// Suv
	a[2] = Suv;	// Suv	
	a[3] = Svv;	// Svv

	b[0] = (Suuu+Suvv) / 2.0; 
	b[1] = (Svvv+Suuv) / 2.0;

	int nValue = SolveMatrix(a, b, dx, 2);

	if (nValue!=1) return 0;
			
	double alpha = (dx[0]*dx[0]) + (dx[1]*dx[1]) + ((a[0]+a[3])/double(nSize));

	vectorR.push_back(dx[0] + avg_x);
	vectorR.push_back(dx[1] + avg_y);
	vectorR.push_back(sqrt(alpha));

	return 1;
}

int CCHDataFitting::CircleFitting(double *pXValue, double *pYValue, int nSize, double *pResult)
{
	memset(pResult, 0, sizeof(double)*3);

	if (nSize < 3) return 0;

	double avg_x = 0;
	double avg_y = 0;
	for (int i=0; i<nSize; i++)
	{
		avg_x += pXValue[i];
		avg_y += pYValue[i];
	}
	avg_x /= double(nSize);
	avg_y /= double(nSize);

	double a[4] = {0, 0, 0, 0};
	double b[2] = {0, 0};
	double dx[2] = {0, 0};

	double Suu = 0;
	double Suv = 0;
	double Svv = 0;
	double Suuu = 0;
	double Svvv = 0;
	double Suvv = 0;
	double Suuv = 0;

	double ui, vi;
	for (size_t i=0; i<nSize; i++)
		{
		ui = pXValue[i] - avg_x;
		vi = pYValue[i] - avg_y;

		Suu += (ui*ui);
		Suv += (ui*vi);
		Svv += (vi*vi);

		Suuu += (ui*ui*ui);
		Svvv += (vi*vi*vi);

		Suvv += (ui*vi*vi);
		Suuv += (ui*ui*vi);
		}

	a[0] = Suu;	// Suu
	a[1] = Suv;	// Suv
	a[2] = Suv;	// Suv	
	a[3] = Svv;	// Svv

	b[0] = (Suuu+Suvv) / 2.0; 
	b[1] = (Svvv+Suuv) / 2.0;

	int nValue = SolveMatrix(a, b, dx, 2);

	if (nValue!=1) return 0;

	double alpha = (dx[0]*dx[0]) + (dx[1]*dx[1]) + ((a[0]+a[3])/double(nSize));

	pResult[0] = dx[0] + avg_x;
	pResult[0] = dx[1] + avg_y;
	pResult[0] = sqrt(alpha);

	return 1;
}

int CCHDataFitting::EllipseFitting(double *pXValue, double *pYValue, int nSize, double *pResult)
{
	if( nSize < 5 ) return 0;
	
	/*
	*  New fitellipse algorithm, contributed by Dr. Daniel Weiss
	*/
	cv::AutoBuffer<double> Ad, bd;
	double gfp[5], rp[5], t;
	CvMat A, b, x;
	const double min_eps = 1e-6;

	Ad.allocate(nSize*5);
	bd.allocate(nSize);

	// first fit for parameters A - E
	A = cvMat( nSize, 5, CV_64F, Ad );
	b = cvMat( nSize, 1, CV_64F, bd );
	x = cvMat( 5, 1, CV_64F, gfp );

	double avg_x = 0.0;
	double avg_y = 0.0;
	for (int i=0; i<nSize; i++)
	{
		avg_x += pXValue[i];
		avg_y += pYValue[i];
	}
	avg_x /= double(nSize);
	avg_y /= double(nSize);

	double val_x, val_y;
	for (int i=0; i<nSize; i++)
	{
		val_x = pXValue[i] - avg_x;
		val_y = pYValue[i] - avg_y;

		bd[i] = 10000.0; // 1.0?
		Ad[i*5] = -val_x * val_x;		// A - C signs inverted as proposed by APP
		Ad[i*5 + 1] = -val_y * val_y;
		Ad[i*5 + 2] = -val_x * val_y;
		Ad[i*5 + 3] = val_x;
		Ad[i*5 + 4] = val_y;
	}
	cvSolve( &A, &b, &x, CV_SVD );

	// now use general-form parameters A - E to find the ellipse center:
	// differentiate general form wrt x/y to get two equations for cx and cy
	A = cvMat( 2, 2, CV_64F, Ad );
	b = cvMat( 2, 1, CV_64F, bd );
	x = cvMat( 2, 1, CV_64F, rp );

	Ad[0] = 2 * gfp[0];
	Ad[1] = Ad[2] = gfp[2];
	Ad[3] = 2 * gfp[1];
	bd[0] = gfp[3];
	bd[1] = gfp[4];
	cvSolve( &A, &b, &x, CV_SVD );

	// re-fit for parameters A - C with those center coordinates
	A = cvMat( nSize, 3, CV_64F, Ad );
	b = cvMat( nSize, 1, CV_64F, bd );
	x = cvMat( 3, 1, CV_64F, gfp );

	for(int i=0; i<nSize; i++) 
	{
		val_x = pXValue[i] - avg_x;
		val_y = pYValue[i] - avg_y;

		bd[i] = 1.0;
		Ad[i * 3] = (val_x - rp[0]) * (val_x - rp[0]);
		Ad[i * 3 + 1] = (val_y - rp[1]) * (val_y - rp[1]);
		Ad[i * 3 + 2] = (val_x - rp[0]) * (val_y - rp[1]);
	}
	cvSolve(&A, &b, &x, CV_SVD);

	// store angle and radii
	rp[4] = -0.5 * atan2(gfp[2], gfp[1] - gfp[0]); // convert from APP angle usage
	t = sin(-2.0 * rp[4]);
	if( fabs(t) > fabs(gfp[2])*min_eps )
		t = gfp[2]/t;
	else
		t = gfp[1] - gfp[0];
	rp[2] = fabs(gfp[0] + gfp[1] - t);
	if( rp[2] > min_eps )
		rp[2] = sqrt(2.0 / rp[2]);
	rp[3] = fabs(gfp[0] + gfp[1] + t);
	if( rp[3] > min_eps )
		rp[3] = sqrt(2.0 / rp[3]);

	pResult[0] = rp[0] + avg_x;	// center x
	pResult[1] = rp[1] + avg_y;	// center y
	pResult[2] = rp[2]*2.0;		// size width
	pResult[3] = rp[3]*2.0;		// size height
	pResult[4] = 90.0 + rp[4]*180.0/CV_PI; // angle

	if( pResult[2] > pResult[3] )
	{
		double tmp = pResult[2];
		pResult[2] = pResult[3];
		pResult[3] = tmp;
	}

	if( pResult[4]< -180 ) pResult[4] += 360;
	if( pResult[4] > 360 ) pResult[4] -= 360;
	
	return 1;
}

int CCHDataFitting::EllipseFitting(VectorDouble& vectorX , VectorDouble& vectorY, VectorDouble& vectorResult)
{
	int n = (int) vectorX.size();

	if( n < 5 ) return 0;

	/*
	*  New fitellipse algorithm, contributed by Dr. Daniel Weiss
	*/
	cv::AutoBuffer<double> Ad, bd;
	double gfp[5], rp[5], t;
	CvMat A, b, x;
	const double min_eps = 1e-6;

	Ad.allocate(n*5);
	bd.allocate(n);

	// first fit for parameters A - E
	A = cvMat( n, 5, CV_64F, Ad );
	b = cvMat( n, 1, CV_64F, bd );
	x = cvMat( 5, 1, CV_64F, gfp );

	double avg_x = 0.0;
	double avg_y = 0.0;
	for(int i=0; i<n; i++) 
	{
		avg_x += vectorX[i];
		avg_y += vectorY[i];
	}
	avg_x /= double(n);
	avg_y /= double(n);

	double val_x, val_y;
	for(int i=0; i<n; i++)
	{
		val_x = vectorX[i] - avg_x;
		val_y = vectorY[i] - avg_y;

		bd[i] = 10000.0;				// 1.0?
		Ad[i*5] = -val_x * val_x;		// A - C signs inverted as proposed by APP
		Ad[i*5 + 1] = -val_y * val_y;
		Ad[i*5 + 2] = -val_x * val_y;
		Ad[i*5 + 3] = val_x;
		Ad[i*5 + 4] = val_y;
	}

	cvSolve( &A, &b, &x, CV_SVD );

	// now use general-form parameters A - E to find the ellipse center:
	// differentiate general form wrt x/y to get two equations for cx and cy
	A = cvMat( 2, 2, CV_64F, Ad );
	b = cvMat( 2, 1, CV_64F, bd );
	x = cvMat( 2, 1, CV_64F, rp );

	Ad[0] = 2 * gfp[0];
	Ad[1] = Ad[2] = gfp[2];
	Ad[3] = 2 * gfp[1];
	bd[0] = gfp[3];
	bd[1] = gfp[4];
	cvSolve( &A, &b, &x, CV_SVD );

	// re-fit for parameters A - C with those center coordinates
	A = cvMat( n, 3, CV_64F, Ad );
	b = cvMat( n, 1, CV_64F, bd );
	x = cvMat( 3, 1, CV_64F, gfp );

	for(int i=0; i<n; i++)
	{
		val_x = vectorX[i] - avg_x;
		val_y = vectorY[i] - avg_y;

		bd[i] = 1.0;
		Ad[i * 3] = (val_x - rp[0]) * (val_x - rp[0]);
		Ad[i * 3 + 1] = (val_y - rp[1]) * (val_y - rp[1]);
		Ad[i * 3 + 2] = (val_x - rp[0]) * (val_y - rp[1]);
	}
	cvSolve(&A, &b, &x, CV_SVD);

	// store angle and radii
	rp[4] = -0.5 * atan2(gfp[2], gfp[1] - gfp[0]); // convert from APP angle usage
	t = sin(-2.0 * rp[4]);
	if( fabs(t) > fabs(gfp[2])*min_eps )
	t = gfp[2]/t;
	else
	t = gfp[1] - gfp[0];
	rp[2] = fabs(gfp[0] + gfp[1] - t);
	if( rp[2] > min_eps )
	rp[2] = sqrt(2.0 / rp[2]);
	rp[3] = fabs(gfp[0] + gfp[1] + t);
	if( rp[3] > min_eps )
	rp[3] = sqrt(2.0 / rp[3]);


	vectorResult.clear();
	vectorResult.push_back(rp[0] + avg_x);	// center x
	vectorResult.push_back(rp[1] + avg_y);	// center y
	vectorResult.push_back(rp[2]*2.0);		// size width
	vectorResult.push_back(rp[3]*2.0);		// size height
	vectorResult.push_back(90.0 + rp[4]*180.0/CV_PI); // angle

	if( vectorResult[2] > vectorResult[3] )
	{
		double tmp = vectorResult[2];
		vectorResult[2] = vectorResult[3];
		vectorResult[3] = tmp;
	}

	if( vectorResult[4]< -180 ) vectorResult[4] += 360;
	if( vectorResult[4] > 360 ) vectorResult[4] -= 360;

	return 1;
}

int CCHDataFitting::IRLS_ParaboloidFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorZ, VectorDouble& vectorResult, int nIteration, double dWeight, double dStopThres)
{
	vectorResult.clear();

	int nSize = (int)vectorX.size();
	if (nSize<6) return 0;

	VectorDouble vectorWeight, vectorResidual;
	vectorWeight.resize(nSize);
	vectorResidual.resize(nSize);

	for (int i=0; i<nSize; i++)
	{
		vectorWeight[i] = 1.0;
	}

	double pX[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double pA[6] = {0, 0, 0, 0, 0, 0};
	double pY[6] = {0, 0, 0, 0, 0, 0};

	double dCurValue = 0.0;
	double dPrevValue = 0.0;

	int nReturnIteration = 0;
	for (nReturnIteration=0; nReturnIteration<nIteration; nReturnIteration++)
	{
		int nTemp = 1;
		for (int i=0; i<nSize; i++)
		{
			// 1
			pX[0] = (pX[0]*i + vectorWeight[i]) / nTemp; // 1
			pX[1] = (pX[1]*i + vectorWeight[i]*vectorY[i]) / nTemp; // y
			pX[2] = (pX[2]*i + vectorWeight[i]*vectorX[i]) / nTemp; // x
			pX[3] = (pX[3]*i + vectorWeight[i]*vectorY[i]*vectorY[i]) / nTemp; // yy
			pX[4] = (pX[4]*i + vectorWeight[i]*vectorX[i]*vectorY[i]) / nTemp; // xy
			pX[5] = (pX[5]*i + vectorWeight[i]*vectorX[i]*vectorX[i]) / nTemp; // xx
			// 2
			pX[9] = (pX[9]*i + vectorWeight[i]*vectorY[i]*vectorY[i]*vectorY[i]) / nTemp; // yyy
			pX[10] = (pX[10]*i + vectorWeight[i]*vectorX[i]*vectorY[i]*vectorY[i]) / nTemp; // xyy
			pX[11] = (pX[11]*i + vectorWeight[i]*vectorX[i]*vectorX[i]*vectorY[i]) / nTemp; // xxy
			// 3
			pX[17] = (pX[17]*i + vectorWeight[i]*vectorX[i]*vectorX[i]*vectorX[i]) / nTemp; // xxx
			// 4
			pX[21] = (pX[21]*i + vectorWeight[i]*vectorY[i]*vectorY[i]*vectorY[i]*vectorY[i]) / nTemp; // yyyy
			pX[22] = (pX[22]*i + vectorWeight[i]*vectorX[i]*vectorY[i]*vectorY[i]*vectorY[i]) / nTemp; // xyyy
			pX[23] = (pX[23]*i + vectorWeight[i]*vectorX[i]*vectorX[i]*vectorY[i]*vectorY[i]) / nTemp; // xxyy
			// 5
			pX[29] = (pX[29]*i + vectorWeight[i]*vectorX[i]*vectorX[i]*vectorX[i]*vectorY[i]) / nTemp; // xxxy
			// 6
			pX[35] = (pX[35]*i + vectorWeight[i]*vectorX[i]*vectorX[i]*vectorX[i]*vectorX[i]) / nTemp; // xxxx

			pY[0] = (pY[0]*i +  vectorWeight[i]*vectorZ[i]) / nTemp; // z
			pY[1] = (pY[1]*i +  vectorWeight[i]*vectorZ[i]*vectorY[i]) / nTemp; // zy
			pY[2] = (pY[2]*i +  vectorWeight[i]*vectorZ[i]*vectorX[i]) / nTemp; // zx
			pY[3] = (pY[3]*i +  vectorWeight[i]*vectorZ[i]*vectorY[i]*vectorY[i]) / nTemp; // zyy
			pY[4] = (pY[4]*i +  vectorWeight[i]*vectorZ[i]*vectorX[i]*vectorY[i]) / nTemp; // zxy
			pY[5] = (pY[5]*i +  vectorWeight[i]*vectorZ[i]*vectorX[i]*vectorX[i]) / nTemp; // zxx

			nTemp++;
		}
		// 2
		pX[6] = pX[1]; // y
		pX[7] = pX[3]; // yy
		pX[8] = pX[4]; // xy
		// 3
		pX[12] = pX[2]; // x
		pX[13] = pX[4]; // xy
		pX[14] = pX[5]; // xx
		pX[15] = pX[10]; // xyy
		pX[16] = pX[11]; // xxy
		// 4
		pX[18] = pX[3]; // yy
		pX[19] = pX[9]; // yyy
		pX[20] = pX[10]; // xyy
		// 5
		pX[24] = pX[4]; // xy
		pX[25] = pX[10]; // xyy
		pX[26] = pX[11]; // xxy
		pX[27] = pX[22]; // xyyy
		pX[28] = pX[23]; // xxyy
		// 6
		pX[30] = pX[5]; // xx
		pX[31] = pX[11]; // xxy
		pX[32] = pX[17]; // xxx
		pX[33] = pX[23]; // xxyy
		pX[34] = pX[29]; // xxxy

		if (SolveMatrix(pX, pY, pA, 6))
		{
			if (nIteration==1) break;

			for (int i=0; i<nSize; i++)
			{
				vectorResidual[i] = vectorZ[i] - (
					(pA[0]) + 
					(pA[1]*vectorY[i]) + 
					(pA[2]*vectorX[i]) + 
					(pA[3]*vectorY[i]*vectorY[i]) + 
					(pA[4]*vectorX[i]*vectorY[i]) + 
					(pA[5]*vectorX[i]*vectorX[i]) );
			}

			dCurValue = OneStepBiweightAlgorithm(vectorResidual, vectorWeight, dWeight);
		}
		else
		{
			return 0;
		}

		if ( fabs(dPrevValue-dCurValue) < dStopThres ) break;
		
		dPrevValue = dCurValue;
	}

	for (size_t i=0; i<6; i++)
	{
		vectorResult.push_back(pA[i]);
	}

	return nReturnIteration;
}

int CCHDataFitting::IRLS_ParaboloidFitting(double *pXValue, double *pYValue, double *pZValue, int nSize, double *pResult, int nIteration, double dWeight, double dStopThres)
{
	memset(pResult, 0, sizeof(double)*6);

	if (nSize<6) return 0;

	double *pWeight = new double[nSize];
	double *pResidual = new double[nSize];
	for (int i=0; i<nSize; i++)
	{
		pWeight[i] = 1.0;
	}

	double pX[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double pA[6] = {0, 0, 0, 0, 0, 0};
	double pY[6] = {0, 0, 0, 0, 0, 0};

	double dCurValue = 0.0;
	double dPrevValue = 0.0;

	int nReturnIteration = 0;
	for (nReturnIteration=0; nReturnIteration<nIteration; nReturnIteration++)
	{
		int nTemp = 1;
		for (int i=0; i<nSize; i++)
		{
			// 1
			pX[0] = (pX[0]*i + pWeight[i]) / nTemp; // 1
			pX[1] = (pX[1]*i + pWeight[i]*pYValue[i]) / nTemp; // y
			pX[2] = (pX[2]*i + pWeight[i]*pXValue[i]) / nTemp; // x
			pX[3] = (pX[3]*i + pWeight[i]*pYValue[i]*pYValue[i]) / nTemp; // yy
			pX[4] = (pX[4]*i + pWeight[i]*pXValue[i]*pYValue[i]) / nTemp; // xy
			pX[5] = (pX[5]*i + pWeight[i]*pXValue[i]*pXValue[i]) / nTemp; // xx
			// 2
			pX[9] = (pX[9]*i + pWeight[i]*pYValue[i]*pYValue[i]*pYValue[i]) / nTemp; // yyy
			pX[10] = (pX[10]*i + pWeight[i]*pXValue[i]*pYValue[i]*pYValue[i]) / nTemp; // xyy
			pX[11] = (pX[11]*i + pWeight[i]*pXValue[i]*pXValue[i]*pYValue[i]) / nTemp; // xxy
			// 3
			pX[17] = (pX[17]*i + pWeight[i]*pXValue[i]*pXValue[i]*pXValue[i]) / nTemp; // xxx
			// 4
			pX[21] = (pX[21]*i + pWeight[i]*pYValue[i]*pYValue[i]*pYValue[i]*pYValue[i]) / nTemp; // yyyy
			pX[22] = (pX[22]*i + pWeight[i]*pXValue[i]*pYValue[i]*pYValue[i]*pYValue[i]) / nTemp; // xyyy
			pX[23] = (pX[23]*i + pWeight[i]*pXValue[i]*pXValue[i]*pYValue[i]*pYValue[i]) / nTemp; // xxyy
			// 5
			pX[29] = (pX[29]*i + pWeight[i]*pXValue[i]*pXValue[i]*pXValue[i]*pYValue[i]) / nTemp; // xxxy
			// 6
			pX[35] = (pX[35]*i + pWeight[i]*pXValue[i]*pXValue[i]*pXValue[i]*pXValue[i]) / nTemp; // xxxx
			
			pY[0] = (pY[0]*i +  pWeight[i]*pZValue[i]) / nTemp; // z
			pY[1] = (pY[1]*i +  pWeight[i]*pZValue[i]*pYValue[i]) / nTemp; // zy
			pY[2] = (pY[2]*i +  pWeight[i]*pZValue[i]*pXValue[i]) / nTemp; // zx
			pY[3] = (pY[3]*i +  pWeight[i]*pZValue[i]*pYValue[i]*pYValue[i]) / nTemp; // zyy
			pY[4] = (pY[4]*i +  pWeight[i]*pZValue[i]*pXValue[i]*pYValue[i]) / nTemp; // zxy
			pY[5] = (pY[5]*i +  pWeight[i]*pZValue[i]*pXValue[i]*pXValue[i]) / nTemp; // zxx

			nTemp++;
		}
		// 2
		pX[6] = pX[1]; // y
		pX[7] = pX[3]; // yy
		pX[8] = pX[4]; // xy

		// 3
		pX[12] = pX[2]; // x
		pX[13] = pX[4]; // xy
		pX[14] = pX[5]; // xx
		pX[15] = pX[10]; // xyy
		pX[16] = pX[11]; // xxy

		// 4
		pX[18] = pX[3]; // yy
		pX[19] = pX[9]; // yyy
		pX[20] = pX[10]; // xyy

		// 5
		pX[24] = pX[4]; // xy
		pX[25] = pX[10]; // xyy
		pX[26] = pX[11]; // xxy
		pX[27] = pX[22]; // xyyy
		pX[28] = pX[23]; // xxyy

		// 6
		pX[30] = pX[5]; // xx
		pX[31] = pX[11]; // xxy
		pX[32] = pX[17]; // xxx
		pX[33] = pX[23]; // xxyy
		pX[34] = pX[29]; // xxxy

		if (SolveMatrix(pX, pY, pA, 6))
		{
			if (nIteration==1) break;

			for (int i=0; i<nSize; i++)
			{
				pResidual[i] = pZValue[i] - (
					(pA[0]) + 
					(pA[1]*pYValue[i]) + 
					(pA[2]*pXValue[i]) + 
					(pA[3]*pYValue[i]*pYValue[i]) + 
					(pA[4]*pXValue[i]*pYValue[i]) + 
					(pA[5]*pXValue[i]*pXValue[i]) );
			}

			dCurValue = OneStepBiweightAlgorithm(pResidual, pWeight, nSize, dWeight);
		}
		else
		{
			delete [] pWeight;
			delete [] pResidual;
			return 0;
		}

		if ( fabs(dPrevValue-dCurValue) < dStopThres ) break;
		
		dPrevValue = dCurValue;
	}

	// copy result
	memcpy(pResult, pA, sizeof(double)*6);

	delete [] pWeight;
	delete [] pResidual;

	return nReturnIteration + 1;
}

void AperB(double **_A, double **_B, double **_res, int _righA, int _colA, int _righB, int _colB) 
{
	int p,q,l;                                      
	for (p=1;p<=_righA;p++)       
	{
		for (q=1;q<=_colB;q++)                        
		{ 
			_res[p][q]=0.0;                            
			for (l=1;l<=_colA;l++)                     
			{
				_res[p][q]=_res[p][q]+_A[p][l]*_B[l][q];  
			}
		}                                            
	}
}     

void A_TperB(double **_A, double **_B, double **_res, int _righA, int _colA, int _righB, int _colB) 
{
	int p,q,l;                                      
	for (p=1;p<=_colA;p++)  
	{
		for (q=1;q<=_colB;q++)                        
		{ 
			_res[p][q]=0.0;                            
			for (l=1;l<=_righA;l++)  
			{
				_res[p][q]=_res[p][q]+_A[l][p]*_B[l][q];  
			}
		}                                            
	}
}

void AperB_T(double **_A, double **_B, double **_res, int _righA, int _colA, int _righB, int _colB) 
{
	int p,q,l;                                      
	for (p=1;p<=_colA;p++)     
	{
		for (q=1;q<=_colB;q++)                        
		{ 
			_res[p][q]=0.0;                            
			for (l=1;l<=_righA;l++)                    
			{
				_res[p][q]=_res[p][q]+_A[p][l]*_B[q][l];  
			}
		}                                            
	}
}

//  Perform the Cholesky decomposition    
// Return the lower triangular L  such that L*L'=A  
void choldc(double **a, int n, double **l)
{
	int i,j,k;
	double sum;
	double *p = new double[n+1];
	memset(p, 0, sizeof(double)*(n+1));

	for (i=1; i<=n; i++)  
	{
		for (j=i; j<=n; j++) 
		{
			for (sum=a[i][j],k=i-1;k>=1;k--) 
			{
				sum -= a[i][k]*a[j][k];
				if (sum<=0.0)
				{
					TRACE("");
				}

			}

			if (i == j) 
			{
				if (sum<=0.0)  
				{
					TRACE("");
				}
				else 
				{
					p[i]=sqrt(sum); 
				}
			}
			else 
			{
				a[j][i]=sum/p[i];
			}
		}
	}  

	for (i=1; i<=n; i++)  
	{
		for (j=i; j<=n; j++)  
		{
			if (i==j)
			{
				l[i][i] = p[i];
			}
			else
			{
				l[j][i]=a[j][i];  
				l[i][j]=0.0;
			}    
		}
	}

	delete [] p;
}

/********************************************************************/
/**    Calcola la inversa della matrice  B mettendo il risultato   **/
/**    in InvB . Il metodo usato per l'inversione e' quello di     **/
/**    Gauss-Jordan.   N e' l'ordine della matrice .               **/
/**    ritorna 0 se l'inversione  corretta altrimenti ritorna     **/
/**    SINGULAR .                                                  **/
/********************************************************************/
int inverse(double **TB, double **InvB, int N)
{  
	int k,i,j,p,q;
	double mult;
	double D,temp;
	double maxpivot;
	int npivot;

	double **B = new double* [N+1];
	double **A = new double* [N+1];
	double **C = new double* [N+1];

	for (int i=0; i<N+1; i++)
	{
		B[i] = new double [N+2];
		memset(B[i], 0, sizeof(double)*(N+2));

		A[i] = new double [2*N+2];
		memset(A[i], 0, sizeof(double)*(2*N+2));

		C[i] = new double [N+1];
		memset(C[i], 0, sizeof(double)*(N+1));
	}

	double eps = 10e-20;

	for(k=1;k<=N;k++)
	{
		for(j=1;j<=N;j++)
		{
			B[k][j]=TB[k][j];
		}
	}

	for (k=1;k<=N;k++)
	{
		for (j=1;j<=N+1;j++)
		{
			A[k][j]=B[k][j];
		}
		for (j=N+2;j<=2*N+1;j++)
		{
			A[k][j]=(double)0;
		}
		A[k][k-1+N+2]=(double)1;
	}

	for (k=1;k<=N;k++)
	{
		maxpivot=abs((double)A[k][k]);
		npivot=k;

		for (i=k;i<=N;i++)
		{
			if (maxpivot<abs((double)A[i][k]))
			{
				maxpivot=abs((double)A[i][k]);
				npivot=i;
			}

			if (maxpivot>=eps)
			{      
				if (npivot!=k)
				{
					for (j=k;j<=2*N+1;j++)
					{
						temp=A[npivot][j];
						A[npivot][j]=A[k][j];
						A[k][j]=temp;
					} 
				}

				D=A[k][k];
				for (j=2*N+1;j>=k;j--)
				{
					A[k][j]=A[k][j]/D;
				}

				for (i=1;i<=N;i++)
				{
					if (i!=k)
					{
						mult=A[i][k];
						for (j=2*N+1;j>=k;j--)
						{
							A[i][j]=A[i][j]-mult*A[k][j] ;
						}
					}
				}
			}
			else
			{  
				return -1;
			}
		}
	}

	/**   Copia il risultato nella matrice InvB  ***/
	for (k=1,p=1;k<=N;k++,p++)
	{
		for (j=N+2,q=1;j<=2*N+1;j++,q++)
		{
			InvB[p][q]=A[k][j];
		}
	}

	for (int i=0; i<N+1; i++)
	{
		delete [] B[i];
		delete [] A[i];
		delete [] C[i];
	}

	delete [] B;
	delete [] A;
	delete [] C;

	return 0;
}            /*  End of INVERSE   */

void ROTATE(double **a, int i, int j, int k, int l, double tau, double s) 
{
	double g,h;
	g = a[i][j];
	h = a[k][l];
	a[i][j] = g - s*(h+g*tau);
	a[k][l]=h+s*(g-h*tau);
}

void jacobi(double **a, int n, double *d , double **v, int nrot)      
{
	int j,iq,ip,i;
	double tresh,theta,tau,t,sm,s,h,g,c;

	double *b = new double[n+1];
	double *z = new double[n+1];

	memset(b, 0, sizeof(double)*(n+1));
	memset(z, 0, sizeof(double)*(n+1));

	for (ip=1;ip<=n;ip++) 
	{
		for (iq=1;iq<=n;iq++) 
		{
			v[ip][iq] = 0.0;
		}
		v[ip][ip] = 1.0;
	}

	for (ip=1;ip<=n;ip++) 
	{
		b[ip] = d[ip] = a[ip][ip];
		z[ip] = 0.0;
	}

	nrot=0;
	for (i=1;i<=50;i++) 
	{
		sm=0.0;
		for (ip=1;ip<=n-1;ip++) 
		{
			for (iq=ip+1;iq<=n;iq++)
			{
				sm += abs(a[ip][iq]);
			}
		}

		if (sm == 0.0) 
		{
			delete [] b;
			delete [] z;

			return;
		}

		if (i < 4)
		{
			tresh=0.2*sm/(n*n);
		}
		else
		{
			tresh=0.0;
		}

		for (ip=1;ip<=n-1;ip++) 
		{
			for (iq=ip+1;iq<=n;iq++) 
			{
				g=100.0*abs(a[ip][iq]);
				if (i > 4 && abs(d[ip])+g == abs(d[ip]) && abs(d[iq])+g == abs(d[iq]))
				{
					a[ip][iq]=0.0;
				}
				else if (abs(a[ip][iq]) > tresh) 
				{
					h=d[iq]-d[ip];
					if (abs(h)+g == abs(h))
					{
						t=(a[ip][iq])/h;
					}
					else 
					{
						theta=0.5*h/(a[ip][iq]);
						t=1.0/(abs(theta)+sqrt(1.0+theta*theta));
						if (theta < 0.0) 
						{
							t = -t;
						}
					}

					c=1.0/sqrt(1+t*t);
					s=t*c;
					tau=s/(1.0+c);
					h=t*a[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					d[ip] -= h;
					d[iq] += h;
					a[ip][iq]=0.0;

					for (j=1;j<=ip-1;j++) 
					{
						ROTATE(a,j,ip,j,iq,tau,s);
					}

					for (j=ip+1;j<=iq-1;j++) 
					{
						ROTATE(a,ip,j,j,iq,tau,s);
					}

					for (j=iq+1;j<=n;j++) 
					{
						ROTATE(a,ip,j,iq,j,tau,s);
					}

					for (j=1;j<=n;j++) {
						ROTATE(v,j,ip,j,iq,tau,s);
					}
					++nrot;
				}
			}
		}

		for (ip=1;ip<=n;ip++) 
		{
			b[ip] += z[ip];
			d[ip]=b[ip];
			z[ip]=0.0;
		}
	}

	delete [] b;
	delete [] z;

	//printf("Too many iterations in routine JACOBI");
}

int CCHDataFitting::EllipseFitting2(double *pXValue, double *pYValue, int nSize, double *pResult)
{
	if (nSize<6) return 0 ;

	double **D = new double*[nSize+1];
	for (int i=0; i<=nSize; i++)
	{			
		D[i] = new double[7];
		memset(D[i], 0, sizeof(double)*7);
	}

	double **S = new double*[7];
	double **L = new double*[7];
	double **invL = new double*[7];
	double **Const = new double*[7];
	double **temp = new double*[7];
	double **C = new double*[7];
	double **V = new double*[7];
	double **sol = new double*[7];

	double *d = new double[7];
	memset(d, 0, sizeof(double)*7);

	for (int i=0; i<7; i++)
	{			
		S[i] = new double[7];
		memset(S[i], 0, sizeof(double)*7);
		L[i] = new double[7];
		memset(L[i], 0, sizeof(double)*7);
		invL[i] = new double[7];
		memset(invL[i], 0, sizeof(double)*7);
		Const[i] = new double[7];
		memset(Const[i], 0, sizeof(double)*7);
		temp[i] = new double[7];
		memset(temp[i], 0, sizeof(double)*7);
		C[i] = new double[7];
		memset(C[i], 0, sizeof(double)*7);
		V[i] = new double[7];
		memset(V[i], 0, sizeof(double)*7);
		sol[i] = new double[7];
		memset(sol[i], 0, sizeof(double)*7);
	}

	double tx,ty;
	
	int nrot=0;
	int mode = 0;

	switch (mode) 
	{
	case 0:
		Const[1][3]=-2;
		Const[2][2]=1;
		Const[3][1]=-2;	
		break;

	case 1:
		Const[1][1]=2;
		Const[2][2]=1;
		Const[3][3]=2;	
		break;
	}


	// Now first fill design matrix
	for (int i=1; i <= nSize; i++)
	{ 
		tx = pXValue[i-1];
		ty = pYValue[i-1];
		D[i][1] = tx*tx;
		D[i][2] = tx*ty;
		D[i][3] = ty*ty;
		D[i][4] = tx;
		D[i][5] = ty;
		D[i][6] = 1.0;
	}

	//pm(Const,"Constraint");
	// Now compute scatter matrix  S
	A_TperB(D, D, S, nSize, 6, nSize, 6); // (A') * (B)
	//pm(S,"Scatter");

	choldc(S, 6, L);    
	//pm(L,"Cholesky");

	inverse(L, invL, 6);
	//pm(invL,"inverse");

	AperB_T(Const, invL, temp, 6, 6, 6, 6);

	AperB(invL, temp, C, 6, 6, 6, 6);
	//pm(C,"The C matrix");

	jacobi(C, 6, d, V, nrot);
	//pm(V,"The Eigenvectors");  /* OK */
	//pv(d,"The eigevalues");

	A_TperB(invL, V, sol, 6, 6, 6, 6);
	//pm(sol,"The GEV solution unnormalized");  /* SOl */

	// Now normalize them 
	for (int j=1;j<=6;j++)  /* Scan columns */
	{
		double mod = 0.0;
		for (int i=1;i<=6;i++)
		{
			mod += sol[i][j]*sol[i][j];
		}
		for (int i=1;i<=6;i++)
		{
			sol[i][j] /= sqrt(mod); 
		}
	}

	//pm(sol,"The GEV solution");  /* SOl */
	double zero=10e-20;
	double minev=10e+20;
	int  solind=0;
	
	switch (mode) 
	{
	case 0:
		for (int i=1; i<=6; i++)
		{
			if (d[i]<0.0 && abs(d[i])>zero)	
			{
				solind = i;
			}
		}
		break;
	case 1:  // smallest eigenvalue	 	   
		for (int i=1; i<=6; i++)
		{
			if (d[i]<minev && abs(d[i])>zero)	
			{
				solind = i;
			}
		}
		break;
	}

	// Now fetch the right solution
	for (int j=1;j<=6;j++)
	{
		pResult[j-1] = sol[j][solind];
	}

	for (int i=0; i<=nSize; i++)
	{
		delete [] D[i];
	}

	for (int i=0; i<7; i++)
	{
		delete [] S[i];
		delete [] L[i];
		delete [] invL[i];
		delete [] Const[i];
		delete [] temp[i];
		delete [] C[i];
		delete [] V[i];
		delete [] sol[i];
	}

	delete [] D;
	delete [] S;
	delete [] L;
	delete [] invL;
	delete [] Const;
	delete [] temp;
	delete [] C;
	delete [] V;
	delete [] sol;
	delete [] d;

	return 1;
}

int CCHDataFitting::EllipseFitting2(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorResult)
{
	int nSize = (int) vectorX.size();

	if (nSize<6) return 0 ;

	double **D = new double*[nSize+1];
	for (int i=0; i<=nSize; i++)
{
		D[i] = new double[7];
		memset(D[i], 0, sizeof(double)*7);
	}

	double **S = new double*[7];
	double **L = new double*[7];
	double **invL = new double*[7];
	double **Const = new double*[7];
	double **temp = new double*[7];
	double **C = new double*[7];
	double **V = new double*[7];
	double **sol = new double*[7];

	double *d = new double[7];
	memset(d, 0, sizeof(double)*7);

	for (int i=0; i<7; i++)
	{
		S[i] = new double[7];
		memset(S[i], 0, sizeof(double)*7);
		L[i] = new double[7];
		memset(L[i], 0, sizeof(double)*7);
		invL[i] = new double[7];
		memset(invL[i], 0, sizeof(double)*7);
		Const[i] = new double[7];
		memset(Const[i], 0, sizeof(double)*7);
		temp[i] = new double[7];
		memset(temp[i], 0, sizeof(double)*7);
		C[i] = new double[7];
		memset(C[i], 0, sizeof(double)*7);
		V[i] = new double[7];
		memset(V[i], 0, sizeof(double)*7);
		sol[i] = new double[7];
		memset(sol[i], 0, sizeof(double)*7);
	}

	double tx,ty;

	int nrot=0;
	int mode = 0;

	switch (mode) 
	{
	case 0:
		Const[1][3]=-2;
		Const[2][2]=1;
		Const[3][1]=-2;	
		break;

	case 1:
		Const[1][1]=2;
		Const[2][2]=1;
		Const[3][3]=2;	
		break;
	}


	// Now first fill design matrix
	for (int i=1; i <= nSize; i++)
	{ 
		tx = vectorX[i-1];
		ty = vectorY[i-1];
		D[i][1] = tx*tx;
		D[i][2] = tx*ty;
		D[i][3] = ty*ty;
		D[i][4] = tx;
		D[i][5] = ty;
		D[i][6] = 1.0;
	}

	//pm(Const,"Constraint");
	// Now compute scatter matrix  S
	A_TperB(D, D, S, nSize, 6, nSize, 6); // (A') * (B)
	//pm(S,"Scatter");

	choldc(S, 6, L);    
	//pm(L,"Cholesky");

	inverse(L, invL, 6);
	//pm(invL,"inverse");

	AperB_T(Const, invL, temp, 6, 6, 6, 6);

	AperB(invL, temp, C, 6, 6, 6, 6);
	//pm(C,"The C matrix");

	jacobi(C, 6, d, V, nrot);
	//pm(V,"The Eigenvectors");  /* OK */
	//pv(d,"The eigevalues");

	A_TperB(invL, V, sol, 6, 6, 6, 6);
	//pm(sol,"The GEV solution unnormalized");  /* SOl */

	// Now normalize them 
	for (int j=1;j<=6;j++)  /* Scan columns */
	{
		double mod = 0.0;
		for (int i=1;i<=6;i++)
		{
			mod += sol[i][j]*sol[i][j];
		}
		for (int i=1;i<=6;i++)
		{
			sol[i][j] /= sqrt(mod); 
		}
	}

	//pm(sol,"The GEV solution");  /* SOl */
	double zero=10e-20;
	double minev=10e+20;
	int  solind=0;

	switch (mode) 
	{
	case 0:
		for (int i=1; i<=6; i++)
		{
			if (d[i]<0.0 && abs(d[i])>zero)	
			{
				solind = i;
			}
		}
		break;
	case 1:  // smallest eigenvalue	 	   
		for (int i=1; i<=6; i++)
		{
			if (d[i]<minev && abs(d[i])>zero)	
			{
				solind = i;
			}
		}
		break;
	}

	// Now fetch the right solution
	vectorResult.clear();
	for (int j=1;j<=6;j++)
	{
		vectorResult.push_back(sol[j][solind]);
	}

	for (int i=0; i<=nSize; i++)
	{
		delete [] D[i];
	}

	for (int i=0; i<7; i++)
	{
		delete [] S[i];
		delete [] L[i];
		delete [] invL[i];
		delete [] Const[i];
		delete [] temp[i];
		delete [] C[i];
		delete [] V[i];
		delete [] sol[i];
	}

	delete [] D;
	delete [] S;
	delete [] L;
	delete [] invL;
	delete [] Const;
	delete [] temp;
	delete [] C;
	delete [] V;
	delete [] sol;
	delete [] d; 

	return 1;
}


int CCHDataFitting::Conic2Ellipse(double *conic_param, double *ellipse_param)
	{
	double a = conic_param[0];
	double b = conic_param[1];
	double c = conic_param[2];
	double d = conic_param[3];
	double e = conic_param[4];
	double f = conic_param[5];

	// get ellipse orientation
	double theta = 0.5 * atan2(b, a-c);

	// get scaled major/minor axes
	double ct = cos(theta);
	double st = sin(theta);
	double ap = a*ct*ct + b*ct*st + c*st*st;
	double cp = a*st*st + b*ct*st + c*ct*ct;

	// get translations
	double cx = (2.0*c*d - b*e) / (b*b - 4*a*c);
	double cy = (2.0*a*e - b*d) / (b*b - 4*a*c);

	// get scale factor
	double val = a*cx*cx + b*cx*cy + c*cy*cy;
	double scale_inv = val - f;

	if ((scale_inv/ap) <=0.0 || (scale_inv/cp) <=0.0)
		{
		return 0;
		}

// 	const double min_eps = 1e-6;
// 	double t = sin(2.0 * theta);
// 	if( fabs(t) > fabs(b)*min_eps )
// 		t = b / t;
// 	else
// 		t = a - c;
// 
// 	double min = fabs(c + a - t);
// 	if( min > min_eps )
// 		min = sqrt(2.0 / min);
// 
// 	double max = fabs(c + a + t);
// 	if( max > min_eps )
// 		max = sqrt(2.0 / max);
	
	ellipse_param[0] = sqrt(scale_inv / ap);
	ellipse_param[1] = sqrt(scale_inv / cp);
	ellipse_param[2] = cx;
	ellipse_param[3] = cy;
	ellipse_param[4] = theta;

	return 1;
	}

int CCHDataFitting::Conic2Ellipse(VectorDouble&  conic_param, VectorDouble& ellipse_param)
{
	if (conic_param.size()!=6) return 0;

	double a = conic_param[0];
	double b = conic_param[1];
	double c = conic_param[2];
	double d = conic_param[3];
	double e = conic_param[4];
	double f = conic_param[5];

	// get ellipse orientation
	double theta = 0.5 * atan2(b, a-c);

	// get scaled major/minor axes
	double ct = cos(theta);
	double st = sin(theta);
	double ap = a*ct*ct + b*ct*st + c*st*st;
	double cp = a*st*st + b*ct*st + c*ct*ct;

	// get translations
	double cx = (2.0*c*d - b*e) / (b*b - 4*a*c);
	double cy = (2.0*a*e - b*d) / (b*b - 4*a*c);

	// get scale factor
	double val = a*cx*cx + b*cx*cy + c*cy*cy;
	double scale_inv = val - f;

	if ((scale_inv/ap) <=0.0 || (scale_inv/cp) <=0.0)
	{
		return 0;
	}
// 	const double min_eps = 1e-6;
// 	double t = sin(2.0 * theta);
// 	if( fabs(t) > fabs(b)*min_eps )
// 		t = b / t;
// 	else
// 		t = a - c;
// 
// 	double min = fabs(c + a - t);
// 	if( min > min_eps )
// 		min = sqrt(2.0 / min);
// 
// 	double max = fabs(c + a + t);
// 	if( max > min_eps )
// 		max = sqrt(2.0 / max);

	ellipse_param.clear();
	ellipse_param.push_back(sqrt(scale_inv / ap));
	ellipse_param.push_back(sqrt(scale_inv / cp));
	ellipse_param.push_back(cx);
	ellipse_param.push_back(cy);
	ellipse_param.push_back(theta);

	return 1;
}