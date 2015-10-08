#ifndef _PLIB_CMP_PROCESSOR_H
#define _PLIB_CMP_PROCESSOR_H
#if defined(__PIC24F__)
    #include "cmp_p24Fxxxx.h"

#elif defined(__PIC24H__)
    #include "cmp_p24Hxxxx.h"

#elif defined(__dsPIC30F__)
    #include "cmp_p30Fxxxx.h"

#elif defined(__dsPIC33E__)
    #include "cmp_p33Exxxx.h"

#elif defined(__dsPIC33F__)
    #include "cmp_p33Fxxxx.h"

#elif defined(__PIC32MX__)
    #include "cmp_p32xxxx.h"

#elif defined(__PIC32MZ__)
    #include "cmp_p32xxxx.h"


#else
    #error "Can't find header"

#endif

#endif//_PLIB_CMP_PROCESSOR_H
