/*******************************************************************************
 Queue support functions

  Summary:
 Queues objects in a FIFO

  Description:
    -Provides queue mechanisms to process objects in a FIFO queue

 *******************************************************************************/

/*******************************************************************************
FileName:  queue.c
Copyright © 2012 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS?WITHOUT WARRANTY OF ANY KIND,
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

#include "queue.h"

/******************************************************************************
 * Function:        void init_queue(struct queue *q)
 *
 * PreCondition:
 *
 * Input:           queue pointer
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Initializes the queue
 *
 * Note:
 *
 *****************************************************************************/
void init_queue(struct queue *q)
{
    q->count = 0;
    q->front = 0;
    q->back = -1;
}

/******************************************************************************
 * Function:        uint32_t queue_isFull(struct queue *q)
 *
 * PreCondition:
 *
 * Input:           queue pointer
 *
 * Output:          1 - queue is full, 0 - queue is not full
 *
 * Side Effects:    None
 *
 * Overview:        Checks to see if queue is full
 *
 * Note:
 *
 *****************************************************************************/
uint32_t queue_isFull(struct queue *q)
{
    if(q->count == MY_QUEUE_SIZE)
        return 1;
    else
        return 0;
}

/******************************************************************************
 * Function:        void enqueue(struct queue *q, uint32_t messageType)
 *
 * PreCondition:    queue is initialized
 *
 * Input:           queue pointer, message type
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:
 *
 *****************************************************************************/
void enqueue(MY_QUEUE *q, MY_QUEUE_OBJECT queueObject)
{
    if(queue_isFull(q))
    {
        q->back = (q->back + 1) % MY_QUEUE_SIZE;
        q->message_queue[q->back] = queueObject;

        q->front = (q->front + 1) % MY_QUEUE_SIZE;
    }
    else
    {
        q->count++;
        q->back = (q->back + 1) % MY_QUEUE_SIZE;
        q->message_queue[q->back] = queueObject;
    }
}

/******************************************************************************
 * Function:        void dequeue(struct queue *q)
 *
 * PreCondition:
 *
 * Input:           queue pointer
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Removes item from front of queue
 *
 * Note:
 *
 *****************************************************************************/
void dequeue(MY_QUEUE *q)
{
    if(queue_isEmpty(q))
    {
        ; // Nothing to dequeue
    }
    else
    {
        q->count--;
        q->front = (q->front + 1) % MY_QUEUE_SIZE;
    }
}

/******************************************************************************
 * Function:        uint8_t ueue_isEmpty(struct queue *q)
 *
 * PreCondition:
 *
 * Input:           queue pointer
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Checks to see if queue is empty
 *
 * Note:
 *
 *****************************************************************************/
uint32_t queue_isEmpty(MY_QUEUE *q)
{
    if(q->count == 0)
        return 1;
    else
        return 0;
}