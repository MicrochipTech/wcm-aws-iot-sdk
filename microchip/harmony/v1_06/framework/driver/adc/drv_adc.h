/*******************************************************************************
  ADC Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    drv_adc.h

  Summary:
    ADC Driver interface definition.

  Description:
    The ADC device driver provides a simple interface to manage the ADC
    modules on Microchip microcontrollers.  This file defines the
    interface definition for the ADC driver.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _DRV_ADC_H
#define _DRV_ADC_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
/* Note:  A file that maps the interface definitions above to appropriate static
          implementations (depending on build mode) is included at the bottom of
          this file.
*/
#include <stdint.h>
#include <stdbool.h>

#include "system_config.h"
#include "system/common/sys_common.h"
#include "system/common/sys_module.h"
#include "system/debug/sys_debug.h"
#include "system/int/sys_int.h"  
#include "driver/driver_common.h"
#include "osal/osal.h"        
#include "peripheral/adc/plib_adc.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: ADC Driver Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Driver ADC Module Index Numbers

  Summary:
    ADC driver index definitions.

  Description:
    These constants provide ADC driver index definitions.

  Remarks:
    These constants should be used in place of hard-coded numeric literals.

    These values should be passed into the DRV_ADC_Initialize function
    to identify the driver instance in use.
*/

#define DRV_ADC_INDEX_0        0
#define DRV_ADC_INDEX_1        1
#define DRV_ADC_INDEX_2        2

// *****************************************************************************
/* ADC Driver Buffer Handle

  Summary:
    This type defines the ADC Driver Buffer handle.

  Description:
    This type defines the ADC Driver Buffer handle.

  Remarks:
    None.
*/

typedef uintptr_t DRV_ADC_BUFFER_HANDLE;

// *****************************************************************************
/* ADC Initialization Flags

  Summary:
    Identifies the initialization flags of the ADC module.

  Description:
    This data type identifies the initialization flags of the ADC module.

  Remarks:
    Not all modes are available on all devices.  Refer to the specific data
    sheet to determine availability.
*/

typedef enum {

    /* Stops the conversion on the interrupt */
    DRV_ADC_STOP_CONVERSION_ON_INTERRUPT    /*DOM-IGNORE-BEGIN*/ = (1 << 0) /*DOM-IGNORE-END*/,

    /* Alternate Input Sampling */
    DRV_ADC_ALTERNATE_INPUT_SAMPLING        /*DOM-IGNORE-BEGIN*/ = (1 << 1) /*DOM-IGNORE-END*/,

    /*Begin sampling automatically after previous conversion*/
    DRV_ADC_AUTO_SAMPLING                   /*DOM-IGNORE-BEGIN*/ = (1 << 2) /*DOM-IGNORE-END*/,

    /* Manual Sampling */
    DRV_ADC_MANUAL_SAMPLING                 /*DOM-IGNORE-BEGIN*/ = (1 << 3) /*DOM-IGNORE-END*/

} DRV_ADC_INIT_FLAGS ;


// *****************************************************************************
/* ADC Driver Initialization Data

  Summary:
    Defines the data required to initialize or reinitialize the ADC driver.

  Description:
    This structure defines the data required to initialize or reinitialize the 
    ADC driver.

  Remarks:
    Not all init features are available for all devices. Refer to the specific data
    sheet to determine availability.
*/

typedef struct
{
    /* System module initialization */
    SYS_MODULE_INIT                     moduleInit;

    /* Identifies ADC hardware module (PLIB-level) ID */
    ADC_MODULE_ID                       adcId;

    /* Selects input clock source */
    ADC_CLOCK_SOURCE                    adcClkSrc;

    /* Conversion clock value */
    ADC_CONVERSION_CLOCK                tadClk;

    /* Data acquisition time in auto sampling mode */
    ADC_ACQUISITION_TIME                acquisitionTime;

    /* Conversion Trigger Source */
    ADC_CONVERSION_TRIGGER_SOURCE       convTrgSrc;

    /* Initialization Flags */
    //TODO: PM - Look into elaborating init flags
    DRV_ADC_INIT_FLAGS                  initFlags;

    /* Voltage Reference Selection */
    ADC_VOLTAGE_REFERENCE               voltageReference;

    /* Samples Per Interrupt valid values = 0- 15*/
    ADC_SAMPLES_PER_INTERRUPT           samplesPerInterrupt;

    /* Result Format */
    ADC_RESULT_FORMAT                   resultFormat;

    /* Input Channel to convert */
    ADC_INPUTS_POSITIVE                 analogInput;

    /* Interrupt Source for the module */
    INT_SOURCE                          interruptSource;

} DRV_ADC_INIT ;


