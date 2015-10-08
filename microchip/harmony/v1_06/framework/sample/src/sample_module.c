/*******************************************************************************
  Sample MPLAB Harmony Sample Library Module

  Company:
    Microchip Technology Inc.

  File Name:
    sample_module.h

  Summary:
    Sample of an MPLAB Harmony library module.

  Description:
    This is a sample of an MPLAB Harmony library interface module.  This file 
    contains module's implementation.
    
  Remarks:
    The samle module has no associated hardware and thus, it has no associated
    interrupt source.  To simulate an interrupt-driven configuration, some
    other interrupt source (usually a timer) must be artificially configured and
    setup by the system and be ready for the sample module to enable that
    interrupt source when SAMPLE_Initialize is called.
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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include "system_config.h"
#include "sample/sample_module.h"
#include "system/common/sys_common.h"
#include "system/debug/sys_debug.h"
#include "osal/osal.h"


// *****************************************************************************
// *****************************************************************************
// Section: Default Configuration Option Values
// *****************************************************************************
// *****************************************************************************

/* Default to polled mode. */
#ifndef SAMPLE_MODULE_INTERRUPT_MODE
    #define SAMPLE_MODULE_INTERRUPT_MODE    false
    #warning "Sample module defaulting to polled mode."
#endif

/* Default number of sample module instances. */
#ifndef SAMPLE_MODULE_INSTANCES_NUMBER
    #define SAMPLE_MODULE_INSTANCES_NUMBER  1
    #warning "Sample module defaulting to 1 instance."
#endif

/* Default sample object mutex timeout. */
#ifndef SAMPLE_MODULE_TIMEOUT
    #define SAMPLE_MODULE_TIMEOUT           100
    #warning "Sample module defaulting to 100 ms access timeout."
#endif


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Sample Module Instance Data

  Summary:
    Data necessary to maintain an instance of the sample module.

  Description:
    This data structure contains data necessary to maintain an instance of the
    sample module.

  Remarks:
    None.
*/

typedef struct _sample_module_data
{
    // Current status of the module.
    SYS_STATUS              status;

    // Newly Entered Data
    int                     dataNew;

    // Flag indicating new data is valid.
    bool                    dataNewIsValid;

    // Some more data.
    int                     dataProcessed;

    // Flag indicating processed data is valid.
    bool                    dataProcessedIsValid;
    
    /* Mutex to lock the module object. */
    OSAL_MUTEX_HANDLE_TYPE  mutex;

    // Interrupt Source (ignored in polled configurations).
    INT_SOURCE              interrupt;

} SAMPLE_MODULE_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Sample Module Global Data
// *****************************************************************************
// *****************************************************************************

static SAMPLE_MODULE_DATA gObj[SAMPLE_MODULE_INSTANCES_NUMBER];


// *****************************************************************************
// *****************************************************************************
// Section: Local Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    bool _ObjectIsValid ( SYS_MODULE_OBJ object )

  Summary:
    Local inline function to validate object handle.

  Description:
    This local inline function validates an object handle.

  Precondition:
    None.

  Parameters:
    object          - Sample module object handle, returned from the 
                      SAMPLE_Initialize routine
  
  Returns:
    true            - If object handle matches a valid instance object.
    
    false           - If object handle does not match a valid instance 
                      object.

  Example:
    <code>
    void SAMPLE_Function( SYS_MODULE_OBJ  object )
    {
        SYS_ASSERT(_ObjectIsValid(object), "Invalid object handle"  );
        
        // Use object
    }
    </code>

  Remarks:
    This function should only be used inside a SYS_ASSERT or similar 
    macro as it may add significant overhead when used.
*/

static inline bool _ObjectIsValid ( SYS_MODULE_OBJ object )
{
    unsigned int i;

    for (i=0; i < SAMPLE_MODULE_INSTANCES_NUMBER; i++) 
    {
        if (object == (SYS_MODULE_OBJ)&gObj[i])
        {
            return true;
        }
    }

    return false;
}


