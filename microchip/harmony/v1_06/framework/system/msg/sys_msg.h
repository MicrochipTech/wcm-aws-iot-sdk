/***********************************************************************
  Company:
    Microchip Technology Inc.
	
  File Name:
    sys_msg.h
	
  Summary:
    System Service for the messaging module.
	
  Description:
    Messaging System-Library Interface Definition.
    This file contains the interface definition for the messaging system
    service. It provides a way to interact with the messaging subsystem.
  ***********************************************************************/

//DOM-IGNORE-BEGIN
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
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT O
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

#ifndef _SYS_MSG_H
#define _SYS_MSG_H


// *****************************************************************************
// *****************************************************************************
// Section: Include Files (Also, see below for more include files)
// *****************************************************************************
// *****************************************************************************
#include "system_config.h"
#include "system/common/sys_module.h"
#include "system/msg/config/sys_msg_config.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  

    extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: type definitions
// *****************************************************************************
// *****************************************************************************

/*****************************************************************************/
/* SYS_MODULE_OBJ Rename

  Summary:
    SYS_MODULE_OBJ is badly named.  It should be SYS_MODULE_OBJ_HANDLE or
    something shorter.  For brevity, it is renamed to SYS_OBJ_HANDLE.

  Description:
    SYS_MODULE_OBJ is badly named.  It should be SYS_MODULE_OBJ_HANDLE or
    something shorter.  For brevity, it is renamed to SYS_OBJ_HANDLE.

  Remarks:
    None.
*/

typedef uintptr_t SYS_OBJ_HANDLE;
#define SYS_OBJ_HANDLE_INVALID  ( (SYS_OBJ_HANDLE) -1 )
#define SYS_OBJ_HANDLE_STATIC   ( (SYS_OBJ_HANDLE)  0 )


/*****************************************************************************/
/* Enumeration of the Allowable of System Messaging Instances

  Summary:
    System Messaging instances numbering is from 0,1, to SYS_MSG_MAX_INSTANCE.

  Description:
    System Messaging instances numbering is from 0,1, to SYS_MSG_MAX_INSTANCE.

  Remarks:
    None.
*/

typedef enum
{
    SYS_MSG_0,
    SYS_MSG_1,
    SYS_MSG_2,
    SYS_MSG_3,
    SYS_MSG_4,
    SYS_MSG_NUM_INSTANCES,

} SYS_MSG_INSTANCE;


#ifndef _SYS_MSG_OBJECT
#define _SYS_MSG_OBJECT
/*****************************************************************************/
/* System Message Format

   Summary
    Defines the contents of a system message.

   Description
    Defines the contents of a system message.  This information is copied into
    an element of the system message queue identified by the message priority.

   Remarks:
    None.
*/

typedef struct
{
    union
    {
        struct
        {
            uint8_t nMessageTypeID; // Message type identifier
            uint8_t        nSource; // Message source identifier
            uint16_t        param0; // Message parameter zero
            uint16_t        param1; // Message parameter one
            uint16_t        param2; // Message parameter two
        };
        struct
        {
            uint16_t   dummy;
            uint16_t   nSizeData; // Size of data that pData identifies
            uintptr_t   *  pData; // Pointer to additional message data
        };
    };

} SYS_MSG_OBJECT;
#endif //_SYS_MSG_OBJECT


// *****************************************************************************
/* System Messaging: Number of 16-Bit Bit Maps for Mailboxes

  Summary:
    Specifies the number of 16-bit bit maps needed for mailboxes.

  Description:
    Specifies the number of 16-bit bit maps needed for mailboxes.

  Remarks:
    None.

*/
#if ( SYS_MSG_MAX_MAILBOXES % 16 !=0 )  // need to round up the number of bitmaps
    #define SYS_MSG_MAILBOXES_ADDONE 1
#else                                   // don't need to round up the number of bitmaps
    #define SYS_MSG_MAILBOXES_ADDONE 0
#endif

#define SYS_MSG_NUM_MAILBOX_BITMAPS  (SYS_MSG_MAX_MAILBOXES/16 + SYS_MSG_MAILBOXES_ADDONE)


