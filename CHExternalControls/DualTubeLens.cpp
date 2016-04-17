#include "StdAfx.h"
#include "DualTubeLens.h"


CDualTubeLens::CDualTubeLens(int nIndex, int nControlType) : CRevolverControl(nIndex, nControlType)
{

}

CDualTubeLens::~CDualTubeLens(void)
{

}

BOOL CDualTubeLens::Connect(const CString& strPort, int nBaud)
{
	return TRUE;
}

void CDualTubeLens::Disconnect()
{

}

BOOL CDualTubeLens::Backward()
{
	return TRUE;
}

BOOL CDualTubeLens::Forward()
{
	return TRUE;
}

BOOL CDualTubeLens::GetPosition(int& nPos)
{
	return TRUE;
}

BOOL CDualTubeLens::SetPosition(int nPos)
{
	return TRUE;
}
