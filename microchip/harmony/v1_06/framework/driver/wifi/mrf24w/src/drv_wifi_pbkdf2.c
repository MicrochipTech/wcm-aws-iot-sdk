/*******************************************************************************
  MRF24W pbkdf2 algorithm

  File Name:
    drv_wifi_pbkdf2.c

  Summary:
    Module for Microchip TCP/IP Stack

  Description:
    SHA1 hash implementation and interface functions

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

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

/* WPA Supplicant
*
* SHA1 hash implementation and interface functions
* Copyright (c) 2003-2005, Jouni Malinen <jkmaline@cc.hut.fi>
*
* This program is free software; distributed under the terms of BSD
* license:
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
*
* 1.    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* 2.    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
*        in the documentation and/or other materials provided with the distribution.
* 3.    Neither the name(s) of the above-listed copyright holder(s) nor the names of its contributors may be used to endorse
*        or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "tcpip/src/tcpip_private.h"

#if defined(TCPIP_IF_MRF24W)

#include <string.h>

typedef uint8_t  u8;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  U8;
typedef uint32_t U32;
typedef uint16_t U16;

#define LITTLEENDIAN 1

#define SHA1_MAC_LEN 20

#define STATEMACHINE_pbkdf2_sha1    // Using StateMachine for function pbkdf2_sha1_f()

void sha1_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac);

/**
 * hmac_sha1_vector - HMAC-SHA1 over data vector (RFC 2104)
 * @key: Key for HMAC operations
 * @key_len: Length of the key in bytes
 * @num_elem: Number of elements in the data vector
 * @addr: Pointers to the data areas
 * @len: Lengths of the data blocks
 * @mac: Buffer for the hash (20 bytes)
 */
void hmac_sha1_vector(const u8 *key, size_t key_len, size_t num_elem,
              const u8 *addr[], const size_t *len, u8 *mac)
{
    unsigned char k_pad[64]; /* padding - key XORd with ipad/opad */
    unsigned char tk[20];
    int i;
    const u8 *_addr[6];
    size_t _len[6];

    if (num_elem > 5) {
        /*
         * Fixed limit on the number of fragments to avoid having to
         * allocate memory (which could fail).
         */
        return;
    }

        /* if key is longer than 64 bytes reset it to key = SHA1(key) */
        if (key_len > 64) {
        sha1_vector(1, &key, &key_len, tk);
        key = tk;
        key_len = 20;
        }

    /* the HMAC_SHA1 transform looks like:
     *
     * SHA1(K XOR opad, SHA1(K XOR ipad, text))
     *
     * where K is an n byte key
     * ipad is the byte 0x36 repeated 64 times
     * opad is the byte 0x5c repeated 64 times
     * and text is the data being protected */

    /* start out by storing key in ipad */
    memset(k_pad, 0, sizeof(k_pad));
    memcpy((U8*)k_pad, (U8*)key, key_len);
    /* XOR key with ipad values */
    for (i = 0; i < 64; i++)
        k_pad[i] ^= 0x36;

    /* perform inner SHA1 */
    _addr[0] = k_pad;
    _len[0] = 64;
    for (i = 0; i < num_elem; i++) {
        _addr[i + 1] = addr[i];
        _len[i + 1] = len[i];
    }
    sha1_vector(1 + num_elem, _addr, _len, mac);

    memset(k_pad, 0, sizeof(k_pad));
    memcpy((U8*)k_pad, (U8*)key, key_len);
    /* XOR key with opad values */
    for (i = 0; i < 64; i++)
        k_pad[i] ^= 0x5c;

    /* perform outer SHA1 */
    _addr[0] = k_pad;
    _len[0] = 64;
    _addr[1] = mac;
    _len[1] = SHA1_MAC_LEN;
    sha1_vector(2, _addr, _len, mac);
}

/**
 * hmac_sha1 - HMAC-SHA1 over data buffer (RFC 2104)
 * @key: Key for HMAC operations
 * @key_len: Length of the key in bytes
 * @data: Pointers to the data area
 * @data_len: Length of the data area
 * @mac: Buffer for the hash (20 bytes)
 */
