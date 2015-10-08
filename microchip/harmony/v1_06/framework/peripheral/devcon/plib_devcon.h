/*******************************************************************************
  Device Control (DEVCON) Peripheral Library Interface Header

  Company:
    Microchip Technology Inc.

  File Name:
    plib_devcon.h

  Summary:
    Defines the Device Control Peripheral Library Interface.

  Description:
    This header file contains the function prototypes and definitions of
    the data types and constants that make up the interface to the Device Control
    Peripheral Library for Microchip microcontrollers. The definitions in this
    file are for the Device Control module.
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

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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

#ifndef _PLIB_DEVCON_H
#define _PLIB_DEVCON_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Constants and Data Types
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************
/* Alternate Clock Targets Enumeration

  Summary:
    Selects Input Capture (IC) or Output Compare (OC).

  Description:
    Selects Input Capture (IC) or Output Compare (OC) for enabling/disabling
	the alternate clock.

  Remarks:
    None.
*/
typedef enum {

    // Input Capture
    DEVCON_ALT_CLOCK_IC  /*DOM-IGNORE-BEGIN*/ = 1 /*DOM-IGNORE-END*/,

    // Output Compare
    DEVCON_ALT_CLOCK_OC  /*DOM-IGNORE-BEGIN*/ = 2 /*DOM-IGNORE-END*/

} DEVCON_ALT_CLOCK_TARGET;


//*****************************************************************************
/* USB Clock In Sleep Mode Enumeration

  Summary:
    Selects whether the USB clock operates in Sleep mode.

  Description:
    Selects whether the USB clock operates in Sleep mode.

  Remarks:
    None.
*/
typedef enum {

    // USB PHY clock is shut down when Sleep mode is active.
    DEVCON_USB_NO_CLOCK_IN_SLEEP  /*DOM-IGNORE-BEGIN*/ = 0 /*DOM-IGNORE-END*/,

    // USP PHY clock continues to run when Sleep mode is active
    DEVCON_USB_CLOCK_IN_SLEEP     /*DOM-IGNORE-BEGIN*/ = 1 /*DOM-IGNORE-END*/

} DEVCON_USB_SLEEP_MODE;


//*****************************************************************************
/* Module Registers for Lock/Unlock enumeration.

  Summary:
    Selects module registers for write lock or unlock.

  Description:
    Selects module registers for write lock or unlock.

  Remarks:
    Can be ORed together.
*/
typedef enum {

    // Peripheral Pin Select registers
    DEVCON_PPS_REGISTERS  /*DOM-IGNORE-BEGIN*/ = 1 /*DOM-IGNORE-END*/,

    // Peripheral Module Disable registers
    DEVCON_PMD_REGISTERS  /*DOM-IGNORE-BEGIN*/ = 2 /*DOM-IGNORE-END*/,

    // Permission Group registers
    DEVCON_PERMISSION_GROUP_REGISTERS  /*DOM-IGNORE-BEGIN*/ = 4 /*DOM-IGNORE-END*/,

    // All lockable registers
    DEVCON_ALL_REGISTERS  /*DOM-IGNORE-BEGIN*/ = 7 /*DOM-IGNORE-END*/

} DEVCON_REGISTER_SET;


//*****************************************************************************
/* Flash Error Correcting Code (ECC) Configuration enumeration

  Summary:
    Selects how ECC is applied to Flash memory.

  Description:
    Selects how ECC is applied to Flash memory.

  Remarks:
    None.
*/
typedef enum {

    // ECC and dynamic ECC are disabled
    DEVCON_ECC_DISABLED        /*DOM-IGNORE-BEGIN*/ = 0 /*DOM-IGNORE-END*/,

    // ECC and dynamic ECC are disabled and the configuration locked
    DEVCON_ECC_DISABLED_LOCKED /*DOM-IGNORE-BEGIN*/ = 1 /*DOM-IGNORE-END*/,

    // Dynamic Flash ECC is enabled and the configuration is locked
    DEVCON_DYN_ECC_ENABLED_LOCKED  /*DOM-IGNORE-BEGIN*/ = 2 /*DOM-IGNORE-END*/,

    // Flash ECC is enabled, the configuration is locked, word Flash writes disabled
    DEVCON_FLASH_ECC_ENABLED_LOCKED  /*DOM-IGNORE-BEGIN*/ = 3 /*DOM-IGNORE-END*/

} DEVCON_ECC_CONFIG;


