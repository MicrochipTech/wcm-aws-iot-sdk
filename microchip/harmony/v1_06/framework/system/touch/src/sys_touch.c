/*******************************************************************************
  Touch System Service Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    sys_touch.c

  Summary:
    Touch System Service Implementation.

  Description:
    The Touch System Service provides a simple interface to manage the touch
    screen drivers. This file implements the core interface routines
    for the Touch System Service.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute Software
only when embedded on a Microchip microcontroller or digital  signal  controller
that is integrated into your product or third party  product  (pursuant  to  the
sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
//DOM-IGNORE-END

//#include "osal/osal.h"
#include "system/touch/src/sys_touch_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the driver instance object array. */
SYS_TOUCH_OBJ gSysTouchObj[SYS_TOUCH_INSTANCES_NUMBER] ;

/* This is the client object array. */
SYS_TOUCH_CLIENT_OBJ gSysTouchClientObj[SYS_TOUCH_CLIENTS_NUMBER];

/* This object maintains data that is required by all Touch instances. */
SYS_TOUCH_COMMON_DATA_OBJ gSysTouchCommonDataObj;


// *****************************************************************************
// *****************************************************************************
// Section: Initialization
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
/* Function:
    SYS_MODULE_OBJ SYS_TOUCH_Initialize ( const SYS_MODULE_INIT * const init)

  Summary:
    Initializes and Enables the SYS touch service.

  Description:
    This function initializes the System touch services.  .

    This routine initializes Touch services  making it ready for clients to
    open and use it. The initialization data is specified by the init parameter.


  Precondition:
    None.

  Parameters:
    init            - Pointer to the data structure containing any data
                      necessary to initialize the hardware. This pointer may
                      be null if no data is required and default
                      initialization is to be used.

  Returns:
    If successful, returns a valid handle to the DMA module object.
    Otherwise, it returns SYS_MODULE_OBJ_INVALID.

  Example:
  <code>
    SYS_MODULE_OBJ objectHandle;
    const DRV_TOUCH_INIT sysTouchInit0 =
    {
        .drvInitialize           = DRV_TOUCH_ADC10BIT_Initialize,
        .drvOpen                 = DRV_TOUCH_ADC10BIT_Open,

        .orientation             = DISP_ORIENTATION,
        .horizontalResolution    = DISP_HOR_RESOLUTION,
        .verticalResolution      = DISP_VER_RESOLUTION,
    };

    initParam.sidl = SYS_DMA_SIDL_ENABLE;
    objectHandle = SYS_TOUCH_Initialize(SYS_TOUCH_INDEX_0,
                                      (SYS_MODULE_INIT*)initParam);
    if (SYS_MODULE_OBJ_INVALID == objectHandle)
    {
        // Handle error
    }
  </code>

  Remarks:
    This routine must be called before any other Touch systems service routines
    are called.

    Not all features are available on all micro-controllers.
*/
SYS_MODULE_OBJ SYS_TOUCH_Initialize (const SYS_MODULE_INDEX   moduleIndex,
                                     const SYS_MODULE_INIT * const init)

{

    DRV_TOUCH_INIT *initp;

    /* Validate the init object */
    if ((SYS_MODULE_INIT *)NULL == init)
    {
        SYS_ASSERT(false, "Invalid Init Object");
        return SYS_MODULE_OBJ_INVALID;
    }

    SYS_TOUCH_OBJ *dObj = (SYS_TOUCH_OBJ*)NULL;

    /* Check if the specified driver index is in valid range */
    if(moduleIndex >= SYS_TOUCH_INSTANCES_NUMBER)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Check if this hardware instance was already initialized */
    if(gSysTouchObj[moduleIndex].inUse != false)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Assign to the local pointer the init data passed */
    initp = (DRV_TOUCH_INIT *)init;
    
    /* Allocate the driver object and set the operation flag to be in use */
    dObj = &gSysTouchObj[moduleIndex];
    dObj->inUse = true;

    /* Set the driver status as busy */
    dObj->status =  SYS_STATUS_BUSY ;

    dObj->nClients = 0;
    dObj->isExclusive = false;

    /* Set driver task state to open touch driver */
    dObj->state = SYS_TOUCH_TASK_TOUCH_DRIVER_OPEN;

    /* save driver init data */
    dObj->driverInitData = initp;

    /* Return the object structure */
    return ((SYS_MODULE_OBJ) dObj);
}

