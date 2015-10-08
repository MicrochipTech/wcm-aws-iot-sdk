/*******************************************************************************
 ADC Driver Interface Implementation
 
  Company:    
    Microchip Technology Inc.

  File Name:
    drv_adc_pic32mx.c

  Summary:    
    ADC Driver Interface Definition

  Description:
    The ADc Driver provides an interface to access the ADC module functions on
	the PIC32MX series microcontrollers. This file should be included in the 
	projects with PIC32MX device variants if ADC driver functionality is needed.
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

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include "driver/adc_new/src/drv_adc_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Objects
// *****************************************************************************
// *****************************************************************************

/* Driver object array */
DRV_ADC_OBJECT   gDrvADCObj[DRV_ADC_INSTANCES_NUMBER];

/* Client object array */
DRV_ADC_CLIENT_OBJECT gDrvADCClientObj[DRV_ADC_CLIENTS_NUMBER];

/* This object maintains data that is required by all ADC
   driver instances. */
DRV_ADC_COMMON_DATA_OBJ gDrvADCCommonDataObj;

// *****************************************************************************
// *****************************************************************************
// Section: ADC Driver Local Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/*  Function:  
    static void _DRV_ADC_HardwareSetup (DRV_ADC_OBJECT * dObj, 
	                                    const DRV_ADC_INIT * const adcInit)

  Summary:
    Initializes the hardware registers.

  Description:
    Takes the initialization data from the application through the 
	DRV_ADC_Initialize function and initializes the hardware registers.

  Remarks:
    See drv_adc.h for usage information
*/

static void _DRV_ADC_HardwareSetup
(
DRV_ADC_OBJECT * dObj,
const DRV_ADC_INIT * const adcInit
)
{
    const register ADC_MODULE_ID adcId = adcInit->adcId;

    /* Disable ADC */
    PLIB_ADC_Disable(adcId);
	
    /* Get the ADC Interrupt Source */
    dObj->interruptSource = adcInit->interruptSource;

    /* Power state initialization */
    if (adcInit->moduleInit.sys.powerState == SYS_MODULE_POWER_IDLE_STOP)
    {
	PLIB_ADC_StopInIdleEnable(adcId);
    }
    else
    {
	PLIB_ADC_StopInIdleDisable(adcId);
    }
	
    /* Conversion clock selection */
    PLIB_ADC_ConversionClockSelect(adcId, adcInit->tadClk);

    /* Conversion clock source selection */
    PLIB_ADC_ConversionClockSource(adcId, adcInit->adcClkSrc);

    /* Conversion trigger source selection */
    PLIB_ADC_ConversionTriggerSourceSelect (adcId, adcInit->convTrgSrc);

    /* Conversion trigger source selection */
    PLIB_ADC_SamplesPerInterruptSelect(adcId, adcInit->samplesPerInterrupt - 1) ;
	
    /* Update the driver object */
    dObj->samplesPerInterrupt = adcInit->samplesPerInterrupt;

    /* Check if Auto sampling is supported */
    if (adcInit->initFlags && DRV_ADC_AUTO_SAMPLING )
    {
        /* Auto sampling is supported, enable it */
        PLIB_ADC_SampleAutoStartEnable(adcId);
        dObj->initFlags |= DRV_ADC_AUTO_SAMPLING;
    }

    /* Check if the user has requested for stopping the conversion sequence */
    if (adcInit->initFlags && DRV_ADC_STOP_CONVERSION_ON_INTERRUPT)
    {
        PLIB_ADC_ConversionStopSequenceEnable(adcId);
        dObj->initFlags |= DRV_ADC_STOP_CONVERSION_ON_INTERRUPT;
    }

    /* Check if the user has requested for Alternating the inputs */
    if (adcInit->initFlags && DRV_ADC_ALTERNATE_INPUT_SAMPLING)
    {
        PLIB_ADC_ModeAlternateInputSampleEnable(adcId);
        dObj->initFlags |= DRV_ADC_ALTERNATE_INPUT_SAMPLING;
    }

    /* Voltage reference selection */
    PLIB_ADC_VoltageReferenceSelect(adcId, adcInit->voltageReference);

    /* Enable scanning of inputs */
    PLIB_ADC_MuxAInputScanEnable(adcId);

    /* Sample acquisition time selection */
    PLIB_ADC_SampleAcqusitionTimeSelect(adcId, adcInit->acquisitionTime);

    /* Select the Analog Channel to convert */
    _DRV_ADC_SelectInput(adcId, adcInit->analogInput);
    
	/* Update the driver object */
    dObj->inputsMask = adcInit->analogInput;

} /* _DRV_ADC_HardwareSetup */

