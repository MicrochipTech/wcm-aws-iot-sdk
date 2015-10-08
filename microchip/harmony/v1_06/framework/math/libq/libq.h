/*******************************************************************************
  Company:
    Microchip Technology Inc.
	
  File Name:
    libq.h
	
  Summary:
    Optimized fixed point math functions for the PIC32MZ families of devices with
	microAptiv core features.
	
  Description:
	The LibQ Fixed-Point Math Library provides fixed-point math functions that are 
	optimized for  performance on the PIC32MZ families of devices that have microAptiv 
	core features. All functions are optimized for speed. This header file specifies 
	characteristics of each function, including execution time, memory size, and 
	resolution.

    Signed fixed point types are defined as follows:

    Qn_m where:
      * n is the number of data bits to the left of the radix point
      * m is the number of data bits to the right of the radix point
      * a signed bit is implied

    For convenience, short names are also defined:

    Exact Name (& Bits) Required Short Name _Q0_15 (16) _Q15; _Q15_16
    (32) _Q16; _Q0_31 (32) _Q31

    Functions in the library are prefixed with the type of the return
    value. For example, _LIBQ_Q16Sqrt returns a Q16 value equal to the
    square root of its argument.

    Argument types do not always match the return type. Refer to the
    function prototype for a specification of its arguments.

    In cases where the return value is not a fixed point type, the argument
    type is appended to the function name. For example, _LIBQ_ToFloatQ31
    accepts a type _Q31 argument.

    In some cases, both the return type and the argument type are specified
    within the function name. For example,

    Function Name (Return Type) [Argument Type]: _LIBQ_Q15_sin_Q2_13
    (_Q15) [_Q2_13]; _LIBQ_Q31_sin_Q2_29 (_Q31) [_Q2_29]

    Table of LIBQ functions:

      Divide:
          _Q16 _LIBQ_Q16Div (_Q16 dividend, _Q16 divisor);

      Square root:
          _Q16 _LIBQ_Q16Sqrt (_Q16 x);

      Exponential:
          _Q16 _LIBQ_Q16Exp (_Q16 x);

      Log:
          _Q4_11 _LIBQ_Q4_11_ln_Q16 (_Q16 x);
          _Q3_12 _LIBQ_Q3_12_log10_Q16 (_Q16 x);
          _Q5_10 _LIBQ_Q5_10_log2_Q16 (_Q16 x);

      Power:
          _Q16 _LIBQ_Q16Power (_Q16 x, _Q16 y);

      Sine:
          _Q15 _LIBQ_Q15_sin_Q2_13 (_Q2_13 x);
          _Q31 _LIBQ_Q31_sin_Q2_29 (_Q2_29 x);

      Cosine:
          _Q15 _LIBQ_Q15_cos_Q2_13 (_Q2_13 x);
          _Q31 _LIBQ_Q31_cos_Q2_29 (_Q2_29 x);

      Tangent:
          _Q7_8 _LIBQ_Q7_8_tan_Q2_13 (_Q2_13 x);
          _Q16z _LIBQ_Q16_tan_Q2_29 (_Q2_29 x);

      Arcsin:
          _Q2_13 _LIBQ_Q2_13_asin_Q15 (_Q15 x);
          _Q2_29 _LIBQ_Q2_29_asin_Q31 (_Q31 x);
          _Q2_29 _LIBQ_Q2_29_asin_Q31_Fast (_Q31 x);

      Arccos:
          _Q2_13 _LIBQ_Q2_13_acos_Q15 (_Q15 x);
          _Q2_29 _LIBQ_Q2_29_acos_Q31 (_Q31 x);
          _Q2_29 _LIBQ_Q2_29_acos_Q31_Fast (_Q31 x);

      Arctan:
          _Q2_13 _LIBQ_Q2_13_atan_Q7_8 (_Q7_8 x);
          _Q2_29 _LIBQ_Q2_29_atan_Q16 (_Q16 x);

      Arctan2:
          _Q2_13 _LIBQ_Q2_13_atan2_Q7_8 (_Q7_8 y, _Q7_8 x);
          _Q2_29 _LIBQ_Q2_29_atan2_Q16 (_Q16 y, _Q16 x);

      Random number:
          _Q15 _LIBQ_Q15Rand (int64_t &pSeed);
          _Q31 _LIBQ_Q31Rand (int64_t &pSeed);

      Float:
          float _LIBQ_ToFloatQ31 (_Q31 x);
          float _LIBQ_ToFloatQ15 (_Q15 x);
          _Q31 _LIBQ_Q31FromFloat (float x);
          _Q15 _LIBQ_Q15FromFloat (float x);

      String:
          void _LIBQ_ToStringQ15 (_Q15 x, char &s);
          _Q15 _LIBQ_Q15FromString (char &s);

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
// DOM-IGNORE-END

#ifndef _LIBQ_H    // Guards against multiple inclusion
#define _LIBQ_H

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
#include "stdlib.h"
#include "stdint.h"


#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Fixed point data types

  Summary:
    Fixed point data types used within LibQ.

  Description:
    This section defines the fixed point data types used with the
    LibQ fixed point library.

  Remarks:
    The underlying data type for most fixed point types is an integer.
    However, multiple fixed point types are defined so that the real
    value can be inferred from an internal integer value.
*/

typedef int32_t _Q15_16;   // 1 sign bit, 15 bits left of radix, 16 bits right of radix
typedef _Q15_16 _Q16;      // Short name for _Q15_16

typedef int32_t _Q0_31;    // 1 sign bit, 31 bits right of radix
typedef _Q0_31  _Q31;      // Short name for _Q_0_31

typedef int32_t _Q2_29;    // 1 sign bit, 2 bits left of radix, 29 bits right of radix

typedef int16_t _Q0_15;    // 1 sign bit, 15 bits right of radix
typedef _Q0_15  _Q15;      // Short name for _Q0_15

typedef int16_t _Q7_8;     // 1 sign bit, 7 bits left of radix,  8 bits right of radix

typedef int16_t _Q5_10;    // 1 sign bit, 5 bits left of radix, 10 bits right of radix

typedef int16_t _Q4_11;    // 1 sign bit, 4 bits left of radix, 11 bits right of radix

typedef int16_t _Q3_12;    // 1 sign bit, 3 bits left of radix, 12 bits right of radix

