/*******************************************************************************
  System Messaging Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    sys_msg.c

  Summary:
    System Messaging Implementation

  Description:

    While building the driver from source, ALWAYS use this file in the build.
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


// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include <stdlib.h>
#include <string.h>
#include <sys/appio.h>

#include "system/common/sys_common.h"
#include "system/msg/sys_msg.h"
#include "system/msg/src/sys_msg_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables
// *****************************************************************************
// *****************************************************************************
static SYS_MSG_MESSAGING_OBJECT *handleSysMsg[SYS_MSG_NUM_INSTANCES];

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

void AssertToIDE(char *test, char *message, uint16_t line, char *file)
{
    DBPRINTF("%s :: SYS_ASSERT failed: %s, at line %d in %s\n",message,test,line,file);
    asm("break");
}


static SYS_MSG_MESSAGE_DEFN * _ID2hMsgType( SYS_MSG_MESSAGING_OBJECT *hSysMsg,
                                            uint8_t         nMessageTypeID   )
{
    uint16_t iMessageType;
    SYS_MSG_MESSAGE_DEFN * hMsgType = NULL;

    // Find hMsgType corresponding to nMessageTypeID
    for ( iMessageType = 0; iMessageType < SYS_MSG_MAX_TYPES; iMessageType++ )
    {
        if ( NULL != hSysMsg->pMessageType[iMessageType] )
        {// If slot is in use
            if ( hSysMsg->nMessageTypeIDs[iMessageType] == nMessageTypeID )
            {// Found matching message type, return handle to message definition
                hMsgType = hSysMsg->pMessageType[iMessageType];
                break;
            }
        }
    }

    return hMsgType;
}


static bool _GotMessages( SYS_MSG_MESSAGING_OBJECT *hSysMsg )
{
    int16_t iPriority;
    uint16_t nMessagePriorities;
    SYS_MSG_QUEUE *pQ;
    bool bGotMessages = false;

    SYS_ASSERT( NULL != hSysMsg, "Bad argument!" );     // Check argument

    // Find queue that is not empty
    nMessagePriorities = hSysMsg->nMessagePriorities;
    for (iPriority=nMessagePriorities-1;iPriority>=0;iPriority--)
    {
        pQ = &(hSysMsg->msgQueues[iPriority]);
        if (!SYS_MSGQ_IsEmpty(pQ) )
        {
            bGotMessages = true;
            break;
        }
    }

    return bGotMessages;
}


// *****************************************************************************
// *****************************************************************************
// Section: System Messaging Initialization
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
/* Function:
    SYS_OBJ_HANDLE SYS_MSG_Initialize ( const SYS_MSG_INSTANCE iSysMsg,
                                        const SYS_MODULE_INIT * const pInitSysMsg )
  Summary:
    Configures and initializes the messaging subsystem.
*/

    SYS_OBJ_HANDLE SYS_MSG_Initialize ( const SYS_MSG_INSTANCE iSysMsg,
                                        SYS_OBJ_HANDLE pInitializeSysMsg )
{
    uint16_t iPriority;
    SYS_MSGQ_RESULT initResult;
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;
    SYS_MSG_INIT * pInitSysMsg = (SYS_MSG_INIT *)pInitializeSysMsg;

    // Check arguments
    if ( iSysMsg >= SYS_MSG_NUM_INSTANCES )
    {// Bad instance index
        return SYS_OBJ_HANDLE_INVALID;
    }
    if ( NULL != handleSysMsg[iSysMsg] )
    {// Instance index already in use.
        return SYS_OBJ_HANDLE_INVALID;
    }

    hSysMsg = (SYS_MSG_MESSAGING_OBJECT*)malloc(sizeof(SYS_MSG_MESSAGING_OBJECT));
    if ( NULL == hSysMsg )
    {
        return SYS_OBJ_HANDLE_INVALID;
    }
    memset(hSysMsg,0x00,sizeof(SYS_MSG_MESSAGING_OBJECT));

    if ( NULL != pInitSysMsg )
    {// Pointer to init structure provided, use init structure values.
        // Initialize Sytem Message Object
        hSysMsg->nMaxMsgsDelivered  = pInitSysMsg->nMaxMsgsDelivered;
        hSysMsg->nMessagePriorities = pInitSysMsg->nMessagePriorities;

        // Initialize individual message queues, one for each priority
        for (iPriority=0;iPriority<pInitSysMsg->nMessagePriorities;iPriority++)
        {
            initResult = SYS_MSGQ_Init(hSysMsg->msgQueues + iPriority,pInitSysMsg->nQSizes[iPriority]);
            if (SYS_MSGQ_Success !=initResult )
            {
                return SYS_OBJ_HANDLE_INVALID;
            }
        }
    }
    else
    {// No pointer to init structure provided, use default values from config file.
        // Initialize Sytem Message Object
        hSysMsg->nMaxMsgsDelivered  = SYS_MSG_MAX_MSGS_DELIVERED;
        hSysMsg->nMessagePriorities = SYS_MSG_MAX_PRIORITY+1;

        // Initialize individual message queues, one for each priority
        uint16_t nQSizes[] = SYS_MSG_BUFFER_SIZES;
        for (iPriority=0;iPriority<(SYS_MSG_MAX_PRIORITY+1);iPriority++)
        {
            initResult = SYS_MSGQ_Init(hSysMsg->msgQueues + iPriority,nQSizes[iPriority]);
            if (SYS_MSGQ_Success !=initResult )
            {
                return SYS_OBJ_HANDLE_INVALID;
            }
        }
     }

  //NOTE: Using memset above means these initializations aren't necessary
  //hSysMsg->nMessageTypes = 0;
  //hSysMsg->nMailboxes    = 0;
  //Also: nMessageTypeIDs[], pMessageType[], and pMailbox[] arrays don't need initialization to zero

    handleSysMsg[iSysMsg] = hSysMsg;

    return (SYS_OBJ_HANDLE)hSysMsg;
}


