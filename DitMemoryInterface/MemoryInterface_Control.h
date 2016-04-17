#pragma once
#include "memoryinterface.h"

class AFX_EXT_CLASS CMemoryInterface_Control : public CMemoryInterface
{
public:
	CMemoryInterface_Control(int nType, IMemoryInterface2Parent* pIMI2P=NULL, DWORD dwPeriod=50);
	virtual ~CMemoryInterface_Control(void);
};

