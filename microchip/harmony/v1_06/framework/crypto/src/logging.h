/**************************************************************************
  Crypto Framework Library Header

  Company:
    Microchip Technology Inc.

  File Name:
    logging.h
  
  Summary:
    Crypto Framework Library header for cryptographic functions.

  Description:
    This header file contains function prototypes and definitions of
    the data types and constants that make up the Cryptographic Framework
    Library for PIC32 families of Microchip microcontrollers.
**************************************************************************/

//DOM-IGNORE-BEGIN
/******************************************************************************
Copyright © 2013 released Microchip Technology Inc.  All rights reserved.

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



/* submitted by eof */


#ifndef CYASSL_LOGGING_H
#define CYASSL_LOGGING_H


#ifdef __cplusplus
    extern "C" {
#endif


enum  CYA_Log_Levels {
    ERROR_LOG = 0,
    INFO_LOG,
    ENTER_LOG,
    LEAVE_LOG,
    OTHER_LOG
};

typedef void (*CyaSSL_Logging_cb)(const int logLevel,
                                  const char *const logMessage);

CYASSL_API int CyaSSL_SetLoggingCb(CyaSSL_Logging_cb log_function);


#ifdef DEBUG_CYASSL

    void CYASSL_ENTER(const char* msg);
    void CYASSL_LEAVE(const char* msg, int ret);

    void CYASSL_ERROR(int);
    void CYASSL_MSG(const char* msg);

#else /* DEBUG_CYASSL   */

    #define CYASSL_ENTER(m)
    #define CYASSL_LEAVE(m, r)

    #define CYASSL_ERROR(e) 
    #define CYASSL_MSG(m)

#endif /* DEBUG_CYASSL  */

#ifdef __cplusplus
}
#endif

#endif /* CYASSL_MEMORY_H */
