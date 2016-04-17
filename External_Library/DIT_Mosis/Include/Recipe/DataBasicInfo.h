// DataBasicInfo.h: interface for the CDataBasicInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASICINFO_H__3DA443DA_6AE4_483F_BC34_58D29D259D66__INCLUDED_)
#define AFX_DATABASICINFO_H__3DA443DA_6AE4_483F_BC34_58D29D259D66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	KEY_BASIC		"Software\\AOIServer\\BASIC\\"		// 레지스트리 접근 경로설정

typedef struct _INSPECTIONPROPERTY
{
	BOOL				s_bUse;
	InspectionMethod	s_InspectionMethod;
	CString				s_strMethodName;
	int					s_nConvUnitWidth;
	int					s_nConvUnitHeight;

	_INSPECTIONPROPERTY()
	{
		Reset();
	}
	void Reset()
	{
		s_bUse				= FALSE;
		s_InspectionMethod	= IM_Convolution;
		s_strMethodName		= _T("");
		s_nConvUnitWidth	= 2;
		s_nConvUnitHeight	= 3;
	}
} InspectionProperty, *pInspectionProperty;

typedef struct _CAMERABASICINFO
{
	int			m_nMarginLeft;		// 카메라별 왼쪽마진
	int			m_nMarginRight;		// 카메라별 오른쪽마진
	double		m_dResolution;		// 카메라별 해상도
	double		m_dAccumCoord;		// 누적 좌표값
	int			m_nStartLine;		// 전체 시작선에서 카메라 시작선까지의 거리.
	int			m_nIsFlip;			// 이미지를 가로방향으로 Flip할 것인지 체크. 0 - FALSE, 1 - TRUE
	int			m_nLightBarIdx;		// 카메라가 보고있는 수정봉의 인덱스.
	int			m_nStartLineOffset;		// 마스크 등 시작선에 대한 보정이필요한 경우의 값.
	int			m_nLEDControlID;	// LED Controller ID
	int			m_nLEDControlCH;	// LED Controller Channel

	_CAMERABASICINFO()
	{
		Reset();
	}
	void Reset()
	{
		m_nMarginLeft		= 0;
		m_nMarginRight		= 0;
		m_dResolution		= 0.0;
		m_dAccumCoord		= 0.0;
		m_nStartLine		= 0;
		m_nIsFlip			= 0;
		m_nLightBarIdx		= -1;
		m_nStartLineOffset	= 0;
		m_nLEDControlID		= -1;
		m_nLEDControlCH		= -1;
	}
	_CAMERABASICINFO(const _CAMERABASICINFO& rhs)
	{
		if (this != &rhs)
		{
			m_nMarginLeft		= rhs.m_nMarginLeft;
			m_nMarginRight		= rhs.m_nMarginRight;
			m_dResolution		= rhs.m_dResolution;
			m_dAccumCoord		= rhs.m_dAccumCoord;
			m_nStartLine		= rhs.m_nStartLine;
			m_nIsFlip			= rhs.m_nIsFlip;
			m_nLightBarIdx		= rhs.m_nLightBarIdx;
			m_nStartLineOffset	= rhs.m_nStartLineOffset;
			m_nLEDControlID		= rhs.m_nLEDControlID;
			m_nLEDControlCH		= rhs.m_nLEDControlCH;
		}
	}
	_CAMERABASICINFO& operator=(const _CAMERABASICINFO& rhs)
	{
		if (this != &rhs)
		{
			m_nMarginLeft		= rhs.m_nMarginLeft;
			m_nMarginRight		= rhs.m_nMarginRight;
			m_dResolution		= rhs.m_dResolution;
			m_dAccumCoord		= rhs.m_dAccumCoord;
			m_nStartLine		= rhs.m_nStartLine;
			m_nIsFlip			= rhs.m_nIsFlip;
			m_nLightBarIdx		= rhs.m_nLightBarIdx;
			m_nStartLineOffset	= rhs.m_nStartLineOffset;
			m_nLEDControlID		= rhs.m_nLEDControlID;
			m_nLEDControlCH		= rhs.m_nLEDControlCH;
		}
		return *this;
	}
} CameraBasicInfo, *pCameraBasicInfo;

