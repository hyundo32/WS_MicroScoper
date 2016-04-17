#pragma once

#include <vector>
typedef std::vector<double>						VectorDouble;
typedef std::vector<double>::iterator			VectorDoubleIt;
typedef std::vector<int>						VectorInteger;
typedef std::vector<int>::iterator				VectorIntegerIt;

enum Method_Type { Method_LU=0, Method_SVD, Method_SVD_SYM, Method_CHOLESKY, Method_QR, Method_NORMAL };

class AFX_EXT_CLASS CCHDataFitting
{
public:
	CCHDataFitting(void);
	~CCHDataFitting(void);

	// matrix function
	static int		SolveMatrix(double *pSrc, double *pSrc2Arg, double *pDst, int nSize, int nMethod=Method_LU);
	static double	InvertMatrix(double *pSrc, double *pDst, int nSize, int nMethod=Method_LU);
	static int		EigenVectorValue(double *pSrcMatrix, double *pEignVector, double *pEignValue, int nSize);
	static int		MatrixMultiply(double *pSrcA, double *pSrcB, double *pDst, int nRows, int nCols);
	static int		MatrixTranspose(double *pSource, double *pResult, int nRows, int nCols);

	// LS Fitting
	static int	CircleFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorResult);
	static int	CircleFitting(double *pXValue, double *pYValue, int nSize, double *pResult);
	static int	EllipseFitting(double *pXValue, double *pYValue, int nSize, double *pResult);
	static int	EllipseFitting(VectorDouble& vectorX , VectorDouble& vectorY, VectorDouble& vectorResult);

	// LS Fitting
	static int	PolynomialFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorResult, int nOrder);
	static int	PolynomialFitting(double *pXValue, double *pYValue, int nSize, double *pResult, int nOrder);
	static int	GaussianFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorResult);
	static int	GaussianFitting(double *pXValue, double *pYValue, int nSize, double *pResult);

	// IRLS Fitting
	static int IRLS_PolynomialFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorResult, int nOrder, int nIteration, double dWeight, double dStopThres=0.0001);
	static int IRLS_PolynomialFitting(double *pXValue, double *pYValue, int nSize, double *pResult, int nOrder, int nIteration, double dWeight, double dStopThres=0.0001);
	static int IRLS_GaussianFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorResult, int nIteration, double dWeight, double dStopThres=0.0001);
	static int IRLS_GaussianFitting(double *pXValue, double *pYValue, int nSize, double *pResult, int nIteration, double dWeight, double dStopThres=0.0001);

	static int IRLS_PlaneFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorZ, VectorDouble& vectorResult, int nIteration, double dWeight, double dStopThres=0.0001);
	static int IRLS_PlaneFitting(double *pXValue, double *pYValue, double *pZValue, int nSize, double *pResult, int nIteration, double dWeight, double dStopThres=0.0001);
	static int IRLS_ParaboloidFitting(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorZ, VectorDouble& vectorResult, int nIteration, double dWeight, double dStopThres=0.0001);
	static int IRLS_ParaboloidFitting(double *pXValue, double *pYValue, double *pZValue, int nSize, double *pResult, int nIteration, double dWeight, double dStopThres=0.0001);

	// weight function
	static double OneStepBiweightAlgorithm(double *pResidual, double *pWeight, int nSize, double dWeight=5.0, double dEpsilon=0.0001);
	static double OneStepBiweightAlgorithm(VectorDouble &vectorResidual, VectorDouble &vectorWeight, double dWeight=5.0, double dEpsilon=0.0001);


	// test function
	static int		EllipseFitting2(double *pXValue, double *pYValue, int nSize, double *pResult);
	static int		EllipseFitting2(VectorDouble& vectorX, VectorDouble& vectorY, VectorDouble& vectorResult);
	static int		Conic2Ellipse(double *conic_param, double *ellipse_param);
	static int		Conic2Ellipse(VectorDouble&  conic_param, VectorDouble& ellipse_param);

private:

};
