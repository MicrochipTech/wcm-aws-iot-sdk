/*******************************************************************************
  System Messaging Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    sys_msg_local.h

  Summary:
    System Messaging local declarations and definitions

  Description:
    This file contains local declarations and definitions for System Messaging.
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

#ifndef _SYS_MSG_LOCAL_H
#define _SYS_MSG_LOCAL_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "system/msg/sys_msg.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

/*****************************************************************************/
/* System Messaging Queue Results

   Summary
    Messaging Queue primitive results enumeration.

   Description
    Messaging Queue primitive results enumeration.

   Remarks:
    None
*/
typedef enum
{
 // Failure
    SYS_MSGQ_Failure  /*DOM-IGNORE-BEGIN*/ = SYS_STATUS_ERROR_EXTENDED - 0 /*DOM-IGNORE-END*/ ,

 // Message sent
    SYS_MSGQ_Success  /*DOM-IGNORE-BEGIN*/ = SYS_STATUS_READY_EXTENDED + 0 /*DOM-IGNORE-END*/

} SYS_MSGQ_RESULT;


// *****************************************************************************
/* System Messaging: Number of 16-Bit Bit Maps for Message Types

  Summary:
    Specifies the number of 16-bit bit maps needed for message types.

  Description:
    Specifies the number of 16-bit bit maps needed for message types.

  Remarks:
    None

*/
#if ( SYS_MSG_MAX_TYPES % 16 !=0 )  // need to round up the number of bitmaps
    #define SYS_MSG_TYPES_ADDONE 1
#else                               // don't need to round up the number of bitmaps
    #define SYS_MSG_TYPES_ADDONE 0
#endif

#define SYS_MSG_NUM_TYPES_BITMAPS  (SYS_MSG_MAX_TYPES/16 + SYS_MSG_TYPES_ADDONE)


/*****************************************************************************/
/* System Messaging Mailbox Object

   Summary

   Description

   Remarks:
    None
*/

typedef struct
{
    struct _SYS_MSG_MESSAGING_OBJECT *  hSysMsg; // Handle of System Messaging object that owns this mailbox
    uint8_t        iMailbox; // Index into array of mailboxes

 // Pointer to message received callback function
    SYS_MSG_RECEIVE_CALLBACK  msgCallBackFunction;

 // Bit map of messages of interest for this mailbox
    uint16_t  msgInterestBitMap[SYS_MSG_NUM_TYPES_BITMAPS];

} SYS_MSG_MAILBOX_OBJECT;


/*****************************************************************************/
/* Message Type Definition Object

   Summary

   Description

   Remarks:
    None
*/

typedef struct
{
    struct _SYS_MSG_MESSAGING_OBJECT * hSysMsg; // Handle of System Messaging object that owns this message type
    uint8_t      iMessageType; // Index into array of message type definitions
    uint8_t    nMessageTypeID; // Message type identifier
    uint8_t  nMessagePriority; // Message priority

 // Bit map for mailboxes interested in this message type, modified as each mailbox is notified.
    uint16_t mailboxInterestBitMap[SYS_MSG_NUM_MAILBOX_BITMAPS];

} SYS_MSG_MESSAGE_DEFN;


/*****************************************************************************/
/* System Messaging Queue

   Summary
    Defines the message queue belonging to each message priority.

   Description
    Defines the message queue belonging to each message priority.

   Remarks:
    None
*/

typedef struct
{
    int        nQSize;  /* maximum number of elements           */
    int        nCount;  /* number of elements in queue          */
    int        iStart;  /* index of oldest element              */
    int        iEnd;    /* index at which to write new element  */
    SYS_MSGQ_ELEMENT *qElements;  /* vector of elements  */

} SYS_MSG_QUEUE;


/*****************************************************************************/
/* System Messaging Queues Object

   Summary
    Defines set of system message queues.

   Description
    Defines set of system message queues, one queue for each message priority.

   Remarks:
    None
*/

typedef struct _SYS_MSG_MESSAGING_OBJECT
{
    uint8_t   nMessagePriorities; // Number of message priorities (number of queues)
    uint8_t  nMessageTypeIDs[SYS_MSG_MAX_TYPES]; // Associated Message IDs

    uint8_t        nMessageTypes; // # of active message type definitions
    uint8_t           nMailboxes; // # of active mailboxes

    uint16_t   nMaxMsgsDelivered; // Maximum # of messages delivered per call to SYS_MSG_Tasks

    SYS_MSG_QUEUE         msgQueues[SYS_MSG_MAX_PRIORITY+1]; // Messaging queues array
    SYS_MSG_MESSAGE_DEFN   *pMessageType[SYS_MSG_MAX_TYPES];   // Pointers to message defn objects
    SYS_MSG_MAILBOX_OBJECT  *pMailbox[SYS_MSG_MAX_MAILBOXES]; // pointers to mailbox objects

} SYS_MSG_MESSAGING_OBJECT;