// *****************************************************************************
/* ADC Client Status

  Summary:
    Defines the client-specific status of the ADC driver.

  Description:
    This enumeration defines the client-specific status codes of the ADC 
    driver.
    
  Remarks:
    Returned by the DRV_ADC_ClientStatus function.
*/

typedef enum
{
    /* ADC Started */
    DRV_ADC_CLIENT_STATUS_STARTED
        /*DOM-IGNORE-BEGIN*/ = DRV_CLIENT_STATUS_READY + 2 /*DOM-IGNORE-END*/,

    /* stopped on error */
    DRV_ADC_CLIENT_STATUS_STOPPED
        /*DOM-IGNORE-BEGIN*/ = DRV_CLIENT_STATUS_READY + 1 /*DOM-IGNORE-END*/,

    /* Driver OK, ready for client operations */
    DRV_ADC_CLIENT_STATUS_READY
        /*DOM-IGNORE-BEGIN*/ = DRV_CLIENT_STATUS_READY + 0 /*DOM-IGNORE-END*/,

    /* An operation is currently in progress */
    DRV_ADC_CLIENT_STATUS_BUSY
        /*DOM-IGNORE-BEGIN*/ = DRV_CLIENT_STATUS_BUSY      /*DOM-IGNORE-END*/,

    /* Client in an invalid (or unopened) state */
    DRV_ADC_CLIENT_STATUS_INVALID
        /*DOM-IGNORE-BEGIN*/ = DRV_CLIENT_STATUS_ERROR - 0 /*DOM-IGNORE-END*/,

    /* Driver Overflowed */
    DRV_ADC_CLIENT_STATUS_OVERFLOW
        /*DOM-IGNORE-BEGIN*/ = DRV_CLIENT_STATUS_ERROR - 1 /*DOM-IGNORE-END*/,

    /* Input Set Read Buffer Too Small */
    DRV_ADC_CLIENT_STATUS_BUFFER_TOO_SMALL
        /*DOM-IGNORE-BEGIN*/ = DRV_CLIENT_STATUS_ERROR - 2 /*DOM-IGNORE-END*/,

} DRV_ADC_CLIENT_STATUS;

// ***********************************************************************
/* ADC Driver Buffer Status

  Summary:
    Specifies the status of the buffer for the read, write and erase
    operations.
	
  Description:
    ADC Driver Buffer Status
    
    This type specifies the status of the buffer for the read, write and
    erase operations.
	
  Remarks:
    None.                                                               
*/  

typedef enum
{
    /*Done OK and ready */
    DRV_ADC_BUFFER_COMPLETED          = 0 ,

    /*Scheduled but not started */
    DRV_ADC_BUFFER_QUEUED             = 1,

    /*Currently being in transfer */
    DRV_ADC_BUFFER_IN_PROGRESS        = 2,

    /*Unknown buffer */
    DRV_ADC_BUFFER_ERROR_UNKNOWN      = -1,

} DRV_ADC_BUFFER_STATUS;

// *****************************************************************************
/* ADC Driver Events

   Summary:
    Identifies the possible events that can result from a request.

   Description:
    This enumeration identifies the possible events that can result from a 
    Write, or Erase request caused by the client. Note that the ADC driver
    does not generate events for read.

   Remarks:
    One of these values is passed in the "event" parameter of the event 
    handling callback function that client registered with the driver by
    calling the DRV_ADC_EventHandlerSet function when a block 
    request is completed.
*/

typedef enum
{
    /* Buffer operation has been completed successfully. */
    /* Read/Write/Erase Complete */
    DRV_ADC_EVENT_BUFFER_COMPLETE 
       /* DOM-IGNORE-BEGIN */ = 1 /* DOM-IGNORE-END */,

    /* There was an error during the block operation */
    /* Read/Write/Erase Error */
    DRV_ADC_EVENT_BUFFER_ERROR
       /* DOM-IGNORE-BEGIN */ = -1 /* DOM-IGNORE-END */

} DRV_ADC_EVENT;