// *****************************************************************************
/* Function:
    SYS_TOUCH_HANDLE SYS_TOUCH_Open
    (
        const SYS_MODULE_INDEX drvIndex
    )

  Summary:
    Opens the specified touch instance.

*/

SYS_TOUCH_HANDLE SYS_TOUCH_Open
(
    const SYS_MODULE_INDEX moduleIndex
)
{
    SYS_TOUCH_CLIENT_OBJ *clientObj;
    SYS_TOUCH_OBJ *dObj;
    unsigned int iClient;

    if (moduleIndex >= SYS_TOUCH_INSTANCES_NUMBER)
    {
        /* Invalid server index */
        return (SYS_TOUCH_HANDLE_INVALID);
    }

    dObj = &gSysTouchObj[moduleIndex];

    if(dObj->inUse == false)
    {
        /* The moduleIndex module should be ready */

        return SYS_TOUCH_HANDLE_INVALID;
    }

    if(dObj->isExclusive)
    {
        /* This means the another client has opened the driver in exclusive
           mode. The driver cannot be opened again */

        return ( SYS_TOUCH_HANDLE_INVALID ) ;
    }

    /* open touch driver */
    dObj->driverInitData->drvOpen(moduleIndex, DRV_IO_INTENT_READWRITE);

    /* Grab client object mutex here */

//    if(OSAL_MUTEX_Lock(&(gSysTouchCommonDataObj.mutexClientObjects), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
//    {
        /* Enter here only if the lock was obtained (appplicable in
           RTOS only). If the mutex lock fails due to time out then
           this code does not get executed */

        for(iClient = 0; iClient != SYS_TOUCH_CLIENTS_NUMBER; iClient ++)
        {
            if(!gSysTouchClientObj[iClient].inUse)
            {
                /* This means we have a free client object to use */
                clientObj = &gSysTouchClientObj[iClient];
                clientObj->inUse        = true;

                /* We have found a client object. Release the mutex */

//                OSAL_ASSERT(OSAL_MUTEX_Unlock(&(gSysTouchCommonDataObj.mutexClientObjects)),
//                        "Unable to unlock clients objects routine mutex");

                clientObj->hDriver      = dObj;

//                if(ioIntent & DRV_IO_INTENT_EXCLUSIVE)
//                {
//                    /* Set the driver exclusive flag */
//                    dObj->isExclusive = true;
//                }

                dObj->nClients ++;

                // /* Create the semaphores */
                // OSAL_ASSERT(((OSAL_SEM_Create(&(clientObj->semReadDone), OSAL_SEM_TYPE_COUNTING, 1, 0)) == OSAL_RESULT_TRUE),
                        // "Unable to create client read done semaphore");
                // OSAL_ASSERT(((OSAL_SEM_Create(&(clientObj->semWriteDone), OSAL_SEM_TYPE_COUNTING, 1, 0)) == OSAL_RESULT_TRUE),
                        // "Unable to create client write done semaphore");

                /* Update the client status */
                clientObj->clientStatus = SYS_STATUS_READY;
                return ((DRV_HANDLE) clientObj );
            }
        }

        /* Set driver task state to get touch points */
        dObj->state = SYS_TOUCH_TASK_TOUCH_POINTS_GET;
    
//        /* Could not find a client object. Release the mutex and
//           return with an invalid handle. */
//        OSAL_ASSERT((OSAL_MUTEX_Unlock(&(gDrvSST25VF016BCommonDataObj.mutexClientObjects))),
//                    "Unable to unlock clients objects routine mutex");
//    }

    /* If we have reached here, it means either we could not find a spare
       client object or the mutex timed out in a RTOS environment. */

    return SYS_TOUCH_HANDLE_INVALID;
}