/*****************************************************************************/
/* System Messaging Queues Element

   Summary
    Defines queue element for message queue belonging to each priority.

   Description
    Defines queue element for message queue belonging to each priority.

   Remarks:
    None..
*/

typedef struct
{
    SYS_MSG_OBJECT  sMessage; // System Message
 // Bit map for mailboxes interested in this message type, modified as each mailbox is notified.
    uint16_t mailboxInterestBitMap[SYS_MSG_NUM_MAILBOX_BITMAPS];

} SYS_MSGQ_ELEMENT;


/*****************************************************************************/
/* System Messaging Queue Status Enumeration

  Summary:
    Messaging queue status enumeration.

  Description:
    Messaging queue status enumeration.  Positive values indicate number of
    messages in the queue.

  Remarks:
    None.
*/

typedef enum
{
 // QUEUE Status: full
    SYS_MSG_QUEUE_BAD    /*DOM-IGNORE-BEGIN*/ = SYS_STATUS_ERROR_EXTENDED /*DOM-IGNORE-END*/ ,

 // QUEUE Status: full
    SYS_MSG_QUEUE_FULL   /*DOM-IGNORE-BEGIN*/ = -1 /*DOM-IGNORE-END*/ ,

 // QUEUE Status: empty
    SYS_MSG_QUEUE_EMPTY  /*DOM-IGNORE-BEGIN*/ =  0 /*DOM-IGNORE-END*/

} SYS_MSG_QUEUE_STATUS;


/*****************************************************************************/
/* System Messaging Results Enumeration

  Summary:
    Enumeration of message send results.

  Description:
    Enumeration of message send results.

  Remarks:
    SYS_MSG_SENT aligns with SYS_MSGQ_Success.
    SYS_MSG_NOT_SENT aligns with SYS_MSGQ_Failure
*/

typedef enum
{
 // Message could not be sent, no room available in priority queues
    SYS_MSG_NOT_SENT_QFULL      /*DOM-IGNORE-BEGIN*/ = SYS_STATUS_ERROR_EXTENDED - 3 /*DOM-IGNORE-END*/ ,

 // Message could not be sent, Message Message priority bad
    SYS_MSG_BAD_PRIORITY        /*DOM-IGNORE-BEGIN*/ = SYS_STATUS_ERROR_EXTENDED - 2 /*DOM-IGNORE-END*/ ,

 // Message could not be sent, Message type bad
    SYS_MSG_BAD_MSGTYPE         /*DOM-IGNORE-BEGIN*/ = SYS_STATUS_ERROR_EXTENDED - 1 /*DOM-IGNORE-END*/ ,

 // Message could not be sent, no other information available
    SYS_MSG_NOT_SENT            /*DOM-IGNORE-BEGIN*/ = SYS_STATUS_ERROR_EXTENDED - 0 /*DOM-IGNORE-END*/ ,

 // Message sent
    SYS_MSG_SENT                /*DOM-IGNORE-BEGIN*/ = SYS_STATUS_READY_EXTENDED + 0 /*DOM-IGNORE-END*/ ,

} SYS_MSG_RESULTS;


// *****************************************************************************
/* Pointer to the System Message Received Callback Function

  Summary:
    Pointer to the System Message Received Callback Function

  Description:
    Pointer to the function provided for each system messaging mailbox that is
    called when a system message is received for each mailbox.
 */
typedef void (*SYS_MSG_RECEIVE_CALLBACK) ( SYS_MSG_OBJECT *pMessage );


// *****************************************************************************
/* System Messaging Service Initialization Data

  Summary:
    Contains all the data necessary to initialize an instance of the System
    Messaging Service.

  Description:
    Contains all the data necessary to initialize an instance of the System
    Messaging Service.

  Remarks:
    A pointer to a structure of this format containing the desired
    initialization data must be passed into the SYS_MSG_Initialize routine.
    If nMaxMsgsDelivered == 0 then ALL messages in priority queues are delivered
    each time SYS_MSG_Tasks is called.
*/

typedef struct
{
    /* Maximum number of messages delivered per call to SYS_MSG_Tasks */
    uint8_t   nMaxMsgsDelivered;

    /* Number of message priorities desired */
    uint8_t   nMessagePriorities;

    /* // Array of queue sizes for priorities 0,1,...SYS_MSG_MAX_PRIORITY */
    uint16_t  * nQSizes;

} SYS_MSG_INIT;