// *****************************************************************************
/* ADC Driver Event Handler Function Pointer

   Summary:
    Pointer to a ADC Driver Event handler function

   Description
    This data type defines the required function signature for the 
    ADC event handling callback function. A client must register
    a pointer to an event handling function whose function signature (parameter
    and return value types) match the types specified by this function pointer
    in order to receive event calls back from the driver.
    
    The parameters and return values and return value are described here and
    a partial example implementation is provided.

  Parameters:
    event           - Identifies the type of event
    
    commandHandle   - Handle returned from the Read/Write/Erase requests
    
    context         - Value identifying the context of the application that
                      registered the event handling function

  Returns:
    None.

  Example:
    <code>
    void APP_MyBufferEventHandler
    (
        DRV_ADC_EVENT event,
        DRV_ADC_BUFFER_HANDLE bufferHandle, 
        uintptr_t context
    )
    {
        MY_APP_DATA_STRUCT pAppData = (MY_APP_DATA_STRUCT) context;
        
        switch(event)
        {
            case DRV_ADC_EVENT_BUFFER_COMPLETE:

                // Handle the completed buffer. 
                break;
            
            case DRV_ADC_EVENT_BUFFER_ERROR:
            default:

                // Handle error.
                break;
        }
    }
    </code>

  Remarks:
    If the event is DRV_ADC_EVENT_BUFFER_COMPLETE, it means that the
    write or a erase operation was completed successfully. 
    
    If the event is DRV_ADC_EVENT_BUFFER_ERROR, it means that the scheduled
    operation was not completed successfully.
     
    The context parameter contains the a handle to the client context,  
    provided at the time the event handling function was  registered using the
    DRV_ADC_EventHandlerSet function. This context handle value is
    passed back to the client as the "context" parameter.  It can be any value
    necessary to identify the client context or instance (such as a pointer to
    the client's data) instance of the client that made the read/write/erase
    request.

    The event handler function executes in the driver peripheral's interrupt
    context when the driver is configured for interrupt mode operation. It is
    recommended of the application to not perform process intensive or blocking
    operations with in this function.

*/

typedef void ( * DRV_ADC_EVENT_HANDLER ) 
(
    DRV_ADC_EVENT event, 
    DRV_ADC_BUFFER_HANDLE bufferHandle,
    uintptr_t context
);

// *****************************************************************************
// *****************************************************************************
// Section: ADC Driver Module Interface Routines
// *****************************************************************************
// *****************************************************************************

//***********************************************************************************
/*  Function:
       SYS_MODULE_OBJ DRV_ADC_Initialize( const SYS_MODULE_INDEX index,
                                          const SYS_MODULE_INIT * const init )
    
  Summary:
    Initializes the ADC driver.
	<p><b>Implementation:</b> Static/Dynamic</p>
	
  Description:
    This function initializes the ADC driver, making it ready for clients
    to open and use it.
	
  Precondition:
    None.
	
  Parameters:
    drvIndex -  Index for the driver instance to be initialized
    init -      Pointer to a data structure containing any data necessary to
                initialize the driver. This pointer may be null if no data
                is required because static overrides have been provided.
				
  Returns:
    If successful, returns a valid handle to a driver instance object.
    Otherwise, it returns SYS_MODULE_OBJ_INVALID.
	
  Example:
    <code>
    DRV_ADC_INIT    init;
    SYS_MODULE_OBJ  objectHandle;
    
    // Populate the init structure
    init.moduleInit.value           = SYS_MODULE_POWER_RUN_FULL;
    init.adcID                      = ADC_ID_1;
    init.initFlags                  = DRV_ADC_AUTO_SAMPLING;
    init.acquisitionTime            = ADC_ACQUISITION_TIME_15_TAD;
    init.voltageReference           = ADC_VREF_POS_TO_VDD_VREF_NEG_TO_VSS;
    init.conversionClockPrescaler   = ADC_CONV_CLOCK_5_TCY;
    init.conversionClockSource      = ADC_CLOCK_SRC_INTERNAL_RC;
    init.conversionTriggerSource    = ADC_CONVERSION_TRIGGER_INTERNAL_COUNT;
    init.samplesPerInterrupt        = ADC_SAMPLE_PER_INTERRUPT_AT_EACH_SAMPLE;
    init.resultFormat               = ADC_RESULT_FORMAT_INTEGER_16BIT;
    init.analogInput                = ADC_INPUT_AN2
    init.interruptSource            = INT_SOURCE_ADC_1;
    
    // Initialize the ADC driver
    objectHandle = DRV_ADC_Initialize(DRV_ADC_INDEX_0, (SYS_MODULE_INIT*)&init);
    if (SYS_MODULE_OBJ_INVALID == objectHandle)
    {
        // Handle error
    }
    </code>
	
  Remarks:
    This function must be called before any other ADC function is called.
    
    This function should only be called once during system initialization
    unless DRV_ADC_Deinitialize is called to deinitialize the driver
    instance.
    
    This function will NEVER block for hardware access. If the operation
    requires time to allow the hardware to reinitialize, it will be
    reported by the DRV_ADC_Status operation. The system must use
    DRV_ADC_Status to find out when the driver is in the ready state.
    
    Build configuration options may be used to statically override options
    in the "init" structure and will take precedence over initialization
    data passed using this function.                                                
  ***********************************************************************************/

SYS_MODULE_OBJ DRV_ADC_Initialize( const SYS_MODULE_INDEX drvIndex,
                                   const SYS_MODULE_INIT * const init );