//******************************************************************************
/* Function:
    void  SYS_MSG_Deinitialize ( SYS_OBJ_HANDLE hSysMsg )

  Summary:
    Deinitializes System Messaging Instance
*/
void  SYS_MSG_Deinitialize ( SYS_OBJ_HANDLE hSysMsgForDeinit )
{
    SYS_MSG_MESSAGING_OBJECT *hSysMsg = (SYS_MSG_MESSAGING_OBJECT *)hSysMsgForDeinit;
    uint16_t iSysMsg;

    for ( iSysMsg=0; iSysMsg < SYS_MSG_NUM_INSTANCES; iSysMsg++ )
    {
        if ( hSysMsg == handleSysMsg[iSysMsg] )
        {// Zero out pointer
            handleSysMsg[iSysMsg] = NULL;
            break;
        }
    }
    free( hSysMsg );
}


//******************************************************************************
/* Function:
    void  SYS_MSG_Tasks ( SYS_OBJ_HANDLE hSysMsg )

*/
void  SYS_MSG_Tasks ( SYS_OBJ_HANDLE handleSysMsg )
{
    SYS_MSG_MESSAGING_OBJECT *hSysMsg = (SYS_MSG_MESSAGING_OBJECT *)handleSysMsg;

    SYS_ASSERT( NULL != hSysMsg, "Bad handle argument!"); // Check argument

    if ( _GotMessages(hSysMsg) )
    {
        int16_t iPriority;
        int16_t nMsgsDelivered      = 0;
        uint16_t nMaxMsgsDelivered  = hSysMsg->nMaxMsgsDelivered;
        uint16_t nMessagePriorities = hSysMsg->nMessagePriorities;

        SYS_MSG_QUEUE *pQ;
        SYS_MSGQ_ELEMENT *pQElement;
        SYS_MSG_MESSAGE_DEFN * hMsgType;
        SYS_MSG_MAILBOX_OBJECT * pMailbox;

        uint16_t iBit, iBitMap, bitMap, iMailbox;
        do
        {
            // Find the first queue that is not empty
            pQElement = NULL;
            for (iPriority=nMessagePriorities-1;iPriority>=0;iPriority--)
            {
                pQ = &(hSysMsg->msgQueues[iPriority]);
                if (!SYS_MSGQ_IsEmpty(pQ) )
                {
                    pQElement = SYS_MSGQ_GetNextQRef(pQ);
                    break;
                }
            }

            if ( NULL != pQElement )
            {// Found message to deliver
                hMsgType = _ID2hMsgType(hSysMsg,pQElement->sMessage.nMessageTypeID);
                SYS_ASSERT( NULL != hMsgType, "Null message type handle!" );

                // Notify all interested mailboxes of this message
                for ( iBitMap = 0; iBitMap < (SYS_MSG_NUM_MAILBOX_BITMAPS); iBitMap++ )
                {
                    bitMap = pQElement->mailboxInterestBitMap[iBitMap];
                    for ( iBit = 0; iBit < 16; iBit++ )
                    {
                        if ( (bitMap & 1<<iBit) > 0 )
                        {
                            iMailbox = iBit + 16*iBitMap;
                            pMailbox = hSysMsg->pMailbox[iMailbox];
                            if ( NULL != pMailbox->msgCallBackFunction )
                            {
                                (*(pMailbox->msgCallBackFunction))( &(pQElement->sMessage) ); // Launch callback
                            }
                        }
                    }
                }
                nMsgsDelivered = (nMaxMsgsDelivered > 0) ? nMsgsDelivered+1 : -1;

                // Dequeue message now that all mailboxes have been notified
                iPriority = hMsgType->nMessagePriority;
                pQ = &(hSysMsg->msgQueues[iPriority]);
                SYS_MSGQ_Dequeue(pQ);
            }

        } while ( nMsgsDelivered < nMaxMsgsDelivered && NULL != pQElement );

    }//end if ( _GotMessages(hSysMsg) )
}


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
*/