// *****************************************************************************
// *****************************************************************************
// Section: Initialization
// *****************************************************************************
// *****************************************************************************

/******************************************************************************

  Function:
    SYS_OBJ_HANDLE SYS_MSG_Initialize ( const SYS_MSG_INSTANCE iSysMsg,
                                        SYS_OBJ_HANDLE pInitSysMsg    )

  Summary:
    Configures and initializes the messaging subsystem.

  Description:
    This  routine configures and initializes the messaging subsystem
    appropriately for the current system design.

  Precondition:
    None.

  Parameters:
    iSysMsg - Index of System Messaging Service to be initialized.
    pInitSysMsg - Pointer to System Messaging initialization data structure.
                  If NULL default config values are used.

  Returns:
    Handle to the System Messaging object created.
    Returns SYS_OBJ_HANDLE_INVALID if allocation of data structure fails.
    Returns SYS_OBJ_HANDLE_INVALID if pointer to initialization data structure is NULL.

  Example:
  <code>
    SYS_OBJ_HANDLE hSysMsg;
    SYS_MSG_INIT sInitSysMsg = { 0, (SYS_MSG_MAX_PRIORITY+1), { SYS_MSG_BUFFER_SIZES } };
  //uint16_t nQSizes[] = SYS_MSG_BUFFER_SIZES;

    // Initialize the messaging system. This needs to done in the initialization code.
    // Choose System Messaging instance that supports 8 byte messages
    hSysMsg = SYS_MSG_Initialize(SYS_MSG_8Bytes,&sInitSysMsg);

    if (SYS_OBJ_HANDLE_INVALID == hSysMsg)
    {
        // Handle error
    }

  </code>

  Remarks:
    None.
*/

SYS_OBJ_HANDLE SYS_MSG_Initialize ( const SYS_MSG_INSTANCE iSysMsg,
                                    SYS_OBJ_HANDLE pInitializeSysMsg );


//******************************************************************************
/* Function:
    void  SYS_MSG_Deinitialize ( SYS_OBJ_HANDLE hSysMsg )

  Summary:
    Deinitializes System Messaging Instance.

  Description:
    Deinitializes System Messaging Instance and frees up allocated memory for it.

  Precondition:
    None.

  Parameters:
    hSysMsg - handle to System Messaging Object for instance to be removed.

  Returns:
    None.

  Example:
  <code>
    SYS_OBJ_HANDLE hSysMsg;
    uint16_t nQSizes[] = SYS_MSG_BUFFER_SIZES;

    // Initialize the messaging system. This needs to done in the initialization code.
    // Choose System Messaging instance that supports 8 byte messages
    hSysMsg = SYS_MSG_Initialize(SYS_MSG_8Bytes,SYS_MSG_MAX_PRIORITY+1,nQSizes);

    if (SYS_OBJ_HANDLE_INVALID == hSysMsg)
    {
        // Handle error
    }
    .
    .
    .
    // Remove this instance.
    SYS_MSG_Deinitialize( hSysMsg );

  </code>

  Remarks:
    None.
*/

void  SYS_MSG_Deinitialize ( SYS_OBJ_HANDLE handleSysMsg );


//******************************************************************************
/* Function:
    void  SYS_MSG_Tasks ( SYS_OBJ_HANDLE hSysMsg )

  Summary:
    System Messaging Service Tasks routine.

  Description:
    System Messaging Service Tasks routine.

  Precondition:
    hSysMsg must have been returned from a call to SYS_MSG_Initialize.

  Parameters:
    hSysMsg - handle to System Messaging Object.

  Returns:
    None.

  Example:
  <code>
    while ( SYS_MSG_GotMessages(iSysMsg) )
    {
        SYS_MSG_Tasks(hSysMsg);
    }
  </code>

  Remarks:
    None.
*/

void  SYS_MSG_Tasks ( SYS_OBJ_HANDLE handleSysMsg );


