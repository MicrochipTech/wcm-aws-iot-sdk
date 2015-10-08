/*******************************************************************************
  Big Integer Library

  Summary:
    Microchip Big Interger Library
    
    Description
    - Provides support for integers larger than 32 bits
*******************************************************************************/

/*******************************************************************************
File Name:  big_int.c
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

#include "big_int.h"





// External declarations for assembly helpers
#if defined(__C30__)
    extern __attribute__((__near__)) BIGINT_DATA_TYPE *_iA, *_iB, *_xA, *_xB, *_iR, _wC;
#else
    extern BIGINT_DATA_TYPE *_iA, *_iB, *_xA, *_xB, *_iR, _wC;
#endif
extern void _addBI(void);
extern void _subBI(void);
extern void _zeroBI(void);
extern void _msbBI(void);
extern void _mulBI(void);
extern void _copyBI(void);
extern void _sqrBI(void);
extern void _masBI(void);
extern void _addBIROM(void);
extern void _subBIROM(void);
extern void _mulBIROM(void);
extern void _masBIROM(void);


#if BIGINT_PROFILE
    uint32_t addBICounter = 0;
    uint32_t addBIROMCounter = 0;
    uint32_t subBICounter = 0;
    uint32_t subBIROMCounter = 0;S
    uint32_t zeroBICounter = 0;
    uint32_t msbBICounter = 0;
    uint32_t mulBICounter = 0;
    uint32_t mulBIROMCounter = 0;
    uint32_t sqrBICounter = 0;
    uint32_t masBICounter = 0;
    uint32_t masBIROMCounter = 0;
    uint32_t copyBICounter = 0;
    
    #define    addBI()        {addBICounter -= SYS_TMR_TickCountGet(); _addBI(); addBICounter += SYS_TMR_TickCountGet();}
    #define    addBIROM()    {addBIROMCounter -= SYS_TMR_TickCountGet(); _addBIROM();addBIROMCounter += SYS_TMR_TickCountGet();}
    #define    subBI()        {subBICounter -= SYS_TMR_TickCountGet(); _subBI(); subBICounter += SYS_TMR_TickCountGet();}
    #define    subBIROM()    {subBIROMCounter -= SYS_TMR_TickCountGet(); _subBIROM(); subBIROMCounter += SYS_TMR_TickCountGet();}
    #define    zeroBI()    {zeroBICounter -= SYS_TMR_TickCountGet(); _zeroBI(); zeroBICounter += SYS_TMR_TickCountGet();}
    #define    msbBI()        {msbBICounter -= SYS_TMR_TickCountGet(); _msbBI(); msbBICounter += SYS_TMR_TickCountGet();}
    #define    mulBI()        {mulBICounter -= SYS_TMR_TickCountGet(); _mulBI(); mulBICounter += SYS_TMR_TickCountGet();}
    #define    mulBIROM()    {mulBIROMCounter -= SYS_TMR_TickCountGet(); _mulBIROM(); mulBIROMCounter += SYS_TMR_TickCountGet();}
    #define    sqrBI()        {sqrBICounter -= SYS_TMR_TickCountGet(); _sqrBI(); sqrBICounter += SYS_TMR_TickCountGet();}
    #define    masBI()        {masBICounter -= SYS_TMR_TickCountGet(); _masBI(); masBICounter += SYS_TMR_TickCountGet();}
    #define    masBIROM()    {masBIROMCounter -= SYS_TMR_TickCountGet(); _masBIROM(); masBIROMCounter += SYS_TMR_TickCountGet();}
    #define    copyBI()    {copyBICounter -= SYS_TMR_TickCountGet(); _copyBI(); copyBICounter += SYS_TMR_TickCountGet();}

#else
    #define    addBI()        _addBI()
    #define    addBIROM()    _addBIROM()
    #define    subBI()        _subBI()
    #define    subBIROM()    _subBIROM()
    #define    zeroBI()    _zeroBI()
    #define    msbBI()        _msbBI()
    #define    mulBI()        _mulBI()
    #define    mulBIROM()    _mulBIROM()
    #define    sqrBI()        _sqrBI()
    #define    masBI()        _masBI()
    #define    masBIROM()    _masBIROM()
    #define    copyBI()    _copyBI()
#endif

#if BIGINT_DEBUG
    #if defined(__C30__)
        void BigIntPrint(const BIGINT *a)
        {
              uint16_t w;
              uint8_t v;
              
              BIGINT_DATA_TYPE *ptr;
              
              for(ptr = a->ptrMSBMax; ptr >= a->ptrLSB; ptr--)
              {
                  TCPIP_UINT16_VAL wv;
                  
                  wv.Val = *ptr;
                  
                SYS_MESSAGE(btohexa_high(wv.v[1]));
                SYS_MESSAGE(btohexa_low(wv.v[1]));
                SYS_MESSAGE(btohexa_high(wv.v[0]));
                SYS_MESSAGE(btohexa_low(wv.v[0]));
            }
        }
    #endif
    
    void putulhexUART(uint32_t dw)
    {
        SYS_MESSAGE('0');
        SYS_MESSAGE('x');
        SYS_MESSAGE(btohexa_high(((uint8_t*)&dw)[3]));
        SYS_MESSAGE(btohexa_low(((uint8_t*)&dw)[3]));
        SYS_MESSAGE(btohexa_high(((uint8_t*)&dw)[2]));
        SYS_MESSAGE(btohexa_low(((uint8_t*)&dw)[2]));
        SYS_MESSAGE(btohexa_high(((uint8_t*)&dw)[1]));
        SYS_MESSAGE(btohexa_low(((uint8_t*)&dw)[1]));
        SYS_MESSAGE(btohexa_high(((uint8_t*)&dw)[0]));
        SYS_MESSAGE(btohexa_low(((uint8_t*)&dw)[0]));
    }
#endif


static BIGINT_DATA_TYPE* BigIntMSB(BIGINT *n);

/*********************************************************************
 * Function:        void BigInt(BIGINT *theInt, BIGINT_DATA_TYPE *data, uint16_t wWordLength)
 *
 * PreCondition:    None
 *
 * Input:           *theInt: the integer to create
 *                    *data: a pointer to the data
 *                    wWordLength: the number of words in the integer (a word is 2 bytes on PIC24/dsPIC)
 *
 * Output:          The BIGINT is ready to use
 *
 * Side Effects:    None
 *
 * Overview:        Call BigInt() to correctly set up the pointers.
 *
 * Note:            None
 ********************************************************************/
