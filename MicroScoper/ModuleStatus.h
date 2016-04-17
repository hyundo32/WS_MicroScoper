#pragma once

#include <vector>

struct SCameraStatus
{
	SCameraStatus() 
	{
		bSelected			= FALSE;
		bAFMTracking		= FALSE;
		bDigitalZoom		= FALSE;
		nCameraControl		= 0;
		nMagnificLevel		= 0;
		strAFMRecipe		= _T("");
		nAFMRecipe			= 0;
		nAFMLevel			= 0;

		nLightLevel			= 0;
		nLightStatus		= 100;
		nLightLevelMin		= 0;
		nLightLevelMax		= 255;

		nExposureTime		= 500;		// usec
	}

	BOOL	bSelected;
	BOOL	bAFMTracking;
	BOOL	bDigitalZoom;
	int		nCameraControl;
	int		nMagnificLevel;

	CString	strAFMRecipe;
	int		nAFMRecipe;
	int		nAFMLevel;

	int		nLightStatus;
	int		nLightLevel;
	int		nLightLevelMin;
	int		nLightLevelMax;

	int		nExposureTime;
};

struct SCameraInfo
{
	SCameraInfo() 
	{
		nFrameWidth		= 0;
		nFrameHeight	= 0;
		nFrameChannels	= 0;

		nDirectionX		= 1;
		nDirectionY		= 1;

		dOffsetX		= 0.0;
		dOffsetY		= 0.0;

		dTriangleGab	= 100.0;	//
		dResolution		= 1.0;		// um/pixel
	}

	int		nFrameWidth;
	int		nFrameHeight;
	int		nFrameChannels;

	int		nDirectionX;
	int		nDirectionY;

	double	dOffsetX;
	double	dOffsetY;

	double	dTriangleGab;
	double	dResolution;

};
typedef std::vector<SCameraInfo>			VectorSCameraInfo;
typedef std::vector<SCameraInfo>::iterator	VectorSCameraInfoIt;

struct SMotorPosition
{
	SMotorPosition() 
	{
		dMotorPosX	= 0.0;
		dMotorPosY	= 0.0;
		dMotorPosZ	= 0.0;

		dGlassPosX	= 0.0;
		dGlassPosY	= 0.0;
		dGlassPosZ	= 0.0;
	}

	double	dMotorPosX;
	double	dMotorPosY;
	double	dMotorPosZ;

	double	dGlassPosX;
	double	dGlassPosY;
	double	dGlassPosZ;
};

struct SMotorStatus
{
	SMotorStatus() 
	{
		lMotorStatusX	= 0;
		lMotorStatusY	= 0;
		lMotorStatusSx	= 0;
		nBitCount		= 8;
	}

	BOOL AxisStatusX(int nIndex)
	{
		long nTemp = 1 << nIndex;
		return (lMotorStatusX & nTemp);
	}

	BOOL AxisStatusY(int nIndex)
	{
		long nTemp = 1 << nIndex;
		return (lMotorStatusY & nTemp);
	}

	BOOL AxisStatusSx(int nIndex)
	{
		long nTemp = 1 << nIndex;
		return (lMotorStatusSx & nTemp);
	}

	long	lMotorStatusX;
	long	lMotorStatusY;
	long	lMotorStatusSx;

	int		nBitCount;
};

struct SThetaMotorInfo
{
	SThetaMotorInfo() 
	{
		lStatus		= 0;
		dPosition	= 0.0;
	}

	BOOL AxisStatus(int nIndex)
	{
		long nTemp = 1 << nIndex;
		return (lStatus & nTemp);
	}

	long	lStatus;
	double	dPosition;
};

struct SAxisIndex
{
	SAxisIndex() 
	{
		nAxisIndexX		= -1;
		nAxisIndexY		= -1;
		nAxisIndexSx	= -1;
	}
	int		nAxisIndexX;
	int		nAxisIndexY;
	int		nAxisIndexSx;
};

struct SCoordInfo
{
	SCoordInfo() 
	{
		bStandard	= FALSE;
		nDirectionX	= 1;
		nDirectionY	= 1;
		dDistanceX	= 0.0;
		dDistanceY	= 0.0;
	}

	BOOL	bStandard;
	short	nDirectionX;
	short	nDirectionY;
	double	dDistanceX;
	double	dDistanceY;
};

struct SExternalIndex
{
	SExternalIndex()
	{
		nLight_Index		= -1;
		nLight_Channel		= 0;
		nAFM_Index			= -1;
		nAFM_Channel		= 0;
		nRevolver_Index		= -1;
		nRevolver_Channel	= 0;
		nWSI_Index			= -1;
		nWSI_Channel		= 0;
	}

	int nLight_Index;
	int nLight_Channel;
	int nAFM_Index;
	int nAFM_Channel;
	int nRevolver_Index;
	int nRevolver_Channel;
	int nWSI_Index;
	int nWSI_Channel;
};

