#include "StdAfx.h"
#include "PLCControl.h"

CPLCControl::CPLCControl(DWORD dwPeriod) : CTimerThreadPools(dwPeriod)
{
	m_nPrevPLCSignal_01 = m_nReadPLCSignal_01 = 0;
	m_nWPrevPLCSignal_01 = m_nWReadPLCSignal_01 = 0;

	m_nDelayTime		= 50;
	m_nSendAddress		= 6000;
}

CPLCControl::~CPLCControl(void)
{

}

int CPLCControl::MakePLCString(char *Destination, unsigned char *Source, int nSize)
{
	int nStringLen = nSize;
	if (nSize % 2 == 1)
		nStringLen = nSize + 1;

	int			i;
	for (i = 0; i < nStringLen; i++)
	{
		if ((i / 2) * 2 + ((i + 1) % 2) < nSize)
			sprintf(Destination + i * 2, "%02X", *(Source + (i / 2) * 2 + ((i + 1) % 2)));
		else
			sprintf(Destination + i * 2, "%02X", ' ');
	}

	return i;
}

CString CPLCControl::GetDataWord2Str(CString& value, int& addr, int length)
{
	CString tmp(""),Rstr(""),Fstr(""),buff(""),returnValue("");
	int i;
	char *p;	
	int		nData;
	CString strData;

	if (value.GetLength() >= addr * 2 + length * 2)
	{
		tmp = value.Mid(addr * 2, length * 2);

		for (i = 0; i < tmp.GetLength(); i = i + 2)
		{		
			nData = __toascii(strtol((LPSTR)(LPCTSTR)(tmp.Mid(i, 2)), &p, 16));
			strData.Format(_T("%d"), nData);
			Rstr += strData;
		}

		for(i = 0; i < Rstr.GetLength()-1; i = i + 2)
		{
			Fstr += Rstr.Mid(i+1, 1);
			Fstr += Rstr.Mid(i, 1);
		}
		if (Rstr.GetLength() % 2 == 1)
			Fstr += Rstr.Mid(Rstr.GetLength(),1);

		int k = Fstr.GetLength();

		//공백을 없앤다
		for(i=0;i<Fstr.GetLength();i++)
		{
			if(Fstr.Mid(i,1) != " ")
				returnValue += Fstr.Mid(i,1);
		}

		k = returnValue.GetLength();
		//아무런 data도 없으면 "*" 처리한다
		if(returnValue.GetLength() == 0)
			returnValue.Format(_T("*"));
		else if(returnValue.Mid(0,1) == "")
			returnValue.Format(_T("*"));
	}
	else
		returnValue.Format(_T("*"));

	return returnValue;
}

int CPLCControl::Hex2Dec(const CString& hex_str)
{
	CString hex("0123456789ABCDEF");

	int		cnv = 1;
	int     dec = 0;

	for(int i=0 ; i<hex_str.GetLength() ; i++)
	{
		dec += hex.Find(hex_str.GetAt(hex_str.GetLength()-1-i))*cnv;
		cnv *= 16;
	}
	return dec;
}