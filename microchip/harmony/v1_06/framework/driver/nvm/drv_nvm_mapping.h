/*******************************************************************************
  NVM Device Driver interface names mapping

  Company:
    Microchip Technology Inc.

  File Name:
    drv_nvm_mapping.h

  Summary:
    NVM Device Driver Interface names mapping

  Description:
    This header file contains the data type definitions.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END
#ifndef _DRV_NVM_MAPPING_H
#define _DRV_NVM_MAPPING_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>

#include "system_config.h"
//#include "driver/nvm/drv_nvm_static_single.h"
//#include "driver/nvm/drv_nvm_static_multi.h"
#include "peripheral/nvm/plib_nvm.h"


// *****************************************************************************
// *****************************************************************************
// Section: Build Parameter Checking
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Macro: DRV_NVM_INSTANCES_NUMBER Check

  Summary:
    Checks the DRV_NVM_INSTANCES_NUMBER definition

  Description:
    If DRV_NVM_INSTANCES_NUMBER is greater than the number of
    NVM instances available on the part, an error is generated.

  Remarks:
    The _NVM_EXISTS is a processor-specific value defined by the processor
    headers in the PLIB.

    If the customer does not specify the number of driver instances to allocate
    it defaults to then number of NVM instances on the part.
*/

//#if defined(DRV_NVM_INSTANCES_NUMBER)
//
//    #if (DRV_NVM_INSTANCES_NUMBER > DRV_NVM_NUMBER_OF_MODULES )
//
//        #error "The number of NVM instances configured is more than the available NVMs on the part"
//
//    #endif
//
//#endif

// *****************************************************************************
// *****************************************************************************
// Section: NVM Driver Static API Name Generation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Macro: _DRV_NVM_MAKE_NAME(name)

  Summary:
    Creates an instance-specific static interface name

  Description:
     This macro creates the instance-specific name of the given static interface
     routine by inserting the index number into the name.

     Example DRV_NVM_Initialize becomes DRV_NVM1_Initialize for an index of 1.

  Remarks:
    Multi-client configurations add the word "multi" to the API name, single-
    client configurations do not.

    Example DRV_NVM_Initialize becomes DRV_NVM1multi_Initialize for an index
    of 1.
*/

#if !defined(DRV_NVM_INSTANCES_NUMBER)

    #if !defined(DRV_NVM_CLIENTS_NUMBER)

        // Static Single-Client Interface Name Generation
        #define _DRV_NVM_STATIC_API_A(index, name)    DRV_NVM ## index ## _ ## name

    #else

        // Static multi-client interface name generation
        #define _DRV_NVM_STATIC_API_A(index , name)   DRV_NVM ## index ## multi_ ## name

    #endif

    // Static Naming Macros
    #define _DRV_NVM_STATIC_API(index , name)     _DRV_NVM_STATIC_API_A(index , name)
    #define _DRV_NVM_MAKE_NAME(name)              _DRV_NVM_STATIC_API(DRV_NVM_CONFIG_INDEX, name)

#else // dynamic build

        // Dynamic Interface Name Generation
        #define _DRV_NVM_MAKE_NAME(name)  DRV_NVM_ ## name

#endif

// *****************************************************************************
// *****************************************************************************
// Section: ADC Driver Static API Mapping
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Static Interface Mapping

  Summary:
    Maps the dynamic interface calls to appropriate static interface

  Description:
    These macros map calls to the dynamic interface routines to calls to the
    appropriate instance-specific static interface routine when a static build
    (DRV_NVM_INSTANCES_NUMBER is not defined) is configured.

    Example:

        DRV_NVM_Status(DRV_NVM_CONFIG_INDEX_1);

    Becomes:

        DRV_NVM_Status();

  Remarks:
    Static configuration eliminate the need to pass the object parameter.
    However, the index is "returned" as the object handle (from the
    "Initialize" routine) to allow code written for the dynamic API to continue
    to build when using a static configuration.

    Example:

        object = DRV_NVM_Initialize(DRV_NVM_CONFIG_INDEX_1, &initData);

    Becomes:

        object = ( DRV_NVM1_Initialize(&initData), DRV_NVM_CONFIG_INDEX);

    Static single-client configurations also eliminate the client handle
    parameter.  However, the index (the driver-object index) is "returned" from
    the "Open" routine to allow code written for multi-client drivers to build
    when using a static single-open configuration.

    Example:

        handle = DRV_NVM_Open(DRV_NVM_CONFIG_INDEX_1, intent);

    Becomes:

        handle = ( DRV_NVM_Open(intent), DRV_NVM_CONFIG_INDEX);
