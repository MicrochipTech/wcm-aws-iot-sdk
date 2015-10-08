/*******************************************************************************
  Hash Function Library

  Summary:
    Library for Microchip TCP/IP Stack
    
  Description:
    - Calculates MD5 and SHA-1 Hashes
    - Reference: RFC 1321 (MD5), RFC 3174 and FIPS 180-1 (SHA-1)
*******************************************************************************/

/*******************************************************************************
File Name:  Hashes.c
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

#include <string.h>

#include "hashes.h"

/*****************************************************************************
  Internal:
  	Performance Statistics are as follows:
  	(Given in instructions per block = 512 bits = 64 bytes)
	
					MD5					SHA1
		Hi-Tech C	19k instr/block		50k instr/block
		C30			21k instr/block		17k instr/block
		
  ***************************************************************************/


/****************************************************************************
  Section:
	Functions and variables required for both hash types
  ***************************************************************************/
typedef union
{
    uint32_t Val;
    uint16_t w[2] __attribute__((packed));
    uint8_t  v[4];
} HASH_UINT32_VAL;

// Rotations are more efficient in C30 and C32
#define leftRotateDWORD(x, n) (((x) << (n)) | ((x) >> (32-(n))))

// Stores a copy of the last block with the required padding
uint8_t lastBlock[64] __attribute__ ((aligned (4)));

/*****************************************************************************
  Function:
	void HashAddData(HASH_SUM* theSum, const uint8_t* data, uint16_t len)

  Description:
	Adds data to the hash sum.

  Precondition:
	The hash sum has already been initialized

  Parameters:
	theSum - hash context state
	data - the data to be added to the hash sum
	len - length of data

  Returns:
  	None
  	
  Remarks:
	This function calls the appropriate hashing function based on the 
	hash typed defined in theSum.
  ***************************************************************************/
void HashAddData(HASH_SUM* theSum, const uint8_t* data, uint16_t len)
{
	if(theSum->hashType == HASH_MD5)
    {
		MD5AddData(theSum, data, len);
    }
    else if(theSum->hashType == HASH_SHA1)
    {
		SHA1AddData(theSum, data, len);
    }
}




/****************************************************************************
  Section:
	Functions and variables required for MD5
  ***************************************************************************/


