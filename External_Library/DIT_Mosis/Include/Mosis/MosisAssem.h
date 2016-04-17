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

// CMosisAssem ������ �־��� �˻� ������ ������ Thread ���� �˻縦 �ǽ��Ѵ�.
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
	void		ResetMosisAssem();	// Inspect_Pre(); �� ResetMosisAssem()���� ���Ǻ����Ѵ�.
	void		InspectPAD_Clear();

	BOOL		EnableInternalThread(int nThrea, int maxDefect);

public:		// �ܺο��� �˻�� ȣ���ϴ� ���� �������̽�[�Ϸ��� �˻� ����(Reset, Check Validity, Inspect, PostProcess..]�� ���ο��� �����Ѵ�]
	DIT_RESULT	Inspect_Rect(CConvParam *pParam);
	DIT_RESULT	Inspect_RectThread(CConvParam *pParam);
	DIT_RESULT	Inspect_Trapezium(CConvParam *pParam);
	DIT_RESULT	Inspect_ASG(CConvParam *pParam);	// Asg�� Zone �˻簡 �߰��Ǹ鼭 Inspect �迭�� Inspect_ASG() �߰�

public:
	void		CountReinspect()	{m_cReinspect++;}

public:
	DIT_RESULT	CheckParameter(BOOL &bNoInspect);
	DIT_RESULT	CheckParameter_Vert(BOOL &bNoInspect);

public:// Assem ȣ���� ���� ���� �˻� Control�ϴ� �Լ���. Assem�� ���� �˻� ��ƾ�� �߰� �Ϸ��ŵ� ���� �迭�� ����Ѵ�.
	int			Compare_Float();
	int			Compare_Pixel();
	void		Compare_Zone();			// Compare_Zone2(); �� ��ü
	int			Compare_Zone2();		//  Zone �� ���̳��� Threshold �߰��� ����.
	void		Compare_Trapezium();
	int			Compare_ASGZone();


protected:// �˻翡 ���� �̹����� ���� �߰� ���� Control
	void		ResetConBuff(int &InspSize);			// ���ι��� �񱳸� ���� 16���� ���� ���
	short*		GetConBuffComp(){return m_ConvBlock.GetDataAddr();};
	short*		GetConBuffComp2(){return m_ConvBlock2.GetDataAddr();};



public:
	DIT_RESULT		ASGVertConvolution(CConvParam *pParam);


public:
	int			Inspect_Rect_Intrin(CConvParam &param);
};

