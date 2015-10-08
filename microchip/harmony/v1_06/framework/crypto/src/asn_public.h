/**************************************************************************
  Crypto Framework Library Header

  Company:
    Microchip Technology Inc.

  File Name:
    asn_public.h
  
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




#ifndef CTAO_CRYPT_ASN_PUBLIC_H
#define CTAO_CRYPT_ASN_PUBLIC_H

#include "crypto/src/types.h"
#include "crypto/src/ecc.h"
#ifdef CYASSL_CERT_GEN
    #include "crypto/src/rsa.h"
#endif


#ifdef __cplusplus
    extern "C" {
#endif


/* Certificate file Type */
enum CertType {
    CERT_TYPE       = 0, 
    PRIVATEKEY_TYPE,
    DH_PARAM_TYPE,
    CRL_TYPE,
    CA_TYPE,
    ECC_PRIVATEKEY_TYPE,
    CERTREQ_TYPE
};


/* Signature type, by OID sum */
enum Ctc_SigType {
    CTC_SHAwDSA      = 517,
    CTC_MD2wRSA      = 646,
    CTC_MD5wRSA      = 648,
    CTC_SHAwRSA      = 649,
    CTC_SHAwECDSA    = 520,
    CTC_SHA256wRSA   = 655,
    CTC_SHA256wECDSA = 524,
    CTC_SHA384wRSA   = 656,
    CTC_SHA384wECDSA = 525,
    CTC_SHA512wRSA   = 657,
    CTC_SHA512wECDSA = 526
};


#ifdef CYASSL_CERT_GEN

#ifndef HAVE_ECC
    typedef struct ecc_key ecc_key;
#endif

enum Ctc_Misc {
    CTC_NAME_SIZE    =   64,
    CTC_DATE_SIZE    =   32,
    CTC_MAX_ALT_SIZE = 8192,    /* may be huge */
    CTC_SERIAL_SIZE  =    8
};

typedef struct CertName {
    char country[CTC_NAME_SIZE];
    char state[CTC_NAME_SIZE];
    char locality[CTC_NAME_SIZE];
    char sur[CTC_NAME_SIZE];
    char org[CTC_NAME_SIZE];
    char unit[CTC_NAME_SIZE];
    char commonName[CTC_NAME_SIZE];
    char email[CTC_NAME_SIZE];  /* !!!! email has to be last !!!! */
} CertName;


/* for user to fill for certificate generation */
typedef struct Cert {
    int      version;                   /* x509 version  */
    byte     serial[CTC_SERIAL_SIZE];   /* serial number */
    int      sigType;                   /* signature algo type */
    CertName issuer;                    /* issuer info */
    int      daysValid;                 /* validity days */
    int      selfSigned;                /* self signed flag */
    CertName subject;                   /* subject info */
    int      isCA;                      /* is this going to be a CA */
    /* internal use only */
    int      bodySz;                    /* pre sign total size */
    int      keyType;                   /* public key type of subject */
#ifdef CYASSL_ALT_NAMES
    byte     altNames[CTC_MAX_ALT_SIZE]; /* altNames copy */
    int      altNamesSz;                 /* altNames size in bytes */
    byte     beforeDate[CTC_DATE_SIZE];  /* before date copy */
    int      beforeDateSz;               /* size of copy */
    byte     afterDate[CTC_DATE_SIZE];   /* after date copy */
    int      afterDateSz;                /* size of copy */
#endif
#ifdef CYASSL_CERT_REQ
    char     challengePw[CTC_NAME_SIZE];
#endif
} Cert;




/* Initialize and Set Certficate defaults:
   version    = 3 (0x2)
   serial     = 0 (Will be randomly generated)
   sigType    = SHA_WITH_RSA
   issuer     = blank
   daysValid  = 500
   selfSigned = 1 (true) use subject as issuer
   subject    = blank
   isCA       = 0 (false)
   keyType    = RSA_KEY (default)
*/
CYASSL_API void InitCert(Cert*);
CYASSL_API int  MakeCert(Cert*, byte* derBuffer, word32 derSz, RsaKey*,
                         ecc_key*, RNG*);
#ifdef CYASSL_CERT_REQ
    CYASSL_API int  MakeCertReq(Cert*, byte* derBuffer, word32 derSz, RsaKey*,
                                ecc_key*);
#endif
CYASSL_API int  SignCert(int requestSz, int sigType, byte* derBuffer,
                         word32 derSz, RsaKey*, ecc_key*, RNG*);
CYASSL_API int  MakeSelfCert(Cert*, byte* derBuffer, word32 derSz, RsaKey*,
                             RNG*);
CYASSL_API int  SetIssuer(Cert*, const char*);
CYASSL_API int  SetSubject(Cert*, const char*);
#ifdef CYASSL_ALT_NAMES
    CYASSL_API int  SetAltNames(Cert*, const char*);
#endif
CYASSL_API int  SetIssuerBuffer(Cert*, const byte*, int);
CYASSL_API int  SetSubjectBuffer(Cert*, const byte*, int);
CYASSL_API int  SetAltNamesBuffer(Cert*, const byte*, int);
CYASSL_API int  SetDatesBuffer(Cert*, const byte*, int);

    #ifdef HAVE_NTRU
        CYASSL_API int  MakeNtruCert(Cert*, byte* derBuffer, word32 derSz,
                                     const byte* ntruKey, word16 keySz, RNG*);
    #endif

#endif /* CYASSL_CERT_GEN */


#if defined(CYASSL_KEY_GEN) || defined(CYASSL_CERT_GEN)
    CYASSL_API int DerToPem(const byte* der, word32 derSz, byte* output,
                            word32 outputSz, int type);
#endif

#ifdef HAVE_ECC
    /* private key helpers */
    CYASSL_API int EccPrivateKeyDecode(const byte* input,word32* inOutIdx,
                                         ecc_key*,word32);
#endif


#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* CTAO_CRYPT_ASN_PUBLIC_H */

