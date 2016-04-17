#pragma once

#include "GlassRecipe.h"

#define			RECIPE_MAX_SCAN_COUNT		3
#define			GLASS_PACKET_SIZE		10240

typedef struct _SCAN_RECIPE_PARM
{
	char				m_strHpanelID[MAX_STRING_LENGTH];
	char				m_strProcessID[MAX_STRING_LENGTH];
	char				m_strPPID[MAX_STRING_LENGTH];	
	CameraType			m_nCamType;
	BOOL				m_bInspRegiStartLine;
	int					m_nFrameWidth;
	int					m_nFrameHeight;
	int					m_nInspRegiStartLine[RECIPE_MAX_SCAN_COUNT];
	int					m_nStartLineOffset[RECIPE_MAX_SCAN_COUNT];
	double				m_dConvResolution[RECIPE_MAX_SCAN_COUNT];
	double				m_dScanResolution;
	int					m_nMarginLeft[RECIPE_MAX_SCAN_COUNT];
	int					m_nMarginRight[RECIPE_MAX_SCAN_COUNT];
	int					m_nAccumCoordConv[RECIPE_MAX_SCAN_COUNT];	

	int					m_nScanCount;
	ScanDirectionIns	m_nFirstScanDir;
	int					m_nThresSuppress;
	int					m_nSideThreshold;

	int					m_nAlignMarkNearX;
	int					m_nAlignMarkNearY;
	int					m_nAlignMarkDistX;
	int					m_nAlignMarkDistY;

	int					m_nAlignHSize;
	int					m_nAlignVSize;
	int					m_nAlignFindWidth;
	int					m_nAlignFindHeight;
	int					m_nAlignFindMethod;
	int					m_nAlignMatchingRate;

	int					m_nFindPatternWidth;
	int					m_nFindPatternHeight;
	int					m_nFindSideWidth;
	int					m_nFindSideHeight;
	int					m_nFindSideCycle;
	int					m_nFindSideSubRange;

	BOOL				m_bDISaveBlack;
	BOOL				m_bDISaveWhite;
	BOOL				m_bFilterCosmicRay;
	int					m_nFilterCosmicRayTH;

	BOOL				m_bUseShrink;
	int					m_nShrinkFrameLimit;
	int					m_nShrinkFrameMin;

	BOOL				m_bGetSampleImage;
	BOOL				m_bEdgeCrackVert;
	BOOL				m_bEdgeCrackHor;

	BOOL				m_bSavePADImg;

	_SCAN_RECIPE_PARM()
	{
		Reset();
	}
	void ResetCamType()
	{
		m_nCamType = CamType_Unknown;
		m_dScanResolution = 0.;	
		m_nFrameWidth = 0;
		m_nFrameHeight = 0;
		m_bEdgeCrackVert = FALSE;
		m_bEdgeCrackHor = FALSE;
		m_nThresSuppress = 255;
		m_nSideThreshold = 0;
		ZeroMemory(m_nInspRegiStartLine,sizeof(int)*RECIPE_MAX_SCAN_COUNT);	
		ZeroMemory(m_nStartLineOffset,sizeof(int)*RECIPE_MAX_SCAN_COUNT);	
		ZeroMemory(m_dConvResolution,sizeof(double)*RECIPE_MAX_SCAN_COUNT);	
		ZeroMemory(m_nMarginLeft,sizeof(int)*RECIPE_MAX_SCAN_COUNT);	
		ZeroMemory(m_nMarginRight,sizeof(int)*RECIPE_MAX_SCAN_COUNT);	
		ZeroMemory(m_nAccumCoordConv,sizeof(int)*RECIPE_MAX_SCAN_COUNT);
	}
	void Reset()
	{
		ResetCamType();

		ZeroMemory(m_strHpanelID,sizeof(char)*MAX_STRING_LENGTH);
		ZeroMemory(m_strProcessID,sizeof(char)*MAX_STRING_LENGTH);
		ZeroMemory(m_strPPID,sizeof(char)*MAX_STRING_LENGTH);

		m_nScanCount = 0;
		m_nFirstScanDir = SD_Unknown;	
		m_nAlignMarkNearX = 0;
		m_nAlignMarkNearY = 0;
		m_nAlignMarkDistX = 0;
		m_nAlignMarkDistY = 0;
		m_nAlignHSize = 0;
		m_nAlignVSize = 0;
		m_nAlignFindWidth = 0;
		m_nAlignFindHeight = 0;
		m_nAlignFindMethod = 0;
		m_nAlignMatchingRate = 0;
		m_nFindPatternWidth = 0;
		m_nFindPatternHeight = 0;
		m_nFindSideWidth = 0;
		m_nFindSideHeight = 0;
		m_nFindSideCycle = 0;
		m_nFindSideSubRange = 0;
		m_bDISaveBlack = FALSE;
		m_bDISaveWhite = FALSE;
		m_bFilterCosmicRay = FALSE;
		m_nFilterCosmicRayTH = 0;
		m_bUseShrink = FALSE;
		m_nShrinkFrameLimit = 0;
		m_nShrinkFrameMin = 0;
		m_bGetSampleImage = FALSE;		
		m_bSavePADImg = FALSE;
	}	
} SCAN_RECIPE_PARM, *pSCAN_RECIPE_PARM;

class AFX_EXT_CLASS CScanRecipe
{
public:
	CScanRecipe(void);
	virtual ~CScanRecipe(void);

public:
	void				Reset();
	void				ResetCamType();
	void				SetScanCount(int nCount);
	int					GetScanCount(){return m_pScanRecipePrm.m_nScanCount;}
	void				SetHPanelID(CString strName){CopyMemory(m_pScanRecipePrm.m_strHpanelID,strName.GetBuffer(0),sizeof(char)*MAX_STRING_LENGTH);}
	void				SetProcessID(CString strName){CopyMemory(m_pScanRecipePrm.m_strProcessID,strName.GetBuffer(0),sizeof(char)*MAX_STRING_LENGTH);}
	void				SetPPID(CString strName){CopyMemory(m_pScanRecipePrm.m_strPPID,strName.GetBuffer(0),sizeof(char)*MAX_STRING_LENGTH);}
	CString				GetHPanelID();
	CString				GetProcessID();
	CString				GetPPID();
	BOOL				MakePacket(CGlassRecipe *pGlassRecipe);
	BOOL				ReadPacket(CMosisPacket *pPacket,CGlassRecipe *pGlassRecipe);

	BYTE				*GetPacketBuffer(){return m_pGlassPacket.GetDataAddress();}
	int					GetPacketSize(){return m_pGlassPacket.GetSize();}

	SCAN_RECIPE_PARM	*GetScanRecipePrm(){return &m_pScanRecipePrm;}

protected:
	BOOL				WriteRecipe(CMosisPacket *pPacket);
	BOOL				ReadRecipe(CMosisPacket *pPacket);


protected:
	CMosisPacket		m_pGlassPacket;
	SCAN_RECIPE_PARM	m_pScanRecipePrm;


};