// *****************************************************************************
/* Function:
   void DRV_ADC_Deinitialize( SYS_MODULE_OBJ object )

  Summary:
    Deinitializes the specified instance of the ADC driver module.
	<p><b>Implementation:</b> Static/Dynamic</p>

  Description:
    This function deinitializes the specified instance of the ADC driver module, 
    disabling its operation (and any hardware).  Invalidates all the internal data.

  Precondition:
    The DRV_ADC_Initialize function should have been called before calling this
    function.

  Parameters:
    object          - Driver object handle, returned from DRV_ADC_Initialize

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ      object;     //  Returned from DRV_ADC_Initialize
    SYS_STATUS          status;


    DRV_ADC_Deinitialize(object);

    status = DRV_ADC_Status(object);
    if (SYS_MODULE_DEINITIALIZED == status)
    {
        // Check again later if you need to know 
        // when the driver is deinitialized.
    }
    </code>

  Remarks:
    Once the Initialize operation has been called, the Deinitialize operation
    must be called before the Initialize operation can be called again.

    This function will NEVER block waiting for hardware. If the operation
    requires time to allow the hardware to complete, this will be reported by 
    the DRV_ADC_Status operation.  The system has to use DRV_ADC_Status to find 
    out when the module is in the ready state.
*/

void DRV_ADC_Deinitialize( SYS_MODULE_OBJ object );


// *****************************************************************************
/* Function:
    SYS_STATUS DRV_ADC_Status ( SYS_MODULE_OBJ object )

  Summary:
    Provides the current status of the ADC driver module.
	<p><b>Implementation:</b> Dynamic</p>

  Description:
    This function provides the current status of the ADC driver module.

  Precondition:
    The DRV_ADC_Initialize function must have been called before calling this
    function.

  Parameters:
    object          - Driver object handle, returned from the DRV_ADC_Initialize
                      function

  Returns:
    SYS_STATUS_READY          - Indicates that the driver is busy with a 
                                previous system level operation and cannot start
                                another
                                
                                Note: Any value greater than SYS_STATUS_READY is
                                also a normal running state in which the driver
                                is ready to accept new operations.
                                
    SYS_STATUS_BUSY           - Indicates that the driver is busy with a 
                                previous system level operation and cannot start
                                another
                                
    SYS_STATUS_ERROR          - Indicates that the driver is in an error state
                                
                                Note: Any value less than SYS_STATUS_ERROR is
                                also an error state.
    
    SYS_MODULE_DEINITIALIZED  - Indicates that the driver has been deinitialized
                                
                                Note:  This value is less than SYS_STATUS_ERROR

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_ADC_Initialize
    SYS_STATUS          status;

    status = DRV_ADC_Status(object);
    else if (SYS_STATUS_ERROR >= status)
    {
        // Handle error
    }
    </code>

  Remarks:
    The this operation can be used to determine when any of the driver's module 
    level operations has completed.
    
    If the status operation returns SYS_STATUS_BUSY, the a previous operation 
    has not yet completed.  Once the status operation returns SYS_STATUS_READY, 
    any previous operations have completed.

    The value of SYS_STATUS_ERROR is negative (-1).  Any value less than that is
    also an error state.

    This function will NEVER block waiting for hardware.
    
    If the Status operation returns an error value, the error may be cleared by
    calling the reinitialize operation.  If that fails, the deinitialize
    operation will need to be called, followed by the initialize operation to
    return to normal operations.
*/

SYS_STATUS DRV_ADC_Status ( SYS_MODULE_OBJ object );


//***************************************************************************
/* Function:
       void DRV_ADC_Tasks ( SYS_MODULE_OBJ object )
    
  Summary:
    Maintains the driver's state machine and implements its ISR.
	<p><b>Implementation:</b> Dynamic</p>
  Description:
    This function is used to maintain the driver's internal state machine
    and implement its ISR for interrupt-driven implementations.
	
  Precondition:
    The DRV_ADC_Initialize function must have been called for the specified
    ADC driver instance.
	
  Parameters:
    object -  Object handle for the specified driver instance (returned from
              DRV_ADC_Initialize)
			  
  Returns:
    None.
	
  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_ADC_Initialize
    
    while (true)
    {
        DRV_ADC_Tasks (object);
    
        // Do other tasks
    }
    </code>
	
  Remarks:
    This function is normally not called directly by an application. It is
    called by the system's Tasks function (SYS_Tasks) or by the appropriate
    raw ISR.
    
    This function may execute in an ISR context and will never block or
    access any resources that may cause it to block.                        
  ***************************************************************************/

void DRV_ADC_Tasks( SYS_MODULE_OBJ object );