struct SMagnificInfo
{
	SMagnificInfo()
	{
		strMagnificName		= _T("1.0X");
		dMagnification		= 1.0;
		dMagnificOffsetX	= 0.0;
		dMagnificOffsetY	= 0.0;
	}

	double	dMagnification;
	CString strMagnificName;
	double	dMagnificOffsetX;
	double	dMagnificOffsetY;
};
typedef std::vector<SMagnificInfo>					VectorSMagnificInfo;
typedef std::vector<SMagnificInfo>::iterator		VectorSMagnificInfoIt;
typedef std::vector<SMagnificInfo>::const_iterator	constVectorSMagnificInfoIt;

class CModuleStatus
{
public:
	CModuleStatus(int nType, int nLens, int nModuleIdx, int nGantryIdx, int nHeaderIdx);
	virtual ~CModuleStatus(void);

	void	SetStandard(BOOL bStandard)	{ sCoordInfo.bStandard = bStandard; }
	void	SetSelected(BOOL bSelected);	

	int		GetModuleType() const		{ return nModuleType; }
	int		GetLensType() const			{ return nLensType; }
	int		GetModuleIndex() const		{ return nModuleIndex; }
	int		GetGantryIndex() const		{ return nGantryIndex; }
	int		GetHeaderIndex() const		{ return nHeaderIndex; }
	BOOL	GetStandard() const			{ return sCoordInfo.bStandard; }
	BOOL	GetSelected() const			{ return sCameraStatus.bSelected; }

	int		GetCameraInfoCount() const;	
	int		GetMagnificInfoCount() const;

	SExternalIndex*				GetExternalIndex();
	SCameraStatus*				GetCameraStatus();
	SMotorPosition*				GetMotorPosition();
	SMotorStatus*				GetMotorStatus();
	SAxisIndex*					GetAxisIndex();
	SCoordInfo*					GetCoordInfo();
	SCameraInfo*				GetCameraInfo(int nIndex);
	SCameraInfo*				GetCurrentCameraInfo();
	SMagnificInfo*				GetCurrentMagnificInfo();
	SMagnificInfo*				GetMagnificInfo(int nIndex);
	VectorSCameraInfo*			GetVectorCameraInfo();
	VectorSMagnificInfo*		GetVectorSMagnificInfo();

	const SExternalIndex*		GetExternalIndex() const;
	const SCameraStatus*		GetCameraStatus() const;
	const SMotorPosition*		GetMotorPosition() const;
	const SMotorStatus*			GetMotorStatus() const;
	const SAxisIndex*			GetAxisIndex() const;
	const SCoordInfo*			GetCoordInfo() const;
	const SCameraInfo*			GetCameraInfo(int nIndex) const;
	const SCameraInfo*			GetCurrentCameraInfo() const;
	const SMagnificInfo*		GetMagnificInfo(int nIndex) const;
	const SMagnificInfo*		GetCurrentMagnificInfo() const;
	const VectorSCameraInfo*	GetVectorCameraInfo() const;
	const VectorSMagnificInfo*	GetVectorSMagnificInfo() const;


	void SetRevolverType(int nType);
	void SetExternalIndex(const SExternalIndex& externalIndex);
	void SetCameraStatus(const SCameraStatus& cameraStatus);
	void SetMotorPosition(const SMotorPosition& motorPosition);
	void SetMotorStatus(const SMotorStatus& motorStatus);
	void SetAxisIndex(const SAxisIndex& axisIndex);
	void SetCoordInfo(const SCoordInfo& coordInfo);
	void SetCameraInfo(int nIndex, const SCameraInfo& cameraInfo);
	void SetVectorCameraInfo(const VectorSCameraInfo& vectorCameraInfo);
	void SetMagnificInfo(int nIndex, const SMagnificInfo& magnificInfo);
	void SetVectorMagnificInfo(const VectorSMagnificInfo& vecMagnificInfo);

	void AddCameraInfo(const SCameraInfo& cameraInfo);
	void AddMagnificInfo(const SMagnificInfo& magnificInfo);

private:
	int					nModuleType;
	int					nLensType;
	int					nModuleIndex;
	int					nGantryIndex;
	int					nHeaderIndex;

	SExternalIndex		sExternalIndex;
	SCameraStatus		sCameraStatus;
	SMotorPosition		sMotorPosition;
	SMotorStatus		sMotorStatus;
	SAxisIndex			sAxisIndex;
	SCoordInfo			sCoordInfo;

	VectorSMagnificInfo	vecMagnificInfo;
	VectorSCameraInfo	vecCameraInfo;
};
typedef std::vector<CModuleStatus>					VectorModuleStatus;
typedef std::vector<CModuleStatus>::iterator		VectorModuleStatusIt;
typedef std::vector<CModuleStatus>::const_iterator	constVectorModuleStatusIt;