SYS_OBJ_HANDLE SYS_MSG_TypeCreate( const SYS_MSG_INSTANCE iSysMsg,
                                   uint8_t  nMessageTypeID,
                                   uint8_t  nMessagePriority )
{
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;
    SYS_MSG_MESSAGE_DEFN * hMsgType;  // Return this value
    uint16_t iMessageType;
    uint8_t  nMessageTypes;

    // Check arguments
    if ( iSysMsg >= SYS_MSG_NUM_INSTANCES ||
         NULL == handleSysMsg[iSysMsg]      )
    {// Bad instance index
        return SYS_OBJ_HANDLE_INVALID;
    }
    hSysMsg = handleSysMsg[iSysMsg];
    if ( nMessagePriority >= hSysMsg->nMessagePriorities )
    {// Bad message priority
        return SYS_OBJ_HANDLE_INVALID;
    }


    // Don't duplicate message type IDs
    for ( iMessageType = 0; iMessageType < SYS_MSG_MAX_TYPES; iMessageType++ )
    {
        if ( hSysMsg->pMessageType[iMessageType] != NULL &&
             hSysMsg->nMessageTypeIDs[iMessageType] == nMessageTypeID )
        {// Trying to reuse message type ID
            return SYS_OBJ_HANDLE_INVALID;
        }
    }

    // Allocate memory for this new message type
    hMsgType = (SYS_MSG_MESSAGE_DEFN *)calloc(1, sizeof(SYS_MSG_MESSAGE_DEFN) );
    if ( NULL == hMsgType )
    {// Can't allocate memory for message type definition
        return SYS_OBJ_HANDLE_INVALID;
    }

    //Set iMessageType to first open slot in pMessageType array
    //TODO: MUTEX on pMessageType[] array
    for ( iMessageType = 0; iMessageType < SYS_MSG_MAX_TYPES; iMessageType++ )
    {
        if ( NULL == hSysMsg->pMessageType[iMessageType] )
        {
            // Use this slot in the array
            hSysMsg->pMessageType[iMessageType] = hMsgType;
            break;
        }
    }
    //TODO: Release MUTEX on pMessageType[] array

    if ( iMessageType >= SYS_MSG_MAX_TYPES )
    {// Couldn't find open slot in pMessageType[] array
        free( hMsgType );
        return SYS_OBJ_HANDLE_INVALID;
    }

    //TODO: MUTEX on hSysMsg->nMessageTypes
    nMessageTypes = hSysMsg->nMessageTypes + 1;
    if ( nMessageTypes > SYS_MSG_MAX_TYPES )
    {// Have exceeded maximum allowable message types
        free( hMsgType );
        return SYS_OBJ_HANDLE_INVALID;
    }
    hSysMsg->nMessageTypes = nMessageTypes;
    //TODO: Release MUTEX on hSysMsg->nMessageTypes

    // Remember this message type in the System Messaging object
    hSysMsg->nMessageTypeIDs[iMessageType] = nMessageTypeID;

    // Record message information
    hMsgType->hSysMsg          = hSysMsg;
    hMsgType->iMessageType     = iMessageType;
    hMsgType->nMessageTypeID   = nMessageTypeID;
    hMsgType->nMessagePriority = nMessagePriority;

    return (SYS_OBJ_HANDLE)hMsgType;
}

// *****************************************************************************
/* Function:
    void SYS_MSG_TypeRemove( SYS_OBJ_HANDLE hMsgType )

  Summary:
    Removes an existing message type.
*/

void SYS_MSG_TypeRemove( SYS_OBJ_HANDLE handleMsgType )
{
    SYS_MSG_MESSAGE_DEFN * hMsgType = (SYS_MSG_MESSAGE_DEFN *)handleMsgType;
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;
    uint16_t iMessageType;

    // Check paramaters
    SYS_ASSERT( NULL != hMsgType, "Null hMsgType!" );

    hSysMsg = (SYS_MSG_MESSAGING_OBJECT *)hMsgType->hSysMsg;
    SYS_ASSERT( NULL != hSysMsg, "Null hSysMsg!" );

    //TODO: MUTEX on hSysMsg->nMessageTypes
    hSysMsg->nMessageTypes -= 1;
    //TODO: Disable MUTEX on hSysMsg->nMessageTypes

    iMessageType = hMsgType->iMessageType;
    hSysMsg->pMessageType[iMessageType]    = NULL;
    hSysMsg->nMessageTypeIDs[iMessageType] = 0;

    free(hMsgType);
}


// *****************************************************************************
/* Function:
    SYS_OBJ_HANDLE SYS_MSG_ID2hMsgType( const SYS_MSG_INSTANCE iSysMsg,
                                        uint8_t        nMessageTypeID )
  Summary:
    Translates message type identifier into handle of corresponding message type object.
*/

