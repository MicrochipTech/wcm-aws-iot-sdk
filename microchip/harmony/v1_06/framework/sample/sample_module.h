/*******************************************************************************
  MPLAB Harmony Sample Library Module Interface Header

  Company:
    Microchip Technology Inc.

  File Name:
    sample_module.h

  Summary:
    Sample of a MPLAB Harmony library module.

  Description:
    This is a sample of a MPLAB Harmony library interface module.  This file 
    defines the system and application interface.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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

#ifndef _SAMPLE_MODULE_H    // Guards against multiple inclusion
#define _SAMPLE_MODULE_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section lists the other files that are included in this file.
*/

#include "system_config.h"
#include "system/common/sys_common.h"
#include "system/common/sys_module.h"
#include "system/int/sys_int.h"
#include "peripheral/tmr/plib_tmr.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif


// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Module Instance Indexes

  Summary:
    Labels for module index numbers.

  Description:
    The following symbolic labels can be used in place of numeric literals to 
    identify module indexes.
*/

#define SAMPLE_INDEX_0  0
#define SAMPLE_INDEX_1  1


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Sample Module Initialization Data

  Summary:
    Data necessary to initialize the sample module.

  Description:
    This data structure contains data necessary to initialize an instance of the
    sample module.

  Remarks:
    A pointer to a persistent copy of this structure must be passed into the 
    SAMPLE_Initialize function.
*/

typedef struct _sample_module_init_data
{
    /* Standard system module init data. */
    SYS_MODULE_INIT     moduleInit;

    /* Some initialization data. */
    int                 dataSome;

    /* When used in interrupt-driven mode, the sample module utilizes a 
       directly (via the timer peripheral library).   The following 
       initialization entries are ignored in polled mode.
       
       Note: The clock source for the timer is always the internal 
       peripheral bus clock and it is always operated in 16-bit mode. 
    */

    /* Timer used in interrupt-driven configurations. */
    TMR_MODULE_ID           tmr;

	/* Timer prescaler setting. */
    TMR_PRESCALE            prescale;

    /* Timer period setting. */
    uint16_t                period;

    /* Timer interrupt source. */
    INT_SOURCE              interrupt;

    /* Timer interrupt vector. */
    INT_VECTOR              vector;

    /* Timer interrupt priority. */
    INT_PRIORITY_LEVEL      priority;

    /* Timer interrupt subpriority. */
    INT_SUBPRIORITY_LEVEL   subpriority;

} SAMPLE_MODULE_INIT_DATA;


// *****************************************************************************
/* Sample Module Data Status

  Summary:
    Possible data processing states.

  Description:
    This enumeration defines the possible data processing states of the sample
    module.

  Remarks:
    This status is provided by the SAMPLE_DataStatus function..
*/

typedef enum _sample_module_data_status
{
    /* Indicates that the sample module has no data available. */
    SAMPLE_MODULE_DATA_NONE = 0,

    /* Indicates that the sample module is busy processing and will have data
    available soon. */
    SAMPLE_MODULE_DATA_BUSY,

    /* Indicates that the sample module has available data. */
    SAMPLE_MODULE_DATA_AVAILABLE

} SAMPLE_MODULE_DATA_STATUS;


// *****************************************************************************
// *****************************************************************************
// Section: ClientInterface Routines
// *****************************************************************************
// *****************************************************************************
/*  These functions can be called by clients to this module to utilize the 
    functionality it provides.
*/

// *****************************************************************************
/* Function:
   SAMPLE_MODULE_DATA_STATUS SAMPLE_DataStatus ( const SYS_MODULE_INDEX index )

  Summary:
    Identifies the current status of the data processed by the module.

  Description:
    This function identifies the current status of the data processed by the 
    module.

  Precondition:
    The sample module must be initialized before calling this API function.

  Parameters:
    index       - Index of the desired module instance.
      
  Returns:
    SAMPLE_MODULE_DATA_NONE         - If the sample module has no data to give.
    
    SAMPLE_MODULE_DATA_BUSY         - If the sample module is busy processing 
                                      and will have data available soon.
    
    SAMPLE_MODULE_DATA_AVAILABLE    - If the sample module has data available.
  
  Example:
    <code>
    if (SAMPLE_MODULE_DATA_BUSY == SAMPLE_DataStatus(SAMPLE_INDEX_0))
    {
        // Check again later.
    }
    </code>

  Remarks:
    In a polled "super-loop" system, you cannot poll this function waiting
    for the sample module to change status from SAMPLE_MODULE_DATA_BUSY to 
    SAMPLE_MODULE_DATA_AVAILABLE.  The loop must be allowed to continue so
    that the sample module can finish processing the data before it will 
    become available.
*/

SAMPLE_MODULE_DATA_STATUS SAMPLE_DataStatus ( const SYS_MODULE_INDEX index );


