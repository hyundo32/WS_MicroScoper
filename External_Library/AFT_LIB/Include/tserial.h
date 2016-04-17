
/* ------------------------------------------------------------------------ --
--                                                                          --
--                        PC serial port connection object                  --
--                           for non-event-driven programs                  --
--                                                                          --
--                                                                          --
--                                                                          --
--  Copyright @ 2001          Thierry Schneider                             --
--                            thierry@tetraedre.com                         --
--                                                                          --
--                                                                          --
--                                                                          --
-- ------------------------------------------------------------------------ --
--                                                                          --
--  Filename : sertest2.cpp                                                 --
--  Author   : Thierry Schneider                                            --
--  Created  : April 4th 2000                                               --
--  Modified : April 8th 2001                                               --
--  Plateform: Windows 95, 98, NT, 2000 (Win32)                             --
-- ------------------------------------------------------------------------ --
--                                                                          --
--  This software is given without any warranty. It can be distributed      --
--  free of charge as long as this header remains, unchanged.               --
--                                                                          --
-- ------------------------------------------------------------------------ */


// modified by Chris O. Nov-02-2010

/* ---------------------------------------------------------------------- */
#ifndef TSERIAL_H
#define TSERIAL_H

enum ComPortTimeouts
{
	RxIntrvlTimeout      = 200,  // Read Interval Timeout
	RxTotalTimeoutMultip = 10,   // Read Total Timeout Multiplier
	RxTotalTimeoutConst  = 500,  // Read Total Timeout Constant
	TxTotalTimeoutMultip = 10,   // Write Total Timeout Multiplier
	TxTotalTimeoutConst  = 200   // Write Total Timeout Constant 
};

enum serial_parity  { spNONE, spODD, spEVEN };


/* -------------------------------------------------------------------- */
/* -----------------------------  Tserial  ---------------------------- */
/* -------------------------------------------------------------------- */
class Tserial
{
    // -------------------------------------------------------- //
protected:
	static const int iMaxPortName = 10;
    char              port[iMaxPortName];            // port name "com1",...
    int               rate;                          // baudrate
    serial_parity     parityMode;
    HANDLE            serial_handle;                 // ...
	DWORD			  dwReadTotalTimeout;			 // timeout set while port is opened

	DCB dcbOrg;
	COMMTIMEOUTS ctoOrg;
	DWORD cmOrg;

    // ++++++++++++++++++++++++++++++++++++++++++++++
    // .................. EXTERNAL VIEW .............
    // ++++++++++++++++++++++++++++++++++++++++++++++
public:
    Tserial();
    ~Tserial();

	DWORD connect (const char *port_arg, 
				   int rate_arg,
				   serial_parity parity_arg); 
    DWORD disconnect (void);
	DWORD reconnect (void);
	BOOL isconnected() { return serial_handle==INVALID_HANDLE_VALUE ? FALSE : TRUE; }

    DWORD sendChar (char c);
    DWORD sendArray (const char *buffer, int len);
    DWORD getChar(char *c);
    DWORD getArray (char *buffer, int len, DWORD *iReadNum);
    int   getNbrOfBytes (void);

	DWORD ChangeCommTimeouts(DWORD dwTimeout);
	DWORD ResetCommTimeouts();
	DWORD ChangeCommBaudrate(DWORD dwBaudRate);
	DWORD GetCommBaudrate(DWORD *dwBaudrate);
};
/* -------------------------------------------------------------------- */

#endif TSERIAL_H