void hmac_sha1(const u8 *key, size_t key_len, const u8 *data, size_t data_len,
           u8 *mac)
{
    hmac_sha1_vector(key, key_len, 1, &data, &data_len, mac);
}

/**
 * sha1_prf - SHA1-based Pseudo-Random Function (PRF) (IEEE 802.11i, 8.5.1.1)
 * @key: Key for PRF
 * @key_len: Length of the key in bytes
 * @label: A unique label for each purpose of the PRF
 * @data: Extra data to bind into the key
 * @data_len: Length of the data
 * @buf: Buffer for the generated pseudo-random key
 * @buf_len: Number of bytes of key to generate
 *
 * This function is used to derive new, cryptographically separate keys from a
 * given key (e.g., PMK in IEEE 802.11i).
 */
void sha1_prf(const u8 *key, size_t key_len, const char *label,
          const u8 *data, size_t data_len, u8 *buf, size_t buf_len)
{
    u8 zero = 0, counter = 0;
    size_t pos, plen;
    u8 hash[SHA1_MAC_LEN];
    size_t label_len = strlen(label);
    const unsigned char *addr[4];
    size_t len[4];

    addr[0] = (u8 *) label;
    len[0] = label_len;
    addr[1] = &zero;
    len[1] = 1;
    addr[2] = data;
    len[2] = data_len;
    addr[3] = &counter;
    len[3] = 1;

    pos = 0;
    while (pos < buf_len) {
        plen = buf_len - pos;
        if (plen >= SHA1_MAC_LEN) {
            hmac_sha1_vector(key, key_len, 4, addr, len,
                     &buf[pos]);
            pos += SHA1_MAC_LEN;
        } else {
            hmac_sha1_vector(key, key_len, 4, addr, len,
                     hash);
            memcpy(&buf[pos], hash, plen);
            break;
        }
        counter++;
    }
}

/**
 * sha1_t_prf - EAP-FAST Pseudo-Random Function (T-PRF)
 * @key: Key for PRF
 * @key_len: Length of the key in bytes
 * @label: A unique label for each purpose of the PRF
 * @seed: Seed value to bind into the key
 * @seed_len: Length of the seed
 * @buf: Buffer for the generated pseudo-random key
 * @buf_len: Number of bytes of key to generate
 *
 * This function is used to derive new, cryptographically separate keys from a
 * given key for EAP-FAST. T-PRF is defined in
 * draft-cam-winget-eap-fast-02.txt, Appendix B.
 */
void sha1_t_prf(const u8 *key, size_t key_len, const char *label,
        const u8 *seed, size_t seed_len, u8 *buf, size_t buf_len)
{
    unsigned char counter = 0;
    size_t pos, plen;
    u8 hash[SHA1_MAC_LEN];
    size_t label_len = strlen(label);
    u8 output_len[2];
    const unsigned char *addr[5];
    size_t len[5];

    addr[0] = hash;
    len[0] = 0;
    addr[1] = (unsigned char *) label;
    len[1] = label_len + 1;
    addr[2] = seed;
    len[2] = seed_len;
    addr[3] = output_len;
    len[3] = 2;
    addr[4] = &counter;
    len[4] = 1;

    output_len[0] = (buf_len >> 8) & 0xff;
    output_len[1] = buf_len & 0xff;
    pos = 0;
    while (pos < buf_len) {
        counter++;
        plen = buf_len - pos;
        hmac_sha1_vector(key, key_len, 5, addr, len, hash);
        if (plen >= SHA1_MAC_LEN) {
            memcpy(&buf[pos], hash, SHA1_MAC_LEN);
            pos += SHA1_MAC_LEN;
        } else {
            memcpy(&buf[pos], hash, plen);
            break;
        }
        len[0] = SHA1_MAC_LEN;
    }
}

static TCPIP_MAC_RES pbkdf2_sha1_f(const char *passphrase, const char *ssid,size_t ssid_len, int iterations, int count,u8 *digest);

