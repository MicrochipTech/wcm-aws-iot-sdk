<#--
/*******************************************************************************
  Cryptographic Library Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    crypto.h

  Summary:
    Cryptographic Library.

  Description:
    This file contains the interface definition for the Crypto Library. 
    It sets up the definitions required to add or remove elements of the
    library that the user has specified.
*******************************************************************************/

/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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
-->

<#if CONFIG_USE_CRYPTO_LIB && !CONFIG_USE_3RDPARTY_WOLFSSL>
#define HAVE_MCAPI
<#if CONFIG_USE_CRYPTO_HW>
#define CYASSL_MICROCHIP_PIC32MZ
</#if>
<#if CONFIG_USE_CRYPTO_MD5 == false && CONFIG_USE_CRYPTO_ECC == false>
#define NO_MD5
</#if>
<#if CONFIG_USE_CRYPTO_SHA == false && CONFIG_USE_CRYPTO_ECC == false>
#define NO_SHA
</#if>
<#if CONFIG_USE_CRYPTO_SHA_256 == false>
#define NO_SHA256
</#if>
<#if CONFIG_USE_CRYPTO_SHA_384 == true>
#define CYASSL_SHA384
</#if>
<#if CONFIG_USE_CRYPTO_SHA_512 == true || CONFIG_USE_CRYPTO_SHA_384 == true>
#define CYASSL_SHA512
</#if>
<#if CONFIG_USE_CRYPTO_COMPRESS == true>
#define HAVE_LIBZ
</#if>
<#if CONFIG_USE_CRYPTO_ECC == true>
#define HAVE_ECC
</#if>
<#if CONFIG_USE_CRYPTO_AES == false>
#define NO_AES
</#if>
<#if CONFIG_USE_CRYPTO_RSA == false>
#define NO_RSA
</#if>
<#if CONFIG_USE_CRYPTO_AES_COUNTER == true>
#define CYASSL_AES_COUNTER
</#if>
<#if CONFIG_USE_CRYPTO_AES_DIRECT == true>
#define CYASSL_AES_DIRECT
</#if>
<#if CONFIG_USE_CRYPTO_HMAC == false && CONFIG_USE_CRYPTO_ECC == false>
#define NO_HMAC
</#if>
<#if CONFIG_USE_CRYPTO_RANDOM == false && CONFIG_USE_CRYPTO_ECC == false && CONFIG_USE_CRYPTO_RSA == false>
#define NO_RNG
</#if>
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->

