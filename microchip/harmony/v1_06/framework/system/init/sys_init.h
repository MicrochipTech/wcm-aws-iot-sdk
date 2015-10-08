/*******************************************************************************
  Initialization System-Library Definitions
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    sys_init.h

  Summary:
    Initialization system-library definitions and declarations

  Description:
    This file contains initialization system-library definitions and 
    declarations.
    
  Remarks:
    This file is included by "system.h" and need not be included directly.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2012 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _SYS_INIT_H    // Guards against multiple inclusion
#define _SYS_INIT_H


// *****************************************************************************
// *****************************************************************************
// Section: Included files
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include "system/common/sys_common.h"
#include "system/common/sys_module.h"


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Board Initialization Routine Pointer

  Function:
    bool <board_init_function_name> ( void )

  Summary:
    Pointer to a routine that initializes low-level board hardware and services
    necessary to initialize the system

  Description:
    This data type is a pointer to a routine that initializes any low-level 
    board hardware necessary to initialize the system services, drivers, and 
    other modules.

  Precondition:
    The C-language run-time environment and stack must have been initialized.
  
  Returns:
    true        - If the board was successfully initialized
    
    false       - If the board was not successfully initialized

  Example:
    None.

  Remarks:
    This routine must be implemented either by a Board Support Package or by the
    application (or both, since the application's board-initialization routine 
    can call a BSP's initialization routine).
*/

typedef bool (* BOARD_INIT_ROUTINE) ( void );


// *****************************************************************************
/* Application Initialization Routine Pointer

  Function:
    void <App_init_function_name> ( void )

  Summary:
    Pointer to a routine that initializes the application specific stuff.

  Description:
    This data type is a pointer to a routine that initializes any application
    specific stuff such as opening a driver, registering callbacks with the
    driver etc.

  Precondition:
    The C-language run-time environment and stack must have been initialized.
  
  Returns:
    None.

  Example:
    None.

  Remarks:
    This routine need not be registered with the system service if it is decided
    not to use the services of system init module or if the application uses the
    static style of implementation. Also in this case the application should
    invoke such routines explicitly.
*/

typedef void (* APP_INIT_ROUTINE) ( void );


// *****************************************************************************
/* Modules Registration Routine Pointer

  Function:
    void <modules_init_function_name> ( void )

  Summary:
    Pointer to a routine that registers all modules with the system.  

  Description:
    This data type is a pointer to a routine that registers all modules with the
    system.  This routine must register the module routines and index for each
    instance of a module with the system services.  This routine must also 
    register any "Tasks" routines required by each module with either the 
    System's Interrupt (SYS INT) service or the System's Tasks (SYS Tasks)
    service.

  Precondition:
    The C-language run-time environment and stack must have been initialized.
  
  Returns:
    None.

  Example:
    None.

  Remarks:
    This routine must be implemented either by an application configuration 
    or by a Board Support Package (or both, since the application's module-
    registration routine can call a BSP's registration routine).
*/

typedef void (* MODULES_REGISTERATION_ROUTINE) ( void );


// *****************************************************************************
/* Application Tasks Routine Pointer

  Function:
    void (* SYS_APP_TASK_ROUTINE) ( void );

  Summary:
    Pointer to a routine that performs the tasks necessary to maintain the
    functionality of the application.

  Description:
    This data type is a pointer to a routine that performs the tasks necessary
    to maintain the functionality of the application.

  Precondition:
    The low-level board initialization must have (and will be) completed and the
    module's initialization routine will have been called.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    Called by the SYS_Tasks routine.
*/

typedef void (* SYS_APP_TASK_ROUTINE) ( void );


// *****************************************************************************
/* System Initialization Data

  Summary:
    Data structure contains pointers to the initialization routines and data 
    required to initialize the system

  Description:
    This data structure contains pointers to the initialization routines and 
    data required to initialize the system.

  Remarks:
    None.
*/

typedef struct _SYS_INIT_DATA
{
    // Pointer to the initialization routine for the board
    BOARD_INIT_ROUTINE              callout_BoardInitialize;

    // Pointer to the application init function
    APP_INIT_ROUTINE                callout_AppInitialize;

    // Pointer to the application "Tasks" function
    SYS_APP_TASK_ROUTINE            pTasksApp;

    // Pointer to the modules registration routine
    MODULES_REGISTERATION_ROUTINE   callout_ModulesRegister;

    // Flag indicating if interrupts should be enabled
    bool                            enableInterrupts;

} SYS_INIT_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Initialize ( SYS_INIT_DATA *data )

  Summary:
    Initializes the board, services, drivers, and other modules

  Description:
    This routine initializes the board, services, drivers, and other modules
    as configured at build time.  In a bare-metal environment (where no OS is 
    supported), this routine should be called almost immediately after entering 
    the "main" routine.

  Precondition:
    The C-language run-time environment and stack must have been initialized.
  
  Parameters:
    data        - Pointer to the system initialization data structure containing
                  pointers to the board, system service, and driver 
                  initialization routines

  Returns:
    None.

  Example:
    <code>
    SYS_INIT_DATA   initData = 
    {
        // System-specific initialization data
    };
    
    int main ( void )
    {
        SYS_Initialize(&initData);
        
        while (true)
        {
            SYS_Tasks();
        }
    }
    </code>

  Remarks:
    Basic System Initialization Sequence:
    
    1.  Initialize minimal board services and processor-specific items
        (enough to use the board to initialize drivers and services)
    2.  Initialize all supported system services
    3.  Initialize all supported modules 
        (libraries, drivers, middleware, and application-level modules)
    4.  Wait for all services and modules to complete initialization
    5.  Start all supported services
    
    The order in which services and modules are initialized and started may be
    important and is controlled (in the default implementation) by the order in
    which they are placed in the initialization tables.
    
    This routine may be overridden and implemented as desired as long as it
    appropriately initializes all necessary board, services, and modules.
*/

void SYS_Initialize ( void *data );


/*******************************************************************************
  Function:
    SYS_MODULE_OBJ SYS_ModuleRegister ( SYS_MODULE_INTERFACE *interface, 
                                        SYS_TASKS_PRIORITY priority,
                                        unsigned int count )

  Summary:
    Registers the given module interface with the system

  Description:
    This routine registers the given module interface with the system so that 
    it can be initialized and maintained.

  Precondition:
    SYS_Initialize must have been called.
  
  Parameters:
    interface   - Pointer to the system module interface structure that has 
                  been initialized with pointers to the module's interface 
                  routines, instance index, and a pointer a data structure 
                  containing the module's initialization data.

  Returns:
    A non-NULL handle if the module was successfully registered and initialized
    
    NULL if the module was not successfully registered and initialized

  Example:
    <code>
    </code>

  Remarks:
    This routine is usually called from the module initialization callout, which
    must be implemented by the system designer or integrator if dynamic module
    registration is used. 
*/

SYS_MODULE_OBJ SYS_ModuleRegister ( SYS_MODULE_INTERFACE *interface, 
                                    SYS_TASKS_PRIORITY priority,
                                    unsigned int count );


#endif // _SYS_INIT_H

/*******************************************************************************
 End of File
*/

