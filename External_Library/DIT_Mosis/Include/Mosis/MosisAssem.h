#pragma once

#include "AssemParam.h"
#include "BlobStorage.h"
#include "AssemUtil.h"

#include "MosisStrg.h"
#define ASSEM_CONVBUFFER_SIZE	1024*1024

class CBlobStorage;
class CMosisAssem;

class AFX_EXT_CLASS CAssemControl
{
	int				m_nThread;
	int				m_nSpace;
	CMosisAssem		*m_pMosisAssem;

public:
	CAssemControl();
	~CAssemControl();

public:
	int				InitAssemControl(int nThread, int maxDefect= 10000);
	void			ResetReinspectCount();

public:
	DIT_RESULT		Inspect_Rect(int iThread, CConvParam *pParam, BOOL bAsg= FALSE);//, class CDefectStorage *pDefectStorage, int iFrame, int iScan, int iModel, BOOL bCosmicFiler);
	DIT_RESULT		Inspect_Trapezium(int iThread, CConvParam *pParam);//, class CDefectStorage *pDefectStorage, int iFrame, int iScan, int iModel, BOOL bCosmicFiler);
	DIT_RESULT		Inspect_ASG(int iThread, CConvParam *pParam);
	DIT_RESULT		ASGVertConvolution(int iThread, CConvParam *pParam);

public:
	CMosisAssem*	GetMosisAssem(int iThread);
};

// CMosisAssem 내에서 주어진 검사 영역을 분할해 Thread 분할 검사를 실시한다.
class CAssemThreadControl
{
	int				m_nThread;
	int				m_nSpace;
	CMosisAssem		*m_pMosisAssem;

public:
	CAssemThreadControl();
	~CAssemThreadControl();
	int				GetThreadCount(){return m_nThread;}
	BOOL			InitAssemThreadControl(int nThread, int maxDefect);
	DIT_RESULT		Inspect_RectThread(int iThread, CConvParam *pParam);//, class CDefectStorage *pDefectStorage, int iFrame, int iScan, int iModel, BOOL bCosmicFiler);

	CMosisAssem*	GetMosisAssemThread(int iThread);
};

class AFX_EXT_CLASS CMosisAssem	 : public CBlobStorage
{
public:
	CAssemThreadControl	*m_pAssemControl;
	int		m_iThread;
	//	BOOL	m_bNowInspect;

	double	m_ConvTime;
	int		m_cReinspect;


protected:
	CMosisBlock<short>	m_ConvBlock;
	CMosisBlock<short>	m_ConvBlock2;

public:
	CMosisAssem(void);
	~CMosisAssem(void);

public:

	BOOL		InitAssem(int iThread, int maxDefect);
	void		ResetReinspectCount()	{m_cReinspect= 0;}
	void		ResetMosisAssem();	// Inspect_Pre(); 를 ResetMosisAssem()으로 명의변경한다.
	void		InspectPAD_Clear();

	BOOL		EnableInternalThread(int nThrea, int maxDefect);

public:		// 외부에서 검사시 호출하는 실제 인터페이스[일련의 검사 과정(Reset, Check Validity, Inspect, PostProcess..]을 내부에서 포함한다]
	DIT_RESULT	Inspect_Rect(CConvParam *pParam);
	DIT_RESULT	Inspect_RectThread(CConvParam *pParam);
	DIT_RESULT	Inspect_Trapezium(CConvParam *pParam);
	DIT_RESULT	Inspect_ASG(CConvParam *pParam);	// Asg에 Zone 검사가 추가되면서 Inspect 계열에 Inspect_ASG() 추가

public:
	void		CountReinspect()	{m_cReinspect++;}

public:
	DIT_RESULT	CheckParameter(BOOL &bNoInspect);
	DIT_RESULT	CheckParameter_Vert(BOOL &bNoInspect);

public:// Assem 호출을 통해 실제 검사 Control하는 함수들. Assem등 실제 검사 루틴을 추가 하려거든 이쪽 계열로 사용한다.
	int			Compare_Float();
	int			Compare_Pixel();
	void		Compare_Zone();			// Compare_Zone2(); 로 대체
	int			Compare_Zone2();		//  Zone 별 다이나믹 Threshold 추가된 버전.
	void		Compare_Trapezium();
	int			Compare_ASGZone();


protected:// 검사에 사용될 이미지를 위한 중간 버퍼 Control
	void		ResetConBuff(int &InspSize);			// 세로방향 비교를 위해 16배의 공간 사용
	short*		GetConBuffComp(){return m_ConvBlock.GetDataAddr();};
	short*		GetConBuffComp2(){return m_ConvBlock2.GetDataAddr();};



public:
	DIT_RESULT		ASGVertConvolution(CConvParam *pParam);


public:
	int			Inspect_Rect_Intrin(CConvParam &param);
};

