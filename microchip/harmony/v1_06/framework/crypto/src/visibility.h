/**************************************************************************
  Crypto Framework Library Header

  Company:
    Microchip Technology Inc.

  File Name:
    visibility.h
  
  Summary:
    Crypto Framework Library header for cryptographic functions.

  Description:
    This header file contains function prototypes and definitions of
    the data types and constants that make up the Cryptographic Framework
    Library for PIC32 families of Microchip microcontrollers.
**************************************************************************/

//DOM-IGNORE-BEGIN
/******************************************************************************
Copyright � 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND,
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
//DOM-IGNORE-END



/* Visibility control macros */


#ifndef CTAO_CRYPT_VISIBILITY_H
#define CTAO_CRYPT_VISIBILITY_H


/* CYASSL_API is used for the public API symbols.
        It either imports or exports (or does nothing for static builds)

   CYASSL_LOCAL is used for non-API symbols (private).
*/

#if defined(BUILDING_CYASSL)
    #if defined(HAVE_VISIBILITY) && HAVE_VISIBILITY
        #define CYASSL_API   __attribute__ ((visibility("default")))
        #define CYASSL_LOCAL __attribute__ ((visibility("hidden")))
    #elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550)
        #define CYASSL_API   __global  
        #define CYASSL_LOCAL __hidden
    #elif defined(_MSC_VER)
        #ifdef CYASSL_DLL
            #define CYASSL_API extern __declspec(dllexport)
        #else
            #define CYASSL_API
        #endif
        #define CYASSL_LOCAL
    #else
        #define CYASSL_API
        #define CYASSL_LOCAL
    #endif /* HAVE_VISIBILITY */
#else /* BUILDING_CYASSL */
    #if defined(_MSC_VER)
        #ifdef CYASSL_DLL
            #define CYASSL_API extern __declspec(dllimport)
        #else
            #define CYASSL_API
        #endif
        #define CYASSL_LOCAL
    #else
        #define CYASSL_API
        #define CYASSL_LOCAL
    #endif
#endif /* BUILDING_CYASSL */


#endif /* CTAO_CRYPT_VISIBILITY_H */

