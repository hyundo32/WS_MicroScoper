#pragma once

#include <vector>
#include "MemoryInterface_Aoi.h"
#include "MemoryInterface_Review.h"
#include "MemoryInterface_Control.h"

#include "ModuleStatus.h"

class CGlassResult;
typedef std::vector<CGlassResult*>					VectorGlassResult;
typedef std::vector<CGlassResult*>::iterator		VectorGlassResultIt;
typedef std::vector<CGlassResult*>::const_iterator	constVectorGlassResultIt;

class CMainData : public IMemoryInterface2Parent
{
public:
	CMainData(void);
	virtual ~CMainData(void);

	// interface memory callback
	virtual void IMI2P_SendSignalValue(int nType, int nSignalType, int nSignalIndex, int nValue);
	
	// glass result
	CGlassResult*			GlassResult_GetNewGlassResult(int& nTotalCount);		
	CGlassResult*			GlassResult_GetGlassResult(int nIndex=-1);
	int						GlassResult_GetCurrentGlassResultIndex();
	CGlassResult*			GlassResult_GetCurrentGlassResult();
	int						GlassResult_GetTotalGlassResultCount();
	int						GlassResult_GetGlassResultCount();

	// module status
	int						ModuleStatus_GetModuleStatusCount(int nModuleType=0) const;
	CModuleStatus*			ModuleStatus_GetModuleStatus(int nModuleIdx);
	SExternalIndex*			ModuleStatus_GetExternalIndex(int nModuleIdx);
	SCameraStatus*			ModuleStatus_GetCameraStatus(int nModuleIdx);
	SMotorPosition*			ModuleStatus_GetMotorPosition(int nModuleIdx);
	SMotorStatus*			ModuleStatus_GetMotorStatus(int nModuleIdx);
	SAxisIndex*				ModuleStatus_GetAxisIndex(int nModuleIdx);
	SCoordInfo*				ModuleStatus_GetCoordInfo(int nModuleIdx);
	SCameraInfo*			ModuleStatus_GetCurrentCameraInfo(int nModuleIdx);
	SCameraInfo*			ModuleStatus_GetCameraInfo(int nModuleIdx, int nMagIdx=0);
	SMagnificInfo*			ModuleStatus_GetCurrentMagnificInfo(int nModuleIdx);
	SMagnificInfo*			ModuleStatus_GetMagnificInfo(int nModuleIdx, int nMagIdx=0);	
		
	// standard module status
	int						ModuleStatus_GetStandardModuleIndex() const;
	CModuleStatus*			ModuleStatus_GetStandardModuleStatus();
	SMotorPosition*			ModuleStatus_GetStandardMotorPosition();
	SAxisIndex*				ModuleStatus_GetStandardAxisIndex();

	// selected module status
	int						ModuleStatus_GetSelectedModuleIndex() const;
	CModuleStatus*			ModuleStatus_GetSelectedModuleStatus();
	SMotorPosition*			ModuleStatus_GetSelectedMotorPosition();
	SAxisIndex*				ModuleStatus_GetSelectedAxisIndex();
	
private:
	void	InitData();
	void	DeinitData();

	// Aoi interface memory
	int		InitAoiInterfaceMemory();
	void	DeinitAoiInterfaceMemory();

	// review interface memory
	int		InitReviewInterfaceMemory();
	void	DeinitReviewInterfaceMemory();

	// control interface memory
	int		InitControlInterfaceMemory();
	void	DeinitControlInterfaceMemory();

	// glass result
	int		InitGlassResult();
	void	DeinitGlassResult();

	// module status
	int		InitModuleStatus();
	void	DeinitModuleStatus();

	// process signal
	void	ProcessSignal_Aoi(int nSignalType, int nSignalIndex, int nValue);
	void	ProcessSignal_Review(int nSignalType, int nSignalIndex, int nValue);
	void	ProcessSignal_Control(int nSignalType, int nSignalIndex, int nValue);

private:
	// interface memory
	CMemoryInterface_Aoi*		m_pAoiMemory;
	CMemoryInterface_Review*	m_pReviewMemory;
	CMemoryInterface_Control*	m_pControlMemory;

	// glass result
	int							m_nTotalGlassResultCount;
	int							m_nCurrentGlassResultIndex;
	CGlassResult*				m_pCurrentGlassResult;
	VectorGlassResult			m_vecGlassResult;

	// module status
	VectorModuleStatus			m_vecModuleStatus;

	// critical section
	CRITICAL_SECTION			m_csResultData;			

};

