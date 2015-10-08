/**************************************************************************
  Crypto Framework Library Source

  Company:
    Microchip Technology Inc.

  File Name:
    arc4.c
  
  Summary:
    Crypto Framework Libarary source for cryptographic functions.

  Description:
    This source file contains functions that make up the Cryptographic 
	Framework Library for PIC32 families of Microchip microcontrollers.
**************************************************************************/

//DOM-IGNORE-BEGIN
/******************************************************************************
File Name:  arc4.c
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

#ifndef NO_RC4

#include "crypto/src/arc4.h"


#ifdef HAVE_CAVIUM
    static void Arc4CaviumSetKey(Arc4* arc4, const byte* key, word32 length);
    static void Arc4CaviumProcess(Arc4* arc4, byte* out, const byte* in,
                                  word32 length);
#endif


void Arc4SetKey(Arc4* arc4, const byte* key, word32 length)
{
    word32 i;
    word32 keyIndex = 0, stateIndex = 0;

#ifdef HAVE_CAVIUM
    if (arc4->magic == CYASSL_ARC4_CAVIUM_MAGIC)
        return Arc4CaviumSetKey(arc4, key, length);
#endif

    arc4->x = 1;
    arc4->y = 0;

    for (i = 0; i < ARC4_STATE_SIZE; i++)
        arc4->state[i] = (byte)i;

    for (i = 0; i < ARC4_STATE_SIZE; i++) {
        word32 a = arc4->state[i];
        stateIndex += key[keyIndex] + a;
        stateIndex &= 0xFF;
        arc4->state[i] = arc4->state[stateIndex];
        arc4->state[stateIndex] = (byte)a;

        if (++keyIndex >= length)
            keyIndex = 0;
    }
}


static INLINE byte MakeByte(word32* x, word32* y, byte* s)
{
    word32 a = s[*x], b;
    *y = (*y+a) & 0xff;

    b = s[*y];
    s[*x] = (byte)b;
    s[*y] = (byte)a;
    *x = (*x+1) & 0xff;

    return s[(a+b) & 0xff];
}


void Arc4Process(Arc4* arc4, byte* out, const byte* in, word32 length)
{
    word32 x;
    word32 y;

#ifdef HAVE_CAVIUM
    if (arc4->magic == CYASSL_ARC4_CAVIUM_MAGIC)
        return Arc4CaviumProcess(arc4, out, in, length);
#endif

    x = arc4->x;
    y = arc4->y;

    while(length--)
        *out++ = *in++ ^ MakeByte(&x, &y, arc4->state);

    arc4->x = (byte)x;
    arc4->y = (byte)y;
}


#ifdef HAVE_CAVIUM

#include "crypto/src/logging.h"
#include "cavium_common.h"

/* Initiliaze Arc4 for use with Nitrox device */
int Arc4InitCavium(Arc4* arc4, int devId)
{
    if (arc4 == NULL)
        return -1;

    if (CspAllocContext(CONTEXT_SSL, &arc4->contextHandle, devId) != 0)
        return -1;

    arc4->devId = devId;
    arc4->magic = CYASSL_ARC4_CAVIUM_MAGIC;
   
    return 0;
}


/* Free Arc4 from use with Nitrox device */
void Arc4FreeCavium(Arc4* arc4)
{
    if (arc4 == NULL)
        return;

    if (arc4->magic != CYASSL_ARC4_CAVIUM_MAGIC)
        return;

    CspFreeContext(CONTEXT_SSL, arc4->contextHandle, arc4->devId);
    arc4->magic = 0;
}


static void Arc4CaviumSetKey(Arc4* arc4, const byte* key, word32 length)
{
    word32 requestId;

    if (CspInitializeRc4(CAVIUM_BLOCKING, arc4->contextHandle, length,
                         (byte*)key, &requestId, arc4->devId) != 0) {
        CYASSL_MSG("Bad Cavium Arc4 Init");
    }
}


static void Arc4CaviumProcess(Arc4* arc4, byte* out, const byte* in,
                              word32 length)
{
    word   offset = 0;
    word32 requestId;

    while (length > CYASSL_MAX_16BIT) {
        word16 slen = (word16)CYASSL_MAX_16BIT;
        if (CspEncryptRc4(CAVIUM_BLOCKING, arc4->contextHandle,CAVIUM_UPDATE,
                          slen, (byte*)in + offset, out + offset, &requestId,
                          arc4->devId) != 0) {
            CYASSL_MSG("Bad Cavium Arc4 Encrypt");
        }
        length -= CYASSL_MAX_16BIT;
        offset += CYASSL_MAX_16BIT;
    }
    if (length) {
        word16 slen = (word16)length;
        if (CspEncryptRc4(CAVIUM_BLOCKING, arc4->contextHandle,CAVIUM_UPDATE,
                          slen, (byte*)in + offset, out + offset, &requestId,
                          arc4->devId) != 0) {
            CYASSL_MSG("Bad Cavium Arc4 Encrypt");
        }
    }
}

#endif /* HAVE_CAVIUM */

#endif /* NO_ARC4 */