/**
 * pbkdf2_sha1 - SHA1-based key derivation function (PBKDF2) for IEEE 802.11i
 * @passphrase: ASCII passphrase
 * @ssid: SSID
 * @ssid_len: SSID length in bytes
 * @interations: Number of iterations to run
 * @buf: Buffer for the generated key
 * @buflen: Length of the buffer in bytes
 *
 * This function is used to derive PSK for WPA-PSK. For this protocol,
 * iterations is set to 4096 and buflen to 32. This function is described in
 * IEEE Std 802.11-2004, Clause H.4. The main construction is from PKCS#5 v2.0.
 */
#if defined(STATEMACHINE_pbkdf2_sha1)
// Using State Machine, because this function takes 3 seconds, it is too long.
TCPIP_MAC_RES pbkdf2_sha1(const char *passphrase, const char *ssid, U16 ssid_len,
         U16 iterations, U8 *buf, U16 buflen)
{
    typedef enum
    {
        pbkdf2_STATE_START= 0,
        pbkdf2_STATE_1,
        pbkdf2_STATE_DONE,
    }pbkdf2_STATE;
    static pbkdf2_STATE pbkdf2_sha1_sm = pbkdf2_STATE_START;

    static int count ;
    static unsigned char *pos ;
    static size_t left, plen;
    static unsigned char digest[SHA1_MAC_LEN];

    TCPIP_MAC_RES RetCode = TCPIP_MAC_RES_PENDING;

    switch (pbkdf2_sha1_sm) {
    case pbkdf2_STATE_START:
        count = 1;
        pos = buf;
        left = buflen;
        pbkdf2_sha1_sm++;
        // not break, because continue to run next state

    case pbkdf2_STATE_1:
        if (left > 0)
        {
            if (TCPIP_MAC_RES_OK == pbkdf2_sha1_f(passphrase, ssid, ssid_len, iterations, count, digest))
            {
            count++;
            plen = left > SHA1_MAC_LEN ? SHA1_MAC_LEN : left;
            memcpy(pos, digest, plen);
            pos += plen;
            left -= plen;
            }
        }
        else
        {
            pbkdf2_sha1_sm++;

        }
        RetCode = TCPIP_MAC_RES_PENDING;
        break;

    case pbkdf2_STATE_DONE:
        pbkdf2_sha1_sm = pbkdf2_STATE_START;
        RetCode = TCPIP_MAC_RES_OK;
        break;
    }

    return RetCode;
}

static TCPIP_MAC_RES pbkdf2_sha1_f(const char *passphrase, const char *ssid,
              size_t ssid_len, int iterations, int count,
              u8 *digest)
{
    typedef enum {
        pbkdf2_sha1_f_SM_begin=0,
        pbkdf2_sha1_f_SM_1,
        pbkdf2_sha1_f_SM_done
    }pbkdf2_sha1_f_SM;
    static pbkdf2_sha1_f_SM pbkdf2_sha1_f_sm = pbkdf2_sha1_f_SM_begin;

    static unsigned char tmp[SHA1_MAC_LEN], tmp2[SHA1_MAC_LEN];
    static int i, j;
    static unsigned char count_buf[4];
    static const u8 *addr[2];
    static size_t len[2];
    static size_t passphrase_len;
    TCPIP_MAC_RES RetCode = TCPIP_MAC_RES_PENDING;

    switch (pbkdf2_sha1_f_sm) {
    case pbkdf2_sha1_f_SM_begin:
        i = 1;
        passphrase_len = strlen(passphrase);
        addr[0] = (u8 *) ssid;
        len[0] = ssid_len;
        addr[1] = count_buf;
        len[1] = 4;

        count_buf[0] = (count >> 24) & 0xff;
        count_buf[1] = (count >> 16) & 0xff;
        count_buf[2] = (count >> 8) & 0xff;
        count_buf[3] = count & 0xff;
        hmac_sha1_vector((u8 *) passphrase, passphrase_len, 2, addr, len, tmp);
        memcpy(digest, tmp, SHA1_MAC_LEN);
        pbkdf2_sha1_f_sm++;
        // no break, continue to work on pbkdf2_sha1_f_1
    case pbkdf2_sha1_f_SM_1:
        if (i < iterations)
        {
            U8* pTemp;

            hmac_sha1((u8 *) passphrase, passphrase_len, ((i&0x01) ? tmp : tmp2), SHA1_MAC_LEN,((i&0x01) ? tmp2 : tmp));
            pTemp = (i & 0x01) ? tmp2 : tmp;
            for (j = 0; j < SHA1_MAC_LEN; j++)
                digest[j] ^= pTemp[j];
            i++;
        }
        else    pbkdf2_sha1_f_sm =pbkdf2_sha1_f_SM_done;

        RetCode = TCPIP_MAC_RES_PENDING;
        break;
    case pbkdf2_sha1_f_SM_done:
        pbkdf2_sha1_f_sm = pbkdf2_sha1_f_SM_begin;
        RetCode = TCPIP_MAC_RES_OK;
        break;

    }

    return RetCode;

}

