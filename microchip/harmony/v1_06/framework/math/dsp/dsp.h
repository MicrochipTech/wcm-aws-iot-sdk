/*******************************************************************************
 Digital Signal Processing (DSP) Library

  Company:
    Microchip Technology Inc.

  File Name:
    dsp.h

  Summary:
    DSP functions for the PIC32MZ device family.

  Description:
    The DSP Library provides functions that are optimized for performance on the 
	PIC32MZ families of devices that have microAptiv core features and utilize DSP ASE.
    The library provides advanced mathematical operations for complex numbers, vector 
	and matrix mathematics, digital filtering and transforms.

    All functions are implemented in efficient assembly with C-callable
    prototypes.  In some cases both 16-bit and 32-bit functions are supplied
    to enable the user with a choice of resolution and performance.

    For most functions, input and output data is represented by 16-bit
    fractional numbers in Q15 format, which is the most commonly used data
    format for signal processing. Some functions use other data formats
    internally for increased precision of intermediate results.

    The Q15 data type used by the DSP functions is specified as int16_t in the C
    header file that is supplied with the library. Note that within C code, care
    must be taken to avoid confusing fixed-point values with integers. To the C
    compiler, objects declared with int16_t type are integers, not fixed-point,
    and all arithmetic operations performed on those objects in C will be done
    as integers. Fixed-point values have been declared as int16_t only because the
    standard C language does not include intrinsic support for fixed-point
    data types.

    Some functions also have versions operating on 32-bit fractional data in Q31
    format. These functions operate similarly to their 16-bit counterparts.
    However, it should be noted that the 32-bit functions do not benefit much
    from the SIMD capabilities offered by DSP ASE. Thus, the performance of the
    32-bit functions is generally reduced compared to the performance of the
    corresponding 16-bit functions.
  
   Signed fixed point types are defined as follows:

        Qn.m where:
            - n is the number of data bits to the left of the radix point
            - m is the number of data bits to the right of the radix point
            - a signed bit is implied

    Unique variable types for fractional representation are also defined:

        Exact Name       # Bits Required            Type
         Q0.15 (Q15)             16                int16_t
         Q0.31 (Q31)             32                int32_t

    Table of DSP Library functions:

  Complex Math:

     void DSP_ComplexAdd32(int32c *indata1, int32c *indata2, int32c *Output);

     void DSP_ComplexConj16(int16c *indata, int16c *Output);
 
     void DSP_ComplexConj32(int32c *indata, int32c *Output);
 
     void DSP_ComplexDotProd32(int32c *indata1, int32c *indata2, int32c *Output);

     void DSP_ComplexMult32(int32c *indata1, int32c *indata2, int32c *Output);
  
     void DSP_ComplexScalarMult32(int32c *indata, int32_t Scalar, int32c *Output);
 
     void DSP_ComplexSub32(int32c *indata1, int32c *indata2, int32c *Output);

   Filter Functions:

     void DSP_FilterFIR32(int32_t *outdata, int32_t *indata, int32_t *coeffs2x,
            int32_t *delayline, int N, int K, int scale);

     void DSP_FilterFIRDecim32(int32_t *outdata, int32_t *indata, int32_t *coeffs,
            int32_t *delayline, int N, int K, int scale, int rate);

     int16_t DSP_FilterIIR16(int16_t in, int16_t *coeffs, int16_t *delayline, int B,
            int scale);

     void DSP_FilterIIRSetup16(int16_t *coeffs, biquad16 *bq, int B);

     void DSP_FilterFIRInterp32(int32_t *outdata, int32_t *indata, int32_t *coeffs,
             int32_t *delayline, int N, int K, int scale, int rate);

     int16_t DSP_FilterLMS16(int16_t in, int16_t ref, int16_t *coeffs, int16_t *delayline,
                                        int16_t *error, int K, int16_t mu);

     int16_t DSP_FilterIIRBQ16_fast(int16_t Xin, PARM_EQUAL_FILTER_16 *pFilter);

     int16_t DSP_FilterIIRBQ16(int16_t Xin, PARM_EQUAL_FILTER *pFilter);

     int32_t DSP_FilterIIRBQ32(int32_t Xin, PARM_EQUAL_FILTER_32 *pFilter);

     int16_t DSP_FilterIIRBQ16_cascade8(int16_t Xin, PARM_EQUAL_FILTER *pFilter_Array);

     int16_t DSP_FilterIIRBQ16_cascade8_fast(int16_t Xin, PARM_EQUAL_FILTER_16 *pFilter_Array);

     int16_t DSP_FilterIIRBQ16_parallel8(int16_t Xin, PARM_EQUAL_FILTER *pFilter);

     int16_t DSP_FilterIIRBQ16_parallel8_fast(int16_t Xin, PARM_EQUAL_FILTER_16 *pFilter);

   Matrix Functions:

     void DSP_MatrixAdd32(matrix32 *resMat, matrix32 *srcMat1, matrix32 *srcMat2);

     void DSP_MatrixEqual32(matrix32 *resMat, matrix32 *srcMat);
   
     void DSP_MatrixInit32(int32_t *data_buffer, int32_t N, int32_t num);
   
     void DSP_MatrixMul32(matrix32 *resMat, matrix32 *srcMat1, matrix32 *srcMat2);
   
     void DSP_MatrixScale32(int32_t *data_buffer, int32_t N, int32_t num);
   
     void DSP_MatrixSub32(matrix32 *resMat, matrix32 *srcMat1, matrix32 *srcMat2);
   
     void DSP_MatrixTranspose32(matrix32 *desMat, matrix32 *srcMat);

   Transforms:

     void DSP_TransformFFT16(int16c *dout, int16c *din, int16c *twiddles, 
                                        int16c *scratch, int log2N);

     void DSP_TransformIFFT16(int16c *dout, int16c *din, int16c *twiddles,
                                        int16c *scratch, int log2N);

     void DSP_TransformFFT16_setup(int16c *twiddles, int log2N);

     void DSP_TransformFFT32(int32c *dout, int32c *din, int32c *twiddles,
                                        int32c *scratch, int log2N);

     void DSP_TransformFFT32_setup(int32c *twiddles, int log2N);
   
     void DSP_TransformWindow_Bart16(int16_t *OutVector, int16_t *InVector, int N);
  
     void DSP_TransformWindow_Bart32(int32_t *OutVector, int32_t *InVector, int N);

     void DSP_TransformWindow_Black16(int16_t *OutVector, int16_t *InVector, int N);

     void DSP_TransformWindow_Black32(int32_t *OutVector, int32_t *InVector, int N);

     void DSP_TransformWindow_Cosine16(int16_t *OutVector, int16_t *InVector, int N);

     void DSP_TransformWindow_Cosine32(int32_t *OutVector, int32_t *InVector, int N);
  
     void DSP_TransformWindow_Hamm16(int16_t *OutVector, int16_t *InVector, int N);
  
     void DSP_TransformWindow_Hamm32(int32_t *OutVector, int32_t *InVector, int N);

     void DSP_TransformWindow_Hann16(int16_t *OutVector, int16_t *InVector, int N);

     void DSP_TransformWindow_Hann32(int32_t *OutVector, int32_t *InVector, int N);

     void DSP_TransformWindow_Kaiser16(int16_t *OutVector, int16_t *InVector, int N);

     void DSP_TransformWindow_Kaiser32(int32_t *OutVector, int32_t *InVector, int N);

     void DSP_TransformWinInit_Bart16(int16_t *OutWindow, int N);

     void DSP_TransformWinInit_Bart32(int32_t *OutWindow, int N);

     void DSP_TransformWinInit_Black16(int16_t *OutWindow, int N);

     void DSP_TransformWinInit_Black32(int32_t *OutWindow, int N);

     void DSP_TransformWinInit_Cosine16(int16_t *OutWindow, int N);

     void DSP_TransformWinInit_Cosine32(int32_t *OutWindow, int N);

     void DSP_TransformWinInit_Hamm16(int16_t *OutWindow, int N);

     void DSP_TransformWinInit_Hamm32(int32_t *OutWindow, int N);

     void DSP_TransformWinInit_Hann16(int16_t *OutWindow, int N);

     void DSP_TransformWinInit_Hann32(int32_t *OutWindow, int N);

     void DSP_TransformWinInit_Kaiser16(int16_t *OutWindow, int N);

     void DSP_TransformWinInit_Kaiser32(int32_t *OutWindow, int N);

   Vector Math:

     void DSP_VectorAbs16(int16_t *outdata, int16_t *indata, int N);

     void DSP_VectorAbs32(int32_t *outdata, int32_t *indata, int N);

     void DSP_VectorAdd16(int16_t *outdata, int16_t *indata1, int16_t *indata2, int N);

     void DSP_VectorAdd32(int32_t *outdata, int32_t *indata1, int32_t *indata2, int N);

     void DSP_VectorAddc16(int16_t *outdata, int16_t *indata, int16_t c, int N);

     void DSP_VectorAddc32(int32_t *outdata, int32_t *indata, int32_t c, int N);

     void  DSP_VectorAutocorr16(int16_t *outCorr, int16_t *inVector, int N, int K);
  
     int DSP_VectorBexp16(int16_t *DataIn, int N);

     int DSP_VectorBexp32(int32_t *DataIn, int N);
  
     int DSP_VectorChkEqu32(int32_t* indata1, int32_t *indata2, int N);

     void DSP_VectorCopy(int32_t *outdata, int32_t *indata, int N);
  
     void DSP_VectorCopyReverse32(int32_t *outdata, int32_t *indata, int N);

     void DSP_VectorDivC(_Q16 *outVector, _Q16 *inVector, _Q16 divisor, int N);

     int16_t DSP_VectorDotp16(int16_t *indata1, int16_t *indata2, int N, int scale);

     int32_t DSP_VectorDotp32(int32_t *indata1, int32_t *indata2, int N, int scale);

     void DSP_VectorExp(_Q16 *outVector, _Q16 *inVector, int N);
  
     void DSP_VectorFill(int32_t *indata, int32_t data, int N);

     void DSP_VectorLog10(_Q3_12 *outVector, _Q16 *inVector, int N);

     void DSP_VectorLog2(_Q5_10 *outVector, _Q16 *inVector, int N);

     void DSP_VectorLn(_Q4_11 *outVector, _Q16 *inVector, int N);

     int32_t DSP_VectorMax32(int32_t *indata, int N);

     int DSP_VectorMaxIndex32(int32_t *indata, int N);

     int32_t DSP_VectorMean32(int32_t *indata, int N);

     int32_t DSP_VectorMin32(int32_t *input, int N);

     int DSP_VectorMinIndex32(int32_t *indata, int N);

     void DSP_VectorMul16(int16_t *outdata, int16_t *indata1, int16_t *indata2, int N);

     void DSP_VectorMul32(int32_t *outdata, int32_t *indata1, int32_t *indata2, int N);

     void DSP_VectorMulc16(int16_t *outdata, int16_t *indata, int16_t c, int N);

     void DSP_VectorMulc32(int32_t *outdata, int32_t *indata, int32_t c, int N);

     void DSP_VectorNegate(int32_t *outdata, int32_t *indata, int N);

     void DSP_VectorRecip(_Q16 *outVector, _Q16 *inVector, int N);

     int16_t DSP_VectorRMS16(int16_t *inVector, int N);

     void DSP_VectorShift(int32_t *outdata, int32_t *indata, int N, int shift);

     int16_t DSP_VectorStdDev16(int16_t *inVector, int N);

     void DSP_VectorSqrt(_Q16 *outVector, _Q16 *inVector, int N);

     void DSP_VectorSub16(int16_t *outdata, int16_t *indata1, int16_t *indata2, int N);

     void DSP_VectorSub32(int32_t *outdata, int32_t *indata1, int32_t *indata2, int N);

     int16_t DSP_VectorSumSquares16(int16_t *indata, int N, int scale);

     int32_t DSP_VectorSumSquares32(int32_t *indata, int N, int scale);

     int16_t DSP_VectorVari16(int16_t *inVector, int N);

     int32_t DSP_VectorVariance(int32_t *inVector, int N);

     void DSP_VectorZeroPad(int32_t *indata, int N);
       
*******************************************************************************/

// DOM-IGNORE-BEGIN
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

/*
 * ****************************Begin Copyright 1D**********************************
 * Unpublished work (c) MIPS Technologies, Inc.  All rights reserved.
 * Unpublished rights reserved under the copyright laws of the United States
 * of America and other countries.
 *
 * This code is confidential and proprietary to MIPS Technologies, Inc. ("MIPS
 * Technologies") and  may be disclosed only as permitted in writing by MIPS
 * Technologies or an authorized third party.  Any copying, reproducing,
 * modifying, use or disclosure of this code (in whole or in part) that is not
 * expressly permitted in writing by MIPS Technologies or an authorized third
 * party is strictly prohibited.  At a minimum, this code is protected under
 * trade secret, unfair competition, and copyright laws.  Violations thereof
 * may result in criminal penalties and fines.
 *
 * MIPS Technologies reserves the right to change this code to improve function,
 * design or otherwise.  MIPS Technologies does not assume any liability arising
 * out of the application or use of this code, or of any error or omission in
 * such code.  Any warranties, whether express, statutory, implied or otherwise,
 * including but not limited to the implied warranties of merchantability or
 * fitness for a particular purpose, are excluded.  Except as expressly provided
 * in any written license agreement from MIPS Technologies or an authorized third
 * party, the furnishing of this code does not give recipient any license to any
 * intellectual property rights, including any patent rights, that cover this code.
 *
 * This code shall not be exported or transferred for the purpose of reexporting
 * in violation of any U.S. or non-U.S. regulation, treaty, Executive Order, law,
 * statute, amendment or supplement thereto.
 *
 * This code may only be disclosed to the United States government ("Government"),
 * or to Government users, with prior written consent from MIPS Technologies or an
 * authorized third party.  This code constitutes one or more of the following:
 * commercial computer software, commercial computer software documentation or
 * other commercial items.  If the user of this code, or any related documentation
 * of any kind, including related technical data or manuals, is an agency,
 * department, or other entity of the Government, the use, duplication,
 * reproduction, release, modification, disclosure, or transfer of this code, or
 * any related documentation of any kind, is restricted in accordance with Federal
 * Acquisition Regulation 12.212 for civilian agencies and Defense Federal
 * Acquisition Regulation Supplement 227.7202 for military agencies.  The use of
 * this code by the Government is further restricted in accordance with the terms
 * of the license agreement(s) and/or applicable contract terms and conditions
 * covering this code from MIPS Technologies or an authorized third party.
 * *******************************End Copyright************************************
 */
// DOM-IGNORE-END

#ifndef _DSP_H_      // Guards against multiple inclusion
#define _DSP_H_

//// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include "libq.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef struct              
{
	int16_t re;           // real portion (a)
	int16_t im;           // imaginary portion (b)
} int16c;                   // Q15 complex number (a + bi)

typedef struct              
{
	int32_t re;           // real portion (a)
	int32_t im;           // imaginary portion (b)
} int32c;                   // Q31 complex number (a + bi)

typedef struct              
{
	int16_t a1;           // feedback delay 1 coef
	int16_t a2;           // feedback delay 2 coef
	int16_t b1;           // feedforward delay 1 coef
	int16_t b2;           // feedforward delay 1 coef
} biquad16;                 // Q15 biquad

typedef struct              
{
	 int32_t row;         // matrix rows
	 int32_t col;         // matrix columns
	 int32_t *pMatrix;    // matrix pointer to data
}matrix32;                  // Q31 matrix

typedef struct
{
    int16_t  fracGain;		// Q15 fractional filter gain
    int16_t expGain;		// log2N (binary) filter gain
} PARM_FILTER_GAIN;				// filter gain structure

typedef struct _PARM_EQUAL_FILTER_16
{
    PARM_FILTER_GAIN G; // Filter max gain multiplier
    int16_t log2Alpha;  // coefficient scaling bit shift value
    int16_t b[3];       // Feedforward Coefficients, Q15 format
    int16_t a[2];       // Feedback Coefficients, Q15 format
    int16_t Z[2];       // Filter memory, should be initialized to zero.

} PARM_EQUAL_FILTER_16;		// IIR BQ filter structure Q15

typedef struct _PARM_EQUAL_FILTER
{
    PARM_FILTER_GAIN G; // Filter max gain multiplier
    int16_t log2Alpha;  // coefficient scaling bit shift value
    int16_t b[3];       // Feedforward Coefficients, Q15 format
    int16_t a[2];       // Feedback Coefficients, Q15 format
    int32_t Z[2];       // Filter memory, should be initialized to zero.

} PARM_EQUAL_FILTER;		// IIR BQ filter structure Q15 data, Q31 storage

typedef struct _PARM_EQUAL_FILTER_32
{
    PARM_FILTER_GAIN G; // Filter max gain multiplier
    int   log2Alpha;  // coefficient scaling bit shift value
    int32_t b[3];       // Feedforward Coefficients, Q15 format
    int32_t a[2];       // Feedback Coefficients, Q15 format
    int32_t Z[2];       // Filter memory, should be initialized to zero.

} PARM_EQUAL_FILTER_32;		// IIR BQ filter structure Q31

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

#define MIN16 ((int16_t) 0x8000)      // minimum Q15
#define MAX16 ((int16_t) 0x7FFF)      // maximum Q15

#define MIN32 ((int32_t) 0x80000000)  // minimum Q31
#define MAX32 ((int32_t) 0x7FFFFFFF)  // maximum Q31

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function DSP_ComplexAdd32:

        void DSP_ComplexAdd32(int32c *indata1, int32c *indata2, int32c *Output);

  Summary:
    Calculates the sum of two complex numbers.

  Description:
    Calculates the sum of two complex numbers, indata1 and indata2, and stores the
    complex result in Output. Complex numbers must be in the structural form that
    includes real and imaginary components. The function saturates the output values
    if maximum or minimum are exceeded. All values are in Q31 fractional data
    format.
            (a + bi) + (c + di) => (a + c) + (b + d)i

  Precondition:
    Complex numbers must be in the int32c format.

  Parameters:
    indata1    - pointer to input complex number (int32c)
    indata2    - pointer to input complex number (int32c)
    Output     - pointer to result complex numbers (int32c)

  Returns:
    None.

  Example:
    <code>

    int32c *res, result;
    int32c *input1, *input2;
    int32c test_complex_1 = {0x40000000,0x0CCCCCCC};
//                  (0.5 + 0.1i)
    int32c test_complex_2 = {0x73333333,0xB3333334};
//                  (0.9 - 0.6i)
    res=&result;
    input1=&test_complex_1;
    input2=&test_complex_2;

    DSP_ComplexAdd32(input1, input2, res);

// result = {0x73333333, 0xC0000000} = (0.9 - 0.5i)

   </code>

  Remarks:
    None.
*/

void DSP_ComplexAdd32(int32c *indata1, int32c *indata2, int32c *Output);

// *****************************************************************************
/* DSP_ComplexConj16:

      void  DSP_ComplexConj16(int16c *indata, int16c *Output);

  Summary:
      Calculates the complex conjugate of a complex number.

  Description:
    CCalculates the complex conjugate of Indata, and stores the result in Outdata.
    Both numbers must be in the complex number data structure which includes real
    and imaginary components. Values are in Q15 fractional data format. The
    function will saturate the output if maximum or minimum values are exceeded.
            (a + bi) => (a - bi)

  Precondition:
    Complex numbers must be in the int32c format.

  Parameters:
    indata     - pointer to input complex number (int16c)
    Output     - pointer to result complex numbers (int16c)

  Returns:
    None.

  Example:
    <code>

    int16c *res, result;
    int16c *input1;
    int16c test_complex_1 = {0x4000,0x0CCC};
//                  (0.5 + 0.1i)

    res=&result;
    input1=&test_complex_1;

    DSP_ComplexConj16(input1, res);

// result = {0x4000, 0xF334} = (0.5 - 0.1i)

   </code>

  Remarks:
    None.
*/

void  DSP_ComplexConj16(int16c *indata, int16c *Output);

// *****************************************************************************
/* Function DSP_ComplexConj32:

      void   DSP_ComplexConj32(int32c *indata, int32c *Output);

  Summary:
      Calculates the complex conjugate of a complex number.

  Description:
    Calculates the complex conjugate of indata, and stores the result in Output.
    Both numbers must be in the complex number data structure, which includes real
    and imaginary components. Values are in Q31 fractional data format. The
    function will saturate the output if maximum or minimum values are exceeded.
            (a + bi) => (a - bi)

  Precondition:
    Complex numbers must be in the int32c format.

  Parameters:
    indata1    - pointer to input complex number (int32c)
    Output     - pointer to result complex numbers (int32c)

  Returns:
    None.

  Example:
    <code>

    int32c *res, result;
    int32c *input1;
    int32c test_complex_1 = {0x40000000,0x0CCCCCCC};
//                  (0.5 + 0.1i)

    res=&result;
    input1=&test_complex_1;

    DSP_ComplexConj32(input1, res);

// result = {0x40000000, 0xF3333334} = (0.5 - 0.1i)

   </code>

  Remarks:
    None.
*/

void DSP_ComplexConj32(int32c *indata, int32c *Output);

