/*******************************************************************************
Microchip DSP Library
    Cosine (Sine) Windows on a Vector

  Company:
    Microchip Technology Inc.

  File Name:
    dsp_transform_window_cosine.c

  Summary:
    Perform a Cosine (Sine) window on a vector.

  Description:
    Compute a Cosine Window on the first N samples of the input vector, InVector.
    The output is stored in OutWindow.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Both 16-bit and 32-bit functions
    are provided.  Input and output values are in Q15 fractional format and Q31
    fractional format respectively. 
    The Cosine Window follows the equation:

       Window(n) = SIN(Pi*n/(N-1))
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

   Notes:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
*******************************************************************************/

/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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
/****************************************************************************
Change log

new content     C15637          2013-10-10

****************************************************************************/

#include <math.h>
#include <stdint.h>
#include "dsp.h"


void DSP_TransformWindow_Cosine16(int16_t *OutVector, int16_t *InVector, int N)
{
	int i;
        double Window;
        int16_t WinFract;
        int32_t WinOut32;
	
	for (i = 0; i < N; i++)
	{
            Window = sin((M_PI * i) / (N-1));   // window function
             if (Window >= 1.0)                        // handle saturation
            {
                WinFract = 0x7FFF;
            }
            else
            {
                WinFract = Window * pow(2,15);      // convert to Q15
            }

            WinOut32 = WinFract * InVector[i];      // process vector
            if (WinOut32<0)                         // round and shift to Q15 output
            {
                OutVector[i] = (WinOut32-(1<<14))>>15;
            }
            else
            {
                OutVector[i]=(WinOut32+(1<<14))>>15;
            }
        }
}

void DSP_TransformWindow_Cosine32(int32_t *OutVector, int32_t *InVector, int N)
{
	int i;
        long double Window;
        int32_t WinFract;
        int64_t WinOut64;
        
	for (i = 0; i < N; i++)
	{
            Window = sin((M_PI * i) / (N-1));   // window function
             if (Window >= 1.0)                        // handle saturation
            {
                WinFract = 0x7FFFFFFF;
            }
            else
            {
                WinFract = Window * pow(2,31);      // convert to Q31
            }

            WinOut64 = (int64_t)WinFract * (int64_t)InVector[i];  // process vector
            if (WinOut64<0)                         // round and shift to Q31 output
            {
                OutVector[i] = (WinOut64-(1<<30))>>31;
            }
            else
            {
                OutVector[i]=(WinOut64+(1<<30))>>31;
            }
        }
}