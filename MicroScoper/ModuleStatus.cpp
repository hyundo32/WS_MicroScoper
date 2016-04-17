#include "StdAfx.h"
#include "ModuleStatus.h"


CModuleStatus::CModuleStatus(int nType, int nLens, int nModuleIdx, int nGantryIdx, int nHeaderIdx) :
	nModuleType(nType), nLensType(nLens), nModuleIndex(nModuleIdx), nGantryIndex(nGantryIdx), nHeaderIndex(nHeaderIdx)
{
}


CModuleStatus::~CModuleStatus(void)
{
}

void CModuleStatus::SetSelected(BOOL bSelected)
{ 
	sCameraStatus.bSelected = bSelected; 
}

void CModuleStatus::SetCameraStatus(const SCameraStatus& cameraStatus)
{
	sCameraStatus = cameraStatus;
}

void CModuleStatus::SetMotorPosition(const SMotorPosition& motorPosition)
{
	sMotorPosition = motorPosition;
}

void CModuleStatus::SetMotorStatus(const SMotorStatus& motorStatus)
{
	sMotorStatus = motorStatus;
}

void CModuleStatus::SetAxisIndex(const SAxisIndex& axisIndex)
{
	sAxisIndex = axisIndex;
}

void CModuleStatus::SetCoordInfo(const SCoordInfo& coordInfo)
{
	sCoordInfo = coordInfo;
}

void CModuleStatus::SetCameraInfo(int nIndex, const SCameraInfo& cameraInfo)
{
	if (nIndex<0 || nIndex>=vecCameraInfo.size())
	{
		return;
	}
	vecCameraInfo[nIndex] = cameraInfo;
}

void CModuleStatus::SetMagnificInfo(int nIndex, const SMagnificInfo& magnificInfo)
{
	if(nIndex < 0 || nIndex >= vecMagnificInfo.size())
	{
		return;
	}
	vecMagnificInfo[nIndex] = magnificInfo;
}

void CModuleStatus::SetVectorCameraInfo(const VectorSCameraInfo& vCameraInfo)
{
	vecCameraInfo = vCameraInfo;
}

void CModuleStatus::SetVectorMagnificInfo(const VectorSMagnificInfo& vMagnificInfo)
{
	vecMagnificInfo = vMagnificInfo;
}

SCameraStatus* CModuleStatus::GetCameraStatus()
{
	return &sCameraStatus;
}

SMotorPosition* CModuleStatus::GetMotorPosition()
{
	return &sMotorPosition;
}

SMotorStatus* CModuleStatus::GetMotorStatus()
{
	return &sMotorStatus;
}

SAxisIndex* CModuleStatus::GetAxisIndex()
{
	return &sAxisIndex;
}

SCoordInfo* CModuleStatus::GetCoordInfo()
{
	return &sCoordInfo;
}

SExternalIndex* CModuleStatus::GetExternalIndex()
{
	return &sExternalIndex;
}

SCameraInfo* CModuleStatus::GetCameraInfo(int nIndex)
{
	if (nIndex<0) return NULL;

	if (nLensType==0)
	{
		return &vecCameraInfo[0];
	}

	if (nIndex>=(int)vecCameraInfo.size())
	{
		return NULL;
	}

	return &vecCameraInfo[nIndex];
}

SMagnificInfo* CModuleStatus::GetMagnificInfo(int nIndex) 
{
	if (nIndex<0 || nIndex>=(int)vecMagnificInfo.size())
	{
		return NULL;
	}
	return &vecMagnificInfo[nIndex];
}

VectorSCameraInfo* CModuleStatus::GetVectorCameraInfo()
{
	return &vecCameraInfo;
}

VectorSMagnificInfo* CModuleStatus::GetVectorSMagnificInfo()
{
	return &vecMagnificInfo;
}

int CModuleStatus::GetCameraInfoCount() const
{
	return (int)vecCameraInfo.size();
}


int CModuleStatus::GetMagnificInfoCount() const
{
	return (int)vecMagnificInfo.size();
}

SCameraInfo* CModuleStatus::GetCurrentCameraInfo()
{
	return GetCameraInfo(sCameraStatus.nMagnificLevel);
}

SMagnificInfo* CModuleStatus::GetCurrentMagnificInfo()
{
	return GetMagnificInfo(sCameraStatus.nMagnificLevel);
}

const SMagnificInfo* CModuleStatus::GetCurrentMagnificInfo() const
{
	return GetMagnificInfo(sCameraStatus.nMagnificLevel);
}

const SExternalIndex* CModuleStatus::GetExternalIndex() const
{
	return &sExternalIndex;
}

const SCameraStatus* CModuleStatus::GetCameraStatus() const
{
	return &sCameraStatus;
}

const SMotorPosition* CModuleStatus::GetMotorPosition() const
{
	return &sMotorPosition;
}

const SMotorStatus* CModuleStatus::GetMotorStatus() const
{
	return &sMotorStatus;
}

const SAxisIndex* CModuleStatus::GetAxisIndex() const
{
	return &sAxisIndex;
}

const SCoordInfo* CModuleStatus::GetCoordInfo() const
{
	return &sCoordInfo;
}

const SCameraInfo* CModuleStatus::GetCameraInfo(int nIndex) const
{
	if (nIndex<0) return NULL;

	if (nLensType==0) // single tube
	{
		return &vecCameraInfo[0];
	}

	if (nIndex>=(int)vecCameraInfo.size())
	{
		return NULL;
	}

	return &vecCameraInfo[nIndex];
}

const SMagnificInfo* CModuleStatus::GetMagnificInfo(int nIndex) const
{
	if (nIndex<0 || nIndex>=(int)vecMagnificInfo.size())
	{
		return NULL;
	}
	return &vecMagnificInfo[nIndex];
}

const SCameraInfo* CModuleStatus::GetCurrentCameraInfo() const
{
	return GetCameraInfo(sCameraStatus.nMagnificLevel);
}

const VectorSCameraInfo* CModuleStatus::GetVectorCameraInfo() const
{
	return &vecCameraInfo;
}

const VectorSMagnificInfo* CModuleStatus::GetVectorSMagnificInfo() const
{
	return &vecMagnificInfo;
}

void CModuleStatus::AddCameraInfo(const SCameraInfo& cameraInfo)
{
	vecCameraInfo.push_back(cameraInfo);
}

void CModuleStatus::AddMagnificInfo(const SMagnificInfo& magnificInfo)
{
	vecMagnificInfo.push_back(magnificInfo);
}