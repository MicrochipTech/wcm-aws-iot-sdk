#ifndef _PLIB_RTCC_PROCESSOR_H
#define _PLIB_RTCC_PROCESSOR_H

#if defined(__PIC16F__)
    #include "rtcc_pic_other.h"

#elif defined(__18CXX)
    #include "rtcc_pic18.h"

#elif defined(_PIC18)
    #include "rtcc_pic18.h"

#elif defined(__PIC24F__)
    #include "rtcc_p24Fxxxx.h"

#elif defined(__PIC24H__)
    #include "rtcc_p24Hxxxx.h"

#elif defined(__dspiC30F__)
    #include "rtcc_p30Fxxxx.h"

#elif defined(__dspiC33E__)
    #include "rtcc_p33Exxxx.h"

#elif defined(__dspiC33F__)
    #include "rtcc_p33Fxxxx.h"

#elif defined(__PIC32MX__)
    #include "rtcc_p32xxxx.h"

#elif defined(__PIC32MZ__)
    #include "rtcc_p32xxxx.h"

#endif

#endif//_PLIB_RTCC_PROCESSOR_H