typedef int16_t _Q2_13;    // 1 sign bit, 2 bits left of radix, 13 bits right of radix


// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Fixed point constants

  Summary:
    Fixed point constants for the various fixed point types defined in LibQ.

  Description:
    Maximum and minimum values are defined for each fixed point type.
*/

#define _Q16_MAX     ((_Q16)0x7FFFFFFF)    // Maximum value of _Q16 (~32768.0)
#define _Q16_MIN     ((_Q16)0x80000000)    // Minimum value of _Q16 (-32768.0)

#define _Q15_MAX     ((_Q15)0x7FFF)        // Maximum value of _Q15 (~1.0)
#define _Q15_MIN     ((_Q15)0x8000)        // Minimum value of _Q15 (-1.0)

#define _Q31_MAX     ((_Q31)0x7FFFFFFF)    // Maximum value of _Q31 (~1.0)
#define _Q31_MIN     ((_Q31)0x80000000)    // Minimum value of _Q31 (-1.0)

#define _Q2_29_MAX   ((_Q2_29)0x7FFFFFFF)  // Maximum value of _Q2_29 (~4.0)
#define _Q2_29_MIN   ((_Q2_29)0x80000000)  // Minimum value of _Q2_29 (-4.0)

#define _Q7_8_MAX    ((_Q7_8)0x7FFF)       // Maximum value of _Q7_8 (~128.0)
#define _Q7_8_MIN    ((_Q7_8)0x8000)       // Minimum value of _Q7_8 (-128.0)

#define _Q5_10_MAX   ((_Q5_10)0x7FFF)      // Maximum value of _Q5_10 (~32.0)
#define _Q5_10_MIN   ((_Q5_10)0x8000)      // Minimum value of _Q5_10 (-32.0)

#define _Q4_11_MAX   ((_Q4_11)0x7FFF)      // Maximum value of _Q4_11 (~16.0)
#define _Q4_11_MIN   ((_Q4_11)0x8000)      // Minimum value of _Q4_11 (-16.0)

#define _Q3_12_MAX   ((_Q3_12)0x7FFF)      // Maximum value of _Q3_12 (~8.0)
#define _Q3_12_MIN   ((_Q3_12)0x8000)      // Minimum value of _Q3_12 (-8.0)

#define _Q2_13_MAX   ((_Q2_13)0x7FFF)      // Maximum value of _Q2_13 (~4.0)
#define _Q2_13_MIN   ((_Q2_13)0x8000)      // Minimum value of _Q2_13 (-4.0)


// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/*  LibQ - Optimized fixed point math library
*/


// *****************************************************************************
/* Function _LIBQ_Q16Div:

    _Q16  _LIBQ_Q16Div (_Q16 dividend, _Q16 divisor);

  Summary:
    _Q16 fixed point divide.

  Description:
    Quotient (_Q16) = Dividend (_Q16) / Divisor (_Q16).

  Precondition:
    Divisor must not equal 0.

  Parameters:
    dividend    - The divide operation dividend (_Q16)

    divisor     - The divide operation divisor (_Q16)

  Returns:
    _Q16 quotient of the divide operation

  Example:
    <code>

    _Q16  quotient, dividend, divisor;

    dividend = (_Q16)0x00010000;  // 1
    divisor  = (_Q16)0x00008000;  // 0.5

    quotient = _LIBQ_Q16Div (dividend, divisor);

    // quotient now equals 2;  i.e.,  (_Q16)0x00020000;

    </code>

  Remarks:
    The _LIBQ_Q16Div operation saturates its result.

    Execution Time (cycles): 143 typical (80 to 244)
    Program Memory   204 bytes

    Error <= 0.000015258789 (accurate to least significant _Q16 bit within the non-saturated range)
*/

_Q16  _LIBQ_Q16Div (_Q16 dividend, _Q16 divisor);


// *****************************************************************************
/* Function _LIBQ_Q16Sqrt:

    _Q16  _LIBQ_Q16Sqrt(_Q16 x);

  Summary:
    Square root of a positive _Q16 fixed point value.

  Description:
    Calculate the square root of a positive _Q16 fixed point value,
    and return the _Q16 result.

  Precondition:
    The input value must be positive.

  Parameters:
    x     - The _Q16 fixed point value input from which
            to find the square root.

  Returns:
    _LIBQ_Q16Sqrt returns the _Q16 fixed point value which is
    the square root of the input parameter.

  Example:
    <code>

    _Q16 squareRoot;

    squareRoot = _LIBQ_Q16Sqrt((_Q16)0x01000000);  // The square root of 256.0 is 16.0 (0x00100000)

    squareRoot = _LIBQ_Q16Sqrt((_Q16)0x00004000);  // The square root of 0.25 is 0.5 (0x00008000)

    squareRoot = _LIBQ_Q16Sqrt((_Q16)0x5851f42d);  // The square root of 22609.953125 is 150.366074 (0x00965db7)

    </code>

  Remarks:
     Execution Time (cycles): 240 typical (104 to 258)
     Program Memory   152 bytes

     Error <= 0.000015258789 (accurate to least significant _Q16 bit)
*/

_Q16  _LIBQ_Q16Sqrt(_Q16 x);



// *****************************************************************************
/* Function _LIBQ_Q16Exp:

    _Q16  _LIBQ_Q16Exp(_Q16 x);

  Summary:
    Calculates the exponential function e^x.

  Description:
    Calculates the exponential function e^x.  The calculation
    will saturate if the resulting value is outside the range of the
    _Q16 representation.  For x > 10.3972015380859375, the resulting
    value will be saturated to 0x7fffffff.  For x < -10.3972015380859375
    the resulting value will be saturated to 0.

  Precondition:
    None.

  Parameters:
    x     - The exponent value

  Returns:
    _LIBQ_Q16Exp returns the _Q16 fixed point result from the calculation e^x.

  Example:
    <code>

    _Q16 expResult;

    expResult = _LIBQ_Q16Exp((_Q16)0x00010000);   // _LIBQ_Q16Exp(1.000000) = 2.718277 (0x0002b7e1)

    </code>

  Remarks:
     The function _LIBQ_Q16Div is called by this routine
     and thus must be linked into the executable image.

     Execution Time (cycles): 170 typical (18 to 292)
     Program Memory    446 bytes

     Error <= 0.000015258789 (accurate to least significant _Q16 bit within the non-saturated range)
*/