// *****************************************************************************
// *****************************************************************************
// Section: ADC Driver Client Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_ADC_Open( const SYS_MODULE_INDEX drvIndex,
                             const DRV_IO_INTENT intent )

  Summary:
    Opens the specified ADC driver instance and returns a handle to it.
	<p><b>Implementation:</b> Static/Dynamic</p>

  Description:
    This function opens the specified ADC driver instance and provides a
    handle that must be provided to all other client-level operations to
    identify the caller and the instance of the driver.

  Precondition:
    The DRV_ADC_Initialize function must have been called before calling this
    function.

  Parameters:
    drvIndex    - Identifier for the object instance to be opened
    
    intent      - Zero or more of the values from the enumeration
                  DRV_IO_INTENT "ORed" together to indicate the intended use
                  of the driver

  Returns:
    If successful, the function returns a valid open-instance handle (a number
    identifying both the caller and the module instance).
    
    If an error occurs, the return value is DRV_HANDLE_INVALID.

  Example:
    <code>
    DRV_HANDLE  handle;

    handle = DRV_ADC_Open(DRV_ADC_INDEX_0, DRV_IO_INTENT_EXCLUSIVE);
    if (DRV_HANDLE_INVALID == handle)
    {
        // Unable to open the driver
    }
    </code>

  Remarks:
    The handle returned is valid until the DRV_ADC_Close function is called.

    This function will NEVER block waiting for hardware.
    
    If the DRV_IO_INTENT_BLOCKING is requested and the driver was built 
    appropriately to support blocking behavior, other client-level
    operations may block waiting on hardware until they are complete.

    If DRV_IO_INTENT_NON_BLOCKING is requested the driver client can call the
    DRV_ADC_ClientStatus operation to find out when the module is in the ready 
    state.

    If the requested intent flags are not supported, the function will return
    DRV_HANDLE_INVALID.
*/

DRV_HANDLE DRV_ADC_Open( const SYS_MODULE_INDEX drvIndex,
                         const DRV_IO_INTENT intent );


// *****************************************************************************
/* Function:
    SYS_STATUS DRV_ADC_Close(DRV_Handle handle)

  Summary:
    Closes an opened-instance of the ADC driver.
	<p><b>Implementation:</b> Static/Dynamic</p>

  Description:
    This function closes an opened-instance of the ADC driver, invalidating the
    handle.

  Precondition:
    The DRV_ADC_Initialize function must have been called for the specified
    ADC driver instance.

    DRV_ADC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

  Returns:
    SYS_STATUS   - System Status

  Example:
    <code>
    DRV_HANDLE handle;  // Returned from DRV_ADC_Open

    DRV_ADC_Close(handle);
    </code>

  Remarks:
    After calling this function, the handle passed in "handle" must not be used
    with any of the remaining driver functions.  A new handle must be obtained by
    calling DRV_ADC_Open before the caller may use the driver again.

    If DRV_IO_INTENT_BLOCKING was requested and the driver was built 
    appropriately to support blocking behavior call may block until the 
    operation is complete.

    If DRV_IO_INTENT_NON_BLOCKING request the driver client can call the
    DRV_ADC_Status operation to find out when the module is in
    the ready state (the handle is no longer valid).

    Note: Usually there is no need for the driver client to verify that the Close
    operation has completed.
*/

SYS_STATUS DRV_ADC_Close ( DRV_HANDLE handle ) ;


//***************************************************************************
/* Function:
       DRV_ADC_CLIENT_STATUS DRV_ADC_ClientStatus( DRV_HANDLE handle )
    
  Summary:
    Gets the current client-specific status the ADC driver.
	<p><b>Implementation:</b> Dynamic</p>
	
  Description:
    This function gets the client-specific status of the ADC driver
    associated with the given handle.
	
  Precondition:
    The DRV_ADC_Initialize function must have been called.
    
    DRV_ADC_Open must have been called to obtain a valid opened device
    handle.
	
  Parameters:
    handle -  A valid open-instance handle, returned from the driver's open
              function
  Returns:
    A DRV_ADC_CLIENT_STATUS value describing the current status of the
    driver.
	
  Example:
    <code>
    DRV_HANDLE              handle;  // Returned from DRV_ADC_Open
    DRV_ADC_CLIENT_STATUS   status;
    
    status = DRV_ADC_ClientStatus(handle);
    if(DRV_ADC_CLIENT_STATUS_ERROR >= status)
    {
        // Handle the error
    }
    </code>
	
  Remarks:
    This function will not block for hardware access and will immediately
    return the current status.                                             
  ***************************************************************************/

DRV_ADC_CLIENT_STATUS DRV_ADC_ClientStatus ( DRV_HANDLE handle ) ;


