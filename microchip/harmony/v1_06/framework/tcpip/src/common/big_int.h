/*******************************************************************************
  Big Integer Class Headers

  Summary:
    
  Description:
*******************************************************************************/

/*******************************************************************************
File Name:  big_int.h
Copyright © 2012 released Microchip Technology Inc.  All rights
reserved.

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

#ifndef __BIGINT_H
#define __BIGINT_H

#include <stdint.h>
#include <stdbool.h>

#define BIGINT_DEBUG			0
#define BIGINT_DEBUG_COMPARE 	0
#define RSAEXP_DEBUG			0

#if defined(__C30__)
	#define BIGINT_DATA_SIZE	16ul	//bits
	#define BIGINT_DATA_TYPE	uint16_t
	#define BIGINT_DATA_MAX		0xFFFFu
	#define BIGINT_DATA_TYPE_2	uint32_t
#elif defined(__C32__)
	#define BIGINT_DATA_SIZE	32ul	//bits
	#define BIGINT_DATA_TYPE	uint32_t
	#define BIGINT_DATA_MAX		0xFFFFFFFFu
	#define BIGINT_DATA_TYPE_2	uint64_t
#endif

typedef struct
{
	BIGINT_DATA_TYPE *ptrLSB;		// Pointer to the least significant byte/word (lowest memory address)
	BIGINT_DATA_TYPE *ptrMSB;		// Pointer to the first non-zero most significant byte/word (higher memory address) if bMSBValid set
	BIGINT_DATA_TYPE *ptrMSBMax;	// Pointer to the maximum memory address that ptrMSB could ever be (highest memory address)
	bool bMSBValid; 
} BIGINT;



void BigInt(BIGINT *theInt, BIGINT_DATA_TYPE *data, uint16_t wWordLength);
void BigIntMod(BIGINT*, BIGINT*);
void BigIntMultiply(BIGINT*, BIGINT*, BIGINT*);

void BigIntAdd(BIGINT*, BIGINT*);
void BigIntSubtract(BIGINT*, BIGINT*);
void BigIntCopy(BIGINT*, BIGINT*);
void BigIntSquare(BIGINT *a, BIGINT *res);
void BigIntZero(BIGINT *theInt);

int BigIntMagnitudeDifference(BIGINT *a, BIGINT *b);
int8_t BigIntCompare(BIGINT*, BIGINT*);
uint16_t BigIntMagnitude(BIGINT *n);

void BigIntSwapEndianness(BIGINT *a);

void BigIntPrint(const BIGINT *a);


#endif
