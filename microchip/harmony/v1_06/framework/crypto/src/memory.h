/**************************************************************************
  Crypto Framework Library Header

  Company:
    Microchip Technology Inc.

  File Name:
    memory.h
  
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


#ifndef CYASSL_MEMORY_H
#define CYASSL_MEMORY_H

#include <stdlib.h>

#ifdef __cplusplus
    extern "C" {
#endif


typedef void *(*CyaSSL_Malloc_cb)(size_t size);
typedef void (*CyaSSL_Free_cb)(void *ptr);
typedef void *(*CyaSSL_Realloc_cb)(void *ptr, size_t size);


/* Public set function */
CYASSL_API int CyaSSL_SetAllocators(CyaSSL_Malloc_cb  malloc_function,
                                    CyaSSL_Free_cb    free_function,
                                    CyaSSL_Realloc_cb realloc_function);

/* Public in case user app wants to use XMALLOC/XFREE */
CYASSL_API void* CyaSSL_Malloc(size_t size);
CYASSL_API void  CyaSSL_Free(void *ptr);
CYASSL_API void* CyaSSL_Realloc(void *ptr, size_t size);


#ifdef __cplusplus
}
#endif

#endif /* CYASSL_MEMORY_H */