_Q16  _LIBQ_Q16Exp (_Q16 x);


// *****************************************************************************
/* Function _LIBQ_Q4_11_ln_Q16:

    _Q4_11  _LIBQ_Q4_11_ln_Q16 (_Q16 x);

  Summary:
    Calculates the natural logarithm ln(x).

  Description:
    Calculates the natural logarithm ln(x).  x is of type _Q16 and must be positive.
    The resulting value is of type _Q4_11.

  Precondition:
    The input x must be positive.

  Parameters:
    x     - The input value from which to calculate ln(x).

  Returns:
    _LIBQ_Q4_11_ln_Q16 returns the _Q4_11 fixed point result
    from the calculation ln(x).

  Example:
    <code>

    _Q4_11 resultLN;

    resultLN = _LIBQ_Q4_11_ln_Q16 ((_Q16)0x00004000);  // _LIBQ_Q4_11_LN_Q16(0.250000) = -1.386230 (0xf4e9)

    </code>

  Remarks:
     Execution Time (cycles): 301 typical (14 to 346)
     Program Memory    176 bytes

     Error <= 0.00048828 (accurate to least significant _Q4_11 bit)
*/

_Q4_11  _LIBQ_Q4_11_ln_Q16 (_Q16 x);


// *****************************************************************************
/* Function _LIBQ_Q3_12_log10_Q16:

    _Q3_12  _LIBQ_Q3_12_log10_Q16 (_Q16 x);

  Summary:
    Calculates the value of Log10(x).

  Description:
    Calculates the log10(x), where log10(x) = ln(x) * log10(e).
    x is of type _Q16 and must be positive. The resulting value is
    of type _Q3_12.

  Precondition:
    The input x must be positive.

  Parameters:
    x     - The input value from which to calculate log10(x).

  Returns:
    _LIBQ_Q3_12_log10_Q16 returns the _Q3_12 fixed point result
    from the calculation log10(x).

  Example:
    <code>

    _Q3_12 resultLog10;

    resultLog10 = _LIBQ_Q3_12_log10_Q16 ((_Q16)0x12ed7d91);  // _LIBQ_Q3_12_log10_Q16(4845.490494) = 3.685303 (0x3af7)

    </code>

  Remarks:
     Execution Time (cycles): 301 typical (14 to 346)
     Program Memory    176 bytes

     Error <= 0.000244140625 (accurate to least significant _Q3_12 bit)
*/

_Q3_12  _LIBQ_Q3_12_log10_Q16 (_Q16 x);


// *****************************************************************************
/* Function _LIBQ_Q5_10_log2_Q16:

    _Q5_10  _LIBQ_Q5_10_log2_Q16 (_Q16 x);

  Summary:
    Calculates the value of log2(x).

  Description:
    Calculates the log2(x), where log2(x) = ln(x) * log2(e).
    x is of type _Q16 and must be positive. The resulting value is
    of type _Q5_10.

  Precondition:
    The input x must be positive.

  Parameters:
    x     - The input value from which to calculate log2(x).

  Returns:
    _LIBQ_Q5_10_log2_Q16 returns the _Q5_10 fixed point result
    from the calculation log2(x).

  Example:
    <code>

    _Q5_10 resultLog2;

    resultLog2 = _LIBQ_Q5_10_log2_Q16 ((_Q16)0x40000000);  // _LIBQ_Q5_10_log2_Q16(16384.000000) = 14.000000 (0x3800)

    </code>

  Remarks:
     Execution Time (cycles): 227 typical (14 to 268)
     Program Memory    164 bytes

     Error <= 0.0009765625 (accurate to least significant _Q5_10 bit)
*/

_Q5_10  _LIBQ_Q5_10_log2_Q16 (_Q16 x);


// *****************************************************************************
/* Function _LIBQ_Q16Power:

    _Q16  _LIBQ_Q16Power (_Q16 x, _Q16 y);

  Summary:
    Calculates the value of x raised to the y power (x^y).

  Description:
    Calculates the x raised to the y power.
    Both x and y are of type _Q16.  x must be positive.
    The calculation will saturate if the resulting value is
    outside the range of the _Q16 representation.

  Precondition:
    x must be positive.

  Parameters:
    x     - The _Q16 input value x from which to calculate x raised to the y.

    y     - The _Q16 input value y from which to calculate x raised to the y.

  Returns:
    _LIBQ_Q16Power returns the _Q16 fixed point result
    from the calculation x raised to the y.

  Example:
    <code>

    _Q16 resultPower;

    resultPower = _LIBQ_Q16Power ((_Q16)0x00020000, (_Q16)0xffff0000);  // _LIBQ_Q16Power(2.000000, -1.000000) = 0.500000 (0x00008000)

    </code>

  Remarks:
     Execution Time (cycles): 882 typical (586 to 1042)
     Program Memory    1038 bytes

     Error <= 0.000015258789 (accurate to least significant _Q16 bit within the non-saturated range)
*/

_Q16  _LIBQ_Q16Power (_Q16 x, _Q16 y);


// *****************************************************************************
/* Function _LIBQ_Q15_sin_Q2_13:

    _Q15  _LIBQ_Q15_sin_Q2_13 (_Q2_13 x);

  Summary:
    Calculates the value of sine(x).

  Description:
    Calculates the sine(x), where x is of type _Q2_13 radians and
    the resulting value is of type _Q15.

  Precondition:
    None.

  Parameters:
    x     - The _Q2_13 input value from which to calculate sine(x).

  Returns:
    _LIBQ_Q15_sin_Q2_13 returns the _Q15 fixed point result
    from the calculation sine(x).

  Example:
    <code>

    _Q15 resultSin;

    resultSin = _LIBQ_Q15_sin_Q2_13 ((_Q2_13)0x4093);  // _LIBQ_Q15_sin_Q2_13(2.017944) = 0.901672 (0x736a)

    </code>

  Remarks:
     Execution Time (cycles): 100 typical (100 to 102)
     Program Memory    220 bytes

     Error <= 0.00003052 (accurate to least significant _Q15 bit)
*/

_Q15  _LIBQ_Q15_sin_Q2_13 (_Q2_13 x);


