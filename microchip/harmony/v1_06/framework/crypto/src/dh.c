/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    dh.c
  
  Summary:
    Crypto Framework Libarary source for cryptographic functions.

  Description:
    This source file contains functions that make up the Cryptographic 
	Framework Library for PIC32 families of Microchip microcontrollers.
**************************************************************************/

//DOM-IGNORE-BEGIN
/******************************************************************************
File Name:  dh.c
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

#include "crypto/src/settings.h"

#ifndef NO_DH

#include "crypto/src/dh.h"
#include "crypto/src/error.h"

#ifndef USER_MATH_LIB
    #include <math.h>
    #define XPOW(x,y) pow((x),(y))
    #define XLOG(x)   log((x))
#else
    /* user's own math lib */
#endif


#ifndef min

    static INLINE word32 min(word32 a, word32 b)
    {
        return a > b ? b : a;
    }

#endif /* min */


void InitDhKey(DhKey* key)
{
    (void)key;
/* TomsFastMath doesn't use memory allocation */
#ifndef USE_FAST_MATH
    key->p.dp = 0;
    key->g.dp = 0;
#endif
}


void FreeDhKey(DhKey* key)
{
    (void)key;
/* TomsFastMath doesn't use memory allocation */
#ifndef USE_FAST_MATH
    mp_clear(&key->p);
    mp_clear(&key->g);
#endif
}


static word32 DiscreteLogWorkFactor(word32 n)
{
    /* assuming discrete log takes about the same time as factoring */
    if (n<5)
        return 0;
    else
        return (word32)(2.4 * XPOW((double)n, 1.0/3.0) *
                XPOW(XLOG((double)n), 2.0/3.0) - 5);
}


static void GeneratePrivate(DhKey* key, RNG* rng, byte* priv, word32* privSz)
{
    word32 sz = mp_unsigned_bin_size(&key->p);
    sz = min(sz, 2 * DiscreteLogWorkFactor(sz * CYASSL_BIT_SIZE) /
                                           CYASSL_BIT_SIZE + 1);
    RNG_GenerateBlock(rng, priv, sz);
    priv[0] |= 0x0C;

    *privSz = sz;
}


static int GeneratePublic(DhKey* key, const byte* priv, word32 privSz,
                          byte* pub, word32* pubSz)
{
    int ret = 0;

    mp_int x; 
    mp_int y;

    if (mp_init_multi(&x, &y, 0, 0, 0, 0) != MP_OKAY)
        return MP_INIT_E;

    if (mp_read_unsigned_bin(&x, priv, privSz) != MP_OKAY)
        ret = MP_READ_E;

    if (ret == 0 && mp_exptmod(&key->g, &x, &key->p, &y) != MP_OKAY)
        ret = MP_EXPTMOD_E;

    if (ret == 0 && mp_to_unsigned_bin(&y, pub) != MP_OKAY)
        ret = MP_TO_E;

    if (ret == 0)
        *pubSz = mp_unsigned_bin_size(&y);

    mp_clear(&y);
    mp_clear(&x);

    return ret;
}


int DhGenerateKeyPair(DhKey* key, RNG* rng, byte* priv, word32* privSz,
                      byte* pub, word32* pubSz)
{
    GeneratePrivate(key, rng, priv, privSz);
    return GeneratePublic(key, priv, *privSz, pub, pubSz);

}

int DhAgree(DhKey* key, byte* agree, word32* agreeSz, const byte* priv,
            word32 privSz, const byte* otherPub, word32 pubSz)
{
    int ret = 0;

    mp_int x; 
    mp_int y;
    mp_int z;

    if (mp_init_multi(&x, &y, &z, 0, 0, 0) != MP_OKAY)
        return MP_INIT_E;

    if (mp_read_unsigned_bin(&x, priv, privSz) != MP_OKAY)
        ret = MP_READ_E;

    if (ret == 0 && mp_read_unsigned_bin(&y, otherPub, pubSz) != MP_OKAY)
        ret = MP_READ_E;

    if (ret == 0 && mp_exptmod(&y, &x, &key->p, &z) != MP_OKAY)
        ret = MP_EXPTMOD_E;

    if (ret == 0 && mp_to_unsigned_bin(&z, agree) != MP_OKAY)
        ret = MP_TO_E;

    if (ret == 0)
        *agreeSz = mp_unsigned_bin_size(&z);

    mp_clear(&z);
    mp_clear(&y);
    mp_clear(&x);

    return ret;
}


#endif /* NO_DH */

