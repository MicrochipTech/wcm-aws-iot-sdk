/* fips_test.c
 *
 * Copyright (C) 2006-2014 wolfSSL Inc.
 *
 * This file is part of CyaSSL.
 *
 * CyaSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CyaSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */


#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

/* in case user set HAVE_FIPS there */
#include <cyassl/ctaocrypt/settings.h>

#ifdef HAVE_FIPS

#include <cyassl/ctaocrypt/fips_test.h>
#include <cyassl/ctaocrypt/aes.h>
#include <cyassl/ctaocrypt/des3.h>
#include <cyassl/ctaocrypt/sha.h>
#include <cyassl/ctaocrypt/sha256.h>
#include <cyassl/ctaocrypt/sha512.h>
#include <cyassl/ctaocrypt/rsa.h>
#include <cyassl/ctaocrypt/hmac.h>
#include <cyassl/ctaocrypt/error-crypt.h>
#include <cyassl/ctaocrypt/coding.h>


#define USE_CERT_BUFFERS_2048
#include <cyassl/certs_test.h>   /* rsa 2048 bit key */


#ifdef USE_WINDOWS_API
    #pragma code_seg(".fipsA$k")
    #pragma const_seg(".fipsB$k")
#endif


extern const unsigned int wolfCrypt_FIPS_ro_start[];
extern const unsigned int wolfCrypt_FIPS_ro_end[];


int wolfCrypt_FIPS_first(void);
int wolfCrypt_FIPS_last(void);

typedef int (*fips_address_function)(void);


/* sanity size checks */
#define MAX_FIPS_DATA_SZ  100000
#define MAX_FIPS_CODE_SZ 1000000

static int GenBase16_Hash(const byte* in, int length, char* out, int outSz);

/* free up to 5 binary buffers */
static void FreeBins(byte* b1, byte* b2, byte* b3, byte* b4, byte* b5)
{
   XFREE(b1, NULL, DYNAMIC_TYPE_TMP_BUFFER); 
   XFREE(b2, NULL, DYNAMIC_TYPE_TMP_BUFFER); 
   XFREE(b3, NULL, DYNAMIC_TYPE_TMP_BUFFER); 
   XFREE(b4, NULL, DYNAMIC_TYPE_TMP_BUFFER); 
   XFREE(b5, NULL, DYNAMIC_TYPE_TMP_BUFFER); 
}