//#endif
/*********************************************************************
 * Function: void SYS_TOUCH_TouchMsgGet()
 *
 * PreCondition: none
 *
 * Input: pointer to the message structure to be populated
 *
 * Output: none
 *
 * Side Effects: none
 *
 * Overview: populates GOL message structure
 *
 * Note: none
 *
 ********************************************************************/

void SYS_TOUCH_TouchMsgGet(SYS_MODULE_INDEX moduleIndex)
{
    static int16_t    prevX = -1;
    static int16_t    prevY = -1;
    static SYS_MSG_OBJECT sMsg;
    
    int16_t           x, y;

    SYS_TOUCH_OBJ *dObj = &gSysTouchObj[moduleIndex];
    
    x = dObj->driverInitData->drvTouchGetX(0);
    y = dObj->driverInitData->drvTouchGetY(0);

    sMsg.nMessageTypeID = TYPE_TOUCHSCREEN;
    sMsg.param0 = EVENT_INVALID;

    if((x == -1) || (y == -1))
    {
        y = -1;
        x = -1;
    }

    if((prevX == x) && (prevY == y) && (x != -1) && (y != -1))
    {
        sMsg.param0 = EVENT_STILLPRESS;
        sMsg.param1 = x;
        sMsg.param2 = y;
        SYS_MSG_MessageSend( moduleIndex, &sMsg);
        return;
    }

    if((prevX != -1) || (prevY != -1))
    {
        if((x != -1) && (y != -1))
        {
            // Move
            sMsg.param0 = EVENT_MOVE;
        }
        else
        {

            // Released
            sMsg.param0 = EVENT_RELEASE;
            sMsg.param1 = prevX;
            sMsg.param2 = prevY;
            prevX = x;
            prevY = y;            
            SYS_MSG_MessageSend( moduleIndex, &sMsg);
            return;
        }
    }
    else
    {
        if((x != -1) && (y != -1))
        {

            // Pressed
            sMsg.param0 = EVENT_PRESS;
        }
        else
        {
            // No message
            sMsg.param0 = EVENT_INVALID;
        }
    }

    sMsg.param1 = x;
    sMsg.param2 = y;
    prevX = x;
    prevY = y;

    SYS_MSG_MessageSend( moduleIndex, &sMsg);
}



/*******************************************************************************
  Function:
    void SYS_TOUCH_Tasks(SYS_MODULE_OBJ object );

  Summary:
    Maintains the system service's state machine.
 */

void SYS_TOUCH_Tasks(SYS_MODULE_OBJ object)
{
    /* Check if the specified module object is in valid range */
    if(object == SYS_TOUCH_HANDLE_INVALID)
    {
        return;
    }

    SYS_TOUCH_OBJ * dObj = (SYS_TOUCH_OBJ*) object;

    if(!dObj->inUse)
    {
        /* This instance of the driver is not initialized. Don't
         * do anything */
        return;
    }

    switch(dObj->state)
    {
        case SYS_TOUCH_TASK_TOUCH_DRIVER_OPEN:
            SYS_TOUCH_Open(SYS_TOUCH_INDEX_0);
            dObj->state = SYS_TOUCH_TASK_TOUCH_POINTS_GET;
            break;
        
        case SYS_TOUCH_TASK_TOUCH_POINTS_GET:
            SYS_TOUCH_TouchMsgGet(SYS_TOUCH_INDEX_0);
            break;
            
        default:
            break;
    }
}

// *****************************************************************************
/* Function:
    void SYS_TOUCH_CalibrationSet
           ( SYS_MODULE_INDEX  moduleIndex, SYS_TOUCH_SAMPLE_POINTS * samplePoints)

  Summary:
     Sets the calibration values for Touch client specified by moduleIndex.
*/
void SYS_TOUCH_CalibrationSet( SYS_MODULE_INDEX moduleIndex, DRV_TOUCH_SAMPLE_POINTS *samplePoints)
{
    SYS_TOUCH_OBJ * dObj = &gSysTouchObj[moduleIndex];
    if ( dObj->driverInitData->drvCalibrationSet )
    {
        dObj->driverInitData->drvCalibrationSet(samplePoints);
    }
}