// *****************************************************************************
/* Function DSP_ComplexDotProd32:

      void DSP_ComplexDotProd32(int32c *indata1, int32c *indata2, int32c *Output);

  Summary:
      Calculates the dot product of two complex numbers.

  Description:
    Calculates the dot product of two complex numbers, indata1 and indata2, and
    stores the result in Output. All numbers must be in complex structural
    format that includes real and imaginary components, and the numbers are in
    fractional Q31 format. The function will saturate the output if it exceeds
    maximum or minimum ratings. The formula for the dot product is as follows:
    Output(real) = (Input1.re * Input2.re) + (Input1.im * Input2.im);
    Output(img) = [(Input1.re * Input2.im) - (Input1.im * Input2.re)]i
            (a + bi) dot (c + di) => (a * c + b * d) + (a * d - b * c)i

  Precondition:
    Complex numbers must be in the int32c format.

  Parameters:
    indata1    - pointer to input complex number (int32c)
    indata2    - pointer to input complex number (int32c)
    Output     - pointer to result complex numbers (int32c)

  Returns:
    None.

  Example:
    <code>

    int32c *res, result;
    int32c *input1, *input2;
    int32c test_complex_1 = {0x40000000,0x0CCCCCCC};
//                  (0.5 + 0.1i)
    int32c test_complex_2 = {0x73333333,0xB3333334};
//                  (0.9 - 0.6i)
    res=&result;
    input1=&test_complex_1;
    input2=&test_complex_2;

    DSP_ComplexDotProd32(input1, input2, res);

// result = {0x31EB851E, 0xCE147AE3} = (0.39 - 0.39i)

   </code>

  Remarks:
    None.
*/

void DSP_ComplexDotProd32(int32c *indata1, int32c *indata2, int32c *Output);

// *****************************************************************************
/* Function DSP_ComplexMult32:

      void DSP_ComplexMult32(int32c *indata1, int32c *indata2, int32c *Output);

  Summary:
      Multiplies two complex numbers.

  Description:
    Multiplies two complex numbers, indata1 and indata2, and stores the complex
    result in Output. All numbers must be in the int32c complex data structure. All
    data is in Q31 fractional format. The function will saturate if maximum or
    minimum values are exceeded.
    Output(real) = (Input1.re * Input2.re) - (Input1.im * Input2.im);
    Output(img) = [(Input1.re * Input2.im) + (Input1.im * Input2.re)]i
            (a + bi) x (c + di) => (a * c - b * d) + (a * d + b * c)i

  Precondition:
    Complex numbers must be in the int32c format.

  Parameters:
    indata1    - pointer to input complex number (int32c)
    indata2    - pointer to input complex number (int32c)
    Output     - pointer to result complex numbers (int32c)

  Returns:
    None.

  Example:
    <code>

    int32c *res, result;
    int32c *input1, *input2;
    int32c test_complex_1 = {0x40000000,0x0CCCCCCC};
//                  (0.5 + 0.1i)
    int32c test_complex_2 = {0x73333333,0xB3333334};
//                  (0.9 - 0.6i)
    res=&result;
    input1=&test_complex_1;
    input2=&test_complex_2;

    DSP_ComplexMult32(input1, input2, res);

// result = {0x4147AE14, 0xE51EB8551} = (0.51 - 0.21i)

   </code>

  Remarks:
    None.
*/

void DSP_ComplexMult32(int32c *indata1, int32c *indata2, int32c *Output);

// *****************************************************************************
/* Function DSP_ComplexScalarMult32:

      void DSP_ComplexScalarMult32(int32c *indata, int32_t Scalar, int32c *Output);

  Summary:
      Multiplies a complex number and a scalar number.

  Description:
   Multiplies a complex number, indata, by a scalar number, Scalar, and stores
    the result in Output. indata and Output must be in int32c structure with
    real and imaginary components. All data must be in the fractional Q31 format.
    The function will saturate if maximum or minimum values are exceeded.
        Output(real) = (Input1.re * Scalar);
        Output(img) = [(Input1.im * Scalar)]i
            (a + bi) * C => (a * C + b * Ci)

  Precondition:
    Complex numbers must be in the int32c format.

  Parameters:
    indata     - pointer to input complex number (int32c)
    Scalar     - fractional scalar input value (int32_t)
    Output     - pointer to result complex numbers (int32c)

  Returns:
    None.

  Example:
    <code>

    int32c *res, result;
    int32c *input1;
    int32_t  scalarInput = 0x20000000; // 0.25
    int32c test_complex_1 = {0x40000000,0x0CCCCCCC};
//                  (0.5 + 0.1i)

    res=&result;
    input1=&test_complex_1;

    DSP_ComplexScalarMult32(input1, scalarInput, res);

// result = {0x10000000, 0x03333333} = (0.125 + 0.025i)

   </code>

  Remarks:
    None.
*/

void DSP_ComplexScalarMult32(int32c *indata, int32_t Scalar, int32c *Output);

// *****************************************************************************
/* Function DSP_ComplexSub32:

       void  DSP_ComplexSub32(int32c *indata1, int32c *indata2, int32c *Output);

  Summary:
      Calculates the difference of two complex numbers.

  Description:
    Calculates the difference of two complex numbers, indata1 less indata2, and
    stores the complex result in Output.  Both numbers must be in a complex data
    structure, which includes real and imaginary components.  The function saturates
    the output values if maximum or minimum are exceeded. Real and imaginary
    components are in the Q31 fractional data format.
            (a + bi) - (c + di) => (a - c) + (b - d)i

  Precondition:
    Complex numbers must be in the int32c format.

  Parameters:
    indata1    - pointer to input complex number (int32c)
    indata2    - pointer to input complex number (int32c)
    Output     - pointer to result complex numbers (int32c)

  Returns:
    None.

  Example:
    <code>

    int32c *res, result;
    int32c *input1, *input2;
    int32c test_complex_1 = {0x40000000,0x0CCCCCCC};
//                  (0.5 + 0.1i)
    int32c test_complex_2 = {0x73333333,0xB3333334};
//                  (0.9 - 0.6i)
    res=&result;
    input1=&test_complex_1;
    input2=&test_complex_2;

    DSP_ComplexSub32(input1, input2, res);

// result = {0xCCCCCCCD, 0x59999998} = (-0.4 + 0.7i)

   </code>

  Remarks:
    None.
*/

void DSP_ComplexSub32(int32c *indata1, int32c *indata2, int32c *Output);

// *****************************************************************************
/* Function DSP_FilterFIR32:

    void DSP_FilterFIR32(int32_t *outdata, int32_t *indata, int32_t *coeffs2x,
               int32_t *delayline, int N, int K, int scale);

  Summary:
    Performs a Finite Infinite Response (FIR) filter on a vector.

  Description:
    Performs an FIR filter on the vector indata, and stores the output in the
    vector outdata.  The number of samples processed in the array is given by N.
    The number of filter taps is given by K.  The values are scaled upon input
    by the binary scaling factor (right shift), scale.  The array of 2*K coefficients
    is contained in the array coeffs2x, where the values are in order b0, b1, b2...
    and repeated.  Lastly the delayline is an array of K values that are
    initialized to zero and represent previous values.  All values are in
    fractional Q31 data format.  The function will saturate results if minimum
    or maximum values are exceeded.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.
    K must be greater than 2 and a multiple of 2.
    delayline must have K elements, and be initialized to zero.
    coeffs2x must have 2*K elements.

  Parameters:
    outdata     - pointer to destination array of elements (int32_t)
    indata      - pointer to source array of elements (int32_t)
    coeffs2x    - pointer to an array of coefficients (int32_t)
    delayline   - pointer to an array of delay variables (int32_t)
    N           - number of points in the array to process (int)
    K           - number of filter taps
    scale       - binary scaler divisor (1 / 2^scale) (int)
    N           - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    #define TAPS 4
    #define numPOINTS 256

    int filterN = numPOINTS;
    int filterK = TAPS;
    int filterScale = 1;  // scale output by 1/2^1 => output * 0.5
    int32_t FilterCoefs[TAPS*2] = {0x40000000, 0x20000000, 0x20000000, 0x20000000,
                0x40000000, 0x20000000, 0x20000000, 0x20000000};
 // note repeated filter coefs, A B C D A B C D
 //             0.5, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 0.25

    int32_t outFilterData[numPOINTS]={0};
    int32_t inFilterData[numPOINTS];
    int filterDelayLine[TAPS]={0};

    while(true)
    {

    //   put some data into input array, inFilterData, here //

    DSP_FilterFIR32(outFilterData, inFilterData, FilterCoefs, filterDelayLine,
				filterN, filterK, filterScale);
    }

   </code>

  Remarks:
    Filter coefs must be repeated within the array. The array is twice as large
    as the number of taps, and the values are repeated in order b0, b1, b2,...bn,
    b0, b1, b2,... bn.
    The function updates the delayline array, which must be K elements long. The
    array should be initialized to zero prior to the first processing. It will
    contain values for processing cascaded filters within a loop.
*/

void DSP_FilterFIR32(int32_t *outdata, int32_t *indata, int32_t *coeffs2x, int32_t *delayline,
				int N, int K, int scale);

// *****************************************************************************
/* Function DSP_FilterIIR16:

    int16_t   DSP_FilterIIR16(int16_t in, int16_t *coeffs, int16_t *delayline,
                           int B, int scale);

  Summary:
    Performs a single-sample cascaded biquad Infinite Impulse Response (IIR) filter.

  Description:
    Performs a single element cascaded biquad IIR filter on the input, in.  The
    filter contains B number of biquad sections, and cascades the output of one
    to the input of the next.  B must be greater than 2 and a multiple of 2. The
    int16_t output generated by the function is the computation from the final
    biquad stage.  Delay pipeline array delayline must contain 2*B values and be
    initialized to zero prior to use.  The coefficient array must contain 4*B
    elements, and must be set up in order of biquad a1, a2, b1, b2. A binary
    (right shift) factor, scale, will scale the output equivalent to (1/2^scale).
    All numerical values must be in Q15 fractional data format. The function
    will saturate values if maximum or minimum values are exceeded.

         Y = X0 + (b1 * X(-1)) + (b2 * X(-2) + (a1 * Y(-1)) + (a2 * Y(-2))

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    B must be greater than 2 and a multiple of 2.
    delayline must have 2*B elements, and be initialized to zero.
    coeffs must have 4*B elements.

  Parameters:
    in          - input data element X (int16_t)
    coeffs      - pointer to an array of coefficients (int16_t)
    delayline   - pointer to an array of delay variables (int16_t)
    B           - number of cascaded biquad filter groups to process (int)
    scale       - binary scaler divisor (1 / 2^scale) (int)

  Returns:
    Sample output Y (int16_t)

  Example:
    <code>

    #define B 8         // use * biquad filters in cascade

    int dataSamples = 256;
    int i, j;
    biquad16 bquad[B];
    int16_t coefs[4*B]= {0};
    int16_t delaylines[2*B]= {0};
    int16_t Y, X;
    int scaleBquad = 1;  // scale output (1 /2^n) => Y * 0.5

    // do something to set up coefs, for instance this example //

    for (j=0; j<B; j++)
    {
        bquad[j].a1 = 0x0CCC;
        bquad[j].a2 = 0x4000;
        bquad[j].b1 = 0xC000;
        bquad[j].b2 = 0x2000;
    }

    DSP_FilterIIRSetup16(coefs, bquad, B);  // coef array setup

    for (i=0; i < dataSamples; i++)
    {
    //   get some data for input, X = inputBuffer[i], here //

    Y = DSP_FilterIIR16(X, coefs, delaylines, B, scaleBquad);

    // do something with the output data, Y, here //
    }

   </code>

  Remarks:
    Filter coefs must be stored within the array as a1, a2, b1, b2, a1, a2, b1,
    b2, in order of biquads form input to output. A function to translate the
    coeffs from biquad structure to coeffs is available in DSP_FilterIIRSetup16.
    The function updates the delayline array, which must be 2*B elements long. The
    array should be initialized to zero prior to the first processing. It will
    contain values for processing cascaded filters within a loop.
*/

 int16_t DSP_FilterIIR16(int16_t in, int16_t *coeffs, int16_t *delayline, int B, int scale);

 // *****************************************************************************
/* Function DSP_FilterIIRSetup16:

    void   DSP_FilterIIRSetup16(int16_t *coeffs, biquad16 *bq, int B);

  Summary:
    Converts biquad structure to coeffs array to set up IIR filter.

  Description:
    Converts an array of biquad coefficients, bq, into an linear array of
    coefficients, coeffs.  The output array must be 4*B elements long. The
    number of biquads in the resulting factor is given by B. All numerical
    values must be in Q15 fractional data format.

  Precondition:
    coeffs must have 4*B elements.

  Parameters:
    coeffs      - pointer to an array of coefficients (int16_t)
    bq          - pointer to array of biquad structure filter coefs (biquad16)
    B           - number of cascaded biquad filter groups to process (int)

  Returns:
    None.

  Example:
    <code>

    see DSP_FilterIIR16 for example.

   </code>

  Remarks:
    None.
*/

void DSP_FilterIIRSetup16(int16_t *coeffs, biquad16 *bq, int B);

// *****************************************************************************
/* Function DSP_FilterFIRDecim32:

    void DSP_FilterFIRDecim32(int32_t *outdata, int32_t *indata, int32_t *coeffs,
                int32_t *delayline, int N, int K, int scale, int rate);

  Summary:
    Performs a decimating FIR filter on the input array.

  Description:
    Compute a FIR decimation filter on the input vector indata, and store the
    results to the vector outdata.  The total number of output elements is set
    by N, and therefore the outdata array must be at least N in length.  The
    decimation ratio is given by rate.  The input is sampled every integer value
    of rate, skipping every (rate-1) input samples.  The input array must therefore
    be (rate*N) samples long.  The amount of filter taps is specified by K.
    Coeffs specifies the coefficients array.  The delayline array holds delay
    inputs for calculation, and must be initialized to zero prior to calling the
    filter.  Both coeffs and delayline must be K in length.  Scale divides the
    input by a scaling factor by right shifting the number of bits (1/2^scale).
    All values of input, output, and coeffs are given in Q31 fractional data
    format.  The function will saturate if the output value exceeds the maximum
    or minimum value.

         Y = b0 * X0 + (b1 * X(-1)) + (b2 * X(-2)

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    delayline must have K elements, and be initialized to zero.
    coeffs must have K elements.
    outdata must have N elements
    indata must have (N*rate) elements

  Parameters:
    outdata     - pointer to output array of elements (int32_t)
    indata      - pointer to input array of elements (int32_t)
    coeffs      - pointer to an array of coefficients (int32_t)
    delayline   - pointer to an array of delay variables (int32_t)
    N           - number of output elements to be processed (int)
    K           - number of filter taps and coeffs (int)
    scale       - binary scaler divisor (1 / 2^scale) (int)
    rate        - decimation ratio (int)

  Returns:
    None.

  Example:
    <code>

 #define N 8         // number of output samples
 #define TAPS 5
 #define SKIP 3

    int testFilterN = N;        // number of output elements
    int testFilterK = TAPS;     // number of taps
    int testFilterRate = SKIP;  // decimation rate R
    int32_t outFiltDataDec[N]={0};
    int32_t *inTestFilter[N*SKIP];
    int filtScaleNum = 1;       // scale output (1 /2^n) => Y * 0.5

    int32_t filtDelayTest[8]={0};  // always initialize to zero

    // get pointer to input buffer here //
    inTestFilter = &inputBuffer;

    DSP_FilterFIRDecim32(outFiltDataDec, inTestFilter, inTestCoefs,
            filtDelayTest, testFilterN, testFilterK, filtScaleNum, testFilterRate);

   </code>

  Remarks:
    Coefs are loaded into the array with corresponding to the least delay first
        (C0, C(-1), C(-2)).
    K must be greater than rate.
    Even while decimating the input stream, every input passes through the
    delayline.  So FIR filters of arbitrary length will give the same output as
    a non-decimating FIR, just with fewer responses.
*/

void DSP_FilterFIRDecim32(int32_t *outdata, int32_t *indata, int32_t *coeffs, int32_t *delayline,
				int N, int K, int scale, int rate);

// *****************************************************************************
/* Function DSP_FilterFIRInterp32:

    void DSP_FilterFIRInterp32(int32_t *outdata, int32_t *indata, int32_t *coeffs,
            int32_t *delayline, int N, int K, int scale, int rate);

  Summary:
    Performs an interpolating FIR filter on the input array.

  Description:
    Perform an interpolating FIR filter on the first N samples of indata, and
    stores the result in outdata. The number of output elements is N*rate. The
    number of filter taps, K, must be an even multiple of N. The coefficients
    array, Coeffs, must be K elements long. The delay line array, delayline,
    must be K/R elements long, and be initialized to zero. All data elements must
    be in Q31 fractional data format. Scaling is performed via binary shift on
    the input equivalent to (1/2^shift). The function will saturate the output if
    it exceeds maximum or minimum values.
    The function creates R output values for each input value processed. The
    delayline of previous values is processed with R elements of the coefficient
    array. Numerically:

    Y(1,0) = X(0)*C(0) + X(-1)*C(rate) + X(-2)*C(2*rate) ...
    Y(1,1) = X(0)*C(1) + X(-1)*C(rate+1) + X(-2)*C(2*rate + 1) ...
    Y(1,rate) = X(0)*C(N) + X(-1)*C(rate+N) + X(-2)*C(2*rate + N) ...

    where output Y corresponds to (input,rate) different outputs, input X has
    (M/rate) sample delays and C is the coefficient array.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    delayline must have (K/R) elements, and be initialized to zero.
    K (taps) must be an even multiple of R (rate).
    outdata must have R*N elements.

  Parameters:
    outdata     - pointer to output array of elements (int32_t)
    indata      - pointer to input array of elements (int32_t)
    coeffs      - pointer to an array of coefficients (int32_t)
    delayline   - pointer to an array of delay variables (int32_t)
    N           - number of output elements to be processed (int)
    K           - number of filter taps and coeffs (int)
    scale       - binary scaler divisor (1 / 2^scale) (int)
    rate        - decimation ratio (int)

  Returns:
    None.

  Example:
    <code>
// interpret evenly 1/3 spaced values
 #define N 4         // number of output samples
 #define TAPS 6
 #define INTERP 3

    int ifiltN = N;
    int ifiltK = TAPS;  // k must be an even multiple of R
    int ifiltR = INTERP;

    int32_t ifiltOut[N*INTERP]={0};
    int32_t ifiltDelay[2]={0};  // must be initialized to zero
    int ifiltScale = 0;         // no scaling

    int32_t ifiltCoefsThirds[TAPS]={0x2AAAAAA9, 0x55555555,0x7FFFFFFE,
                                    0x55555555,0x2AAAAAA9,0x00000000};
//                  0.333333, 0.6666667, 0.99999999, 0.6666667, 0.33333333, 0

    int32_t ifiltInput[N]={0x0CCCCCCD, 0x19999999, 0x26666666, 0x33333333};
//                  0.1,  0.2,  0.3,  0.4

 DSP_FilterFIRInterp32(ifiltOut, ifiltInput, ifiltCoefsThirds, ifiltDelay,
                                    ifiltN, ifiltK, ifiltScale, ifiltR);

// ifiltOut = {0x04444444, 0x08888889, 0x0CCCCCCD, 0x11111111, 0x15555555, 0x19999999,
//          0x1DDDDDDD, 0x22222221, 0x26666665, 0x2AAAAAAA,0x2EEEEEEE, 0x33333332}
// = 0.0333, 0.0667, 0.1, 0.1333, 0.1667, 0.2, 0.2333, 0.2667, 0.3, 0.3333, 0.3667, 0.4

   </code>

  Remarks:
    The function processes each input (rate) times. With each pass, coefficients
    are offset so that (K/rate) multiply accumulate cycles occur.
*/

void DSP_FilterFIRInterp32(int32_t *outdata, int32_t *indata, int32_t *coeffs, int32_t *delayline,
				int N, int K, int scale, int rate);

// *****************************************************************************
/* Function DSP_FilterLMS16:

    int16_t DSP_FilterLMS16(int16_t in, int16_t ref, int16_t *coeffs, int16_t *delayline,
                                        int16_t *error, int K, int16_t mu);

  Summary:
    Performs a single sample Least Mean Squares FIR Filter.

  Description:
    Computes an LMS adaptive filter on the input in. Filter output of the FIR is
    given as a 16 bit value. The filter target is ref, and the calculation
    difference between the output and the target is error. The filter adapts its
    coefficients, coefs, on each pass. The number of coefficients (filter taps)
    is given by the value K. The delayline array should be initialized to zero
    prior to calling the filter for the first time, and have K elements. The
    value mu is the rate at which the filter adapts. All values are Q15 fractional
    numbers. The function will saturate the output if it exceeds maximum or
    minimum values.
    The LMS will adapt its coefs to attempt to drive the output value toward the
    ref value. The rate of adaption on each pass depends on mu and the error
    from the previous calculation.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    delayline must have (K) elements, and be initialized to zero.
    K (taps) must be a multiple of 4, and >= 8.
    mu must be positive.

  Parameters:
    in          - input data value (int16_t)
    ref         - target output value (int16_t)
    coeffs       - pointer to an array of coefficients (int16_t)
    delayline   - pointer to an array of delay variables (int16_t)
    error       - output minus reference (int16_t)
    K           - number of filter taps and coeffs (int)
    mu          - adaption rate (int16_t)

  Returns:
    (int16_t) - FIR filter output

  Example:
    <code>
#define lmsTAPS 8

 int16_t lmsOut;
 int lmsTaps = lmsTAPS;

 int16_t lmsCoefs[lmsTAPS]={0x5000, 0x4000,0x3000, 0x2000,0x1000, 0x0000,0xF000, 0xE000};
 int16_t lmsDelay[lmsTAPS]={0};
 int16_t *ptrLMSError;
 int16_t lmsError = 0x0200;
 int16_t inVal=0;

 int16_t refVal = 0x0CCC;  // some target value = 0.1
 int16_t lmsAdapt = 0x3000;

 ptrLMSError = &lmsError;

 for (i=0;i<200;i++)
 {
    // get some input value here //
     if (i < 100)
     {
         inVal = 0x4233;
     }
     else
     {
         inVal = 0xCF10;
     }

     lmsOut =  DSP_FilterLMS16(inVal, refVal, lmsCoefs, lmsDelay,
		   ptrLMSError, lmsTaps, lmsAdapt);
 }
   </code>

  Remarks:
    Filter coefs may start at random or zero value, but convergence is dependent
    on the amount of update required.
*/

