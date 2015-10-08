/*******************************************************************************

Summary: LFSR generation

Description: simple Linear Feedback Shift Register implementation

 *******************************************************************************/

/*******************************************************************************
File Name:	lfsr.h
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


#ifndef __LFSR_H_
#define __LFSR_H_


#include <stdint.h>
#include <stdbool.h>


/*****************************************************************************
  Function:
	uint32_t LFSRSeedRand(uint32_t dwSeed)

  Summary:
	Seeds the LFSR random number generator invoked by the LFSRRand() function.  
	The prior seed is returned.

  Description:
	Seeds the LFSR random number generator invoked by the LFSRRand() function.  
	The prior seed is returned.

  Precondition:
	None

  Parameters:
	wSeed - The new 32-bit seed value to assign to the LFSR.

  Returns:
  	The last seed in use.  This can be saved and restored by a subsequent call 
	to LFSRSeedRand() if you wish to use LFSRRand() in multiple contexts 
	without disrupting the random number sequence from the alternative 
	context.  For example, if App 1 needs a given sequence of random numbers 
	to perform a test, if you save and restore the seed in App 2, it is 
	possible for App 2 to not disrupt the random number sequence provided to 
	App 1, even if the number of times App 2 calls LFSRRand() varies.
  	
  Side Effects:
	None
	
  Remarks:
	Upon initial power up, the internal seed is initialized to 0x1.  Using a 
	dwSeed value of 0x0 will return the same sequence of random numbers as 
	using the seed of 0x1.
  ***************************************************************************/
uint32_t    LFSRSeedRand(uint32_t dwSeed);


/*****************************************************************************
  Function:
	uint16_t LFSRRand(void)

  Summary:
	Returns a pseudo-random 16-bit unsigned integer in the range from 0 
	to 65535 (0x0000 to 0xFFFF).

  Description:
	Returns a pseudo-random 16-bit unsigned integer in the range from 0 
	to 65535 (0x0000 to 0xFFFF).  The random number is generated using a 
	Linear Feedback Shift Register (LFSR) type pseudo-random number generator 
	algorithm.  The LFSR can be seeded by calling the LFSRSeedRand() function
	to generate the same sequence of random numbers as a prior string of calls.
	
	The internal LFSR will repeat after 2^32-1 iterations.

  Precondition:
	None

  Parameters:
	None

  Returns:
  	Random 16-bit unsigned integer.
  	
  Side Effects:
	The internal LFSR seed is updated so that the next call to LFSRRand() 
	will return a different random number.
	
  Remarks:
	None
  ***************************************************************************/
uint16_t        LFSRRand(void);


#endif  // __LFSR_H_