// *****************************************************************************
/* Function _LIBQ_Q31_sin_Q2_29:

    _Q31  _LIBQ_Q31_sin_Q2_29 (_Q2_29 x);

  Summary:
    Calculates the value of sine(x).

  Description:
    Calculates the sine(x), where x is of type _Q2_29 radians and
    the resulting value is of type _Q31.

  Precondition:
    None.

  Parameters:
    x     - The _Q2_29 input value from which to calculate sine(x).

  Returns:
    _LIBQ_Q31_sin_Q2_29 returns the _Q31 fixed point result
    from the calculation sine(x).

  Example:
    <code>

    _Q31 resultSin;

    resultSin = _LIBQ_Q31_sin_Q2_29 ((_Q2_29)0x5a637cfe);  // _LIBQ_Q31_sin_Q2_29( 2.824644562) =  0.311668121 (0x27e4bdb1)

    </code>

  Remarks:
     Execution Time (cycles): 246 typical (244 to 266)
     Program Memory    598 bytes

     Error <= 0.00000000047 (accurate to least significant _Q31 bit)
*/

_Q31  _LIBQ_Q31_sin_Q2_29 (_Q2_29 x);


// *****************************************************************************
/* Function _LIBQ_Q15_cos_Q2_13:

    _Q15  _LIBQ_Q15_cos_Q2_13 (_Q2_13 x);

  Summary:
    Calculates the value of cosine(x).

  Description:
    Calculates the cosine(x), where x is of type _Q2_13 radians and
    the resulting value is of type _Q15.

  Precondition:
    None

  Parameters:
    x     - The _Q2_13 input value from which to calculate cosine(x).

  Returns:
    _LIBQ_Q15_cos_Q2_13 returns the _Q15 fixed point result
    from the calculation cosine(x).

  Example:
    <code>

    _Q15 resultCos;

    resultCos = _LIBQ_Q15_cos_Q2_13 ((_Q2_13)0x2171);  // _LIBQ_Q15_cos_Q2_13(1.045044) = 0.501862 (0x403d)

    </code>

  Remarks:
     Execution Time (cycles): 102 cycles
     Program Memory    224 bytes

     Error <= 0.00003052 (accurate to least significant _Q15 bit)
*/

_Q15  _LIBQ_Q15_cos_Q2_13 (_Q2_13 x);


// *****************************************************************************
/* Function _LIBQ_Q31_cos_Q2_29:

    _Q31  _LIBQ_Q31_cos_Q2_29 (_Q2_29 x);

  Summary:
    Calculates the value of cosine(x).

  Description:
    Calculates the cosine(x), where x is of type _Q2_29 radians and
    the resulting value is of type _Q31.

  Precondition:
    None.

  Parameters:
    x     - The _Q2_29 input value from which to calculate cosine(x).

  Returns:
    _LIBQ_Q31_cos_Q2_29 returns the _Q31 fixed point result
    from the calculation sine(x).

  Example:
    <code>

    _Q31 resultCos;

    resultCos = _LIBQ_Q31_cos_Q2_29 ((_Q2_29)0x07e2e1c2);  // _LIBQ_Q31_cos_Q2_29( 0.246445540) =  0.969785686 (0x7c21eff7)

    </code>

  Remarks:
     Execution Time (cycles): 265 typical (22 to 288)
     Program Memory    746 bytes

     Error <= 0.00000000047 (accurate to least significant _Q31 bit)
*/

_Q31  _LIBQ_Q31_cos_Q2_29 (_Q2_29 x);


// *****************************************************************************
/* Function _LIBQ_Q7_8_tan_Q2_13:

    _Q7_8  _LIBQ_Q7_8_tan_Q2_13 (_Q2_13 x);

  Summary:
    Calculates the value of tan(x).

  Description:
    Calculates the tan(x), where x is of type _Q2_13 radians and
    the resulting value is of type _Q7_8.

  Precondition:
    None

  Parameters:
    x     - The _Q2_13 input value from which to calculate tan(x).

  Returns:
    _LIBQ_Q7_8_tan_Q2_13 returns the _Q7_8 fixed point result
    from the calculation tan(x).

  Example:
    <code>

    _Q7_8 resultTan;

    resultTan = _LIBQ_Q7_8_tan_Q2_13 ((_Q2_13)0x2e20);  // _LIBQ_Q7_8_tan_Q2_13(1.441406) = 7.683594 (0x07af)

    </code>

  Remarks:
     Execution Time (cycles): 288 typical (18 to 346)
     Program Memory    980 bytes

     Error <= 0.00390625 (accurate to least significant _Q7_8 bit)
*/

_Q7_8  _LIBQ_Q7_8_tan_Q2_13 (_Q2_13 x);


// *****************************************************************************
/* Function _LIBQ_Q16_tan_Q2_29:

    _Q16  _LIBQ_Q16_tan_Q2_29 (_Q2_29 x);

  Summary:
    Calculates the value of tan(x).

  Description:
    Calculates the tan(x), where x is of type _Q2_29 radians and
    the resulting value is of type _Q16.

  Precondition:
    None

  Parameters:
    x     - The _Q2_29 input value from which to calculate tan(x).

  Returns:
    _LIBQ_Q16_tan_Q2_29 returns the _Q16 fixed point result
    from the calculation tan(x).  The resulting value is saturated.

  Example:
    <code>

    _Q16 resultTan;

    resultTan = _LIBQ_Q16_tan_Q2_29 ((_Q2_29)0x16720c36);  // _LIBQ_Q16_tan_Q2_29( 0.701421838) =  0.844726562 (0x0000d840)

    </code>

  Remarks:
     The functions _LIBQ_Q31_sin_Q2_29, _LIBQ_Q31_cos_Q2_29, and _LIBQ_Q16Div
     are called by this routine and thus must be linked into the executable image.

     Execution Time (cycles): 703 typical (22 to 796)
     Program Memory    88 bytes

     Error <= 0.000015259 (accurate to least significant _Q16 bit for the input range -1.568 .. 1.568)
     Error rises from 0.0 to 0.065 for the input range -1.568 .. -1.570765808 and 1.568 .. 1.570765808)
*/

_Q16  _LIBQ_Q16_tan_Q2_29 (_Q2_29 x);