// *****************************************************************************
// *****************************************************************************
// Section: Messages
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_OBJ_HANDLE SYS_MSG_TypeCreate( const SYS_MSG_INSTANCE iSysMsg,
                                       uint8_t  nMessageTypeID,
                                       uint8_t  nMessagePriority )
  Summary:
    Creates a new message type.

  Description:
    Creates a new message type, defining an integer message type and priority.

  Precondition:
    iSysMsg must have been used in a call to SYS_MSG_Initialize.

  Parameters:
    iSysMsg          - Index of System Messaging Service instance.
    nMessageTypeID   - Integer message type identifier
    nMessagePriority - Message priority, between 0 and SYS_MSG_MAX_PRIORITIES

  Returns:
    Handle to new message type definition.

  Example:
  <code>
    SYS_OBJ_HANDLE hMsgType;
    hMsgType = SYS_MSG_TypeCreate( iSysMsg, 1, 3 );
    if (SYS_OBJ_HANDLE_INVALID == hMsgType)
    {
        // Handle error
    }
  </code>

  Remarks:
    None.
*/

SYS_OBJ_HANDLE SYS_MSG_TypeCreate( const SYS_MSG_INSTANCE iSysMsg,
                                   uint8_t  nMessageTypeID,
                                   uint8_t  nMessagePriority );


// *****************************************************************************
/* Function:
    void SYS_MSG_TypeRemove( SYS_OBJ_HANDLE hMsgType )

  Summary:
    Removes an existing message type.

  Description:
    Removes an existing message type.

  Precondition:
    None.

  Parameters:
    hMsgType   - Handle to message type that is to be removed

  Returns:
    None.

  Example:
  <code>
    SYS_OBJ_HANDLE hMsgType;
    hMsgType = SYS_MSG_TypeCreate( iSysMsg, 1, 3 );
    SYS_MSG_TypeRemove( hMsgType );
  </code>
  Alternately:
  <code>
    SYS_OBJ_HANDLE hMsgType;
    hMsgType = SYS_MSG_TypeCreate( iSysMsg, 1, 3 );
    SYS_MSG_TypeRemove( SYS_MSG_ID2hMsgType(iSysMsg,1) );
  </code>

  Remarks:
    When the message type handle is unknown but the message ID is known use
    SYS_MSG_ID2hMsgType to provide the message type handle.  See code example.
*/

void SYS_MSG_TypeRemove( SYS_OBJ_HANDLE hMsgType );


// *****************************************************************************
/* Function:
    SYS_OBJ_HANDLE SYS_MSG_ID2hMsgType( const   SYS_MSG_INSTANCE iSysMsg,
                                        uint8_t nMessageTypeID          )
  Summary:
    Translates message type identifier into handle of corresponding message type object.

  Description:
    Translates message type identifier into handle of corresponding message type object.

  Precondition:
    iSysMsg must have been used in a call to SYS_MSG_Initialize.

  Parameters:
    iSysMsg          - Index of System Messaging Service instance.
    nMessageTypeID   - Integer message type identifier.

  Returns:
    Handle to message type definition object corresponding to the message type
    identifier.  Returns NULL if the message type is not defined.

  Example:
  <code>
#define MY_MESSAGE_TYPE_ID 1;

    // Create message type
    SYS_OBJ_HANDLE hMsgType;
    uint8_t nPriority = 3;
    hMsgType = SYS_MSG_TypeCreate( iSysMsg, MY_MESSAGE_TYPE_ID, nPriority );
    if (SYS_OBJ_HANDLE_INVALID == hMsgType)
    {
        // Handle error
    }
    .
    .
    .
    // Remove message type without knowing message type handle
    SYS_MSG_TypeRemove(iSysMsg,SYS_MSG_ID2hMsgType(iSysMsg,MY_MESSAGE_TYPE_ID));
  </code>

  Remarks:
    This function is useful in situations where the message type identifier
    is known but the message type handle is not.  This allows applications
    to statically define message type IDs and use them in code instead of
    having to wait until message handles are known and then dynamically sharing
    message handles via global variables.
*/
SYS_OBJ_HANDLE SYS_MSG_ID2hMsgType( const   SYS_MSG_INSTANCE iSysMsg,
                                    uint8_t nMessageTypeID          );


