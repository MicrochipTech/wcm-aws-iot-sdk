/*******************************************************************************
  SNTP Client Module API Header File

  Company:
    Microchip Technology Inc.
    
  File Name:
    sntp.h

  Summary:
    The SNTP module implements the Simple Network Time Protocol. The module
    (by default) updates its internal time every 10 minutes using a pool of 
    public global time servers.

  Description:
    The SNTP module implements the Simple Network Time Protocol. The module 
    (by default) updates its internal time every 10 minutes using a pool of 
    public global time servers. It then calculates reference times on any call 
    to SNTPGetUTCSeconds using the internal Tick timer module.
********************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

#ifndef __SNTP_H
#define __SNTP_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// Placeholder for SNTP Module Configuration
typedef struct
{
    const char*     ntp_server;             // the NTP server to contact; name or IP address
    const char*     ntp_interface;          // the default NTP interface to use
    IP_ADDRESS_TYPE ntp_connection_type;    // the IPv4/IPv6 connection type
    uint32_t        ntp_reply_timeout;      // timeout for the server reply in seconds
    uint32_t        ntp_stamp_timeout;      // timeout for the time stamp in seconds
    uint32_t        ntp_success_interval;   // server query interval after an successful update, in seconds
    uint32_t        ntp_error_interval;     // server query interval after an error, in seconds
} TCPIP_SNTP_MODULE_CONFIG;


// *****************************************************************************
/* TCPIP_SNTP_RESULT Enumeration

  Summary:
    Provides a list of possible results for the SNTP module.

  Description:
    Provides a list of possible SNTP results.

  Remarks:
    None
 */
typedef enum
{
    // success codes
    SNTP_RES_OK,                // the operation was successful
    SNTP_RES_PROGRESS,          // an NTP operation is in progress

    // error codes
    SNTP_RES_BUSY               = -1,   // module is busy
    SNTP_RES_TSTAMP_STALE       = -2,   // timestamp is stale, there's no recent timestamp
    SNTP_RES_SKT_ERR            = -3,   // NTP socket could not be opened
    SNTP_RES_NTP_SERVER_TMO     = -4,   // NTP server could not be accessed
    SNTP_RES_NTP_VERSION_ERR    = -5,   // wrong NTP version received
    SNTP_RES_NTP_TSTAMP_ERR     = -6,   // wrong NTP time stamp received
    SNTP_RES_NTP_SYNC_ERR       = -7,   // NTP time synchronization error
    SNTP_RES_NTP_KOD_ERR        = -8,   // an NTP KissOfDeath code has been received
    SNTP_RES_NTP_DNS_ERR        = -9,   // an NTP DNS error
    SNTP_RES_NTP_IF_ERR         = -10,  // an NTP interface error
} TCPIP_SNTP_RESULT;


/*****************************************************************************
  Function:
    uint32_t TCPIP_SNTP_UTCSecondsGet(void)

  Summary:
    Obtains the current time from the SNTP module.

  Description:
    This function obtains the current time as reported by the SNTP module.
    Use this value for absolute time stamping.  The value returned is (by
    default) the number of seconds since 01-Jan-1970 00:00:00.

  Precondition:
    TCP/IP Stack should have been initialized

  Parameters:
    None

  Returns:
    The number of seconds since the Epoch.  (Default 01-Jan-1970 00:00:00)

  Remarks:
    Do not use this function for time difference measurements.  The Tick
    module is more appropriate for those requirements.
 */
uint32_t    TCPIP_SNTP_UTCSecondsGet(void);


/*****************************************************************************
  Function:
    TCPIP_SNTP_RESULT TCPIP_SNTP_ConnectionParamSet(TCPIP_NET_HANDLE netH, IP_ADDRESS_TYPE ntpConnType, const char* ntpServer);

  Summary:
    Sets the current SNTP connection parameters

  Description:
    This function sets the parameters for the next SNTP connections.
    
  Precondition:
    TCP/IP Stack should have been initialized

  Parameters:
    netH            - new interface to use as default SNTP interface
                      if 0, the current interface is not changed
    ntpConnType     - type of connection to make: IPv4 or IPv6
                      if IP_ADDRESS_TYPE_ANY, the current setting is not changed
    ntpServer       - the NTP server to be used; name or an IP address can be used
                      if 0, the current NTP server is not changed

  Returns:
    SNTP_RES_OK - if the call succeeded
    TCPIP_SNTP_RESULT error code otherwise

  Remarks:
    None
 */
TCPIP_SNTP_RESULT    TCPIP_SNTP_ConnectionParamSet(TCPIP_NET_HANDLE netH, IP_ADDRESS_TYPE ntpConnType, const char* ntpServer);

/*****************************************************************************
  Function:
    TCPIP_SNTP_RESULT TCPIP_SNTP_ConnectionInitiate(void);

  Summary:
    Forces a connection to the NTP server

  Description:
    This function will start a connection to the NTP server
    
  Precondition:
    TCP/IP Stack should have been initialized

  Parameters:
    None

  Returns:
    SNTP_RES_OK - if the call succeeded
    SNTP_RES_BUSY error code if the connection could not have been started

  Remarks:
    None
 */
TCPIP_SNTP_RESULT     TCPIP_SNTP_ConnectionInitiate(void);

/*****************************************************************************
  Function:
    TCPIP_SNTP_RESULT TCPIP_SNTP_TimeStampGet(uint64_t* pTStamp, uint32_t* pLastUpdate);

  Summary:
    Gets the last valid timestamp obtained from an NTP server

  Description:
    This function gets the last valid timestamp obtained from an NTP server
    
  Precondition:
    TCP/IP Stack should have been initialized

  Parameters:
    pTStamp     - pointer to a 64 bit buffer to store the last NTP timestamp 
                  could be NULL if the timestamp not needed
    pLastUpdate - pointer to store the last time stamp update tick 
                  could be NULL if the update time not needed

  Returns:
    SNTP_RES_OK - if the call succeeded
    SNTP_RES_TSTAMP_STALE error code if there's no recent time stamp

  Remarks:
    None
 */
TCPIP_SNTP_RESULT     TCPIP_SNTP_TimeStampGet(uint64_t* pTStamp, uint32_t* pLastUpdate);


/*****************************************************************************
  Function:
    TCPIP_SNTP_RESULT TCPIP_SNTP_LastErrorGet(void);

  Summary:
    Gets the last error code set in the NTP server

  Description:
    This function returns the last NTP error code and clears the current error code
    
  Precondition:
    TCP/IP Stack should have been initialized

  Parameters:
    None

  Returns:
    the last error code encountered by the NTP module

  Remarks:
    None
 */
TCPIP_SNTP_RESULT     TCPIP_SNTP_LastErrorGet(void);



// *****************************************************************************
/*
  Function:
    void  TCPIP_SNTP_Task(void)

  Summary:
    Standard TCP/IP stack module task function.

  Description:
    Performs SNTP module tasks in the TCP/IP stack.

  Precondition:
    SNTP module should have been initialized

  Parameters:
    None.

  Returns:
    None.

  Example:
    None

  Remarks:
    None.
*/
void  TCPIP_SNTP_Task(void);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END


#endif  // __SNTP_H