// Array of pre-defined R vales for MD5
static const uint8_t _MD5_r[64] = {7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
				  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
				  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
				  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

// Array of pre-defined K values for MD5
static const uint32_t _MD5_k[64] = { 0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501, 
							0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821, 
							0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8, 
							0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A, 
							0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70, 
							0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05, 0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665, 
							0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1, 
							0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, 0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391 };

static void MD5HashBlock(uint8_t* data, uint32_t* h0, uint32_t* h1, uint32_t* h2, uint32_t* h3);

/*****************************************************************************
  Function:
	void MD5Initialize(HASH_SUM* theSum)

  Description:
	Initializes a new MD5 hash.

  Precondition:
	None

  Parameters:
	theSum - pointer to the allocated HASH_SUM object to initialize as MD5

  Returns:
  	None
  ***************************************************************************/
void MD5Initialize(HASH_SUM* theSum)
{
	theSum->h0 = 0x67452301;
	theSum->h1 = 0xefcdab89;
	theSum->h2 = 0x98badcfe;
	theSum->h3 = 0x10325476;
	theSum->bytesSoFar = 0;
	theSum->hashType = HASH_MD5;
}

/*****************************************************************************
  Function:
	void MD5AddData(HASH_SUM* theSum, const uint8_t* data, uint16_t len)

  Description:
	Adds data to an MD5 hash calculation.

  Precondition:
	The hash context has already been initialized.

  Parameters:
	theSum - a pointer to the hash context structure
	data - the data to add to the hash
	len - the length of the data to add

  Returns:
  	None
  ***************************************************************************/
void MD5AddData(HASH_SUM* theSum, const uint8_t* data, uint16_t len)
{
	uint8_t *blockPtr;

	// Seek to the first free byte
	blockPtr = theSum->partialBlock + ( theSum->bytesSoFar & 0x3f );

	// Update the total number of bytes
	theSum->bytesSoFar += len;

	// Copy data into the partial block
	while(len != 0u)
	{
		*blockPtr++ = *data++;

		// If the partial block is full, hash the data and start over
		if(blockPtr == theSum->partialBlock + 64)
		{
			MD5HashBlock(theSum->partialBlock, &theSum->h0, &theSum->h1, &theSum->h2, &theSum->h3);
			blockPtr = theSum->partialBlock;
		}
		
		len--;
	}
	
}


/*****************************************************************************
  Function:
	static void MD5HashBlock(uint8_t* data, uint32_t* h0, uint32_t* h1, 
								uint32_t* h2, uint32_t* h3)

  Summary:
	Calculates the MD5 hash sum of a block.

  Description:
	This function calculates the MD5 hash sum over a block and updates
	the values of h0-h3 with the next context.

  Precondition:
	The data pointer must be uint16_t aligned on 16-bit parts and uint32_t
	aligned on 32-bit PICs.  If alignment is not correct, a memory alignment
	exception will occur.

  Parameters:
	data - The block of 64 bytes to hash
	h0 - the current hash context h0 value
	h1 - the current hash context h1 value
	h2 - the current hash context h2 value
	h3 - the current hash context h3 value

  Returns:
  	None
  
  ***************************************************************************/
static void MD5HashBlock(uint8_t* data, uint32_t* h0, uint32_t* h1, uint32_t* h2, uint32_t* h3)
{
	uint32_t a, b, c, d, f, temp;
	uint8_t i, j;

	// Set up a, b, c, d
	a = *h0;
	b = *h1;
	c = *h2;
	d = *h3;

	// Main mixer loop for 64 operations
	for(i = 0; i < 64u; i++)
	{
		if(i <= 15u)
		{
			f = (b & c) | ((~b) & d);
			j = i;
		}
		else if(i > 15u && i <= 31u)
		{
			f = (d & b) | ((~d) & c);
			j = (5 * i + 1) & 0x0f;
		}
		else if(i > 31u && i <= 47u)
		{
			f = (b ^ c ^ d);
			j = (3 * i + 5) & 0x0f;
		}
		else
		{
			f = c ^ (b | (~d));
			j = (7 * i) & 0x0f;
		}
		
		// Calculate the new mixer values
		temp = d;
		d = c;
		c = b;
		j *= 4;
		b = leftRotateDWORD(a+f+_MD5_k[i]+(*(uint32_t*)&data[j]),_MD5_r[i]) + b;
		a = temp;
	}

	// Add the new hash to the sum
	*h0 += a;
	*h1 += b;
	*h2 += c;
	*h3 += d;

}

/*****************************************************************************
  Function:
	void MD5Calculate(HASH_SUM* theSum, uint8_t* result)

  Summary:
	Calculates an MD5 hash

  Description:
	This function calculates the hash sum of all input data so far.  It is
	non-destructive to the hash context, so more data may be added after
	this function is called.

  Precondition:
	The hash context has been properly initialized.

  Parameters:
	theSum - the current hash context
	result - 16 byte array in which to store the resulting hash

  Returns:
  	None
  ***************************************************************************/
void MD5Calculate(HASH_SUM* theSum, uint8_t* result)
{
	uint32_t h0, h1, h2, h3;
	uint8_t i, *partPtr, *endPtr;

	// Initialize the hash variables
	h0 = theSum->h0;
	h1 = theSum->h1;
	h2 = theSum->h2;
	h3 = theSum->h3;

	// Find out how far along we are in the partial block and copy to last block
	partPtr = theSum->partialBlock;
	endPtr = partPtr + ( theSum->bytesSoFar & 0x3f );
	for(i = 0; partPtr != endPtr; i++)
	{
		lastBlock[i] = *partPtr++;
	}

	// Add one more 1 bit and 7 zeros
	lastBlock[i++] = 0x80;

	// If there's 8 or more bytes left to 64, then this is the last block
	if(i > 56u)
	{// If there's not enough space, then zero fill this and add a new block
		// Zero pad the remainder
		for( ; i < 64u; lastBlock[i++] = 0x00);

		// Calculate a hash on this block and add it to the sum
		MD5HashBlock(lastBlock, &h0, &h1, &h2, &h3);

		// Create a new block for the size
		i = 0;
	}

	// Zero fill the rest of the block
	for( ; i < 56u; lastBlock[i++] = 0x00);

	// Fill in the size, in bits, in little-endian
	lastBlock[56] = theSum->bytesSoFar << 3;
	lastBlock[57] = theSum->bytesSoFar >> 5;
	lastBlock[58] = theSum->bytesSoFar >> 13;
	lastBlock[59] = theSum->bytesSoFar >> 21;
	lastBlock[60] = theSum->bytesSoFar >> 29;
	lastBlock[61] = 0;
	lastBlock[62] = 0;
	lastBlock[63] = 0;

	// Calculate a hash on this final block and add it to the sum
	MD5HashBlock(lastBlock, &h0, &h1, &h2, &h3);
	
	// Format the result in little-endian format
	memcpy((void*)result,(void*)&h0,4);
	memcpy((void*)&result[4],(void*)&h1,4);
	memcpy((void*)&result[8],(void*)&h2,4);
	memcpy((void*)&result[12],(void*)&h3,4);
}


/****************************************************************************
  Section:
	Functions and variables required for SHA-1
  ***************************************************************************/


static void SHA1HashBlock(uint8_t* data, uint32_t* h0, uint32_t* h1, uint32_t* h2, uint32_t* h3, uint32_t* h4);

/*****************************************************************************
  Function:
	void SHA1Initialize(HASH_SUM* theSum)

  Description:
	Initializes a new SHA-1 hash.

  Precondition:
	None

  Parameters:
	theSum - pointer to the allocated HASH_SUM object to initialize as SHA-1

  Returns:
  	None
  ***************************************************************************/
void SHA1Initialize(HASH_SUM* theSum)
{
	theSum->h0 = 0x67452301;
	theSum->h1 = 0xEFCDAB89;
	theSum->h2 = 0x98BADCFE;
	theSum->h3 = 0x10325476;
	theSum->h4 = 0xC3D2E1F0;
	theSum->bytesSoFar = 0;
	theSum->hashType = HASH_SHA1;
}

/*****************************************************************************
  Function:
	void SHA1AddData(HASH_SUM* theSum, const uint8_t* data, uint16_t len)

  Description:
	Adds data to a SHA-1 hash calculation.

  Precondition:
	The hash context has already been initialized.

  Parameters:
	theSum - a pointer to the hash context structure
	data - the data to add to the hash
	len - the length of the data to add

  Returns:
  	None
  ***************************************************************************/
void SHA1AddData(HASH_SUM* theSum, const uint8_t* data, uint16_t len)
{
	uint8_t *blockPtr;

	// Seek to the first free byte
	blockPtr = theSum->partialBlock + ( theSum->bytesSoFar & 0x3f );

	// Update the total number of bytes
	theSum->bytesSoFar += len;

	// Copy data into the partial block
	while(len != 0u)
	{
		*blockPtr++ = *data++;

		// If the partial block is full, hash the data and start over
		if(blockPtr == theSum->partialBlock + 64)
		{
			SHA1HashBlock(theSum->partialBlock, &theSum->h0, &theSum->h1,
							 &theSum->h2, &theSum->h3, &theSum->h4);
			blockPtr = theSum->partialBlock;
		}
		
		len--;
	}
	
}


/*****************************************************************************
  Function:
	static void SHA1HashBlock(uint8_t* data, uint32_t* h0, uint32_t* h1, 
								uint32_t* h2, uint32_t* h3, uint32_t* h4)

  Summary:
	Calculates the SHA-1 hash sum of a block.

  Description:
	This function calculates the SHA-1 hash sum over a block and updates
	the values of h0-h3 with the next context.

  Precondition:
	The data pointer must be uint16_t aligned on 16-bit parts and uint32_t
	aligned on 32-bit PICs.  If alignment is not correct, a memory alignment
	exception will occur.

  Parameters:
	data - The block of 64 bytes to hash
	h0 - the current hash context h0 value
	h1 - the current hash context h1 value
	h2 - the current hash context h2 value
	h3 - the current hash context h3 value
	h4 - the current hash context h4 value

  Returns:
  	None
  
  ***************************************************************************/
static void SHA1HashBlock(uint8_t* data, uint32_t* h0, uint32_t* h1, uint32_t* h2, 
							uint32_t* h3, uint32_t* h4)
{
	uint32_t a, b, c, d, e, f, k, temp;
	HASH_UINT32_VAL *w = (HASH_UINT32_VAL*)lastBlock;
	uint8_t i, back3, back8, back14;

	// Set up a, b, c, d, e
	a = *h0;
	b = *h1;
	c = *h2;
	d = *h3;
	e = *h4;

	// Set up the w[] vector
	if(lastBlock == data)
	{// If they're the same, just swap endian-ness
		for(i = 0; i < 16u; i++)
		{
			back3 = data[3];
			data[3] = data[0];
			data[0] = back3;
			back3 = data[1];
			data[1] = data[2];
			data[2] = back3;
			data += 4;
		}
	}
	else
	{// Otherwise, copy values in swaping endian-ness as we go
		for(i = 0; i < 16u; i++)
		{
			w[i].v[3] = *data++;
			w[i].v[2] = *data++;
			w[i].v[1] = *data++;
			w[i].v[0] = *data++;
		}
	}
	back3 = 13;
	back8 = 8;
	back14 = 2;

	// Main mixer loop for 80 operations
	for(i = 0; i < 80u; i++)
	{
		if(i <= 19u)
		{
			f = (b & c) | ((~b) & d);
			k = 0x5A827999;
		}
		else if(i >= 20u && i <= 39u)
		{
			f = b ^ c ^ d;
			k = 0x6ED9EBA1;
		}
		else if(i >= 40u && i <= 59u)
		{
			f = (b & c) | (b & d) | (c & d);
			k = 0x8F1BBCDC;
		}
		else
		{
			f = b ^ c ^ d;
			k = 0xCA62C1D6;
		}

		// Calculate the w[] value and store it in the array for future use
		if(i >= 16u)
		{
			w[i&0x0f].Val = leftRotateDWORD( ( w[back3].Val ^ w[back8].Val ^
											w[back14].Val ^ w[i&0x0f].Val), 1);
			back3 += 1;
			back8 += 1;
			back14 += 1;
			back3 &= 0x0f;
			back8 &= 0x0f;
			back14 &= 0x0f;
		}
		
		// Calculate the new mixers
		temp = leftRotateDWORD(a, 5) + f + e + k + w[i & 0x0f].Val;
		e = d;
		d = c;
		c = leftRotateDWORD(b, 30);
		b = a;
		a = temp;
	}

	// Add the new hash to the sum
	*h0 += a;
	*h1 += b;
	*h2 += c;
	*h3 += d;
	*h4 += e;

}

/*****************************************************************************
  Function:
	void SHA1Calculate(HASH_SUM* theSum, uint8_t* result)

  Summary:
	Calculates a SHA-1 hash

  Description:
	This function calculates the hash sum of all input data so far.  It is
	non-destructive to the hash context, so more data may be added after
	this function is called.

  Precondition:
	The hash context has been properly initialized.

  Parameters:
	theSum - the current hash context
	result - 20 byte array in which to store the resulting hash

  Returns:
  	None
  ***************************************************************************/
void SHA1Calculate(HASH_SUM* theSum, uint8_t* result)
{
	uint32_t h0, h1, h2, h3, h4;
	uint8_t i, *partPtr, *endPtr;

	// Initialize the hash variables
	h0 = theSum->h0;
	h1 = theSum->h1;
	h2 = theSum->h2;
	h3 = theSum->h3;
	h4 = theSum->h4;

	// Find out how far along we are in the partial block and copy to last block
	partPtr = theSum->partialBlock;
	endPtr = partPtr + ( theSum->bytesSoFar & 0x3f );
	for(i = 0; partPtr != endPtr; i++)
	{
		lastBlock[i] = *partPtr++;
	}

	// Add one more bit and 7 zeros
	lastBlock[i++] = 0x80;

	// If there's 8 or more bytes left to 64, then this is the last block
	if(i > 56u)
	{// If there's not enough space, then zero fill this and add a new block
		// Zero pad the remainder
		for( ; i < 64u; lastBlock[i++] = 0x00);

		// Calculate a hash on this block and add it to the sum
		SHA1HashBlock(lastBlock, &h0, &h1, &h2, &h3, &h4);

		//create a new block for the size
		i = 0;
	}

	// Zero fill the rest of the block
	for( ; i < 56u; lastBlock[i++] = 0x00);

	// Fill in the size, in bits, in big-endian
	lastBlock[63] = theSum->bytesSoFar << 3;
	lastBlock[62] = theSum->bytesSoFar >> 5;
	lastBlock[61] = theSum->bytesSoFar >> 13;
	lastBlock[60] = theSum->bytesSoFar >> 21;
	lastBlock[59] = theSum->bytesSoFar >> 29;
	lastBlock[58] = 0;
	lastBlock[57] = 0;
	lastBlock[56] = 0;

	// Calculate a hash on this final block and add it to the sum
	SHA1HashBlock(lastBlock, &h0, &h1, &h2, &h3, &h4);
	
	// Format the result in big-endian format
	*result++ = ((uint8_t*)&h0)[3];
	*result++ = ((uint8_t*)&h0)[2];
	*result++ = ((uint8_t*)&h0)[1];
	*result++ = ((uint8_t*)&h0)[0];
	*result++ = ((uint8_t*)&h1)[3];
	*result++ = ((uint8_t*)&h1)[2];
	*result++ = ((uint8_t*)&h1)[1];
	*result++ = ((uint8_t*)&h1)[0];
	*result++ = ((uint8_t*)&h2)[3];
	*result++ = ((uint8_t*)&h2)[2];
	*result++ = ((uint8_t*)&h2)[1];
	*result++ = ((uint8_t*)&h2)[0];
	*result++ = ((uint8_t*)&h3)[3];
	*result++ = ((uint8_t*)&h3)[2];
	*result++ = ((uint8_t*)&h3)[1];
	*result++ = ((uint8_t*)&h3)[0];
	*result++ = ((uint8_t*)&h4)[3];
	*result++ = ((uint8_t*)&h4)[2];
	*result++ = ((uint8_t*)&h4)[1];
	*result++ = ((uint8_t*)&h4)[0];
}


