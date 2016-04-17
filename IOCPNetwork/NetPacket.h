#pragma once

#define PACKET_HEADERSIZE		(sizeof(int) * 3 + sizeof(SHORT) * 4)	

class AFX_EXT_CLASS CNetPacket
{
public:
	CNetPacket(void);
	virtual ~CNetPacket(void);

	//////////////////////////////////////////////////////////////////////////
	// Packet Header
	void		SetVersion(SHORT sNetworkCode, SHORT sVersion);
	void		SetPacketCode(int nCode)	{ m_nPacketCode = nCode; }
	void		SetModuleNo(int nModuleNo)	{ m_nModuleNo = nModuleNo; }
	int			Read_PacketHeader(WSABUF* pWsaBuf);
	int			Read_PacketBody(char* pBuf);

	// 변수에 접근
	int			GetSize()					{ return m_nSize; }
	SHORT		GetNetworkCode()			{ return m_sNetworkCode; }
	SHORT		GetVersion()				{ return m_sVersion; }
	int			GetPacketCode()				{ return m_nPacketCode; }
	int			GetResult()					{ return m_sResult; }
	int			GetPacketIndex()			{ return m_sPacketIndex; }
	int			GetModuleNo()				{ return m_nModuleNo; }

	//////////////////////////////////////////////////////////////////////////
	// Packet Body
	void		ResetValues();

	BOOL		SetShort(short sValue);
	BOOL		SetInt(int nValue);
	BOOL		SetDouble(double dValue);
	BOOL		SetString(const CString& strString);
	BOOL		SetBuffer(int nSize, char* pBuffer);

	int			GetShortCount()				{ return m_nShortCount; }
	int			GetIntCount()				{ return m_nIntCount; }
	int			GetDoubleCount()			{ return m_nDoubleCount; }
	int			GetStringCount()			{ return m_nStringCount; }
	int			GetBufferSize()				{ return m_nBufferSize; }

	short		GetShort(int nIdx);
	int			GetInt(int nIdx);
	double		GetDouble(int nIdx);
	CString		GetString(int nIdx);
	char*		GetBuffer();

	//////////////////////////////////////////////////////////////////////////
	// 관리 함수들
	void		UnlockPacket()				{ m_bUsing = FALSE; }
	BOOL		IsUsing()					{ return m_bUsing; }
	void		LockPacket()				{ m_bUsing = TRUE; }
protected:
	BOOL		m_bUsing;

	//////////////////////////////////////////////////////////////////////////
	// Packet Header 부분
	int			m_nSize;				// Size를 포함한 전체 패킷 크기			4 bytes
	SHORT		m_sNetworkCode;			// Program Code * 10 + Network Code		2 bytes
	SHORT		m_sVersion;				// Network Module Version				2 bytes
	int			m_nPacketCode;			// Packet Code  4 bytes
	SHORT		m_sPacketIndex;			// 여러 Packet일 때의 Index				2 bytes
	SHORT		m_sResult;				// Result - 단순 Ack 등을 위한 예약공간	2 bytes
	int			m_nModuleNo;			// Module Number						4 bytes

	SHORT		m_sSetNetworkCode;		// Program Code * 10 + Network Code		2 bytes
	SHORT		m_sSetVersion;			// Network Module Version				2 bytes

	//////////////////////////////////////////////////////////////////////////
	// Packet Body 부분
	int			m_nShortCount;
	int			m_nShortArraySize;
	short*		m_psShort;

	int			m_nIntCount;
	int			m_nIntArraySize;
	int*		m_pnInt;

	int			m_nDoubleCount;
	int			m_nDoubleArraySize;
	double*		m_pdDouble;

	int			m_nStringCount;
	int			m_nStringArraySize;
	CString*	m_pstrString;

	int			m_nBufferSize;
	char*		m_pBuffer;
};