// *****************************************************************************
/* Function _LIBQ_Q2_13_asin_Q15:

    _Q2_13  _LIBQ_Q2_13_asin_Q15 (_Q15 x);

  Summary:
    Calculates the asin value of asin(x).

  Description:
    Calculates asin(x), where x is of type _Q15 and
    the resulting value is of type _Q2_13.  The output value
    will be radians in the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    x     - The _Q15 input value from which to calculate asin(x).

  Returns:
    _LIBQ_Q2_13_asin_Q15 returns the _Q2_13 fixed point result
    from the calculation asin(x).

  Example:
    <code>

    _Q2_13 resultAsin;

    resultAsin = _LIBQ_Q2_13_asin_Q15 ((_Q15)0x3231);  // _LIBQ_Q2_13_asin_Q15(0.392120) = 0.402954 (0x0ce5)

    </code>

  Remarks:
     The functions _LIBQ_Q16Sqrt and _LIBQ_Q16Div are called by this routine
     and thus must be linked into the executable image.

     Execution Time (cycles): 578 typical (22 to 656)
     Program Memory    336 bytes

     Error <= 0.00012207 (accurate to least significant _Q2_13 bit)

     A higher resolution version of this function exists with equivalent performance, see _LIBQ_Q2_29_asin_Q31_Fast
*/

_Q2_13  _LIBQ_Q2_13_asin_Q15 (_Q15 x);


// *****************************************************************************
/* Function _LIBQ_Q2_29_asin_Q31:

    _Q2_29  _LIBQ_Q2_29_asin_Q31 (_Q31 x);

  Summary:
    Calculates the value of asin(x).

  Description:
    Calculates the asin(x), where x is of type _Q31 and
    the resulting value is of type _Q2_29.  The output value
    will be in radians the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    x     - The _Q31 input value from which to calculate asin(x).

  Returns:
    _LIBQ_Q2_29_asin_Q31 returns the _Q2_29 fixed point result
    from the calculation asin(x).

  Example:
    <code>

    _Q2_29 resultAsin;

    resultAsin = _LIBQ_Q2_29_asin_Q31 ((_Q31)0x7fe50658);  // _LIBQ_Q2_29_asin_Q31( 0.9991767816) =  1.5302172359 (0x30f78a23)

    </code>

  Remarks:
     The functions _LIBQ_Q2_29_asin_Q31_Fast and_LIBQ_Q31_sin_Q2_29 are called
     by this routine and thus must be linked into the executable image.

     Execution Time (cycles): 2525 typical (286 to 4330)
     Program Memory    138 bytes

     Error <= 0.0000000019 (accurate to least significant _Q2_29 bit for the range -0.9993..0.9993)
     Error <= 0.0000000346 (accurate to 5th least significant _Q2_29 bit for the range -1.0 .. -0.9993 and 0.9993 .. 1.0)

     A faster version of this function exists with modestly reduced accuracy, see _LIBQ_Q2_29_asin_Q31_Fast

*/

_Q2_29  _LIBQ_Q2_29_asin_Q31 (_Q31 x);


// *****************************************************************************
/* Function _LIBQ_Q2_29_asin_Q31_Fast:

    _Q2_29  _LIBQ_Q2_29_asin_Q31_Fast (_Q31 x);

  Summary:
    Calculates the value of asin(x).  This function executes faster than
    the _LIBQ_Q2_29_asin_Q31 function, but is less precise.

  Description:
    Calculates the asin(x), where x is of type _Q31 and
    the resulting value is of type _Q2_29.  The output value
    will be in radians the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    x     - The _Q31 input value from which to calculate asin(x).

  Returns:
    _LIBQ_Q2_29_asin_Q31_Fast returns the _Q2_29 fixed point result
    from the calculation asin(x).

  Example:
    <code>

    _Q2_29 resultAsin;

    resultAsin = _LIBQ_Q2_29_asin_Q31_Fast ((_Q31)0x7fe50658);  // _LIBQ_Q2_29_asin_Q31_Fast( 0.9991767816) =  1.5302172359 (0x30f78a23)

    </code>

  Remarks:
     Execution Time (cycles): 507 typical (22 to 1300)
     Program Memory    638 bytes

     Error <= 0.000000911 (accurate to 9 least significant _Q2_29 bits)

     A higher resolution version of this function exists with reduced performance, see _LIBQ_Q2_29_asin_Q31
*/

_Q2_29  _LIBQ_Q2_29_asin_Q31_Fast (_Q31 x);


// *****************************************************************************
/* Function _LIBQ_Q2_13_acos_Q15:

    _Q2_13  _LIBQ_Q2_13_acos_Q15 (_Q15 x);

  Summary:
    Calculates the value of acos(x).

  Description:
    Calculates the acos(x), where x is of type _Q15 and
    the resulting value is of type _Q2_13.  The output value
    will be radians in the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    x     - The _Q15 input value from which to calculate acos(x).

  Returns:
    _LIBQ_Q2_13_acos_Q15 returns the _Q2_13 fixed point result
    from the calculation acos(x).

  Example:
    <code>

    _Q2_13 resultAcos;

    resultAcos = _LIBQ_Q2_13_acos_Q15((_Q15)0x2993); // _LIBQ_Q2_13_acos_Q15(0.324799) = 1.239990 (0x27ae)

    </code>

  Remarks:
     The function _LIBQ_Q2_13_asin_Q15 is called by this routine
     and thus must be linked into the executable image.

     Execution Time (cycles): 588 typical (32 to 666)
     Program Memory    24 bytes

     Error <= 0.00012207 (accurate to least significant _Q2_13 bit)

     A higher precision function with equivalent performance exists, see _LIBQ_Q2_29_acos_Q31_Fast
*/

_Q2_13  _LIBQ_Q2_13_acos_Q15 (_Q15 x);