SYS_OBJ_HANDLE SYS_MSG_ID2hMsgType( const SYS_MSG_INSTANCE iSysMsg,
                                    uint8_t         nMessageTypeID)
{
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;

    // Check arguments
    if ( iSysMsg >= SYS_MSG_NUM_INSTANCES ||
         NULL == handleSysMsg[iSysMsg]      )
    {// Bad instance index
        return (SYS_OBJ_HANDLE)NULL;
    }
    else
    {
        uint16_t iMessageType;
        SYS_MSG_MESSAGE_DEFN * hMsgType = NULL;

        hSysMsg = handleSysMsg[iSysMsg];

        // Find hMsgType corresponding to nMessageTypeID
        for ( iMessageType = 0; iMessageType < SYS_MSG_MAX_TYPES; iMessageType++ )
        {
            if ( NULL != hSysMsg->pMessageType[iMessageType] )
            {// If slot is in use
                if ( hSysMsg->nMessageTypeIDs[iMessageType] == nMessageTypeID )
                {// Found matching message type, return handle to message definition
                    hMsgType = hSysMsg->pMessageType[iMessageType];
                    break;
                }
            }
        }

        return (SYS_OBJ_HANDLE)hMsgType;
    }
}


// *****************************************************************************
/* Function:
    bool SYS_MSG_GotMessages( const SYS_MSG_INSTANCE iSysMsg )

  Summary:
    Returns true if system messaging has undelivered messages, false otherwise.
*/
bool SYS_MSG_GotMessages( const SYS_MSG_INSTANCE iSysMsg )
{
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;
    int16_t iPriority;
    uint16_t nMessagePriorities;
    SYS_MSG_QUEUE *pQ;
    bool bGotMessages = false;

    // Check argument
    SYS_ASSERT( iSysMsg < SYS_MSG_NUM_INSTANCES && NULL != handleSysMsg[iSysMsg], "Bad arguments!" );

    // Find queue that is not empty
    hSysMsg = handleSysMsg[iSysMsg];
    nMessagePriorities = hSysMsg->nMessagePriorities;
    for (iPriority=nMessagePriorities-1;iPriority>=0;iPriority--)
    {
        pQ = &(hSysMsg->msgQueues[iPriority]);
        if (!SYS_MSGQ_IsEmpty(pQ) )
        {
            bGotMessages = true;
            break;
        }
    }
    return bGotMessages;
}


// *****************************************************************************
/* Function:
    SYS_MSG_RESULTS SYS_MSG_MessageSend( const SYS_MSG_INSTANCE iSysMsg,
                                         SYS_MSG_OBJECT *pMessage      )
  Summary:
    Sends a message, as defined by a message structure.
*/

SYS_MSG_RESULTS SYS_MSG_MessageSend( const SYS_MSG_INSTANCE iSysMsg,
                                     SYS_MSG_OBJECT *pMessage      )
{
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;
    SYS_MSG_MESSAGE_DEFN *hMsgType;
    uint8_t  nMessagePriority;
    SYS_MSGQ_ELEMENT qElement;
    SYS_MSGQ_RESULT result;

    // Check arguments
    SYS_ASSERT( iSysMsg < SYS_MSG_NUM_INSTANCES && NULL != handleSysMsg[iSysMsg], "Bad arguments!");
    SYS_ASSERT( NULL != pMessage, "Null message pointer!" );

    hMsgType = (SYS_MSG_MESSAGE_DEFN *)SYS_MSG_ID2hMsgType(iSysMsg,pMessage->nMessageTypeID);
    if ( NULL == hMsgType )
    {
        return SYS_MSG_BAD_MSGTYPE;
    }

    hSysMsg = handleSysMsg[iSysMsg];
    nMessagePriority = hMsgType->nMessagePriority;
    if ( nMessagePriority >= hSysMsg->nMessagePriorities )
    {
        return SYS_MSG_BAD_PRIORITY;
    }

    // Load message into queue element
    qElement.sMessage = *pMessage;
    memcpy(qElement.mailboxInterestBitMap,
           hMsgType->mailboxInterestBitMap,
           SYS_MSG_NUM_MAILBOX_BITMAPS*sizeof(uint16_t));

    // Enqueue the message
    //TODO: MUTEX on message queues
    result = SYS_MSGQ_Enqueue( &(hSysMsg->msgQueues[nMessagePriority]), &qElement);
    //TODO: Disable MUTEX on message queues

    return result;
}


// *****************************************************************************
/* Function:
    SYS_MSGQ_ELEMENT *SYS_MSG_MessageReceive( const SYS_MSG_INSTANCE iSysMsg )

  Summary:
    Receive next message in message queues.
*/

