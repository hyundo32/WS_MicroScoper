#pragma once
#include "memoryinterface.h"

class AFX_EXT_CLASS CMemoryInterface_Aoi : public CMemoryInterface
{
public:
	CMemoryInterface_Aoi(int nType, IMemoryInterface2Parent* pIMI2P=NULL, DWORD dwPeriod=50);
	virtual ~CMemoryInterface_Aoi(void);
};