/* convert hex string to binary, store size, 0 success (free mem on failure) */
static int ConvertHexToBin(const char* h1, byte** b1, word32* b1Sz,
                           const char* h2, byte** b2, word32* b2Sz,
                           const char* h3, byte** b3, word32* b3Sz,
                           const char* h4, byte** b4, word32* b4Sz)
{
    int ret;

    /* b1 */
    if (h1 && b1 && b1Sz) {
        *b1Sz = (int)XSTRLEN(h1) / 2;
        *b1   = (byte*)XMALLOC(*b1Sz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (*b1 == NULL)
            return MEMORY_E;
        ret = Base16_Decode((const byte*)h1, (int)XSTRLEN(h1), *b1, b1Sz);
        if (ret != 0) {
            FreeBins(*b1, NULL, NULL, NULL, NULL);
            return ret;
        }
    }

    /* b2 */
    if (h2 && b2 && b2Sz) {
        *b2Sz = (int)XSTRLEN(h2) / 2;
        *b2   = (byte*)XMALLOC(*b2Sz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (*b2 == NULL) {
            FreeBins(b1 ? *b1 : NULL, NULL, NULL, NULL, NULL);
            return MEMORY_E;
        }
        ret = Base16_Decode((const byte*)h2, (int)XSTRLEN(h2), *b2, b2Sz);
        if (ret != 0) {
            FreeBins(b1 ? *b1 : NULL, *b2, NULL, NULL, NULL);
            return ret;
        }
    }

    /* b3 */
    if (h3 && b3 && b3Sz) {
        *b3Sz = (int)XSTRLEN(h3) / 2;
        *b3   = (byte*)XMALLOC(*b3Sz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (*b3 == NULL) {
            FreeBins(b1 ? *b1 : NULL, b2 ? *b2 : NULL, NULL, NULL, NULL);
            return MEMORY_E;
        }
        ret = Base16_Decode((const byte*)h3, (int)XSTRLEN(h3), *b3, b3Sz);
        if (ret != 0) {
            FreeBins(b1 ? *b1 : NULL, b2 ? *b2 : NULL, *b3, NULL, NULL);
            return ret;
        }
    }

    /* b4 */
    if (h4 && b4 && b4Sz) {
        *b4Sz = (int)XSTRLEN(h4) / 2;
        *b4   = (byte*)XMALLOC(*b4Sz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (*b4 == NULL) {
            FreeBins(b1 ? *b1 : NULL,b2 ? *b2 : NULL,b3 ? *b3 : NULL,NULL,NULL);
            return MEMORY_E;
        }
        ret = Base16_Decode((const byte*)h4, (int)XSTRLEN(h4), *b4, b4Sz);
        if (ret != 0) {
            FreeBins(b1 ? *b1 : NULL, b2 ? *b2 : NULL, b3 ? *b3 : NULL, *b4,
                     NULL);
            return ret;
        }
    }

    return 0;
}



/* 0 on success */
static int AesKnownAnswerTest(const char* key, const char* iv,
                              const char* plainText, const char* cbc)
{
    Aes   aes;
    byte* binKey   = NULL;
    byte* binIv    = NULL;
    byte* binPlain = NULL;
    byte* binCbc   = NULL;
    byte* cipher   = NULL;

    word32 keySz   = 0;
    word32 ivSz    = 0;
    word32 plainSz = 0;
    word32 cbcSz   = 0;

    int ret = ConvertHexToBin(key, &binKey, &keySz,
                              iv,  &binIv,  &ivSz,
                              plainText, &binPlain,  &plainSz,
                              cbc, &binCbc,  &cbcSz);
    if (ret != 0)
        return ret;

    cipher = (byte*)XMALLOC(plainSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (cipher == NULL) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return MEMORY_E;
    }

    ret = AesSetKey_fips(&aes, binKey, keySz, binIv, AES_ENCRYPTION);
    if (ret != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return ret;
    }

    ret = AesCbcEncrypt_fips(&aes, cipher, binPlain, plainSz);
    if (ret != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return ret;
    }

    if (XMEMCMP(cipher, binCbc, plainSz) != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return -1;
    }

    ret = AesSetKey_fips(&aes, binKey, keySz, binIv, AES_DECRYPTION);
    if (ret != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return ret;
    }

    /* decrypt cipher in place back to plain for verify */
    ret = AesCbcDecrypt_fips(&aes, cipher, cipher, plainSz);
    if (ret != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return ret;
    }

    if (XMEMCMP(cipher, binPlain, plainSz) != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return -1;
    }

    FreeBins(binKey, binIv, binPlain, binCbc, cipher);

    return 0; 
}


/* 0 on success */
static int AesGcm_KnownAnswerTest(int decrypt,
                                  const char* key, const char* iv,
                                  const char* plain, const char* auth,
                                  const char* cipher, const char* tag)
{
    Aes aes;
    byte* binKey    = NULL;
    byte* binIv     = NULL;
    byte* binPlain  = NULL;
    byte* binAuth   = NULL;
    byte* binCipher = NULL;
    byte* binTag    = NULL;
    byte* out       = NULL;
    byte* check     = NULL;

    word32 binKeySz, binIvSz, binPlainSz, binAuthSz, binCipherSz, binTagSz;

    int ret = ConvertHexToBin(key, &binKey, &binKeySz, iv, &binIv, &binIvSz,
                              NULL, NULL, NULL, NULL, NULL, NULL);
    if (ret != 0)
        return ret;

    ret = ConvertHexToBin(plain, &binPlain, &binPlainSz,
                          auth, &binAuth, &binAuthSz,
                          cipher, &binCipher, &binCipherSz,
                          tag, &binTag, &binTagSz);
    if (ret != 0) {
        FreeBins(binKey, binIv, NULL, NULL, NULL);
        return ret;
    }

    out = (byte*)XMALLOC(binPlainSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (out == NULL) {
        FreeBins(binKey, binIv, binPlain, binAuth, NULL);
        FreeBins(binCipher, binTag, out, check, NULL);
        return MEMORY_E;
    }

    ret = AesGcmSetKey_fips(&aes, binKey, binKeySz);
    if (ret != 0) {
        FreeBins(binKey, binIv, binPlain, binAuth, NULL);
        FreeBins(binCipher, binTag, out, check, NULL);
        return ret;
    }

    if (decrypt) {
        ret = AesGcmDecrypt_fips(&aes, out, binCipher,
                                 binCipherSz, binIv, binIvSz,
                                 binTag, binTagSz,
                                 binAuth, binAuthSz);
        if (ret != 0) {
            FreeBins(binKey, binIv, binPlain, binAuth, NULL);
            FreeBins(binCipher, binTag, out, check, NULL);
            return ret;
        }

        if (XMEMCMP(binPlain, out, binPlainSz) != 0) {
            FreeBins(binKey, binIv, binPlain, binAuth, NULL);
            FreeBins(binCipher, binTag, out, check, NULL);
            return -1;
        }
    }
    else {
        check = (byte*)XMALLOC(binTagSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (check == NULL) {
            FreeBins(binKey, binIv, binPlain, binAuth, NULL);
            FreeBins(binCipher, binTag, out, check, NULL);
            return MEMORY_E;
        }

        ret = AesGcmEncrypt_fips(&aes, out, binPlain, binPlainSz,
                                 binIv, binIvSz,
                                 check, binTagSz,
                                 binAuth, binAuthSz);

        if (ret != 0) {
            FreeBins(binKey, binIv, binPlain, binAuth, NULL);
            FreeBins(binCipher, binTag, out, check, NULL);
            return -1;
        }

        if (XMEMCMP(binCipher, out, binCipherSz) != 0 &&
            XMEMCMP(binTag, check, binTagSz) != 0) {

            FreeBins(binKey, binIv, binPlain, binAuth, NULL);
            FreeBins(binCipher, binTag, out, check, NULL);
            return -1;
        }
    }

    FreeBins(binKey, binIv, binPlain, binAuth, NULL);
    FreeBins(binCipher, binTag, out, check, NULL);
    return 0;
}


/* 0 on success */
static int Des3_KnownAnswerTest(const char* key, const char* iv,
                                const char* plainText, const char* cbc)
{
    Des3  des3;
    byte* binKey   = NULL;
    byte* binIv    = NULL;
    byte* binPlain = NULL;
    byte* binCbc   = NULL;
    byte* cipher   = NULL;

    word32 keySz   = 0;
    word32 ivSz    = 0;
    word32 plainSz = 0;
    word32 cbcSz   = 0;

    int ret = ConvertHexToBin(key, &binKey, &keySz,
                              iv,  &binIv,  &ivSz,
                              plainText, &binPlain,  &plainSz,
                              cbc, &binCbc,  &cbcSz);
    if (ret != 0)
        return ret;

    cipher = (byte*)XMALLOC(plainSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (cipher == NULL) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return MEMORY_E;
    }

    ret = Des3_SetKey_fips(&des3, binKey, binIv, DES_ENCRYPTION);
    if (ret != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return ret;
    }

    ret = Des3_CbcEncrypt_fips(&des3, cipher, binPlain, plainSz);
    if (ret != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return ret;
    }

    if (XMEMCMP(cipher, binCbc, plainSz) != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return -1;
    }

    ret = Des3_SetKey_fips(&des3, binKey, binIv, DES_DECRYPTION);
    if (ret != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return ret;
    }

    /* decrypt cipher in place back to plain for verify */
    ret = Des3_CbcDecrypt_fips(&des3, cipher, cipher, plainSz);
    if (ret != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return ret;
    }

    if (XMEMCMP(cipher, binPlain, plainSz) != 0) {
        FreeBins(binKey, binIv, binPlain, binCbc, cipher);
        return -1;
    }

    FreeBins(binKey, binIv, binPlain, binCbc, cipher);

    return 0; 
}


/* 0 on success */
static int HMAC_KnownAnswerTest(int type, const char* key, const char* msg,
                                const char* digest)
{
    Hmac        hmac;
    const byte* binMsg    = (const byte*)msg;
    byte*       binDigest = NULL;
    byte*       binKey    = NULL;
    byte        final[MAX_DIGEST_SIZE];

    word32 msgSz    = (word32)XSTRLEN(msg);
    word32 digestSz = 0;
    word32 keySz    = 0;

    int ret = ConvertHexToBin(digest, &binDigest, &digestSz,
                              key, &binKey, &keySz,
                              NULL, NULL, NULL,
                              NULL, NULL, NULL);
    if (ret != 0)
        return ret;

    ret = HmacSetKey_fips(&hmac, type, binKey, keySz);
    if (ret != 0) {
        FreeBins(binDigest, binKey, NULL, NULL, NULL);
        return ret;
    }

    ret = HmacUpdate_fips(&hmac, binMsg, msgSz);
    if (ret != 0) {
        FreeBins(binDigest, NULL, NULL, NULL, NULL);
        return ret;
    }

    ret = HmacFinal_fips(&hmac, final);
    if (ret != 0) {
        FreeBins(binDigest, binKey, NULL, NULL, NULL);
        return ret;
    }

    if (XMEMCMP(final, binDigest, digestSz) != 0) {
        FreeBins(binDigest, binKey, NULL, NULL, NULL);
        return -1;
    }

    FreeBins(binDigest, binKey, NULL, NULL, NULL);

    return 0;
}


/* 0 on success */
static int DRBG_KnownAnswerTest(int reseed, const char* entropyA,
                                const char* entropyB, const char* output)
{
    byte* binEntropyA = NULL;
    byte* binEntropyB = NULL;
    byte* binOutput = NULL;
    word32 binEntropyASz;
    word32 binEntropyBSz;
    word32 binOutputSz;

    byte check[SHA256_DIGEST_SIZE * 4];

    int ret = ConvertHexToBin(entropyA, &binEntropyA, &binEntropyASz,
                              entropyB, &binEntropyB, &binEntropyBSz,
                              output, &binOutput, &binOutputSz,
                              NULL, NULL, NULL);

    if (ret != 0)
        return ret;

    /* Test Body */
    ret = RNG_HealthTest_fips(reseed, binEntropyA, binEntropyASz,
                                      binEntropyB, binEntropyBSz,
                                      check, sizeof(check));
    if (ret != 0) {
        FreeBins(binEntropyA, binEntropyB, binOutput, NULL, NULL);
        return ret;
    }

    if (XMEMCMP(binOutput, check, sizeof(check)) != 0) {
        FreeBins(binEntropyA, binEntropyB, binOutput, NULL, NULL);
        return -1;
    }

    FreeBins(binEntropyA, binEntropyB, binOutput, NULL, NULL);

    return 0;
}


/* dead simple base16 encoder, 0 on success */
static int GenBase16_Hash(const byte* in, int length, char* out, int outSz)
{
    int i;
    int index = 0;

    if ( (length * 2 + 1) > outSz)
        return -1;

    for (i = 0; i < length; i++) {
        byte a = in[i] >> 4;
        byte b = in[i] & 0x0f;

        if (a > 9)
            a+=7;
        if (b > 9)
            b+=7;

        out[index++] = a + 0x30;
        out[index++] = b + 0x30;
    }
    out[index++] = '\0';

    return 0;
}


static int RsaSignPKCS1v15_KnownAnswerTest(int type, const char* msg,
                                           const char* sig)
{
    RsaKey      rsa;
    const byte* binMsg = (const byte*)msg;
    byte*       binSig = NULL;
    byte*       result = NULL;
    byte        final[MAX_DIGEST_SIZE];
    byte        verify[MAX_DIGEST_SIZE];
    word32 msgSz = (word32)XSTRLEN(msg);
    word32 sigSz = 0;
    word32 digestSz = 0;
    word32 verifySz = (word32)sizeof(verify);
    word32 resultSz = 0;
    word32 idx      = 0;

    int ret = ConvertHexToBin(sig, &binSig, &sigSz,
                              NULL, NULL, NULL,
                              NULL, NULL, NULL,
                              NULL, NULL, NULL);
    if (ret != 0)
        return ret;

    result = (byte*)XMALLOC(sigSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (result == NULL) {
        FreeBins(binSig, NULL, NULL, NULL, NULL);
        return MEMORY_E;
    }
    resultSz = sigSz;

    ret = InitRsaKey_fips(&rsa, NULL);
    if (ret != 0) {
        FreeBins(binSig, result, NULL, NULL, NULL);
        return ret;
    }

    switch (type) {
        case SHA256 :
        {
            Sha256 sha256;

            InitSha256_fips(&sha256);
            Sha256Update_fips(&sha256, binMsg, msgSz);
            Sha256Final_fips(&sha256, final);
            digestSz = SHA256_DIGEST_SIZE;

            break;
        } 

        default:
            FreeRsaKey_fips(&rsa);
            FreeBins(binSig, result, NULL, NULL, NULL);
            return -1;
    }

    ret = RsaPrivateKeyDecode_fips(client_key_der_2048, &idx, &rsa,
                                   sizeof_client_key_der_2048);
    if (ret != 0) {
        FreeRsaKey_fips(&rsa);
        FreeBins(binSig, result, NULL, NULL, NULL);
        return ret;
    }

    ret = RsaSSL_Sign_fips(final, digestSz, result, resultSz, &rsa, NULL);
    if (ret != (int)sigSz) {
        FreeRsaKey_fips(&rsa);
        FreeBins(binSig, result, NULL, NULL, NULL);
        return ret;
    }

    if (XMEMCMP(result, binSig, sigSz) != 0) {
        FreeRsaKey_fips(&rsa);
        FreeBins(binSig, result, NULL, NULL, NULL);
        return -1;
    }

    ret = RsaSSL_Verify_fips(result, sigSz, verify, verifySz, &rsa);
    if (ret != (int)digestSz) {
        FreeRsaKey_fips(&rsa);
        FreeBins(binSig, result, NULL, NULL, NULL);
        return ret;
    }

    if (XMEMCMP(verify, final, digestSz) != 0) {
        FreeRsaKey_fips(&rsa);
        FreeBins(binSig, result, NULL, NULL, NULL);
        return -1;
    }

    FreeRsaKey_fips(&rsa);
    FreeBins(binSig, result, NULL, NULL, NULL);

    return 0;
}


/* hmac-sha256 in memory core verify hash, output to pos callback,
 * copy here when changes */
static const char verifyCore[] =
"15F4F61336FF1B485C1E720F0B45FFB2BF065D80B4E9732FF62C804EA881DD66";


/* Do in core memory integrity check, 0 on success */
static int DoInCoreCheck(char* base16_hash, int base16_hashSz)
{
    Hmac     hmac;
    byte     hash[SHA256_DIGEST_SIZE];
    byte*    binVerify = NULL;
    byte*    binCoreKey = NULL;
    word32   verifySz  = 0;
    word32   binCoreSz  = 0;
    int      ret;
    const char coreKey[] =
             "EAD92D38850B03D7160EA4F5C90EDD49C0B6933FAA4833213ED6F0F1596E356B";

    fips_address_function first = wolfCrypt_FIPS_first;
    fips_address_function last  = wolfCrypt_FIPS_last;

    char* start = (char*)wolfCrypt_FIPS_ro_start;
    char* end   = (char*)wolfCrypt_FIPS_ro_end;

    unsigned long code_sz = (unsigned long)last - (unsigned long)first;
    unsigned long data_sz = (unsigned long)end - (unsigned long)start;

    if (data_sz > MAX_FIPS_DATA_SZ)
        return -1;  /* bad fips data size */

    if (code_sz > MAX_FIPS_CODE_SZ)
        return -1;  /* bad fips code size */

    ret = ConvertHexToBin(coreKey, &binCoreKey, &binCoreSz,
                          NULL, NULL, NULL,
                          NULL, NULL, NULL,
                          NULL, NULL, NULL);
    if (ret != 0) return ret;

    HmacSetKey_fips(&hmac, SHA256, binCoreKey, binCoreSz);
    FreeBins(binCoreKey, NULL, NULL, NULL, NULL);

    HmacUpdate_fips(&hmac, (byte*)first, (word32)code_sz);

    /* don't hash verifyCore or changing verifyCore will change hash */
    if (verifyCore >= start && verifyCore < end) {
        data_sz = (unsigned long)verifyCore - (unsigned long)start;
        HmacUpdate_fips(&hmac, (byte*)start, (word32)data_sz);
        start   = (char*)verifyCore + sizeof(verifyCore);
        data_sz = (unsigned long)end - (unsigned long)start;
    }
    HmacUpdate_fips(&hmac, (byte*)start, (word32)data_sz);

    HmacFinal_fips(&hmac, hash);

    ret = GenBase16_Hash(hash, sizeof(hash), base16_hash, base16_hashSz);
    if (ret != 0)
        return ret;

    ret = ConvertHexToBin(verifyCore, &binVerify, &verifySz,
                          NULL, NULL, NULL,
                          NULL, NULL, NULL,
                          NULL, NULL, NULL);
    if (ret != 0)
        return ret;

    if (XMEMCMP(hash, binVerify, sizeof(hash)) != 0) {
        ret = -1;
    }
    FreeBins(binVerify, NULL, NULL, NULL, NULL);

    return ret;
}


/* do all tests, 0 on success */
int DoKnownAnswerTests(char* base16_hash, int base16_hashSz)
{
    if (DoInCoreCheck(base16_hash, base16_hashSz) != 0) {
        return IN_CORE_FIPS_E;
    }

    if (AesKnownAnswerTest(
             "2b7e151628aed2a6abf7158809cf4f3c",  /* key */
             "000102030405060708090a0b0c0d0e0f",  /* iv */
             "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac" /* plainText */
             "9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a"
             "52eff69f2445df4f9b17ad2b417be66c3710",
             "7649abac8119b246cee98e9b12e9197d5086cb9b507219" /* cbc */
             "ee95db113a917678b273bed6b8e3c1743b7116e69e2222"
             "95163ff1caa1681fac09120eca307586e1a7"
             ) != 0) {
        return AES_KAT_FIPS_E;
    }

    if (AesGcm_KnownAnswerTest(0,
             "298efa1ccf29cf62ae6824bfc19557fc",                /* key */
             "6f58a93fe1d207fae4ed2f6d",                        /* iv */
             "cc38bccd6bc536ad919b1395f5d63801f99f8068d65ca5ac" /* plain */
             "63872daf16b93901",
             "021fafd238463973ffe80256e5b1c6b1",                /* auth */
             "dfce4e9cd291103d7fe4e63351d9e79d3dfd391e32671046" /* cipher */
             "58212da96521b7db",
             "542465ef599316f73a7a560509a2d9f2"                 /* tag */
             ) != 0) {
        return AESGCM_KAT_FIPS_E;
    }

    if (AesGcm_KnownAnswerTest(1,
             "afa272c03d0343f882008f6e163d6047",                /* key */
             "271ba21f8fdcac34dc93be54",                        /* iv */
             "f3ee01423f192c36033542221c5545dd939de52ada18b9e8" /* plain */
             "b72ba17d02c5dddd",
             "cdf5496a50214683304aec0a80337f9a",                /* auth */
             "36a4029c9e7d0307d31c29cea885bb6c8022452016a29754" /* cipher */
             "ba8a344c5bbfc3e1",
             "ed8d916c171f0688d7e7cca547ab3ab2"                 /* tag */
             ) != 0) {
        return AESGCM_KAT_FIPS_E;
    }

    if (Des3_KnownAnswerTest(
			"385D7189A5C3D485E1370AA5D408082B5CCCCB5E19F2D90E",  /* key */
			"C141B5FCCD28DC8A",                                  /* iv  */
			"6E1BD7C6120947A464A6AAB293A0F89A563D8D40D3461B68",  /* plain */
			"6235A461AFD312973E3B4F7AA7D23E34E03371F8E8C376C9"   /* cbc */
             ) != 0) {
        return DES3_KAT_FIPS_E;
    }

    if (HMAC_KnownAnswerTest(SHA,                             /* type */
            "303132333435363738393a3b3c3d3e3f40414243",       /* key */
            "Sample #2",                                      /* msg */
            "0922D3405FAA3D194F82A45830737D5CC6C75D24"        /* digest */
            ) != 0) {
        return HMAC_KAT_FIPS_E;
    }

    if (HMAC_KnownAnswerTest(SHA512,                          /* type */
            "303132333435363738393a3b3c3d3e3f40414243",       /* key */
            "Sample #2",                                      /* msg */
            "809d44057c5b954105bd041316db0fac44d5a4d5d0892bd04e866412c0907768"
            "f187b77c4fae2c2f21a5b5659a4f4ba74702a3de9b51f145bd4f252742989905"
            /* digest */
            ) != 0) {
        return HMAC_KAT_FIPS_E;
    }

    if (RsaSignPKCS1v15_KnownAnswerTest(SHA256,               /* type */
            "Everyone gets Friday off.",                      /* msg */
            "8CFA57979578B9D781C7F7EEDD21E962FC45D8B7CCDA68837"
            "D84E8345973856089C025A06F89F77D7C3694C483A6EF6B42"
            "EE69B8C2E01CC113F137F498890752EF6C6094D3819979122"
            "7928ED82D5BB50FB96A754F977D66FE75ABCF70F5D9448352"
            "26D30BF6F62D7B9CAFFA18179C5DABCE58BA497424A5AC8D6"
            "11814B726CF3294D0C238000DC2B775791925CA528F6B4947"
            "D3E4BA1F8CDF4C3E88E1AA2FCDAE461F6DF245DD3C39F980F"
            "D0FEC213FCB7B7D1679F4689D08538E16A8E0F357BADFD1F0"
            "D56C635B9E6E7CBD6E2F32F347AB9E07685166016EEF8F857"
            "37542185635688469BC08AF743B02B5C6FB5CED8924B20C14"
            "7B9F349FAA1943DBF677CA"
            /* signature */
            ) != 0) {
        return RSA_KAT_FIPS_E;
    }

    if (DRBG_KnownAnswerTest(0,
            "a65ad0f345db4e0effe875c3a2e71f42"
            "c7129d620ff5c119a9ef55f05185e0fb"
            "8581f9317517276e06e9607ddbcbcc2e", /* entropy + nonce input */
            NULL,                               /* no reseed */
            "d3e160c35b99f340b2628264d1751060"
            "e0045da383ff57a57d73a673d2b8d80d"
            "aaf6a6c35a91bb4579d73fd0c8fed111"
            "b0391306828adfed528f018121b3febd"
            "c343e797b87dbb63db1333ded9d1ece1"
            "77cfa6b71fe8ab1da46624ed6415e51c"
            "cde2c7ca86e283990eeaeb9112041552"
            "8b2295910281b02dd431f4c9f70427df"  /* pseudorandom output */
            ) != 0) {
        return DRBG_KAT_FIPS_E;
    }

    if (DRBG_KnownAnswerTest(1,
            "63363377e41e86468deb0ab4a8ed683f"
            "6a134e47e014c700454e81e95358a569"
            "808aa38f2a72a62359915a9f8a04ca68", /* entropy + nonce input */
            "e62b8a8ee8f141b6980566e3bfe3c049"
            "03dad4ac2cdf9f2280010a6739bc83d3", /* reseed entropy input */
            "04eec63bb231df2c630a1afbe724949d"
            "005a587851e1aa795e477347c8b05662"
            "1c18bddcdd8d99fc5fc2b92053d8cfac"
            "fb0bb8831205fad1ddd6c071318a6018"
            "f03b73f5ede4d4d071f9de03fd7aea10"
            "5d9299b8af99aa075bdb4db9aa28c18d"
            "174b56ee2a014d098896ff2282c955a8"
            "1969e069fa8ce007a180183a07dfae17"  /* pseudorandom output */
            ) != 0) {
        return DRBG_KAT_FIPS_E;
    }

    return 0;  /* success */
}



#endif /* HAVE_FIPS */