int16_t DSP_FilterLMS16(int16_t in, int16_t ref, int16_t *coeffs, int16_t *delayline,
                                        int16_t *error, int K, int16_t mu);

// *****************************************************************************
/* Function DSP_FilterIIRBQ16_fast:

    int16_t   DSP_FilterIIRBQ16_fast(int16_t Xin, PARM_EQUAL_FILTER_16 *pFilter);

  Summary:
    Performs a single-pass IIR Biquad Filter.

  Description:
    Calculates a single pass IIR biquad filter on Xin, and delivers the result
    as a 16-bit output.  All math is performed using 16 bit instructions, with
    results rounded to 16-bits for the output.  The delay register is stored
    as a 16-bit value for subsequent functions.  The function will saturate the
    results if maximum or minimum fractional values are exceeded. All values are
    fractional Q15 format.

         Y = X(0)*b0 + (b1 * X(-1)) + (b2 * X(-2)) - (a1 * Y(-1)) - (a2 * Y(-2))

  Precondition:
    Delay register values should be initialized to zero.

  Parameters:
    Xin         - input data element X (int16_t)
    pFilter     - pointer to filter coef and delay structure

  Returns:
    Sample output Y (int16_t)

  Example:
    <code>

    PARM_EQUAL_FILTER_16 *ptrFilterEQ;
    PARM_EQUAL_FILTER_16 FilterEQ;
    uint16_t DataIn, DataOut;
    ptrFilterEQ = &FilterEQ;

 // 48KHz sampling; 1 KHz bandpass filter; Q=0.9
 //  divide by 2 and convert to Q15
 //   b0 = 0.06761171785499065
 //   b1 = 0
 //   b2 = -0.06761171785499065
 //   a1 = -1.848823142275648
 //   a2 = 0.8647765642900187

    // note all coefs are half value of original design, gain handled in algorithm
    ptrFiltEQ32->b[0]=0x0453;       // feed forward b0 coef
    ptrFiltEQ32->b[1]=0;            // feed forward b1 coef
    ptrFiltEQ32->b[2]=0xFBAD;       // feed forward b2 coef

    // note all coefs are half value of original design, gain handled in algorithm
    // note subtract is handled in algorithm, so coefs go in at actual value
    ptrFiltEQ32->a[0]=0x89AD;       // feedback a1 coef
    ptrFiltEQ32->a[1]=0x3758;       // feedback a2 coef

    for (i=0;i<256;i++)
        {
        // *** get some input data here
           DataIn32 = three_hundred_hz[i];

           DataOut = DSP_FilterIIRBQ16_fast(DataIn, ptrFilterEQ);

         // *** do something with the DataOut here
         }
   </code>

  Remarks:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.
    This function is optimized with microAptiv core instructions.
*/

int16_t DSP_FilterIIRBQ16_fast(int16_t Xin, PARM_EQUAL_FILTER_16 *pFilter);

// *****************************************************************************
/* Function DSP_FilterIIRBQ16:

    int16_t   DSP_FilterIIRBQ16(int16_t Xin, PARM_EQUAL_FILTER *pFilter);

  Summary:
    Performs a single-pass IIR Biquad Filter.

  Description:
    Calculates a single pass IIR biquad filter on Xin, and delivers the result
    as a 16-bit output.  All math is performed using 32 bit instructions, with
    results truncated to 16-bits for the output.  The delay register is stored
    as a 32-bit value for subsequent functions.
    All values are fractional Q15 and Q31, see data structure for specifics.

         Y = X(0)*b0 + (b1 * X(-1)) + (b2 * X(-2)) - (a1 * Y(-1)) - (a2 * Y(-2))

  Precondition:
    Delay register values should be initialized to zero.

  Parameters:
    Xin         - input data element X (int16_t)
    pFilter     - pointer to filter coef and delay structure

  Returns:
    Sample output Y (int16_t)

  Example:
    <code>

    PARM_EQUAL_FILTER *ptrFilterEQ;
    PARM_EQUAL_FILTER FilterEQ;
    uint16_t DataIn, DataOut;
    ptrFilterEQ = &FilterEQ;

 // 48KHz sampling; 1 KHz bandpass filter; Q=0.9
 //  divide by 2 and convert to Q15
 //   b0 = 0.06761171785499065
 //   b1 = 0
 //   b2 = -0.06761171785499065
 //   a1 = -1.848823142275648
 //   a2 = 0.8647765642900187

    // note all coefs are half value of original design, gain handled in algorithm
    ptrFiltEQ32->b[0]=0x0453;       // feed forward b0 coef
    ptrFiltEQ32->b[1]=0;            // feed forward b1 coef
    ptrFiltEQ32->b[2]=0xFBAD;       // feed forward b2 coef

    // note all coefs are half value of original design, gain handled in algorithm
    // note subtract is handled in algorithm, so coefs go in at actual value
    ptrFiltEQ32->a[0]=0x89AD;       // feedback a1 coef
    ptrFiltEQ32->a[1]=0x3758;       // feedback a2 coef

    for (i=0;i<256;i++)
        {
        // *** get some input data here
           DataIn32 = three_hundred_hz[i];

           DataOut = DSP_FilterIIRBQ16(DataIn, ptrFilterEQ);

         // *** do something with the DataOut here
         }
   </code>

  Remarks:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.

*/

int16_t DSP_FilterIIRBQ16(int16_t Xin, PARM_EQUAL_FILTER *pFilter);

// *****************************************************************************
/* Function DSP_FilterIIRBQ32:

    int32_t   DSP_FilterIIRBQ32(int32_t Xin, PARM_EQUAL_FILTER_32 *pFilter);

  Summary:
    Performs a high resolution single-pass IIR Biquad Filter.

  Description:
    Calculates a single pass IIR biquad filter on Xin, and delivers the result
    as a 16-bit output.  All math is performed using 32 bit instructions, with
    results truncated to 32-bits for the output.  The delay register is stored
    as a 32-bit value for subsequent functions.
    All values are fractional Q31, see data structure for specifics.

         Y = X(0)*b0 + (b1 * X(-1)) + (b2 * X(-2)) - (a1 * Y(-1)) - (a2 * Y(-2))

  Precondition:
    Delay register values should be initialized to zero.

  Parameters:
    Xin         - input data element X (int32_t)
    pFilter     - pointer to high resolution filter coef and delay structure

  Returns:
    Sample output Y (int32_t)

  Example:
    <code>

    PARM_EQUAL_FILTER_32 *ptrFiltEQ32;
    PARM_EQUAL_FILTER_32 FilterEQ32;
    int32_t DataIn32, DataOut32;

    ptrFiltEQ32 = &FilterEQ32;

    ptrFiltEQ32->Z[0]=0;
    ptrFiltEQ32->Z[1]=0;

    //  1000 Hz Q= 0.9 BP filter design, 44.1K sampling
    //
    //    b0 = 0.07311778239751009  forward
    //    b1 = 0
    //    b2 = -0.07311778239751009
    //    a1 = -1.8349811166056893  back
    //    a2 = 0.8537644352049799

    // note all coefs are half value of original design, gain handled in algorithm
    ptrFiltEQ32->b[0]=0x04ADF635;   // feed forward b0 coef
    ptrFiltEQ32->b[1]=0;            // feed forward b1 coef
    ptrFiltEQ32->b[2]=0xFB5209CB;   // feed forward b2 coef

    // note all coefs are half value of original design, gain handled in algorithm
    // note subtract is handled in algorithm, so coefs go in at actual value
    ptrFiltEQ32->a[0]=0x8A8FAB5D;  // feedback a1 coef
    ptrFiltEQ32->a[1]=0x36A41395;  // feedback a2 coef

    for (i=0;i<256;i++)
        {
         // *** get input data here
           DataIn32 = three_hundred_hz[i];

           DataOut32 = DSP_FilterIIRBQ32(DataIn32, ptrFiltEQ32);

         // *** do something with the DataOut32 here
         }
   </code>

  Remarks:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.
    This function is optimized with microAptiv core instructions.
*/

int32_t DSP_FilterIIRBQ32(int32_t Xin, PARM_EQUAL_FILTER_32 *pFilter);

// *****************************************************************************
/* Function DSP_FilterIIRBQ16_cascade8:

    int16_t   DSP_FilterIIRBQ16_cascade8(int16_t Xin,
                                       PARM_EQUAL_FILTER *pFilter_Array);

  Summary:
    Performs a single-sample IIR Biquad Filter as a cascade of 8 series filters.

  Description:
    Calculates a single pass IIR biquad cascade filter on Xin, and delivers the
    result as a 16-bit output.  The cascade of filters is 8 unique biquad filters
    arranged in series such that the output of one is provided as the input to
    the next.  A unique filter coefficient set is provided to each, and 32 bit
    delay lines are maintained for each.  All math is performed using 32 bit
    instructions, which results truncated to 16-bits for the output.
    Global gain values are available on the output. Fracgain is a Q15 fractional
    gain value and expgain is a binary shift gain value.  The combination of the
    two can be utilized to normalize the output as desired.
    All values are fractional Q15 and Q31, see data structure for specifics.

         Y = Y7 <- Y6 <- Y5 <- Y4 <- Y3 <- Y2 <- Y1 <- Y0
    where each Yn filter element represents a unique IIR biquad:
       Yn = Y(n-1)*b0 + (b1 * Y(n-2)) + (b2 * Y(n-3)) - (a1 * Yn(-1)) - (a2 * Yn(-2))
    and:
       for Y0; Y(n-1) = Xin(0)

  Precondition:
    Delay register values should be initialized to zero.

  Parameters:
    Xin         - input data element X (int16_t)
    pFilter     - pointer to filter coef and delay structure

  Returns:
    Sample output Y (int16_t)

  Example:
    <code>

    PARM_EQUAL_FILTER filtArray[8];
    uint16_t dataY, dataX;

    // example to use 2 filter blocks as notch filters
    // fill entire Filter Array with coefs
    for (i=0;i<8;i++)
    {
        filtArray[i].Z[0]=0;
        filtArray[i].Z[1]=0;

        // note all coefs are half value of original design, gain handled in algorithm
        // all pass
        filtArray[i].b[0]=0x4000;
        filtArray[i].b[1]=0;        // feed forward b1 coef
        filtArray[i].b[2]=0;  // feed forward b2 coef

        filtArray[i].a[0]=0;  // feedback a1 coef
        filtArray[i].a[1]=0;  // feedback a2 coef
    }
        // Unique filters for example
        // 10KHz notch filter -- divide coefs by 2
        b0 = 0.5883783602332997
        b1 = -0.17124071441396285
        b2 = 0.5883783602332997
        a1 = -0.17124071441396285
        a2 = 0.1767567204665992

        // note all coefs are half value of original design, gain handled in algorithm
        filtArray[3].b[0]=0x25a7;           // feed forward b0 coef
        filtArray[3].b[1]=0xf508;           // feed forward b1 coef
        filtArray[3].b[2]=0x25a7;           // feed forward b2 coef

        // note all coefs are half value of original design, gain handled in algorithm
        // note subtract is handled in algorithm, so coefs go in at actual value
        filtArray[3].a[0]=0xf508;           // feedback a1 coef
        filtArray[3].a[1]=0x0b4f;           // feedback a2 coef

        // 1 KHz notch filter -- divide coefs by 2
        b0 = 0.9087554064944908
        b1 = -1.7990948352036205
        b2 = 0.9087554064944908
        a1 = -1.7990948352036205
        a2 = 0.8175108129889816

        // note all coefs are half value of original design, gain handled in algorithm
        filtArray[7].b[0]=0x3a29;           // feed forward b0 coef
        filtArray[7].b[1]=0x8cdc;           // feed forward b1 coef
        filtArray[7].b[2]=0x3a29;           // feed forward b2 coef

        // note all coefs are half value of original design, gain handled in algorithm
        // note subtract is handled in algorithm, so coefs go in at actual value
        filtArray[7].a[0]=0x8cdc;           // feedback a1 coef
        filtArray[7].a[1]=0x3452;           // feedback a2 coef

    for (i=0;i<256;i++)
        {
         // *** get input data here
         dataX = compound_300_1K_hz16[i];

         dataY = DSP_FilterIIRBQ16_cascade8(dataX, filtArray);

         // *** do something with the DataY here
         }
   </code>

  Remarks:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.

 */

int16_t DSP_FilterIIRBQ16_cascade8(int16_t Xin, PARM_EQUAL_FILTER *pFilter_Array);

// *****************************************************************************
/* Function DSP_FilterIIRBQ16_cascade8_fast:

    int16_t   DSP_FilterIIRBQ16_cascade8_fast(int16_t Xin,
                                       PARM_EQUAL_FILTER_16 *pFilter_Array);

  Summary:
    Performs a single-sample IIR Biquad Filter as a cascade of 8 series filters.

  Description:
    Calculates a single pass IIR biquad cascade filter on Xin, and delivers the
    result as a 16-bit output.  The cascade of filters is 8 unique biquad filters
    arranged in series such that the output of one is provided as the input to
    the next.  A unique filter coefficient set is provided to each, and 16 bit
    delay lines are maintained for each.  All math is performed using 16 bit
    instructions, which results rounded to 16-bits for the output.
    All values are fractional Q15, see data structure for specifics.  The function
    will saturate the output should it exceed maximum or minimum values.

         Y = Y7 <- Y6 <- Y5 <- Y4 <- Y3 <- Y2 <- Y1 <- Y0
    where each Yn filter element represents a unique IIR biquad:
       Yn = Y(n-1)*b0 + (b1 * Y(n-2)) + (b2 * Y(n-3)) - (a1 * Yn(-1)) - (a2 * Yn(-2))
    and:
       for Y0; Y(n-1) = Xin(0)

  Precondition:
    Delay register values should be initialized to zero.

  Parameters:
    Xin         - input data element X (int16_t)
    pFilter     - pointer to filter coef and delay structure

  Returns:
    Sample output Y (int16_t)

  Example:
    <code>

    PARM_EQUAL_FILTER_16 filtArray[8];
    uint16_t dataY, dataX;

    // example to use 2 filter blocks as notch filters
    // fill entire Filter Array with coefs
    for (i=0;i<8;i++)
    {
        filtArray[i].Z[0]=0;
        filtArray[i].Z[1]=0;

        // note all coefs are half value of original design, gain handled in algorithm
        // all pass
        filtArray[i].b[0]=0x4000;
        filtArray[i].b[1]=0;        // feed forward b1 coef
        filtArray[i].b[2]=0;  // feed forward b2 coef

        filtArray[i].a[0]=0;  // feedback a1 coef
        filtArray[i].a[1]=0;  // feedback a2 coef
    }
        // Unique filters for example
        // 10KHz notch filter -- divide coefs by 2
        b0 = 0.5883783602332997
        b1 = -0.17124071441396285
        b2 = 0.5883783602332997
        a1 = -0.17124071441396285
        a2 = 0.1767567204665992

        // note all coefs are half value of original design, gain handled in algorithm
        filtArray[3].b[0]=0x25a7;           // feed forward b0 coef
        filtArray[3].b[1]=0xf508;           // feed forward b1 coef
        filtArray[3].b[2]=0x25a7;           // feed forward b2 coef

        // note all coefs are half value of original design, gain handled in algorithm
        // note subtract is handled in algorithm, so coefs go in at actual value
        filtArray[3].a[0]=0xf508;           // feedback a1 coef
        filtArray[3].a[1]=0x0b4f;           // feedback a2 coef

        // 1 KHz notch filter -- divide coefs by 2
        b0 = 0.9087554064944908
        b1 = -1.7990948352036205
        b2 = 0.9087554064944908
        a1 = -1.7990948352036205
        a2 = 0.8175108129889816

        // note all coefs are half value of original design, gain handled in algorithm
        filtArray[7].b[0]=0x3a29;           // feed forward b0 coef
        filtArray[7].b[1]=0x8cdc;           // feed forward b1 coef
        filtArray[7].b[2]=0x3a29;           // feed forward b2 coef

        // note all coefs are half value of original design, gain handled in algorithm
        // note subtract is handled in algorithm, so coefs go in at actual value
        filtArray[7].a[0]=0x8cdc;           // feedback a1 coef
        filtArray[7].a[1]=0x3452;           // feedback a2 coef

    for (i=0;i<256;i++)
        {
         // *** get input data here
         dataX = compound_300_1K_hz16[i];

         dataY = DSP_FilterIIRBQ16_cascade8_fast(dataX, filtArray);

         // *** do something with the DataY here
         }
   </code>

  Remarks:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.
    This function is optimized with microAptiv core instructions.
 */

int16_t DSP_FilterIIRBQ16_cascade8_fast(int16_t Xin, PARM_EQUAL_FILTER_16 *pFilter_Array);

// *****************************************************************************
/* Function DSP_FilterIIRBQ16_parallel8:

    int16_t DSP_FilterIIRBQ16_parallel8(int16_t Xin, PARM_EQUAL_FILTER *pFilter);

  Summary:
    Performs a 8 parallel single-pass IIR Biquad Filters, and sums the result.

  Description:
    Calculates a 8 parallel, single pass IIR biquad filters on Xin, sums the result
    and delivers the result as a 16-bit output.  All math is performed using 32
    bit instructions, which results truncated to 16-bits for the output.  The
    delay register is stored as a 32-bit value for subsequent functions.
    Output is tuned by 2 multiplier factors.  First each parallel section has a
    fractional gain (attenuation) that enables individual scaling of that section.
    Second, a global binary (log2N) gain is applied to the result.  The combination
    of gain factors enable both gain and attenuation.
    All values are fractional Q15 and Q31, see data structure for specifics.

         Y = Y7/8 + Y6/8 + Y5/8 + Y4/8 + Y3/8 + Y2/8 + Y1/8 + Y0/8
    where each Yn filter element represents a unique IIR biquad:
       Yn = X(0)*b0 + (b1 * X(n-1)) + (b2 * X(n-2)) - (a1 * Yn(-1)) - (a2 * Yn(-2))

  Precondition:
    Delay register values should be initialized to zero.
    The sum of all fracgain should be <= 1

  Parameters:
    Xin         - input data element X (int16_t)
    pFilter     - pointer to filter coef and delay structure

  Returns:
    Sample output Y (int16_t)

  Example:
    <code>

    PARM_EQUAL_FILTER filtArrayPara[8];
    uint16_t dataY, dataX;

    // fill entire Filter Array with coefs
    for (i=0;i<8;i++)
    {
        filtArrayPara[i].Z[0]=0;
        filtArrayPara[i].Z[1]=0;

        filtArrayPara[i].G.fracGain = 0x7FFF;       // gain = 1 default
        filtArrayPara[i].G.expGain = 1;             // == 2^N; gain of 2

        // note all coefs are half value of original design, gain handled in algorithm
        // none pass -- default
        filtArrayPara[i].b[0]=0;        // feed forward b0 coef
        filtArrayPara[i].b[1]=0;        // feed forward b1 coef
        filtArrayPara[i].b[2]=0;        // feed forward b2 coef

        // note all coefs are half value of original design, gain handled in algorithm
        // note subtract is handled in algorithm, so coefs go in at actual value
        filtArrayPara[i].a[0]=0;        // feedback a1 coef
        filtArrayPara[i].a[1]=0;        // feedback a2 coef
     }

        // 1K bandpass Q=0.9
        filtArrayPara[7].G.fracGain = 0x4000;   // gain = 0.5 because using 2 outputs
        // note all coefs are half value of original design, gain handled in algorithm
        filtArrayPara[7].b[0]=0x04ad;
        filtArrayPara[7].b[1]=0;                // feed forward b1 coef
        filtArrayPara[7].b[2]=0xfb53;           // feed forward b2 coef

        // note all coefs are half value of original design, gain handled in algorithm
        // note subtract is handled in algorithm, so coefs go in at actual value
        filtArrayPara[7].a[0]=0x8a90;           // feedback a1 coef
        filtArrayPara[7].a[1]=0x36a4;           // feedback a2 coef

        // 300 Hz bandpass    Q=0.9
        filtArrayPara[6].G.fracGain = 0x1000;   // gain = 0.125 as an example
        // note all coefs are half value of original design, gain handled in algorithm
        filtArrayPara[6].b[0]=0x017b;           // feed forward b0 coef
        filtArrayPara[6].b[1]=0;                // feed forward b1 coef
        filtArrayPara[6].b[2]=0xfe85;           // feed forward b2 coef

        // note all coefs are half value of original design, gain handled in algorithm
        // note subtract is handled in algorithm, so coefs go in at actual value
        filtArrayPara[6].a[0]=0x8316;           // feedback a1 coef
        filtArrayPara[6].a[1]=0x3d08;           // feedback a2 coef

    for (i=0;i<256;i++)
        {
         // *** get input data here
         dataX = compound_300_1K_hz16[i];

         dataY = DSP_FilterIIRBQ16_cascade8_fast(dataX, filtArray);

         // *** do something with the DataY here
         }
   </code>

  Remarks:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.
    A div 8 has been applied in the function for each block, as the 8 blocks are
    summed to form the end output.  Each block has a functional hard-coded gain
    of 0.125.
    Digital attenuation on a per channel basis has been been applied by fracgain.
    This is a Q15 fractional value.
    Binary gain globally has been applied to the first instance of expgain.

 */