// *****************************************************************************
// *****************************************************************************
// Section: Individual Queue Support
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MSGQ_RESULT SYS_MSGQ_Init(SYS_MSG_QUEUE *pQ, int nQSize)

  Summary:
    Initialized individual queue.

  Description:
    Initialized individual queue, including allocating memory for queue elements.

  Precondition:
    None

  Parameters:
    pQ - pointer to queue to be initialized
    nQSize - number of queue elements for this queue

  Returns:
    SYS_MSGQ_Success ( if allocation of queue elements succeeds ) or
    SYS_MSGQ_Failure ( if allocation fails )

  Example:
  <code>
    SYS_MSG_QUEUE newQ;
    SYS_MSGQ_RESULT initResult;
    initResult = SYS_MSGQ_Init(&newQ,10);
    SYS_ASSERT( SYS_MSGQ_Success == initResult );
  </code>

  Remarks:
    None.
*/

SYS_MSGQ_RESULT SYS_MSGQ_Init(SYS_MSG_QUEUE *pQ, int nQSize);


// *****************************************************************************
/* Function:
    void SYS_MSGQ_Delete(SYS_MSG_QUEUE *pQ)

  Summary:
    Deletes existing message queue.

  Description:
    Deletes existing message queue, including deallocation of queue elements.

  Precondition:
    Message queue should have been created by SYS_MSGQ_Init.

  Parameters:
    pQ - pointer to message queue to be deleted.

  Returns:
    None.

  Example:
  <code>
    SYS_MSG_QUEUE newQ;
    SYS_MSGQ_RESULT initResult;
    initResult = SYS_MSGQ_Init(&newQ,10);
    SYS_ASSERT( SYS_MSGQ_Success == initResult );
    SYS_MSGQ_Delete(&newQ);
  </code>

  Remarks:
    None.
*/

void SYS_MSGQ_Delete(SYS_MSG_QUEUE *pQ);


// *****************************************************************************
/* Function:
    bool SYS_MSGQ_IsFull(SYS_MSG_QUEUE *pQ)

  Summary:
    Returns true if message queue is full, false otherwise.

  Description:
    Returns true if message queue is full, false otherwise.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    true if queue is full, false otherwise.

  Example:
  <code>
    SYS_MSGQ_RESULT result;
    if ( !SYS_MSGQ_IsFull( &myQ )
    {
        result = SYS_MSGQ_Enqueue( &myQ, &myMessage );
        SYS_ASSERT( SYS_MSGQ_Success == result );
    }
    else
    {
        // Error: handle full queue
    }
  </code>

  Remarks:
    None.
*/

bool SYS_MSGQ_IsFull(SYS_MSG_QUEUE *pQ);


// *****************************************************************************
/* Function:
    bool SYS_MSGQ_IsEmpty(SYS_MSG_QUEUE *pQ)

  Summary:
    Returns true if message queue is empty, false otherwise.

  Description:
    Returns true if message queue is empty, false otherwise.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    true if queue is empty, false otherwise.

  Example:
  <code>
    while( !SYS_MSGQ_IsEmpty( &myQ ) ){} // Wait until queue is empty
    SYS_MSGQ_Delete( &myQ ); // Delete empty queue
  </code>

  Remarks:
    None.
*/

bool SYS_MSGQ_IsEmpty(SYS_MSG_QUEUE *pQ);


// *****************************************************************************
/* Function:
    SYS_MSGQ_RESULT SYS_MSGQ_Enqueue(SYS_MSG_QUEUE *pQ, SYS_MSGQ_ELEMENT *pMsg)

  Summary:
    Add (enqueue) new message to a message queue.

  Description:
    Add (enqueue) new message to a message queue.

  Precondition:
    Message queue must be initialized with SYS_MSGQ_Init.

  Parameters:
    pQ - pointer to message queue.
    pMsg - pointer to message

  Returns:
    SYS_MSGQ_Success ( if message added to queue ) or
    SYS_MSGQ_Failure ( if queue is full and message not added to queue )

  Example:
  <code>
    SYS_MSGQ_ELEMENT myMessage;
    SYS_MSGQ_RESULT  result;
    result = SYS_MSGQ_Enqueue( &myQ, &myMessage );
    SYS_ASSERT( SYS_MSGQ_Success == result );
  </code>

  Remarks:
    None.
*/

SYS_MSGQ_RESULT SYS_MSGQ_Enqueue(SYS_MSG_QUEUE *pQ, SYS_MSGQ_ELEMENT *pMsg);