//*****************************************************************************
/* Ignore Debugger Freeze for Peripheral Module(s) enumeration

  Summary:
    Sets modules to ignore debugger's freeze command.

  Description:
    Sets modules to ignore debugger's freeze command.

  Remarks:
    None.
*/
typedef enum {

    // USB module
    DEVCON_DEBUG_USB   /*DOM-IGNORE-BEGIN*/ =  1 /*DOM-IGNORE-END*/,

    // UART 1
    DEVCON_DEBUG_UART1 /*DOM-IGNORE-BEGIN*/ =  2 /*DOM-IGNORE-END*/,

    // UART 2
    DEVCON_DEBUG_UART2 /*DOM-IGNORE-BEGIN*/ =  4 /*DOM-IGNORE-END*/,

    // SPI 1
    DEVCON_DEBUG_SPI1  /*DOM-IGNORE-BEGIN*/ =  8 /*DOM-IGNORE-END*/,

    // USB, UART 1, UART 2, and SPI 1
    DEVCON_DEBUG_ALL   /*DOM-IGNORE-BEGIN*/ = 15 /*DOM-IGNORE-END*/

} DEVCON_DEBUG_PERIPHERAL;


// *****************************************************************************
// *****************************************************************************
// Section: Includes
// *****************************************************************************
// *****************************************************************************
/* See Bottom of file for implementation header include files.
*/
// DOM-IGNORE-BEGIN
#include "peripheral/devcon/processor/devcon_processor.h"
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Peripheral Library Interface Routines
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_AlternateClockEnable(DEVCON_MODULE_ID index,
                                          DEVCON_ALT_CLOCK_TARGET targetAltClock )

  Summary:
    Selects the alternate clock source for Input Capture or Output Compare modules.

  Description:
    Selects the alternate clock source for Input Capture or Output Compare modules.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID
    targetAltClock - DEVCON_ALT_CLOCK_IC or DEVCON_ALT_CLOCK_OC

  Returns:
    None.

  Example:
  <code>
    // Call system service to unlock oscillator
    PLIB_DEVCON_AlternateClockEnable(DEVCON_ID, DEVCON_ALT_CLOCK_IC || DEVCON_ALT_CLOCK_OC );
  </code>

  Remarks:
    The feature is not supported on all devices.  Refer to the specific device
	data sheet to determine availability.
    A system unlock must be performed before this function can be executed.
*/

void PLIB_DEVCON_AlternateClockEnable(DEVCON_MODULE_ID index,
                                      DEVCON_ALT_CLOCK_TARGET targetAltClock);


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_AlternateClockDisable(DEVCON_MODULE_ID index,
                                           DEVCON_ALT_CLOCK_TARGET targetAltClock )

  Summary:
    Disables the alternate clock source for Input Capture or Output Compare modules,
    The primary clock source will be used instead.

  Description:
    Disables the alternate clock source for Input Capture or Output Compare modules.
    The primary clock source will be used instead.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID
    targetAltClock - DEVCON_ALT_CLOCK_IC or DEVCON_ALT_CLOCK_OC

  Returns:
    None.

  Example:
  <code>
    // Call system service to unlock oscillator
    PLIB_DEVCON_AlternateClockDisable(DEVCON_ID, DEVCON_ALT_CLOCK_IC || DEVCON_ALT_CLOCK_OC );
  </code>

  Remarks:
    The feature is not supported on all devices.  Refer to the specific device
	data sheet to determine availability.
    A system unlock must be performed before this function can be executed.
*/

void PLIB_DEVCON_AlternateClockDisable(DEVCON_MODULE_ID index,
                                       DEVCON_ALT_CLOCK_TARGET targetAltClock);


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_DeviceRegistersLock(DEVCON_MODULE_ID index,
                                         DEVCON_REGISTER_SET registersToLock )

  Summary:
    Locks device module registers, preventing modifications to the registers.

  Description:
    Locks device module registers, preventing modifications to the registers.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID
    registersToLock - element from DEVCON_REGISTER_SET, which can be ORed together

  Returns:
    None.

  Example:
  <code>
    // Call system service to unlock oscillator
    PLIB_DEVCON_DeviceRegistersLock(DEVCON_ID,  DEVCON_ALL_REGISTERS);
    PLIB_DEVCON_DeviceRegistersUnlock(DEVCON_ID,DEVCON_PPS_REGISTERS);
  </code>

  Remarks:
    The feature is not supported on all devices.  Refer to the specific device
	data sheet to determine availability.
    A system unlock must be performed before this function can be executed.
