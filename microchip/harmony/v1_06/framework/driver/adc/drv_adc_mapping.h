/*******************************************************************************
  ADC Device Driver interface names mapping

  Company:
    Microchip Technology Inc.

  File Name:
    drv_adc_mapping.h

  Summary:
    ADC Device Driver Interface names mapping.

  Description:
    This header file contains the data type definitions.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _DRV_ADC_MAPPING_H
#define _DRV_ADC_MAPPING_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
/* Note:  See the bottom of file for implementation header include files.
*/

#include <stdint.h>
#include <stdbool.h>

#include "system_config.h"
#include "driver/adc/drv_adc_static_single.h"
#include "driver/adc/drv_adc_static_multi.h"


// *****************************************************************************
// *****************************************************************************
// Section: Build Parameter Checking
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Macro: DRV_ADC_INSTANCES_NUMBER Check

  Summary:
    Checks the DRV_ADC_INSTANCES_NUMBER definition.

  Description:
    If DRV_ADC_INSTANCES_NUMBER is greater than the number of
    ADC instances available on the part, an error is generated.

  Remarks:
    The _ADC_EXISTS is a processor-specific value defined by the processor
    headers in the peripheral library.
    
    If the configuration does not specify the number of driver instances to 
    allocate it defaults to then number of ADC instances on the device.
*/

#if defined(DRV_ADC_INSTANCES_NUMBER)

    #if (DR_ADC_DRIVER_OBJECTS_NUMBER > _ADC_EXISTS )

        #error "The number of ADC instances configured is more than the available ADCs on the part"

    #endif

#endif


// *****************************************************************************
// *****************************************************************************
// Section: ADC Driver Static API Name Generation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Macro: _DRV_ADC_MAKE_NAME(name)

  Summary:
    Creates an instance-specific static interface name.

  Description:
     This macro creates the instance-specific name of the given static interface
     routine by inserting the index number into the name.
     
     Example: DRV_ADC_Initialize becomes DRV_ADC1_Initialize for an index of 1.
     
  Remarks:
    Multi-client configurations add the word "multi" to the API name, single-
    client configurations do not.
    
    Example: DRV_ADC_Initialize becomes DRV_ADC1multi_Initialize for an index
    of 1.
*/


#if !defined(DRV_ADC_INSTANCES_NUMBER)

    // Static builds use static naming to reduce calling overhead. 
    #if !defined(DRV_ADC_CLIENTS_NUMBER)
    
        // Static Single-Client Interface Name Generation
        #define _DRV_ADC_STATIC_API_SINGLE(index, name)     DRV_ADC ## index ## _ ## name
        #define _DRV_ADC_STATIC_API(index, name)            _DRV_ADC_STATIC_API_SINGLE(index, name)

    #else // ( DRV_ADC_CLIENTS_NUMBER >= 1 )

        // Static Multi-Client Interface Name Generation
        #define _DRV_ADC_STATIC_API_MULTI(index, name)      DRV_ADC ## index ## multi_ ## name
        #define _DRV_ADC_STATIC_API(index, name)            _DRV_ADC_STATIC_API_MULTI(index, name)

    #endif

    // Static naming Macros
    #define _DRV_ADC_MAKE_NAME(name)                        _DRV_ADC_STATIC_API(DRV_ADC_INDEX, name)
        
#else // (DRV_ADC_CONFIG_BUILD_TYPE == DRV_BUILD_TYPE_DYNAMIC)

        // Dynamic Interface Name Generation
        #define _DRV_ADC_MAKE_NAME(name)  DRV_ADC_ ## name

#endif


// *****************************************************************************
// *****************************************************************************
// Section: ADC Driver Static API Mapping
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Static Interface Mapping

  Summary:
    Maps the dynamic interface calls to appropriate static interface.

  Description:
    These macros map calls to the dynamic interface routines to calls to the 
    appropriate instance-specific static interface routine when a static build
    (DRV_ADC_INSTANCES_NUMBER is not defined) is configured.
    
    Example:
    
        DRV_ADC_Status(DRV_ADC_INDEX_1);
        
    Becomes:
    
        DRV_ADC_Status();
        
  Remarks:
    Static configuration eliminate the need to pass the object parameter.  
    However, the index is "returned" as the object handle (from the 
    "Initialize" routine) to allow code written for the dynamic API to continue
    to build when using a static configuration.
    
    Example:
    
        object = DRV_ADC_Initialize(DRV_ADC_INDEX_1, &initData);
        
    Becomes:
    
        object = ( DRV_ADC1_Initialize(&initData), DRV_ADC_INDEX );
        
    Static single-client configurations also eliminate the client handle 
    parameter.  However, the index (the driver-object index) is "returned" from
    the "Open" routine to allow code written for multi-client drivers to build
    when using a static single-open configuration.
    
    Example:
    
        handle = DRV_ADC_Open(DRV_ADC_INDEX_1, intent);
        
    Becomes:
    
        handle = ( DRV_ADC_Open(intent), DRV_ADC_INDEX );