SYS_MSGQ_ELEMENT *SYS_MSG_MessageReceive( const SYS_MSG_INSTANCE iSysMsg )
{
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;
    int16_t iPriority;
    uint16_t nMessagePriorities;
    SYS_MSG_QUEUE *pQ;
    SYS_MSGQ_ELEMENT *pElem = NULL;

    // Check argument
    SYS_ASSERT( iSysMsg < SYS_MSG_NUM_INSTANCES && NULL != handleSysMsg[iSysMsg], "Bad arguments!");

    // Find queue that is not empty
    hSysMsg = handleSysMsg[iSysMsg];
    nMessagePriorities = hSysMsg->nMessagePriorities;
    for (iPriority=nMessagePriorities-1;iPriority>=0;iPriority--)
    {
        pQ = &(hSysMsg->msgQueues[iPriority]);
        if (!SYS_MSGQ_IsEmpty(pQ) )
        {
            pElem = SYS_MSGQ_GetNextQRef(pQ);
            break;
        }
    }

    return pElem;
}


// *****************************************************************************
/* Function:
    void SYS_MSG_MessageDeliver( const SYS_MSG_INSTANCE iSysMsg, SYS_MSGQ_ELEMENT *pQElement );

  Summary:
    Deliver message to mailboxes.
*/

void SYS_MSG_MessageDeliver( const SYS_MSG_INSTANCE iSysMsg, SYS_MSGQ_ELEMENT *pQElement )
{
    SYS_MSG_MESSAGING_OBJECT * hSysMsg;
    SYS_MSG_MESSAGE_DEFN     * hMsgType;
    SYS_MSG_MAILBOX_OBJECT   * pMailbox;
    SYS_MSG_QUEUE            * pQ;

    uint16_t iPriority;
    uint16_t iMailbox, iBitMap, iBit, bitMap;

    // Check arguments
    SYS_ASSERT( iSysMsg < SYS_MSG_NUM_INSTANCES && NULL != handleSysMsg[iSysMsg], "Bad arguments!");
    SYS_ASSERT( NULL != pQElement, "Null pQElement!" );

    hSysMsg = handleSysMsg[iSysMsg];

    hMsgType = (SYS_MSG_MESSAGE_DEFN *)SYS_MSG_ID2hMsgType(iSysMsg,pQElement->sMessage.nMessageTypeID);
    SYS_ASSERT( NULL != hMsgType, "Null message type handle!" );

    // Notify all interested mailboxes of this message
    for ( iBitMap = 0; iBitMap < (SYS_MSG_NUM_MAILBOX_BITMAPS); iBitMap++ )
    {
        bitMap = pQElement->mailboxInterestBitMap[iBitMap];
        for ( iBit = 0; iBit < 16; iBit++ )
        {
            if ( (bitMap & 1<<iBit) > 0 )
            {
                iMailbox = iBit + 16*iBitMap;
                pMailbox = hSysMsg->pMailbox[iMailbox];
                if ( NULL != pMailbox->msgCallBackFunction )
                {
                    (*(pMailbox->msgCallBackFunction))( &(pQElement->sMessage) ); // Launch callback
                }
            }
        }
    }

    // Dequeue message now that all mailboxes have been notified
    iPriority = hMsgType->nMessagePriority;
    pQ = &(hSysMsg->msgQueues[iPriority]);
    SYS_MSGQ_Dequeue(pQ);
}


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
*/

SYS_OBJ_HANDLE SYS_MSG_MailboxOpen( const SYS_MSG_INSTANCE iSysMsg,
                                    SYS_MSG_RECEIVE_CALLBACK msgCallBackFunction )
{
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;
    SYS_MSG_MAILBOX_OBJECT *hMailbox;
    uint16_t  iMailbox, nMailboxes;

    // Check arguments
    SYS_ASSERT( iSysMsg < SYS_MSG_NUM_INSTANCES && NULL != handleSysMsg[iSysMsg], "Bad arguments!");
    hSysMsg = handleSysMsg[iSysMsg];

    // Allocate memory for this new message type
    hMailbox = (SYS_MSG_MAILBOX_OBJECT *)calloc(1, sizeof(SYS_MSG_MAILBOX_OBJECT) );
    if ( NULL == hMailbox )
    {// Can't allocate memory for message type definition
        return SYS_OBJ_HANDLE_INVALID;
    }

    // Set iMailbox to first unused slot in pMailbox[] array
    //TODO: MUTEX on pMailbox[] array
    for ( iMailbox = 0; iMailbox < SYS_MSG_MAX_MAILBOXES; iMailbox++ )
    {
        if ( NULL == hSysMsg->pMailbox[iMailbox] )
        {
            hSysMsg->pMailbox[iMailbox] = hMailbox;
            break;
        }
    }
    //TODO: Release MUTEX on pMailbox[] array

    if ( iMailbox >= SYS_MSG_MAX_MAILBOXES )
    {// Couldn't find open slot in pMailbox[] array
        free( hMailbox );
        return SYS_OBJ_HANDLE_INVALID;
    }

    // Have one more mailbox
    //TODO: MUTEX on hSysMsg->nMailboxes
    nMailboxes = hSysMsg->nMailboxes + 1;
    if ( nMailboxes > SYS_MSG_MAX_MAILBOXES )
    {// Have exceeded maximum allowable number of mailboxes
        free( hMailbox );
        //TODO: Release MUTEX on hSysMsg->nMailboxes
        return SYS_OBJ_HANDLE_INVALID;
    }
    else
    {
        hSysMsg->nMailboxes = nMailboxes;
        //TODO: Release MUTEX on hSysMsg->nMailboxes
    }

    // Initialize mailbox.
    hMailbox->hSysMsg  = handleSysMsg[iSysMsg];
    hMailbox->iMailbox = iMailbox;
    hMailbox->msgCallBackFunction = msgCallBackFunction;

    return (SYS_OBJ_HANDLE)hMailbox;
}