// *****************************************************************************
/* Function:
    void _InterruptInitialize ( SAMPLE_MODULE_DATA           *pObj, 
                                const SYS_MODULE_INIT * const init )
  Summary:
    Local function to initialize the sample module's interrupt source.

  Description:
    When configured for interrupt mode, this local function initializes the 
    sample module's interrupt source.


  Precondition:
    The sample module must be otherwise properly initialized.

  Parameters:
    pObj        - Validate pointer to the current sample module data object.
    
    pInit       - Pointer to the sample module's initialization data.
  
  Returns:
    None.

  Example:
    None.

  Remarks:
    This function is nulled out (generates no object code) when the module is
    not configured for interrupt mode.
*/

#if (SAMPLE_MODULE_INTERRUPT_MODE == true)

    static void _InterruptInitialize ( SAMPLE_MODULE_DATA                   *pObj, 
                                       const SAMPLE_MODULE_INIT_DATA * const pInit )
    {
        SYS_ASSERT(NULL != pInit, "Initial data must identify interrupt source.");

        // Initialize the timer
        PLIB_TMR_Stop(pInit->tmr);
        PLIB_TMR_ClockSourceSelect(pInit->tmr, TMR_CLOCK_SOURCE_PERIPHERAL_CLOCK);
        PLIB_TMR_PrescaleSelect(pInit->tmr, pInit->prescale);
        PLIB_TMR_Mode16BitEnable(pInit->tmr);
        PLIB_TMR_Counter16BitClear(pInit->tmr);
        PLIB_TMR_Period16BitSet(pInit->tmr, pInit->period);

        // Capture, setup and enable the given interrupt source.
        pObj->interrupt = pInit->interrupt;
        SYS_INT_SourceEnable(pInit->interrupt);
        SYS_INT_VectorPrioritySet(pInit->vector, pInit->priority );
        SYS_INT_VectorSubprioritySet(pInit->vector, pInit->subpriority );
        SYS_INT_SourceEnable(pInit->interrupt);

        /* Start Timer*/
        PLIB_TMR_Start(pInit->tmr);
    }

#else

    #define _InterruptInitialize(o,i)

#endif


// *****************************************************************************
/* Function:
    bool _InterruptDisable ( INT_SOURCE source )

  Summary:
    Disable the interrupt source.

  Description:
    This local macro to disable the given interrupt source.

  Precondition:
    The module and Interrupt service (if so configured) must be initialized.

  Parameters:
    source          - The desired interrupt source.
      
  Returns:
    true            - If the interrupt source was previously enabled.
    
    false           - If the interrupt source was previously disabled.

  Example:
    None.

  Remarks:
    This macro is nulled out (generates no object code) when the module is
    not configured for interrupt mode.
*/

#if (SAMPLE_MODULE_INTERRUPT_MODE == true)

    #define _InterruptDisable(s)    SYS_INT_SourceDisable(s)

#else

    #define _InterruptDisable(s)    false
    
#endif


// *****************************************************************************
/* Function:
    void _InterruptRestore ( INT_SOURCE source, bool enabled )

  Summary:
    Restores the interrupt source to given state.

  Description:
    This local inline function restores the interrupt source to the given 
    state (enabling if or disabling it as identified in the "enabled" 
    parameter.

  Precondition:
    The module and Interrupt service (if so configured) must be initialized.

  Parameters:
    source          - The desired interrupt source.
        
    enabled         - State to restore:
                        true    - If the interrupt source is to be enabled.
                        false   - If the interrupt source is to be disabled.
  
  Returns:
    None.

  Example:
    None.

  Remarks:
    This function is nulled out (generates no object code) when the module is
    not configured for interrupt mode.
*/

#if (SAMPLE_MODULE_INTERRUPT_MODE == true)

    static inline void _InterruptRestore ( INT_SOURCE source, bool enabled )
    {
        if (enabled)
        {
            SYS_INT_SourceEnable(source);
        }
        else
        {
            SYS_INT_SourceDisable(source);
        }
        
    }

#else

    #define _InterruptRestore(s,e)

#endif