void BigInt(BIGINT *theInt, BIGINT_DATA_TYPE *data, uint16_t wWordLength)
{
    theInt->ptrLSB = data;
    theInt->ptrMSB = data + wWordLength - 1;
    theInt->ptrMSBMax = theInt->ptrMSB;
    theInt->bMSBValid = 0;
}


/*********************************************************************
 * Function:        void BigIntZero(BIGINT* theInt)
 *
 * PreCondition:    None
 *
 * Input:           *theInt: the integer to clear
 *
 * Output:          theInt = 0
 *
 * Side Effects:    None
 *
 * Overview:        Call BigIntZero() zero all data bytes in the BigInt
 *
 * Note:            None
 ********************************************************************/
void BigIntZero(BIGINT *theInt)
{
    _iA = theInt->ptrLSB;
    _xA = theInt->ptrMSBMax;
    zeroBI();

    // Set up the new MSB pointer
    theInt->ptrMSB = theInt->ptrLSB;
    theInt->bMSBValid = 1;
}

/*********************************************************************
 * Function:        void BigIntMod(BIGINT *n, const BIGINT* m)
 *
 * PreCondition:    None
 *
 * Input:           *n: a pointer to the number
 *                    *m: a pointer to the modulus
 *                    
 * Output:          *n contains the modded number
 *                     i.e: *n = *n % *m
 *
 * Side Effects:    None
 *
 * Overview:        Call BigIntMod() to calculate the modulus of two
 *                    really big numbers.
 *
 * Note:            Supports at least 2048 bits
 ********************************************************************/
