/*******************************************************************************
  TFTP Client Module API Header File

  Company:
    Microchip Technology Inc.
    
  File Name:
    tftpc.h

  Summary:
    The TFTP Client module implements the Trivial File Transfer Protocol (TFTP). 
    
  Description:
    The TFTP Client module implements the Trivial File Transfer Protocol (TFTP). 
	By default, the module opens a client socket for the default interface 
	configuration.
    From the command prompt, the TFTP client module mode will be selected. At 
	present only two modes are supported: Read and Write.
    * For Read mode - File will be fetched from the Server using the GET command.
    * For Write mode - Server will be able to fetch the file from the client using 
	  the PUT command.
    The TFTP module needs the file system for GET and PUT command operation.
    When one mode is in operation, access to the other mode or another server 
	is not allowed.
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright © 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
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

#ifndef __TFTPC_H
#define __TFTPC_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  


// *****************************************************************************
/*
  Enumeration:
    TCPIP_TFTP_CMD_TYPE

  Summary:
    File command mode used for TFTP PUT and GET commands.

  Description:
    These enum values are issued from the command line.
*/
typedef enum _TFTP_CMD_TYPE
{
    TFTP_CMD_PUT_TYPE=0,   // TFTP client issues a PUT command to write a file 
	                       // to the server
    TFTP_CMD_GET_TYPE,     // TFTP client issues a GET command to read the file 
	                       // from the server
    TFTP_CMD_NONE,
}TCPIP_TFTP_CMD_TYPE;



// *****************************************************************************
/*
  Enumeration:
    TCPIP_TFTPC_ACCESS_ERROR

  Summary:
    Standard error codes For TFTP PUT and GET command operation.

  Description:
    Standard error codes For TFTP PUT and GET command operation.
*/

typedef enum _TFTP_ACCESS_ERROR
{
    TFTPC_ERROR_NONE = 0,
    TFTPC_ERROR_FILE_NOT_FOUND = -1,     // TFTP client file not found
    TFTPC_ERROR_ACCESS_VIOLATION = -2,   // TFTP client access violation
    TFTPC_ERROR_DISK_FULL = -3,          // TFTP client buffer full
    TFTPC_ERROR_INVALID_OPERATION = -4,  // TFTP client invalid command operation
    TFTPC_ERROR_UNKNOWN_TID = -5,        // TFTP ID error
    TFTPC_ERROR_FILE_EXISTS = -6,        // TFTP client file already exists
    TFTPC_ERROR_NO_SUCH_USE = -7,        // TFTP client not in use
    TFTPC_ERROR_DNS_RESOLVE_ERR = -8,    // TFTP client DNS resolve error
    TFTPC_RES_TFTP_IF_ERR       = -9,    // TFTP client interface error
    TFTPC_RES_INVALID_FILE_LENGTH = -10, // TFTP client file length is more than 
	                                     // the expected size, which should be
                                         // the size of SYS_FS_MAX_PATH

} TCPIP_TFTPC_ACCESS_ERROR;

// *****************************************************************************
/*
  Structure:
    TCPIP_TFTPC_MODULE_CONFIG

  Summary:
    Placeholder for TFTP Client module configuration.

  Description:
    Placeholder for TFTP Client module configuration.
*/
typedef struct
{
    const char*     tftpc_interface;
    uint32_t        tftpc_reply_timeout;      // time-out for the server reply in seconds
} TCPIP_TFTPC_MODULE_CONFIG;


/*********************************************************************
 Function:
    TCPIP_TFTPC_ACCESS_ERROR TCPIP_TFTPC_SetCommandOption(char * hostname,
                                TCPIP_TFTP_CMD_TYPE cmdType,char * fileName )

  Summary:
    TFTP client command operation configuration.

  Description:
    This function is used to set the client mode, server, and file name.
    The file name is accessed as per the TFTP command mode.

  Precondition:
    The TCP/IP Stack should have been initialized.

  Parameters:
    hostname - Server address
    cmdType -  GET or PUT command
    fileName - File to be processed

  Returns:
    TCPIP_TFTPC_ACCESS_ERROR.

  Remarks:
    None.
 */
TCPIP_TFTPC_ACCESS_ERROR TCPIP_TFTPC_SetCommandOption(char * hostname,TCPIP_TFTP_CMD_TYPE cmdType,char * fileName );



// *****************************************************************************
/*
  Function:
    void  TCPIP_TFTPC_Task(void)

  Summary:
    Standard TCP/IP stack module task function.

  Description:
    Performs TFTP module tasks in the TCP/IP stack.

  Precondition:
    TFTP module should have been initialized

  Parameters:
    None.

  Returns:
    None.

  Example:
    None

  Remarks:
    None.
*/
void  TCPIP_TFTPC_Task(void);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END


#endif  // __TFTPC_H