// *****************************************************************************
/* Function:
    bool _TasksMutexLock ( SAMPLE_MODULE_DATA *pObj )

  Summary:
    Calls OSAL_MUTEX_Lock when built in polled mode.

  Description:
    This local function calls OSAL_MUTEX_Lock when built in polled mode.

  Precondition:
    The system and module must be initialized.

  Parameters:
    pObj            - Pointer to the module instance structure.

  Returns:
    true            - If able to lock the mutex (or in interrupt mode).
    
    false           - If unable to lock the mutex.

  Example:
    None.

  Remarks:
    This function is forced to a constant "true" when building in interrupt
    mode.
*/

#if (SAMPLE_MODULE_INTERRUPT_MODE == false)

    static inline bool _TasksMutexLock (SAMPLE_MODULE_DATA *pObj)
    {
        if (OSAL_MUTEX_Lock(&pObj->mutex, SAMPLE_MODULE_TIMEOUT) == OSAL_RESULT_TRUE)
        {
            return true;
        }

        return false;
    }

#else

    #define _TasksMutexLock(p)      true

#endif


// *****************************************************************************
/* Function:
    void _TasksMutexUnlock ( SAMPLE_MODULE_DATA *pObj )

  Summary:
    Calls OSAL_MUTEX_Unlock when built in polled mode.

  Description:
    This local function calls OSAL_MUTEX_Unlock when built in polled mode.

  Precondition:
    The system and module must be initialized.

  Parameters:
    pObj            - Pointer to the module instance structure.

  Returns:
    None.

  Example:
    None.

  Remarks:
    This function is nulled out (generates no object code) when the module is
    configured for interrupt mode.
*/

#if (SAMPLE_MODULE_INTERRUPT_MODE == false)

    static inline void _TasksMutexUnlock ( SAMPLE_MODULE_DATA *pObj )
    {
        OSAL_MUTEX_Unlock(&pObj->mutex);
    }

#else

    #define _TasksMutexUnlock(p)

#endif


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

  Remarks:
    See sample_module.h for additional details.
*/

SAMPLE_MODULE_DATA_STATUS SAMPLE_DataStatus ( const SYS_MODULE_INDEX index )
{
    SAMPLE_MODULE_DATA         *pObj;
    SAMPLE_MODULE_DATA_STATUS   status;
    bool                        intState;

    // Get a local object pointer.
    SYS_ASSERT(index < SAMPLE_MODULE_INSTANCES_NUMBER, "Invalid index");
    pObj = (SAMPLE_MODULE_DATA *)&gObj[index];

    // Protect Non-atomic access to module object.
    if (OSAL_MUTEX_Lock(&pObj->mutex, SAMPLE_MODULE_TIMEOUT) == OSAL_RESULT_TRUE)
    {
        // Guard against interrupts
        intState = _InterruptDisable(pObj->interrupt);

        // Determine current module data processing status.
        if (pObj->dataProcessedIsValid)
        {
            status = SAMPLE_MODULE_DATA_AVAILABLE;
        }
        else if (pObj->dataNewIsValid)
        {
            status = SAMPLE_MODULE_DATA_BUSY;
        }
        else
        {
            status = SAMPLE_MODULE_DATA_NONE;
        }

        // Restore interrupt state and unlock module object.
        _InterruptRestore(pObj->interrupt, intState);
        OSAL_MUTEX_Unlock(&pObj->mutex);
    }
    else
    {
        status = SAMPLE_MODULE_DATA_BUSY;
    }

    return status;
}


// *****************************************************************************
/* Function:
    bool SAMPLE_DataGive ( const SYS_MODULE_INDEX index, int data )

  Summary:
    Gives data to the sample module.

  Description:
    This function gives data to the sample module.

  Remarks:
    See sample_module.h for additional details.
*/