#else //#if defined(STATEMACHINE_pbkdf2_sha1)
TCPIP_MAC_RES pbkdf2_sha1(const char *passphrase, const char *ssid, U16 ssid_len,
         U16 iterations, U8 *buf, U16 buflen)
{
    int count = 0;
    unsigned char *pos = buf;
    size_t left = buflen, plen;
    unsigned char digest[SHA1_MAC_LEN];
    uint8_t buf_t[50];

    while (left > 0) {
        count++;
        pbkdf2_sha1_f(passphrase, ssid, ssid_len, iterations, count,
                  digest);
        plen = left > SHA1_MAC_LEN ? SHA1_MAC_LEN : left;
        memcpy(pos, digest, plen);
        pos += plen;
        left -= plen;
    }
    return TCPIP_MAC_RES_OK;  // must return true
}
static TCPIP_MAC_RES pbkdf2_sha1_f(const char *passphrase, const char *ssid,
              size_t ssid_len, int iterations, int count,
              u8 *digest)
{
    unsigned char tmp[SHA1_MAC_LEN], tmp2[SHA1_MAC_LEN];
    int i, j;
    unsigned char count_buf[4];
    const u8 *addr[2];
    size_t len[2];
    size_t passphrase_len = strlen(passphrase);

    addr[0] = (u8 *) ssid;
    len[0] = ssid_len;
    addr[1] = count_buf;
    len[1] = 4;

    /* F(P, S, c, i) = U1 xor U2 xor ... Uc
     * U1 = PRF(P, S || i)
     * U2 = PRF(P, U1)
     * Uc = PRF(P, Uc-1)
     */

    count_buf[0] = (count >> 24) & 0xff;
    count_buf[1] = (count >> 16) & 0xff;
    count_buf[2] = (count >> 8) & 0xff;
    count_buf[3] = count & 0xff;
    hmac_sha1_vector((u8 *) passphrase, passphrase_len, 2, addr, len, tmp);
    memcpy(digest, tmp, SHA1_MAC_LEN);

    for (i = 1; i < iterations; i++) {
        U8* pTemp;

        hmac_sha1((u8 *) passphrase, passphrase_len, ((i & 0x01) ? tmp : tmp2), SHA1_MAC_LEN,
              ((i & 0x01) ? tmp2 : tmp));
        //memcpy(tmp, tmp2, SHA1_MAC_LEN);

        pTemp = (i & 0x01) ? tmp2 : tmp;

        for (j = 0; j < SHA1_MAC_LEN; j++)
            digest[j] ^= pTemp[j];
    }

    return TCPIP_MAC_RES_OK;
}
#endif //#if defined(STATEMACHINE_pbkdf2_sha1)

typedef struct {
    u32 state[5];
    u32 count[2];
    unsigned char buffer[64];
} SHA1_CTX;

static void SHA1Init(SHA1_CTX *context);
static void SHA1Update(SHA1_CTX *context, const void *data, u32 len);
static void SHA1Final(unsigned char digest[20], SHA1_CTX* context);
static void SHA1Transform(u32 state[5], const unsigned char buffer[64]);