// *****************************************************************************
/* Function:
    DRV_ADC_CLIENT * _DRV_ADC_ClientHandleValidate( DRV_HANDLE handle );

  Summary:
    Returns NULL if the client handle is not valid

  Description:
    This function returns NULL if the client handle is invalid else return the
    client object associated with the handle.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

DRV_ADC_CLIENT_OBJECT * _DRV_ADC_ClientHandleValidate(DRV_HANDLE handle)
{
    DRV_ADC_CLIENT_OBJECT * clientObj;
    DRV_ADC_OBJECT * dObj;

    /* Validate the handle */
    if((0 == handle)|| (DRV_HANDLE_INVALID == handle))
    {
        return NULL;
    }

    /* See if the client has been opened */
    clientObj = (DRV_ADC_CLIENT_OBJECT *)handle;

    if(!clientObj->inUse)
    {
        return NULL;
    }

    /* Check if the driver not ready for operation */
    dObj = (DRV_ADC_OBJECT *)clientObj->driverObj;
	
    if(dObj->status != SYS_STATUS_READY)
    {
        /* Associated hardware instance object is not ready for
         * any operation */
        return NULL;
    }

    return clientObj;
}

// *****************************************************************************
// *****************************************************************************
// Section: ADC Driver Interface Implementations
// *****************************************************************************
// *****************************************************************************

// *******************************************************************************
/*  Function:
    SYS_MODULE_OBJ DRV_ADC_Initialize ( const SYS_MODULE_INDEX drvIndex,
                                        const SYS_MODULE_INIT * const init )

  Summary:
    Initializes hardware and data for the instance of the ADC module.

  Description:
    This function initializes hardware for the instance of the ADC module,
    using the hardware initialization given data.  It also initializes any
    internal data structures.

  Parameters:
    drvIndex        - Identifier for the instance to be initialized
    init            - Pointer to the data structure containing any data
                      necessary to initialize the hardware. This pointer may
                      be null if no data is required and default
                      initialization is to be used.

  Returns:
    If successful, returns a valid handle to a driver instance object.
    Otherwise, it returns SYS_MODULE_OBJ_INVALID.

  Remarks:
    See drv_adc.h for usage information
*/

