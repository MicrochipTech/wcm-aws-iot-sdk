/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    logging.c
  
  Summary:
    Crypto Framework Libarary source for cryptographic functions.

  Description:
    This source file contains functions that make up the Cryptographic 
	Framework Library for PIC32 families of Microchip microcontrollers.
**************************************************************************/

//DOM-IGNORE-BEGIN
/******************************************************************************
File Name:  logging.c
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



#ifdef HAVE_CONFIG_H
    #include "crypto/src/config.h"
#endif

#ifndef LIBRARY_BUILD
#include "system_config.h"
#endif

#include "crypto/src/settings.h"

/* submitted by eof */

#include "crypto/src/logging.h"
#include "crypto/src/error.h"


#ifdef __cplusplus
    extern "C" {
#endif
    CYASSL_API int  CyaSSL_Debugging_ON(void);
    CYASSL_API void CyaSSL_Debugging_OFF(void);
#ifdef __cplusplus
    } 
#endif


#ifdef DEBUG_CYASSL

/* Set these to default values initially. */
static CyaSSL_Logging_cb log_function = 0;
static int loggingEnabled = 0;

#endif /* DEBUG_CYASSL */


int CyaSSL_SetLoggingCb(CyaSSL_Logging_cb f)
{
#ifdef DEBUG_CYASSL
    int res = 0;

    if (f)
        log_function = f;
    else
        res = BAD_FUNC_ARG;

    return res;
#else
    (void)f;
    return NOT_COMPILED_IN;
#endif
}


int CyaSSL_Debugging_ON(void)
{
#ifdef DEBUG_CYASSL
    loggingEnabled = 1;
    return 0;
#else
    return NOT_COMPILED_IN;
#endif
}


void CyaSSL_Debugging_OFF(void)
{
#ifdef DEBUG_CYASSL
    loggingEnabled = 0;
#endif
}


#ifdef DEBUG_CYASSL

#ifdef FREESCALE_MQX
    #include "crypto/src/fio.h"
#else
    #include <stdio.h>   /* for default printf stuff */
#endif

#ifdef THREADX
    int dc_log_printf(char*, ...);
#endif

static void cyassl_log(const int logLevel, const char *const logMessage)
{
    if (log_function)
        log_function(logLevel, logMessage);
    else {
        if (loggingEnabled) {
#ifdef THREADX
            dc_log_printf("%s\n", logMessage);
#elif defined(MICRIUM)
        #if (NET_SECURE_MGR_CFG_EN == DEF_ENABLED)
            NetSecure_TraceOut((CPU_CHAR *)logMessage);
        #endif
#elif defined(CYASSL_MDK_ARM)
            fflush(stdout) ;
            printf("%s\n", logMessage);
            fflush(stdout) ;
#else
            fprintf(stderr, "%s\n", logMessage);
#endif
        }
    }
}


void CYASSL_MSG(const char* msg)
{
    if (loggingEnabled)
        cyassl_log(INFO_LOG , msg);
}


void CYASSL_ENTER(const char* msg)
{
    if (loggingEnabled) {
        char buffer[80];
        sprintf(buffer, "CyaSSL Entering %s", msg);
        cyassl_log(ENTER_LOG , buffer);
    }
}


void CYASSL_LEAVE(const char* msg, int ret)
{
    if (loggingEnabled) {
        char buffer[80];
        sprintf(buffer, "CyaSSL Leaving %s, return %d", msg, ret);
        cyassl_log(LEAVE_LOG , buffer);
    }
}


void CYASSL_ERROR(int error)
{
    if (loggingEnabled) {
        char buffer[80];
        sprintf(buffer, "CyaSSL error occured, error = %d", error);
        cyassl_log(ERROR_LOG , buffer);
    }
}

#endif  /* DEBUG_CYASSL */ 
