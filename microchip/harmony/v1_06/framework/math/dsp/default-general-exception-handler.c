/*********************************************************************
 *
 *        Default General Exception Handler Implementation
 *
 *********************************************************************
 * Filename:        default-general-exception-handler.c
 *
 * Processor:       PIC32
 *
 * Compiler:        MPLAB C Compiler for PIC32 MCUs
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the 'Company') for its PIC32/PIC24F Microcontroller is intended
 * and supplied to you, the Company's customer, for use solely and
 * exclusively on Microchip PIC32/PIC24F Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ********************************************************************/
/* This is a default definition of a general exception handler function.
 * It does nothing and just goes into an infinite loop. If the user
 * application supplies a handler function, this function will not be
 * referenced and thus not pulled in from the library.
 */

#include <stdint.h>

typedef struct _XCPT_FRAME  // access to all major registers from the instruction that caused the exception
{
    uint32_t at;
    uint32_t v0;
    uint32_t v1;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t t7;
    uint32_t t8;
    uint32_t t9;
    uint32_t ra;
    uint32_t lo;
    uint32_t hi;
    uint32_t cause;
    uint32_t status;
    uint32_t epc;

}XCPT_FRAME;

static enum {
    EXCEP_IRQ = 0,	// interrupt, 0x0
    EXCEP_AdEL = 4,	// address error exception (load or ifetch), 0x4
    EXCEP_AdES,         // address error exception (store), 0x5
    EXCEP_IBE,          // bus error (ifetch), 0x6
    EXCEP_DBE,          // bus error (load/store), 0x7
    EXCEP_Sys,          // syscall, 0x8
    EXCEP_Bp,           // breakpoint, 0x9
    EXCEP_RI,           // reserved instruction, 0xA
    EXCEP_CpU,          // coprocessor unusable, 0xB
    EXCEP_Overflow,	// arithmetic overflow, 0xC
    EXCEP_Trap,         // trap (possible divide by zero), 0xD
    EXCEP_IS1 = 16,	// implementation specfic 1
    EXCEP_CEU,          // CorExtend Unuseable
    EXCEP_C2E           // coprocessor 2
} _excep_code;

static unsigned int _excep_code;
static unsigned int _excep_addr;

void 
__attribute__((weak, nomips16)) _general_exception_handler (XCPT_FRAME* const pXFrame)
{
   
    _excep_code = pXFrame->cause;   // capture exception type

    _excep_code = (_excep_code & 0x0000007C) >> 2;

    if (_excep_code == EXCEP_Overflow)
    {

    pXFrame->v0 = 0x7FFF;           // set function output to maximum (saturation)
    pXFrame->v1 = 0x7FFF;           // set intermediate results to maximum (saturation)
    pXFrame->epc = pXFrame->epc + 4; // set return from exception to next instructino (skip)

    }

    return;

    while (1) {
        // we should only end up here if our exception generates an exception

    }
}