SYS_MODULE_OBJ DRV_ADC_Initialize 
( 
const SYS_MODULE_INDEX drvIndex,
const SYS_MODULE_INIT * const init
)
{
    DRV_ADC_OBJECT * dObj = (DRV_ADC_OBJECT *) NULL;
    DRV_ADC_INIT * adcInit = NULL;

    /* Check if the specified driver index is in valid range */
    if ( drvIndex >= DRV_ADC_INSTANCES_NUMBER )
    {
        SYS_DEBUG(0, "Invalid driver index");
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Check if this hardware instance was already initialized */
    if(gDrvADCObj[drvIndex].inUse)
    {
        SYS_DEBUG(0, "Instance already in use");
        return SYS_MODULE_OBJ_INVALID;
    }
	
    /* Assign to the local pointer the init data passed */
    adcInit = ( DRV_ADC_INIT * ) init;

    /* Allocate the driver object and set the operation flag to be in use */
    dObj = &gDrvADCObj[drvIndex];
    dObj->inUse = true;
	
    /* Initialize driver object elements */
    dObj->moduleId 			= adcInit->adcId;
    dObj->nClients			= 0;
    dObj->IsExclusive			= false;
    dObj->currentSampleSet 		= 0;
    dObj->nextSampleSet 		= 0;
    dObj->loopedAround 			= false;

    /* Setup the Hardware */
    _DRV_ADC_HardwareSetup(dObj, adcInit);

    /* Clear the interrupt Sources */
    SYS_INT_SourceStatusClear(dObj->interruptSource);

    /* Enable the interrupt source in case of interrupt mode */
    _DRV_ADC_InterruptSourceEnable(dObj->interruptSource);
	
    /* Create the hardware instance mutex */
    if (OSAL_MUTEX_Create(&(dObj->adcDrvObjMutex)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG(0, "API:DRV_ADC_Initialize - ADC driver object mutex can not be created");
        return OSAL_RESULT_FALSE;
    }

    /* Check if  the global mutexes have been creates and create them if not */
    if(!gDrvADCCommonDataObj.membersAreInitialized)
    {
	if (OSAL_MUTEX_Create(&(gDrvADCCommonDataObj.adcClientObjMutex)) != OSAL_RESULT_TRUE)
        {
            SYS_DEBUG(0, "API:DRV_ADC_Initialize - ADC client object mutex can not be created");
	    return OSAL_RESULT_FALSE;		
	}
    }
	
    /* Initialization is complete; Set the status as ready */
    dObj->status = SYS_STATUS_READY;

    /* Return the object structure */
    return ( (SYS_MODULE_OBJ)drvIndex );

} /* DRV_ADC_Initialize */

/*******************************************************************************
  Function:
    void DRV_ADC_Deinitialize ( SYS_MODULE_OBJ object )

  Summary:
    Deinitializes the specified instance module instance of the ADC module.

  Description:
    Deinitializes the specific module instance disabling its operation (and any
    hardware for driver modules).  Resets all the internal data structures and
    fields for the specified instance to the default settings.

  Parameters:
    object           - Identifier for the instance to be deinitialized

  Returns:
    None.
*/

void DRV_ADC_Deinitialize ( SYS_MODULE_OBJ object )
{
    DRV_ADC_OBJECT * dObj = (DRV_ADC_OBJECT *) object;

    /* Validate the object */
    if((SYS_MODULE_OBJ_INVALID == object) ||(object >= DRV_ADC_INSTANCES_NUMBER))
    {
        return;
    }

    /* Disable the interrupt */
    _DRV_ADC_InterruptSourceDisable(dObj->interruptSource) ;

    /* Disable ADC module */
    PLIB_ADC_Disable(dObj->moduleId);

    /* Set the Device Status */
    dObj->status = SYS_STATUS_UNINITIALIZED ;

    /* Remove the driver Instance usage */
    dObj->inUse = false ;

    /* Delete driver object mutex */
    if (OSAL_MUTEX_Delete(&(dObj->adcDrvObjMutex)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG(0, "API:DRV_ADC_Initialize - ADC driver object mutex can not be deleted");
        return;
    }
 
    if (OSAL_MUTEX_Delete(&(gDrvADCCommonDataObj.adcClientObjMutex)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG(0, "API:DRV_ADC_DeInitialize - ADC client object mutex can not be deleted");
        return;
    }

} /* DRV_ADC_Deinitialize */


/*******************************************************************************
  Function:
    SYS_STATUS DRV_ADC_Status ( SYS_MODULE_OBJ object )

  Summary:
    Gets the status of the ADC instance.

  Description:
    Returns the current status of the ADC module.

  Remarks:
    SYS_STATUS_READY	Indicates that any previous module operation for the
                        specified module has completed
    SYS_STATUS_BUSY	Indicates that a previous module operation for the
                        specified module has not yet completed
    SYS_STATUS_ERROR	Indicates that the specified module is in an error state
*/

SYS_STATUS DRV_ADC_Status ( SYS_MODULE_OBJ object )
{
    DRV_ADC_OBJ * dObj = (DRV_ADC_OBJ *) object;

    /* Check that the object is valid */
    if((SYS_MODULE_OBJ_INVALID == object)||(object >= DRV_ADC_INSTANCES_NUMBER))
    {
        /* Object is invalid */
        return SYS_STATUS_UNINITIALIZED;
    }

    /* Return the status associated with the driver handle */
    return gDrvADCObj[object].status;

} /* DRV_ADC_Status */

// *****************************************************************************
// *****************************************************************************
// Section: ADC Driver Client Routines
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    DRV_HANDLE DRV_ADC_Open( const SYS_MODULE_INDEX index,
                             const DRV_IO_INTENT intent )

  Summary:
    Opens a new client for the device instance.

  Description:
    Returns a handle of the opened client instance. All client operation APIs
    will require this handle as an argument.
	
  Parameters:
    index           - Identifier for the instance to be initialized
    ioIntent        - Possible values from the enumeration DRV_IO_INTENT

  Remarks:
    If successful, the routine returns a valid open-instance handle (a number
    identifying both the caller and the module instance)
    If an error occurs, the return value is DRV_HANDLE_INVALID
*/

DRV_HANDLE DRV_ADC_Open 
( 
const SYS_MODULE_INDEX drvIndex,
const DRV_IO_INTENT ioIntent
)
{
    /* Local Variables */
    DRV_ADC_CLIENT_OBJECT * clientObj = (DRV_ADC_CLIENT_OBJECT *) gDrvADCClientObj;
    DRV_ADC_OBJECT * dObj;
	
    unsigned int iClient;

    /* Validate the driver index */
    if (drvIndex >= DRV_ADC_INSTANCES_NUMBER)
    {
        return DRV_HANDLE_INVALID;
    }

    dObj = &gDrvADCObj[drvIndex];

    if(dObj->status != SYS_STATUS_READY)
    {
        /* Driver is not ready to be opened */
        return DRV_HANDLE_INVALID;
    }
	
    /* Check for exclusive access */
    if (dObj->IsExclusive)
    {
        /* Driver is already opened in exclusive mode */
        return DRV_HANDLE_INVALID;
    }
	
    if((dObj->nClients > 0) && (ioIntent & DRV_IO_INTENT_EXCLUSIVE))
    {
        /* Driver has already been opened and cannot be
         * opened exclusively */
        return DRV_HANDLE_INVALID;
    }

    if (OSAL_MUTEX_Lock(&(gDrvADCCommonDataObj.adcClientObjMutex),OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG(0, "API:DRV_ADC_Deinitialize - ADC client object mutex can not be locked");
        return OSAL_RESULT_FALSE;
    }
	
    /* Find available slot in array of client objects */
    for (iClient = 0; iClient < DRV_ADC_CLIENTS_NUMBER ; iClient++)
    {
	if ( !clientObj->inUse )
	{
            /* Found a client object that can be used */
	    clientObj->inUse = true;
	    clientObj->driverObj = dObj;
	    clientObj->status = DRV_ADC_CLIENT_STATUS_READY;
	    clientObj->intent = ioIntent;
            if(ioIntent & DRV_IO_INTENT_EXCLUSIVE)
            {
                /* Driver was opened in exclusive mode */
                dObj->IsExclusive = true;
            }
	    clientObj->clientSampleSet = -1;
	    clientObj->currentReadIndex = 0;
			
            dObj->nClients ++;
			
            OSAL_MUTEX_Unlock(&(gDrvADCCommonDataObj.adcClientObjMutex));
            /* Found the object */
            return ( (DRV_HANDLE) clientObj);			
		}
		clientObj += 1;
	}
	
    OSAL_MUTEX_Unlock(&(gDrvADCCommonDataObj.adcClientObjMutex));

    /* Couldn't find open slot in object array */
    return DRV_HANDLE_INVALID;	
    
} /* DRV_ADC_Open */


/*******************************************************************************
  Function:
    SYS_STATUS DRV_ADC_Close( DRV_HANDLE handle )

  Summary:
    Removes an opened client.

  Description:
    Closes an opened client of the ADC. Clears the memory used by the client
    and removes the client from the driver.

   Remarks:
    None.
*/

SYS_STATUS DRV_ADC_Close( DRV_HANDLE handle )
{
    DRV_ADC_CLIENT_OBJECT * clientObj;
    DRV_ADC_OBJECT * dObj;

    /* Get the Client object from the handle passed */
    clientObj = _DRV_ADC_ClientHandleValidate(handle);
	
    if(NULL == clientObj)
    {
        /* Driver handle is not valid */
        return SYS_STATUS_ERROR;
    }

    dObj = clientObj->driverObj;
	
    /* Delete the Client Instance */
    clientObj->inUse = false ;

    /* Set the Client Status */
    clientObj->status = DRV_ADC_CLIENT_STATUS_INVALID;
	
	/* Update the client count */
    dObj->nClients --;
    dObj->IsExclusive = false;

} /* DRV_ADC_Close */


/*******************************************************************************
  Function:
    DRV_ADC_CLIENT_STATUS DRV_ADC_ClientStatus( DRV_HANDLE handle )

  Summary:
    Gets the status of the ADC client.

  Description:
    Returns the present status of the client.

  Remarks:
    The status will be specific to ADC and all possible status
    options are listed in DRV_ADC_CLIENT_STATUS enum.
*/

DRV_ADC_CLIENT_STATUS DRV_ADC_ClientStatus ( DRV_HANDLE handle )
{
    DRV_ADC_CLIENT_OBJECT * clientObj;

    /* Get the Client object from the handle passed */
    clientObj = _DRV_ADC_ClientHandleValidate(handle);

	if(NULL == clientObj)
    {
        /* Driver handle is not valid */
        return DRV_HANDLE_INVALID;
    }

    /* Return the client status associated with the handle passed */
    return clientObj->status;
    
} /* DRV_ADC_ClientStatus */


/*******************************************************************************
  Function:
    void DRV_ADC_Start( DRV_HANDLE handle )

  Summary:
        Starts the module.

  Description:
        Makes the ADC module enabled (switches ON the module).

  Remarks:
        This should be called once the initialization is complete and ADC is ready
        to operate.
*/

void DRV_ADC_Start( DRV_HANDLE handle )
{
    DRV_ADC_CLIENT_OBJECT * clientObj;
    DRV_ADC_OBJECT * dObj;

    /* Get the Client object from the handle passed */
    clientObj = _DRV_ADC_ClientHandleValidate(handle);

    /* Check for the Client validity */
    if(NULL == clientObj)
    {
        /* Driver handle is not valid */
        return;
    }		
	
    /* Get the driver object from the client */
    dObj = clientObj->driverObj;

    /* Start the ADC */
    PLIB_ADC_Enable(dObj->moduleId);

    /* trigger conversion in case the device supports only manual */
    //PM - TODO Check this
    _DRV_ADC_ManualStart(dObj->moduleId);
    
    /* Update the Client Status */
    clientObj->status = DRV_ADC_CLIENT_STATUS_STARTED ;

} /* DRV_ADC_Start */


/*******************************************************************************
  Function:
    void DRV_ADC_Stop( DRV_HANDLE handle )

  Summary:
        Stops the module.

  Description:
        Stops (disables) the ADC module.

  Remarks:
        Applications should call this once it is done with its use of the ADC
        module.
*/

void DRV_ADC_Stop( DRV_HANDLE handle )
{
    DRV_ADC_CLIENT_OBJECT * clientObj;
    DRV_ADC_OBJECT * dObj;

   /* Get the Client object from the handle passed */
    clientObj = _DRV_ADC_ClientHandleValidate(handle);

    /* Check for the Client validity */
    if(NULL == clientObj)
    {
        /* Driver handle is not valid */
        return;
    }
	
    /* Get the driver object from the client */
    dObj = clientObj->driverObj;

    /* Stop the ADC */
    PLIB_ADC_Disable(dObj->moduleId);

    /* Update the Client Status */
    clientObj->status = DRV_ADC_CLIENT_STATUS_STOPPED ;

} /* DRV_ADC_Stop */


/*******************************************************************************
  Function:
    void DRV_ADC_InputsRegister ( DRV_HANDLE handle , uint32_t inputsMask )

  Summary:
    Registers an input set with the driver for sampling.

  Description:
    This function registers an input set with the driver for sampling.

  Parameters:
    handle       - Handle of the communication channel as returned by the
                   DRV_ADC_Open function.=
    inputsMask   - Mask bits recognizing the various analog channels

  Returns:
    None
*/

void DRV_ADC_InputsRegister 
( 
DRV_HANDLE handle, 
uint32_t inputsMask 
)
{
    DRV_ADC_CLIENT_OBJECT * clientObj;
    DRV_ADC_OBJECT * dObj;

    /* Get the Client object from the handle passed */
    clientObj = _DRV_ADC_ClientHandleValidate(handle);

    /* Check for the Client validity */
    if(NULL == clientObj)
    {
        /* Driver handle is not valid */
        return;
    }
	
    /* Get the driver object from the client */
    dObj = clientObj->driverObj;

    /* Get the channels to be scanned for conversion */
    /* Activate the required inputs */
    //PM - TODO Check this	
    _DRV_ADC_SelectInput(dObj-moduleId, inputsMask);
    /* Update the driver object */
    dObj->inputsMask |= inputsMask;

} /* DRV_ADC_InputsRegister */


/*******************************************************************************
  Function:
    unsigned short DRV_ADC_SamplesRead ( DRV_HANDLE         handle,
                                         ADC_SAMPLE     *buffer,
                                         unsigned short     bufferSize)

  Summary:
    Returns the ADC converted value.

  Description:
    This function is used to return the ADC converted value.

  Parameters:
    Driver handle

  Returns:
    None
*/

unsigned shortDRV_ADC_SamplesRead
( 
DRV_HANDLE handle,
ADC_SAMPLE *buffer,
unsigned short bufferSize
)
{
    DRV_ADC_CLIENT_OBJECT * clientObj = (DRV_ADC_CLIENT_OBJECT *)NULL;
    DRV_ADC_OBJECT * dObj = (DRV_ADC_OBJECT *)NULL;
	
    uint8_t buffFillLoop= 0;
    uint8_t totalSampleSet = 0;
    uint8_t clientSampleSet = 0;
    uint8_t nextSampleSet = 0;
    uint8_t currentSampleSet = 0 ;

    /* Get the Client object from the handle passed */
    clientObj = _DRV_ADC_ClientHandleValidate(handle);

    /* Check for the Client validity */
	if(NULL == clientObj)
    {
        /* Driver handle is not valid */
        return DRV_HANDLE_INVALID;
    }

	/* Get the driver object from the client */
	dObj = clientObj->driverObj;

    /* if bufferSize is < DRV_ADC_CONFIG_SAMPLES_PER_INTERRUPT , return DRV_ADC_STATUS_BUFFER_TOO_SMALL */
    if(bufferSize < DRV_ADC_SAMPLES_PER_INTERRUPT )
    {
        clientObj->status = DRV_ADC_CLIENT_STATUS_BUFFER_TOO_SMALL;
        return DRV_ADC_CLIENT_STATUS_BUFFER_TOO_SMALL;
    }

    /* Get the read index from the client object and read the bufferSize number
       of samples */
    currentSampleSet = dObj->currentSampleSet;

    if (clientObj->clientSampleSet == (uint8_t)-1 )
    {
        clientSampleSet = clientObj->clientSampleSet = _DRV_ADC_SampleSetNextGet(currentSampleSet) * dObj->loopedAround;
    }
    else
    {
        clientSampleSet = clientObj->clientSampleSet;
        
        if(dObj->readRequestFlag[clientSampleSet] & clientObj->accessMask)
        {
            /* The sample set it is supposed to be reading is not yet
               overwritten, so read it */
        }
        else
        {
            /* The sample set it is supposed to be reading is overwritten
               so go to the tail and grab the oldest sample set. */
            clientObj->status = DRV_ADC_CLIENT_STATUS_OVERFLOW;
            clientSampleSet = clientObj->clientSampleSet = _DRV_ADC_SampleSetNextGet(currentSampleSet);
        }

    }

    totalSampleSet = clientObj->clientSampleSet * dObj->samplesPerInterrupt;

    /* Restrict the buffer size */
    if( bufferSize > DRV_ADC_INTERNAL_BUFFER_SIZE )
    {
        /* The driver can at most return the sample set array - 1, 
           since the task routine might be working on the latest one.  */
        bufferSize = DRV_ADC_INTERNAL_BUFFER_SIZE;
    }

    /* Read the result from the result buffer
       Restrict the buffer size to the SAMPLE SET SIZE */
    for( buffFillLoop =0 ; buffFillLoop < bufferSize ; buffFillLoop++)
    {
        buffer[buffFillLoop] = dObj->resultBuffer[totalSampleSet++];
    }

    /* Set the read request flag for the next sample set */
    /* The next sample set in this case will be clientSampleSet + 1
       (since you are allowing only read of one sample set at a time),
       and not necessary the current sample set
       if you allow multiple sample set reads at time, this logic will change */

    nextSampleSet = _DRV_ADC_SampleSetNextGet(currentSampleSet);
    dObj->readRequestFlag[nextSampleSet] |= clientObj->accessMask;

    return buffFillLoop;

} /* DRV_ADC_SamplesRead */


/*******************************************************************************
  Function:
    unsigned short DRV_ADC_SamplesReadLatest ( DRV_HANDLE        handle,
                                               ADC_SAMPLE   *buffer,
                                               unsigned short    bufferSize)

   Summary:
    Returns the ADC converted value.

  Description:
    This function is used to return the ADC converted value.

  Parameters:
    Driver handle.

  Returns:
    None.
*/

unsigned short DRV_ADC_SamplesReadLatest
( 
DRV_HANDLE handle,
ADC_SAMPLE  *buffer,
unsigned short  bufferSize
)
{
    DRV_ADC_CLIENT_OBJECT * clientObj = (DRV_ADC_CLIENT_OBJECT *)NULL;
    DRV_ADC_OBJECT * dObj = (DRV_ADC_OBJECT *)NULL;
	
    unsigned short buffFillLoop , totalSampleSet ;

    /* Get the Client object from the handle passed */
    clientObj = _DRV_ADC_ClientHandleValidate(handle);

    /* Check for the Client validity */
    if(NULL == clientObj)
    {
        /* Driver handle is not valid */
        return;
    }

    /* Get the driver object from the client */
    dObj = clientObj->driverObj;

    if ( bufferSize > DRV_ADC_SAMPLES_PER_INTERRUPT )
    {
        bufferSize = DRV_ADC_SAMPLES_PER_INTERRUPT ;
    }

    /* Get the read index from the client object and read the bufferSize number
       of samples */
    totalSampleSet = dObj->currentSampleSet * dObj->samplesPerInterrupt;

    for( buffFillLoop =0 ; buffFillLoop < bufferSize ; buffFillLoop++)
    {
        buffer[buffFillLoop] = dObj->resultBuffer[totalSampleSet++];
    }

    clientObj->status = DRV_ADC_CLIENT_STATUS_READY;
    
    return buffFillLoop;

} /* DRV_ADC_SamplesReadLatest */


/*******************************************************************************
  Function:
    bool DRV_ADC_SamplesAvailable ( DRV_HANDLE handle )

   Summary:
    Gives an indication about the sample availability.

  Description:
    This function is used to provide the status of sample availability.

  Parameters:
    Client handle

  Returns:
    -true  - Sample is available
    -false - Sample is not available

  Remarks:
    Note: Status is set as unavailable after a call to the function is made.
*/

bool DRV_ADC_SamplesAvailable ( DRV_HANDLE handle )
{
    DRV_ADC_CLIENT_OBJECT * clientObj = (DRV_ADC_CLIENT_OBJECT *)NULL;
    DRV_ADC_OBJECT * dObj = (DRV_ADC_OBJECT *)NULL;

    /* Get the Client object from the handle passed */
    clientObj = _DRV_ADC_ClientHandleValidate(handle);

    /* Check for the Client validity */
    if(NULL == clientObj)
    {
        /* Driver handle is not valid */
        return DRV_HANDLE_INVALID;
    }

    /* Get the driver object from the client */
    dObj = clientObj->driverObj;

    /* Check if the ADC Converted data is available */
    if (dObj->adcSamplesAvailable)
    {
        /* Clear the flag for the next iteration */
	dObj->adcSamplesAvailable =  false ;
        return true ;
    }
    else
    {
        return false ;
    }

} /* DRV_ADC_SamplesAvailable */


/*******************************************************************************
  Function:
    void DRV_ADC_Tasks( SYS_MODULE_OBJ object)

   Summary:
    Used to maintain the driver's state machine and implement its ISR.

  Description:
    This function is used to maintain the driver's internal state machine and
    implement its ISR for interrupt-driven implementations.

  Parameters:
    contextData - Device-specific hardware data

  Returns:
    None.
*/

void DRV_ADC_Tasks ( SYS_MODULE_OBJ object )
{
    DRV_ADC_OBJECT  *dObj = (DRV_ADC_OBJECT *) object;
    ADC_MODULE_ID   adcId;
    ADC_SAMPLE      sampleBuffer[DRV_ADC_INTERNAL_BUFFER_SIZE];
    uint8_t         buffLoop = 0;
    uint8_t         writeIndex = 0;
    uint8_t         currentSampleSet = 0;

    if (true == _DRV_ADC_InterruptSourceStatusGet(dObj->interruptSource))
    {
        if (dObj->nextSampleSet < DRV_ADC_INTERNAL_BUFFER_SIZE/DRV_ADC_SAMPLES_PER_INTERRUPT)
        {
            currentSampleSet = dObj->currentSampleSet = dObj->nextSampleSet;
            dObj->nextSampleSet += 1;
            dObj->loopedAround = false ;
        }
        else
        {
            dObj->nextSampleSet = 0;
            currentSampleSet = dObj->currentSampleSet = dObj->nextSampleSet;
            dObj->nextSampleSet += 1;
            dObj->loopedAround = true ;
        }

        /* Setup overflow for all the clients */
        dObj->readRequestFlag[currentSampleSet]= 0;


        writeIndex = currentSampleSet * dObj->samplesPerInterrupt;

        for ( buffLoop = 0 ; buffLoop < dObj->samplesPerInterrupt; buffLoop++ )
        {
            adcId = dObj->moduleId;
            //sampleBuffer = _DRV_ADC_ReadSample(adcId , buffLoop) ;
            //dObj->resultBuffer[writeIndex++] = sampleBuffer;
        }

        /* Indicate to the application that the Conversion has been completed */
        dObj->adcSamplesAvailable = true;

        /* Clear ADC Interrupt/Status Flag */
        _DRV_ADC_InterruptSourceClear(dObj->interruptSource);

        /* Manual Sampling/Conversion Start */
        /* trigger conversion in case the device supports manual */
        _DRV_ADC_ManualStart(dObj->moduleId);
    }

} /* DRV_ADC_Tasks */

/*******************************************************************************
End of File
*/