int16_t DSP_FilterIIRBQ16_parallel8(int16_t Xin, PARM_EQUAL_FILTER *pFilter);

// *****************************************************************************
/* Function DSP_FilterIIRBQ16_parallel8_fast:

    int16_t DSP_FilterIIRBQ16_parallel8_fast(int16_t Xin, PARM_EQUAL_FILTER_16 *pFilter);

  Summary:
    Performs a 8 parallel single-pass IIR Biquad Filters, and sums the result.

  Description:
    Calculates a 8 parallel, single pass IIR biquad filters on Xin, sums the result
    and delivers the result as a 16-bit output.  All math is performed using 16
    bit instructions, which results rounded to 16-bits for the output.  The
    delay register is stored as a 16-bit value for subsequent functions.
    Output is tuned by 2 multiplier factors.  First each parallel section has a
    fractional gain (attenuation) that enables individual scaling of that section.
    Second, a global binary (log2N) gain is applied to the result.  The combination
    of gain factors enable both gain and attenuation.
    All values are fractional Q15.  The function will round outputs and saturate
    if maximum or minimum values are exceeded.

         Y = Y7/8 + Y6/8 + Y5/8 + Y4/8 + Y3/8 + Y2/8 + Y1/8 + Y0/8
    where each Yn filter element represents a unique IIR biquad:
       Yn = X(0)*b0 + (b1 * X(n-1)) + (b2 * X(n-2)) - (a1 * Yn(-1)) - (a2 * Yn(-2))

  Precondition:
    Delay register values should be initialized to zero.
    The sum of all fracgain should be <= 1

  Parameters:
    Xin         - input data element X (int16_t)
    pFilter     - pointer to filter coef and delay structure

  Returns:
    Sample output Y (int16_t)

  Example:
    <code>

    PARM_EQUAL_FILTER_16 filtArrayPara[8];      // note change in data structure
    uint16_t dataY, dataX;

    // fill entire Filter Array with coefs
    for (i=0;i<8;i++)
    {
        filtArrayPara[i].Z[0]=0;
        filtArrayPara[i].Z[1]=0;

        filtArrayPara[i].G.fracGain = 0x7FFF;       // gain = 1 default
        filtArrayPara[i].G.expGain = 1;             // log2N; gain of 2

        // note all coefs are half value of original design, gain handled in algorithm
        // none pass -- default
        filtArrayPara[i].b[0]=0;        // feed forward b0 coef
        filtArrayPara[i].b[1]=0;        // feed forward b1 coef
        filtArrayPara[i].b[2]=0;        // feed forward b2 coef

        // note all coefs are half value of original design, gain handled in algorithm
        // note subtract is handled in algorithm, so coefs go in at actual value
        filtArrayPara[i].a[0]=0;        // feedback a1 coef
        filtArrayPara[i].a[1]=0;        // feedback a2 coef
     }

        // 1K bandpass Q=0.9
        filtArrayPara[7].G.fracGain = 0x4000;   // gain = 0.5 because using 2 outputs
        // note all coefs are half value of original design, gain handled in algorithm
        filtArrayPara[7].b[0]=0x04ad;
        filtArrayPara[7].b[1]=0;                // feed forward b1 coef
        filtArrayPara[7].b[2]=0xfb53;           // feed forward b2 coef

        // note all coefs are half value of original design, gain handled in algorithm
        // note subtract is handled in algorithm, so coefs go in at actual value
        filtArrayPara[7].a[0]=0x8a90;           // feedback a1 coef
        filtArrayPara[7].a[1]=0x36a4;           // feedback a2 coef

        // 300 Hz bandpass    Q=0.9
        filtArrayPara[6].G.fracGain = 0x1000;   // gain = 0.125 as an example
        // note all coefs are half value of original design, gain handled in algorithm
        filtArrayPara[6].b[0]=0x017b;           // feed forward b0 coef
        filtArrayPara[6].b[1]=0;                // feed forward b1 coef
        filtArrayPara[6].b[2]=0xfe85;           // feed forward b2 coef

        // note all coefs are half value of original design, gain handled in algorithm
        // note subtract is handled in algorithm, so coefs go in at actual value
        filtArrayPara[6].a[0]=0x8316;           // feedback a1 coef
        filtArrayPara[6].a[1]=0x3d08;           // feedback a2 coef

    for (i=0;i<256;i++)
        {
         // *** get input data here
         dataX = compound_300_1K_hz16[i];

         dataY = DSP_FilterIIRBQ16_cascade8_fast(dataX, filtArray);

         // *** do something with the DataY here
         }
   </code>

  Remarks:
    The delay register values should be initialized to zero prior to the first
    call to the function, they are updated each pass.
    A gain of 2 has been hard coded into the function. This implies that all
    coefs should be input at half value. This is purposeful, since many filter
    designs need a div2 to have each coef between the required -1<n<1.
    A div 8 has been applied in the function for each block, as the 8 blocks are
    summed to form the end output.  Each block has a functional hard-coded gain
    of 0.125.
    Digital attenuation on a per channel basis has been been applied by fracgain.
    This is a Q15 fractional value.
    Binary gain globally has been applied to the first instance of expgain.
    This function is optimized with microAptiv core instructions.
 */

int16_t DSP_FilterIIRBQ16_parallel8_fast(int16_t Xin, PARM_EQUAL_FILTER_16 *pFilter);

// *****************************************************************************
/* Function DSP_MatrixAdd32:

    void DSP_MatrixAdd32(matrix32 *resMat, matrix32 *srcMat1, matrix32 *srcMat2);

  Summary:
    Addition of two matrices C = (A + B).

  Description:
    Vector summation of two matrices, where both have 32-bit integer elements.
    The resulting output will saturate if element addition exceeds MAX32 or MIN32.

  Precondition:
    Both matrices must be equivalent in rows and columns.
    Both Matrices must be set into structure (ROWS, COLUMNS, vector_pointer).

  Parameters:
    resMat     - pointer to new sum Matrix C (*int32_t)
    srcMat1    - pointer to the Matrix A structure (*int32_t)
    srcMat2    - pointer to the Matrix B structure (*int32_t)

  Returns:
     None.

  Example:
    <code>

 #define ROW 2
 #define COL 2

    matrix32 *resMat, *srcMat1, *srcMat2;
    int32_t result[ROW*COL];

    int32_t matA[ROW*COL] = {1,2,3,4};
    int32_t matB[ROW*COL] = {2,4,6,8};

    matrix32 mat, mat2, mat3;
    resMat=&mat;
    srcMat1=&mat2;
    srcMat2=&mat3;

    srcMat1->row=ROW;
    srcMat1->col=COL;
    srcMat1->pMatrix=matA;

    srcMat2->col=COL;
    srcMat2->row=ROW;
    srcMat2->pMatrix=matB;

    resMat->row=ROW;
    resMat->col=COL;
    resMat->pMatrix=result;

    DSP_MatrixAdd32(resMat, srcMat1, srcMat2);

    // result[i] = matA[i] + matB[i] = {3,6,9,0xA}

    </code>

  Remarks:
    Execution Time (cycles): 225 cycles + 23 / matrix_element. The function will
    saturate the output value if it exceeds maximum limits per element.
*/

void DSP_MatrixAdd32(matrix32 *resMat, matrix32 *srcMat1, matrix32 *srcMat2);

// *****************************************************************************
/* Function DSP_MatrixEqual32:

    void DSP_MatrixEqual32(matrix32 *resMat, matrix32 *srcMat);

  Summary:
    Equality of two matrices C = (A).

  Description:
    Vector copy of all elements from one matrix to another.
    C is a duplicate of A.

  Precondition:
     None.

  Parameters:
    resMat     - pointer to completed new Matrix C (*int32_t)
    srcMat     - pointer to the Matrix A structure (*int32_t)

  Returns:
    None.

  Example:
    <code>

 #define ROW 2
 #define COL 2

    matrix32 *resMat, *srcMat1, *srcMat2;
    int32_t result[ROW*COL];

    int32_t matA[ROW*COL] = {5,2,-3,8};

    matrix32 mat, mat2;
    resMat=&mat;
    srcMat1=&mat2;

    srcMat1->row=ROW;
    srcMat1->col=COL;
    srcMat1->pMatrix=matA;

    resMat->row=ROW;
    resMat->col=COL;
    resMat->pMatrix=result;

    DSP_MatrixEqual32(resMat, srcMat1, srcMat2);

    // result[i] = matA[i] = {5, 2, -3, 8}

    </code>

  Remarks:
    Execution Time (cycles): 163 cycles + 12 / matrix_element.

*/

void DSP_MatrixEqual32(matrix32 *resMat, matrix32 *srcMat);

// *****************************************************************************
/* Function DSP_MatrixInit32:

    void DSP_MatrixInit32(int32_t *data_buffer, int N, int32_t num);

  Summary:
    Initializes the first N elements of a Matrix to the value num.

  Description:
    Copy the value num into the first N Matrix elements of data_buffer.

  Precondition:
    data_buffer must be predefined to be equal to or greater than N elements.
    N must be a factor of four, or it will truncate to the nearest factor of four.

  Parameters:
    data_buffer   - pointer to the Matrix to be initialized (int32_t[M*N])
    N             - number of elements to be initialized (int32_t)
    num           - value to be initialized into the matrix (int32_t)
   
  Returns:
    None.

  Example:
    <code>

 #define ROW 3
 #define COL 3

    int32_t numElements = 4;  // multiple of 4
    int valueElements = -1;

    int32_t matA[ROW*COL] = {5,2,-3,8,4,2,-6,8,9};

    DSP_MatrixInit32(matA, numElements, valueElements);

    // matA[i] = {-1,-1,-1,-1,4,2,-6,8,9}

    </code>

  Remarks:
    None.
*/

void DSP_MatrixInit32(int32_t *data_buffer, int N, int32_t num);

// *****************************************************************************
/* Function DSP_MatrixMul32:

    void DSP_MatrixMul32(matrix32 *resMat, matrix32 *srcMat1, matrix32 *srcMat2);

  Summary:
    Multiplication of two matrices C = A x B.

  Description:
    Multiplication of two matrices, with inputs and outputs being in fractional
    Q31 numerical format.  The output elements will saturate if the dot product
    exceeds maximum or minimum fractional values.

  Precondition:
    Matrices must be aligned such that columns of A = rows of B.
    resMat must have the format of rows of A, columns of B.
    All Matrices must be set into structure (ROWS, COLUMNS, vector_pointer).

  Parameters:
    resMat     - pointer to different Matrix C structure (*int32_t)
    srcMat1    - pointer to the Matrix A structure (*int32_t)
    srcMat2    - pointer to the Matrix B structure (*int32_t)

  Returns:
    None.

  Example:
    <code>

 #define ROW1 3
 #define COL1 2
 #define ROW2 2
 #define COL2 2

    matrix32 *resMat, *srcMat1, *srcMat2;
    int32_t result[ROW1*COL2];

    int32_t test_MatrixA[ROW1*COL1]=
    {
        0x40000000,0x20000000,      // 0.5,  0.25
        0xD999999A,0x4CCCCCCC,      // -0.3, 0.6
        0xC0000000,0x0CCCCCCD       // -0.5  0.1
    };

    int32_t test_MatrixB[ROW2*COL2]=
    {
        0x40000000,0x20000000,      // 0.5, 0.25
        0x0CCCCCCD,0xCCCCCCCD       // 0.1, -0.4
    };

    matrix32 mat, mat2, mat3;
    resMat=&mat;
    srcMat1=&mat2;
    srcMat2=&mat3;

    srcMat1->row=ROW1;
    srcMat1->col=COL1;
    srcMat1->pMatrix=test_MatrixA;

    srcMat2->col=COL2;
    srcMat2->row=ROW2;
    srcMat2->pMatrix=test_MatrixB;

    resMat->row=ROW1;  // note resulting matrix MUST have ROW1 & COL2 format
    resMat->col=COL2;
    resMat->pMatrix=result;

    DSP_MatrixMul32(resMat, srcMat1, srcMat2);

    // result[] = matA[] x matB[] =
    //    { 0x23333333, 0x03333333   // 0.275, 0.025
    //      0xF47AE147, 0xD7AE147B   // -0.9, -0.315
    //      0xE147AE14, 0xEAE147AE } // -0.24, -0.165

    </code>

  Remarks:
     Execution Time (cycles): 319 cycles + 38 / output matrix_element. The function 
     will saturate the output value if it exceeds maximum limits per element.
*/

void DSP_MatrixMul32(matrix32 *resMat, matrix32 *srcMat1, matrix32 *srcMat2);

// *****************************************************************************
/* Function DSP_MatrixScale32:

    void DSP_MatrixScale32(int32_t *data_buffer, int N, int32_t num);

  Summary:
      Scales each element of an input buffer (matrix) by a fixed number.

  Description:
      Multiply the first N elements of an input buffer by a fixed scalar num.  The
      resulting value is stored back into the input buffer.  N number total samples
      of the input buffer are processed.  All values are in Q31 fractional integer
      format. The result of calculations is saturated to the MAX32 or MIN32 value if
      exceeded.

  Precondition:
      data_buffer must be predefined to be equal to or greater than N elements.
      N must be a factor of four, or will truncate to the nearest factor of four.

  Parameters:
     data_buffer   - pointer to the Matrix to be initialized (int32_t[M*N])
     N             - number of elements to be initialized (int)
     num           - value to be initialized into the matrix (int32_t)

  Returns:
    None.

  Example:
    <code>

    int32_t numScale = 0x40000000;  // 0.5
    int valN = 12;
    int32_t inputBufScale[12] = {0x40000000, 0x40000000, 0x20000000, 0x20000000,
                0x19999999, 0xCCCCCCCD, 0xF3333333, 0x80000000,
                0x7FFFFFFF, 0x00000000, 0x40000000, 0x70000000 };
    // 0.5, 0.5, 0.25, 0.25, 0.25, 0.2, -0.4, -0.1, -1, 1, 0, 0.5, 0.875

    DSP_MatrixScale32(inputBufScale,valN,numScale);

 // inputBufScale[i] = {0x20000000, 0x20000000, 0x10000000, 0x10000000,
 //          0x0CCCCCCC, 0xE6666666, 0xF9999999, 0xC0000000,
 //          0x3FFFFFFF, 0x00000000, 0x20000000, 0x38000000}
 //  0.25, 0.25, 0.125, 0.125, 0.1, -0.2, -0.05, -0.5, 0.5, 0, 0.25, 0.4375

    </code>

  Remarks:
    Execution time (cycles): 190 + 9 cycles / element, typical.
*/

void DSP_MatrixScale32(int32_t *data_buffer, int N, int32_t num);

// *****************************************************************************
/* Function DSP_MatrixSub32:

    void DSP_MatrixSub32(matrix32 *resMat, matrix32 *srcMat1, matrix32 *srcMat2);

  Summary:
    Subtraction of two matrices C = (A - B).

  Description:
    Vector subtraction of two matrices, where both have 32-bit integer elements.
    The resulting output will saturate if element addition exceeds MAX32 or MIN32.

  Precondition:
    Both matrices must be equivalent in rows and columns.
    All Matrices must be set into structure (ROWS, COLUMNS, vector_pointer)

  Parameters:
    resMat     - pointer to different Matrix C structure (*int32_t)
    srcMat1    - pointer to the Matrix A structure (*int32_t)
    srcMat2    - pointer to the Matrix B structure (*int32_t)

  Returns:
    None.

  Example:
    <code>

 #define ROW 2
 #define COL 2

    matrix32 *resMat, *srcMat1, *srcMat2;
    int32_t result[ROW*COL];

    int32_t matA[ROW*COL] = {5,2,-3,8};
    int32_t matB[ROW*COL] = {2,2,2,2};

    matrix32 mat, mat2, mat3;
    resMat=&mat;
    srcMat1=&mat2;
    srcMat2=&mat3;

    srcMat1->row=ROW;
    srcMat1->col=COL;
    srcMat1->pMatrix=matA;

    srcMat2->col=COL;
    srcMat2->row=ROW;
    srcMat2->pMatrix=matB;

    resMat->row=ROW;
    resMat->col=COL;
    resMat->pMatrix=result;

    DSP_MatrixSub32(resMat, srcMat1, srcMat2);

    // result[i] = matA[i] - matB[i] = {3,0,-5,6}

    </code>

  Remarks:
     Execution Time (cycles): 222 cycles + 21 / matrix_element. The function will 
     saturate the output value if it exceeds maximum limits per element.
*/

void DSP_MatrixSub32(matrix32 *resMat, matrix32 *srcMat1, matrix32 *srcMat2);

// *****************************************************************************
/* Function DSP_MatrixTranspose32:

    void DSP_MatrixTranspose32(matrix32 *desMat, matrix32 *srcMat);

  Summary:
    Transpose of a Matrix C = A (T).

  Description:
    Transpose of rows and columns of a matrix.

  Precondition:
    Matrix definitions for ROWS and COLS must be transposed prior to the function 
    call.

  Parameters:
    desMat      - pointer to transposed new Matrix C (*int32_t)
    srcMat      - pointer to the Matrix A structure (*int32_t)
   
  Returns:
    None.

  Example:
    <code>

 #define ROW 3
 #define COL 4

    matrix32 *resMat, *srcMat1;
    int32_t result[ROW*COL];

    int32_t matA[ROW*COL] = { 1,  2,  3,  4,
                            5,  6,  7,  8,
                           -1, -3, -5, -7};

    matrix32 mat, mat2;
    resMat=&mat;
    srcMat1=&mat2;

    srcMat1->row=ROW;
    srcMat1->col=COL;
    srcMat1->pMatrix=matA;

    resMat->row=COL;   // note the shift in columns and rows
    resMat->col=ROW;
    resMat->pMatrix=result;

    DSP_MatrixTranspose32(resMat, srcMat1);

    // result[] = matA(T)[] = {   1,  5, -1,
    //                            2,  6, -3,
    //                            3,  7, -5,
    //                            4,  8, -7}

    </code>

  Remarks:
    Execution Time (cycles): 210 cycles + 10 / matrix_element.

*/

void DSP_MatrixTranspose32(matrix32 *desMat, matrix32 *srcMat);

// *****************************************************************************
/* Function DSP_TransformFFT16:

    void DSP_TransformFFT16(int16c *dout, int16c *din,
                           int16c *twiddles, int16c *scratch, int log2N);

  Summary:
    Creates an Fast Fourier Transform (FFT) from a time domain input.

  Description:
    Performs an complex FFT on the input, din, and stores the complex result in
    dout.  Performs 2^log2N point calculation, and the working buffer scratch as
    well as the input and output must be 2^log2N in length.  Coefficient twiddle
    factors come from twiddles, and may be loaded with the use of
    DSP_TransformFFT16_setup.  All values are 16 bit (Q15) fractional.

  Precondition:
    din, dout, twiddles and scratch must have N elements
    N is calculated as 2^(log2N)
    log2N must be >= 3
    FFT factors must be calculated in advance, use DSP_TransformFFT16_setup

  Parameters:
    dout        - pointer to complex output array (int16c)
    din         - pointer to complex input array (int16c)
    twiddles    - pointer to an complex array of factors (int16c)
    scratch     - pointer to a complex scratch pad buffer (int16c)
    log2N       - binary exponent of number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int log2N = 8;  // log2(256) = 8
    int fftSamples = 256;

    int16c  *fftDin;
    int16c  fftDout[fftSamples];
    int16c  scratch[fftSamples];
    int16c  fftCoefs[fftSamples];

    int16c *fftc;
    fftc = &fftCoefs;

    DSP_TransformFFT16_setup(fftc, log2N);  // call setup function

    while (1)
    {
        fftDin = &fftin_8Khz_long_window16;  // get 256 point complex data

        DSP_TransformFFT16(fftDout, fftDin, fftc, scratch, log2N);

        // do something with the output, fftDout
    };
   </code>

  Remarks:
    Scratch must be declared but need not be initialized.
    Din may be aided with a window function prior to calling the FFT, but is not
    required.
    Din is a complex number array, but may be loaded solely with real numbers if
    no phase information is available.
*/


void DSP_TransformFFT16(int16c *dout, int16c *din, int16c *twiddles, int16c *scratch, int log2N);