// *****************************************************************************
/* Function:
    bool SYS_MSG_GotMessages( const SYS_MSG_INSTANCE iSysMsg )

  Summary:
    Returns true if system messaging has undelivered messages, false otherwise.

  Description:
    Returns true if system messaging has undelivered messages, false otherwise.

  Precondition:
    iSysMsg must have been used in a call to SYS_MSG_Initialize.

  Parameters:
    iSysMsg  - Index of System Messaging Service instance.

  Returns:
    True if there are undelivered system messages, false otherwise.

  Example:
  <code>
    DBPRINTF("\r\nGot Messages: %d\r\n",SYS_MSG_GotMessages(iSysMsg));
    while ( (pNextMessage = SYS_MSG_MessageReceive(iSysMsg)) != NULL )
    {
        SYS_MSG_MessageDeliver(iSysMsg,pNextMessage);
        DBPRINTF("\r\n");
    }
    DBPRINTF("Got Messages: %d\r\n",SYS_MSG_GotMessages(ihSysMsg));
  </code>

  Remarks:
    None.
*/

bool SYS_MSG_GotMessages( const SYS_MSG_INSTANCE iSysMsg );


// *****************************************************************************
/* Function:
    SYS_MSG_RESULTS SYS_MSG_MessageSend( const SYS_MSG_INSTANCE iSysMsg,
                                         SYS_MSG_OBJECT *pMessage   )
  Summary:
    Sends a message, as defined by a message structure.

  Description:
    Sends a message, as defined by a message structure.

  Precondition:
    iSysMsg must have been used in a call to SYS_MSG_Initialize.

  Parameters:
    iSysMsg   - Index of System Messaging Service instance.
    pMessage  - Pointer to message definition.

  Returns:
    Message result from SYS_MSG_RESULTS enumeration.

  Example:
  <code>
    SYS_MSG_OBJECT myMessage;
    SYS_MSG_RESULTS myMessageStatus;
    SYS_OBJ_HANDLE hMyMsgType;

    hMyMsgType = SYS_MSG_TypeCreate( iSysMsg, 1, 3 );
    if (SYS_OBJ_HANDLE_INVALID == hMyMsgType)
    {
        // Handle error
    }

    myMessage.hMsgType = hMyMsgType;
    myMessage.nSource  = myMsgSource;
    myMessage.param1   = parameterOneValue;
    myMessage.param1   = parameterTwoValue;

    myMessageStatus = SYS_MSG_MessageSend(iSysMsg,&myMessage);
    SYS_ASSERT( myMessageStatus > 0, "Bad message status!" );

  </code>

  Remarks:
    None.
*/

SYS_MSG_RESULTS SYS_MSG_MessageSend( const SYS_MSG_INSTANCE iSysMsg,
                                     SYS_MSG_OBJECT *pMessage );


// *****************************************************************************
/* Function:
    SYS_MSGQ_ELEMENT *SYS_MSG_MessageReceive( const SYS_MSG_INSTANCE iSysMsg )

  Summary:
    Receive next message in message queues.

  Description:
    Receive next message in message queues, returning NULL if queues are empty.

  Precondition:
    iSysMsg must have been used in a call to SYS_MSG_Initialize.

  Parameters:
    iSysMsg  - Index of System Messaging Service instance.

  Returns:
    Pointer to next message, as found in a message queue element, NULL if there are no messages.

  Example:
  <code>
    SYS_MSGQ_ELEMENT *pQElement;
    SYS_MSG_OBJECT nextMessage;
    while ( (pQElement = SYS_MSG_MessageReceive(iSysMsg)) != NULL )
    {
        // In case you desire to examine message before delivering it.
        nextMessage = pQElement->sMessage;

        // Deliver message to all interested mailboxes
        SYS_MSG_MessageDeliver(iSysMsg,pQElement);
    }
  </code>

  Remarks:
    None.
*/

SYS_MSGQ_ELEMENT *SYS_MSG_MessageReceive( const SYS_MSG_INSTANCE iSysMsg );