*/

void PLIB_DEVCON_DeviceRegistersLock(DEVCON_MODULE_ID index,
                                     DEVCON_REGISTER_SET registersToLock );


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_DeviceRegistersUnlock(DEVCON_MODULE_ID index,
                                           DEVCON_REGISTER_SET registersToLock )

  Summary:
    Unlocks device module registers, allowing modifications to the registers.

  Description:
    Unlocks device module registers, allowing modifications to the registers.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID
    registersToLock - element from DEVCON_REGISTER_SET, which can be ORed together

  Returns:
    None.

  Example:
  <code>
    // Call system service to unlock oscillator
    PLIB_DEVCON_DeviceRegistersLock(DEVCON_ID,  DEVCON_ALL_REGISTERS);
    PLIB_DEVCON_DeviceRegistersUnlock(DEVCON_ID,DEVCON_PPS_REGISTERS);
  </code>

  Remarks:
    The feature is not supported on all devices.  Refer to the specific device
	data sheet to determine availability.
    A system unlock must be performed before this function can be executed.
*/

void PLIB_DEVCON_DeviceRegistersUnlock(DEVCON_MODULE_ID index,
                                       DEVCON_REGISTER_SET registersToLock );


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_USBPHYSleepModeSet(DEVCON_MODULE_ID index,
                                        DEVCON_USB_SLEEP_MODE sleepOrRun)

  Summary:
    Selects USB PHY clocking during in sleep mode.

  Description:
    Selects USB PHY clocking during in sleep mode.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID
    sleepOrRun - DEVCON_USB_NO_CLOCK_IN_SLEEP or DEVCON_USB_CLOCK_IN_SLEEP

  Returns:
    None.

  Example:
  <code>
    // Call system service to unlock oscillator
    PLIB_DEVCON_USBPHYSleepModeSet(DEVCON_ID,DEVCON_USB_NO_CLOCK_IN_SLEEP)
  </code>

  Remarks:
    The feature is not supported on all devices.  Refer to the specific device
	data sheet to determine availability.
    A system unlock must be performed before this function can be executed.

 */
void PLIB_DEVCON_USBPHYSleepModeSet(DEVCON_MODULE_ID index,
                                    DEVCON_USB_SLEEP_MODE sleepOrRun);


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_FlashErrCorrectionModeSet(DEVCON_MODULE_ID index,
                                               DEVCON_ECC_CONFIG flashECC)
  Summary:
    Sets FLASH error correction operation.

  Description:
    Sets FLASH error correction operation.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID
    flashECC - DEVCON_ECC_DISABLED, DEVCON_ECC_DISABLED_LOCKED,
               DEVCON_DYN_ECC_ENABLED_LOCKED, or DEVCON_FLASH_ECC_ENABLED_LOCKED

  Returns:
    None.

  Example:
  <code>
  </code>

  Remarks:
    The feature is not supported on all devices.  Refer to the specific device
	data sheet to determine availability.
    Once ECC has been locked, it cannot be unlocked except through a system reset.
*/
void PLIB_DEVCON_FlashErrCorrectionModeSet(DEVCON_MODULE_ID index,
                                           DEVCON_ECC_CONFIG flashECC);


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_JTAGPortEnable(DEVCON_MODULE_ID index)

  Summary:
    Enables the JTAG port on the device.

  Description:
    Enables the JTAG port on the device.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID

  Returns:
    None.

  Example:
  <code>
  </code>

  Remarks:
    None.
*/

void PLIB_DEVCON_JTAGPortEnable(DEVCON_MODULE_ID index);


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_JTAGPortDisable(DEVCON_MODULE_ID index)

  Summary:
    Disables the JTAG port on the device.

  Description:
    Disables the JTAG port on the device.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID

  Returns:
    None.

  Example:
  <code>
  </code>

  Remarks:
    None.
*/

void PLIB_DEVCON_JTAGPortDisable(DEVCON_MODULE_ID index);

// *****************************************************************************
/* Function:
    void PLIB_DEVCON_AnalogIOChargePumpEnable(DEVCON_MODULE_ID index)

  Summary:
    Enables the I/O Analog Charge Pump on the device.

  Description:
    Enables the I/O Analog Charge Pump on the device.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID

  Returns:
    None.

  Example:
  <code>
  </code>

  Remarks:
    None.
*/