*/

#if !defined(DRV_NVM_INSTANCES_NUMBER) // static

    #if !defined(DRV_NVM_CLIENTS_NUMBER)    // Single-Client

        #define DRV_NVM_Initialize(sysID, inData)          (_DRV_NVM_MAKE_NAME(Initialize)(inData), DRV_NVM_CONFIG_INDEX)

        #define DRV_NVM_Reinitialize(sysObj, inData)        _DRV_NVM_MAKE_NAME(Reinitialize)(inData)

        #define DRV_NVM_Deinitialize(sysObj)                _DRV_NVM_MAKE_NAME(Deinitialize)()

        #define DRV_NVM_Status(sysObj)                      _DRV_NVM_MAKE_NAME(Status)()

        #define DRV_NVM_Tasks_Write(sysObj)                  _DRV_NVM_MAKE_NAME(Tasks_Write)()

        #define DRV_NVM_Tasks_Read(sysObj)                  _DRV_NVM_MAKE_NAME(Tasks_Read)()

        #define DRV_NVM_Tasks_Erase(sysObj)                  _DRV_NVM_MAKE_NAME(Tasks_Erase)()

        #define DRV_NVM_Open(sysID, intent)                (_DRV_NVM_MAKE_NAME(Open)(intent), DRV_NVM_CONFIG_INDEX)

        #define DRV_NVM_Close(handle)                       _DRV_NVM_MAKE_NAME(Close)()

        #define DRV_NVM_ClientStatus(handle)                _DRV_NVM_MAKE_NAME(ClientStatus)()

        #define DRV_NVM_BufferAdd(handle,value)             _DRV_NVM_MAKE_NAME(BufferAdd)(value)

        #define DRV_NVM_BufferStatus(handle, bhandle)       _DRV_NVM_MAKE_NAME(BufferStatus)(bhandle)
	
    #else //Multi-Client

        #define DRV_NVM_Initialize(sysID, inData)          (_DRV_NVM_MAKE_NAME(Initialize)(inData), DRV_NVM_CONFIG_INDEX)

        #define DRV_NVM_Reinitialize(sysObj, inData)        _DRV_NVM_MAKE_NAME(Reinitialize)(inData)

        #define DRV_NVM_Deinitialize(sysObj)                _DRV_NVM_MAKE_NAME(Deinitialize)()

        #define DRV_NVM_Status(sysObj)                      _DRV_NVM_MAKE_NAME(Status)()

        #define DRV_NVM_Tasks_Write(sysObj)                  _DRV_NVM_MAKE_NAME(Tasks_Write)()

        #define DRV_NVM_Tasks_Read(sysObj)                  _DRV_NVM_MAKE_NAME(Tasks_Read)()

        #define DRV_NVM_Tasks_Erase(sysObj)                  _DRV_NVM_MAKE_NAME(Tasks_Erase)()

        #define DRV_NVM_Open(sysID, intent)                _DRV_NVM_MAKE_NAME(Open)(intent)

        #define DRV_NVM_Close(handle)                       _DRV_NVM_MAKE_NAME(Close)(handle)

        #define DRV_NVM_ClientStatus(handle)                _DRV_NVM_MAKE_NAME(ClientStatus)(handle)

        #define DRV_NVM_BufferAdd(handle,value)             _DRV_NVM_MAKE_NAME(BufferAdd)(handle,value)

        #define DRV_NVM_BufferStatus(handle, bhandle)       _DRV_NVM_MAKE_NAME(BufferStatus)(handle, bhandle)

#endif

#endif


#endif // #ifndef _DRV_NVM_MAPPING_H

/*******************************************************************************
 End of File
*/

