/******************************************************************************
 *	
 *	(C) Copyright WDI 2010
 *	
 ******************************************************************************
 *
 *	FILE:		epp.h
 *
 *	PROJECT:	AFT Sensors
 *
 *	SUBPROJECT:	.
 *
 *	Description: Embedded part configuration protocol.
 *               Protocol design for accessing configuration of the part.
 *	
 ******************************************************************************
 *	
 *	Change Activity
 *	Defect  Date       Developer        Description
 *	Number  DD/MM/YYYY Name
 *	======= ========== ================ =======================================
 *          21/09/2009 Andrew L.        Initial version
 *          02/11/2010 Chris O.           
 *          15/11/2010 Chris O.         Added synchronization objects for
 *                                      multithreaded applications
 *                                     
 *****************************************************************************/

#pragma once

#include "atf_def.h"

#define DtTypeSize(dt) (((dt)&DtDataType)==DtDwordEnm ? 4 : (((dt)&DtDataType)>>4))
#define DtGetDataType(dt) ((dt)&DtDataType)
#define INVALID_IDX	-1

typedef enum  
{
	ErrOK						= 0,
	ErrNoAccess					= 1,
	ErrWrongType				= 2,
	ErrOutOfBound				= 3,
	ErrInvalid					= 4,
	ErrUnavailable				= 5,
	ErrNotSupported				= 6,
	ErrSntaxError				= 7,
	ErrNoresources				= 8,
	ErrInternal					= 9,
	ErrOperFailed				= 10,
	ErrTimeout					= 11,
	ErrChksum					= 12,
	ErrUnknown
} EppErrorCodes;

#define ErrNamesTab const char * g_eppErrorNames[] = \
{	\
		"0-ErrOK",						\
		"1-ErrNoAccess",				\
		"2-ErrWrongType",				\
		"3-ErrOutOfBound",				\
		"4-ErrInvalid",					\
		"5-ErrUnknown",					\
		"6-ErrNotSupported",			\
		"7-ErrSntaxError",				\
		"8-ErrNoresources",				\
		"9-ErrInternal",				\
		"10-ErrOperFailed",				\
		"11-ErrTimeout",				\
		"12-ErrChksum",					\
		"ErrUnknown"					\
};

#ifdef __cplusplus
#include "tserial.h"
#include "logger.h"

typedef unsigned char Combuf_Char;

class CEpp : public Tserial
{
	// communication buffer
	enum { MAX_COM_BUFFER_SIZE = 5000 };
	Combuf_Char* p_commBuff;

public:
	// query code for data types
	enum 
	{
		QcParamInfo					= 1,
		QcParamName					= 2,
		QcParamComment				= 3,
		QcParamError				= 4,        // most recent error code visible on this parameter
		QcReadTime					= 5,        // time in seconds parameter was read most recently
		QcWriteTime					= 6,        // time in seconds parameter was written most recently
		QcValueCurrent				= 7,
		QcValueMax					= 8,
		QcValueMin					= 9,
		QcValueSetDate				= 10,
		QcEnumValueExclusive		= 11,
		QcEnumValueCombined			= 12,
		QcEnumName					= 13,
		QcEnumAllParamInfo			= 15,
	};
//	#define TagGetTag(tg) ((tg)&0xf)


	// session commands
	enum 
	{
		SessionCmdRead	            = 'R',
		SessionCmdWrite             = 'W',
		SessionCmdReadResponse      = 'r',
	};
	// transport commands
	enum 
	{
		TransportCodingBinaryLittleEndian	= 'l',
		TransportCodingBinaryBigEndian	    = 'b',
		TransportCodingAsciiStx		        = '<',
		TransportCodingAsciiEtx		        = '>',
		TransportAck			            = 'A',
		TransportNak			            = 'N'
	};