// *****************************************************************************
/* Function:
    void SYS_MSG_MailboxClose( SYS_OBJ_HANDLE hMailbox )

  Summary:
    Closes (destroys) a mailbox previously opened with SYS_MSG_MailboxOpen.
*/

void SYS_MSG_MailboxClose( SYS_OBJ_HANDLE handleMailbox )
{
    SYS_MSG_MAILBOX_OBJECT *hMailbox = (SYS_MSG_MAILBOX_OBJECT *)handleMailbox;
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;
    SYS_MSG_INSTANCE iSysMsg;
    uint8_t  nMsgTypeID;
    uint16_t index, iMailbox, iBitMap, iBit, bitMap;

    // Check argument.
    SYS_ASSERT( NULL != hMailbox, "NULL mailbox handle!" );

    hSysMsg = hMailbox->hSysMsg;
    SYS_ASSERT( NULL != hSysMsg, "Null hSysMsg!" );
    for ( iSysMsg=0; iSysMsg < SYS_MSG_NUM_INSTANCES; iSysMsg++ )
    {
        if ( hSysMsg == handleSysMsg[iSysMsg] )
        {// Zero out pointer
            break;
        }
    }
    SYS_ASSERT( iSysMsg < SYS_MSG_NUM_INSTANCES, "iSysMsg undefined!" );

    // remove messages to avoid orphaned message deliveries
    for ( iBitMap = 0; iBitMap < (SYS_MSG_NUM_TYPES_BITMAPS); iBitMap++ )
    {
        bitMap = hMailbox->msgInterestBitMap[iBitMap];
        for ( iBit = 0; iBit < 16; iBit++ )
        {
            if ( (bitMap & 1<<iBit) > 0 )
            {
                index = iBit + 16*iBitMap;
                nMsgTypeID = hSysMsg->nMessageTypeIDs[index];
                SYS_MSG_MailboxMsgRemove(handleMailbox,
                                         SYS_MSG_ID2hMsgType(iSysMsg,nMsgTypeID) );
            }
        }
    }

    // remove mailbox from data structures
    hSysMsg->nMailboxes -= 1;
    iMailbox = hMailbox->iMailbox;
    hSysMsg->pMailbox[iMailbox] = NULL;

    free(hMailbox);
}


// *****************************************************************************
/* Function:
    void SYS_MSG_MailboxReinit( SYS_OBJ_HANDLE hMailbox,
                                SYS_MSG_RECEIVE_CALLBACK msgCallBackFunction );

  Summary:
    Reinitialize previously opened mailbox.
*/

void SYS_MSG_MailboxReinit( SYS_OBJ_HANDLE handleMailbox,
                            SYS_MSG_RECEIVE_CALLBACK msgCallBackFunction )
{
    SYS_MSG_MAILBOX_OBJECT *hMailbox = (SYS_MSG_MAILBOX_OBJECT *)handleMailbox;
    uint16_t iMsgInterest;

    // Check argument.
    SYS_ASSERT( NULL != hMailbox, "NULL mailbox handle!" );

    // Remember new call back function.
    hMailbox->msgCallBackFunction = msgCallBackFunction;

    // Clear message interest bit map
    for ( iMsgInterest = 0; iMsgInterest < (SYS_MSG_MAX_TYPES/16); iMsgInterest++ )
    {
        hMailbox->msgInterestBitMap[iMsgInterest] = 0;
    }
}


// *****************************************************************************
/* Function:
    void SYS_MSG_MailboxMsgAdd( SYS_OBJ_HANDLE hMailbox, SYS_OBJ_HANDLE hMsgType )

  Summary:
    Adds a message type to the list of messages received by a mailbox.
*/

void SYS_MSG_MailboxMsgAdd( SYS_OBJ_HANDLE handleMailbox, SYS_OBJ_HANDLE handleMsgType )
{
    SYS_MSG_MAILBOX_OBJECT *hMailbox = (SYS_MSG_MAILBOX_OBJECT *)handleMailbox;
    SYS_MSG_MESSAGE_DEFN   *hMsgType = (SYS_MSG_MESSAGE_DEFN   *)handleMsgType;
    uint16_t iMailbox, iMessageType;

    // Check arguments
    SYS_ASSERT( NULL != hMailbox, "Null mailbox handle!" );
    SYS_ASSERT( NULL != hMsgType, "Null message type handle!" );

    // Get object indexes
    iMailbox     = hMailbox->iMailbox;
    iMessageType = hMsgType->iMessageType;

    // Set bit maps.
    hMsgType->mailboxInterestBitMap[iMailbox>>4] |= 1 << (iMailbox % (1<<4));
    hMailbox->msgInterestBitMap[iMessageType>>4] |= 1 << (iMessageType % (1<<4));
}