*/

#if !defined( DRV_ADC_INSTANCES_NUMBER )   // Static configuration
    
    #if !defined( DRV_ADC_CLIENTS_NUMBER )    // single-Client
    
        #define DRV_ADC_Initialize(sysID, inData)                  (_DRV_ADC_MAKE_NAME(Initialize)(inData), DRV_ADC_INDEX)

        #define DRV_ADC_Reinitialize(sysObj,inData)                 _DRV_ADC_MAKE_NAME(Reinitialize)(inData)

        #define DRV_ADC_Deinitialize(sysObj)                        _DRV_ADC_MAKE_NAME(Deinitialize)()

        #define DRV_ADC_Status(sysObj)                              _DRV_ADC_MAKE_NAME(Status)()

        #define DRV_ADC_Tasks(sysObj)                               _DRV_ADC_MAKE_NAME(Tasks)()

        #define DRV_ADC_Open(sysID, intent)                        (_DRV_ADC_MAKE_NAME(Open)(intent), DRV_ADC_INDEX)

        #define DRV_ADC_Close(handle)                               _DRV_ADC_MAKE_NAME(Close)()

        #define DRV_ADC_ClientStatus(handle)                        _DRV_ADC_MAKE_NAME(ClientStatus)()

        #define DRV_ADC_Start(handle)                               _DRV_ADC_MAKE_NAME(Start)()

        #define DRV_ADC_Stop(handle)                                _DRV_ADC_MAKE_NAME(Stop)()

        #define DRV_ADC_SamplesAvailable(handle)                    _DRV_ADC_MAKE_NAME(SamplesAvailable)()

        #define DRV_ADC_InputsRegister(handle, inputsMask)          _DRV_ADC_MAKE_NAME(InputsRegister)(inputsMask)

        #define DRV_ADC_SamplesRead(handle, buffer, numBytes)       _DRV_ADC_MAKE_NAME(SamplesRead)(buffer, numBytes)

        #define DRV_ADC_SamplesReadLatest(handle, buffer, numBytes) _DRV_ADC_MAKE_NAME(SamplesReadLatest)(buffer, numBytes)

    #else // multi client, ( DRV_ADC_CLIENTS_NUMBER >= 1 )

        #define DRV_ADC_Initialize(sysID, inData)                  (_DRV_ADC_MAKE_NAME(Initialize)(inData), DRV_ADC_INDEX)

        #define DRV_ADC_Reinitialize(sysObj,inData)                 _DRV_ADC_MAKE_NAME(Reinitialize)(inData)

        #define DRV_ADC_Deinitialize(sysObj)                        _DRV_ADC_MAKE_NAME(Deinitialize)()

        #define DRV_ADC_Status(sysObj)                              _DRV_ADC_MAKE_NAME(Status)()

        #define DRV_ADC_Tasks(sysObj)                               _DRV_ADC_MAKE_NAME(Tasks)()

        #define DRV_ADC_Open(sysID, intent)                         _DRV_ADC_MAKE_NAME(Open)(intent)

        #define DRV_ADC_Close(handle)                               _DRV_ADC_MAKE_NAME(Close)(handle)

        #define DRV_ADC_ClientStatus(handle)                        _DRV_ADC_MAKE_NAME(ClientStatus)(handle)

        #define DRV_ADC_Start(handle)                               _DRV_ADC_MAKE_NAME(Start)(handle)

        #define DRV_ADC_Stop(handle)                                _DRV_ADC_MAKE_NAME(Stop)(handle)

        #define DRV_ADC_InputsRegister(handle, inputsMask)          _DRV_ADC_MAKE_NAME(InputsRegister)(handle, inputsMask)

        #define DRV_ADC_SamplesAvailable(handle)                    _DRV_ADC_MAKE_NAME(SamplesAvailable)(handle)

        #define DRV_ADC_SamplesRead(handle, buffer, numBytes)       _DRV_ADC_MAKE_NAME(SamplesRead)(handle, buffer, numBytes)

        #define DRV_ADC_SamplesReadLatest(handle, buffer, numBytes) _DRV_ADC_MAKE_NAME(SamplesReadLatest)(handle, buffer, numBytes)

    #endif

#endif


// *****************************************************************************
// *****************************************************************************
// Section: ADC Driver API Mapping
// *****************************************************************************
// *****************************************************************************
/* This section maps the max and min values of the ADC available on the part
   to the appropriate interface of the PLIB */

#define DRV_ADC_SampleMinGet(handle)   PLIB_ADC_SampleMinGet(0)

#define DRV_ADC_SampleMaxGet(handle)   PLIB_ADC_SampleMaxGet(0)


#endif // _DRV_ADC_MAPPING_H

/*******************************************************************************
 End of File
*/