class AFX_EXT_CLASS CDataBasicInfo
{
public:
	CDataBasicInfo();
	virtual ~CDataBasicInfo();

	BOOL				ReadRegisteryBasicInfo();
	BOOL				WriteRegisteryBasicInfo();

	BOOL				ReadFileBasicInfo(CString strFileName);
	BOOL				WriteFileBasicInfo(CString strFileName);

	//////////////////////////
	ClientSelect		GetClientSelect()		{ return m_ClientSelect; }

	RS232PortSelect		GetRS232PortSelect()	{ return m_RS232PortSelect; }
	PLCConnMethod		GetPLCConnMethod()		{ return m_PLCConnMethod; }
	RS232SpeedSelect	GetRS232SpeedSelect()	{ return m_RS232SpeedSelect; }
	DIOCardNumSelect	GetDIOCardNumSelect()	{ return m_DIOCardNumSelect; }
	DIOChanelNumSelect	GetDIOChanelNumSelect()	{ return m_DIOChanelNumSelect; }
	DIONameSlect		GetDIONameSelect()		{ return m_DIONameSelect; }

	int					GetRS232Port()			{ return m_nRS232Port; }
	int					GetRS232Speed()			{ return m_nRS232Speed; }
	int					GetDIOCardNum()			{ return m_nDIOCardNum; }
	int					GetDIOChanelNum()		{ return m_nDIOChanelNum; }
	CString				GetDIOName()			{ return m_strDIOName; }

	CString				GetStrLine()			{ return m_strLine; }
	CString				GetStrPhase()			{ return m_strPhase; }

	//////////////////////////
	ScanCoordinate		GetScanCoordination()	{ return m_ScanCoordination; }
	ScanDirectionEng	GetScanDirection()		{ return m_ScanDirection; }

	//////////////////////////
	int					GetInspMethodCount()	{ return m_nInspMethodCount; }
	int					GetInspMethodUse(int nIdx)
		{ return m_pInspProperty && nIdx >= 0 && nIdx < m_nInspMethodCount ? m_pInspProperty[nIdx].s_bUse : FALSE; }
	int					GetInspMethod(int nIdx)
		{ return m_pInspProperty && nIdx >= 0 && nIdx < m_nInspMethodCount ? m_pInspProperty[nIdx].s_InspectionMethod : IM_Convolution; }
	CString				GetInspMethodName(int nIdx)
		{ return m_pInspProperty && nIdx >= 0 && nIdx < m_nInspMethodCount ? m_pInspProperty[nIdx].s_strMethodName : ""; }
	int					GetInspMethodConvUnitWidth(int nIdx)
		{ return m_pInspProperty && nIdx >= 0 && nIdx < m_nInspMethodCount ? m_pInspProperty[nIdx].s_nConvUnitWidth : 2; }
	int					GetInspMethodConvUnitHeight(int nIdx)
		{ return m_pInspProperty && nIdx >= 0 && nIdx < m_nInspMethodCount ? m_pInspProperty[nIdx].s_nConvUnitHeight : 3; }

	//////////////////////////
	int					GetThicknessCount()		{ return m_nThicknessCount; }
	double				GetThickness(int nIdx)	{ return m_pdThickness ? m_pdThickness[nIdx] : 0.0; }

	//////////////////////////
	int					GetCameraNum()			{ return m_nCameraNum; }
	int					GetTCameraNum()			{ return m_nTCameraNum; }
	BOOL				GetUseZMotorT()			{ return m_bUseZMotorT; }
	BOOL				GetUseZMotorR()			{ return m_bUseZMotorR; }

	BOOL				GetAutoZMotor()			{ return m_bAutoZMotor; }
	int					GetRCameraNum()			{ return m_nRCameraNum; }
	double				GetTScanResolution()	{ return m_dTScanResolution; }
	double				GetRScanResolution()	{ return m_dRScanResolution; }
	int					GetFrameWidthTransfer()	{ return m_nFrameWidthTransfer; }
	int					GetFrameHeightTransfer(){ return m_nFrameHeightTransfer; }
	int					GetFrameWidthReflect()	{ return m_nFrameWidthReflect; }
	int					GetFrameHeightReflect()	{ return m_nFrameHeightReflect; }