/**
 * sha1_vector - SHA-1 hash for data vector
 * @num_elem: Number of elements in the data vector
 * @addr: Pointers to the data areas
 * @len: Lengths of the data blocks
 * @mac: Buffer for the hash
 */
void sha1_vector(size_t num_elem, const u8 *addr[], const size_t *len,
         u8 *mac)
{
    SHA1_CTX ctx;
    int i;

    SHA1Init(&ctx);
    for (i = 0; i < num_elem; i++)
        SHA1Update(&ctx, addr[i], len[i]);
    SHA1Final(mac, &ctx);
}

/**
 * sha1_transform - Perform one SHA-1 transform step
 * @state: SHA-1 state
 * @data: Input data for the SHA-1 transform
 *
 * This function is used to implement random number generation specified in
 * NIST FIPS Publication 186-2 for EAP-SIM. This PRF uses a function that is
 * similar to SHA-1, but has different message padding and as such, access to
 * just part of the SHA-1 is needed.
 */
void sha1_transform(u8 *state, const u8 data[64])
{
    SHA1Transform((u32 *) state, data);
}

/* ===== start - public domain SHA1 implementation ===== */

/*
SHA-1 in C
By Steve Reid <sreid@sea-to-sky.net>
100% Public Domain

-----------------
Modified 7/98
By James H. Brown <jbrown@burgoyne.com>
Still 100% Public Domain

Corrected a problem which generated improper hash values on 16 bit machines
Routine SHA1Update changed from
    void SHA1Update(SHA1_CTX* context, unsigned char* data, unsigned int
len)
to
    void SHA1Update(SHA1_CTX* context, unsigned char* data, unsigned
long len)

The 'len' parameter was declared an int which works fine on 32 bit machines.
However, on 16 bit machines an int is too small for the shifts being done
against
it.  This caused the hash function to generate incorrect values if len was
greater than 8191 (8K - 1) due to the 'len << 3' on line 3 of SHA1Update().

Since the file IO in main() reads 16K at a time, any file 8K or larger would
be guaranteed to generate the wrong hash (e.g. Test Vector #3, a million
"a"s).

I also changed the declaration of variables i & j in SHA1Update to
unsigned long from unsigned int for the same reason.

These changes should make no difference to any 32 bit implementations since
an
int and a long are the same size in those environments.

--
I also corrected a few compiler warnings generated by Borland C.
1. Added #include <process.h> for exit() prototype
2. Removed unused variable 'j' in SHA1Final
3. Changed exit(0) to return(0) at end of main.

ALL changes I made can be located by searching for comments containing 'JHB'
-----------------
Modified 8/98
By Steve Reid <sreid@sea-to-sky.net>
Still 100% public domain

1- Removed #include <process.h> and used return() instead of exit()
2- Fixed overwriting of finalcount in SHA1Final() (discovered by Chris Hall)
3- Changed email address from steve@edmweb.com to sreid@sea-to-sky.net

-----------------
Modified 4/01
By Saul Kravitz <Saul.Kravitz@celera.com>
Still 100% PD
Modified to run on Compaq Alpha hardware.

-----------------
Modified 4/01
By Jouni Malinen <jkmaline@cc.hut.fi>
Minor changes to match the coding style used in Dynamics.

Modified September 24, 2004
By Jouni Malinen <jkmaline@cc.hut.fi>
Fixed alignment issue in SHA1Transform when SHA1HANDSOFF is defined.

*/