void BigIntMod(BIGINT *n, BIGINT* m)
{
    BIGINT_DATA_TYPE *ptrMSBn, MSBm;
    BIGINT_DATA_TYPE_2 qHatInt;
    union
    {
        BIGINT_DATA_TYPE v[2];
        BIGINT_DATA_TYPE_2 Val;
    } topTwoWords;

    // Find the starting MSBs
    ptrMSBn = BigIntMSB(n);
    MSBm = *BigIntMSB(m);

    // Set up assembly pointers for m
    // _iB and _xB are limiters in the _mas function
    _iB = m->ptrLSB;
    _xB = BigIntMSB(m);

    // Find out how many bytes we need to shift and move the LSB up
    _iR = n->ptrLSB + (BigIntMagnitudeDifference(n, m) - 1);

    // This loops while the order of magnitude (in words) of n > m
    // Each iteration modulos off one word of magnitude from n
    while(_iR >= n->ptrLSB)
    {
        // Find qHat = MSBn:MSBn-1/MSBm
        topTwoWords.Val = *((BIGINT_DATA_TYPE_2*)(ptrMSBn - 1));
        qHatInt = topTwoWords.Val / MSBm;
        if(qHatInt > BIGINT_DATA_MAX)  
            qHatInt = BIGINT_DATA_MAX;

#if BIGINT_DEBUG
        SYS_MESSAGE("\r\n\r\n    n = ");
        BigIntPrint(n);
        SYS_MESSAGE("\r\n    m = ");
        BigIntPrint(m);
        SYS_MESSAGE("\r\n    qHat (");
        SYS_MESSAGE((char*)qHatInt);
        SYS_MESSAGE(") = topTwo(");
        SYS_MESSAGE((char*)topTwoWords.Val);
        SYS_MESSAGE(") / (");
        SYS_MESSAGE((char*)MSBm);
        SYS_MESSAGE(") ");
#endif

        // Once qHat is determined, we multiply M by qHat, shift it up
        // as many bytes as possible, and subtract the result.
        // In essence, qHat is a rough estimation of the quotient, divided
        // by a power of 2^16 (PIC24/dsPIC) or 2^32 (PIC32)
        
        // This implementation multiplies and subtracts in the same step
        // using a _mas function which saves about 30% of clock cycles.

        // Save the old MSB and set up the ASM pointers
        _wC = (BIGINT_DATA_TYPE)qHatInt;

        // Do the multiply and subtract
        // Occassionally this results in underflow...this is solved below.
        masBI();

        // qHat may have been 1 or 2 greater than possible.  If so,
        // the new MSB will be greater than the old one, so we *add*
        // M back to N in the shifted position until overflow occurs
        // and this case corrects itself.
        while(topTwoWords.v[1] < *BigIntMSB(n))
//        while(((BIGINT_DATA_TYPE*)&topTwoWords)[1] < *BigIntMSB(n))
        {
            _iA = _iR;
            _xA = BigIntMSB(n);
            addBI();
        }

        // We should have modulated off a word (or two if we were lucky),
        // so move our MSB and LSB pointers as applicable
        while(*ptrMSBn == 0x0u)
        {
            _iR--;
            n->ptrMSB--;
            ptrMSBn--;
        }
    }

    // Iteration of the _mas function can only handle full-byte orders
    // of magnitude.  The result may still be a little larger, so this
    // cleans up the last few multiples with simple subtraction.
    while(BigIntCompare(n, m) >= 0)
    {
        _iA = n->ptrLSB;
        _xA = n->ptrMSB;
        subBI();
    
        // Invalidate MSB pointer
        n->bMSBValid = 0;
    }
}


/*********************************************************************
 * Function:        int8_t BigIntCompare(BIGINT *a, BIGINT *b)
 *
 * PreCondition:    None
 *
 * Input:           *a: a pointer to the first number
 *                    *b: a pointer to the second number
 *
 * Output:          0 if a == b
 *                    1 if a > b
 *                    -1 if a < b
 *
 * Side Effects:    None
 *
 * Overview:        Determines if a > b, a < b, or a == b
 *
 * Note:            Supports at least 2048 bits.
 *                    
 ********************************************************************/
int8_t BigIntCompare(BIGINT *a, BIGINT *b)
{
    uint32_t                 magA, magB;
    BIGINT_DATA_TYPE         valA, valB;
    BIGINT_DATA_TYPE         *ptrA;
    BIGINT_DATA_TYPE         *ptrB;
    
    magA = BigIntMSB(a) - a->ptrLSB;
    magB = BigIntMSB(b) - b->ptrLSB;

    #if BIGINT_DEBUG_COMPARE
        SYS_MESSAGE("\r\n    Compared Magnitudes |a|:");
        putulhexUART(w1);
        SYS_MESSAGE(" |b|:");
        putulhexUART(w2);
        SYS_MESSAGE(" diff:");
        putulhexUART(w1-w2);
    #endif

    if(magA > magB)
    {
        #if BIGINT_DEBUG_COMPARE
            SYS_MESSAGE(" a > b");
        #endif
        
        return 1;
    }
    else if(magA < magB)
    {
        #if BIGINT_DEBUG_COMPARE
            SYS_MESSAGE(" a < b");
        #endif
        
        return -1;
    }

    #if BIGINT_DEBUG_COMPARE
        SYS_MESSAGE(" Checking further bytes...");
    #endif
    
    // Loop through all words, looking for a non-equal word
    ptrA = BigIntMSB(a);
    ptrB = BigIntMSB(b);
    while(ptrA >= a->ptrLSB)    // Magnitude is same, no need to check ptrB bounds
    {
        valA = *ptrA--;
        valB = *ptrB--;
        
        if(valA > valB)
        {
            #if BIGINT_DEBUG_COMPARE
                SYS_MESSAGE(" a > b");
            #endif
            
            return 1;
        }
        else if(valA < valB)
        {
            #if BIGINT_DEBUG_COMPARE
                SYS_MESSAGE(" a < b");
            #endif
            
            return -1;
        }    
    }

    // All words were exactly identical, return match
    return 0;
}