// *****************************************************************************
/* Function:
    void SYS_MSGQ_Dequeue(SYS_MSG_QUEUE *pQ)

  Summary:
    Removes (dequeues) the oldest message from a message queue.

  Description:
    Removes (dequeues) the oldest message from a message queue.

  Precondition:
    Message queue must be initialized with SYS_MSGQ_Init.

  Parameters:
    pQ - pointer to message queue.

  Returns:
    None.

  Example:
  <code>
    SYS_MSGQ_ELEMENT message;
    while (!SYS_MSGQ_IsEmpty(&myQ)) {
        message = SYS_MSGQ_ReadQueue(&myQ);
        // Process message contents
        SYS_MSGQ_Dequeue(&myQ);
    }
  </code>

  Remarks:
    Message should be read using SYS_MSGQ_ReadQueue before removal from the queue.
*/

void SYS_MSGQ_Dequeue(SYS_MSG_QUEUE *pQ);


// *****************************************************************************
/* Function:
    SYS_MSGQ_ELEMENT SYS_MSGQ_ReadQueue(SYS_MSG_QUEUE *pQ )

  Summary:
    Reads oldest message on a queue.

  Description:
    Reads oldest message on a queue, but does not remove (dequeue) the message.

  Precondition:
    Message queue must be initialized with SYS_MSGQ_Init.

  Parameters:
    pQ - pointer to message queue.

  Returns:
    Oldest message on the queue.

  Example:
  <code>
    SYS_MSGQ_ELEMENT message;
    while (!SYS_MSGQ_IsEmpty(&myQ)) {
        message = SYS_MSGQ_ReadQueue(&myQ);
        // Process message contents
        SYS_MSGQ_Dequeue(&myQ);
    }
  </code>

  Remarks:
    None.
*/

SYS_MSGQ_ELEMENT SYS_MSGQ_ReadQueue(SYS_MSG_QUEUE *pQ );


// *****************************************************************************
/* Function:
    SYS_MSGQ_ELEMENT *SYS_MSGQ_GetNextQRef(SYS_MSG_QUEUE *pQ )

  Summary:
    Returns a pointer to oldest message on a queue.

  Description:
    Returns a pointer to oldest message on a queue.

  Precondition:
    Message queue must be initialized with SYS_MSGQ_Init.

  Parameters:
    pQ - pointer to message queue.

  Returns:
    Pointer to oldest message element on the queue.

  Example:
  <code>
    SYS_MSGQ_ELEMENT pMessage;
    while (!SYS_MSGQ_IsEmpty(&myQ)) {
        pMessage = SYS_MSGQ_GetNextQRef(&myQ);
        // Process message contents
        SYS_MSGQ_Dequeue(&myQ);
    }
  </code>

  Remarks:
    None.
*/

SYS_MSGQ_ELEMENT *SYS_MSGQ_GetNextQRef(SYS_MSG_QUEUE *pQ );


// *****************************************************************************
/* Function:
    SYS_MSGQ_ELEMENT *SYS_MSGQ_QRefGet(SYS_MSG_QUEUE *pQ, uint8_t iQElement)

  Summary:
    Returns pointer to the iQElement'th queue element in the queue defined by pQ.

  Description:
    Returns pointer to the iQElement'th queue element in the queue defined by pQ.

  Precondition:
    None.

  Parameters:
    pQ - pointer to message queue.
    iQElement - index into the message queue.
                iQElement = 0 identifies the oldest queue element.
                iQElement = 1 identifies the next oldest queue element.

  Returns:
    Pointer to queue element defined by iQElement index.

  Example:
  <code>
    // Find first message of interest to this mailbox.
    nMessagePriorities = hSysMsg->nMessagePriorities;
    for (iPriority=nMessagePriorities-1;iPriority>=0;iPriority--)
    {
        pQ = &(hSysMsg->msgQueues[iPriority]);
        iQElement = 0;
        while ( NULL != ( pQElement = SYS_MSGQ_QRefGet(pQ,iQElement) ) )
        {
            bitMap = pQElement->mailboxInterestBitMap[iBitMap];
            if ( (bitMap & iBit) > 0 )
            {// Found a message of interest
                bitMap &= ~iBit; // Clear this mailbox bit.
                pQElement->mailboxInterestBitMap[iBitMap] = bitMap; // Store it.
                break;
            }
            iQElement++; // Move on to next element in queue
        }
        if ( NULL != pQElement )
        {// Found a message to report out to mailbox
            break;
        }
    }
  </code>

  Remarks:
    None.
*/

SYS_MSGQ_ELEMENT *SYS_MSGQ_QRefGet(SYS_MSG_QUEUE *pQ, uint8_t iQElement);


#endif //#ifndef _SYS_MSG_LOCAL_H
