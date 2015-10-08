/*******************************************************************************
  Hash Function Library Headers

  Summary:
    
  Description:
*******************************************************************************/

/*******************************************************************************
File Name: Hashes.h 
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

#ifndef __HASHES_H
#define __HASHES_H

#include <stdint.h>
#include <stdbool.h>

/****************************************************************************
  Section:
	Data Types
  ***************************************************************************/

// Type of hash being calculated
typedef enum
{
	HASH_MD5	= 0u,		// MD5 is being calculated
	HASH_SHA1				// SHA-1 is being calculated
} HASH_TYPE;

// Context storage for a hash operation
typedef struct
{
	uint32_t h0;				// Hash state h0
	uint32_t h1;				// Hash state h1
	uint32_t h2;				// Hash state h2
	uint32_t h3;				// Hash state h3
	uint32_t h4;				// Hash state h4
	uint32_t bytesSoFar;		// Total number of bytes hashed so far
	uint8_t partialBlock[64];	// Beginning of next 64 byte block
	HASH_TYPE hashType;		// Type of hash being calculated
} HASH_SUM;


/****************************************************************************
  Section:
	Function Prototypes
  ***************************************************************************/

void SHA1Initialize(HASH_SUM* theSum);
void SHA1AddData(HASH_SUM* theSum, const uint8_t* data, uint16_t len);
void SHA1Calculate(HASH_SUM* theSum, uint8_t* result);

void MD5Initialize(HASH_SUM* theSum);
void MD5AddData(HASH_SUM* theSum, const uint8_t* data, uint16_t len);
void MD5Calculate(HASH_SUM* theSum, uint8_t* result);

void HashAddData(HASH_SUM* theSum, const uint8_t* data, uint16_t len);

#endif

