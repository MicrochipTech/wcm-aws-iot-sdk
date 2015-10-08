/* fips.c
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

#ifdef USE_WINDOWS_API
    #pragma code_seg(".fipsA$j")
    #pragma const_seg(".fipsB$j")
#endif


/* set FIPS_NO_WRAPPERS before headers, use direct internal f()s not wrappers */
#define FIPS_NO_WRAPPERS

#include <cyassl/ctaocrypt/aes.h>
#include <cyassl/ctaocrypt/des3.h>
#include <cyassl/ctaocrypt/sha.h>
#include <cyassl/ctaocrypt/sha256.h>
#include <cyassl/ctaocrypt/sha512.h>
#include <cyassl/ctaocrypt/hmac.h>
#include <cyassl/ctaocrypt/rsa.h>
#include <cyassl/ctaocrypt/fips_test.h>
#include <cyassl/ctaocrypt/random.h>
#include <cyassl/ctaocrypt/error-crypt.h>


#ifdef HAVE_FORCE_FIPS_FAILURE
    #include <stdio.h>
    static void FIPS_MSG(const char* msg)
    {
        printf("%s\n", msg);
    }
#else
    #define FIPS_MSG(m)
#endif


#ifdef USE_WINDOWS_API

    #define CCALL __cdecl
    #pragma section(".CRT$XCU",read)
    #define INITIALIZER(f) \
       static void __cdecl f(void); \
       __declspec(allocate(".CRT$XCU")) void (__cdecl*f##_)(void) = f; \
       static void __cdecl f(void)

#else

    #define INITIALIZER(f) static void __attribute__((constructor)) f(void)

#endif


/* power on self (pos) test status */
enum POS_STATUS {
    POS_NOT_DONE,    /* in progress, not complete yet */
    POS_FAILURE,     /* done, but failed  */
    POS_SUCCESS      /* done, and SUCCESS */
};

static enum POS_STATUS posStatus = POS_NOT_DONE;   /* our pos status */
static int             posReturn = 0;              /* pos return value */
static char base16_hash[SHA256_DIGEST_SIZE*2+1];   /* caclulated hash */


#ifdef HAVE_THREAD_LS
    /* Note: this thread local stuff doesn't work in pre-Vista DLLs.
     * Need to use TlsAlloc, etc, in that case. */

    static THREAD_LS_T int thisThreadInPOS = 0;          /* per thread in pos */

    static INLINE int InitThisThreadInPOS(void)
    {
        return 0;
    }

    static INLINE int GetThisThreadInPOS(void)
    {
        return thisThreadInPOS;
    }

    static INLINE void SetThisThreadInPOS(int inPOS)
    {
        thisThreadInPOS = inPOS;
    }

#else

    static pthread_key_t thisThreadInPOSKey;

    static INLINE int InitThisThreadInPOS(void)
    {
        int* thisThreadInPOS;

        if (pthread_key_create(&thisThreadInPOSKey, NULL) != 0)
            return THREAD_STORE_KEY_E;

        thisThreadInPOS = (int*)malloc(sizeof(int));
        if (thisThreadInPOS == NULL)
            return MEMORY_E;

        *thisThreadInPOS = 0;

        if (pthread_setspecific(thisThreadInPOSKey, thisThreadInPOS) != 0) {
            free(thisThreadInPOS);
            return THREAD_STORE_SET_E;
        }

        return 0;
    }

    static INLINE int GetThisThreadInPOS(void)
    {
        int *thisThreadInPOS = pthread_getspecific(thisThreadInPOSKey);

        if (thisThreadInPOS != NULL)
            return *thisThreadInPOS;

        return 0;
    }

    static INLINE void SetThisThreadInPOS(int inPOS)
    {
        int *thisThreadInPOS = pthread_getspecific(thisThreadInPOSKey);

        if (thisThreadInPOS != NULL)
            *thisThreadInPOS = inPOS;
    }

#endif

static CyaSSL_Mutex conTestMutex;       /* continous test mutex */
static int          conTestFailure = 0; /* in failure mode */

wolfCrypt_fips_cb errCb = NULL;                    /* error callback */

/* user callback setter for err result */
int wolfCrypt_SetCb_fips(wolfCrypt_fips_cb cbf)
{
    errCb = cbf;

    return 0;
}


/* check continuous test status, return 0 if status ok, else < 0 */
static int CheckConTestStatus(void)
{
    int localFailure = 0;

    if (LockMutex(&conTestMutex) != 0) {
        conTestFailure = 1;
        localFailure   = 1;
    } else {
        if (conTestFailure)
            localFailure = 1;
        UnLockMutex(&conTestMutex);
    }

    if (localFailure) {
        return -1;
    }

    return 0;
}


/* set continuous test failure status, return 0 on success */
static int SetConTestFailure(void)
{
    if (LockMutex(&conTestMutex) != 0) {
        conTestFailure = 1;
    } else {
        conTestFailure = 1;
        UnLockMutex(&conTestMutex);
    }

    return 0;
}


#ifdef HAVE_FORCE_FIPS_FAILURE

int wolfCrypt_SetStatus_fips(int status)
{
    if (status == DRBG_CONT_FIPS_E) {
        SetConTestFailure();
        return 0;
    }
    else if (status < 0) {
        posStatus = POS_FAILURE;
        posReturn = status;
        return 0;
    }

    return BAD_FUNC_ARG;
}

#endif /* HAVE_FORCE_FIPS_FAILURE */


/* return 0 on allowed (success), < 0 on error */
static int FipsAllowed(void)
{
    if (posStatus == POS_NOT_DONE && GetThisThreadInPOS() == 1)
        return 0;  /* allow POS on this thread only */
    else if (posStatus == POS_FAILURE) {
        if (errCb)
            errCb(0, posReturn, base16_hash);
        return -1;
    }

    if (CheckConTestStatus() != 0) {
        if (errCb)
            errCb(0, DRBG_CONT_FIPS_E, base16_hash);
        return -1;
    }

    if (posStatus == POS_SUCCESS)
        return 0;

    return -1;  /* default failure */
}


/* power on self test proper, only function to change POS status, only called
 * by entry point */
static void DoSelfTest(void)
{
    FIPS_MSG("Starting Power On Self Test");

    /* switch to not done, mark this thread as in pos */
    posStatus = POS_NOT_DONE;

    if ( (posReturn = InitThisThreadInPOS()) != 0) {
        posStatus = POS_FAILURE;
        FIPS_MSG("Power On Self Test FAILURE");
        return;
    }

    SetThisThreadInPOS(1);

    /* do tests proper */
    if ( (posReturn = DoKnownAnswerTests(base16_hash,
                                         sizeof(base16_hash))) != 0) {
        posStatus = POS_FAILURE;
        SetThisThreadInPOS(0);
        FIPS_MSG("Power On Self Test FAILURE");
        return;
    }

    /* completed success */
    posStatus = POS_SUCCESS;
    SetThisThreadInPOS(0);

    FIPS_MSG("Power On Self Test SUCCESS");
}


/* fips entry point, auto */
INITIALIZER(fipsEntry)
{
    DoSelfTest();
    if (InitMutex(&conTestMutex) != 0) {
        conTestFailure = 1;
    }
}


#if defined(USE_WINDOWS_API) && defined(WOLFSSL_DLL)

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
    (void)hinstDLL;
    (void)lpReserved;

    if (fdwReason == DLL_PROCESS_ATTACH)
        fipsEntry();
    return posStatus == POS_SUCCESS;
}