	int					GetFrameDefectPixel()	{ return m_nFrameDefectPixel; }

	int					GetLightControlPort()	{ return m_nLightControlPort; }
	int					GetLightControlCount()	{ return m_nLightControlCount; }
	int					GetLightBarController(int nIdx)		{ return m_pnLightBarController ? m_pnLightBarController[nIdx] : -1; }
	int					GetLightTBarCount()		{ return m_nLightTBarCount; }
	int					GetLightRBarCount()		{ return m_nLightRBarCount; }
	int					GetLightTLampCount()	{ return m_nLightTLampCount; }
	int					GetLightRLampCount()	{ return m_nLightRLampCount; }

	int					GetLightDataCount()		{ return m_nLightDataCount; }
	int					GetLightData(int nIdx);


	//////////////////////////////////////////////////////////////////////////
	// LED Controller
	int					GetLEDLightControlPort() { return m_nLEDLightControlPort; }
	int					GetLEDLightControlCount() { return m_nLEDLightControlCount; }
	int					GetLEDLightCameraID(int iControlidx,int iLightCh);

	void				SetLEDLightControlPort(int nPort) { m_nLEDLightControlPort=nPort; }
	void				SetLEDLightControlCount(int nCount) { m_nLEDLightControlCount=nCount; }


	//////////////////////////
	int					GetScanCount()			{ return m_nScanCount; }
	ScanType			GetScanType()			{ return m_ScanType; }
	double				GetIndexTime()			{ return m_dIndexTime; }
	double				GetScanTime()			{ return m_dScanTime; }
	AcqMode				GetAcqMode()			{ return m_AcqMode; }
	int					GetTThresSuppress()		{ return m_nTThresSuppress; }
	int					GetRThresSuppress()		{ return m_nRThresSuppress; }
	int					GetTInsFrameNo()		{ return m_nTInsFrameNo; }
	int					GetRInsFrameNo()		{ return m_nRInsFrameNo; }
	int					GetThreadCount()		{ return m_nThreadCount; }
	int					GetGlassStartMargin()	{ return m_nGlassStartMargin; }
	int					GetGlassStartLine()		{ return m_nGlassStartLine; }
	BOOL				GetInspWithStartLine()	{ return m_bInspWithStartLine; }
	BOOL				GetInspInReview()		{ return m_bInReview; }
	int					GetConvMode()			{ return m_nConvMode; }
	BOOL				GetUnpairBlob()			{ return m_bUnpairBlob; }
	BOOL				GetInspWithZoneTh()		{ return m_bInspWithZoneTh; }
	BOOL				GetClassifyVert()		{ return m_bClassifyVert; }

	int					GetLightControllerType()		{ return m_nLightControllerType; }
	int					GetVCRPort()			{ return m_nVCRPort; }
	BOOL				GetUseDIO()				{ return m_bUseDIO; }
	int					GetTCPIPPort()			{ return m_nTCPIPPort; }
	BOOL				GetUseUmac()			{ return m_bUseUmac; }
	int					GetUmacPort()			{ return m_nUmacPort; }
	CString				GetTCPIPAddr()			{ return m_strTCPIPAddr; }

	//////////////////////////
	int					GetGlassTypeCount()		{ return m_nGlassTypeCount; }
	int					GetGlassSizeX(int nIdx)	{ return m_pGlassType ? m_pGlassType[nIdx].m_nGlassSizeX : 0; }
	int					GetGlassSizeY(int nIdx)	{ return m_pGlassType ? m_pGlassType[nIdx].m_nGlassSizeY : 0; }
	GlassDirection		GetGlassOriginDir(int nIdx)	{ return m_pGlassType ? m_pGlassType[nIdx].m_OriginDirection : GD_LeftTop; }
	GlassDirection		GetGlassCCutDir(int nIdx)	{ return m_pGlassType ? m_pGlassType[nIdx].m_CCutDirection : GD_LeftTop; }