// *****************************************************************************
/* Function:
   bool SAMPLE_DataGive ( const SYS_MODULE_INDEX index, int data )

  Summary:
    Gives data to the sample module.

  Description:
    This function gives data to the sample module.

  Precondition:
    The sample module should be initialized before calling this API function.

  Parameters:
    index       - Index of the desired module instance.
    
    data        - Data value to give to the sample module.
  
  Returns:
    true        - If successful.
    
    false       - If an error occurred.

  Example:
    <code>
    if (SAMPLE_DataGive(SAMPLE_INDEX_0, 42))
    {
        // Data given
    }
    </code>

  Remarks:
    Any data value given to the sample module will be available via the 
    SAMPLE_DataGet function later, after the sample module state machine is 
    ready to give it back.
*/

bool SAMPLE_DataGive ( const SYS_MODULE_INDEX index, int data );


// *****************************************************************************
/* Function:
   bool SAMPLE_DataGet ( const SYS_MODULE_INDEX index, int *pData )

  Summary:
    Gets data from the sample module.

  Description:
    This function gets data from the sample module. If no data has been given
    to the sample module yet, it will return a failure

  Precondition:
    The sample module should be initialized before calling this API function.

  Parameters:
    index       - Index of the desired module instance.
    
    pData       - Pointer to the variable to receive the data value.
  
  Returns:
    true        - If successful.
    
    false       - If an error occurred.  If an error occurs, the variable 
                  pointed to by pData is not updated.

  Example:
    <code>
    int data;
    
    if (SAMPLE_DataGet(SAMPLE_INDEX_0, &data))
    {
        // Data valid
    }
    </code>

  Remarks:
    Any data value given to the sample module by the SAMPLE_DataGive function
    will be available via this function later, after the sample module state 
    machine is ready to give it back.
*/

bool SAMPLE_DataGet ( const SYS_MODULE_INDEX index, int *data );


// *****************************************************************************
// *****************************************************************************
// Section: System Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ SAMPLE_Initialize( const SYS_MODULE_INDEX        index,
                                      const SYS_MODULE_INIT * const init )

  Summary:
    Initializes the sample module.

  Description:
    This routine initializes the sample module, making it ready for clients to
    open and use it.

  Precondition:
    None.

  Parameters:
    index           - Index for the module instance to be initialized

    init            - Pointer to a data structure containing any data necessary
                      to initialize the module.

  Returns:
    If successful, returns a valid handle to a system module object.  
    Otherwise, it returns SYS_MODULE_OBJ_INVALID. The returned object handle 
    must be passed as an argument to SAMPLE_Reinitialize, SAMPLE_Deinitialize, 
    SAMPLE_Tasks and SAMPLE_Status routines.

  Example:
    <code>
    SAMPLE_MODULE_INIT_DATA init;
    SYS_MODULE_OBJ          obj;

    // Populate the sample initialization structure
    init.sys.powerState     = SYS_MODULE_POWER_RUN_FULL;
    init.dataSome           = 42;

    // Initialize the first instance of the sample module.
    obj = SAMPLE_Initialize(SAMPLE_INDEX_0, (SYS_MODULE_INIT *)&init);
    if (SYS_MODULE_OBJ_INVALID == obj)
    {
        // Handle failure
    }
    </code>

  Remarks:
    This routine must be called before any other sample module routine is 
    called.

    This routine should only be called once during system initialization
    unless SAMPLE_Deinitialize is called to deinitialize the module instance.

    This routine will NEVER block for internal state changes. If the operation
    requires time to complete, it will be reported by the SAMPLE_Status 
    operation. The system can use SAMPLE_Status to find out when the associated
    instance of the sample module is in a ready state.
*/

SYS_MODULE_OBJ SAMPLE_Initialize ( const SYS_MODULE_INDEX        index,
                                   const SYS_MODULE_INIT * const init );


// *****************************************************************************
/* Function:
    void SAMPLE_Reinitialize( SYS_MODULE_OBJ                object,
                              const SYS_MODULE_INIT * const init )

  Summary:
    Reinitializes the sample module and refreshes any associated internal 
    settings.

  Description:
    This routine reinitializes the sample module and refreshes any associated 
    settings using the initialization data given.  However, it will not interrupt 
    ongoing client operations.

  Precondition:
    Function SAMPLE_Initialize must have been called before calling this
    routine and a valid SYS_MODULE_OBJ must have been returned.

  Parameters:
    object          - Sample module object handle, returned from the 
                      SAMPLE_Initialize routine

    init            - Pointer to the initialization data structure

  Returns:
    None

  Example:
    <code>
    SYS_MODULE_OBJ          obj;    // Returned from SAMPLE_Initialize
    SAMPLE_MODULE_INIT_DATA init;
    SYS_STATUS              status;

    // Populate the sample initialization structure
    init.sys.powerState     = SYS_MODULE_POWER_SLEEP;
    init.dataSome           = 0;

    SAMPLE_Reinitialize(obj, (SYS_MODULE_INIT*)&init);

    status = SAMPLE_Status(obj);
    if (SYS_STATUS_BUSY == status)
    {
        // Check again later to ensure the driver is ready
    }
    else if (SYS_STATUS_ERROR >= status)
    {
        // Handle error
    }
    </code>

  Remarks:
    This function can be called multiple times to reinitialize the module.

    This operation can be used to refresh any supported internal settings as
    specified by the initialization data or to change the power state of the
    module.

    This routine will NEVER block for internal state changes. If the operation 
    requires time to compete, it will be reported by the SAMPLE_Status 
    operation. The system can use SAMPLE_Status to find out when the specified
    sample module instance is in a ready state.
*/

