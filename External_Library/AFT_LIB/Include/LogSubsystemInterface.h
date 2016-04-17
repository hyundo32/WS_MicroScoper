/*****************************************************************************
 *	
 *	(C) Copyright WDI 2014
 *	
 *****************************************************************************/

#pragma once

#include <Unknwn.h>
#include <Prsht.h>

#ifndef LOG_SUBSYSTEM_IMPLEMENTATION
	#define DECLSPEC __declspec(dllimport)
#else
	#define DECLSPEC __declspec(dllexport)
#endif

// {BE844E1A-D78A-4d21-B7AD-B38FD1051EFA}
DEFINE_GUID ( GUIDLOGSUBSYSTEM, 0xbe844e1a, 0xd78a, 0x4d21, 0xb7, 0xad, 0xb3, 0x8f, 0xd1, 0x5, 0x1e, 0xfa);

namespace NSLogSubsystem
{
	class ILogRecord
	{
	public:
		// Method AddEventData copies specified amount of data into internal buffer.
		// pData buffer can be reused immediately.
		// Data set name will be stored in the log. It represents relative path to the dataset
		// and therefore only characters allowed for the file relative pathname should be used.  
		virtual HRESULT __stdcall AddEventData ( const char *		szDataSetName, 
												 void *				pData,
												 ULONG				uDataSize
											   ) = 0 ;

		// Method AddEventFile adds file to event log asynchronously.
		// If bDeleteAfterAdd is set to true the file will be deleted.
		// Data set name will be stored in the log. It represents relative path to the dataset
		// and therefore only characters allowed for the file relative pathname should be used.  
		virtual HRESULT __stdcall AddEventFile ( const char *		szDataSetName,
												 const char *		szFileName,
												 bool				bDeleteAfterAdd
											   ) = 0 ;

		// Method Submit sends the record to the log buffer.
		// ILogRecord object is not accessible any more after this method is called.
		virtual HRESULT __stdcall Submit () = 0 ;
	};

	class ILogSubsystem : public IUnknown
	{
	public:
		enum STYLE_FLAGS { SF_SHOW_HEADER			= 1, 
						   SF_SHOW_SEVERITY_ICON	= 2,
						   SF_SHOW_EVENT_DATE_TIME	= 4,
						   SF_SHOW_EVENT_SOURCE		= 8,
						   SF_SHOW_EVENT_SEVERITY	= 16,
						   SF_SHOW_EVENT_DESCRIPTION= 32,
						   SF_SHOW_EVENT_USERNAME	= 64,
						   SF_SHOW_ALL = ( SF_SHOW_HEADER			| 
										   SF_SHOW_SEVERITY_ICON	| 
										   SF_SHOW_EVENT_DATE_TIME	| 
										   SF_SHOW_EVENT_SOURCE		| 
										   SF_SHOW_EVENT_SEVERITY	| 
										   SF_SHOW_EVENT_DESCRIPTION| 
										   SF_SHOW_EVENT_USERNAME 
										 )
						 } ;

		enum EVENT_SEVERITY { ES_TRACE,			// White
							  ES_NORMAL,		// Green
							  ES_WARNING,		// Yellow
							  ES_ERROR,			// Red
							  ES_FATAL,			// Red with black frame
							  ES_NEVER_SHOWN
							};

		enum { MAX_COUNTER_NAME_LENGTH = 31 } ;

		enum { KEEP_LOG_FILES_DAYS_MIN = 1, KEEP_LOG_FILES_DAYS_MAX = 365 } ;
		enum { MAX_LOG_FILE_SUFFIX_LENGTH = 32,
			   MAX_SOURCE_NAME_LENGTH	  = 32
			 } ;

		enum { KEEP_LOG_FILES_DAYS_DEF	 = 31,
			   LOG_SEVERITY_DEF			 = ES_WARNING,
			   SHOW_MESSAGE_SEVERITY_DEF = ES_FATAL
		     };