	// com port timeouts for operations taking long period of time
	enum
	{
		MicroStepTimeout    = 10000,
		MakeZeroTimeout     = 6000,
		SaveToSensorTimeout = 8000,
		MagnifChangeTimeout = 1000,
		MotorParamTimeout	= 10000
	};

#define EPPENTERFUN(funname) saveToLog2(CLogger::LogLow, "%s%s", funname, " - entered\n");
#define EPPEXITFUN(funname) saveToLog2(CLogger::LogLow, "%s%s", funname, " - exited\n");
#define EPPLOGMSG(pLogMsg, LogLev) saveToLog(pLogMsg, LogLev);
#define EPPLOGMSG2(LogLev, format, ...) saveToLog2(LogLev, format, __VA_ARGS__);

	// constructor & destructor
	CEpp () : Tserial(), m_pLogger(NULL), p_commBuff(NULL)
	{
        m_pFun = (ErrorFunctionPtr)0;
        m_pFunLocale = 0;
		InitializeCriticalSection(&m_cs);
	}
	CEpp (CLogger *pLogger) : Tserial(), p_commBuff(NULL)
	{
		m_pLogger = pLogger;       
		m_pFun = (ErrorFunctionPtr)0;
        m_pFunLocale = 0;
		InitializeCriticalSection(&m_cs);
	}

	~CEpp() 
	{ 
		Close();
		DeleteCriticalSection(&m_cs);
	}

	// serial port, communication
	int Open (const char *port_arg, int rate_arg); 
	// close port 
	void Close (); 

	// reuse port that is already opened
	void Attach (HANDLE h); 
	// leave handle opened, assuming someone else needs it
	void Detach (); 

    typedef void (*ErrorFunctionPtr)(void* locale, bool bReading, EppErrorCodes err,int bid, int pid, EppDataType tag, int num, int offset);
    private: 
        ErrorFunctionPtr m_pFun;
        void* m_pFunLocale;
    public:
	void PrintfError (void* locale, bool bReading, EppErrorCodes err, int bid, int pid, EppDataType tag, int num, int offset);

    void RegisterErrorFunction (ErrorFunctionPtr pFun, void* pFunLocale) 
	{
        m_pFun = pFun;
        m_pFunLocale = pFunLocale;
    }

	// ------- generic interface --------------------
	static const char* errName (EppErrorCodes err);

	// read the content of the element. returns ErrOK if all is fine, or other 
	// error otherwise
	EppErrorCodes _ReadArray (void *to, int bid, int pid, EppDataType tag, int num = 1, int offset = 0);
	EppErrorCodes ReadArray (void *to, int bid, int pid, EppDataType tag, int num = 1, int offset = 0) 
	{
		EppErrorCodes err = _ReadArray (to, bid, pid, tag, num, offset);
        if (serial_handle!=INVALID_HANDLE_VALUE && err!=ErrOK && err!=ErrUnavailable) 
		{
            PrintfError(m_pFunLocale, true, err, bid, pid, tag, num, offset);
        }
		if (err==ErrOperFailed) 
		{
			int bide = 0, pide = 0, err = 0;
			if (ReadMostRecentRetcode (&bide, &pide, (EppErrorCodes*)(&err))==ErrOK) 
			{
				EPPLOGMSG2(CLogger::LogAll, "%s%c%s%d%s%d%s",
						   "ATF Last Read Error: id=(", isalpha(bide) ? bide : '?', ", ", pide, ") Err=", err, ".\n");
			}
		}
		return err;
	}
	// sets the value of the single element
	EppErrorCodes _WriteArray (void *from, int bid, int pid, EppDataType tag, int num = 1, int offset = 0);
	EppErrorCodes WriteArray (void *from, int bid, int pid, EppDataType tag, int num = 1, int offset = 0) 
	{
		if (DtGetDataType(tag)==DtNoDataEnm) 
		{
			num=0;
			offset = 0;
		}
		EppErrorCodes err = _WriteArray (from, bid, pid, tag, num, offset);
        if (serial_handle!=INVALID_HANDLE_VALUE && err!=ErrOK && err!=ErrUnavailable) 
		{
            PrintfError(m_pFunLocale, false, err, bid, pid, tag, num, offset);
        }
		if (err==ErrOperFailed) 
		{
			int bide = 0, pide = 0, err = 0;
			if (ReadMostRecentRetcode (&bide, &pide, (EppErrorCodes*)(&err))==ErrOK) 
			{
				EPPLOGMSG2(CLogger::LogAll, "%s%c%s%d%s%d%s",
						   "ATF Last Read Error: id=(", isalpha(bide) ? bide : '?', ", ", pide, ") Err=", err, ".\n");
			}
			/* * * * This is code sample for testing only
			if (ReadIdError (bid, pid,(EppErrorCodes*)(&err))==ErrOK) 
			{
				EPPLOGMSG2(CLogger::LogLow, "%s%c%s%d%s%d%s",
						   "ATF Last Write Error 2: id=(", isalpha(bide) ? bid : '?', ", ", pid, ") Err=", err, ".\n");
			}
			EppDataType tag;
			int num;
			int ver;
			if (ReadIdCommInfo (bid, pid, &tag, &num, &ver)==ErrOK) 
			{
				EPPLOGMSG2(CLogger::LogLow, "%s%c%s%d%s%d%s%d%s%d%s",
						   "ATF Last Write Error Tag: id=(", isalpha(bide) ? bid : '?', ", ", pid, "), tag=", tag, ", num=", num, ", ver=", ver, ".\n");
			}
			*/
		}
		return err;
	}

