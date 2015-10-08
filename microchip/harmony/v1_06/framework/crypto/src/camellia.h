/**************************************************************************
  Crypto Framework Library Header

  Company:
    Microchip Technology Inc.

  File Name:
    camellia.h
  
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




#ifdef HAVE_CAMELLIA

#ifndef CRYPTO_CAMELLIA_H
#define CRYPTO_CAMELLIA_H


#include "crypto/src/types.h"

#ifdef __cplusplus
    extern "C" {
#endif


enum {
    CAMELLIA_BLOCK_SIZE = 16
};

#define CAMELLIA_TABLE_BYTE_LEN 272
#define CAMELLIA_TABLE_WORD_LEN (CAMELLIA_TABLE_BYTE_LEN / sizeof(word32))

typedef word32 KEY_TABLE_TYPE[CAMELLIA_TABLE_WORD_LEN];

typedef struct Camellia {
    word32 keySz;
    KEY_TABLE_TYPE key;
    word32 reg[CAMELLIA_BLOCK_SIZE / sizeof(word32)]; /* for CBC mode */
    word32 tmp[CAMELLIA_BLOCK_SIZE / sizeof(word32)]; /* for CBC mode */
} Camellia;


CYASSL_API int  CamelliaSetKey(Camellia* cam,
                                   const byte* key, word32 len, const byte* iv);
CYASSL_API int  CamelliaSetIV(Camellia* cam, const byte* iv);
CYASSL_API void CamelliaEncryptDirect(Camellia* cam, byte* out, const byte* in);
CYASSL_API void CamelliaDecryptDirect(Camellia* cam, byte* out, const byte* in);
CYASSL_API void CamelliaCbcEncrypt(Camellia* cam,
                                          byte* out, const byte* in, word32 sz);
CYASSL_API void CamelliaCbcDecrypt(Camellia* cam,
                                          byte* out, const byte* in, word32 sz);


#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* CTAO_CRYPT_AES_H */
#endif /* HAVE_CAMELLIA */