// *****************************************************************************
/* Function:
    void SYS_MSG_MailboxMsgRemove( SYS_OBJ_HANDLE hMailbox, SYS_OBJ_HANDLE hMsgType )

  Summary:
    Removes a message type from the list of messages received by a mailbox.
*/

void SYS_MSG_MailboxMsgRemove( SYS_OBJ_HANDLE handleMailbox, SYS_OBJ_HANDLE handleMsgType )
{
    SYS_MSG_MAILBOX_OBJECT *hMailbox = (SYS_MSG_MAILBOX_OBJECT *)handleMailbox;
    SYS_MSG_MESSAGE_DEFN   *hMsgType = (SYS_MSG_MESSAGE_DEFN   *)handleMsgType;
    uint16_t iMailbox, iMessageType;

    // Check arguments
    SYS_ASSERT( NULL != hMailbox, "Null mailbox handle!" );
    SYS_ASSERT( NULL != hMsgType, "Null message type handle!" );

    // Get object indexes
    iMailbox     = hMailbox->iMailbox;
    iMessageType = hMsgType->iMessageType;

    // Clear bit maps.
    hMsgType->mailboxInterestBitMap[iMailbox>>4] &= ~( 1 << (iMailbox % (1<<4))     );
    hMailbox->msgInterestBitMap[iMessageType>>4] &= ~( 1 << (iMessageType % (1<<4)) );
}


// *****************************************************************************
/* Function:
    SYS_MSG_OBJECT *SYS_MSG_MailboxMessagesGet( SYS_OBJ_HANDLE hMailbox )

  Summary:
    Get queued messages for a mailbox.
*/