// *****************************************************************************
/* Function:
    void DRV_ADC_Start( DRV_HANDLE handle )

  Summary:
    Starts the ADC driver sampling and converting analog to digital values.
	<p><b>Implementation:</b> Static/Dynamic</p>

  Description:
    This function starts the ADC driver sampling the selected analog inputs and 
    converting the samples to digital values.  
 
  Precondition:
    The DRV_ADC_Initialize function must have been called.

    DRV_ADC_Open must have been called to obtain a valid opened device handle.

    The desired analog input set must have been selected by calling 
    DRV_ADC_InputsRegister.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

  Returns:
    None.

  Example:
    <code>
    DRV_HANDLE handle;  // Returned from DRV_ADC_Open
    
    // Use DRV_ADC_InputsRegister to register the desired inputs.
    
    DRV_ADC_Start(handle);
    </code>

  Remarks:
    Call DRV_ADC_SamplesAvailable to find out when one or more samples is 
    available.
*/

void DRV_ADC_Start( DRV_HANDLE handle );


// *****************************************************************************
/* Function:
    void DRV_ADC_Stop( DRV_HANDLE handle )

  Summary:
    Stops the ADC driver from sampling and converting analog to digital values.
	<p><b>Implementation:</b> Static/Dynamic</p>

  Description:
    This function stops the ADC driver from sampling analog inputs and 
    converting the samples to digital values.  
 
  Precondition:
    The DRV_ADC_Initialize function must have been called.

    DRV_ADC_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

  Returns:
    None.

  Example:
    <code>
    DRV_HANDLE handle;  // Returned from DRV_ADC_Open
        
    DRV_ADC_Stop(handle);
    </code>

  Remarks:
    Call DRV_ADC_Start to restart sampling and conversion of analog inputs to
    digital values.
*/

void DRV_ADC_Stop( DRV_HANDLE handle );


//****************************************************************************
/* Function:
      void DRV_ADC_InputsRegister ( DRV_HANDLE handle , uint32_t inputsMask )
    
  Summary:
    Registers an input set with the driver for sampling.
	<p><b>Implementation:</b> Dynamic</p>
  Description:
    This function registers an input set with the driver for sampling.
	
  Precondition:
    The DRV_ADC_Initialize function must have been called for the specified
    ADC device instance and the DRV_ADC_Status must have returned
    SYS_STATUS_READY.
    
    DRV_ADC_Open must have been called to obtain a valid opened device
    handle.
	
  Parameters:
    handle -      A valid open-instance handle, returned from the driver's
                  open function
    inputsMask -  Mask bits recognizing the various Analog Channels
	
  Returns:
    None.
	
  Example:
    <code>
    DRV_HANDLE handle;  // Returned from DRV_ADC_Open
    
    DRV_ADC_InputsRegister (handle, ADC_INPUT_AN2|ADC_INPUT_AN3);
    </code>
	
  Remarks:
    None.                                                                    
  ****************************************************************************/

void DRV_ADC_InputsRegister ( DRV_HANDLE handle , uint32_t inputsMask ) ;


// *****************************************************************************
/* Function:
   bool DRV_ADC_SamplesAvailable ( DRV_HANDLE handle )

  Summary:
    Identifies if any the ADC driver has any samples available to read.
	<p><b>Implementation:</b> Static/Dynamic</p>

  Description:
    This function identifies if any the ADC driver has any samples available to 
    read.

  Precondition:
    The DRV_ADC_Initialize function must have been called.

    DRV_ADC_Open must have been called to obtain a valid opened device handle.

    The desired analog input set must have been selected by calling 
    DRV_ADC_InputsRegister.

    DRV_ADC_Start must have been called to start the driver sampling and 
    converting analog input samples to digital values. 

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

  Returns:
    - true    - If one or more samples are available for the registered input set
 
    - false   - If no samples are available

  Example:
    <code>
    DRV_HANDLE      handle;  // Returned from DRV_ADC_Open
    DRV_ADC_SAMPLE  buffer;
    
    // An input set must have been registered and the ADC started.
        
    if (DRV_ADC_SamplesAvailable(handle))
    {
        DRV_ADC_SamplesRead(handle, &buffer, sizeof(buffer));
    }
    </code>

  Remarks:
    None.
*/

bool DRV_ADC_SamplesAvailable ( DRV_HANDLE handle );