/*********************************************************************
 * Function:        int BigIntMagnitudeDifference(const BIGINT *n)
 *
 * PreCondition:    None
 *
 * Input:           *a: a pointer to the first number
 *                    *b: a pointer to the second number
 *                    
 * Output:          Returns the magnitude of difference in zero-bytes
 *
 * Side Effects:    None
 *
 * Overview:        Helps to quickly determine a byte shift for operations
 *
 * Note:            Supports at least 2048 bits
 ********************************************************************/
int BigIntMagnitudeDifference(BIGINT *a, BIGINT *b)
{
    return BigIntMagnitude(a) - BigIntMagnitude(b);
}


/*********************************************************************
 * Function:        uint16_t BigIntMagnitude(BIGINT *n)
 *
 * PreCondition:    None
 *
 * Input:           *n: a pointer to the number
 *                    
 * Output:          Returns the number of significant words in the data, less one (ex: 0x12, has zero magnitude)
 *
 * Side Effects:    None
 *
 * Overview:        Helps to quickly determine the magnitude of the number
 *
 * Note:            Supports at least 2048 bits
 ********************************************************************/
uint16_t BigIntMagnitude(BIGINT *n)
{
    return BigIntMSB(n) - n->ptrLSB;
}


/*********************************************************************
 * Function:        static BIGINT_DATA_TYPE* BigIntMSB(const BIGINT *n)
 *
 * PreCondition:    None
 *
 * Input:           *n: a pointer to the number
 *                    
 * Output:          n->ptrMSB points to the MSB of n
 *
 * Side Effects:    None
 *
 * Overview:        Updates the ptrMSB.  Use after an operation in which
 *                    the new MSB cannot be estimated.
 *
 * Note:            Supports at least 2048 bits
 ********************************************************************/
static BIGINT_DATA_TYPE* BigIntMSB(BIGINT *n)
{
    BIGINT_DATA_TYPE *iASave, *xASave;
    
    // If cached value is valid, use it
    if(n->bMSBValid)
        return n->ptrMSB;

    // Otherwise find a new MSB and save it
    iASave = _iA;
    xASave = _xA;
    _iA = n->ptrLSB;
    _xA = n->ptrMSBMax;
    msbBI();
    n->ptrMSB = _xA;
    n->bMSBValid = 1;

    _iA = iASave;
    _xA = xASave;

    return n->ptrMSB;
}

/*********************************************************************
 * Function:        void BigIntMultiply(const BIGINT *a, const BIGINT *b, BIGINT *res)
 *
 * PreCondition:    res->ptrMSBMax - res->ptrLSB + 1 >= a->ptrMSBMax - a->ptrLSB + 1 + b->ptrMSBMax - b->ptrLSB + 1, &res != &[a|b]
 *
 * Input:           *a: a pointer to the first number
 *                    *b: a pointer to the second number
 *                    *res: a pointer to memory to store the result
 *
 * Output:          *res contains the result of a * b
 *
 * Side Effects:    None
 *
 * Overview:        Call BigIntMultiply() to multiply two really big numbers.
 *
 * Note:            Supports at least 2048 result bits.
 *                    This essentially implements elementary school long
 *                    multiplication in base 65536 (PIC24/dsPIC).  
 *                    This is the fastest    algorithm until you pass about 
 *                    1024 bits.  This is O(n^2).
 *                    res CANNOT be A or B.
 ********************************************************************/
void BigIntMultiply(BIGINT *a, BIGINT *b, BIGINT *res)
{
    // Clear out the result
    BigIntZero(res);

    // Load the start and stop pointers
    _iA = a->ptrLSB;
    _xA = BigIntMSB(a);
    _iB = b->ptrLSB;
    _xB = BigIntMSB(b);
    _iR = res->ptrLSB;

    // Perform the multiplication
    mulBI();

    // Invalidate the MSB ptr
    res->bMSBValid = 0;
}