void PLIB_DEVCON_AnalogIOChargePumpEnable(DEVCON_MODULE_ID index);

// *****************************************************************************
/* Function:
    void PLIB_DEVCON_AnalogIOChargePumpDisable(DEVCON_MODULE_ID index)

  Summary:
    Disables the I/O Analog Charge Pump on the device.

  Description:
    Disables the I/O Analog Charge Pump on the device.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID

  Returns:
    None.

  Example:
  <code>
  </code>

  Remarks:
    None.
*/

void PLIB_DEVCON_AnalogIOChargePumpDisable(DEVCON_MODULE_ID index);
// *****************************************************************************
/* Function:
    void PLIB_DEVCON_TraceOutputEnable(DEVCON_MODULE_ID index)

  Summary:
    Enable trace outputs and start trace clock.

  Description:
    Enable trace outputs and start trace clock (trace probe must be present).

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID

  Returns:
    None.

  Example:
  <code>
  </code>

  Remarks:
*/

void PLIB_DEVCON_TraceOutputEnable(DEVCON_MODULE_ID index);


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_TraceOutputDisable(DEVCON_MODULE_ID index)

  Summary:
    Disable trace outputs and stop trace clock.

  Description:
    Disable trace outputs and stop trace clock.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID

  Returns:
    None.

  Example:
  <code>
  </code>

  Remarks:
*/

void PLIB_DEVCON_TraceOutputDisable(DEVCON_MODULE_ID index);


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_2WireJTAGEnableTDO(DEVCON_MODULE_ID index)

  Summary:
    Enable 2-Wire JTAG protocol to use TDO.

  Description:
    Enable 2-Wire JTAG protocol to use TDO.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID

  Returns:
    None.

  Example:
  <code>
  </code>

  Remarks:
    The feature is not supported on all devices. Refer to the specific device
	data sheet to determine availability.
*/

void PLIB_DEVCON_2WireJTAGEnableTDO(DEVCON_MODULE_ID index);


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_2WireJTAGDisableTDO(DEVCON_MODULE_ID index)

  Summary:
    Disable 2-Wire JTAG protocol use of TDO.

  Description:
    Disable 2-Wire JTAG protocol use of TDO.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID

  Returns:
    None.

  Example:
  <code>
  </code>

  Remarks:
    The feature is not supported on all devices. Refer to the specific device
	data sheet to determine availability.
*/

void PLIB_DEVCON_2WireJTAGDisableTDO(DEVCON_MODULE_ID index);


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_IgnoreDebugFreezeEnable(DEVCON_MODULE_ID index,
                                             DEVCON_DEBUG_PERIPHERAL myPeripheral )

  Summary:
    Allows module to ignore FREEZE command from debugger and continue running.

  Description:
    Allows module to ignore FREEZE command from debugger and continue running.

  Precondition:
    None

  Parameters:
    index - Always DEVCON_ID
    myPeripheral - DEVCON_DEBUG_USB, DEVCON_DEBUG_UART1, DEVCON_DEBUG_UART2,
                    DEVCON_DEBUG_SPI1, or  DEVCON_DEBUG_ALL (for all modules)

  Returns:
    None.

  Example:
  <code>
    PLIB_DEVCON_DebugIgnoreFreezeEnable(DEVCON_ID,DEVCON_DEBUG_ALL);
    PLIB_DEVCON_DebugIgnoreFreezeDisable(DEVCON_ID,DEVCON_DEBUG_SPI1);
  </code>

  Remarks:
    The feature is not supported on all devices. Refer to the specific device
	data sheet to determine availability.
    Peripherals can be ORed together.
*/

void PLIB_DEVCON_IgnoreDebugFreezeEnable(DEVCON_MODULE_ID index,
                                         DEVCON_DEBUG_PERIPHERAL myPeripheral );


// *****************************************************************************
/* Function:
    void PLIB_DEVCON_IgnoreDebugFreezeDisable(DEVCON_MODULE_ID index,
                                              DEVCON_DEBUG_PERIPHERAL myPeripheral )

  Summary:
    Module stops when commanded by debugger.

  Description:
    Module stops when commanded by debugger.

  Precondition:
    None

  Parameters:
    index - Always DEVCON_ID
    myPeripheral - DEVCON_DEBUG_USB, DEVCON_DEBUG_UART1, DEVCON_DEBUG_UART2,
                    DEVCON_DEBUG_SPI1, or  DEVCON_DEBUG_ALL (for all modules)

  Returns:
    None.

  Example:
  <code>
    PLIB_DEVCON_DebugIgnoreFreezeEnable(DEVCON_ID,DEVCON_DEBUG_ALL);
    PLIB_DEVCON_DebugIgnoreFreezeDisable(DEVCON_ID,DEVCON_DEBUG_SPI1);
  </code>

  Remarks:
    The feature is not supported on all devices. Refer to the specific device
	data sheet to determine availability.
    Peripherals can be ORed together.
*/

