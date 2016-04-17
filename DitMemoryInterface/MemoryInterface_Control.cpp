#include "StdAfx.h"
#include "MemoryInterface_Control.h"


CMemoryInterface_Control::CMemoryInterface_Control(int nType, IMemoryInterface2Parent* pIMI2P, DWORD dwPeriod) : CMemoryInterface(nType, pIMI2P, dwPeriod)
{
}


CMemoryInterface_Control::~CMemoryInterface_Control(void)
{
}