	public:
		struct SConfiguration
		{
			int					nDaysToKeepLogFiles ;	// Should be in range KEEP_LOG_FILES_DAYS_MIN ... KEEP_LOG_FILES_DAYS_MAX
			EVENT_SEVERITY		eMinSeverityToLog	;	// Default = LOG_SEVERITY_DEF
			EVENT_SEVERITY		eMessageBoxThreshold;	// Default = SHOW_MESSAGE_SEVERITY_DEF
			char				szTraceLogSuffix[MAX_LOG_FILE_SUFFIX_LENGTH] ;
			ULONG				uSourceSuffiexCnt	;
			struct SSourceSuffix
			{
				char	szSource[MAX_SOURCE_NAME_LENGTH] ;
				char	szSuffix[MAX_LOG_FILE_SUFFIX_LENGTH] ;
			}					Suffixes[1]			;	// Actual array size is uSourceSuffiexCnt 
		} ;

	public:
		// ------------- General ---------------
		// Method SetCurrentUserName sets user name for the user - dependent log records and counters.
		virtual HRESULT	__stdcall SetCurrentUser ( const char * szUserName ) = 0 ;

		// ------------- Configuration ---------------
		// Method CreatePropertyPage returns HPROPSHEETPAGE value to be used with Property Sheets.
		virtual HRESULT __stdcall CreatePropertyPage ( HPROPSHEETPAGE * phPropSheetPage ) = 0 ;

		// ------------- Inserts ---------------
		// Method CreateLogInsert creates Log insert in the parent window
		virtual HRESULT __stdcall CreateLogInsert ( // Window position and style
													HWND			hParent,				// Parent window handle
													int				nLeft,					// Client coordinates in pixels
													int				nTop,					// Client coordinates in pixels
													int				nWidth,					// Insert width in pixels
													int				nHeight,				// Insert height in pixels
													DWORD			dwFlags,				// combination of the SF_xxx values
													int				nLinesNum,				// number of log events to show.
																							// if the number is more than zero it shows only this number of
																							// the latest events. 
																							// if number is zero it shows all events in the buffer. Scroll bar may appear.
													// Filter options
													const char *	pszEventSourceList,		// Sequence of zero - terminated source names or NULL if any
													int				nSourceCnt,				// Number of source names in the filter
													EVENT_SEVERITY 	nLowestSeverity,		// Lowest severity to show
													const char *	szRequestorName,		// Unique identified of the caller and instance
													// Output value
													HWND *			phInsertHandle			// Insert window handle
												  ) = 0 ;

		// Method DestroyLogInsert destroys GUI Insert.
		// This method is optional. Insert gets destroyed automatically when the
		// parent window is closed.
		virtual HRESULT __stdcall DestroyLogInsert ( HWND			hInsertHandle ) = 0 ;

		// Method CreateUsgInsert creates usage counters insert in the parent window
		virtual HRESULT __stdcall CreateUsgInsert ( // Window position and style
													HWND			hParent,			// Parent window handle
													int				nLeft,				// Client coordinates in pixels
													int				nTop,				// Client coordinates in pixels
													int				nWidth,				// Insert width in pixels
													int				nHeight,			// Insert height in pixels
													// Filter options
													const char *	pszCounterList,		// Sequence of zero - terminated counter names or NULL if any
													int				nSourceCnt,			// Number of counter names in the filter
													const char *	szRequestorName,	// Unique identified of the caller and instance
													// Output value
													HWND *			phInsertHandle		// Insert window handle
												  ) = 0 ;

		// Method DestroyUsgInsert destroys usage counters Insert.
		// This method is optional. Insert gets destroyed automatically when the
		// parent window is closed.
		virtual HRESULT __stdcall DestroyUsgInsert ( HWND			hInsertHandle ) = 0 ;