void PLIB_DEVCON_IgnoreDebugFreezeDisable(DEVCON_MODULE_ID index,
                                          DEVCON_DEBUG_PERIPHERAL myPeripheral );


// *****************************************************************************
/* Function:
    uint8_t PLIB_DEVCON_DeviceVersionGet(DEVCON_MODULE_ID index)

  Summary:
    Get device version.

  Description:
    Get device version.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID

  Returns:
    Device Version.

  Example:
  <code>
  </code>

  Remarks:
    None.
*/

uint8_t PLIB_DEVCON_DeviceVersionGet(DEVCON_MODULE_ID index);


// *****************************************************************************
/* Function:
    uint32_t PLIB_DEVCON_DeviceIdGet(DEVCON_MODULE_ID index)

  Summary:
    Get device identifier.

  Description:
    Get device identifier.

  Precondition:
    None.

  Parameters:
    index - Always DEVCON_ID

  Returns:
    Device Version.

  Example:
  <code>
  </code>

  Remarks:
    None.
*/

uint32_t PLIB_DEVCON_DeviceIdGet(DEVCON_MODULE_ID index);


//******************************************************************************
/* Function :  PLIB_DEVCON_SystemLock( DEVCON_MODULE_ID index )

  Summary:
    Execute the system lock sequence.

  Description:
    This function executes the system lock sequence.

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:

  Remarks:
    Should only be called after PLIB_DEVCON_SystemUnlock and the action that
    required the unlock have been performed.
*/