// *****************************************************************************
/* Function DSP_TransformIFFT16:

    void DSP_TransformIFFT16(int16c *dout, int16c *din, int16c *twiddles,
                   int16c *scratch, int log2N);

  Summary:
    Creates an Inverse Fast Fourier Transform (FFT) from a frequency domain input.

  Description:
    Performs an complex Inverse FFT on the input, din, and stores the complex result
    in dout.  Performs 2^log2N point calculation, and the working buffer scratch as
    well as the input and output must be 2^log2N in length.  Coefficient twiddle
    factors come from twiddles, and may be loaded with the use of
    DSP_TransformFFT16_setup.  All values are 16 bit (Q15) fractional.

  Precondition:
    din, dout, twiddles and scratch must have N elements
    N is calculated as 2^(log2N)
    log2N must be >= 3
    FFT factors must be calculated in advance, use DSP_TransformFFT16_setup

  Parameters:
    dout        - pointer to complex output array (int16c)
    din         - pointer to complex input array (int16c)
    twiddles    - pointer to an complex array of factors (int16c)
    scratch     - pointer to a complex scratch pad buffer (int16c)
    log2N       - binary exponent of number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int ilog2N = 10;  // log2(64) = 6; log2(256) = 8; log2(1024) = 10;
    int ifftSamples = pow(2,ilog2N);

    int16c  *ifftDin;
    int16c ifftDout[ifftSamples];
    int16c  iscratch[ifftSamples];
    int16c  ifftCoefs[ifftSamples];
    int16c  ifftTimeOut[ifftSamples];

 // set up twiddle factors, these are used for both FFT and iFFT

    int16c *ifftc;
    ifftc = &ifftCoefs;
    DSP_TransformFFT16_setup( ifftc, ilog2N);  // call to coef setup

  // in this example, we take an FFT of an original time domain (sine wave)
  // the output of the FFT is used as the input of the iFFT for comparison

    ifftDin = &fftin_800hz_verylong16;
    DSP_TransformFFT16(ifftDout, ifftDin, ifftc, iscratch, ilog2N);

  // ifftDout = frequency domain output, complex number array

    DSP_TransformIFFT16(ifftTimeOut, ifftDout, ifftc, iscratch, ilog2N);

  // do something with the output, fftTimeOut, time domain

    </code>

  Remarks:
    Scratch must be declared but need not be initialized.
    Din may be aided with a window function prior to calling the FFT, but is not
    required.
    A very similar function to the FFT is executed for the inverse FFT.  This requires
    twiddle factors set in advance with the same method as used in the FFT.  Complex
    conjugate and scaling are handled within the algorithm.
    The output is scaled using binary shifting based on log2N.  Since the algorithm
    reduces the output by a scale factor of log2N, the resolution is reduced proportionally
    to the number of data points.
*/

void DSP_TransformIFFT16(int16c *dout, int16c *din, int16c *twiddles, int16c *scratch, int log2N);

// *****************************************************************************
/* Function DSP_TransformFFT16_setup:

    void    DSP_TransformFFT16_setup(int16c *twiddles, int log2N);

  Summary:
    Creates FFT coefficients for use in the FFT16 function.

  Description:
    Calculates the N twiddle factors required to operate the FFT16 function.
    These factors are done in serial fashion, and require considerable processing
    power. Ideally this function would be run only once prior to an ongoing FFT,
    and the results held in a buffer.

  Precondition:
    twiddles must be N in length
    N is calculated (2^log2N)

  Parameters:
    twiddles      - pointer to a complex array of factors (int16c)
    log2N         - binary exponent of number of data points (int)

  Returns:
    None.

  Example:
    <code>

    see DSP_TransformFFT16 for example.

   </code>

  Remarks:
    This function is of considerable length and executed in C.  It is recommended
    it only be called once for any given FFT length in time sensitive applications.
*/

void DSP_TransformFFT16_setup(int16c *twiddles, int log2N);

// *****************************************************************************
/* Function DSP_TransformFFT32:

    void DSP_TransformFFT32(int32c *dout, int32c *din, int32c *twiddles,
                       int32c *scratch, int log2N);

  Summary:
    Creates an Fast Fourier Transform (FFT) from a time domain input.

  Description:
    Performs an complex FFT on the input, din, and stores the complex result in
    dout.  Performs 2^log2N point calculation, and the working buffer scratch as
    well as the input and output must be 2^log2N in length.  Coefficient twiddle
    factors come from twiddles, and may be loaded with the use of
    DSP_TransformFFT16_setup.  All values are 16 bit (Q31) fractional.

  Precondition:
    din, dout, twiddles and scratch must have N elements
    N is calculated as 2^(log2N)
    log2N must be >= 3
    FFT factors must be calculated in advance, use DSP_TransformFFT32_setup

  Parameters:
    dout        - pointer to complex output array (int32c)
    din         - pointer to complex input array (int32c)
    twiddles    - pointer to an complex array of FFT factors (int32c)
    scratch     - pointer to a complex scratch pad buffer (int32c)
    log2N       - binary exponent of number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int log2N = 8;  // log2(256) = 8
    int fftSamples = 256;

    int32c  *fftDin;
    int32c  fftDout[fftSamples];
    int32c  scratch[fftSamples];
    int32c  fftCoefs[fftSamples];

    int32c *fftc;
    fftc = &fftCoefs;

    DSP_TransformFFT32_setup(fftc, log2N);  // call setup function

    while (1)
    {
        fftDin = &fftin_5Khz_long_window32;  // get 256 point complex data

        DSP_TransformFFT32(fftDout, fftDin, fftc, scratch, log2N);

        // do something with the output, fftDout
    };
   </code>

  Remarks:
    Scratch must be declared but need not be initialized.
    Din may be aided with a window function prior to calling the FFT, but is not
    required.
    Din is a complex number array, but may be loaded solely with real numbers if
    no phase information is available.
*/

void DSP_TransformFFT32(int32c *dout, int32c *din, int32c *twiddles, int32c *scratch, int log2N);

// *****************************************************************************
/* Function DSP_TransformFFT32_setup:

    void        DSP_TransformFFT32_setup(int32c *twiddles, int log2N);

  Summary:
    Creates FFT coefficients for use in the FFT32 function.

  Description:
    Calculates the N FFT twiddle factors required to operate the FFT32 function.
    These factors are done in serial fashion, and require considerable processing
    power. Ideally this function would be run only once prior to an ongoing FFT,
    and the results held in a buffer.

  Precondition:
    twiddles must be N in length
    N is calculated (2^log2N)

  Parameters:
    twiddles      - pointer to a complex array of coefficients (int32c)
    log2N         - binary exponent of number of data points (int)

  Returns:
    None.

  Example:
    <code>

    see DSP_TransformFFT32 for example.

   </code>

  Remarks:
    This function is of considerable length and executed in C.  It is recommended
    it only be called once for any given FFT length in time sensitive applications.
*/

void DSP_TransformFFT32_setup(int32c *twiddles, int log2N);

// *****************************************************************************
/* Function DSP_TransformWindow_Bart16:

    void    DSP_TransformWindow_Bart16(int16_t *OutVector, int16_t *InVector, int N);

  Summary:
    Perform a Bartlett window on a vector.

  Description:
    Compute a Bartlett (Triangle) Window on the first N samples of the input
    vector, InVector. The output is stored in OutVector.  Operations are performed
    at higher resolution and rounded for the most accuracy possible.  Input and
    output values are in Q15 fractional format.
    The Bartlett Window follows the equation:

       Window(n) = 1 - (abs(2*n - N)/N)
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t OutVector16[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector16[i]= 0x4000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Bart16(OutVector16, InVector16, WindowN);
// OutWindow = 0x0000, 0x1000, 0x2000, 0x3000, 0x4000, 0x3000, 0x2000, 0x1000

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Bart16(int16_t *OutVector, int16_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Bart32:

    void    DSP_TransformWindow_Bart32(int32_t *OutVector, int32_t *InVector, int N);

  Summary:
    Perform a Bartlett window on a vector.

  Description:
    Compute a Bartlett (Triangle) Window on the first N samples of the input vector, InVector.
    The output is stored in OutVector.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in
    Q31 fractional format.
    The Bartlett Window follows the equation:

       Window(n) = 1 - (abs(2*n - N)/N)
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int32_t OutVector32[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector32[i]= 0x40000000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Bart32(OutVector32, InVector32, WindowN);
// OutWindow = 0x0, 0x10000000, 0x20000000, 0x30000000, 0x40000000,
//                      0x30000000, 0x20000000, 0x10000000

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Bart32(int32_t *OutVector, int32_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Black16:

    void DSP_TransformWindow_Black16(int16_t *OutVector, int16_t *InVector, int N);

  Summary:
    Perform a Blackman window on a vector.

  Description:
    Compute a Blackman Window on the first N samples of the input vector, InVector.
    The output is stored in OutVector.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in Q15
    fractional format.
    The Blackman Window follows the equation:

       Window(n) = 0.42659 - 0.49656 * COS(2*Pi*n/(N-1)) + 0.076849 * COS(4*Pi*n/(N-1))
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t OutVector16[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector16[i]= 0x4000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Black16(OutVector16, InVector16, WindowN);
// OutWindow = 0x0071, 0x0665, 0x1DF1, 0x3B00, 0x3B00, 0x1DF1, 0x0665, 0x0071

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Black16(int16_t *OutVector, int16_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Black32:

    void DSP_TransformWindow_Black32(int32_t *OutVector, int32_t *InVector, int N);

  Summary:
    Perform a Blackman window on a vector.

  Description:
    Compute a Blackman Window on the first N samples of the input vector, InVector.
    The output is stored in OutVector.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in Q31
    fractional format.
    The Blackman Window follows the equation:

       Window(n) = 0.42659 - 0.49656 * COS(2*Pi*n/(N-1)) + 0.076849 * COS(4*Pi*n/(N-1))
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int32_t OutVector32[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector32[i]= 0x40000000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Black32(OutVector32, InVector32, WindowN);
// OutWindow = 0x0070B490, 0x06649680, 0x1DF13240, 0x3B003D80, 0x3B003D80,
//                      0x1DF13240, 0x06649680, 0x0070B490

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Black32(int32_t *OutVector, int32_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Cosine16:

    void DSP_TransformWindow_Cosine16(int16_t *OutVector, int16_t *InVector, int N);

  Summary:
    Perform a Cosine (Sine) window on a vector.

  Description:
    Compute a Cosine Window on the first N samples of the input vector, InVector.
    The output is stored in OutWindow.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in
    Q15 fractional format.
    The Cosine Window follows the equation:

       Window(n) = SIN(Pi*n/(N-1))
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t OutVector16[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector16[i]= 0x4000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Cosine16(OutVector16, InVector16, WindowN);
// OutWindow = 0x0000, 0x1BC5, 0x320A, 0x3E65, 0x3E65, 0x320A, 0x1BC5, 0x0071

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Cosine16(int16_t *OutVector, int16_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Cosine32:

    void DSP_TransformWindow_Cosine32(int32_t *OutVector, int32_t *InVector, int N);

  Summary:
    Perform a Cosine (Sine) window on a vector.

  Description:
    Compute a Cosine Window on the first N samples of the input vector, InVector.
    The output is stored in OutWindow.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in
    Q31 fractional format.
    The Cosine Window follows the equation:

       Window(n) = SIN(Pi*n/(N-1))
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int32_t OutVector32[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector32[i]= 0x40000000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Cosine32(OutVector32, InVector32, WindowN);
// OutWindow = 0x00000000, 0x1BC4C060, 0x32098700, 0x3E653800, 0x3E653800,
//                      0x32098700, 0x1BC4C060, 0x00000000

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Cosine32(int32_t *OutVector, int32_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Hamm16:

    void DSP_TransformWindow_Hamm16(int16_t *OutVector, int16_t *InVector, int N);

  Summary:
    Perform a Hamming window on a vector.

  Description:
    Compute a Hamming Window on the first N samples of the input vector, InVector.
    The output is stored in OutVector.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in
    Q15 fractional format.
    The Hamming Window follows the equation:

       Window(n) = 0.54 - 0.46 * COS(2*Pi*n/N)
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t OutVector16[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector16[i]= 0x4000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Hamm16(OutVector16, InVector16, WindowN);
// OutWindow = 0x051F, 0x0DBE, 0x228F, 0x3761, 0x4000, 0x3761, 0x228F, 0x0DBE

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Hamm16(int16_t *OutVector, int16_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Hamm32:

    void DSP_TransformWindow_Hamm32(int32_t *OutVector, int32_t *InVector, int N);

  Summary:
    Perform a Hamming window on a vector.

  Description:
    Compute a Hamming Window on the first N samples of the input vector, InVector.
    The output is stored in OutVector.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in
    Q31 fractional format.
    The Hamming Window follows the equation:

       Window(n) = 0.54 - 0.46 * COS(2*Pi*n/N)
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int32_t OutVector32[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector32[i]= 0x40000000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Hamm32(OutVector32, InVector32, WindowN);
// OutWindow = 0x051EB860, 0x0DBE26C0, 0x228F5C40, 0x37609200, 0x40000000,
//                      0x37609200, 0x228F5C40, 0x0DBE26C0

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Hamm32(int32_t *OutVector, int32_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Hann16:

    void DSP_TransformWindow_Hann16(int16_t *OutVector, int16_t *InVector, int N);

  Summary:
    Perform a Hanning window on a vector.

  Description:
    Compute a Hanning Window on the first N samples of the input vector, InVector.
    The output is stored in OutVector.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in
    Q15 fractional format.
    The Hanning Window follows the equation:

       Window(n) = 0.5 - 0.5 * COS(2*Pi*n/N)
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t OutVector16[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector16[i]= 0x4000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Hann16(OutVector16, InVector16, WindowN);
// OutWindow = 0x0000, 0x095F, 0x2000, 0x36A1, 0x4000, 0x36A1, 0x2000, 0x095F

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Hann16(int16_t *OutVector, int16_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Hann32:

    void DSP_TransformWindow_Hann32(int32_t *OutVector, int32_t *InVector, int N);

  Summary:
    Perform a Hanning window on a vector.

  Description:
    Compute a Hanning Window on the first N samples of the input vector, InVector.
    The output is stored in OutVector.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in
    Q31 fractional format.
    The Hanning Window follows the equation:

       Window(n) = 0.5 - 0.5 * COS(2*Pi*n/N)
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int32_t OutVector32[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector32[i]= 0x40000000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Hann32(OutVector32, InVector32, WindowN);
// OutWindow = 0x00000000, 0x095F61C0, 0x20000000, 0x36A09E80, 0x40000000,
//                      0x36A09E80, 0x20000000, 0x095F61C0

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Hann32(int32_t *OutVector, int32_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Kaiser16:

    void DSP_TransformWindow_Kaiser16(int16_t *OutVector, int16_t *InVector, int N);

  Summary:
    Perform a Kaiser window on a vector.

  Description:
    Compute a Kaiser Window on the first N samples of the input vector, InVector.
    The output is stored in OutVector.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in
    Q15 fractional format.
    The Kaiser Window follows the equation:

       Window(n) = 0.402 - 0.498 * COS(2*Pi*n/N) + 0.098 * cos(4*Pi*n/N) + 0.001 * cos(6*Pi*n/N)
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t OutVector16[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector16[i]= 0x4000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Kaiser16(OutVector16, InVector16, WindowN);
// OutWindow = 0x0031, 0x0325, 0x1375, 0x304F, 0x3FCF, 0x304F, 0x1375, 0x0325

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Kaiser16(int16_t *OutVector, int16_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWindow_Kaiser32:

    void DSP_TransformWindow_Kaiser32(int32_t *OutVector, int32_t *InVector, int N);

  Summary:
    Perform a Kaiser window on a vector.

  Description:
    Compute a Kaiser Window on the first N samples of the input vector, InVector.
    The output is stored in OutVector.  Operations are performed at higher resolution
    and rounded for the most accuracy possible.  Input and output values are in
    Q31 fractional format.
    The Kaiser Window follows the equation:

       Window(n) = 0.402 - 0.498 * COS(2*Pi*n/N) + 0.098 * cos(4*Pi*n/N) + 0.001 * cos(6*Pi*n/N)
         where n is the window sample number
         N is the total number of samples
    The functional output computes WinVector(n) = Window(n) * InVector(n)

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int32_t OutVector32[8]={0};
    int  WindowN = 8;

    for (i=0;i<WindowN;i++)
    {
       InVector32[i]= 0x40000000;  // constant 0.5 for functional testing
    }

       DSP_TransformWindow_Kaiser32(OutVector32, InVector32, WindowN);
// OutWindow = 0x003126F6, 0x032555C8, 0x1374BCA0, 0x304F66C0, 0x3FCED900,
//                      0x304F66C0, 0x1374BCA0, 0x032555C8

   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.

*/

