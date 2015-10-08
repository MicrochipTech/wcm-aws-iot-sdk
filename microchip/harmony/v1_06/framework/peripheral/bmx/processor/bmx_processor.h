#ifndef _BMX_PROCESSOR_H
#define _BMX_PROCESSOR_H

#if defined(__PIC16F__)
    #include "bmx_pic_other.h"

#elif defined(__18CXX)
    #include "bmx_pic18.h"

#elif defined(_PIC18)
    #include "bmx_pic18.h"

#elif defined(__PIC24F__)
    #include "bmx_p24Fxxxx.h"

#elif defined(__PIC24H__)
    #include "bmx_p24Hxxxx.h"

#elif defined(__dsPIC30F__)
    #include "bmx_p30Fxxxx.h"

#elif defined(__dsPIC33E__)
    #include "bmx_p33Exxxx.h"

#elif defined(__dsPIC33F__)
    #include "bmx_p33Fxxxx.h"

#elif defined(__PIC32MX__)
    #include "bmx_p32xxxx.h"

#elif defined(__PIC32MZ__)
    #include "bmx_p32xxxx.h"

#else
    #error "Can't find header"

#endif

#endif//_BMX_PROCESSOR_H