// *****************************************************************************
/* Function:
    void SYS_MSG_MessageDeliver( const SYS_MSG_INSTANCE iSysMsg, SYS_MSGQ_ELEMENT *pQElement );

  Summary:
    Deliver message to mailboxes.

  Description:
    Deliver message to mailboxes, remove message from queue when done.

  Precondition:
    iSysMsg must have been used in a call to SYS_MSG_Initialize.

  Parameters:
    iSysMsg  - Index of System Messaging Service instance.
    pQElement - pointer to queue element to be delivered.

  Returns:
    None.

  Example:
  <code>
    SYS_MSGQ_ELEMENT *pQElement;
    SYS_MSG_OBJECT nextMessage;
    while ( (pQElement = SYS_MSG_MessageReceive(iSysMsg)) != NULL )
    {
        // In case you desire to examine message before delivering it.
        nextMessage = pQElement->sMessage;

        // Deliver message to all interested mailboxes
        SYS_MSG_MessageDeliver(iSysMsg,pQElement);
    }
  </code>

  Remarks:
    None.
*/

void SYS_MSG_MessageDeliver( const SYS_MSG_INSTANCE iSysMsg, SYS_MSGQ_ELEMENT *pQElement );


// *****************************************************************************
// *****************************************************************************
// Section: Mailboxes
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_OBJ_HANDLE SYS_MSG_MailboxOpen( const SYS_MSG_INSTANCE iSysMsg,
                                        SYS_MSG_RECEIVE_CALLBACK msgCallBackFunction );

  Summary:
    Opens a system messaging mailbox.

  Description:
    Opens a system messaging mailbox, providing a message callback function that is
    called whenever a message is received.

  Precondition:
    iSysMsg must have been used in a call to SYS_MSG_Initialize.
    Message callback function will not be called until SYS_MSG_MailboxSignUp has been
    used to sign up the mailbox for messages of interest.

  Parameters:
    iSysMsg  - Index of System Messaging Service instance.
    msgCallBackFunction  - pointer to message callback function

  Returns:
    Handle to new system messaging mailbox.

  Example:
  <code>
    SYS_OBJ_HANDLE hMyMailbox;

    hMyMailbox = SYS_MSG_MailboxOpen( iSysMsg, &myCallBackFunction );
    if (SYS_OBJ_HANDLE_INVALID == SYS_MSG_TypeRemove)
    {
        // Handle error
    }

  </code>

  Remarks:
    A null callback function disables messaging callbacks.
*/

SYS_OBJ_HANDLE SYS_MSG_MailboxOpen( const SYS_MSG_INSTANCE iSysMsg,
                                    SYS_MSG_RECEIVE_CALLBACK msgCallBackFunction );


// *****************************************************************************
/* Function:
    void SYS_MSG_MailboxClose( SYS_OBJ_HANDLE hMailbox )

  Summary:
    Closes (destroys) a mailbox previously opened with SYS_MSG_MailboxOpen.

  Description:
    Closes (destroys) a mailbox previously opened with SYS_MSG_MailboxOpen.

  Precondition:
    hMailbox provided by call to SYS_MSG_MailboxOpen.

  Parameters:
    hMailbox -  Handle to mailbox that is to be closed (destroyed).

  Returns:
    None.

  Example:
  <code>
    SYS_OBJ_HANDLE hMyMailbox;

    hMyMailbox = SYS_MSG_MailboxOpen( iSysMsg, &myCallBackFunction );
    SYS_MSG_MailboxClose(hMyMailbox);
  </code>

  Returns:
    None.
*/

void SYS_MSG_MailboxClose( SYS_OBJ_HANDLE hMailbox );


// *****************************************************************************
/* Function:
    void SYS_MSG_MailboxReinit( SYS_OBJ_HANDLE hMailbox,
                                SYS_MSG_RECEIVE_CALLBACK msgCallBackFunction );

  Summary:
    Reinitialize previously opened mailbox.

  Description:
    Reinitialize previously opened mailbox by providing new call back function
    and clearing all message type assignments.

  Precondition:
    hMailbox provided by call to SYS_MSG_MailboxOpen.

  Parameters:
    hMailbox - Object handle to mailbox
    msgCallBackFunction - pointer to new message callback function

  Example:
  <code>
    SYS_OBJ_HANDLE hMyMailbox;

    hMyMailbox = SYS_MSG_MailboxOpen( iSysMsg, &myCallBackFunction );
    SYS_MSG_MailboxReinit(hMyMailbox,&anotherCallBackFunction );
  </code>

  Remarks:
    A null callback function disables messaging callbacks.
*/

