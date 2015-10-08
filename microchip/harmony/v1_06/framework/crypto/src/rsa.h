/**************************************************************************
  Crypto Framework Library Header

  Company:
    Microchip Technology Inc.

  File Name:
    rsa.h
  
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



#ifndef NO_RSA

#ifndef CTAO_CRYPT_RSA_H
#define CTAO_CRYPT_RSA_H

#include "crypto/src/types.h"
#include "crypto/src/integer.h"
#include "crypto/src/random.h"

#ifdef __cplusplus
    extern "C" {
#endif

#define CYASSL_RSA_CAVIUM_MAGIC 0xBEEF0006

enum {
    RSA_PUBLIC   = 0,
    RSA_PRIVATE  = 1
};

/* RSA */
typedef struct RsaKey {
    mp_int n, e, d, p, q, dP, dQ, u;
    int   type;                               /* public or private */
    void* heap;                               /* for user memory overrides */
#ifdef HAVE_CAVIUM
    int    devId;           /* nitrox device id */
    word32 magic;           /* using cavium magic */
    word64 contextHandle;   /* nitrox context memory handle */
    byte*  c_n;             /* cavium byte buffers for key parts */
    byte*  c_e;
    byte*  c_d;
    byte*  c_p;
    byte*  c_q;
    byte*  c_dP;
    byte*  c_dQ;
    byte*  c_u;             /* sizes in bytes */
    word16 c_nSz, c_eSz, c_dSz, c_pSz, c_qSz, c_dP_Sz, c_dQ_Sz, c_uSz;
#endif
} RsaKey;


CYASSL_API void InitRsaKey(RsaKey* key, void*);
CYASSL_API void FreeRsaKey(RsaKey* key);

CYASSL_API int  RsaPublicEncrypt(const byte* in, word32 inLen, byte* out,
                                 word32 outLen, RsaKey* key, RNG* rng);
CYASSL_API int  RsaPrivateDecryptInline(byte* in, word32 inLen, byte** out,
                                        RsaKey* key);
CYASSL_API int  RsaPrivateDecrypt(const byte* in, word32 inLen, byte* out,
                                  word32 outLen, RsaKey* key);
CYASSL_API int  RsaSSL_Sign(const byte* in, word32 inLen, byte* out,
                            word32 outLen, RsaKey* key, RNG* rng);
CYASSL_API int  RsaSSL_VerifyInline(byte* in, word32 inLen, byte** out,
                                    RsaKey* key);
CYASSL_API int  RsaSSL_Verify(const byte* in, word32 inLen, byte* out,
                              word32 outLen, RsaKey* key);
CYASSL_API int  RsaEncryptSize(RsaKey* key);

CYASSL_API int RsaPrivateKeyDecode(const byte* input, word32* inOutIdx, RsaKey*,
                                   word32);
CYASSL_API int RsaPublicKeyDecode(const byte* input, word32* inOutIdx, RsaKey*,
                                  word32);
#ifdef CYASSL_KEY_GEN
    CYASSL_API int MakeRsaKey(RsaKey* key, int size, long e, RNG* rng);
    CYASSL_API int RsaKeyToDer(RsaKey*, byte* output, word32 inLen);
#endif

#ifdef HAVE_CAVIUM
    CYASSL_API int  RsaInitCavium(RsaKey*, int);
    CYASSL_API void RsaFreeCavium(RsaKey*);
#endif


#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* CTAO_CRYPT_RSA_H */

#endif /* NO_RSA */
