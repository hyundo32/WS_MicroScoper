#pragma once

enum SocketType_DIT	{ Socket_None			// 0
					, Socket_AFM_Server		// 1
					, Socket_Align_Server	// 2
					, Socket_WSI_Server		// 3
					, Socket_Result_Server	// 4
					, Socket_All_Client=9	// 9
					};

// Network IP
#define	NETIP_AFM_SERVER						_T("192.168.0.230")
#define	NETIP_ALIGN_SERVER						_T("192.168.0.210")
#define	NETIP_WSI_SERVER						_T("192.168.0.230")
#define NETIP_ALL_CLIENT						_T("192.168.0.1")
//////////////////////////////////////////////////////////////////////////

// Network Port
#define NETPORT_AFM_SERVER_ALL					10241
#define NETPORT_ALIGN_SERVER_ALL				10242
#define NETPORT_WSI_SERVER_ALL					10243
#define NETPORT_RESULT_SERVER_ALL				10244
//////////////////////////////////////////////////////////////////////////

// Network Module Code
#define NETCODE_AFM_SERVER_ALL					7101
#define NETCODE_ALIGN_SERVER_ALL				7102
#define NETCODE_WSI_SERVER_ALL					7103
#define NETCODE_RESULT_SERVER_ALL				7104
//////////////////////////////////////////////////////////////////////////

// Network Version
#define NETVER_AFM_SERVER_ALL					1001
#define NETVER_ALIGN_SERVER_ALL					1002
#define NETVER_WSI_SERVER_ALL					1003
#define NETVER_RESULT_SERVER_ALL				1004
/////////////////////////////////////////////////////////////////////////

//Network Message
#define	NET_RESULT_FAIL							0
#define	NET_RESULT_SUCCESS						1
/////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------
//--- aFm_server[1], all_Client[9]
//--------------------------------------------------------
#define		FC_MODULE_INDEX						19001	
#define		CF_MODULE_INDEX						91001	

#define		FC_SEND_SIGNAL						19002	//
#define		CF_SEND_SIGNAL						91002	//

#define		FC_RECV_SIGNAL						19003	//	
#define		CF_RECV_SIGNAL						91003	//

#define		FC_SEND_RECIPE						19004
#define		CF_SEND_RECIPE						91004

#define		FC_RECV_RECIPE						19005
#define		CF_RECV_RECIPE						91005

#define		FC_RECIPE_COMP						19006
#define		CF_RECIPE_COMP						91006

#define		FC_RECIPE_COUNT						19007
#define		CF_RECIPE_COUNT						91007

#define		FC_RECIPE_INFO						19008
#define		CF_RECIPE_INFO						91008

#define		FC_SEND_BIT							19009
#define		CF_SEND_BIT							91009

#define		FC_RECV_BIT							19010
#define		CF_RECV_BIT							91010

#define		FC_RECV_CURRENT_POS					19011
#define		CF_RECV_CURRENT_POS					91011

#define		FC_RECV_DIFF_FOCUS					19012
#define		CF_RECV_DIFF_FOCUS					91012

#define		FC_RECV_WSI_MODE					19013
#define		CF_RECV_WSI_MODE					91013

//--------------------------------------------------------
//--- Align_server[2], all_Client[9]
//--------------------------------------------------------
#define		AC_MODULE_INDEX						29000
#define		CA_MODULE_INDEX						92000

#define		AC_ALIVE							29001	// not use
#define		CA_ALIVE							92001	// not use
	
#define		AC_ALIGN_GLASS						29002	// net use
#define		CA_ALIGN_GLASS						92002	// net use

#define		AC_ALIGN_START						29003	// net use
#define		CA_ALIGN_START						92003	// net use

#define		AC_ALIGN_RESULT						29004
#define		CA_ALIGN_RESULT						92004

#define		AC_ALIGN_SETTING					29005	// net use
#define		CA_ALIGN_SETTING					92005	// net use

//--------------------------------------------------------
//--- Wsi_server[3], Client[9]
//--------------------------------------------------------
#define		WC_MODULE_INDEX						39000
#define		CW_MODULE_INDEX						93000

#define		WC_MEASURE_READY					39001
#define		CW_MEASURE_READY					93001

#define		WC_MEASURE_START					39002
#define		CW_MEASURE_START					93002

#define		WC_MEASURE_END						39003
#define		CW_MEASURE_END						93003

#define		WC_MEASURE_TIMEOVER					39004
#define		CW_MEASURE_TIMEOVER					93004

#define		WC_MEASURE_RESULT					39005
#define		CW_MEASURE_RESULT					93005

#define		WC_MEASURE_INFO						39006 
#define		CW_MEASURE_INFO						93006

//--------------------------------------------------------
//--- Result_server[4], Client[9]
//--------------------------------------------------------
#define		RC_MODULE_INDEX						49000
#define		CR_MODULE_INDEX						94000

#define		RC_RESULT_INFO						49001
#define		CR_RESULT_INFO						94001