void SYS_MSG_MailboxReinit( SYS_OBJ_HANDLE hMailbox,
                            SYS_MSG_RECEIVE_CALLBACK msgCallBackFunction );


// *****************************************************************************
/* Function:
    void SYS_MSG_MailboxMsgAdd( SYS_OBJ_HANDLE hMailbox, SYS_OBJ_HANDLE hMsgType )

  Summary:
    Adds a message type to the list of messages received by a mailbox.

  Description:
    Adds a message type to the list of messages received by a mailbox.

  Precondition:
    hMailbox provided by call to SYS_MSG_MailboxOpen.

  Parameters:
    hMailbox - Object handle to mailbox
    hMsgType - Handle to message type of interest for this mailbox.

  Returns:
    None.

  Example:
  <code>
    const SYS_MSG_INSTANCE iSysMsg;
    SYS_OBJ_HANDLE hMsgType[5], hMailbox[3];

    // Create three mailboxes
    hMailbox[0] = SYS_MSG_MailboxOpen( iSysMsg, &myCallBack0 );
    hMailbox[1] = SYS_MSG_MailboxOpen( iSysMsg, &myCallBack1 );
    hMailbox[2] = SYS_MSG_MailboxOpen( iSysMsg, &myCallBack2 );

    // Create five message types
    //                               Message ID:  :Priority
    hMsgType[0] = SYS_MSG_TypeCreate(iSysMsg,1<<0,0);
    hMsgType[1] = SYS_MSG_TypeCreate(iSysMsg,1<<1,1);
    hMsgType[2] = SYS_MSG_TypeCreate(iSysMsg,1<<2,2);
    hMsgType[3] = SYS_MSG_TypeCreate(iSysMsg,1<<3,3);
    hMsgType[4] = SYS_MSG_TypeCreate(iSysMsg,1<<4,4);

    // Add messages to each mailbox
    SYS_MSG_MailboxMsgAdd(hMailbox[0],hMsgType[0]);
    SYS_MSG_MailboxMsgAdd(hMailbox[1],hMsgType[1]);
    SYS_MSG_MailboxMsgAdd(hMailbox[2],hMsgType[2]);

    SYS_MSG_MailboxMsgAdd(hMailbox[0],SYS_MSG_ID2hMsgType(iSysMsg,1<<1));
    SYS_MSG_MailboxMsgAdd(hMailbox[0],SYS_MSG_ID2hMsgType(iSysMsg,1<<2));
    SYS_MSG_MailboxMsgAdd(hMailbox[0],SYS_MSG_ID2hMsgType(iSysMsg,1<<3));
    SYS_MSG_MailboxMsgAdd(hMailbox[0],SYS_MSG_ID2hMsgType(iSysMsg,1<<4));
  </code>

  Remarks:
    When the message type handle is unknown but the message ID is known use
    SYS_MSG_ID2hMsgType to provide the message type handle.  See code example.
*/

void SYS_MSG_MailboxMsgAdd( SYS_OBJ_HANDLE hMailbox, SYS_OBJ_HANDLE hMsgType );


