/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    pwdbased.c
  
  Summary:
    Crypto Framework Libarary source for cryptographic functions.

  Description:
    This source file contains functions that make up the Cryptographic 
	Framework Library for PIC32 families of Microchip microcontrollers.
**************************************************************************/

//DOM-IGNORE-BEGIN
/******************************************************************************
File Name:  pwdbased.c
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

#ifndef NO_PWDBASED

#ifdef CYASSL_PIC32MZ_HASH

#define InitMd5   InitMd5_sw
#define Md5Update Md5Update_sw
#define Md5Final  Md5Final_sw

#define InitSha   InitSha_sw
#define ShaUpdate ShaUpdate_sw
#define ShaFinal  ShaFinal_sw

#define InitSha256   InitSha256_sw
#define Sha256Update Sha256Update_sw
#define Sha256Final  Sha256Final_sw

#endif

#include "crypto/src/pwdbased.h"
#include "crypto/src/hmac.h"
#include "crypto/src/integer.h"
#include "crypto/src/error.h"
#if defined(CYASSL_SHA512) || defined(CYASSL_SHA384)
    #include "crypto/src/sha512.h"
#endif

#ifdef NO_INLINE
    #include "crypto/src/misc.h"
#else
    #include "crypto/src/misc.c"
#endif


#ifndef min

    static INLINE word32 min(word32 a, word32 b)
    {
        return a > b ? b : a;
    }

#endif /* min */


int PBKDF1(byte* output, const byte* passwd, int pLen, const byte* salt,
           int sLen, int iterations, int kLen, int hashType)
{
    Md5  md5;
    Sha  sha;
    int  hLen = (hashType == MD5) ? (int)MD5_DIGEST_SIZE : (int)SHA_DIGEST_SIZE;
    int  i;
    byte buffer[SHA_DIGEST_SIZE];  /* max size */

    if (hashType != MD5 && hashType != SHA)
        return BAD_FUNC_ARG;

    if (kLen > hLen)
        return BAD_FUNC_ARG;

    if (iterations < 1)
        return BAD_FUNC_ARG;

    if (hashType == MD5) {
        InitMd5(&md5);
        Md5Update(&md5, passwd, pLen);
        Md5Update(&md5, salt,   sLen);
        Md5Final(&md5,  buffer);
    }
    else {
        InitSha(&sha);
        ShaUpdate(&sha, passwd, pLen);
        ShaUpdate(&sha, salt,   sLen);
        ShaFinal(&sha,  buffer);
    }

    for (i = 1; i < iterations; i++) {
        if (hashType == MD5) {
            Md5Update(&md5, buffer, hLen);
            Md5Final(&md5,  buffer);
        }
        else {
            ShaUpdate(&sha, buffer, hLen);
            ShaFinal(&sha,  buffer);
        }
    }
    XMEMCPY(output, buffer, kLen);

    return 0;
}


int PBKDF2(byte* output, const byte* passwd, int pLen, const byte* salt,
           int sLen, int iterations, int kLen, int hashType)
{
    word32 i = 1;
    int    hLen;
    int    j;
    Hmac   hmac;
    byte   buffer[MAX_DIGEST_SIZE];

    if (hashType == MD5) {
        hLen = MD5_DIGEST_SIZE;
    }
    else if (hashType == SHA) {
        hLen = SHA_DIGEST_SIZE;
    }
#ifndef NO_SHA256
    else if (hashType == SHA256) {
        hLen = SHA256_DIGEST_SIZE;
    }
#endif
#ifdef CYASSL_SHA512
    else if (hashType == SHA512) {
        hLen = SHA512_DIGEST_SIZE;
    }
#endif
    else
        return BAD_FUNC_ARG;

    HmacSetKey(&hmac, hashType, passwd, pLen);

    while (kLen) {
        int currentLen;
        HmacUpdate(&hmac, salt, sLen);

        /* encode i */
        for (j = 0; j < 4; j++) {
            byte b = (byte)(i >> ((3-j) * 8));
            HmacUpdate(&hmac, &b, 1);
        }
        HmacFinal(&hmac, buffer);

        currentLen = min(kLen, hLen);
        XMEMCPY(output, buffer, currentLen);

        for (j = 1; j < iterations; j++) {
            HmacUpdate(&hmac, buffer, hLen);
            HmacFinal(&hmac, buffer);
            xorbuf(output, buffer, currentLen);
        }

        output += currentLen;
        kLen   -= currentLen;
        i++;
    }

    return 0;
}