void PLIB_DEVCON_SystemLock( DEVCON_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_DEVCON_SystemUnlock( DEVCON_MODULE_ID index )

  Summary:
    Execute the system unlock sequence.

  Description:
    This function executes the system unlock sequence.

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    None.

  Remarks:
    None.
*/

void PLIB_DEVCON_SystemUnlock( DEVCON_MODULE_ID index );


// *****************************************************************************
// *****************************************************************************
// Section: DEVCON Peripheral Library Exists API Routines
// *****************************************************************************
// *****************************************************************************
/* The functions below indicate the existence of the features on the device.
*/

//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsAlternateClock( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the AlternateClock feature exists on the DEVCON module

  Description:
    This function identifies whether the AlternateClock feature is available on
	the DEVCON module.
    When this function returns true, these functions are supported on the device:
    - PLIB_DEVCON_AlternateClockEnable
    - PLIB_DEVCON_AlternateClockDisable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The AlternateClock feature is supported on the device
    - false  - The AlternateClock feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsAlternateClock( DEVCON_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsDeviceRegsLockUnlock( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the DeviceRegsLockUnlock feature exists on the DEVCON module

  Description:
    This function identifies whether the DeviceRegsLockUnlock feature is available
	on the DEVCON module.
    When this function returns true, these functions are supported on the device:
    - PLIB_DEVCON_DeviceRegistersLock
    - PLIB_DEVCON_DeviceRegistersUnlock

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The DeviceRegsLockUnlock feature is supported on the device
    - false  - The DeviceRegsLockUnlock feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsDeviceRegsLockUnlock( DEVCON_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsUSB_PHY_SleepModeSet( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the USB_PHY_SleepModeSet feature exists on the DEVCON module

  Description:
    This function identifies whether the USB_PHY_SleepModeSet feature is available
	on the DEVCON module.
    When this function returns true, this function is supported on the device:
    - PLIB_DEVCON_USBPHYSleepModeSet

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The USB_PHY_SleepModeSet feature is supported on the device
    - false  - The USB_PHY_SleepModeSet feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsUSB_PHY_SleepModeSet( DEVCON_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsECCModes( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the ECCModes feature exists on the DEVCON module

  Description:
    This function identifies whether the ECCModes feature is available on the
	DEVCON module.
    When this function returns true, this function is supported on the device:
    - PLIB_DEVCON_FlashErrCorrectionModeSet

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The ECCModes feature is supported on the device
    - false  - The ECCModes feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsECCModes( DEVCON_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsJTAGEnable( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the JTAGEnable feature exists on the DEVCON module

  Description:
    This function identifies whether the JTAGEnable feature is available on the
	DEVCON module.
    When this function returns true, these functions are supported on the device:
    - PLIB_DEVCON_JTAGPortEnable
    - PLIB_DEVCON_JTAGPortDisable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The JTAGEnable feature is supported on the device
    - false  - The JTAGEnable feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsJTAGEnable( DEVCON_MODULE_ID index );

//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsAnalogChargePumpControl( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the I/O Analog Charge Pump feature exists on the DEVCON module

  Description:
    This function identifies whether the I/O Analog Charge Pump feature is available
    on the DEVCON module.
    When this function returns true, these functions are supported on the device:
    - PLIB_DEVCON_AnalogIOChargePumpEnable
    - PLIB_DEVCON_AnalogIOChargePumpDisable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The I/O Analog Charge Pump feature feature is supported on the device
    - false  - The I/O Analog Charge Pump feature feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsAnalogChargePumpControl( DEVCON_MODULE_ID index );
//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsTraceOutput( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the TraceOutput feature exists on the DEVCON module

  Description:
    This function identifies whether the TraceOutput feature is available on the
	DEVCON module.
    When this function returns true, these functions are supported on the device:
    - PLIB_DEVCON_TraceOutputEnable
    - PLIB_DEVCON_TraceOutputDisable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The TraceOutput feature is supported on the device
    - false  - The TraceOutput feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsTraceOutput( DEVCON_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsJTAGUsesTDO( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the JTAGUsesTDO feature exists on the DEVCON module

  Description:
    This function identifies whether the JTAGUsesTDO feature is available on the
	DEVCON module.
    When this function returns true, these functions are supported on the device:
    - PLIB_DEVCON_2WireJTAGEnableTDO
    - PLIB_DEVCON_2WireJTAGDisableTDO

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The JTAGUsesTDO feature is supported on the device
    - false  - The JTAGUsesTDO feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsJTAGUsesTDO( DEVCON_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsIgnoreDebugFreeze( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the IgnoreDebugFreeze feature exists on the DEVCON module

  Description:
    This function identifies whether the IgnoreDebugFreeze feature is available
	on the DEVCON module.
    When this function returns true, these functions are supported on the device:
    - PLIB_DEVCON_DebugIgnoreFreezeEnable
    - PLIB_DEVCON_IgnoreDebugFreezeDisable

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The IgnoreDebugFreeze feature is supported on the device
    - false  - The IgnoreDebugFreeze feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsIgnoreDebugFreeze( DEVCON_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsDeviceVerAndId( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the DeviceVerAndId feature exists on the DEVCON module

  Description:
    This function identifies whether the DeviceVerAndId feature is available on
	the DEVCON module.
    When this function returns true, these functions are supported on the device:
    - PLIB_DEVCON_DeviceVersionGet
    - PLIB_DEVCON_DeviceIdGet

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The DeviceVerAndId feature is supported on the device
    - false  - The DeviceVerAndId feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsDeviceVerAndId( DEVCON_MODULE_ID index );


//******************************************************************************
/* Function :  PLIB_DEVCON_ExistsSystemLockUnlock( DEVCON_MODULE_ID index )

  Summary:
    Identifies whether the SysLockUnlock feature exists on the DEVCON module

  Description:
    This function identifies whether the SysLockUnlock feature is available on
	the DEVCON module.
    When this function returns true, these functions are supported on the device:
    - PLIB_DEVCON_SystemUnlock
    - PLIB_DEVCON_SystemLock

  Preconditions:
    None.

  Parameters:
    index           - Identifier for the device instance

  Returns:
    - true   - The SysLockUnlock feature is supported on the device
    - false  - The SysLockUnlock feature is not supported on the device

  Remarks:
    None.
*/

bool PLIB_DEVCON_ExistsSystemLockUnlock( DEVCON_MODULE_ID index );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END


// ****************************************************************************
// ****************************************************************************
// Section: Included Files (continued)
// ****************************************************************************
// ****************************************************************************
/*  The file included below  maps the interface definitions above to appropriate
    implementations defined in the implementation (imp) file(s).
*/
//#include "peripheral/devcon/processor/devcon_processor.h"

#endif //_PLIB_DEVCON_H

//******************************************************************************
/* End of File
*/

