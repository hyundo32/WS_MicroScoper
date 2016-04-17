#pragma once


#include "AssemParam.h"
#include "AssemDefect.h"
#include "PixelStorage.h"

class AFX_EXT_CLASS CPairStorage : public CPixelStorage
{
public:
	CPairStorage(void);
	~CPairStorage(void);
	int				m_nPair;
	CDefectPair		*m_pPairDefect;

	int				m_maxPair;
	int				m_nPairSpace;

	long				*m_lFirstIndex;//[MAX_DEFECTPIX_NUM];
	BOOL				*m_bMerged;//[MAX_DEFECTPIX_NUM];
	BOOL				*m_bTemp;//[MAX_DEFECTPIX_NUM];

	int		InitPairStorage(int maxPair= 10000);
	void	ResetPairStorage();
	void	TotalResetPairStorage();// �ʱ�ȭ �ð� ������ ���� TotalReset �迭 �Լ� �߰�.
	int		GetPairCount() {return m_nPair;}
	CDefectPair*	GetPairDefect(int i){return m_pPairDefect+ i;}

	int		FalseFilter_Vert();
	int		FalseFilter_Hori();

	void	ShadowFly();	// UnpairDefect�� ShadowDefect�� ��� ������.
	void	Filtering_Vert();
	void	Filtering_Vert_Temp();
	void	Filtering_Hori();
	BOOL		CrossFilter_Hori(CDefectPair *pPair);
	BOOL		CrossFilter_Vert(CDefectPair *pPair);

	BOOL		JumpFilter_Hori(CDefectPair *pPair);
	BOOL		JumpFilter_Vert(CDefectPair *pPair);
	BOOL		OneFilter_Vert(CDefectPair *pPair);
	BOOL		EchoFilter_Vert(CDefectPair *pPair, int EchoTh);

	DIT_RESULT	PairingPlus_Pixel(double dPitchSize);
	DIT_RESULT	PairingMinus_Pixel(double dPitchsize);

	DIT_RESULT	PairingPlus_Float(double dPitchSize);
	DIT_RESULT	PairingMinus_Float(double dPitchsize);

	DIT_RESULT	PairingPlus_Vert(double dPitchSize);
	DIT_RESULT	PairingMinus_Vert(double dPitchsize);
};