#endif


/* get current error status, 0 on ok */
int wolfCrypt_GetStatus_fips(void)
{
    if (posStatus != POS_SUCCESS)
        return posReturn;

    if (CheckConTestStatus() != 0)
        return DRBG_CONT_FIPS_E;

    return 0;
}


/* get current inCore hash */
const char* wolfCrypt_GetCoreHash_fips(void)
{
    return base16_hash;
}


/* Aes wrappers */
/* setkey wrapper */
int AesSetKey_fips(Aes* aes, const byte* userKey, word32 keylen, const byte* iv,
                   int dir)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return AesSetKey(aes, userKey, keylen, iv, dir);
}


/* set iv wrapper */
int AesSetIV_fips(Aes* aes, const byte* iv)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return AesSetIV(aes, iv);
}


/* cbc encrypt wrapper */
int AesCbcEncrypt_fips(Aes* aes, byte* out, const byte* in, word32 sz)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return AesCbcEncrypt(aes, out, in, sz);
}


/* cbc decrypt wrapper */
int AesCbcDecrypt_fips(Aes* aes, byte* out, const byte* in, word32 sz)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return AesCbcDecrypt(aes, out, in, sz);
}


/* gcm set key wrapper */
int AesGcmSetKey_fips(Aes* aes, const byte* key, word32 len)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return AesGcmSetKey(aes, key, len);
}