// *****************************************************************************
/* Function:
    unsigned short DRV_ADC_SamplesRead ( DRV_HANDLE              handle,
                                         ADC_SAMPLE              *buffer,
                                         unsigned short          bufferSize);

  Summary:
    Reads the converted sample data from the ADC driver.
	<p><b>Implementation:</b> Static/Dynamic</p>

  Description:
    This function reads converted sample data from the ADC driver into the 
    given buffer.  How many samples depends on how many samples are available
    and on the relative sizes of the samples and the buffer passed in.
    
    Zero (0) samples are copied if the bufferSize is less than the size of a 
    complete set of samples for the registered inputs.
    
    N sets of samples where the bufferSize / size of a complete set of 
    samples = N, unless less than N samples are currently available.  Then, the
    number of samples currently available are copied.

  Precondition:
    The DRV_ADC_Initialize function must have been called.

    DRV_ADC_Open must have been called to obtain a valid opened device handle.

    The desired analog input set must have been selected by calling 
    DRV_ADC_InputsRegister.

    DRV_ADC_Start must have been called to start the driver sampling and 
    converting analog input samples to digital values. 

  Parameters:
    handle       - A valid open-instance handle, returned from the driver's
                   open function

    buffer       - A pointer to the buffer to where the sample data will be 
                   copied

    bufferSize   - Size of the buffer (in bytes)

  Returns:
    Number of bytes of sample data copied to the specified buffer.

  Example:
    <code>
    DRV_HANDLE      handle;  // Returned from DRV_ADC_Open
    ADC_SAMPLE      buffer;
    
    // An input set must have been registered and the ADC started.
        
    if (DRV_ADC_SamplesAvailable(handle))
    {
        DRV_ADC_SamplesRead(handle, &buffer, sizeof(buffer));
    }
    </code>

  Remarks:
    The DRV_ADC_SamplesAvailable function can be used to determine if any sample 
    data is available.
    
    Calling this function removes the samples from the driver's internal buffer
    queue of samples.
*/

unsigned short DRV_ADC_SamplesRead ( DRV_HANDLE             handle,
                                     ADC_SAMPLE             *buffer,
                                     unsigned short         bufferSize);


//*******************************************************************************
/*  Function:
       unsigned short DRV_ADC_SamplesReadLatest ( DRV_HANDLE        handle,
                                                  ADC_SAMPLE        *buffer,
                                                  unsigned short    bufferSize )
    
  Summary:
    Reads the most recently converted sample data from the ADC driver.
	<p><b>Implementation:</b> Dynamic</p>
  Description:
    This function reads only the most recently converted sample data from
    the ADC driver into the given buffer. Only the data for a single set of
    samples for the registered inputs is copied to the caller's buffer. If
    the buffer size is less than the size of a complete set of samples for
    the registered inputs, no data is copied to the caller's buffer. Also,
    no sample data is copied to the caller's buffer if no sample data is
    currently available.
	
  Precondition:
    The DRV_ADC_Initialize function must have been called.
    
    DRV_ADC_Open must have been called to obtain a valid opened device
    handle.
    
    The desired analog input set must have been selected by calling
    DRV_ADC_InputsRegister.
    
    DRV_ADC_Start must have been called to start the driver sampling and
    converting analog input samples to digital values.
	
  Parameters:
    handle -      A valid open-instance handle, returned from the driver's
                  open function
    buffer -      A pointer to the buffer to where the sample data will be
                  copied
    bufferSize -  Size of the buffer (in bytes)
	
  Returns:
    Number of bytes of sample data copied to the specified buffer.
	
  Example:
    <code>
    DRV_HANDLE      handle;  // Returned from DRV_ADC_Open
    ADC_SAMPLE      buffer;
    
    // An input set must have been registered and the ADC started.
    
    if (DRV_ADC_SamplesAvailable(handle))
    {
        DRV_ADC_SamplesReadLatest(handle, &buffer, sizeof(buffer));
    }
    </code>
	
  Remarks:
    The DRV_ADC_SamplesAvailable function can be used to determine if any
    sample data is available.
    
    This function does not remove any data from the driver's internal
    buffer queue of sample data.                                                
  *******************************************************************************/

unsigned short DRV_ADC_SamplesReadLatest ( DRV_HANDLE       handle,
                                           ADC_SAMPLE       *buffer,
                                           unsigned short   bufferSize);


//****************************************************************************
/* Function:
    inline void DRV_ADC_NegativeInputSelect(
	                                        ADC_MUX mux, 
											ADC_INPUTS_NEGATIVE input
											)
    
  Summary:
    Selects the negative input.
	<p><b>Implementation:</b> Static</p>
	
  Description:
    This function selects the negative input .
	
  Precondition:
    The DRV_ADC_Initialize function must have been called.
	
  Parameters:
    mux -   Input mux (MUXA/MUXB) for which the negative input is being
	        selected
    input - Negative reference input (AN1/VREFL)
	
  Returns:
    None.
	
  Example:
    <code>    
    DRV_ADC_NegativeInputSelect(ADC_MUX_A, ADC_INPUT_NEGATIVE_VREF_MINUS);
    </code>
	
  Remarks:
    None.                                                                    
  ****************************************************************************/