	//////////////////////////
	int					GetAlignMarkNearX()		{ return m_nAlignMarkNearX; }
	int					GetAlignMarkNearY()		{ return m_nAlignMarkNearY; }
	int					GetAlignMarkDistX()		{ return m_nAlignMarkDistX; }
	int					GetAlignMarkDistY()		{ return m_nAlignMarkDistY; }
	int					GetAlignFindCamIdx()	{ return m_nAlignFindCamIdx; }
	int					GetAlignFindScanIdx()	{ return m_nAlignFindScanIdx; }
	BOOL				GetAlignFindImageFlip()	{ return m_bAlignFindImageFlip; }

	//////////////////////////
	int					GetTCameraMarginLeft(int nCamIdx, int nScanIdx);
	int					GetTCameraMarginRight(int nCamIdx, int nScanIdx);
	double				GetTCameraResolution(int nCamIdx, int nScanIdx);
	double				GetTCameraAccumCoord(int nCamIdx, int nScanIdx);
	int					GetTCameraStartLine(int nCamIdx, int nScanIdx);
	int					GetTCameraIsFlip(int nCamIdx, int nScanIdx);
	int					GetTCameraLightBarIdx(int nCamIdx, int nScanIdx);
	int					GetTCameraLEDLightControlID(int nCamIdx, int nScanIdx);
	int					GetTCameraLEDLightControlCH(int nCamIdx, int nScanIdx);
	//khs 080415-2
	int					GetTCameraStartLineOffset(int nCamIdx, int nScanIdx);


	int					GetRCameraMarginLeft(int nCamIdx, int nScanIdx);
	int					GetRCameraMarginRight(int nCamIdx, int nScanIdx);
	double				GetRCameraResolution(int nCamIdx, int nScanIdx);
	double				GetRCameraAccumCoord(int nCamIdx, int nScanIdx);
	int					GetRCameraStartLine(int nCamIdx, int nScanIdx);
	int					GetRCameraIsFlip(int nCamIdx, int nScanIdx);
	int					GetRCameraLightBarIdx(int nCamIdx, int nScanIdx);
	int					GetRCameraLEDLightControlID(int nCamIdx, int nScanIdx);
	int					GetRCameraLEDLightControlCH(int nCamIdx, int nScanIdx);

	//khs 080415-2
	int					GetRCameraStartLineOffset(int nCamIdx, int nScanIdx);

	CString				GetPwd1()				{ return m_strPwd1; }
	CString				GetPwd2()				{ return m_strPwd2; }
	
	int					GetTMidResolutionIdx();	// 투과 카메라 중 중간 해상도를 갖는 카메라 인덱스.
	int					GetRMidResolutionIdx();	// 반사 카메라 중 중간 해상도를 갖는 카메라 인덱스.
protected:
	void				ResetBaiscInfo();

	//////////////////////////////////////////////////////////////////////////
	// 고객 구분
	ClientSelect		m_ClientSelect;
	PLCConnMethod		m_PLCConnMethod;
	RS232PortSelect		m_RS232PortSelect;
	RS232SpeedSelect	m_RS232SpeedSelect;
	DIOCardNumSelect	m_DIOCardNumSelect;
	DIOChanelNumSelect	m_DIOChanelNumSelect;
	DIONameSlect		m_DIONameSelect;

	int					m_nRS232Port;
	int					m_nRS232Speed;
	int					m_nDIOCardNum;
	int					m_nDIOChanelNum;
	CString				m_strDIOName;

	//////////////////////////////////////////////////////////////////////////
	// 검사기 구분
	CString				m_strLine;
	CString				m_strPhase;

	//////////////////////////////////////////////////////////////////////////
	// Scan 방향.
	ScanCoordinate		m_ScanCoordination;
	ScanDirectionEng	m_ScanDirection;

	//////////////////////////////////////////////////////////////////////////
	// 검사방법
private:
	int					m_nInspMethodCount;
	pInspectionProperty	m_pInspProperty;


	//////////////////////////////////////////////////////////////////////////
	// Glass 두께
	int					m_nThicknessCount;
	double*				m_pdThickness;

