#pragma once

#include <vector>
#include "MemoryData_Review.h"

#define MAX_MOTOR_ADDRESS_SIZE	100

using namespace MemoryInterface_Review;

typedef std::vector<CDefectData_Review>							VectorDefectResult;
typedef std::vector<CDefectData_Review>::iterator				VectorDefectResultIt;
typedef std::vector<CDefectData_Review>::const_iterator			constVectorDefectResultIt;

class CModuleResult
{
public:
	CModuleResult(int nModuleIdx=-1);
	~CModuleResult(void);
	void Reset();

	// check
	BOOL	CheckComplete() const;

	// getter 
	int		GetModuleIndex() const;
	int		GetLastResultIndex() const;
	int		GetStartResultIndex() const;
	int		GetEndResultIndex() const;
	DWORD	GetLastResultTick() const;

	float	GetTotalDurationTime() const;
	float	GetTotalDurationTime(int nIndex) const;
	float	GetDurationTime(int nIndex) const;
	float	GetDurationTime(int nStartIdx, int nEndIdx) const;

	int	GetResultCount(int nResultCode=ReviewResult_None) const;
	CDefectData_Review* GetResult(int nIndex);
	const CDefectData_Review* GetResult(int nIndex) const;

	// setter
	void	SetModuleIndex(int nIdx);
	void	SetLastResultIndex(int nIdx);
	void	SetStartResultIndex(int nIdx);
	void	SetEndResultIndex(int nIdx);
	void	SetLastResultTick(DWORD dwTick);

	// etc
	void	AddResult(const CDefectData_Review& result);
	void	ResizeResult(int nCount);

private:
	int					m_nModuleIndex;
	int					m_nLastResultIndex;
	int					m_nStartResultIndex;
	int					m_nEndResultIndex;
	DWORD				m_dwLastResultTick;
	VectorDefectResult	m_vecResultData;

};