// *****************************************************************************
/* Function _LIBQ_Q2_29_acos_Q31:

    _Q2_29  _LIBQ_Q2_29_acos_Q31 (_Q31 x);

  Summary:
    Calculates the value of acos(x).

  Description:
    Calculates the acos(x), where x is of type _Q31 and
    the resulting value is of type _Q2_29.  The output value
    will be radians in the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    x     - The _Q31 input value from which to calculate acos(x).

  Returns:
    _LIBQ_Q2_29_acos_Q31 returns the _Q2_29 fixed point result
    from the calculation acos(x).

  Example:
    <code>

    _Q2_29 resultAcos;

    resultAcos = _LIBQ_Q2_29_acos_Q31 ((_Q31)0xee63708c);  // _LIBQ_Q2_29_acos_Q31(-0.1375903431) =  1.7088244837 (0x36aeb0af)

    </code>

  Remarks:
     The functions _LIBQ_Q2_29_asin_Q31_Fast and _LIBQ_Q31_cos_Q2_29 are called
     by this routine and thus must be linked into the executable image.

     Execution Time (cycles): 3370 typical (70 to 4824)
     Program Memory    142 bytes

     Error <= 0.0000000019 (accurate to least significant _Q2_29 bit for the range -0.9993..0.9993)
     Error <= 0.0000000355 (accurate to 5th least significant _Q2_29 bit for the range -1.0 .. -0.9993 and 0.9993 .. 1.0)

     A similar function with higher performance and reduced precision exists, see _LIBQ_Q2_29_acos_Q31_Fast

*/

_Q2_29  _LIBQ_Q2_29_acos_Q31 (_Q31 x);


// *****************************************************************************
/* Function _LIBQ_Q2_29_acos_Q31_Fast:

    _Q2_29  _LIBQ_Q2_29_acos_Q31_Fast (_Q31 x);

  Summary:
    Calculates the value of acos(x).  This function executes faster than
    _LIBQ_Q2_29_acos_Q31 but is less precise.

  Description:
    Calculates the acos(x), where x is of type _Q31 and
    the resulting value is of type _Q2_29.  The output value
    will be radians in the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    x     - The _Q31 input value from which to calculate acos(x).

  Returns:
    _LIBQ_Q2_29_acos_Q31_Fast returns the _Q2_29 fixed point result
    from the calculation acos(x).

  Example:
    <code>

    _Q2_29 resultAcos;

    resultAcos = _LIBQ_Q2_29_acos_Q31_Fast ((_Q31)0xee63708c);  // _LIBQ_Q2_29_acos_Q31_Fast(-0.1375903431) =  1.7088244837 (0x36aeb0af)

    </code>

  Remarks:
     The function _LIBQ_Q2_29_asin_Q31_Fast is called by this routine
     and thus must be linked into the executable image.

     Execution Time (cycles): 517 typical (32 to 1310)
     Program Memory    28 bytes

     Error <= 0.000000911 (accurate to 9 least significant _Q2_29 bits)

     A higher precision function with reduced performance exists, see _LIBQ_Q2_29_acos_Q31
*/

_Q2_29  _LIBQ_Q2_29_acos_Q31_Fast (_Q31 x);


// *****************************************************************************
/* Function _LIBQ_Q2_13_atan_Q7_8:

    _Q2_13  _LIBQ_Q2_13_atan_Q7_8 (_Q7_8 x);

  Summary:
    Calculates the value of atan(x).

  Description:
    Calculates the atan(x), where x is of type _Q7_8 and
    the resulting value is of type _Q2_13.  The output value
    will be radians in the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    x     - The _Q7_8 input value from which to calculate atan(x).

  Returns:
    _LIBQ_Q2_13_atan_Q7_8 returns the _Q2_13 fixed point result
    from the calculation atan(x).

  Example:
    <code>

    _Q2_13 resultAtan;

    resultAtan = _LIBQ_Q2_13_atan_Q7_8 ((_Q7_8)0x0097);  // _LIBQ_Q2_13_atan_Q7_8(0.589844) = 0.532959 (0x110e)

    </code>

  Remarks:
     The function _LIBQ_Q2_13_atan2_Q7_8 is called by this routine
     and thus must be linked into the executable image.

     Execution Time (cycles): 240 typical (202 to 256)
     Program Memory    16 bytes

     Error <= 0.00012207 (accurate to least significant _Q2_13 bit)
*/

_Q2_13  _LIBQ_Q2_13_atan_Q7_8 (_Q7_8 x);


// *****************************************************************************
/* Function _LIBQ_Q2_29_atan_Q16:

    _Q2_29  _LIBQ_Q2_29_atan_Q16 (_Q16 x);

  Summary:
    Calculates the value of atan(x).

  Description:
    Calculates the atan(x), where x is of type _Q16 and
    the resulting value is of type _Q2_29.  The output value
    will be radians in the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    x     - The _Q16 input value from which to calculate atan(x).

  Returns:
    _LIBQ_Q2_29_atan_Q16 returns the _Q2_29 fixed point result
    from the calculation atan(x).

  Example:
    <code>

    _Q2_29 resultAtan;

    resultAtan = _LIBQ_Q2_29_atan_Q16 ((_Q16)0x00098b31);  // _LIBQ_Q2_29_atan_Q16(9.543716) = 1.466396 (0x2eecb7ee)

    </code>

  Remarks:
     The function _LIBQ_Q2_29_atan2_Q16 is called by this routine
     and thus must be linked into the executable image.

     Execution Time (cycles): 354 typical (178 to 360)
     Program Memory    16 bytes

     Error <= 0.000000003 (accurate within one least significant _Q2_29 bit)
*/

_Q2_29  _LIBQ_Q2_29_atan_Q16 (_Q16 x);


// *****************************************************************************
/* Function _LIBQ_Q2_13_atan2_Q7_8:

    _Q2_13  _LIBQ_Q2_13_atan2_Q7_8 (_Q7_8 y, _Q7_8 x);

  Summary:
    Calculates the value of atan2(y, x).

  Description:
    Calculates the atan2(y, x), where y and x are of type _Q7_8 and
    the resulting value is of type _Q2_13.  The output value
    will be radians in the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    y     - The _Q7_8 input value from which to calculate atan2(y, x).
    x     - The _Q7_8 input value from which to calculate atan2(y, x).

  Returns:
    _LIBQ_Q2_13_atan2_Q7_8 returns the _Q2_13 fixed point result
    from the calculation atan2(y, x).

  Example:
    <code>

    _Q2_13 resultAtan2;

    resultAtan2 = _LIBQ_Q2_13_atan2_Q7_8 ((_Q7_8)0x589d, (_Q7_8)0xf878); // _LIBQ_Q2_13_atan2_Q7_8(88.613281, -7.531250) = 1.655518 (0x34fa)

    </code>

  Remarks:
     The function _LIBQ_Q16Div is called by this routine
     and thus must be linked into the executable image.

     Execution Time (cycles): 220 typical (22 to 250)
     Program Memory    288 bytes

     Error <= 0.00012207 (accurate to least significant _Q2_13 bit)
*/