bool SAMPLE_DataGive ( const SYS_MODULE_INDEX index, int data )
{
    SAMPLE_MODULE_DATA *pObj;
    bool                intState;
    bool                result = false;

    // Get a local object pointer.
    SYS_ASSERT(index < SAMPLE_MODULE_INSTANCES_NUMBER, "Invalid index");
    pObj = (SAMPLE_MODULE_DATA *)&gObj[index];

    // Protect Non-atomic access to module object.
    if (OSAL_MUTEX_Lock(&pObj->mutex, SAMPLE_MODULE_TIMEOUT) == OSAL_RESULT_TRUE)
    {
        // Guard against interrupts
        intState = _InterruptDisable(pObj->interrupt);

        // Check for storage space
        if (!pObj->dataNewIsValid)
        {
            // Store data
            pObj->dataNew           = data;
            pObj->dataNewIsValid    = true;
            pObj->status            = SYS_STATUS_BUSY;
            result                  = true;
        }

        // Restore interrupt state and unlock module object.
        _InterruptRestore(pObj->interrupt, intState);
        OSAL_MUTEX_Unlock(&pObj->mutex);
    }

    // Module is busy
    return result;
}


// *****************************************************************************
/* Function:
    bool SAMPLE_DataGet ( const SYS_MODULE_INDEX index, int *data )

  Summary:
    Gets data from the sample module.

  Description:
    This function gets data from the sample module. If no data has been given
    to the sample module yet, it will return a failure

  Remarks:
    See sample_module.h for additional details.
*/

bool SAMPLE_DataGet ( const SYS_MODULE_INDEX index, int *data )
{
    SAMPLE_MODULE_DATA *pObj;
    bool                intState;
    bool                result = false;

    SYS_ASSERT(NULL != data, "NULL data pointer");

    // Get a local object pointer.
    SYS_ASSERT(index < SAMPLE_MODULE_INSTANCES_NUMBER, "Invalid index");
    pObj = (SAMPLE_MODULE_DATA *)&gObj[index];

    // Protect Non-atomic access to module object.
    if (OSAL_MUTEX_Lock(&pObj->mutex, SAMPLE_MODULE_TIMEOUT) == OSAL_RESULT_TRUE)
    {
        // Guard against interrupts
        intState = _InterruptDisable(pObj->interrupt);

        if (pObj->dataProcessedIsValid)
        {
            // Provide data
            *data = pObj->dataProcessed;
            pObj->dataProcessedIsValid = false;
            result = true;
        }

        // Restore interrupt state and unlock module object.
        _InterruptRestore(pObj->interrupt, intState);
        OSAL_MUTEX_Unlock(&pObj->mutex);
    }

    // No data available or module is busy
    return result;
}


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

  Remarks:
    See sample_module.h for additional details.
*/

SYS_MODULE_OBJ SAMPLE_Initialize ( const SYS_MODULE_INDEX        index,
                                   const SYS_MODULE_INIT * const init )
{
    SAMPLE_MODULE_DATA      *pObj;
    SAMPLE_MODULE_INIT_DATA *pInit = (SAMPLE_MODULE_INIT_DATA *)init;

    SYS_ASSERT(index < SAMPLE_MODULE_INSTANCES_NUMBER, "Invalid index");
    
    // Get a local object pointer.
    pObj = (SAMPLE_MODULE_DATA *)&gObj[index];

    // Initialize module object.
    pObj->dataProcessedIsValid  = false;
    if (NULL == pInit)
    {
        pObj->status            = SYS_STATUS_READY;
        pObj->dataNewIsValid    = false;
    }
    else
    {
        pObj->status            = SYS_STATUS_BUSY;
        pObj->dataNew           = pInit->dataSome;
        pObj->dataNewIsValid    = true;
    }

    /* Create a mutex to lock the module object. */
    if(OSAL_MUTEX_Create(&pObj->mutex) != OSAL_RESULT_TRUE)
    {
         return SYS_MODULE_OBJ_INVALID;
    }

    // Initialize interrupt source when configured in interrupt-driven mode.
    // (See "Remarks" at the top of this file.)
    _InterruptInitialize(pObj, pInit);

    return (SYS_MODULE_OBJ)pObj;
}


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

  Remarks:
    See sample_module.h for additional details.
*/