inline void DRV_ADC_NegativeInputSelect(ADC_MUX mux, ADC_INPUTS_NEGATIVE input);

//****************************************************************************
/* Function:
    inline void DRV_ADC_PositiveInputSelect(
	                                        ADC_MUX mux, 
											ADC_INPUTS_POSITIVE input
											)
    
  Summary:
    Selects the positive input.
	<p><b>Implementation:</b> Static</p>
	
  Description:
    This function selects the positive input .
	
  Precondition:
    The DRV_ADC_Initialize function must have been called.
	
  Parameters:
    mux -   Input mux (MUXA/MUXB) for which the negative input is being
	        selected
    input - Positive input (AN0...ANx)
	
  Returns:
    None.
	
  Example:
    <code>    
    DRV_ADC_PositiveInputSelect(ADC_MUX_A, ADC_INPUT_POSITIVE_AN2);
    </code>
	
  Remarks:
    None.                                                                    
  ****************************************************************************/

inline void DRV_ADC_PositiveInputSelect(ADC_MUX mux, ADC_INPUTS_POSITIVE input);

//****************************************************************************
/* Function:
    inline void DRV_ADC_ChannelScanInputsAdd(ADC_INPUTS_SCAN scanInput)
    
  Summary:
    Adds scan input.
	<p><b>Implementation:</b> Static</p>
	
  Description:
    This function adds scan input.
	
  Precondition:
    The DRV_ADC_Initialize function must have been called.
	
  Parameters:
    scanInput - Scan input (AN0...ANx)
	
  Returns:
    None.
	
  Example:
    <code>    
    DRV_ADC_ChannelScanInputsAdd(ADC_INPUT_SCAN_AN5);
    </code>
	
  Remarks:
    None.                                                                    
  ****************************************************************************/

inline void DRV_ADC_ChannelScanInputsAdd(ADC_INPUTS_SCAN scanInput);

//****************************************************************************
/* Function:
    inline void DRV_ADC_ChannelScanInputsRemove(ADC_INPUTS_SCAN scanInput)
    
  Summary:
    Removes scan input.
	<p><b>Implementation:</b> Static</p>
	
  Description:
    This function removes scan input.
	
  Precondition:
    The DRV_ADC_Initialize function must have been called.
	
  Parameters:
    scanInput - Scan input (AN0...ANx)
	
  Returns:
    None.
	
  Example:
    <code>    
    DRV_ADC_ChannelScanInputsRemove(ADC_INPUT_SCAN_AN5);
    </code>
	
  Remarks:
    None.                                                                    
  ****************************************************************************/

inline void DRV_ADC_ChannelScanInputsRemove(ADC_INPUTS_SCAN scanInput);

//****************************************************************************
/* Function:
    inline void DRV_ADC_ChannelExtendedScanInputsAdd
	            (
	            ADC_INPUTS_SCAN_EXTENDED eScanInput
				)
    
  Summary:
    Adds extended scan input on the supported devices.
	<p><b>Implementation:</b> Static</p>
	
  Description:
    This function adds extended scan input.
	
  Precondition:
    The DRV_ADC_Initialize function must have been called.
	
  Parameters:
    scanInput - Extended scan input (AN32...ANx)
	
  Returns:
    None.
	
  Example:
    <code>    
    DRV_ADC_ChannelExtendedScanInputsAdd(ADC_INPUT_SCAN_AN36);
    </code>
	
  Remarks:
    None.                                                                    
  ****************************************************************************/

inline void DRV_ADC_ChannelExtendedScanInputsAdd(ADC_INPUTS_SCAN_EXTENDED eScanInput);

//****************************************************************************
/* Function:
    inline void DRV_ADC_ChannelExtendedScanInputsRemove
	            (
	            ADC_INPUTS_SCAN_EXTENDED eScanInput
				)
    
  Summary:
    Removes extended scan input on the supported devices.
	<p><b>Implementation:</b> Static</p>
	
  Description:
    This function removes extended scan input.
	
  Precondition:
    The DRV_ADC_Initialize function must have been called.
	
  Parameters:
    scanInput - Extended scan input (AN32...ANx)
	
  Returns:
    None.
	
  Example:
    <code>    
    DRV_ADC_ChannelExtendedScanInputsRemove(ADC_INPUT_SCAN_AN36);
    </code>
	
  Remarks:
    None.                                                                    
  ****************************************************************************/

inline void DRV_ADC_ChannelExtendedScanInputsRemove(ADC_INPUTS_SCAN_EXTENDED eScanInput);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //_DRV_ADC_H
/*******************************************************************************
 End of File
*/