_Q2_13  _LIBQ_Q2_13_atan2_Q7_8 (_Q7_8 y, _Q7_8 x);


// *****************************************************************************
/* Function _LIBQ_Q2_29_atan2_Q16:

    _Q2_29  _LIBQ_Q2_29_atan2_Q16 (_Q16 y, _Q16 x);

  Summary:
    Calculates the value of atan2(y, x).

  Description:
    Calculates the atan(y, x), where y and x are of type _Q16 and
    the resulting value is of type _Q2_29.  The output value
    will be radians in the range pi >= result >= -pi.

  Precondition:
    None.

  Parameters:
    y     - The _Q16 input value from which to calculate atan2(y, x).
    x     - The _Q16 input value from which to calculate atan2(y, x).

  Returns:
    _LIBQ_Q2_29_atan2_Q16 returns the _Q2_29 fixed point result
    from the calculation atan2(y, x).

  Example:
    <code>

    _Q2_29 resultAtan2;

    resultAtan2 = _LIBQ_Q2_29_atan2_Q16 ((_Q16)0xf6276270, x(_Q16)0x34b4b4c0);  // _LIBQ_Q2_29_atan2_Q16(-2520.615479, 13492.706055) = -0.184684 (0xfa1710c7)

    </code>

  Remarks:
     The C function __divdi3 is called by this routine
     and thus must be linked into the executable image.

     Execution Time (cycles): 348 typical (20 to 376)
     Program Memory    464 bytes

     Error <= 0.000000003 (accurate within one least significant _Q2_29 bit)
*/

_Q2_29  _LIBQ_Q2_29_atan2_Q16 (_Q16 y, _Q16 x);


// *****************************************************************************
/* Function _LIBQ_Q15Rand:

    _Q15  _LIBQ_Q15Rand (int64_t *pSeed);

  Summary:
    Generate a _Q15 random number.

  Description:
    Generates a _Q15 pseudo-random value based on the seed supplied
    as a parameter.  The first time this function is called,
    the seed value must be supplied by the user;  this initial
    seed value can either be constant or random, depending on
    whether the user wants to generate a repeatable or a
    non-repeatable pseudo-random sequence.

    The function updates the *pSeed value each time it is called.
    The updated *pSeed value must be passed back to the function
    with each subsequent call.

    Warning:  The pseudo-random sequence generated by this
              function may be insufficient for cryptographic use.

  Precondition:
    None.

  Parameters:
    pSeed  - A pointer to the seed value used by the function
             to generate a pseudo-random sequence.

  Returns:
    _LIBQ_Q15Rand returns a random _Q15 value.  _LIBQ_Q15Rand
    also updates the int64_t *pSeed value.

  Example:
    <code>

    // Initialize seed to a constant or random value
    static int64_t  randomSeed = 0xA71078BE72D4C1F1;

    _Q15           randomValue;

    randomValue = _LIBQ_Q15Rand(&randomSeed);
    ...
    randomValue = _LIBQ_Q15Rand(&randomSeed);

    </code>

  Remarks:

     Execution Time (cycles): 32
     Program Memory     92 bytes

*/

_Q15  _LIBQ_Q15Rand (int64_t *pSeed);


// *****************************************************************************
/* Function _LIBQ_Q31Rand:

    _Q31  _LIBQ_Q31Rand (int64_t *pSeed);

  Summary:
    Generate a _Q31 random number.

  Description:
    Generates a _Q31 pseudo-random value based on the seed supplied
    as a parameter.  The first time this function is called,
    the seed value must be supplied by the user;  this initial
    seed value can either be constant or random, depending on
    whether the user wants to generate a repeatable or a
    non-repeatable pseudo-random sequence.

    The function updates the *pSeed value each time it is called.
    The updated *pSeed value must be passed back to the function
    with each subsequent call.

    Warning:  The pseudo-random sequence generated by this
              function may be insufficient for cryptographic use.

  Precondition:
    None.

  Parameters:
    pSeed  - A pointer to the seed value used by the function
             to generate a pseudo-random sequence.

  Returns:
    _LIBQ_Q31Rand returns a pseudo-random _Q31 value.  _LIBQ_Q31Rand
    also updates the int64_t *pSeed value.

  Example:
    <code>

    // Initialize seed to a constant or random value
    static int64_t  randomSeed = 0x7F18BA710E72D4C1;

    _Q31  randomValue;

    randomValue = _LIBQ_Q31Rand(&randomSeed);
    ...
    randomValue = _LIBQ_Q31Rand(&randomSeed);

    </code>

  Remarks:

     Execution Time (cycles): 32
     Program Memory    88 bytes

*/

_Q31  _LIBQ_Q31Rand (int64_t *pSeed);


// *****************************************************************************
/* Function _LIBQ_ToFloatQ31:

    float  _LIBQ_ToFloatQ31(_Q31 x);

  Summary:
    Converts a _Q31 value to a float.

  Description:
    Converts a _Q31 fixed point value to a floating point
    representation.  The floating point value is returned
    by the function.

  Precondition:
    None.

  Parameters:
    x     - The _Q31 fixed point value to convert to float

  Returns:
    _LIBQ_ToFloatQ31 returns the floating point (float)
    value corresponding to the _Q31 input value.

  Example:
    <code>

    float f;

    f = _LIBQ_ToFloatQ31((_Q31)0x00004000);  // f now equals 0.000008

    f = _LIBQ_ToFloatQ31((_Q31)0x80000000);  // f now equals -1.0

    f = _LIBQ_ToFloatQ31((_Q31)0x5851f42d);  // f now equals 0.690001

    </code>

  Remarks:
     The C library functions __floatsisf and __divsf3 are called
     by this routine and thus must be linked in to the executable image.

     Execution Time (cycles): 163 typical (54 to 176)
     Program Memory    28 bytes

*/

float  _LIBQ_ToFloatQ31(_Q31 x);