	typedef enum 
	{
		IsEppVerUnknow	= 0,
		IsEppVerCheck	= 1,
		IsEppVerBasic	= 2,
		IsEppVerSupport = 3
	} EppVer_Mode;
	static EppVer_Mode s_m_ev;		// epp protocol version 

	bool EppVerCheck();

	// reads information describing the type of data used for particular communication register
	inline EppErrorCodes ReadIdCommInfo (int bid, int pid, EppDataType *ptag, int *pnum, int *pversion)
	{
		short commTable[4];

		if (!EppVerCheck()) return ErrNotSupported;
		EppErrorCodes err = _ReadArray (commTable, bid, pid, (EppDataType)(QcParamInfo|DtWordEnm), 3, 0);

		if (err==ErrOK) 
		{
			*ptag = (EppDataType)(commTable[0]);
			*pnum = commTable[1];
			*pversion = commTable[2];
		}

		return err;
	}

	// reads most recent status returned with particular command
	inline EppErrorCodes ReadIdError (int bid, int pid, EppErrorCodes* perr)
	{
		short commTable[4];

		if (!EppVerCheck()) return ErrNotSupported;
		EppErrorCodes err = _ReadArray (commTable, bid, pid, (EppDataType)(QcParamError|DtWordEnm), 1);

		if (err==ErrOK) 
		{
			*perr = (EppErrorCodes)(commTable[0]);
		}

		return err;
	}

	typedef struct	          // element describing most recent communication error
	{
		short iError;         // error that has been reported addressing iMostRecentField
		short iField;         // field that had been addressed most recently
		short iLen;           // number of elements accessed in most recent command
		short iOffset;        // offset in most recent command
		short iType;          // type of the acecss and data field used in most recent command
	} CommMostRecent;

	// reads most recent return code assigned by sensor
	inline EppErrorCodes ReadMostRecentRetcode (int *pbid, int *ppid, EppErrorCodes* perr)
	{
		CommMostRecent commErr;

		if (!EppVerCheck()) return ErrNotSupported;
		EppErrorCodes err = _ReadArray (&commErr, 's', 20, DtWordEnm, 2);

		if (err==ErrOK) 
		{
			*pbid = (commErr.iField >> 8) & 0xff;
			*ppid = commErr.iField & 0xff;
			*perr = (EppErrorCodes)(commErr.iError);
		}

		return err;
	}

	// restore communication.
	// returns: 0-failed, 1-app running, 2-boot loader reset to app
	int RecoverSensor();

	// check for Ack character received from the other side
	int PingAck();

	// resets sensor
	int Reset();