SYS_MSG_OBJECT *SYS_MSG_MailboxMessagesGet( SYS_OBJ_HANDLE handleMailbox )
{
    SYS_MSG_MAILBOX_OBJECT *hMailbox = (SYS_MSG_MAILBOX_OBJECT *)handleMailbox;
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;
    uint16_t iMailbox;
    SYS_MSGQ_ELEMENT *pQElement = NULL;
    uint16_t iQElement;
    uint16_t iBitMap, iBit, bitMap;
    SYS_MSG_MAILBOX_OBJECT  * pMailbox = NULL;

    int16_t iPriority;
    uint16_t nMessagePriorities;
    SYS_MSG_QUEUE *pQ;

    // Check argument
    SYS_ASSERT( NULL != hMailbox, "Null mailbox handle!" );

    // Determine iMailbox index for this mailbox
    hSysMsg = hMailbox->hSysMsg;
    for ( iMailbox=0; iMailbox < hSysMsg->nMessageTypes; iMailbox++ )
    {
        if ( hSysMsg->pMailbox[iMailbox] == hMailbox )
        {
            pMailbox = hSysMsg->pMailbox[iMailbox];
            break;
        }
    }
    if ( NULL != pMailbox )
    {
        SYS_ASSERT( NULL != pMailbox, "Mailbox handle not found!" );
    }
    iBitMap = iMailbox/16; iBit = iMailbox % 16;

    // Find first message of interest to this mailbox.
    nMessagePriorities = hSysMsg->nMessagePriorities;
    for (iPriority=nMessagePriorities-1;iPriority>=0;iPriority--)
    {
        pQ = &(hSysMsg->msgQueues[iPriority]);
        iQElement = 0;
        while ( NULL != ( pQElement = SYS_MSGQ_QRefGet(pQ,iQElement) ) )
        {
            bitMap = pQElement->mailboxInterestBitMap[iBitMap];
            if ( (bitMap & 1<<iBit) > 0 )
            {// Found a message of interest
                bitMap &= ~(1<<iBit); // Clear this mailbox bit.
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
    if (  NULL != pQElement )
    {
        return &(pQElement->sMessage);
    }
    else
    {
        return (SYS_MSG_OBJECT *)NULL;
    }
}


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
*/

SYS_MSG_QUEUE_STATUS SYS_MSG_QueueStatus( const SYS_MSG_INSTANCE iSysMsg, uint8_t nMessagePriority )
{
    int nCount,nQSize;
    SYS_MSG_MESSAGING_OBJECT *hSysMsg;

    // Check arguments
    if ( iSysMsg < SYS_MSG_NUM_INSTANCES &&
         NULL != handleSysMsg[iSysMsg]   &&
         nMessagePriority <= SYS_MSG_MAX_PRIORITY )
    {
        hSysMsg = handleSysMsg[iSysMsg];
        nCount = (hSysMsg->msgQueues)[nMessagePriority].nCount;
        nQSize = (hSysMsg->msgQueues)[nMessagePriority].nQSize;
        if ( nCount == nQSize )
        {
            return SYS_MSG_QUEUE_FULL;
        }
        else
        {
            return nCount;
        }
    }
    else
    {
        return SYS_MSG_QUEUE_BAD;
    }
}


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

SYS_MSGQ_RESULT SYS_MSGQ_Init(SYS_MSG_QUEUE *pQ, int nQSize)
{
    SYS_ASSERT( nQSize > 0, "Bad queue size!" );

    pQ->nQSize = nQSize;
    pQ->iStart = 0;
    pQ->iEnd   = 0;
    pQ->nCount = 0;
    pQ->qElements = (SYS_MSGQ_ELEMENT *)malloc(sizeof(SYS_MSGQ_ELEMENT)*(pQ->nQSize));
    if ( pQ->qElements == (SYS_MSGQ_ELEMENT *)NULL ) // memory not allocated
    {
        return SYS_MSGQ_Failure;
    }
    memset(pQ->qElements,0x00,sizeof(SYS_MSGQ_ELEMENT)*(pQ->nQSize));

    return SYS_MSGQ_Success;
}


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

void SYS_MSGQ_Delete(SYS_MSG_QUEUE *pQ)
{
    pQ->nQSize = 0;
    pQ->iStart = 0;
    pQ->iEnd   = 0;
    pQ->nCount = 0;
    free(pQ->qElements); // Deallocate queue elements
    pQ->qElements = (SYS_MSGQ_ELEMENT *)NULL;
}


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
  </code>

  Remarks:
    None.
*/

bool SYS_MSGQ_IsFull(SYS_MSG_QUEUE *pQ)
{
    SYS_ASSERT( NULL != pQ, "Null queue pointer!" );
    return (bool)(pQ->nCount == pQ->nQSize);
}


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
  </code>

  Remarks:
    None.
*/
bool SYS_MSGQ_IsEmpty(SYS_MSG_QUEUE *pQ)
{
    SYS_ASSERT( NULL != pQ, "Null queue pointer!" );
    return (bool)(pQ->nCount == 0);
}


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
  </code>

  Remarks:
    None.
*/

SYS_MSGQ_RESULT SYS_MSGQ_Enqueue(SYS_MSG_QUEUE *pQ, SYS_MSGQ_ELEMENT *pMsg)
{
    int iEnd;

    SYS_ASSERT( NULL != pQ,   "Null queue pointer!"   );
    SYS_ASSERT( NULL != pMsg, "Null element pointer!" );

    if (pQ->nCount == pQ->nQSize) //return error, queue is full
    {
        return SYS_MSGQ_Failure;
    }
    else // Queue is not full
    {
        iEnd = (pQ->iStart + pQ->nCount) % pQ->nQSize;
        pQ->qElements[iEnd] = *pMsg;
        pQ->nCount += 1;
        return SYS_MSGQ_Success;
    }
}


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
  </code>

  Remarks:
    Message should be read using SYS_MSGQ_ReadQueue before removal from the queue.
*/

void SYS_MSGQ_Dequeue(SYS_MSG_QUEUE *pQ)
{
    SYS_ASSERT( NULL != pQ, "Null queue pointer!" );
    if ( pQ->nCount > 0 )
    {
        pQ->iStart = (pQ->iStart + 1) % (pQ->nQSize);
        pQ->nCount -= 1;
    }
}


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
  </code>

  Remarks:
    None.
*/

SYS_MSGQ_ELEMENT SYS_MSGQ_ReadQueue(SYS_MSG_QUEUE *pQ )
{
    SYS_ASSERT( NULL != pQ, "Null queue pointer!" );
    return ( pQ->qElements[pQ->iStart] );
}


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
  </code>

  Remarks:
    None.
*/

SYS_MSGQ_ELEMENT *SYS_MSGQ_GetNextQRef(SYS_MSG_QUEUE *pQ )
{
    SYS_ASSERT( NULL != pQ, "Null queue pointer!" );
    return ( pQ->qElements + pQ->iStart );
}


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
                iQElement = 0 identifies oldest queue element.

  Returns:
    Pointer to queue element defined by iQElement index.

  Example:
  <code>
  </code>

  Remarks:
    None.
*/

SYS_MSGQ_ELEMENT *SYS_MSGQ_QRefGet(SYS_MSG_QUEUE *pQ, uint8_t iQElement)
{
    uint8_t iQIndex;

    SYS_ASSERT( NULL != pQ, "Null queue pointer!" );

    if ( iQElement < pQ->nCount )
    {// Return pointer
        iQIndex = (pQ->iStart + iQElement) % (pQ->nQSize);
        return ( pQ->qElements + iQIndex );
    }
    else
    {// Return NULL
        return NULL;
    }
}