/*********************************************************************
 * Function:        void BigIntSquare(const BIGINT *a, BIGINT *res)
 *
 * PreCondition:    res->size >= 2 * a->size, &res != &a
 *
 * Input:           *a: a pointer to the number
 *                    *res: a pointer to memory to store the result
 *
 * Output:          *res contains the result of a * a
 *
 * Side Effects:    None
 *
 * Overview:        Call BigIntSquare() to square two really big numbers.
 *
 * Note:            Supports at least 2048 result bits.
 *                    Functionally equivalent to BigIntMultiply, except
 *                    an optimization is made for the case of square that
 *                    allows us to skip ~1/2 the iterations.
 *                    res CANNOT be A.
 ********************************************************************/
void BigIntSquare(BIGINT *a, BIGINT *res)
{
    BigIntZero(res);
    _iA = a->ptrLSB;
    _xA = BigIntMSB(a);
    _iR = res->ptrLSB;
    sqrBI();

    // Invalidate the MSB ptr
    res->bMSBValid = 0;
}

/*********************************************************************
 * Function:        void BigIntAdd(BIGINT *a, const BIGINT *b)
 *
 * PreCondition:    a->ptrMSBMax - a->ptrLSB must be >= b->ptrMSB - b->ptrLSB
 *
 * Input:           *a: a pointer to the first number
 *                    *b: a pointer to the second number
 *
 * Output:          a = a + b
 *
 * Side Effects:    None
 *
 * Overview:        Call BigIntAdd() to add two really big numbers
 *
 * Note:            Supports at least 2048 result bits.
 ********************************************************************/
void BigIntAdd(BIGINT *a, BIGINT *b)
{
    _iA = a->ptrLSB;
    _xA = a->ptrMSBMax;
    _iB = b->ptrLSB;
    _xB = BigIntMSB(b);
    addBI();

    // Invalidate MSB pointer
    a->bMSBValid = 0;
}


/*********************************************************************
 * Function:        void BigIntSubtract(BIGINT *a, const BIGINT *b)
 *
 * PreCondition:    a->ptrMSBMax - a->ptrLSB must be >= b->ptrMSB - b->ptrLSB
 *
 * Input:           *a: a pointer to the first number
 *                    *b: a pointer to the second number
 *
 * Output:          a = a - b
 *
 * Side Effects:    None
 *
 * Overview:        Call BigIntSubtract() to subtract two really big numbers
 *
 * Note:            Supports at least 2048 result bits.
 ********************************************************************/
void BigIntSubtract(BIGINT *a, BIGINT *b)
{
    _iA = a->ptrLSB;
    _xA = a->ptrMSBMax;
    _iB = b->ptrLSB;
    _xB = BigIntMSB(b);
    subBI();

    // Invalidate MSB pointer
    a->bMSBValid = 0;
}


/*********************************************************************
 * Function:        void BigIntCopy(BIGINT *a, const BIGINT *b)
 *
 * PreCondition:    None
 *
 * Input:           *a: a pointer to a BIGINT to copy into
 *                    *b: a pointer to the data
 *
 * Output:          a = b
 *
 * Side Effects:    None
 *
 * Overview:        Call BigIntCopy() copy one BIGINT to another
 *
 * Note:            Supports at least 2048 bits.  Only data is copied, so
 *                    if sizeof(b) > sizeof(a), only the least significant 
 *                    sizeof(a) bytes are copied.
 ********************************************************************/
void BigIntCopy(BIGINT *a, BIGINT *b)
{
    _iA = a->ptrLSB;
    _xA = a->ptrMSBMax;
    _iB = b->ptrLSB;
    _xB = b->ptrMSBMax;
    copyBI();

    // Invalidate MSB pointer
    a->bMSBValid = 0;
}


/*********************************************************************
 * Function:        void BigIntSwapEndianness(BIGINT *a)
 *
 * PreCondition:    None
 *
 * Input:           *a: a pointer to the BigInt
 *
 * Output:          *a: same value, with endianness swapped
 *
 * Side Effects:    None
 *
 * Overview:        Converts a big-endian data array to little-endian,
 *                    or a little-endian data array to big-endian.
 *
 * Note:            None
 ********************************************************************/
void BigIntSwapEndianness(BIGINT *a)
{
    uint8_t temp, *front, *end;
    
    // Set up the pointers
    front = (uint8_t*)a->ptrLSB;
    end = (uint8_t*)a->ptrMSBMax + (sizeof(BIGINT_DATA_TYPE) - 1);
    
    // Swap all data elements
    while(front < end)
    {
        temp = *front;
        *front = *end;
        *end = temp;
        front++;
        end--;
    }
            
}