/*
Test Vectors (from FIPS PUB 180-1)
"abc"
  A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
  84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
A million repetitions of "a"
  34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#if LITTLEENDIAN == 1
#define blk0(i) (block->l[i] = (rol(block->l[i], 24) & 0xFF00FF00) | \
    (rol(block->l[i], 8) & 0x00FF00FF))
#else
#define blk0(i) block->l[i]
#endif
#define blk(i) (block->l[i & 15] = rol(block->l[(i + 13) & 15] ^ \
    block->l[(i + 8) & 15] ^ block->l[(i + 2) & 15] ^ block->l[i & 15], 1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) \
    z += ((w & (x ^ y)) ^ y) + blk0(i) + 0x5A827999 + rol(v, 5); \
    w = rol(w, 30);
#define R1(v,w,x,y,z,i) \
    z += ((w & (x ^ y)) ^ y) + blk(i) + 0x5A827999 + rol(v, 5); \
    w = rol(w, 30);
#define R2(v,w,x,y,z,i) \
    z += (w ^ x ^ y) + blk(i) + 0x6ED9EBA1 + rol(v, 5); w = rol(w, 30);
#define R3(v,w,x,y,z,i) \
    z += (((w | x) & y) | (w & x)) + blk(i) + 0x8F1BBCDC + rol(v, 5); \
    w = rol(w, 30);
#define R4(v,w,x,y,z,i) \
    z += (w ^ x ^ y) + blk(i) + 0xCA62C1D6 + rol(v, 5); \
    w=rol(w, 30);

#ifdef VERBOSE  /* SAK */
void SHAPrintContext(SHA1_CTX *context, char *msg)
{
    printf("%s (%d,%d) %x %x %x %x %x\n",
           msg,
           context->count[0], context->count[1],
           context->state[0],
           context->state[1],
           context->state[2],
           context->state[3],
           context->state[4]);
}
#endif

/* Hash a single 512-bit block. This is the core of the algorithm. */

static void SHA1Transform(u32 state[5], const unsigned char buffer[64])
{
    u32 a, b, c, d, e;
    typedef union {
        unsigned char c[64];
        u32 l[16];
    } CHAR64LONG16;
    CHAR64LONG16* block;

    u32 workspace[16];
    block = (CHAR64LONG16 *) workspace;
    memcpy((U8*)block, (U8*)buffer, 64);
    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    /* Wipe variables */
    a = b = c = d = e = 0;

    memset((U8*)block, 0, 64);
}

/* SHA1Init - Initialize new context */
static void SHA1Init(SHA1_CTX* context)
{
    /* SHA1 initialization constants */
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->count[0] = context->count[1] = 0;
}

/* Run your data through this. */

static void SHA1Update(SHA1_CTX* context, const void *_data, u32 len)
{
    u32 i, j;
    const unsigned char *data = _data;

#ifdef VERBOSE
    SHAPrintContext(context, "before");
#endif
    j = (context->count[0] >> 3) & 63;
    if ((context->count[0] += len << 3) < (len << 3))
        context->count[1]++;
    context->count[1] += (len >> 29);
    if ((j + len) > 63) {
        memcpy((U8*)(&context->buffer[j]), (U8*)data, (i = 64-j));
        SHA1Transform(context->state, context->buffer);
        for ( ; i + 63 < len; i += 64) {
            SHA1Transform(context->state, &data[i]);
        }
        j = 0;
    }
    else i = 0;
    memcpy((U8*)(&context->buffer[j]), (U8*)(&data[i]), len - i);
#ifdef VERBOSE
    SHAPrintContext(context, "after ");
#endif
}

/* Add padding and return the message digest. */
static void SHA1Final(unsigned char digest[20], SHA1_CTX* context)
{
    u32 i;
    unsigned char finalcount[8];

    for (i = 0; i < 8; i++) {
        finalcount[i] = (unsigned char)
            ((context->count[(i >= 4 ? 0 : 1)] >>
              ((3-(i & 3)) * 8) ) & 255);  /* Endian independent */
    }
    SHA1Update(context, (unsigned char *) "\200", 1);
    while ((context->count[0] & 504) != 448) {
        SHA1Update(context, (unsigned char *) "\0", 1);
    }
    SHA1Update(context, finalcount, 8);  /* Should cause a SHA1Transform()
                          */
    for (i = 0; i < 20; i++) {
        digest[i] = (unsigned char)
            ((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) &
             255);
    }
    /* Wipe variables */
    i = 0;
    memset(context->buffer, 0, 64);
    memset((U8*)(context->state), 0, 20);
    memset((U8*)(context->count), 0, 8);
    memset(finalcount, 0, 8);
}

#endif // TCPIP_IF_MRF24W

//DOM-IGNORE-END
