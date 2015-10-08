/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    memory.c
  
  Summary:
    Crypto Framework Libarary source for cryptographic functions.

  Description:
    This source file contains functions that make up the Cryptographic 
	Framework Library for PIC32 families of Microchip microcontrollers.
**************************************************************************/

//DOM-IGNORE-BEGIN
/******************************************************************************
File Name:  memory.c
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

#ifdef USE_CYASSL_MEMORY

#include "crypto/src/memory.h"
#include "crypto/src/error.h"

#ifdef CYASSL_MALLOC_CHECK
    #include <stdio.h>
#endif

/* Set these to default values initially. */
static CyaSSL_Malloc_cb  malloc_function = 0;
static CyaSSL_Free_cb    free_function = 0;
static CyaSSL_Realloc_cb realloc_function = 0;

int CyaSSL_SetAllocators(CyaSSL_Malloc_cb  mf,
                         CyaSSL_Free_cb    ff,
                         CyaSSL_Realloc_cb rf)
{
    int res = 0;

    if (mf)
        malloc_function = mf;
	else
        res = BAD_FUNC_ARG;

    if (ff)
        free_function = ff;
    else
        res = BAD_FUNC_ARG;

    if (rf)
        realloc_function = rf;
    else
        res = BAD_FUNC_ARG;

    return res;
}


void* CyaSSL_Malloc(size_t size)
{
    void* res = 0;

    if (malloc_function)
        res = malloc_function(size);
    else
        res = malloc(size);

    #ifdef CYASSL_MALLOC_CHECK
        if (res == NULL)
            puts("CyaSSL_malloc failed");
    #endif
				
    return res;
}

void CyaSSL_Free(void *ptr)
{
    if (free_function)
        free_function(ptr);
    else
        free(ptr);
}

void* CyaSSL_Realloc(void *ptr, size_t size)
{
    void* res = 0;

    if (realloc_function)
        res = realloc_function(ptr, size);
    else
        res = realloc(ptr, size);

    return res;
}

#endif /* USE_CYASSL_MEMORY */