	//////////////////////////////////////////////////////////////////////////
	// PC/카메라 설정
private:
	int					m_nCameraNum;
	int					m_nTCameraNum;
	int					m_nRCameraNum;

protected:
	BOOL				m_bUseZMotorT;
	BOOL				m_bUseZMotorR;
	BOOL				m_bAutoZMotor;
	
	double				m_dTScanResolution;
	double				m_dRScanResolution;

	int					m_nFrameWidthTransfer;
	int					m_nFrameHeightTransfer;
	int					m_nFrameWidthReflect;
	int					m_nFrameHeightReflect;

	int					m_nFrameDefectPixel;

private:
	//////////////////////////////////////////////////////////////////////////
	// LED Controller
	int					m_nLEDLightControlPort;		// LED Controller Port
	int					m_nLEDLightControlCount;	// LED Controller Count


	// 조명 관련 변수들
	int					m_nLightControlPort;	// 첫 번째 컨트롤러 RS232 연결 포트 
	int					m_nLightControlCount;	// 조명 컨트롤 수
	int					m_nLightTBarCount;		// 투과 수정봉 수
	int					m_nLightRBarCount;		// 반사 수정봉 수
	int					m_nLightTLampCount;		// 투과 수정봉 하나에 연결된 램프 수
	int					m_nLightRLampCount;		// 반사 수정봉 하나에 연결된 램프 수

	int*				m_pnLightBarController;	// 수정봉과 컨트롤러와의 상관관계. Array Index는 수정봉 Index.

	int					m_nLightDataCount;		// 조명 설정 데이터 수
	int*				m_pnLightData;			// aabbccdd : aa - LightBar, bb - Lamp, cc - Controller, dd - Channel

	//////////////////////////////////////////////////////////////////////////
	// 검사 관련 설정
private:
	int					m_nScanCount;
protected:
	ScanType			m_ScanType;
	double				m_dIndexTime;			// 전체 검사 시간.
	double				m_dScanTime;			// 한 번 스캔 검사 시간.

	AcqMode				m_AcqMode;
	int					m_nTThresSuppress;		// Image Suppress 반사 문턱값.
	int					m_nRThresSuppress;		// Image Suppress 투과 문턱값.
	int					m_nTInsFrameNo;			// 투과카메라 스캔 프레임 수.
	int					m_nRInsFrameNo;			// 반사카메라 스캔 프레임 수.
	int					m_nThreadCount;
	int					m_nGlassStartMargin;
	int					m_nGlassStartLine;		// 좌표계산에서 사용할 기준 시작선. 카메라마다 설정값은 Offset이다.
	BOOL				m_bInspWithStartLine;	// 설정된 시작선을 기준으로 연산.
	BOOL				m_bInReview;			// 설비내 리뷰기인가?
	int					m_nConvMode;			// Convolution Mode (0 - Float Pitch, 1 - Int Pitch)
	BOOL				m_bUnpairBlob;			// Unpair 결함 재구분 여부
	BOOL				m_bInspWithZoneTh;		// Zone Data가 있건없건 Zone Threshold로 검사.
	BOOL				m_bClassifyVert;		// 세로방향 비교 사용 여부

	int					m_nLightControllerType;
	int					m_nVCRPort;
	BOOL				m_bUseDIO;
	BOOL				m_bUseUmac;
	int					m_nUmacPort;
	int					m_nTCPIPPort;
	CString				m_strTCPIPAddr;

	//////////////////////////////////////////////////////////////////////////
	// Align Mark 위치 정보
	int					m_nAlignMarkNearX;
	int					m_nAlignMarkNearY;
	int					m_nAlignMarkDistX;
	int					m_nAlignMarkDistY;

	int					m_nAlignFindCamIdx;
	int					m_nAlignFindScanIdx;
	BOOL				m_bAlignFindImageFlip;
	
	//////////////////////////////////////////////////////////////////////////
	// Private 접근
	void				ResetInspMethod(int nInspIdx);
	void				SetInspMethodCount(int nCount);
	void				SetInspMethod(int nInspIdx, InspectionProperty* pMethod);

	void				SetThicknessCount(int nCount);
	void				SetThickness(int nIdx, double dValue);

