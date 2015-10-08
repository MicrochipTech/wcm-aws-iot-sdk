/**************************************************************************
  Crypto Framework Library Header

  Company:
    Microchip Technology Inc.

  File Name:
    random.h
  
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




#ifndef CTAO_CRYPT_RANDOM_H
#define CTAO_CRYPT_RANDOM_H

#include "crypto/src/types.h"

#ifndef NO_RC4
    #include "crypto/src/arc4.h"
#else
    #include "crypto/src/sha256.h"
#endif

#ifdef __cplusplus
    extern "C" {
#endif


#if defined(USE_WINDOWS_API)
    #if defined(_WIN64)
        typedef unsigned __int64 ProviderHandle;
        /* type HCRYPTPROV, avoid #include "crypto/src/windows.h" */
    #else
        typedef unsigned long ProviderHandle;
    #endif
#endif


/* OS specific seeder */
typedef struct OS_Seed {
    #if defined(USE_WINDOWS_API)
        ProviderHandle handle;
    #else
        int fd;
    #endif
} OS_Seed;


CYASSL_LOCAL
int GenerateSeed(OS_Seed* os, byte* seed, word32 sz);

#if defined(CYASSL_MDK_ARM)
#undef RNG
#define RNG CyaSSL_RNG   /* for avoiding name conflict in "stm32f2xx.h" */
#endif

#ifndef NO_RC4

#define CYASSL_RNG_CAVIUM_MAGIC 0xBEEF0004

/* secure Random Nnumber Generator */


typedef struct RNG {
    OS_Seed seed;
    Arc4    cipher;
#ifdef HAVE_CAVIUM
    int    devId;           /* nitrox device id */
    word32 magic;           /* using cavium magic */
#endif
} RNG;


#ifdef HAVE_CAVIUM
    CYASSL_API int  InitRngCavium(RNG*, int);
#endif

#else /* NO_RC4 */

#define DBRG_SEED_LEN (440/8)


/* secure Random Nnumber Generator */
typedef struct RNG {
    OS_Seed seed;

    Sha256 sha;
    byte digest[SHA256_DIGEST_SIZE];
    byte V[DBRG_SEED_LEN];
    byte C[DBRG_SEED_LEN];
    word64 reseed_ctr;
} RNG;

#endif

CYASSL_API int  InitRng(RNG*);
CYASSL_API void RNG_GenerateBlock(RNG*, byte*, word32 sz);
CYASSL_API byte RNG_GenerateByte(RNG*);

#ifdef NO_RC4
    CYASSL_API void FreeRng(RNG*);
#endif

#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* CTAO_CRYPT_RANDOM_H */