/* gcm encrypt wrapper */
int AesGcmEncrypt_fips(Aes* aes, byte* out, const byte* in,
                       word32 sz, const byte* iv, word32 ivSz,
                       byte* authTag, word32 authTagSz,
                       const byte* authIn, word32 authInSz)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return AesGcmEncrypt(aes, out, in, sz, iv, ivSz, authTag, authTagSz,
                         authIn, authInSz);
}


/* gcm decrypt wrapper */
int AesGcmDecrypt_fips(Aes* aes, byte* out, const byte* in,
                       word32 sz, const byte* iv, word32 ivSz,
                       const byte* authTag, word32 authTagSz,
                       const byte* authIn, word32 authInSz)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return AesGcmDecrypt(aes, out, in, sz, iv, ivSz, authTag, authTagSz,
                         authIn, authInSz);
}


/* Des3 wrappers */
/* setkey wrapper */
int Des3_SetKey_fips(Des3* des, const byte* userKey, const byte* iv, int dir)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return Des3_SetKey(des, userKey, iv, dir);
}


/* set iv wrapper */
int Des3_SetIV_fips(Des3* des, const byte* iv)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return Des3_SetIV(des, iv);
}


/* cbc encrypt wrapper */
int Des3_CbcEncrypt_fips(Des3* des, byte* out, const byte* in, word32 sz)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return Des3_CbcEncrypt(des, out, in, sz);
}


/* cbc decrypt wrapper */
int Des3_CbcDecrypt_fips(Des3* des, byte* out, const byte* in, word32 sz)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return Des3_CbcDecrypt(des, out, in, sz);
}



/* Hash wrappers */
/* Init SHA wrapper */
int InitSha_fips(Sha* sha)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return InitSha(sha);
}


/* SHA Update wrapper */
int ShaUpdate_fips(Sha* sha, const byte* data, word32 len)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return ShaUpdate(sha, data, len);
}


/* SHA Final wrapper */
int ShaFinal_fips(Sha* sha, byte* hash)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return ShaFinal(sha, hash);
}


/* Init SHA256 wrapper */
int InitSha256_fips(Sha256* sha)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return InitSha256(sha);
}


/* SHA256 Update wrapper */
int Sha256Update_fips(Sha256* sha, const byte* data, word32 len)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return Sha256Update(sha, data, len);
}


/* SHA256 Final wrapper */
int Sha256Final_fips(Sha256* sha, byte* hash)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return Sha256Final(sha, hash);
}


/* Init SHA512 wrapper */
int InitSha512_fips(Sha512* sha)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return InitSha512(sha);
}


/* SHA512 Update wrapper */
int Sha512Update_fips(Sha512* sha, const byte* data, word32 len)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return Sha512Update(sha, data, len);
}


/* SHA512 Final wrapper */
int Sha512Final_fips(Sha512* sha, byte* hash)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return Sha512Final(sha, hash);
}


/* Init SHA384 wrapper */
int InitSha384_fips(Sha384* sha)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return InitSha384(sha);
}


/* SHA384 Update wrapper */
int Sha384Update_fips(Sha384* sha, const byte* data, word32 len)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return Sha384Update(sha, data, len);
}


/* SHA384 Final wrapper */
int Sha384Final_fips(Sha384* sha, byte* hash)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return Sha384Final(sha, hash);
}


/* HMAC wrappers */
/* HMAC SetKey wrapper */
int HmacSetKey_fips(Hmac* hmac, int type, const byte* key, word32 keySz)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return HmacSetKey(hmac, type, key, keySz);
}