void SAMPLE_Reinitialize ( SYS_MODULE_OBJ                object,
                           const SYS_MODULE_INIT * const init );


// *****************************************************************************
/* Function:
    void SAMPLE_Deinitialize ( SYS_MODULE_OBJ object )

  Summary:
    Deinitializes the specified instance of the sample module.

  Description:
    Deinitializes the specified instance of the sample module, disabling
    its operation.  Invalidates all the internal data and client operations.

  Precondition:
    Function SAMPLE_Initialize must have been called before calling this
    routine and a valid SYS_MODULE_OBJ handle must have been returned.

  Parameters:
    object          - Sample module instance's object handle, returned from the
                      SAMPLE_Initialize routine.

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ      object;     //  Returned from SAMPLE_Initialize
    SYS_STATUS          status;

    SAMPLE_Deinitialize(object);

    status = SAMPLE_Status(object);
    if (SYS_MODULE_DEINITIALIZED == status)
    {
        // Check again later if you need to know when the sample module 
        // instance has been deinitialized.
    }
    </code>

  Remarks:
    Once the Initialize operation has been called, the Deinitialize operation
    must be called before the Initialize operation can be called again.

    This routine will NEVER block waiting for internal state changes. If the 
    operation requires time to complete, it will be reported by the 
    SAMPLE_Status operation.  The system can to use SAMPLE_Status to find
    out when the module is in the SYS_STATUS_UNINITIALIZED state.
*/

void SAMPLE_Deinitialize ( SYS_MODULE_OBJ object );


// *****************************************************************************
/* Function:
    SYS_STATUS SAMPLE_Status ( SYS_MODULE_OBJ object )

  Summary:
    Provides the current status of the sample module instance.

  Description:
    This routine provides the current status of the sample module instance.

  Precondition:
    Function SAMPLE_Initialize must have been called before calling this
    function.

  Parameters:
    object          - Sample module instance's object handle, returned from the
                      SAMPLE_Initialize routine.

  Returns:
    SYS_STATUS_ERROR          - Indicates that the module instance is in an 
                                error state.

                                Note:  Any value less than SYS_STATUS_ERROR is
                                also an error state.
                                
    SYS_STATUS_UNINITIALIZED  - Indicates that the driver has been deinitialized.

                                Note:  This value is greater than SYS_STATUS_ERROR

    SYS_STATUS_BUSY           - Indicates that the module instance is busy with a
                                previous system level operation and cannot start
                                another

    SYS_STATUS_READY          - Indicates that the module instance is running and
                                ready to begin another system level operation.

                                Note Any value greater than SYS_STATUS_READY is
                                also a normal running state in which the module
                                is ready to accept new operations

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from SAMPLE_Initialize
    SYS_STATUS          status;

    status = SAMPLE_Status(object);
    else if (SYS_STATUS_ERROR >= status)
    {
        // Handle error
    }
    </code>

  Remarks:
    The this operation can be used to determine when any of the system
    operations have completed.

    If the status operation returns SYS_STATUS_BUSY, the a previous operation
    has not yet completed.  Once the status operation returns SYS_STATUS_READY,
    any previous operations have completed.

    The value of SYS_STATUS_ERROR is negative (-1).  Any value less than that is
    also an error state.

    This routine will NEVER block waiting for an internal state change.

    If the Status operation returns an error value, the error may be cleared by
    calling the reinitialize operation.  If that fails, the deinitialize
    operation will need to be called, followed by the initialize operation to
    return to normal operations.
*/

SYS_STATUS SAMPLE_Status ( SYS_MODULE_OBJ object );


// *****************************************************************************
/* Function:
    void SAMPLE_Tasks( SYS_MODULE_OBJ object )

  Summary:
    Maintains the sample module instance's state machine.

  Description:
    This routine is used to maintain the sample module instance's internal 
    state machine.

  Precondition:
    The SAMPLE_Initialize routine must have been called for the specified
    SAMPLE module instance.

  Parameters:
    object      - Object handle for the specified sample module instance 
                  (returned from SAMPLE_Initialize).

  Returns:
    None

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from SAMPLE_Initialize

    while (true)
    {
        SAMPLE_Tasks (object);

        // Do other tasks
    }
    </code>

  Remarks:
    This routine should not be called directly a client of the sample module.
    It is called by the system's Tasks routine (SYS_Tasks) or by an appropriate 
    ISR.

    This routine may execute in an ISR context and will never block or access any
    resources that may cause it to block.
*/

void SAMPLE_Tasks( SYS_MODULE_OBJ object );


#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif // _SAMPLE_MODULE_H

/*******************************************************************************
 End of File
*/