void SAMPLE_Reinitialize ( SYS_MODULE_OBJ                object,
                           const SYS_MODULE_INIT * const init )
{
    SAMPLE_MODULE_DATA         *pObj;
    SAMPLE_MODULE_INIT_DATA    *pInit = (SAMPLE_MODULE_INIT_DATA *)init;
    bool                        intState;

    SYS_ASSERT(_ObjectIsValid(object), "Invalid object handle");
    
    // Get a local object pointer.
    pObj = (SAMPLE_MODULE_DATA *)object;

    // Reinitialize new data, but not processed data.
    if (OSAL_MUTEX_Lock(&pObj->mutex, SAMPLE_MODULE_TIMEOUT) == OSAL_RESULT_TRUE)
    {
        intState = _InterruptDisable(pObj->interrupt);

        if (NULL == pInit)
        {
            pObj->status            = SYS_STATUS_READY;
            pObj->dataNewIsValid    = false;
        }
        else
        {
            pObj->status            = SYS_STATUS_BUSY;
            pObj->dataNew           = pInit->dataSome;
            pObj->dataNewIsValid    = true;
        }

        _InterruptInitialize(pObj, pInit);
        OSAL_MUTEX_Unlock(&pObj->mutex);
    }
    else
    {
        pObj->status = SYS_STATUS_ERROR;
    }

    return;
}


// *****************************************************************************
/* Function:
    void SAMPLE_Deinitialize ( SYS_MODULE_OBJ object )

  Summary:
    Deinitializes the specified instance of the sample module.

  Description:
    Deinitializes the specified instance of the sample module, disabling
    its operation.  Invalidates all the internal data and client operations.

  Remarks:
    See sample_module.h for additional details.
*/

void SAMPLE_Deinitialize ( SYS_MODULE_OBJ object )
{
    bool                intState;
    SAMPLE_MODULE_DATA *pObj  = (SAMPLE_MODULE_DATA *)object;

    SYS_ASSERT(_ObjectIsValid(object), "Invalid object handle");

    // Disable interrupt source (if so configured).
    intState = _InterruptDisable(pObj->interrupt);

    // Delete the sample object mutex and invalidate the object data.
    if (OSAL_MUTEX_Delete(&pObj->mutex) == OSAL_RESULT_TRUE)
    {
        pObj->dataNewIsValid        = false;
        pObj->dataProcessedIsValid  = false;
        pObj->status                = SYS_STATUS_UNINITIALIZED;
    }
    else
    {
        pObj->status = SYS_STATUS_ERROR;
    }

    return;
}


// *****************************************************************************
/* Function:
    SYS_STATUS SAMPLE_Status ( SYS_MODULE_OBJ object )

  Summary:
    Provides the current status of the sample module instance.

  Description:
    This routine provides the current status of the sample module instance.

  Remarks:
    See sample_module.h for additional details.
*/

SYS_STATUS SAMPLE_Status ( SYS_MODULE_OBJ object )
{
    SAMPLE_MODULE_DATA     *pObj  = (SAMPLE_MODULE_DATA *)object;

    SYS_ASSERT(_ObjectIsValid(object), "Invalid object handle");

    return pObj->status;
}


// *****************************************************************************
/* Function:
    void SAMPLE_Tasks( SYS_MODULE_OBJ object )

  Summary:
    Maintains the sample module instance's state machine.

  Description:
    This routine is used to maintain the sample module instance's internal 
    state machine.

  Remarks:
    See sample_module.h for additional details.
*/

void SAMPLE_Tasks( SYS_MODULE_OBJ object )
{
    SAMPLE_MODULE_DATA     *pObj  = (SAMPLE_MODULE_DATA *)object;

    SYS_ASSERT(_ObjectIsValid(object), "Invalid object handle");

    if (!_TasksMutexLock(pObj))
    {
        return;
    }

    // Process data when ready.
    if (pObj->dataNewIsValid && !pObj->dataProcessedIsValid)
    {
        pObj->dataProcessed         = pObj->dataNew;
        pObj->dataNewIsValid        = false;
        pObj->dataProcessedIsValid  = true;
        pObj->status                = SYS_STATUS_READY;
    }

    _TasksMutexUnlock(pObj);

    return;
}


/*******************************************************************************
 End of File
*/