// *****************************************************************************
/* Function:
    void SYS_MSG_MailboxMsgRemove( SYS_OBJ_HANDLE hMailbox, SYS_OBJ_HANDLE hMsgType )

  Summary:
    Removes a message type from the list of messages received by a mailbox.

  Description:
    Removes a message type from the list of messages received by a mailbox.

  Precondition:
    hMailbox provided by call to SYS_MSG_MailboxOpen.

  Parameters:
    hMailbox - Object handle to mailbox
    hMsgType - Handle to message type to be ignored by this mailbox.

  Returns:
    None.

  Example:
  <code>
    const SYS_MSG_INSTANCE iSysMsg;
    SYS_OBJ_HANDLE hMsgType[5], hMailbox[3];

    // Create three mailboxes
    hMailbox[0] = SYS_MSG_MailboxOpen( iSysMsg, &myCallBack0 );
    hMailbox[1] = SYS_MSG_MailboxOpen( iSysMsg, &myCallBack1 );
    hMailbox[2] = SYS_MSG_MailboxOpen( iSysMsg, &myCallBack2 );

    // Create five message types
    //                               Message ID:  :Priority
    hMsgType[0] = SYS_MSG_TypeCreate(iSysMsg,1<<0,0);
    hMsgType[1] = SYS_MSG_TypeCreate(iSysMsg,1<<1,1);
    hMsgType[2] = SYS_MSG_TypeCreate(iSysMsg,1<<2,2);
    hMsgType[3] = SYS_MSG_TypeCreate(iSysMsg,1<<3,3);
    hMsgType[4] = SYS_MSG_TypeCreate(iSysMsg,1<<4,4);

    // Add messages to each mailbox
    SYS_MSG_MailboxMsgAdd(hMailbox[0],hMsgType[0]);
    SYS_MSG_MailboxMsgAdd(hMailbox[1],hMsgType[1]);
    SYS_MSG_MailboxMsgAdd(hMailbox[2],hMsgType[2]);

    SYS_MSG_MailboxMsgAdd(hMailbox[0],SYS_MSG_ID2hMsgType(iSysMsg,1<<1));
    SYS_MSG_MailboxMsgAdd(hMailbox[0],SYS_MSG_ID2hMsgType(iSysMsg,1<<2));
    SYS_MSG_MailboxMsgAdd(hMailbox[0],SYS_MSG_ID2hMsgType(iSysMsg,1<<3));
    SYS_MSG_MailboxMsgAdd(hMailbox[0],SYS_MSG_ID2hMsgType(iSysMsg,1<<4));
  </code>

  Remarks:
    When the message type handle is unknown but the message ID is known use
    SYS_MSG_ID2hMsgType to provide the message type handle.  See code example.
*/

void SYS_MSG_MailboxMsgRemove( SYS_OBJ_HANDLE hMailbox, SYS_OBJ_HANDLE hMsgType );


// *****************************************************************************
/* Function:
    SYS_MSG_OBJECT *SYS_MSG_MailboxMessagesGet( SYS_OBJ_HANDLE hMailbox )

  Summary:
    Get queued messages for a mailbox.

  Description:
    Get queued messages for a mailbox.  Messages returned by this function will
    not be received via the mailbox's callback function.

  Precondition:
    hMailbox provided by call to SYS_MSG_MailboxOpen.

  Parameters:
    hMailbox - Object handle to mailbox

  Returns:
    Pointer to next message in the queue(s) that is of interest to the mailbox.
    Function returns NULL if no messages are found in the queue.

  Example:
  <code>
    SYS_MSG_OBJECT *pNextMessage;
    while ( NULL != (pNextMessage = SYS_MSG_MailboxMessagesGet(hMyMailbox)) )
    {
        // Process message at *pNextMessage.
    }
  </code>

  Remarks:
    None.
*/

SYS_MSG_OBJECT *SYS_MSG_MailboxMessagesGet( SYS_OBJ_HANDLE hMailbox );


// *****************************************************************************
// *****************************************************************************
// Section: Message Queues
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MSG_QUEUE_STATUS SYS_MSG_QueueStatus( const SYS_MSG_INSTANCE iSysMsg, uint8_t nMessagePriority )

  Summary:
    Returns message queue status for a given message priority.

  Description:
    Returns message queue status for a given message priority.

  Precondition:
    iSysMsg must have been used in a call to SYS_MSG_Initialize.

  Parameters:
    iSysMsg  - Index of System Messaging Service instance.
    nMessagePriority  - message priority of interest, from zero to SYS_MSG_MAX_PRIORITIES.

  Returns:
    Number of messages in queue if not full or SYS_MSG_QUEUE_FULL if full.
    If message priority is not legal, returns SYS_MSG_QUEUE_BAD.

  Example:
  <code>
  </code>

  Remarks:
    None.
*/

SYS_MSG_QUEUE_STATUS SYS_MSG_QueueStatus( const SYS_MSG_INSTANCE iSysMsg, uint8_t nMessagePriority );


// *****************************************************************************
// *****************************************************************************
// Section: Included Files (continued)
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif // _SYS_MSG_H