int PKCS12_PBKDF(byte* output, const byte* passwd, int passLen,const byte* salt,
                 int saltLen, int iterations, int kLen, int hashType, int id)
{
    /* all in bytes instead of bits */
    word32 u, v, dLen, pLen, iLen, sLen, totalLen;
    int    dynamic = 0;
    int    ret = 0;
    int    i;
    byte   *D, *S, *P, *I;
    byte   staticBuffer[1024];
    byte*  buffer = staticBuffer;
#ifdef CYASSL_SHA512
    byte   Ai[SHA512_DIGEST_SIZE];
    byte   B[SHA512_BLOCK_SIZE];
#elif !defined(NO_SHA256)
    byte   Ai[SHA256_DIGEST_SIZE];
    byte   B[SHA256_BLOCK_SIZE];
#else
    byte   Ai[SHA_DIGEST_SIZE];
    byte   B[SHA_BLOCK_SIZE];
#endif

    if (!iterations)
        iterations = 1;

    if (hashType == MD5) {
        v = MD5_BLOCK_SIZE;
        u = MD5_DIGEST_SIZE;
    }
    else if (hashType == SHA) {
        v = SHA_BLOCK_SIZE;
        u = SHA_DIGEST_SIZE;
    }
#ifndef NO_SHA256
    else if (hashType == SHA256) {
        v = SHA256_BLOCK_SIZE;
        u = SHA256_DIGEST_SIZE;
    }
#endif
#ifdef CYASSL_SHA512
    else if (hashType == SHA512) {
        v = SHA512_BLOCK_SIZE;
        u = SHA512_DIGEST_SIZE;
    }
#endif
    else
        return BAD_FUNC_ARG; 

    dLen = v;
    sLen =  v * ((saltLen + v - 1) / v);
    if (passLen)
        pLen = v * ((passLen + v - 1) / v);
    else
        pLen = 0;
    iLen = sLen + pLen;

    totalLen = dLen + sLen + pLen;

    if (totalLen > sizeof(staticBuffer)) {
        buffer = (byte*)XMALLOC(totalLen, 0, DYNAMIC_TYPE_KEY);
        if (buffer == NULL) return MEMORY_E;
        dynamic = 1;
    } 

    D = buffer;
    S = D + dLen;
    P = S + sLen;
    I = S;

    XMEMSET(D, id, dLen);

    for (i = 0; i < (int)sLen; i++)
        S[i] = salt[i % saltLen];
    for (i = 0; i < (int)pLen; i++)
        P[i] = passwd[i % passLen];

    while (kLen > 0) {
        word32 currentLen;
        mp_int B1;

        if (hashType == MD5) {
            Md5 md5;

            InitMd5(&md5);
            Md5Update(&md5, buffer, totalLen);
            Md5Final(&md5, Ai);

            for (i = 1; i < iterations; i++) {
                Md5Update(&md5, Ai, u);
                Md5Final(&md5, Ai);
            }
        }
        else if (hashType == SHA) {
            Sha sha;

            InitSha(&sha);
            ShaUpdate(&sha, buffer, totalLen);
            ShaFinal(&sha, Ai);

            for (i = 1; i < iterations; i++) {
                ShaUpdate(&sha, Ai, u);
                ShaFinal(&sha, Ai);
            }
        }
#ifndef NO_SHA256
        else if (hashType == SHA256) {
            Sha256 sha256;

            InitSha256(&sha256);
            Sha256Update(&sha256, buffer, totalLen);
            Sha256Final(&sha256, Ai);

            for (i = 1; i < iterations; i++) {
                Sha256Update(&sha256, Ai, u);
                Sha256Final(&sha256, Ai);
            }
        }
#endif
#ifdef CYASSL_SHA512
        else if (hashType == SHA512) {
            Sha512 sha512;

            InitSha512(&sha512);
            Sha512Update(&sha512, buffer, totalLen);
            Sha512Final(&sha512, Ai);

            for (i = 1; i < iterations; i++) {
                Sha512Update(&sha512, Ai, u);
                Sha512Final(&sha512, Ai);
            }
        }
#endif

        for (i = 0; i < (int)v; i++)
            B[i] = Ai[i % u];

        if (mp_init(&B1) != MP_OKAY)
            ret = MP_INIT_E;
        else if (mp_read_unsigned_bin(&B1, B, v) != MP_OKAY)
            ret = MP_READ_E;
        else if (mp_add_d(&B1, (mp_digit)1, &B1) != MP_OKAY)
            ret = MP_ADD_E;

        if (ret != 0) {
            mp_clear(&B1);
            break;
        }

        for (i = 0; i < (int)iLen; i += v) {
            int    outSz;
            mp_int i1;
            mp_int res;

            if (mp_init_multi(&i1, &res, NULL, NULL, NULL, NULL) != MP_OKAY) {
                ret = MP_INIT_E;
                break;
            }
            if (mp_read_unsigned_bin(&i1, I + i, v) != MP_OKAY)
                ret = MP_READ_E;
            else if (mp_add(&i1, &B1, &res) != MP_OKAY)
                ret = MP_ADD_E;
            else if ( (outSz = mp_unsigned_bin_size(&res)) < 0)
                ret = MP_TO_E;
            else {
                if (outSz > (int)v) {
                    /* take off MSB */
                    byte  tmp[129];
                    ret = mp_to_unsigned_bin(&res, tmp);
                    XMEMCPY(I + i, tmp + 1, v);
                }
                else if (outSz < (int)v) {
                    XMEMSET(I + i, 0, v - outSz);
                    ret = mp_to_unsigned_bin(&res, I + i + v - outSz);
                }
                else
                    ret = mp_to_unsigned_bin(&res, I + i);
            }

            mp_clear(&i1);
            mp_clear(&res);
            if (ret < 0) break;
        }

        currentLen = min(kLen, (int)u);
        XMEMCPY(output, Ai, currentLen);
        output += currentLen;
        kLen   -= currentLen;
        mp_clear(&B1);
    }

    if (dynamic) XFREE(buffer, 0, DYNAMIC_TYPE_KEY);
    return ret;
}

#endif /* NO_PWDBASED */

