/**************************************************************************
  Crypto Framework Library Header

  Company:
    Microchip Technology Inc.

  File Name:
    ecc.h
  
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



#ifdef HAVE_ECC

#ifndef CTAO_CRYPT_ECC_H
#define CTAO_CRYPT_ECC_H

#include "crypto/src/types.h"
#include "crypto/src/integer.h"
#include "crypto/src/random.h"

#ifdef __cplusplus
    extern "C" {
#endif


enum {
    ECC_PUBLICKEY  = 1,
    ECC_PRIVATEKEY = 2,
    ECC_MAXNAME    = 16,     /* MAX CURVE NAME LENGTH */
    SIG_HEADER_SZ  =  6,     /* ECC signature header size */
    ECC_BUFSIZE    = 256,    /* for exported keys temp buffer */
    ECC_MINSIZE    = 20,     /* MIN Private Key size */
    ECC_MAXSIZE    = 66      /* MAX Private Key size */
};


/* ECC set type defined a NIST GF(p) curve */
typedef struct {
    int size;       /* The size of the curve in octets */
    const char* name;     /* name of this curve */
    const char* prime;    /* prime that defines the field, curve is in (hex) */
    const char* Bf;       /* fields B param (hex) */
    const char* order;    /* order of the curve (hex) */
    const char* Gx;       /* x coordinate of the base point on curve (hex) */
    const char* Gy;       /* y coordinate of the base point on curve (hex) */
} ecc_set_type;


/* A point on an ECC curve, stored in Jacbobian format such that (x,y,z) =>
   (x/z^2, y/z^3, 1) when interpreted as affine */
typedef struct {
    mp_int x;        /* The x coordinate */
    mp_int y;        /* The y coordinate */
    mp_int z;        /* The z coordinate */
} ecc_point;


/* An ECC Key */
typedef struct {
    int type;           /* Public or Private */
    int idx;            /* Index into the ecc_sets[] for the parameters of
                           this curve if -1, this key is using user supplied
                           curve in dp */
    const ecc_set_type* dp;     /* domain parameters, either points to NIST
                                   curves (idx >= 0) or user supplied */
    ecc_point pubkey;   /* public key */  
    mp_int    k;        /* private key */
} ecc_key;


/* ECC predefined curve sets  */
extern const ecc_set_type ecc_sets[];


CYASSL_API
int ecc_make_key(RNG* rng, int keysize, ecc_key* key);
CYASSL_API
int ecc_shared_secret(ecc_key* private_key, ecc_key* public_key, byte* out,
                      word32* outlen);
CYASSL_API
int ecc_sign_hash(const byte* in, word32 inlen, byte* out, word32 *outlen, 
                  RNG* rng, ecc_key* key);
CYASSL_API
int ecc_verify_hash(const byte* sig, word32 siglen, const byte* hash,
                    word32 hashlen, int* stat, ecc_key* key);
CYASSL_API
void ecc_init(ecc_key* key);
CYASSL_API
void ecc_free(ecc_key* key);
CYASSL_API
void ecc_fp_free(void);


/* ASN key helpers */
CYASSL_API
int ecc_export_x963(ecc_key*, byte* out, word32* outLen);
CYASSL_API
int ecc_import_x963(const byte* in, word32 inLen, ecc_key* key);
CYASSL_API
int ecc_import_private_key(const byte* priv, word32 privSz, const byte* pub,
                           word32 pubSz, ecc_key* key);
CYASSL_API
int ecc_export_private_only(ecc_key* key, byte* out, word32* outLen);

/* size helper */
CYASSL_API
int ecc_size(ecc_key* key);
CYASSL_API
int ecc_sig_size(ecc_key* key);


#ifdef HAVE_ECC_ENCRYPT
/* ecc encrypt */

enum ecEncAlgo {
    ecAES_128_CBC = 1,  /* default */
    ecAES_256_CBC = 2
};

enum ecKdfAlgo {
    ecHKDF_SHA256 = 1,  /* default */
    ecHKDF_SHA1   = 2
};

enum ecMacAlgo {
    ecHMAC_SHA256 = 1,  /* default */
    ecHMAC_SHA1   = 2
};

enum {
    KEY_SIZE_128     = 16,   
    KEY_SIZE_256     = 32,   
    IV_SIZE_64       =  8,
    EXCHANGE_SALT_SZ = 16,  
    EXCHANGE_INFO_SZ = 23  
};

enum ecFlags {
    REQ_RESP_CLIENT = 1,
    REQ_RESP_SERVER = 2
};


typedef struct ecEncCtx ecEncCtx;

CYASSL_API
ecEncCtx* ecc_ctx_new(int flags, RNG* rng);
CYASSL_API
void ecc_ctx_free(ecEncCtx*);

CYASSL_API
const byte* ecc_ctx_get_own_salt(ecEncCtx*);
CYASSL_API
int ecc_ctx_set_peer_salt(ecEncCtx*, const byte* salt);

CYASSL_API
int ecc_encrypt(ecc_key* privKey, ecc_key* pubKey, const byte* msg,
                word32 msgSz, byte* out, word32* outSz, ecEncCtx* ctx);
CYASSL_API
int ecc_decrypt(ecc_key* privKey, ecc_key* pubKey, const byte* msg,
                word32 msgSz, byte* out, word32* outSz, ecEncCtx* ctx);

#endif /* HAVE_ECC_ENCRYPT */

#ifdef __cplusplus
    }    /* extern "C" */    
#endif

#endif /* CTAO_CRYPT_ECC_H */
#endif /* HAVE_ECC */
