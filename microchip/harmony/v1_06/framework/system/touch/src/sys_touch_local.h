/*******************************************************************************
  Touch System Service Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    sys_touch_local.h

  Summary:
    Touch System Service local declarations and definitions.

  Description:
    This file contains the Touch System Service local declarations and
    definitions.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _SYS_TOUCH_LOCAL_H
#define _SYS_TOUCH_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "driver/driver_common.h"
#include "system/touch/sys_touch.h"

#ifdef __cplusplus
    extern "C" {
#endif

#define SYS_TOUCH_INSTANCES_NUMBER 1
#define SYS_TOUCH_CLIENTS_NUMBER 1
// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Touch System task states

  Summary
    Lists the different states that Touch task routine can have.

  Description
    This enumeration lists the different states that Touch task routine
    can have.

  Remarks:
    None.
*/

typedef enum
{
    /* Open Touch driver */
    SYS_TOUCH_TASK_TOUCH_DRIVER_OPEN,
            
    /* Steady-state get touch points */
    SYS_TOUCH_TASK_TOUCH_POINTS_GET,

} SYS_TOUCH_STATE;

// *****************************************************************************
/* Touch System Services

  Summary
    Lists the different services that Touch system can do.

  Description
    This enumeration lists the different operations that Touch system can
    do.

  Remarks:
    None.
*/

typedef enum
{
     No_OP,
} SYS_TOUCH_OPERATION;

// *****************************************************************************
/* Touch Service Global Instances Object

  Summary:
    Object used to keep track of data that is common to all instances of the
    SYS Touch service driver.

  Description:
    This object is used to keep track of any data that is common to all
    instances of the Touch system service.

  Remarks:
    None.
*/

typedef struct
{
    /* Set to true if all members of this structure
       have been initialized once */
    bool membersAreInitialized;

    /* Mutex to protect client object pool */
    //OSAL_MUTEX_DECLARE(mutexClientObjects);

} SYS_TOUCH_COMMON_DATA_OBJ;

// *****************************************************************************
/* SYS TOUCH Instance Object

  Summary:
    Object used to keep any data required for an instance of the SYS Touch
    service.

  Description:
    This object is used to keep track of any data that must be maintained to
    manage a single instance of the SYS Touch service.

  Remarks:
    None.
*/

typedef struct
{
    /*  The module index of associated Touch service */
    SYS_MODULE_INDEX    sysTouchModuleIndex;

    /* The status of the driver */
    SYS_STATUS status;

    /* Flag to indicate this obejct is in use  */
    bool inUse;

    /* Flag to indicate that system has been opened exclusively. */
    bool isExclusive;

    /* Keeps track of the number of clients that have opened this driver */
    uint32_t nClients;

    /* State of the task */
    SYS_TOUCH_STATE    	state;

    /* Driver data */
    DRV_TOUCH_INIT      * driverInitData;

    /* Hardware instance mutex */
    //OSAL_MUTEX_DECLARE(mutexDriverInstance);

} SYS_TOUCH_OBJ;

// *****************************************************************************
/* SYS TOUCH Client Object

  Summary:
    Object used to track a single client.

  Description:
    This object is used to keep the data necesssary to keep track of a single
    client.

  Remarks:
    None.
*/

typedef struct
{
    /* The hardware instance object associated with the client */
    SYS_TOUCH_OBJ * hDriver;

    /* This flags indicates if the object is in use or is available */
    bool inUse;

    /* Client Status */
    SYS_STATUS clientStatus;

    /* Application Context associated with this client */
    uintptr_t context;

} SYS_TOUCH_CLIENT_OBJ;


// *****************************************************************************
/* TOUCH System Service Instance Object

  Summary:
    Defines the object required for the maintenance of the driver.

  Description:
    This defines the object required for the maintenance of the driver.

  Remarks:
    None.
*/

typedef struct _SYS_TOUCH_OBJ_STRUCT
{


} SYS_TOUCH_OBJECT;

extern SYS_TOUCH_OBJECT touchObject;

#ifdef __cplusplus
    }
#endif
    
#endif //#ifndef _SYS_TOUCH_LOCAL_H

/*******************************************************************************
 End of File
*/