		// ------------- Event Sources ---------------
		// Method CreateRecord creates new record in the log
		// If specified severity is below the threshold for the log file
		// this method will return S_OK and set * ppLorRecord = NULL.
		// Caller should check the ppLogRecord value to avoid unnecessary 
		// event data processing.
		virtual HRESULT __stdcall CreateRecord ( const char *		szSourceName,			
												 EVENT_SEVERITY		Severity,
												 const char *		szDescription,
												 bool				bDontShowPopup,		// Prevents popup manager from showing message
												 // Output value
												 ILogRecord **		ppLogRecord			// Can be NULL. The record will be submitted immediately in this case.
											   ) = 0 ;

		// ------------- Usage Counters ---------------
		// Method IncreaseCounterBy increases (decreases) the counter value by specified increment.
		// If the counter does not exists it gets created.
		// Return previous value of the counter or 0 if the counter was just created.
		// Counter name length should not exceed MAX_COUNTER_NAME_LENGTH
		virtual INT64   __stdcall IncreaseCounterBy ( const char *	szCounterName,
													  bool			bGlobal,			// True for global counters, false for per - user counter
													  INT64			nIncrement
													) = 0 ;

		// Method SetCounter changes counter value to the once specified by the function argument nValue
		// Method returns previous value of the counter
		virtual INT64	__stdcall SetCounter ( const char *		szCounterName,
											   bool				bGlobal,				// True for global counters, false for per - user counter
											   INT64			nValue
											 ) = 0 ;

		// Method GetCounter reports current counter value.
		// If requested counter does not exists	the function returns 0.
		virtual INT64	__stdcall GetCounter ( const char *		szCounterName,
											   bool				bGlobal					// True for global counters, false for per - user counter
											 ) = 0 ;

		// Method DeletCounter removes counter. Useful for the session conters
		virtual void	__stdcall DeleteCounter ( const char *	szCounterName,
												  bool			bGlobal					// True for global counters, false for per - user counter
											    ) = 0 ;
																						
		// ------------- Exports ---------------
		virtual HRESULT	__stdcall ExportLog ( const char * szFileName,					// Export log as single ZIP file
											  const char * szPassword					// Can be null if no password is required
											) = 0 ;
		virtual HRESULT	__stdcall ExportUsg ( const char * szFileName,					// Export Usage Counters as single ZIP file
											  const char * szPassword					// Can be null if no password is required
											) = 0 ;

		// ------------- Configuration ---------------
		virtual HRESULT __stdcall SetConfiguration ( const SConfiguration * pConfig ) = 0 ;

		// Caller should allocate memory for the SConfiguration structure
		// and set field uSourceSuffiexCnt to show amount of data the structure can hold.
		// On return this field contains the number of entries actually written into the array.
		// Optional parameter puSourceSuffiexCnt receives actual amount of entries configured in the logger.
		// Return values:
		//	S_OK
		//  E_INVALIDARG
		//	HRESULT_FROM_WIN32 ( ERROR_MORE_DATA )
		
		virtual HRESULT __stdcall GetConfiguration ( SConfiguration *	pConfig,
													 PULONG				puSourceSuffiexCnt = NULL
												   ) = 0 ;
	} ;
} ;

// Method GetLogSubsystemInterface allocates required resources and checks for the log storage availability.
// Return values:
//	S_OK				Success
//  E_OUTOFMEMORY		Cannot allocate buffers
//	STG_E_PATHNOTFOUND	Log directory does not exist
//	STG_E_ACCESSDENIED	Write access to the log directory is denied
#define FNGETLOGSUBSYSTEMINTERFACE	"GetLogSubsystemInterface"
typedef HRESULT (__stdcall * PFNGETLOGSUBSYSTEMINTERFACE) ( NSLogSubsystem::ILogSubsystem ** ppLogSubsystem, const char * szAppName ) ;
extern "C" DECLSPEC HRESULT __stdcall GetLogSubsystemInterface ( NSLogSubsystem::ILogSubsystem ** ppLogSubsystem, const char * szAppName ) ;