void DSP_TransformWindow_Kaiser32(int32_t *OutVector, int32_t *InVector, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Bart16:

    void    DSP_TransformWinInit_Bart16(int16_t *OutWindow, int N);

  Summary:
    Create a Bartlett window.

  Description:
    Create a N-element Bartlett (Triangle) Window, and store the output to OutWindow.
    Operations are performed at higher resolution floating point, and rounded for
    the most accuracy possible.  Output values are in Q15 fractional format.
    The Bartlett Window follows the equation:

       Window(n) = 1 - (abs(2*n - N)/N)
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Bart16(int16_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Bart32:

    void    DSP_TransformWinInit_Bart32(int32_t *OutWindow, int N);

  Summary:
    Create a Bartlett window.

  Description:
    Create a N-element Bartlett (Triangle) Window, and store the output to OutWindow.
    Operations are performed at higher resolution floating point, and rounded for
    the most accuracy possible.  Output values Q31 fractional format.
    The Bartlett Window follows the equation:

       Window(n) = 1 - (abs(2*n - N)/N)
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Bart32(int32_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Black16:

    void    DSP_TransformWinInit_Black16(int16_t *OutWindow, int N);

  Summary:
    Create a Blackman window.

  Description:
    Create a N-element Blackman Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Output values are Q16 fractional format.
    The Blackman Window follows the equation:

       Window(n) = 0.42659 - 0.49656 * COS(2*Pi*n/(N-1)) + 0.076849 * COS(4*Pi*n/(N-1))
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Black16(int16_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Black32:

    void    DSP_TransformWinInit_Black32(int32_t *OutWindow, int N);

  Summary:
    Create a Blackman window.

  Description:
    Create a N-element Blackman Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Output values are Q31 fractional format.
    The Blackman Window follows the equation:

       Window(n) = 0.42659 - 0.49656 * COS(2*Pi*n/(N-1)) + 0.076849 * COS(4*Pi*n/(N-1))
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Black32(int32_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Cosine16:

    void    DSP_TransformWinInit_Cosine16(int16_t *OutWindow, int N);

  Summary:
    Create a Cosine (Sine) window.

  Description:
    Create a N-element Cosine Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Output values are in Q15 fractional format.
    The Cosine Window follows the equation:

       Window(n) = SIN(Pi*n/(N-1))
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Cosine16(int16_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Cosine32:

    void    DSP_TransformWinInit_Cosine32(int32_t *OutWindow, int N);

  Summary:
    Create a Cosine (Sine) window.

  Description:
    Create a N-element Cosine Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Output values are in Q31 fractional format.
    The Cosine Window follows the equation:

       Window(n) = SIN(Pi*n/(N-1))
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Cosine32(int32_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Hamm16:

    void    DSP_TransformWinInit_Hamm16(int16_t *OutWindow, int N);

  Summary:
    Create a Hamming window.

  Description:
    Create a N-element Hamming Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Output values are in Q15 fractional format.
    The Hamming Window follows the equation:

       Window(n) = 0.54 - 0.46 * COS(2*Pi*n/N)
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Hamm16(int16_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Hamm32:

    void    DSP_TransformWinInit_Hamm32(int32_t *OutWindow, int N);

  Summary:
    Create a Hamming window.

  Description:
    Create a N-element Hamming Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Output values are in Q31 fractional format.
    The Hamming Window follows the equation:

       Window(n) = 0.54 - 0.46 * COS(2*Pi*n/N)
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Hamm32(int32_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Hann16:

    void    DSP_TransformWinInit_Hann16(int16_t *OutWindow, int N);

  Summary:
    Create a Hanning window.

  Description:
    Create a N-element Hanning Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Output values are in Q15 fractional format.
    The Hanning Window follows the equation:

       Window(n) = 0.5 - 0.5 * COS(2*Pi*n/N)
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Hann16(int16_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Hann32:

    void    DSP_TransformWinInit_Hann32(int32_t *OutWindow, int N);

  Summary:
    Create a Hanning window.

  Description:
    Create a N-element Hanning Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Output values are in Q31 fractional format.
    The Hanning Window follows the equation:

       Window(n) = 0.5 - 0.5 * COS(2*Pi*n/N)
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Hann32(int32_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Kaiser16:

    void        DSP_TransformWinInit_Kaiser16(int16_t *OutWindow, int N);

  Summary:
    Create a Kaiser window.

  Description:
    Create a N-element Kaiser Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Output values are in Q15 fractional format.
    The Kaiser Window follows the equation:

       Window(n) = 0.402 - 0.498 * COS(2*Pi*n/N) + 0.098 * cos(4*Pi*n/N) + 0.001 * cos(6*Pi*n/N)
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Kaiser16(int16_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_TransformWinInit_Kaiser32:

    void        DSP_TransformWinInit_Kaiser32(int32_t *OutWindow, int N);

  Summary:
    Create a Kaiser window.

  Description:
    Create a N-element Kaiser Window, and store the output to OutWindow.  Operations
    are performed at higher resolution floating point, and rounded for the most
    accuracy possible.  Output values are in Q31 fractional format.
    The Kaiser Window follows the equation:

       Window(n) = 0.402 - 0.498 * COS(2*Pi*n/N) + 0.098 * cos(4*Pi*n/N) + 0.001 * cos(6*Pi*n/N)
         where n is the window sample number
         N is the total number of samples

  Precondition:
      N must be a positive number.
      OutWindow must be declared with N elements or larger.

  Parameters:
    OutWindow  - pointer to output array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>



   </code>

  Remarks:
    This function is performed in C.  The function may be optimized for the library.
    It is dependent on the floating point math library.
    The functional window is an intermediate result that needs to be multiplied
    by an input vector prior to FFT processing. Because of significant processing time
    the window need only be computed once and the multiply of the (window * input)
    vector done during recurring loop processing.
*/

void DSP_TransformWinInit_Kaiser32(int32_t *OutWindow, int N);

// *****************************************************************************
/* Function DSP_VectorAbs16:

    void DSP_VectorAbs16(int16_t *outdata, int16_t *indata, int N);

  Summary:
    Calculate the absolute value of a vector.

  Description:
    Computes the absolute value of each element of indata and stores it to
    outdata. The number of samples to process is given by the parameter N.
    Data is in a Q15 fractional format.
         outdata[i] filled with the absolute value of elements of indata

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to eight and a multiple of eight.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int16_t)
    indata    - pointer to input array of 16-bit elements (int16_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int16_t outVal[8];
    int16_t inBufTest[16] = {-5,2,-3,4,-1,0,-2,-8,-21,21,10,100,200,127,-127,-2};
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorAbs16(pOutdata, inBufTest, Num);

    // outVal[i] = {5,2,3,4,1,0,2,8}

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorAbs16(int16_t *outdata, int16_t *indata, int N);

// *****************************************************************************
/* Function DSP_VectorAbs32:

    void DSP_VectorAbs32(int32_t *outdata, int32_t *indata, int N);

  Summary:
    Calculate the absolute value of a vector.

  Description:
    Computes the absolute value of each element of indata and stores it to
    outdata. The number of samples to process is given by the parameter N.
    Data is in a Q31 fractional format.
         outdata[i] filled with N elements of abs(indata[i])

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to eight and a multiple of eight.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int32_t)
    indata    - pointer to input array of 16-bit elements (int32_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int32_t outVal[8];
    int32_t inBufTest[16] = {-5,2,-3,4,-1,0,-2,-8,-21,21,10,100,200,127,-127,-2};
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorAbs32(pOutdata, inBufTest, Num);

    // outVal[i] = {5,2,3,4,1,0,2,8}

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorAbs32(int32_t *outdata, int32_t *indata, int N);

// *****************************************************************************
/* Function DSP_VectorAdd16:

    void DSP_VectorAdd16(int16_t *outdata, int16_t *indata1, int16_t *indata2, int N);

  Summary:
    Calculate the sum of two vectors.

  Description:
    Computes the sum value of each element of indata1 + indata2 and stores it to
    outdata. The number of samples to process is given by the parameter N.
    Data is in the Q15 fractional format.
          outdata[i] filled with N elements of indata1[i] + indata2[i]

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to eight and a multiple of eight.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int16_t)
    indata1   - pointer to input array of 16-bit elements (int16_t)
    indata2   - pointer to input array of 16-bit elements (int16_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int16_t outVal[8];
    int16_t inBufTest[16] = {-5,2,-3,4,-1,0,-2,-8,-21,21,10,100, 200, 127,-127,-2};
    int16_t inBuf2[16]=     { 1,2, 3,4, 5,6, 7, 8, 9, 10,-1,-100,-127,127,-7,  0};
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorAdd16(pOutdata, inBufTest, inBuf2, Num);

    // outVal[i] = inBufTest[i] + inBuf2[i] = {-4,4,0,8,4,6,5,0}

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorAdd16(int16_t *outdata, int16_t *indata1, int16_t *indata2, int N);

// *****************************************************************************
/* Function DSP_VectorAdd32:

    void DSP_VectorAdd32(int32_t *outdata, int32_t *indata1, int32_t *indata2, int N);

  Summary:
    Calculate the sum of two vectors.

  Description:
    Computes the sum value of each element of indata1 + indata2 and stores it to
    outdata. The number of samples to process is given by the parameter N.
    Data is in a Q31 fractional format.
         outdata[i] filled with N elements of indata1[i] + indata2[i]

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int32_t)
    indata1   - pointer to input array of 16-bit elements (int32_t)
    indata2   - pointer to input array of 16-bit elements (int32_t)
    N         - number of samples

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int32_t outVal[8];
    int32_t inBufTest[16] = {-5,2,-3,4,-1,0,-2,-8,-21,21,10,100, 200, 127,-127,-2};
    int32_t inBuf2[16]=     { 1,2, 3,4, 5,6, 7, 8, 9, 10,-1,-100,-127,127,-7,  0};
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorAdd32(pOutdata, inBufTest, inBuf2, Num);

    // outVal[i] = inBufTest[i] + inBuf2[i] = {-4,4,0,8,4,6,5,0}

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorAdd32(int32_t *outdata, int32_t *indata1, int32_t *indata2, int N);

// *****************************************************************************
/* Function DSP_VectorAddc16:

    void DSP_VectorAddc16(int16_t *outdata, int16_t *indata, int16_t c, int N);

  Summary:
    Calculate the sum of a vector and a constant.

  Description:
    Computes the sum value of each element of (indata + c) and stores it to
    outdata. The number of samples to process is given by the parameter N.
    Data is in a Q15 fractional format.
          outdata[i] filled with N elements of indata[i] + c

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to eight and a multiple of eight.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int16_t)
    indata    - pointer to input array of 16-bit elements (int16_t)
    c         - constant value added to all indata1 elements (int16_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int16_t outVal[8];
    int16_t inBufTest[16] = {-5,2,-3,4,-1,0,-2,-8,-21,21,10,100, 200, 127,-127,-2};
    int16_t constValue =  3;
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorAddc16(pOutdata, inBufTest, constValue, Num);

    // outVal[i] = inBufTest[i] + constValue = {-2,5,0,7,2,3,1,-5}

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorAddc16(int16_t *outdata, int16_t *indata, int16_t c, int N);

// *****************************************************************************
/* Function DSP_VectorAddc32:

    void DSP_VectorAddc32(int32_t *outdata, int32_t *indata, int32_t c, int N);

  Summary:
    Calculate the sum of a vector and a constant.

  Description:
    Computes the sum value of each element of (indata + c) and stores it to
    outdata. The number of samples to process is given by the parameter N.
    Data is in a Q31 fractional format.
         outdata[i] filled with N elements of indata1[i] + c

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int32_t)
    indata    - pointer to input array of 16-bit elements (int32_t)
    c         - constant value added to all indata1 elements (int32_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int32_t outVal[8];
    int32_t inBufTest[16] = {-5,2,-3,4,-1,0,-2,-8,-21,21,10,100, 200, 127,-127,-2};
    int32_t constValue =  3;
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorAddc32(pOutdata, inBufTest, constValue, Num);

    // outVal[i] = inBufTest[i] + constValue = {-2,5,0,7,2,3,1,-5}

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorAddc32(int32_t *outdata, int32_t *indata, int32_t c, int N);

// *****************************************************************************
/* Function DSP_VectorAutocorr16:

    void  DSP_VectorAutocorr16(int16_t *outCorr, int16_t *inVector, int N, int K);

  Summary:
      Computes the Autocorrelation of a Vector.

  Description:
    Calculates the autocorrelation, with a lag of 1 to K, on the first N elements
    of inVector and returns the 16-bit scalar result in outCorr.  The autocorrelation is
    calculated from other statistical equations including mean and variance.
    While in some cases these equations exist inside the DSP library, the functions
    are executed in a serial fashion within this code to provide enhanced performance.
    The unbiased function has the form -

    mean (M) = sum[0..N-1](x(n) / N)
    variance (V) = sum[0..N-1]((x(n) - M)^2) / (N-1)
    autocovariance (ACV)[k] = sum[0..(N-k)]((x(n) - M) * (x(n+k) - M) / (N-k))
    autocorrelation (AC)[k] = CV[k] / V
        where
      N is the number of vector elements, n is the index of those elements
      x(n) is a single element in the input vector
      M is the mean of the N elements of the vector
      k is the lag or series index

    The output of the function will return K elements, and the outCorr array
    should be sized to accept those 16-bit results.  The outputs correspond to
    k=1, k=2, .., k=K delay states.  The function returns a 16-bit value in rounded,
    saturated Q15 format.

    Input values of the vector and output scalar value is Q15 fractional format.
    This format has data that ranges from -1 to 1, and has internal saturation
    limits of those same values.  Some care has been taken to reduce the impact
    of saturation by adding processing steps to effectively complete the processing
    in blocks.  However, in some extreme cases of data variance it is still possible
    to reach the saturation limits.

  Precondition:
      The pointers outdata and indata must be aligned on 4-byte boundaries.
      outCorr must be an array with at least K values.
      N must be greater than or equal to four and a multiple of four.

  Parameters:
    outCorr     - pointer to output array (int16_t)
    inVector    - pointer to source array of elements (int16_t)
    N           - number of samples (int)
    K           - lag value, number of output elements (int)

  Returns:
    None.

  Example:
    <code>

    int  autocvN = 16;                     // N value, number of samples in array
    int  autocvLag = 4;                    // Lag value, output shifts to observe
    int16_t outAC[16]={0};
    int16_t CVIn16[16] = {0x1999, 0xD99A, 0x1000, 0x6000, 0x1999, 0x1999, 0x2666,
         0x3333, 0x1000, 0x6000, 0x1999, 0x1999, 0x2666, 0x3333, 0x1999, 0x0CCC};
//  = { .2, -.3, .125, .75, .2, .2, .3, .4, .125, .75, .2, .2, .3, .4, .2, .1};

    DSP_VectorAutocorr16(outAC, CVIn16, autocvN, autocvLag);

 // outAC = {0xF406, D46C, 0x098F, 0x191A}
 //    = -0.093567, -0.34045, 0.07468, 0.19611

   </code>

  Remarks:
    This function is optimized with microMIPS and M14KCe ASE DSP instructions.
    This function is dependent on the LibQ library, and the _LIBQ_Q16Div specifically.
*/

void  DSP_VectorAutocorr16(int16_t *outCorr, int16_t *inVector, int N, int K);

// *****************************************************************************
/* Function DSP_VectorBexp16:

    int   DSP_VectorBexp16(int16_t *DataIn, int N);

  Summary:
    Computes the maximum binary exponent of a vector.

  Description:
    Calculates the maximum binary exponent on the first N elements of the input
    vector DataIn, and stores the integer result.  The returned value represents
    the potential binary scaling of the vector, and may be used with other functions
    that auto scale their output without saturation.  Inputs are given in Q15
    fractional data format.

  Precondition:
    N must be a multiple of 2 and greater or equal to 2.

  Parameters:
    DataIn    - pointer to input array of 16-bit elements (int16_t)
    N         - number of samples (int)

  Returns:
    Binary exponent [log2 multiplier] (int)

  Example:
    <code>

    int valN = 4;
    int16_t  dummy16[valN]={0x3004, 0x00CC, 0xFC04, 0xFFF0};
 //                         0.375, 0.0062, -0.0311, -0.00049
    int answer;

    answer =   DSP_VectorBexp16(dummy16, valN);

 // answer = 1, maximum binary gain is 2.

   </code>

  Remarks:
    None.
*/

int   DSP_VectorBexp16(int16_t *DataIn, int N);

// *****************************************************************************
/* Function DSP_VectorBexp32:

    int   DSP_VectorBexp32(int32_t *DataIn, int N);

  Summary:
    Computes the maximum binary exponent of a vector.

  Description:
    Calculates the maximum binary exponent on the first N elements of the input
    vector DataIn, and stores the integer result.  The returned value represents
    the potential binary scaling of the vector, and may be used with other functions
    that auto scale their output without saturation.  Inputs are given in Q31
    fractional data format.

  Precondition:
    None.

  Parameters:
    DataIn    - pointer to input array of 16-bit elements (int32_t)
    N         - number of samples (int)

  Returns:
    Binary exponent [log2 multiplier] (int)

  Example:
    <code>

    int valN=4;
    int32_t  datInput32[4]={0xFF000000, 0x07000000,0x000CCCCC, 0x08000000};
 //                         -0.007183, 0.054688, 0.0003906, 0.0625
    int answer32;

    answer32 =   DSP_VectorBexp32(datInput32, valN);

 // answer = 3, maximum binary gain is 8.

   </code>

  Remarks:
    None.
*/

int   DSP_VectorBexp32(int32_t *DataIn, int N);

// *****************************************************************************
/* Function DSP_VectorChkEqu32:

    int DSP_VectorChkEqu32(int32_t *indata1, int32_t *indata2, int N);

  Summary:
    Compares two input vectors, returns an integer '1' if equal, and '0' if not equal.

  Description:
    Compares the first N values of indata1 to the same elements of indata2.  The
    comparison requires that all numbers be in Q31 fractional data format.
    Returns the integer value '1' if all numbers are equal, and '0' if they are not
    equal. N must be greater than or equal to four and a multiple of four, or it will 
    be truncated to the nearest multiple of four.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    indata1   - pointer to input array 1 of elements (int32_t)
    indata2   - pointer to input array 2 of elements (int32_t)
    N         - number of samples (int)

  Returns:
    (int)     - '1' if vectors are equal, '0' if vectors are not equal

  Example:
    <code>

    int     outCheck;
    int     Num = 4;
    int32_t inBufTestA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6
    int32_t inBufTestB[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x80000000, 0x40000000, 0x7FFFFFFF, 0x20000000};
//                  1,     -1,      0.9,    0.8,    -1,   0.5,    1,     0.25

   outCheck = DSP_VectorChkEqu32(inBufTestA, inBufTestB, Num);

// outCheck = 1  // true for first 4 numbers of series

   </code>

  Remarks:
    None.
*/

int DSP_VectorChkEqu32(int32_t *indata1, int32_t *indata2, int N);

// *****************************************************************************
/* Function DSP_VectorCopy:

    void    DSP_VectorCopy(int32_t *outdata, int32_t *indata, int N);

  Summary:
    Copies the elements of one vector to another.

  Description:
    Fills the first N values of an input vector outdata with the elements
    from indata.  N must be a multiple of four and greater than or equal to four or 
    it will be truncated to the nearest multiple of four.  The vector result and 
    the scalar value to fill are both Q31 fractional format.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    outdata   - pointer to destination array of values (int32_t)
    indata    - pointer to source array of elements (int32_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int     Num = 4;
    int32_t inBufTestA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6
    int32_t inBufTestB[8]={0x00000000, 0x7FFFFFFF, 0x40000000, 0x0CCCCCCC,
                               0x40000000, 0x60000000, 0x80000000, 0x20000000};
//                  0,     1,      0.5,    0.1,    0.75,   0.5,    -1,     0.25

   DSP_VectorCopy(inBufTestA, inBufTestB, Num);

// inBufTestA =  {0x00000000, 0x7FFFFFFF, 0x40000000, 0x0CCCCCCC,
//           0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334} // first 4 values copied

   </code>

  Remarks:
    None.
*/

void DSP_VectorCopy(int32_t *outdata, int32_t *indata, int N);

// *****************************************************************************
/* Function DSP_VectorCopyReverse32:

   void   DSP_VectorCopyReverse32(int32_t *outdata, int32_t *indata, int N);

  Summary:
    Reverses the order of elements in one vector and copies them into another.

  Description:
    Fills the first N values of an input vector Outdata with the reverse elements
    from INDATA.  N must be a multiple of 4 and greater than 4 or will be
    truncated to the nearest multiple of 4.  The vectors are both Q31 fractional format.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    outdata   - pointer to destination array of values (int32_t)
    indata    - pointer to source array of elements (int32_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int     Num = 4;
    int32_t inBufTestA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6
    int32_t inBufTestB[8]={0x00000000, 0x7FFFFFFF, 0x40000000, 0x0CCCCCCC,
                               0x40000000, 0x60000000, 0x80000000, 0x20000000};
//                  0,     1,      0.5,    0.1,    0.75,   0.5,    -1,     0.25

   DSP_VectorCopyReverse32(inBufTestA, inBufTestB, Num);

// inBufTestA =  {0x0CCCCCCC, 0x40000000, 0x7FFFFFFF, 0x00000000,
//           0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334}
//   first 4 values copied reverse order

   </code>

  Remarks:
    None.
*/

void DSP_VectorCopyReverse32(int32_t *outdata, int32_t *indata, int N);

// *****************************************************************************
/* Function DSP_VectorDivC:

    void  DSP_VectorDivC(_Q16 *outVector, _Q16 *inVector, _Q16 divisor, int N);

  Summary:
    Divides the first N elements of inVector by a constant divisor, and stores
    the result in outVector.

  Description:
    Divides each element of the first N elements of inVector by a constant, divisor.
    The output is stored to outVector.  Both vectors and the scalar are _Q16 format,
    which is 32-bit data with 15 bits for the integer and 16 bits for the fractional
    portion.
    If values exceed maximum or minimum they will saturate to the maximum
    or minimum respectively.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    This function uses the Div function from the LibQ library.  That
    library must be compiled as part of the project.

  Parameters:
    outdata    - pointer to destination array of elements (_Q16)
    indata     - pointer to source array of elements (_Q16)
    divisor    - scalar divisor for the input vector (_Q16)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int divNum = 4;
    _Q16 divScalar = 0x00020000;  // 2.0
    _Q16 inDivVec[8] = {0x08000000, 0xfffc0000,0x00024000,0x00100000,0x00038000,
           0x00400000,0xfffe0000,0x00058000};
 //     2048, -4, 2.25, 16, 3.5, 64, -2, 5.5
    _Q16 outDivVec[8] = {0};

    DSP_VectorDivC(outDivVec, inDivVec,  divScalar, divNum);

// outDivVec =  0x04000000, 0xFFFFE0000, 0x00012000, 0x00080000, 0, 0, 0, 0
//                  1024.0,        -2.0,      1.125,       16.0, 0, 0, 0, 0
   </code>

  Remarks:
    This function uses the Microchip PIC32MZ LibQ library to function.  The user
    must include that library and header file into the design in order to operate
    this function.  For more information on the Div function see the LibQ documentation
    for _LIBQ_Q16Div.
*/

void DSP_VectorDivC(_Q16 *outVector, _Q16 *inVector, _Q16 divisor, int N);

// *****************************************************************************
/* Function DSP_VectorDotp16:

    int16_t DSP_VectorDotp16(int16_t *indata1, int16_t *indata2, int N, int scale);

  Summary:
    Computes the dot product of two vectors, and scales the output by a
    binary factor.

  Description:
    Calculates the dot product of two input vectors, and scales the output.  
    Function will saturate if it exceeds maximum or minimum values.  Scaling
    is done by binary shifting, after accumulation in a 32 bit register.  All
    calculations are done in Q15 fractional format.
            return = 1/(2^scale) * sum(indata1[i] * indata2[i])

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to eight and a multiple of eight.

  Parameters:
    indata1   - pointer to input array of 16-bit elements (int16_t)
    indata2   - pointer to input array of 16-bit elements (int16_t)
    scale     - number of bits to shift return right (int)
    N         - number of samples (int)

  Returns:
    int16_t  -  scaled output of calculation, Q15 format

  Example:
    <code>

    int16_t inBufMultA[8]={0x7FFF, 0x8000, 0x7333, 0x6666, 0x1999, 0x4000, 0x7FFF, 0xB334};
//                         1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6
    int16_t inBufMultB[8]={0x0CCD, 0x0CCD, 0x4000, 0xC000, 0xE667, 0x4000, 0x0000, 0x0CCD};
//                         0.1,    0.1,    0.5,   -0.5,   -0.2,   0.5,    0,      0.1
    int Num = 8;
    int scaleVal = 2;
    int16_t outScalar;

    int Num = 8;

    outScalar = DSP_VectorDotp16(inBufMultA, inBufMultB, Num, scaleVal);

// outScalar = 1/(2^scaleVal)*(inBufMultA[] dot inBufMultB[]) =
//   (1/4) * (0.1 + -0.1 + 0.45 + -0.4 + -0.04 + 0.25 + 0 + -0.06) = 0.25 * 0.20 = 0.05
//      = (int16_t)0x0666

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

int16_t DSP_VectorDotp16(int16_t *indata1, int16_t *indata2, int N, int scale);

// *****************************************************************************
/* Function DSP_VectorDotp32:

    int32_t DSP_VectorDotp32(int32_t *indata1, int32_t *indata2, int N, int scale);

  Summary:
    Computes the dot product of two vectors, and scales the output by a
    binary factor

  Description:
    Calculates the dot product of two input vectors, and scales the output.  
    Function will saturate if it exceeds maximum or minimum values. Scaling
    is done by binary shifting, after calculation of the result.  All
    calculations are done in Q31 fractional format.
            return = 1/(2^scale) * sum(indata1[i] * indata2[i])

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    indata1   - pointer to input array of 16-bit elements (int32_t)
    indata2   - pointer to input array of 16-bit elements (int32_t)
    scale     - number of bits to shift return right (int)
    N         - number of samples (int)

  Returns:
    int16_t  -  scaled output of calculation, Q31 format

  Example:
    <code>

    int32_t inBufMultA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6
    int32_t inBufMultB[8]={0x0CCCCCCD, 0x0CCCCCCD, 0x40000000, 0xC0000000,
                               0xE6666667, 0x40000000, 0x00000000, 0x0CCCCCCD};
//                  0.1,    0.1,    0.5,   -0.5,   -0.2,   0.5,    0,      0.1
    int Num = 8;
    int scaleVal = 2;
    int32_t outScalar;

    int Num = 8;

    outScalar = DSP_VectorDotp32(inBufMultA, inBufMultB, Num, scaleVal);

// outScalar = 1/(2^scaleVal)*(inBufMultA[] dot inBufMultB[]) =
//   (1/4) * (0.1 + -0.1 + 0.45 + -0.4 + -0.04 + 0.25 + 0 + -0.06) = 0.25 * 0.20 = 0.05
//      = (int32_t)0x06666666

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

int32_t DSP_VectorDotp32(int32_t *indata1, int32_t *indata2, int N, int scale);

// *****************************************************************************
/* Function DSP_VectorExp:

    void    DSP_VectorExp(_Q16 *outVector, _Q16 *inVector, int N);

  Summary:
    Computes the EXP (e^x) of the first N elements of inVector, and stores the
    result in outVector.

  Description:
    Computes the Exp value, e to the power of X, on the first N elements of
    inVector.  The output is stored to outVector.  Both vectors are _Q16 format,
    which is 32-bit data with 15 bits for the integer and 16 bits for the fractional
    portion.  If values exceed maximum or minimum they will saturate to the maximum
    or zero respectively.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    This function uses the Exp and Div functions from the LibQ library.  That
    library must be compiled as part of the project.

  Parameters:
    outdata    - pointer to destination array of elements (_Q16)
    indata     - pointer to source array of elements (_Q16)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int expNum = 4;
    _Q16 inExpVec[8] = {0x00010000, 0xffff0000,0x00020000,0x00030000,0x00038000,
                0x00040000,0xfffe0000,0x00058000};
//          1.0,  -1.0,  2.0,  3.0,   3.5,   4.0,  -2.0,  5.5
    _Q16 outExpVec[8] = {0};

    DSP_VectorExp(outExpVec, inExpVec, expNum);

// outExpVec = 0x0002B7E1, 0x00005E2D, 0x00076399, 0x001415E6, 0, 0, 0, 0
//                2.71828,   0.26787,     7.3891,    20.0855, 0, 0, 0, 0
   </code>

  Remarks:
    Inclusion of the LibQ header file and library is mandatory to use this
    function.
*/

void DSP_VectorExp(_Q16 *outVector, _Q16 *inVector, int N);

// *****************************************************************************
/* Function DSP_VectorFill:

    void        DSP_VectorFill(int32_t *indata, int32_t data, int N);

  Summary:
      Fills an input vector with scalar data.

  Description:
    Fills the first N values of an input vector indata with the value data.  N
    must be a multiple of four and greater than or equal to four or it will be 
    truncated to the nearest multiple of four.  The vector result and the scalar value 
    to fill are both Q31 fractional format.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    indata    - pointer to source array of elements (int32_t)
    data      - scalar value to fill the array (int32_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int32_t fillValue = 0x3FFFFFFF;
    int     Num = 4;
    int32_t inBufTestA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6

    DSP_VectorFill(inBufTestA, fillValue, Num);

// inBufTestA =  {0x3FFFFFFF, 0x3FFFFFFF, 0x3FFFFFFF, 0x3FFFFFFF,
//           0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334} // first 4 values filled

   </code>

  Remarks:
    None.
*/

void DSP_VectorFill(int32_t *indata, int32_t data, int N);

// *****************************************************************************
/* Function DSP_VectorLog10:

    void    DSP_VectorLog10(_Q3_12 *outVector, _Q16 *inVector, int N);

  Summary:
    Computes the Log10(x), of the first N elements of inVector, and
    stores the result in outVector.

  Description:
    Computes the Log10(x) value, on the first N elements of inVector.  The output is
    stored to outVector.  Input vector is _Q16 format, which is 32-bit data with
    15 bits for the integer and 16 bits for the fractional portion.  The output
    vector is reduced resolution Q3.12 format, which is a 16-bit integer format
    with 12 bits representing the fractional resolution.
    If values exceed maximum or minimum they will saturate to the maximum
    or zero respectively.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    This function uses the Log10 function from the LibQ library.  That
    library must be compiled as part of the project.

  Parameters:
    outdata    - pointer to destination array of elements (_Q16)
    indata     - pointer to source array of elements (_Q3_12)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

int logNum = 4;
 _Q16 inLogVal[8] = {0x40000000, 0xffff0000,0x00020000,0x00100000,0x00038000,
                        0x00400000,0xfffe0000,0x00058000};
//          16384.0,  -1.0,  2.0,  16.0,   3.5,   64.0,  -2.0,  5.5
   _Q3_12 outLogVal[8] = {0};

    DSP_VectorLog10(outLogVal, inLogVal,  logNum);

// outLogVal =    0x436E,        0x8000,    0x04D1,   0x1344, 0, 0, 0, 0
//                 4.2144,  sat negative,   0.3010,   1.2041, 0, 0, 0, 0
   </code>

  Remarks:
    This function uses the Microchip PIC32MZ LibQ library to function.  The user
    must include that library and header file into the design in order to operate
    this function.  For more information on the Log10 function see the LibQ documentation
    for _LIBQ_Q3_12_log10_Q16.
    A negative number input will return a saturated negative value (0x8000).
*/

void DSP_VectorLog10(_Q3_12 *outVector, _Q16 *inVector, int N);

// *****************************************************************************
/* Function DSP_VectorLog2:

    void    DSP_VectorLog2(_Q5_10 *outVector, _Q16 *inVector, int N);

  Summary:
    Computes the Log2(x) of the first N elements of inVector, and stores the
    result in outVector.

  Description:
    Computes the Log2 value, where log2(x) = ln(x) * log2(e), on the first N elements of
    inVector.  The output is stored to outVector.  Input vector is _Q16 format,
    which is 32-bit data with 15 bits for the integer and 16 bits for the fractional
    portion.  The output vector is reduced resolution Q5.10 format, which is a
    16-bit integer format with 10 bits representing the fractional resolution.
    If values exceed maximum or minimum they will saturate to the maximum
    or zero respectively.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    This function uses the Log2 function from the LibQ library.  That
    library must be compiled as part of the project.

  Parameters:
    outdata    - pointer to destination array of elements (_Q16)
    indata     - pointer to source array of elements (_Q5_10)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

   int log2Num = 4;
   _Q16 inLog2Val[8] = {0x40000000, 0xffff0000,0x00020000,0x00030000,0x00038000,
               0x00040000,0xfffe0000,0x00058000};
//          16384.0,  -1.0,  2.0,  3.0,   3.5,   4.0,  -2.0,  5.5
   _Q5_10 outLog2Val[8] = {0};

    DSP_VectorLog2(outLog2Val, inLog2Val,  log2Num);

// outLog2Val =    0x3800,        0x8000,   0x0400,   0x0657, 0, 0, 0, 0
//                  14.0,  sat negative,      1.0,    1.585, 0, 0, 0, 0
   </code>

  Remarks:
    This function uses the Microchip PIC32MZ LibQ library to function.  The user
    must include that library and header file into the design in order to operate
    this function.  For more information on the Log2 function see the LibQ documentation
    for _LIBQ_Q5_10_log2_Q16.
    A negative number input will return a saturated negative value (0x8000).
*/

void DSP_VectorLog2(_Q5_10 *outVector, _Q16 *inVector, int N);

// *****************************************************************************
/* Function DSP_VectorLn:

    void    DSP_VectorLn(_Q4_11 *outVector, _Q16 *inVector, int N);

  Summary:
    Computes the Natural Log, Ln(x), of the first N elements of inVector, and
    stores the result in outVector.

  Description:
    Computes the Ln(x) value, on the first N elements of inVector.  The output is
    stored to outVector.  Input vector is _Q16 format, which is 32-bit data with
    15 bits for the integer and 16 bits for the fractional portion.  The output
    vector is reduced resolution Q4.11 format, which is a 16-bit integer format
    with 11 bits representing the fractional resolution.
    If values exceed maximum or minimum they will saturate to the maximum
    or zero respectively.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    This function uses the Ln function from the LibQ library.  That
    library must be compiled as part of the project.

  Parameters:
    outdata    - pointer to destination array of elements (_Q16)
    indata     - pointer to source array of elements (_Q4_11)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

   int lnNum = 4;
 _Q16 inLnVal[8] = {0x40000000, 0xffff0000,0x00020000,0x00100000,0x00038000,
           0x00400000,0xfffe0000,0x00058000};
//          16384.0,  -1.0,  2.0,  16.0,   3.5,   64.0,  -2.0,  5.5
   _Q4_11 outLnVal[8] = {0};

   DSP_VectorLn(outLnVal, inLnVal,  lnNum);

// outLnVal =     0x4DA2,        0x8000,   0x058C,   0x162E, 0, 0, 0, 0
//                 9.704,  sat negative,   0.6934,    2.772, 0, 0, 0, 0
   </code>

  Remarks:
    This function uses the Microchip PIC32MZ LibQ library to function.  The user
    must include that library and header file into the design in order to operate
    this function.  For more information on the Ln function see the LibQ documentation
    for _LIBQ_Q4_11_ln_Q16.
    A negative number input will return a saturated negative value (0x8000).
*/

void DSP_VectorLn(_Q4_11 *outVector, _Q16 *inVector, int N);

// *****************************************************************************
/* Function DSP_VectorMax32:

    int32_t   DSP_VectorMax32(int32_t *indata, int N);

  Summary:
    Returns the maximum value of a vector.

  Description:
    Returns the highest value of the first N elements of the vector indata.  The
    comparison requires that all numbers be in Q31 fractional data format.  N
    must be greater than or equal to four and a multiple of four, or it will be 
    truncated to the nearest multiple of four.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and multiple of four.

  Parameters:
    indata     - pointer to input array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    (int32_t)    - maximum value within the vector, Q31 format

  Example:
    <code>

    int32_t   outCheck;
    int     Num = 4;
    int32_t inBufTestA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6

   outCheck = DSP_VectorMax32(inBufTestA, Num);

// outCheck = 0x7FFFFFFF  // first 4 values

   </code>

  Remarks:
    None.
*/

int32_t DSP_VectorMax32(int32_t *indata, int N);

// *****************************************************************************
/* Function DSP_VectorMaxIndex32:

    int     DSP_VectorMaxIndex32(int32_t *indata, int N);

  Summary:
    Returns the index of the maximum value of a vector.

  Description:
    Returns the index of the highest value of the first N elements of the vector
    indata.  The comparison requires that all numbers be in Q31 fractional data
    format.  N must be greater than or equal to four and a multiple of four, or 
    it will be truncated to the nearest multiple of four.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    indata     - pointer to source array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    int        - index of the position of the maximum array element

  Example:
    <code>

    int     indexValue;
    int     Num = 8;
    int32_t inBufTestA[8]={0x3FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  0.5,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6

    indexValue = DSP_VectorMaxIndex32(inBufTestA, Num);

// returnValue =  6 (position corresponding to 0x7FFFFFFF)

   </code>

  Remarks:
    Index values range from 0 .. (n-1).
*/

int DSP_VectorMaxIndex32(int32_t *indata, int N);

// *****************************************************************************
/* Function DSP_VectorMean32:

    int32_t       DSP_VectorMean32(int32_t *indata, int N);

  Summary:
    Calculates the mean average of an input vector.

  Description:
    Calculates the mean average of the first N elements of the vector indata.
    The values of indata1 are in Q31 fractional format.  The value N must be
    greater than or equal to four and a multiple of four, or it will be truncated 
    to the nearest multiple of four.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    indata     - pointer to source array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    int32_t      - mean average value of the vector

  Example:
    <code>

    int32_t   returnValue;
    int     Num = 4;
    int32_t inBufTestA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6

    returnValue = DSP_VectorMean32(inBufTestA, Num);

// returnValue =  0x36666666 = (1-1+0.9+0.8)/4 = 0.425

   </code>

  Remarks:
    None.
*/

int32_t DSP_VectorMean32(int32_t *indata1, int N);

// *****************************************************************************
/* Function DSP_VectorMin32:

    int32_t   DSP_VectorMin32(int32_t *indata, int N);

  Summary:
    Returns the minimum value of a vector.

  Description:
    Returns the lowest value of the first N elements of the vector indata.  The
    comparison requires that all numbers be in Q31 fractional data format.  N
    must be greater than or equal to four and a multiple of four, or it will be
    truncated to the nearest multiple of four.

  Precondition:
      The pointers outdata and indata must be aligned on 4-byte boundaries.
      N must be greater than or equal to four and multiple of four.

  Parameters:
    indata     - pointer to input array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    (int32_t)    - minimum value within the vector, Q31 format

  Example:
    <code>

    int32_t   outCheck;
    int     Num = 4;
    int32_t inBufTestA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6

   outCheck = DSP_VectorMin32(inBufTestA, Num);

// outCheck = 0x80000000  // first 4 values

   </code>

  Remarks:
    None.
*/

int32_t DSP_VectorMin32(int32_t *input, int N);

// *****************************************************************************
/* Function DSP_VectorMinIndex32:

    int     DSP_VectorMinIndex32(int32_t *indata, int N);

  Summary:
    Returns the index of the minimum value of a vector.

  Description:
    Returns the relative position index of the lowest value of the first N
    elements of the vector indata. The comparison requires that all numbers be
    in Q31 fractional data format.  N must be greater than or equal to four and a 
    multiple of four, or it will be truncated to the nearest multiple of four.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    indata     - pointer to source array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    int32_t      - mean average value of the vector

  Example:
    <code>

    int     indexValue;
    int     Num = 8;
    int32_t inBufTestA[8]={0x3FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  0.5,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6

    indexValue = DSP_VectorMinIndex32(inBufTestA, Num);

// returnValue =  1 (position corresponding to 0x80000000)

   </code>

  Remarks:
    Index values range from 0 .. (n-1).
*/

int DSP_VectorMinIndex32(int32_t *indata, int N);

// *****************************************************************************
/* Function DSP_VectorMul16:

    void DSP_VectorMul16(int16_t *outdata, int16_t *indata1, int16_t *indata2, int N);

  Summary:
    Multiplication of a series of numbers in one vector to another vector.

  Description:
    Multiples the value of each element of indata1 * indata2 and
    stores it to outdata. The number of samples to process is given by the
    parameter N. Data is in the Q15 fractional format.
          outdata[i] filled with N elements of indata1[i] * indata2[i]

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to eight and a multiple of eight.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int16_t)
    indata1   - pointer to input array of 16-bit elements (int16_t)
    indata2   - pointer to input array of 16-bit elements (int16_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int16_t outVal[8];
    int16_t inBufMultA[8]={0x7FFF, 0x8000, 0x7333, 0x6666, 0x1999, 0x4000, 0x7FFF, 0xB334};
//                         1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6
    int16_t inBufMultB[8]={0x0CCD, 0x0CCD, 0x4000, 0xC000, 0xE667, 0x4000, 0x0000, 0x0CCD};
//                         0.1,    0.1,    0.5,   -0.5,   -0.2,   0.5,    0,      0.1
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorMul16(pOutdata, inBufMultA, inBufMultB, Num);

// outVal[i] = inBufTest[i] * inBuf2[i] =
//    {0x0CCD, 0xF333, 0x399A, 0xCCCD, 0xFAE2, 0x2000, 0x0000, 0xF852}
//      0.1,   -0.1,   0.45,  -0.4,   -0.04,   0.25,    0,     -0.06

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorMul16(int16_t *outdata, int16_t *indata1, int16_t *indata2, int N);

// *****************************************************************************
/* Function DSP_VectorMul32:

    void DSP_VectorMul32(int32_t *outdata, int32_t *indata1, int32_t *indata2, int N);

  Summary:
    Multiplication of a series of numbers in one vector to another vector.

  Description:
    Multiples the value of each element of indata1 * indata2 and stores it to 
    outdata. The number of samples to process is given by the parameter N. Data 
    is in the Q31 fractional format.
          outdata[i] filled with N elements of indata1[i] * indata2[i]

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int32_t)
    indata1   - pointer to input array of 16-bit elements (int32_t)
    indata2   - pointer to input array of 16-bit elements (int32_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int32_t outVal[8];
    int32_t inBufMultA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6
    int32_t inBufMultB[8]={0x0CCCCCCD, 0x0CCCCCCD, 0x40000000, 0xC0000000,
                               0xE6666667, 0x40000000, 0x00000000, 0x0CCCCCCD};
//                  0.1,    0.1,    0.5,   -0.5,   -0.2,   0.5,    0,      0.1
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorMul32(pOutdata, inBufMultA, inBufMultB, Num);

// outVal[i] = inBufTest[i] * inBuf2[i] =
//    {0x0CCCCCCD, 0xF3333333, 0x3999999A, 0xCCCCCCCD, 0xFAE147AE,
//                                          0x20000000, 0x00000000, 0xF851EB86}
//      0.1,   -0.1,   0.45,  -0.4,   -0.04,   0.25,    0,     -0.06

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorMul32(int32_t *outdata, int32_t *indata1, int32_t *indata2, int N);

// *****************************************************************************
/* Function DSP_VectorMulc16:

    void DSP_VectorMulc16(int16_t *outdata, int16_t *indata, int16_t c, int N);

  Summary:
    Multiplication of a series of numbers in one vector to a scalar value.

  Description:
    Multiples the value of each element of indata1 * c and stores it to
    outdata. The number of samples to process is given by the parameter N.
    Data is in a Q15 fractional format.
          outdata[i] filled with N elements of indata[i] * c

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to eight and a multiple of eight.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int16_t)
    indata    - pointer to input array of 16-bit elements (int16_t)
    c         - scalar multiplicand (int16_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int16_t outVal[8];
    int16_t inBufMultA[8]={0x7FFF, 0x8000, 0x7333, 0x6666, 0x1999, 0x4000, 0x0000, 0xB334};
//                         1,     -1,      0.9,    0.8,    0.2,   0.5,    0,     -0.6
    int16_t constValue = 0x4000;
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorMulc16(pOutdata, inBufMultA, constValue, Num);

// outVal[i] = inBufTest[i] * constValue =
//    {0x4000, 0xC000, 0x399A, 0x3333, 0x1999, 0x2000, 0x0000, 0xD99A}
//      0.5,   -0.5,   0.45,    0.4,    0.1,   0.25,    0,     -0.3

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorMulc16(int16_t *outdata, int16_t *indata, int16_t c, int N);

// *****************************************************************************
/* Function DSP_VectorMulc32:

    void DSP_VectorMulc32(int32_t *outdata, int32_t *indata, int32_t c, int N);

  Summary:
    Multiplication of a series of numbers in one vector to a scalar value.

  Description:
    Multiples the value of each element of indata * c and stores it to
    outdata. The number of samples to process is given by the parameter N.
    Data is in a Q31 fractional format.
          outdata[i] filled with N elements of indata[i] * c

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int32_t)
    indata    - pointer to input array of 16-bit elements (int32_t)
    c         - scalar multiplicand (int32_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int32_t outVal[8];
    int32_t inBufMultA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x00000000, 0xB3333334};
//                    1,    -1,    0.9,    0.8,    0.2,   0.5,    1,    -0.6
    int32_t constValue = 0x4000;
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorMulc32(pOutdata, inBufMultA, constValue, Num);

// outVal[i] = inBufTest[i] * constValue =
//    {0x40000000, 0xC0000000, 0x3999999A, 0x33333333, 0x19999999,
//                                           0x20000000, 0x00000000, 0xD999999A}
//      0.5,   -0.5,   0.45,    0.4,    0.1,   0.25,    0,     -0.3

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorMulc32(int32_t *outdata, int32_t *indata, int32_t c, int N);

// *****************************************************************************
/* Function DSP_VectorNegate:

    void        DSP_VectorNegate(int32_t *outdata, int32_t *indata, int N);

  Summary:
    Inverses the sign (negates) the elements of a vector.

  Description:
    Sign inversion of the first N values of an indata are assigned to outdata.
    N must be a multiple of four and greater than or equal to four or it will be 
    truncated to the nearest multiple of four.  The vector result and the scalar
    value to fill are both Q31 fractional format.

  Precondition:
      The pointers outdata and indata must be aligned on 4-byte boundaries.
      N must be greater than or equal to four and a multiple of four.

  Parameters:
    outdata    - pointer to destination array of elements (int32_t)
    indata     - pointer to source array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int     Num = 4;
    int32_t inBufTestA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6
    int32_t outBufTest[8]={0x0CCCCCCD, 0x0CCCCCCD, 0x40000000, 0xC0000000,
                               0xE6666667, 0x40000000, 0x00000000, 0x0CCCCCCD};
//                  0.1,    0.1,    0.5,   -0.5,   -0.2,   0.5,    0,      0.1;

   DSP_VectorNegate(outBufTest, inBufTestA, Num);

// inBufTestA =  {0x80000000, 0x7FFFFFFF, 0x8CCCCCCD, 0x9999999A,
//           0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334} // first 4 values neg
//              -1,   1,   -0.9,   -0.8,   -0.2,   0.5,   0,   0.1

   </code>

  Remarks:
    None.
*/

void DSP_VectorNegate(int32_t *outdata, int32_t *indata, int N);

// *****************************************************************************
/* Function DSP_VectorRecip:

    void    DSP_VectorRecip(_Q16 *outVector, _Q16 *inVector, int N);

  Summary:
    Computes the reciprocal (1/x) of the first N elements of inVector, and
    stores the result in outVector.

  Description:
    Computes the reciprocal (1/x) on the first N elements of inVector.  The output is
    stored to outVector.  Both vectors are _Q16 format, which is 32-bit data with
    15 bits for the integer and 16 bits for the fractional portion.
    If values exceed maximum or minimum they will saturate to the maximum
    or minimum respectively.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    This function uses the Div function from the LibQ library.  That
    library must be compiled as part of the project.

  Parameters:
    outdata    - pointer to destination array of elements (_Q16)
    indata     - pointer to source array of elements (_Q16)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

   int recNum = 4;
   _Q16 inRecVec[8] = {0x08000000, 0xfffc0000,0x00020000,0x00100000,0x00038000,
                   0x00400000,0xfffe0000,0x00058000};
//            2048.0, -4.0, 2.0, 16.0, 3.5, 64.0, -2.0, 5.5
   _Q16 outRecVec[8] = {0};

    DSP_VectorRecip(outRecVec, inRecVec,  recNum);

// outRecVec =  0x00000020, 0xFFFFC0000, 0x00008000, 0x00001000, 0, 0, 0, 0
//                0.000488,       -0.25,        0.5,     0.0625, 0, 0, 0, 0
   </code>

  Remarks:
    This function uses the Microchip PIC32MZ LibQ library to function.  The user
    must include that library and header file into the design in order to operate
    this function.  For more information on the Div function see the LibQ documentation
    for _LIBQ_Q16Div.
    A value of zero in the array will not cause an error, but will return 0.
*/

void DSP_VectorRecip(_Q16 *outVector, _Q16 *inVector, int N);

// *****************************************************************************
/* Function DSP_VectorRMS16:

    int16_t     DSP_VectorRMS16(int16_t *inVector, int N);

  Summary:
    Computes the root mean square (RMS) value of a vector.

  Description:
    Computes the root mean square value of the first N values of inVector.  Both
    input and output are Q15 fractional values.  The function will saturate if
    maximum or minimum values are exceeded.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and multiple of four.

  Parameters:
    indata    - pointer to input array of 16-bit elements (int16_t)
    N         - number of samples (int)

  Returns:
    int16_t  -  RMS function output, Q15 format

  Example:
    <code>

    int16_t vecRMSIn[32]={0x1999, 0xD99A, 0x4000, 0x2666,0x1999,0x1999,0x2666, 0x3333};
// 0.2, -0.3, 0.5, 0.3, 0.2, 0.2, 0.3, 0.4
    int16_t RMSOut=0;
    int Nrms = 8;

    RMSOut = DSP_VectorRMS16(vecRMSIn, Nrms);

// RMSOut = 0x287C (= 0.31628)

    </code>

  Remarks:
    This function is optimized with microMIPS and M14KCe ASE DSP instructions.
    This function is dependent on the LibQ library, and uses the _LIBQ_Q16Sqrt
    external function call.
*/

int16_t DSP_VectorRMS16(int16_t *inVector, int N);

// *****************************************************************************
/* Function DSP_VectorShift:

    void    DSP_VectorShift(int32_t *outdata, int32_t *indata, int N, int shift);

  Summary:
    Shifts the data index of an input data vector.

  Description:
    Shifts N data elements of indata to outdata, with an index change of shift.
    The amount of data shifted includes zero padding for the first (shift)
    elements if shift is positive.  The vector size of indata and outdata need
    not be the same, however, N must not exceed either array size.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must not exceed the amount of elements in the source array.
    shift must not exceed the number of elements in the destination array.


  Parameters:
    outdata    - pointer to destination array of elements (int32_t)
    indata     - pointer to source array of elements (int32_t)
    N          - number of samples (int)
    shift      - number of indexes to shift (int)

  Returns:
    None.

  Example:
    <code>

    int shiftValue = 3;
    int     Num = 8;
    int32_t inBufTestA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6
    int32_t inBufTestB[8]={0x80000000, 0x7FFFFFFF, 0x40000000, 0x0CCCCCCC,
                               0x40000000, 0x60000000, 0x80000000, 0x20000000};
//                  -1,     1,      0.5,    0.1,    0.75,   0.5,    -1,     0.25

    DSP_VectorShift(inBufTestA, inBufTestB, Num, shiftValue);

// inBufTestA =  {0x00000000, 0x00000000, 0x00000000, 0x80000000,
//           0x7FFFFFFF, 0x40000000, 0x0CCCCCCC, 0x40000000} // shifted 3 positive

   </code>

  Remarks:
    Destination array values shift to left (relative to the input vector) when
    shift is positive (back filled with zeros) and shift to the right when shift is
    negative.  The total amount of values copied to the destination array is the
    length of N less the shift amount.
*/

void DSP_VectorShift(int32_t *outdata, int32_t *indata, int N, int shift);

// *****************************************************************************
/* Function DSP_VectorStdDev16:

    int16_t     DSP_VectorStdDev16(int16_t *inVector, int N);

  Summary:
    Computes the Standard Deviation of a Vector.

  Description:
    Calculates the standard deviation on the first N elements of inVector and returns
    the 16-bit scalar result.  The standard deviation is the square root of the
    variance, which is a measure of the delta from mean values. The mean value of
    the vector is computed in the process.  The function has the form -

    StdDev = SQRT(sum[0..N]((x(i) - M(N))^2) / (N-1))
        where
    N is the number of vector elements
    x(i) is a single element in the vector
    M(N) is the mean of the N elements of the vector

    Input values of the vector and output scalar value is Q15 fractional format.
    This format has data that ranges from -1 to 1, and has internal saturation
    limits of those same values.  Some care has been taken to reduce the impact
    of saturation by adding processing steps to effectively complete the processing
    in blocks.  However, in some extreme cases of data variance it is still possible
    to reach the saturation limits.

  Precondition:
      The pointers inVector must be aligned on 4-byte boundaries.
      N must be greater than or equal to four and a multiple of four.
      Dependent on use of the LibQ library.

  Parameters:
    inVector   - pointer to source array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    int16_t    - Standard Deviation of N selected elements

  Example:
    <code>

    int16_t vecStDevIn[32]={0x4000, 0xD99A, 0x1000, 0x6000,0x1999,0x1999,0x2666, 0x3333};
            // .2, -.3, .125, .75, .2, .2, .3, .4
    int16_t StDevOut, Var16Out;
    int Nstdev = 4;

    StDevOut = DSP_VectorStdDev16(vecStDevIn, Nstdev);
            // StDevOut = 0x3A9E (= 0.45797)

   </code>

  Remarks:
    The input vector elements number, N, must be at least 4 and a multiple of 4.
    This function is optimized with microMIPS and M14KCe ASE DSP instructions.
    This function is dependent on the LibQ library, and the _LIBQ_Q16Sqrt specifically.
*/

int16_t DSP_VectorStdDev16(int16_t *inVector, int N);

// *****************************************************************************
/* Function DSP_VectorSqrt:

    void        DSP_VectorSqrt(_Q16 *outVector, _Q16 *inVector, int N);

  Summary:
    Computes the square root of the first N elements of inVector, and
    stores the result in outVector.

  Description:
    Computes the Sqrt(x) on the first N elements of inVector.  The output is
    stored to outVector.  Both vectors are _Q16 format, which is 32-bit data with
    15 bits for the integer and 16 bits for the fractional portion.
    If values exceed maximum or minimum they will saturate to the maximum
    or zero respectively.

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    This function uses the Sqrt function from the LibQ library.  That
    library must be compiled as part of the project.

  Parameters:
    outdata    - pointer to destination array of elements (_Q16)
    indata     - pointer to source array of elements (_Q16)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

int sqrtNum = 4;
 _Q16 inSqrtVec[8] = {0x40000000, 0xffff0000,0x00020000,0x00100000,0x00038000,
               0x00400000,0xfffe0000,0x00058000};
//          16384.0,  -1.0,  2.0,  16.0,   3.5,   64.0,  -2.0,  5.5
   _Q16 outSqrtVec[8] = {0};

   DSP_VectorSqrt(outSqrtVec, inSqrtVec,  sqrtNum);

// outSqrtVec =  0x00800000, 0x00FFFF80, 0x00016A0A, 0x00040000, 0, 0, 0, 0
//                 128.0,  sat negative,   1.41422,      4.0,    0, 0, 0, 0
   </code>

  Remarks:
    This function uses the Microchip PIC32MZ LibQ library to function.  The user
    must include that library and header file into the design in order to operate
    this function.  For more information on the Sqrt function see the LibQ documentation
    for _LIBQ_Q16Sqrt.
    A negative number input will return a saturated value (0x00FFFFxx).
*/

void DSP_VectorSqrt(_Q16 *outVector, _Q16 *inVector, int N);

// *****************************************************************************
/* Function DSP_VectorSub16:

    void DSP_VectorSub16(int16_t *outdata, int16_t *indata1, int16_t *indata2, int N);

  Summary:
    Calculate the difference of two vectors.

  Description:
    Computes the difference value of each element of indata1 - indata2 and
    stores it to outdata. The number of samples to process is given by the
    parameter N. Data is in a Q15 fractional format.
         outdata[i] filled with N elements of indata1[i] - indata2[i]

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to eight and a multiple of eight.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int16_t)
    indata1   - pointer to input array of 16-bit elements (int16_t)
    indata2   - pointer to input array of 16-bit elements (int16_t)
    N         - number of samples (int)

  Returns:
     None.

  Example:
    <code>

    int16_t *pOutdata;
    int16_t outVal[8];
    int16_t inBufTest[16] = {-5,2,-3,4,-1,0,-2,-8,-21,21,10,100, 200, 127,-127,-2};
    int16_t inBuf2[16]=     { 1,2, 3,4, 5,6, 7, 8, 9, 10,-1,-100,-127,127,-7,  0};
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorSub16(pOutdata, inBufTest, inBuf2, Num);

    // outVal[i] = inBufTest[i] - inBuf2[i] = {-6,0,-6,0,-6,-6,-9,-16}

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorSub16(int16_t *outdata, int16_t *indata1, int16_t *indata2, int N);

// *****************************************************************************
/* Function DSP_VectorSub32:

    void DSP_VectorSub32(int32_t *outdata, int32_t *indata1, int32_t *indata2, int N);

  Summary:
    Calculate the difference of two vectors.

  Description:
    Computes the difference value of each element of indata1 - indata2 and
    stores it to outdata. The number of samples to process is given by the
    parameter N. Data is in a Q31 fractional format.
         outdata[i] filled with N elements of indata1[i] - indata2[i]

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and a multiple of four.

  Parameters:
    outdata   - pointer to output array of 16-bit elements (int32_t)
    indata1   - pointer to input array of 16-bit elements (int32_t)
    indata2   - pointer to input array of 16-bit elements (int32_t)
    N         - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int16_t *pOutdata;
    int32_t outVal[8];
    int32_t inBufTest[16] = {-5,2,-3,4,-1,0,-2,-8,-21,21,10,100, 200, 127,-127,-2};
    int32_t inBuf2[16]=     { 1,2, 3,4, 5,6, 7, 8, 9, 10,-1,-100,-127,127,-7,  0};
    int Num = 8;

    pOutdata = &outVal;

    DSP_VectorSub32(pOutdata, inBufTest, inBuf2, Num);

    // outVal[i] = inBufTest[i] - inBuf2[i] = {-6,0,-6,0,-6,-6,-9,-16}

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

void DSP_VectorSub32(int32_t *outdata, int32_t *indata1, int32_t *indata2, int N);

// *****************************************************************************
/* Function DSP_VectorSumSquares16:

    int16_t DSP_VectorSumSquares16(int16_t *indata, int N, int scale);

  Summary:
    Computes the sum of squares of a vector, and scales the output by a
    binary factor.

  Description:
    Calculates the sum of the squares of each element of an input vector, and
    scales the output.  Function will saturate if it exceeds maximum or minimum 
    values. Scaling is done by binary shifting, after accumulation in a 32 bit
    register. All calculations are done in Q15 fractional format.
            return = 1/(2^scale) * sum(indata[i]^2)

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to eight and a multiple of eight.

  Parameters:
    indata    - pointer to input array of 16-bit elements (int16_t)
    scale     - number of bits to shift return right (int)
    N         - number of samples (int)

  Returns:
    int16_t  -  scaled output of calculation, Q15 format

  Example:
    <code>

    int16_t inBufMultA[8]={0x7FFF, 0x8000, 0x7333, 0x6666, 0x1999, 0x4000, 0x7FFF, 0xB334};
//                         1,     -1,      0.9,    0.8,    0.2,   0.5,    1,     -0.6
    int Num = 8;
    int scaleVal = 3;
    int16_t outScalar;

    outScalar = DSP_VectorSumSquares16(inBufMultA, Num, scaleVal);

// outScalar = 1/(2^scaleVal)* sum(inBufMultA[i]^2) =
//   (1/8) * (1 + 1 + 0.81 + 0.64 + 0.04 + 0.25 + 1 + 0.36) = 0.125 * 5.1 = 0.6375
//      = (int16_t)0x5199

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

int16_t DSP_VectorSumSquares16(int16_t *indata, int N, int scale);

// *****************************************************************************
/* Function DSP_VectorSumSquares32:

    int32_t DSP_VectorSumSquares32(int32_t *indata, int N, int scale);

  Summary:
    Computes the sum of squares of a vector, and scales the output by a
    binary factor.

  Description:
    Calculates the sum of the squares of each element of an input vector, and
    scales the output. The function will saturate if it exceeds maximum or 
    minimum values. Scaling is done by binary shifting, after calculation of the
    results. All calculations are done in Q31 fractional format.
            return = 1/(2^scale) * sum(indata[i]^2)

  Precondition:
    The pointers outdata and indata must be aligned on 4-byte boundaries.
    N must be greater than or equal to four and multiple of four.

  Parameters:
    indata    - pointer to input array of 16-bit elements (int32_t)
    scale     - number of bits to shift return right (int)
    N         - number of samples (int)

  Returns:
    int32_t  -  scaled output of calculation, Q15 format

  Example:
    <code>

    int32_t inBufMultA[8]={0x7FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x00000000, 0xB3333334};
//                    1,    -1,    0.9,    0.8,    0.2,   0.5,    1,    -0.6
    int Num = 8;
    int scaleVal = 3;
    int32_t outScalar;

    outScalar = DSP_VectorSumSquares32(inBufMultA, Num, scaleVal);

// outScalar = 1/(2^scaleVal)* sum(inBufMultA[i]^2) =
//   (1/8) * (1 + 1 + 0.81 + 0.64 + 0.04 + 0.25 + 1 + 0.36) = 0.125 * 5.1 = 0.6375
//      = (int32_t)0x51999999

    </code>

  Remarks:
    This must be assembled with .set microMIPS.
*/

int32_t DSP_VectorSumSquares32(int32_t *indata, int N, int scale);

// *****************************************************************************
/* Function DSP_VectorVari16:

    int16_t     DSP_VectorVari16(int16_t *inVector, int N);

  Summary:
    Computes the variance of N elements of a Vector.

  Description:
    Calculates the variance on the first N elements of inVector and returns the
    16-bit scalar result.  The variance is a measure of the delta from mean values,
    and the mean value of the vector is computed in the process.  The function has
    the form -

    var = sum[0..N]((x(i) - M(N))^2) / (N-1)
        where
    N is the number of vector elements
    x(i) is a single element in the vector
    M(N) is the mean of the N elements of the vector

    Input values of the vector and output scalar value is Q15 fractional format.
    This format has data that ranges from -1 to 1, and has internal saturation
    limits of those same values.  Some care has been taken to reduce the impact
    of saturation by adding processing steps to effectively complete the processing
    in blocks.  However, in some extreme cases of data variance it is still possible
    to reach the saturation limits.

  Precondition:
      The pointers inVector must be aligned on 4-byte boundaries.
      N must be greater than or equal to four and a multiple of four.

  Parameters:
    inVector   - pointer to source array of elements (int16_t)
    N          - number of samples (int)

  Returns:
    int16_t    - Variance of N selected elements

  Example:
    <code>

   int16_t vecStDevIn[32]={0x4000, 0xD99A, 0x1000, 0x6000,0x1999,0x1999,0x2666, 0x3333};
            // .2, -.3, .125, .75, .2, .2, .3, .4
   int16_t Var16Out;
   int Nvar = 4;

   Var16Out= DSP_VectorVari16(vecStDevIn, Nvar);  // 16-bit variance function
            // Var16Out = 0x1AD8 (= 0.20974)

   </code>

  Remarks:
    The input vector elements number, N, must be at least 4 and a multiple of 4.
    This function is optimized with microMIPS and M14KCe ASE DSP instructions.
*/

int16_t DSP_VectorVari16(int16_t *inVector, int N);

// *****************************************************************************
/* Function DSP_VectorVariance:

    int32_t     DSP_VectorVariance(int32_t *inVector, int N);

  Summary:
    Computes the variance of N elements of inVector.

  Description:
    Calculates the variance on the first N elements of inVector and returns the
    32-bit scalar result.  The variance is a measure of the delta from mean values,
    and the mean value of the vector is computed in the process.  The function has
    the form -

    var = sum[0..N]((x(i) - M(N))^2) / (N-1)
        where
    N is the number of vector elements
    x(i) is a single element in the vector
    M(N) is the mean of the N elements of the vector

    Input values of the vector and output scalar value is Q31 fractional format.
    This format has data that ranges from -1 to 1, and has internal saturation
    limits of those same values.  Some care has been taken to reduce the impact
    of saturation by adding processing steps to effectively complete the processing
    in blocks.  However, in some extreme cases of data variance it is still possible
    to reach the saturation limits.

  Precondition:
      The pointers inVector must be aligned on 4-byte boundaries.
      N must be greater than or equal to four and a multiple of four.

  Parameters:
    inVector   - pointer to source array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    int32_t    - Variance of N selected elements

  Example:
    <code>

    int varN = 8;

    int32_t inVarVec[8] = {0xE6666667, 0x40000000,0x40000000,0x0CCCCCCC,
               0x00000000,0x59999999,0x20000000,0xC0000000};
 //     -0.2, 0.5, 0.5, 0.1, 0, 0.7, 0.25, -0.5
    int32_t outVar = 0;

    outVar = DSP_VectorVariance(inVarVec, varN);

 // outVar == 0x1490D2A6, = 0.1606696

   </code>

  Remarks:
    The input vector elements number, N, must be at least 4 and a multiple of 4.
    This function is optimized with microMIPS and M14KCe ASE DSP instructions.
*/

int32_t DSP_VectorVariance(int32_t *inVector, int N);

// *****************************************************************************
/* Function DSP_VectorZeroPad:

    void    DSP_VectorZeroPad(int32_t *indata, int N);

  Summary:
      Fills an input vector with zeros.

  Description:
    Fills the first N values of an input vector indata with the value zero.  N
    must be a multiple of four and greater than or equal to four or it will be truncated 
    to the nearest multiple of four.  The vector result is in Q31 fractional format.

  Precondition:
      The pointers outdata and indata must be aligned on 4-byte boundaries.
      N must be greater than or equal to four and a multiple of four.

  Parameters:
    indata     - pointer to source array of elements (int32_t)
    N          - number of samples (int)

  Returns:
    None.

  Example:
    <code>

    int     Num = 4;
    int32_t inBufTestA[8]={0x3FFFFFFF, 0x80000000, 0x73333333, 0x66666666,
                               0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  0.5,     -1,      0.9,    0.8,    0.2,   0.5,    1,    -0.6

    DSP_VectorZeroPad(inBufTestA, Num);

// inBufTestA =  {0x00000000, 0x00000000, 0x00000000, 0x00000000,
//                             0x19999999, 0x40000000, 0x7FFFFFFF, 0xB3333334};
//                  0,     0,      0,    0,    0.2,   0.5,    1,     -0.6

   </code>

  Remarks:
    None.
*/

void DSP_VectorZeroPad(int32_t *indata, int N);



// *****************************************************************************
// *****************************************************************************
// Section: Support Functions
// *****************************************************************************
// *****************************************************************************

// multiply and shift integer
static inline int16_t mul16(int16_t a, int16_t b)
{
	return (a * b) >> 15;
}

// multiply and shift Q15
static inline int16_t mul16r(int16_t a, int16_t b)
{
	return (a * b + 0x4000) >> 15;
}

// multiply and shift Q31
static inline int32_t mul32(int32_t a, int32_t b)
{
	return ((int64_t)a * b) >> 31;
}

// saturate positive Q15
static inline int32_t SAT16P(int32_t x)
{
	return (x > MAX16) ? MAX16 : x;
}

// saturate negative Q15
static inline int32_t SAT16N(int32_t x)
{
	return (x < MIN16) ? MIN16 : x;
}

// saturate both positive and negative Q15
static inline int32_t SAT16(int32_t x)
{
	int32_t y = SAT16P(x);
	return SAT16N(y);
}

#ifdef __cplusplus
}
#endif

#endif  /* _DSP_H_ */

