/******************************************************************************
 *	
 *	(C) Copyright WDI 2010
 *	
 ******************************************************************************
 *
 *	FILE:		logger.h
 *
 *	PROJECT:	AFT Sensor
 *
 *	SUBPROJECT:	.
 *
 *	Description: Logging mechanism
 *	
 ******************************************************************************
 *	
 *	Change Activity
 *	Defect  Date       Developer        Description
 *	Number  DD/MM/YYYY Name
 *	======= ========== ================ =======================================
 *          15/10/2010 Chris O.         Initial version
 *          15/11/2010 Chris O.         Added synchronization objects for
 *                                      multithreaded applications
 *
 ******************************************************************************/
#pragma once

#ifdef __cplusplus

#include <stdio.h>
#include "..\External_Library\AFT_LIB\Include\LogSubsystemInterface.h"

class CLogger
{
public:
	enum LogLevels
	{
		LogOff = 0,
		LogLow,
		LogMid,
		LogAll
	};

protected:
	FILE *m_pFile;
	LogLevels m_iLogLevel;
	NSLogSubsystem::ILogSubsystem *	m_pLogger ;

private:
	CRITICAL_SECTION m_cs;

public:
	CLogger() : m_pFile(NULL), m_iLogLevel(LogOff), m_pLogger ( NULL ) 
	{
		InitializeCriticalSection(&m_cs);
	}

	~CLogger() 
	{
		if ( m_pLogger != NULL )
		{
			m_pLogger -> Release () ;
		}

		DeleteCriticalSection(&m_cs);
	}

	bool openLogFile(const char *pLogFilePath, const char *pMode);
	bool closeLogFile();
	void setLogLevel(LogLevels iLogLevel);
	LogLevels getLogLevel(void);
	FILE *getLogFileHandle(void);
	void saveToLog(char const *strMsg, LogLevels iLogLevel);
	void setExternalLogger ( NSLogSubsystem::ILogSubsystem * pLogger ) { m_pLogger = pLogger ; }
};

#endif