	EppErrorCodes _ChangeCommTimeouts(DWORD dwTimeout);
	EppErrorCodes _ResetCommTimeouts();
	EppErrorCodes _ChangeCommBaudrate(DWORD dwBaudRate);
	EppErrorCodes _GetCommBaudrate(DWORD *dwBaudrate);
	inline bool isSerialConnection() {return serial_handle != INVALID_HANDLE_VALUE ? true : false; }
	EppErrorCodes sendCmnd(Combuf_Char *pToSend, Combuf_Char *pToRecv = NULL, int iTimeout = 0, int iExpByteNum = 0);

	// logging
	CLogger *m_pLogger;
	void saveToLog(const char *pLogMsg, CLogger::LogLevels iLogLevel);
	void saveToLog2(CLogger::LogLevels iLogLevel, char *pFormat, ...);
	inline void setLogger(CLogger *pLogger) { m_pLogger = pLogger; }

private:
	CRITICAL_SECTION m_cs;
};


// simple critical section class
#ifndef __AFXMT_H__	// defined if MIcrosoft afxmt.h is included
class CCriticalSection
{
private: 
	CRITICAL_SECTION m_cs;
	const bool m_bIsLocalCs;
public:
	CCriticalSection() : m_bIsLocalCs(false) {
		InitializeCriticalSection(&m_cs);
	}
	CCriticalSection(CRITICAL_SECTION &cs) : m_bIsLocalCs (true) {
		m_cs = cs;
	}
	~CCriticalSection() {
		if (m_bIsLocalCs) DeleteCriticalSection(&m_cs);
	}
	void Lock() {
		EnterCriticalSection(&m_cs);
	}
	void Unlock() {
		LeaveCriticalSection(&m_cs);
	}
};
#endif

// this class maintains an array of CEpp objects and track availability in current thread
class CEppArr
{
public: 
	enum { MAX_EPP_NUM = 8 };
	static CEpp gs_epp_arr[MAX_EPP_NUM];			// global table of CEpp
private:
	static bool gs_epp_allocated_arr[MAX_EPP_NUM];	// indicate if object is already allocated
	static CCriticalSection gs_cs;					// to faciliate table identification
	static __declspec( thread ) int gst_epp_index;	// current index per thread

public:
	static int Open (const char *port_arg, int rate_arg);
	static void Close ();

	static inline int SetCurrentEppIndex(int idx) 
	{
		if (idx>=0 
			&& idx<MAX_EPP_NUM
			 && gs_epp_allocated_arr[idx]) 
		{
			gst_epp_index = idx;
			return ErrOK;
		}
		return ErrOperFailed;
	}
	static inline int GetCurrentEppIndex() { return gst_epp_index; }
	static inline CEpp* Epp() 
	{
		if (gst_epp_index>=0 
			&& gst_epp_index<MAX_EPP_NUM
			 && gs_epp_allocated_arr[gst_epp_index]) 
		{
			return &gs_epp_arr[gst_epp_index];
		}
		else if (gst_epp_index==INVALID_IDX
				 && gs_epp_allocated_arr[0]
				  && !gs_epp_allocated_arr[1])
		{
			// we have 0 index used, while 1 is not and gst_epp_index was never set. This is 
			// a typical situation when thread other then one which opened com port 
			// is attempting to communicate and SetCurrentEppIndex() was not explicitly called.
			// to make live easier lets do to here
			gst_epp_index = 0;
			return &gs_epp_arr[gst_epp_index];
		}
		return NULL;
	}
};

// for compatibility with older code which is using a single thread single atf object
static CEpp& g_epp = CEppArr::gs_epp_arr[0];	

#define CEPPARR_DECLARE_CURRENT_OR_RETURN		\
	CEpp *pEpp = CEppArr::Epp();				\
	if (!pEpp) return ErrNoresources;

#define CEPPARR_DECLARE_CURRENT_OR_RETURN_BOOL	\
	CEpp *pEpp = CEppArr::Epp();				\
	if (!pEpp) return false;



#endif // __cplusplus