	void				ResetGlassType(int nGlassTypeIdx);
	void				SetGlassTypeCount(int nCount);
	void				SetGlassSizeX(int nGlassTypeIdx, int nValue);
	void				SetGlassSizeY(int nGlassTypeIdx, int nValue);
	void				SetGlassOriginDir(int nGlassTypeIdx, GlassDirection GD);
	void				SetGlassCCutDir(int nGlassTypeIdx, GlassDirection GD);

	void				SetScanCount(int nScanCount);
	void				SetTCameraNum(int nTCameraNum);
	void				SetTCameraMarginLeft(int nCamIdx, int nScanIdx, int nValue);
	void				SetTCameraMarginRight(int nCamIdx, int nScanIdx, int nValue);
	void				SetTCameraResolution(int nCamIdx, int nScanIdx, double dValue);
	void				SetTCameraAccumCoord(int nCamIdx, int nScanIdx, double dValue);
	void				SetTCameraStartLine(int nCamIdx, int nScanIdx, int nValue);
	void				SetTCameraIsFlip(int nCamIdx, int nScanIdx, int nValue);
	void				SetTCameraLightBarIdx(int nCamIdx, int nScanIdx, int nValue);
	void				SetTCameraLEDLightControlID(int nCamIdx, int nScanIdx, int nValue);
	void				SetTCameraLEDLightControlCH(int nCamIdx, int nScanIdx, int nValue);

	//khs 080415-2
	void				SetTCameraStartLineOffset(int nCamIdx, int nScanIdx, int nValue);

	void				SetRCameraNum(int nRCameraNum);
	void				SetRCameraMarginLeft(int nCamIdx, int nScanIdx,int nValue);
	void				SetRCameraMarginRight(int nCamIdx, int nScanIdx,int nValue);
	void				SetRCameraResolution(int nCamIdx, int nScanIdx,double dValue);
	void				SetRCameraAccumCoord(int nCamIdx, int nScanIdx,double dValue);
	void				SetRCameraStartLine(int nCamIdx, int nScanIdx, int nValue);
	void				SetRCameraIsFlip(int nCamIdx, int nScanIdx, int nValue);
	void				SetRCameraLightBarIdx(int nCamIdx, int nScanIdx, int nValue);
	void				SetRCameraLEDLightControlID(int nCamIdx, int nScanIdx, int nValue);
	void				SetRCameraLEDLightControlCH(int nCamIdx, int nScanIdx, int nValue);

//khs 080415-2
	void				SetRCameraStartLineOffset(int nCamIdx, int nScanIdx, int nValue);

	void				SetPwd1(CString strPwd)				{ m_strPwd1 = strPwd; }
	void				SetPwd2(CString strPwd)				{ m_strPwd2 = strPwd; }

	void				SetLightControlPort(int nPort)		{ m_nLightControlPort = nPort; }
	void				SetLightControlCount(int nCount)	{ m_nLightControlCount = nCount; }
	void				SetLightBarCount(int nTCount, int nRCount);
	void				SetLightBarController(int nIdx, int nValue);
	void				SetLightTLampCount(int nCount)		{ m_nLightTLampCount = nCount; }
	void				SetLightRLampCount(int nCount)		{ m_nLightRLampCount = nCount; }

	int					GetTMainControl(int nLightBarIdx);

	void				SetLightDataCount(int nCount);
	void				SetLightData(int nIdx, int nData);

private:
	//////////////////////////////////////////////////////////////////////////
	// Glass Info
	int					m_nGlassTypeCount;
	pGlassType			m_pGlassType;

	//////////////////////////////////////////////////////////////////////////
	// Camera별 정보
	int					m_nTCameraArraySize;
	pCameraBasicInfo	m_pTCameraInfo;
	int					m_nRCameraArraySize;
	pCameraBasicInfo	m_pRCameraInfo;

	//////////////////////////////////////////////////////////////////////////
	// Password
	CString				m_strPwd1;
	CString				m_strPwd2;
};

#endif // !defined(AFX_DATABASICINFO_H__3DA443DA_6AE4_483F_BC34_58D29D259D66__INCLUDED_)
