#pragma once

#include "AssemDefect.h"
#include "PairStorage.h"


class AFX_EXT_CLASS CBlobStorage : public CPairStorage
{
public:
	CBlobStorage(void);
	~CBlobStorage(void);
	int				m_nBlob;
	CDefectBlob		*m_pBlobDefect;

	int				m_maxBlob;
	int				m_nBlobSpace;
	double			m_BlobTime;;


public:
	int		InitBlobStorage(int maxBlob= 10000);
	void	ResetBlobStorage()	;
	void	TotalResetBlobStorage()	;// 초기화 시간 단축을 위해 TotalReset 계열 함수 추가.
	int		GetBlobCount() {return m_nBlob;}
	CDefectBlob* GetBlobDefect(int i){return m_pBlobDefect+ i;}



public:	// Blobing 기본 인터페이스
	DIT_RESULT	BlobDefect();
	DIT_RESULT	BlobDefectVert();
	DIT_RESULT	BlobDefectVert(CPairStorage *pPairStrg);

	// Filtering 관련.

	void		ShadowFilter(int nStart, double xPitch);
	void		ShadowFilterVert(int nStart, double yPitch);

	int		GetConvolutionValue(int nX, int nY, double dDist, int nSuppress = 255);
	int		GetConvolutionValueVert(int nX, int nY, double dDist, int nSuppress = 255);
	int		GetAreaValue(int nXStart, int nYStart, int nWidth, int nHeight, double dDist, int nSuppress = 255);
	int		GetAreaValueVert(int nXStart, int nYStart, int nWidth, int nHeight, double dDist, int nSuppress = 255);

	// PostProcess 관련.
	void		CheckDefectRect(CDefectBlob& Defect, int nX, int nY);
	int			CalcRScale(CPoint* pVertex);

	BOOL		ReadBlob(CBlobStorage *pBlobStrg);
	BOOL		AddBlob(CDefectBlob *pBlob);

	// 다 지워라.
	DIT_RESULT	BlobDefect2();
	DIT_RESULT	BlobDefect_old();
	DIT_RESULT	BlobUnpairDefect();

	DIT_RESULT	MergeDivision(int nStart, int nMergePixel);
	DIT_RESULT	MergeMix(int nStart, int nMergePixel);
	DIT_RESULT	ClassificationDefect(int nStart);
	BOOL		ClassifyVertical(int nPairedIdx, int nThreshold);
	DIT_RESULT	ClassificationDefectVert(int nStart);

	long				m_nSkipUnpaired;
	long				m_nSkipClassify;
	long				m_nMoveClassify;
	long				m_nSkipMerged;

};
