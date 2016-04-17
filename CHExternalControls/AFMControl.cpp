#include "StdAfx.h"
#include "AFMControl.h"

CAFMControl::CAFMControl(int nIndex, int nControlType)
{
	m_nIndex		= nIndex;
	m_nControlType	= nControlType;
	Reset();
}

CAFMControl::~CAFMControl(void)
{
	Reset();
}

void CAFMControl::Reset()
{
	m_pAFC2P = NULL;
	m_bConnected = FALSE;

	m_nStatus = 0;
	m_nRecipeStatus = 0;

	m_strRecipeName = _T("");
	m_nRecipeIndex = -1;
	m_nZoomIndex = -1;
	m_nTracking = 0;

	m_vecAFMRecipeInfo.clear();
}

BOOL CAFMControl::GetConnected()
{
	return m_bConnected;
}