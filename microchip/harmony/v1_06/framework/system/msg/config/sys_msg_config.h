/*******************************************************************************
  System Messaging Configuration Definitions

  Company:
    Microchip Technology Inc.

  File Name:
    sys_msg_config.h

  Summary:
    System Messaging Configuration definitions file

  Description:
    These definitions statically define the operation of the System Messaging service.
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

#ifndef _SYS_MSG_CONFIG_TEMPLATE__H
#define _SYS_MSG_CONFIG_TEMPLATE__H


// *****************************************************************************
// *****************************************************************************
// Section: System Messaging Message Format Definition
// *****************************************************************************
// *****************************************************************************

// #ifndef _SYS_MSG_OBJECT
// #define _SYS_MSG_OBJECT
// /*****************************************************************************/
// /* System Message Format
//
//    Summary
//     Defines the contents of a system message.
//
//    Description
//     Defines the contents of a system message.  This information is copied into
//     an element of the system message queue identified by the message priority.
//
//    Remarks:
//     None
// */
//
// typedef struct
// {
//     union
//     {
//         struct
//         {
//             uint8_t nMessageTypeID; // Message type identifier
//             uint8_t        nSource; // Message source identifier
//             uint16_t        param0; // Message parameter zero
//             uint16_t        param1; // Message parameter one
//             uint16_t        param2; // Message parameter two
//         };
//         struct
//         {
//             uint16_t   dummy;
//             uint16_t   nSizeData; // Size of data that pData identifies
//             uintptr_t   *  pData; // Pointer to additional message data
//         };
//     };
//
// } SYS_MSG_OBJECT;
//
// #endif //_SYS_MSG_OBJECT


// *****************************************************************************
// *****************************************************************************
// Section: System Messaging Parameters: Mailboxes
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* System Messaging Max Number of Mailboxes

  Summary:
    Specifies the maximum number of mailboxes possible.

  Description:
    Specifies the maximum number of mailboxes possible.

  Remarks:
    Minimum number is 1 mailbox.

*/

#ifndef SYS_MSG_MAX_MAILBOXES
#define SYS_MSG_MAX_MAILBOXES  (2)
#endif


// *****************************************************************************
// *****************************************************************************
// Section: System Messaging Parameters: Message Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* System Messaging Max Number of Message Types

  Summary:
    Specifies the maximum number of message types possible.

  Description:
    Specifies the maximum number of message types possible.

  Remarks:
    Minimum number is 1.

*/

#ifndef SYS_MSG_MAX_TYPES
#define SYS_MSG_MAX_TYPES  (2)
#endif


// *****************************************************************************
// *****************************************************************************
// Section: System Messaging Parameters: Default Max Messages Delivered
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* System Messaging Maximum Number of Messages Delivered per call to SYS_MSG_Tasks

  Summary:
    Specifies the maximum number of messages delivered per each call to
    SYS_MSG_Tasks.

  Description:
    Specifies the maximum number of messages delivered per each call to
    SYS_MSG_Tasks.  If zero then all message queues are emptied before
    the tasks routine finishes execution.

  Remarks:
    0 implies all queues are empty after SYS_MSG_Tasks is done.
*/

#ifndef SYS_MSG_MAX_MSGS_DELIVERED
#define SYS_MSG_MAX_MSGS_DELIVERED (1)
#endif


// *****************************************************************************
// *****************************************************************************
// Section: System Messaging Parameters: Default Message Priorities
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* System Messaging Maximum Priority

  Summary:
    Specifies the maximum message priority.

  Description:
    Specifies the maximum message priority.

  Remarks:
    Message priorities run from 0 to SYS_MSG_MAX_PRIORITIES.
    The number of message queues is SYS_MSG_MAX_PRIORITIES+1.

*/

#ifndef SYS_MSG_MAX_PRIORITY
#define SYS_MSG_MAX_PRIORITY   (0)
#endif


// *****************************************************************************
/* System Messaging Buffer Sizes

  Summary:
    Specifies the size of message buffers

  Description:
    Specifies the size of message buffer for each message priority

  Remarks:
    Example: (with SYS_MSG_MAX_PRIORITY = 4 )
  <code>
    //           Message Priority:  0   1   2   3   4
    #define SYS_MSG_BUFFER_SIZES { 64, 32, 16,  8,  4 }
  </code>

*/

//#define SYS_MSG_BUFFER_SIZES { 4, 4, 4, 4, 1 }

#ifndef SYS_MSG_BUFFER_SIZES
#define SYS_MSG_BUFFER_SIZES { 16 }
#endif


#endif //#ifndef _SYS_MSG_CONFIG_TEMPLATE_H