/* HMAC Update wrapper */
int HmacUpdate_fips(Hmac* hmac, const byte* data, word32 len)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return HmacUpdate(hmac, data, len);
}


/* HMAC Final wrapper */
int HmacFinal_fips(Hmac* hmac, byte* hash)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return HmacFinal(hmac, hash);
}


/* RSA wrappers */
/* Init RsaKey */
int InitRsaKey_fips(RsaKey* key, void* p)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return InitRsaKey(key, p); 
}


/* Free RsaKey */
int FreeRsaKey_fips(RsaKey* key)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return FreeRsaKey(key); 
}


/* Rsa Public Encrypt */
int RsaPublicEncrypt_fips(const byte* in,word32 inLen,byte* out,
                          word32 outLen, RsaKey* key, RNG* rng)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return RsaPublicEncrypt(in, inLen, out, outLen, key, rng); 
}


/* Rsa Private Decrypt Inline */
int RsaPrivateDecryptInline_fips(byte* in, word32 inLen, byte** out,RsaKey* key)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return RsaPrivateDecryptInline(in, inLen, out, key); 
}


/* Rsa Private Decrypt */
int RsaPrivateDecrypt_fips(const byte* in, word32 inLen,
                           byte* out,word32 outLen,RsaKey* key)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return RsaPrivateDecrypt(in, inLen, out, outLen, key); 
}


/* Rsa SSL Sign */
int RsaSSL_Sign_fips(const byte* in, word32 inLen, byte* out,
                     word32 outLen, RsaKey* key, RNG* rng)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return RsaSSL_Sign(in, inLen, out, outLen, key, rng); 
}


/* Rsa SSL Verify Inline */
int RsaSSL_VerifyInline_fips(byte* in, word32 inLen, byte** out, RsaKey* key)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
   
    return RsaSSL_VerifyInline(in, inLen, out, key); 
}


/* Rsa SSL Verify */
int RsaSSL_Verify_fips(const byte* in, word32 inLen, byte* out,
                       word32 outLen, RsaKey* key)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
  
    return RsaSSL_Verify(in, inLen, out, outLen, key); 
}


/* Rsa Encrypt Size */
int RsaEncryptSize_fips(RsaKey* key)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;
  
    return RsaEncryptSize(key);
}


/* Rsa PrivateKey Decode */
int RsaPrivateKeyDecode_fips(const byte* input, word32* inOutIdx, RsaKey* key,
                             word32 inSz)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return RsaPrivateKeyDecode(input, inOutIdx, key, inSz);
}


/* Rsa PublicKey Decode */
int RsaPublicKeyDecode_fips(const byte* input, word32* inOutIdx, RsaKey* key,
                            word32 inSz)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return RsaPublicKeyDecode(input, inOutIdx, key, inSz);
}


/* Init RNG */
int InitRng_fips(RNG* rng)
{
    int ret;

    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    ret = InitRng(rng);
    if (ret == DRBG_CONT_FIPS_E) {
        SetConTestFailure();
        return DRBG_CONT_FIPS_E;
    }

    return ret;
}


/* Free RNG */
int FreeRng_fips(RNG* rng)
{
    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    return FreeRng(rng);
}


/* Generate block of pseudo random numbers */
int RNG_GenerateBlock_fips(RNG* rng, byte* buf, word32 bufSz)
{
    int ret;

    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    ret = RNG_GenerateBlock(rng, buf, bufSz);
    if (ret == DRBG_CONT_FIPS_E) {
        SetConTestFailure();
        return DRBG_CONT_FIPS_E;
    }

    return ret;
}


/* RNG Health Test */
int RNG_HealthTest_fips(int reseed, const byte* entropyA, word32 entropyASz,
                                    const byte* entropyB, word32 entropyBSz,
                                    byte* output, word32 outputSz)
{
    int ret;

    if (FipsAllowed() != 0)
        return FIPS_NOT_ALLOWED_E;

    ret = RNG_HealthTest(reseed, entropyA, entropyASz, entropyB, entropyBSz,
                                                              output, outputSz);
    if (ret == DRBG_CONT_FIPS_E) {
        SetConTestFailure();
        return DRBG_CONT_FIPS_E;
    }

    return ret;
}


#endif /* HAVE_FIPS */