// *****************************************************************************
/* Function _LIBQ_ToFloatQ15:

    float  _LIBQ_ToFloatQ15(_Q15 x);

  Summary:
    Converts a _Q15 value to a float.

  Description:
    Converts a _Q15 fixed point value to a floating point
    representation.  The floating point value is returned
    by the function.

  Precondition:
    None.

  Parameters:
    x     - The _Q15 fixed point value to convert to float

  Returns:
    _LIBQ_ToFloatQ15 returns the floating point (float)
    value corresponding to the _Q15 input value.

  Example:
    <code>

    float f;

    f = _LIBQ_ToFloatQ15((_Q15)0x4000);  // f now equals 0.5

    f = _LIBQ_ToFloatQ15((_Q15)0x8000);  // f now equals -1.0

    f = _LIBQ_ToFloatQ15((_Q15)0xb7ff);  // f now equals -0.562531

    </code>

  Remarks:
     The C library functions __floatsisf and __divsf3 are called
     by this routine and thus must be linked in to the executable image.

     Execution Time (cycles): 158 typical (54 to 176)
     Program Memory    28 bytes

*/

float  _LIBQ_ToFloatQ15(_Q15 x);


// *****************************************************************************
/* Function _LIBQ_Q31FromFloat:

    _Q31  _LIBQ_Q31FromFloat(float x);

  Summary:
    Converts a float to a _Q31 value.

  Description:
    Converts a floating point value to a _Q31 fixed point
    representation.  The _Q31 fixed point value is returned
    by the function.  The conversion will saturate if
    the value is outside the range of the _Q31 representation.

  Precondition:
    None.

  Parameters:
    x     - The floating point value to convert to _Q31 fixed point.

  Returns:
    _LIBQ_Q31FromFloat returns the _Q31 fixed point value
    corresponding to the floating point (float) input value.

  Example:
    <code>

    _Q31 q31;

    q31 = _LIBQ_Q31FromFloat((float)0.000008);  // q31 now equals (_Q31)0x00004000

    q31 = _LIBQ_Q31FromFloat((float)-1.0);  // q31 now equals (_Q31)0x80000000

    q31 = _LIBQ_Q31FromFloat((float)0.690001);  // q31 now equals (_Q31)0x5851f400

    </code>

  Remarks:
     The C library functions __gesf2, __lesf2, __addsf3, __mulsf3, and __fixsfsi are called
     by this routine and thus must be linked into the executable image.

     Execution Time (cycles): 210 typical (158 to 214)
     Program Memory    100 bytes

*/

_Q31  _LIBQ_Q31FromFloat(float x);


// *****************************************************************************
/* Function _LIBQ_Q15FromFloat:

    _Q15  _LIBQ_Q15FromFloat(float x);

  Summary:
    Converts a float to a _Q15 value.

  Description:
    Converts a floating point value to a _Q15 fixed point
    representation.  The _Q15 fixed point value is returned
    by the function.  The conversion will saturate if
    the value is outside the range of the _Q15 representation.

  Precondition:
    None.

  Parameters:
    x     - The float point value to convert to _Q15 fixed point

  Returns:
    _LIBQ_Q15FromFloat returns the _Q15 fixed point value
    corresponding to the floating point (float) input value.

  Example:
    <code>

    _Q15 q15;

    q15 = _LIBQ_Q15FromFloat((float)0.5);  // q15 now equals (_Q15)0x4000

    q15 = _LIBQ_Q15FromFloat((float)-1.0);  // q15 now equals (_Q15)0x8000

    q15 = _LIBQ_Q15FromFloat((float)-0.233828);  // q15 now equals (_Q15)0xe212

    </code>

  Remarks:
     The C library functions __gesf2, __lesf2, __addsf3, __mulsf3, and __fixsfsi are called
     by this routine and thus must be linked into the executable image.

     Execution Time (cycles): 213 typical (158 to 224)
     Program Memory    96 bytes

*/

_Q15  _LIBQ_Q15FromFloat(float x);


// *****************************************************************************
/* Function _LIBQ_ToStringQ15:

    void  _LIBQ_ToStringQ15(_Q15 x, char *s);

  Summary:
    _Q15 to ASCII conversion.

  Description:
    Convert a _Q15 fixed point value to an ASCII string representation
    in a -N.NNNNNN format.

  Precondition:
    The character string "s" must be at least 10 characters
    long, including the Null string terminator.

  Parameters:
    x     - The fixed point value to be converted into an ASCII string (_Q15)

    s     - A pointer to the output string of at least 10 characters

  Returns:
    An ASCII string that represents the _Q15 fixed point value in -N.NNNNNN format.
    The output string will be terminated by a Null (0x00) character.

  Example:
    <code>

    char  s[10];

    _LIBQ_ToStringQ15((_Q15)0x1000, s);      // s will equal "0.125000"

    _LIBQ_ToStringQ15((_Q15)0x8000, s);      // s will equal "-1.000000"

    _LIBQ_ToStringQ15((_Q15)0x7FFF, s);      // s will equal "0.999969"

    </code>

  Remarks:
     Execution Time (cycles): 118 typical (28 to 132)
     Program Memory   200 bytes

*/

void  _LIBQ_ToStringQ15(_Q15 x, char *s);


// *****************************************************************************
/* Function _LIBQ_Q15FromString:

    _Q15  _LIBQ_Q15FromString(char *s);

  Summary:
    ASCII to _Q15 conversion.

  Description:
    Convert an ASCII string into a _Q15 fixed point value.
    The ASCII string must be in an -N.NNNNNN format.
    Leading spaces are ignored.  The conversion stops at
    either the first non-conforming character in the
    string or the Null string terminator.  There must be
    no spaces within the string value itself.

  Precondition:
    None.

  Parameters:
    s     - A pointer to the ASCII input string representing
            the _Q15 fixed point value.

  Returns:
    _LIBQ_Q15FromString returns the _Q15 fixed point value represented
    by the input string.

  Example:
    <code>

    _Q15  x;

    x = _LIBQ_Q15FromString("0.125");      // x will equal 0.125 using
                                // an internal value of 0x1000

    x = _LIBQ_Q15FromString("-1.0");       // x will equal -1.0 using
                                // an internal value of 0x8000

    x = _LIBQ_Q15FromString("0.999969");   // x will equal 0.999969 using
                                // an internal value of 0x7FFF
    </code>

  Remarks:
     Execution Time (cycles): 296 typical (28 to 346)
     Program Memory   172 bytes

*/

_Q15  _LIBQ_Q15FromString(char *s);



#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif


#ifdef __cplusplus
}
#endif

#endif // _LIBQ_H

/*******************************************************************************
 End of File
*/
