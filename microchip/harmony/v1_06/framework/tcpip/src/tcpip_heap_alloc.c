/*******************************************************************************
  TCPIP Heap Allocation Manager

  Summary:
    
  Description:
*******************************************************************************/

/*******************************************************************************
File Name:  tcpip_heap_alloc.c
Copyright © 2012 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
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

#include <string.h>
#include <stdlib.h>

#include <sys/kmem.h>

#include "tcpip/src/tcpip_private.h"
// definitions


// min heap alignment
// always power of 2
#if defined(__PIC32MX__)
typedef uint32_t _heap_Align;
#elif defined(__PIC32MZ__)
typedef struct __attribute__((aligned(16)))
{
    uint64_t     pad[2];
}_heap_Align;
#endif  // defined(__PIC32MX__) || defined(__PIC32MZ__)


typedef union __attribute__((aligned(16))) _tag_headNode
{
    _heap_Align x;
    struct
    {
        union _tag_headNode*	next;
        size_t			        units;
    };
}_headNode;


typedef enum
{
    // user/creation flags
    _TCPIP_HEAP_FLAG_NONE   = 0x0000,


    // internal heap flags
    _TCPIP_HEAP_FLAG_IS_K0  = 0x1000,     // heap is created in K0 space, else K1

}_TCPIP_HEAP_FLAGS;


#define	_TCPIP_HEAP_MIN_BLK_USIZE_  	2	// avoid tiny blocks having units <=  value. 

#define	_TCPIP_HEAP_MIN_BLKS_           64	// efficiency reasons, the minimum heap size that can be handled. 


#if defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) && defined(TCPIP_STACK_DRAM_TRACE_ENABLE) 
    #define _TCPIP_STACK_DRAM_TRACE_ENABLE
    #define _TCPIP_STACK_DRAM_TRACE_NDP_COLLAPSE    1       // collapse NDP info into IPv6 since NDP allocated packets are freed by IPv6
#else
    #undef  _TCPIP_STACK_DRAM_TRACE_ENABLE
#endif

#if defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) && defined(TCPIP_STACK_DRAM_DIST_ENABLE) 
    #define _TCPIP_STACK_DRAM_DIST_ENABLE

    // distribution block sizes that are monitored
    // Note: there's no way (yet) of allowing the user to specify the requested distribution
    static const int _tcpip_heap_dist_sizes[] = 
    {
        0,      32,     64,     128,    256,    512,    640,    768,
        896,    1024,   1152,   1280,   1408,   1536,   1792,   2000,
        10000
    };

#else
    #undef  _TCPIP_STACK_DRAM_DIST_ENABLE
#endif

typedef struct
{
    _headNode*      _heapHead;
    _headNode*      _heapTail;                  // head and tail pointers
    size_t          _heapUnits;                 // size of the heap, units
    size_t          _heapAllocatedUnits;        // how many units allocated out there
    TCPIP_HEAP_RES  _lastHeapErr;               // last error encountered
    _TCPIP_HEAP_FLAGS   _heapFlags;                 // heap flags
#if defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) 
    TCPIP_HEAP_NO_MEM_HANDLER _noMemHandler;    // out of memory handler
    _headNode*   _heapStart;                    // debug checkpoint     
    _headNode*   _heapEnd;                      // debug checkpoint     
#endif
#if defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)
    TCPIP_HEAP_TRACE_ENTRY _heapTraceTbl[TCPIP_STACK_DRAM_TRACE_SLOTS];
#endif
#if defined(_TCPIP_STACK_DRAM_DIST_ENABLE)
    TCPIP_HEAP_DIST_ENTRY _tcpip_heap_dist_array[sizeof(_tcpip_heap_dist_sizes)/sizeof(*_tcpip_heap_dist_sizes) - 1];
#endif
    // alignment padding
    // _headNode _heap[0];           // the heap itself, dynamically allocated

    OSAL_RESULT _heapSemaphoreInit;
    OSAL_SEM_HANDLE_TYPE _heapSemaphore;

}HEAP_DCPT; // descriptor of a heap


// local data
//


#if defined(TCPIP_STACK_DRAM_DEBUG_ENABLE)
#define _heapFailMessage "Heap allocation of %d bytes failed in module %d line %d\r\n"
#endif  // defined(TCPIP_STACK_DRAM_DEBUG_ENABLE)

// local prototypes
//
#if defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)
    static TCPIP_HEAP_TRACE_ENTRY* TCPIP_HEAP_FindEntry(HEAP_DCPT* hDcpt, int moduleId, bool addNewSlot);
    static size_t TCPIP_HEAP_AddToEntry(HEAP_DCPT* hDcpt, int moduleId, size_t nBytes, void* ptr);
    static void TCPIP_HEAP_RemoveFromEntry(HEAP_DCPT* hDcpt, int moduleId, size_t nUnits);
#endif  // defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)

#if defined(_TCPIP_STACK_DRAM_DIST_ENABLE)
static int TCPIP_HEAP_DistCompare(const void *a, const void *b);
static void TCPIP_HEAP_DistAdd(HEAP_DCPT* hDcpt, int moduleId, size_t nBytes);
static void TCPIP_HEAP_DistRem(HEAP_DCPT* hDcpt, int moduleId, size_t nBytes);
#endif  // defined(_TCPIP_STACK_DRAM_DIST_ENABLE)

#if defined(__PIC32MZ__)
// flushes a data cache line/address
extern __inline__ void __attribute__((always_inline)) _TCPIP_HEAP_DataLineFlush(void* address)
{   // issue a hit-writeback invalidate order
    __asm__ __volatile__ ("cache 0x15, 0(%0)" ::"r"(address));
}


// invalidates a range of addresses from the cache
void _TCPIP_HEAP_DataCacheInvalidate(void* address, size_t nBytes)
{
    int ix;

    uint8_t*    pLine = (uint8_t*)((uint32_t)address & 0xfffffff0); // start on an even cache line
    nBytes += (uint8_t*)address - pLine;
    int nLines = (nBytes + 15)/16;   // round up the number of taken cache lines 

    for(ix = 0; ix < nLines; ix ++)
    {
        _TCPIP_HEAP_DataLineFlush(pLine);
        pLine += 16;
    }

    __asm__ __volatile__ ("sync");
}
#endif  // defined(__PIC32MZ__)

TCPIP_HEAP_HANDLE TCPIP_HEAP_Create(uint8_t* heapBuffer, size_t buffSize, TCPIP_HEAP_FLAGS flags, TCPIP_HEAP_RES* pRes)
{
    HEAP_DCPT*      hDcpt;
    size_t          heapSize, heapUnits, headerSize;
    uintptr_t       alignBuffer;
    uint8_t*        heapStart;
    TCPIP_HEAP_RES  res;
    

    while(true)
    {
        hDcpt =0;

        if( heapBuffer == 0 )
        {
            res = TCPIP_HEAP_RES_BUFF_ERR;
            break;
        }

        // align properly: round up and truncate
        alignBuffer = ((uintptr_t)heapBuffer + sizeof(_heap_Align)-1 ) & ~(sizeof(_heap_Align)-1);
        buffSize -= (uint8_t*)alignBuffer - heapBuffer ;
        buffSize &= ~(sizeof(_heap_Align)-1) ;
        heapBuffer = (uint8_t*)alignBuffer;

#if defined(__PIC32MZ__)
        if(IS_KVA0(heapBuffer))
        {   // allocate in K1
            _TCPIP_HEAP_DataCacheInvalidate(heapBuffer, buffSize);
            heapBuffer = (uint8_t*)KVA0_TO_KVA1(heapBuffer);
        }
#endif  // defined(__PIC32MZ__)

        headerSize = ((sizeof(HEAP_DCPT) + sizeof(_headNode) - 1) / sizeof(_headNode)) * sizeof(_headNode);

        heapSize = buffSize - headerSize;

        heapUnits = heapSize / sizeof(_headNode);           // adjust to multiple of heads

        if(heapUnits < _TCPIP_HEAP_MIN_BLKS_)
        {
            res = TCPIP_HEAP_RES_BUFF_SIZE_ERR;
            break;
        }

        heapStart = heapBuffer + headerSize; 
        hDcpt = (HEAP_DCPT*)heapBuffer;
        hDcpt->_heapHead = (_headNode*)heapStart;
        hDcpt->_heapHead->units = heapUnits;
        hDcpt->_heapHead->next = 0;
        hDcpt->_heapTail = hDcpt->_heapHead;
        hDcpt->_heapUnits = heapUnits;
        hDcpt->_heapAllocatedUnits = 0;
        hDcpt->_lastHeapErr = TCPIP_HEAP_RES_OK;
        hDcpt->_heapFlags = _TCPIP_HEAP_FLAG_NONE;
#ifdef TCPIP_STACK_DRAM_DEBUG_ENABLE
        hDcpt->_heapStart = hDcpt->_heapHead;
        hDcpt->_heapEnd = hDcpt->_heapHead + heapUnits;
        hDcpt->_noMemHandler = 0;
#endif

        hDcpt->_heapSemaphoreInit = OSAL_SEM_Create(&hDcpt->_heapSemaphore, OSAL_SEM_TYPE_BINARY, 1, 1);


#if defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)
        memset(hDcpt->_heapTraceTbl, 0, sizeof(hDcpt->_heapTraceTbl)); // clear entries
#endif
#if defined(_TCPIP_STACK_DRAM_DIST_ENABLE)
        {
            // initialize the distribution sizes array
            int ix;
            TCPIP_HEAP_DIST_ENTRY* pEntry;
            const int* pSize;

            pSize = _tcpip_heap_dist_sizes;
            pEntry = hDcpt->_tcpip_heap_dist_array;
            for(ix = 0; ix < sizeof(_tcpip_heap_dist_sizes)/sizeof(*_tcpip_heap_dist_sizes) - 1; ix++, pSize++, pEntry++)
            {
                memset(pEntry, 0, sizeof(*pEntry)); 
                pEntry->lowLimit = *pSize;
                pEntry->highLimit = *(pSize + 1);
            }
        }
#endif

        res = TCPIP_HEAP_RES_OK;
        break;
    }

    if(pRes)
    {
        *pRes = res;
    }

    return hDcpt;
    
}

// deallocates the heap
// NOTE: no check is done if some blocks are still in use!
TCPIP_HEAP_RES TCPIP_HEAP_Delete(TCPIP_HEAP_HANDLE heapH)
{
    HEAP_DCPT*   hDcpt;

    hDcpt = (HEAP_DCPT*)heapH;

    if(hDcpt == 0)
    {
        return TCPIP_HEAP_RES_NO_HEAP; 
    }
    
    if(hDcpt->_heapHead)
    {
    #ifdef TCPIP_STACK_DRAM_DEBUG_ENABLE
        if(hDcpt->_heapAllocatedUnits != 0 || hDcpt->_heapHead != hDcpt->_heapStart || hDcpt->_heapHead->units != hDcpt->_heapUnits)
    #else
        if(hDcpt->_heapAllocatedUnits != 0)
    #endif
        {
            //  deallocating a heap not completely de-allocated or corrupted
            return (hDcpt->_lastHeapErr = TCPIP_HEAP_RES_IN_USE); 
        }

        if (hDcpt->_heapSemaphoreInit == OSAL_RESULT_TRUE)
        {
            if (OSAL_SEM_Delete(&hDcpt->_heapSemaphore) != OSAL_RESULT_TRUE)
            {
                //SYS_Debug message
            }
            hDcpt->_heapSemaphoreInit = OSAL_RESULT_FALSE;
        }

        return TCPIP_HEAP_RES_OK;
    }
    
    return (hDcpt->_lastHeapErr = TCPIP_HEAP_RES_IN_USE); 
}


void* _TCPIP_HEAP_Malloc(TCPIP_HEAP_HANDLE heapH, size_t nBytes)
{
	_headNode	*ptr,*prev;
	size_t      nunits;
    HEAP_DCPT*  hDcpt;


	if(!nBytes)
	{
		return 0;
	}

    hDcpt = (HEAP_DCPT*)heapH;
	
	nunits=(nBytes+sizeof(_headNode)-1)/sizeof(_headNode)+1;	// allocate units   
	prev=0;

    if (hDcpt->_heapSemaphoreInit == OSAL_RESULT_TRUE)
    {
        if (OSAL_SEM_Pend(&hDcpt->_heapSemaphore, OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
        {
            // SYS_DEBUG message
        }
    }

	for(ptr = hDcpt->_heapHead; ptr != 0; prev = ptr, ptr = ptr->next)
	{
		if(ptr->units >= nunits)
		{   // found block
			if(ptr->units-nunits <= _TCPIP_HEAP_MIN_BLK_USIZE_)
			{
				nunits=ptr->units;	// get the whole block
			}

            if(ptr->units == nunits)
			{   // exact match
				if(prev)
				{
					prev->next = ptr->next;
				}
				else
				{
					hDcpt->_heapHead = ptr->next;
                    prev = hDcpt->_heapHead;
				}

                if(hDcpt->_heapTail == ptr)
                {
                    hDcpt->_heapTail = prev;
                }
			}
			else
			{   // larger than we need
				ptr->units -= nunits;
				ptr += ptr->units;
				ptr->units = nunits;
			}

            hDcpt->_heapAllocatedUnits += nunits;
            if (hDcpt->_heapSemaphoreInit == OSAL_RESULT_TRUE)
            {
                if (OSAL_SEM_Post(&hDcpt->_heapSemaphore) != OSAL_RESULT_TRUE)
                {
                    // SYS_DEBUG message
                }
            }
            return ptr+1;
		}
	}

    hDcpt->_lastHeapErr = TCPIP_HEAP_RES_NO_MEM;
    if (hDcpt->_heapSemaphoreInit == OSAL_RESULT_TRUE)
    {
        if (OSAL_SEM_Post(&hDcpt->_heapSemaphore) != OSAL_RESULT_TRUE)
        {
            // SYS_DEBUG message
        }
    }
    return 0;
}

void* _TCPIP_HEAP_Calloc(TCPIP_HEAP_HANDLE heapH, size_t nElems, size_t elemSize)
{
    void* pBuff = _TCPIP_HEAP_Malloc(heapH, nElems * elemSize);
    if(pBuff)
    {
        memset(pBuff, 0, nElems * elemSize);
    }

    return pBuff;

}

int _TCPIP_HEAP_Free(TCPIP_HEAP_HANDLE heapH, const void* pBuff)
{  
    HEAP_DCPT*  hDcpt;
	_headNode	*hdr,*ptr;
    int         fail;
    size_t      freedUnits;

    if(!pBuff)
	{
        return 0;
    }

    hDcpt = (HEAP_DCPT*)heapH;
    ptr = (_headNode*)pBuff-1;

    if (hDcpt->_heapSemaphoreInit == OSAL_RESULT_TRUE)
    {
        if (OSAL_SEM_Pend(&hDcpt->_heapSemaphore, OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
        {
            // SYS_DEBUG message
        }
    }


#ifdef TCPIP_STACK_DRAM_DEBUG_ENABLE
    if(ptr < hDcpt->_heapStart || ptr+ptr->units > hDcpt->_heapEnd)
    {
        hDcpt->_lastHeapErr = TCPIP_HEAP_RES_PTR_ERR;   // not one of our pointers!!!
        if (hDcpt->_heapSemaphoreInit == OSAL_RESULT_TRUE)
        {
            if (OSAL_SEM_Post(&hDcpt->_heapSemaphore) != OSAL_RESULT_TRUE)
            {
                // SYS_DEBUG message
            }
        }
        return 0;
    }
#endif
    
    freedUnits = ptr->units;
    
    fail = 0;
    
    if(!hDcpt->_heapHead)
    {
        ptr->next=0;
        hDcpt->_heapHead = hDcpt->_heapTail = ptr;
    }
    else if(ptr < hDcpt->_heapHead)
    {   // put it in front
        if(ptr+ptr->units == hDcpt->_heapHead)
        {   // compact
            ptr->units += hDcpt->_heapHead->units;
            ptr->next = hDcpt->_heapHead->next;
            if(hDcpt->_heapTail == hDcpt->_heapHead)
            {
                hDcpt->_heapTail = ptr;
            }
        }
        else
        {
            ptr->next = hDcpt->_heapHead;
        }
        hDcpt->_heapHead = ptr;    // new head
    }
    else if(ptr > hDcpt->_heapTail)
    {   // append tail
        if(hDcpt->_heapTail + hDcpt->_heapTail->units == ptr)
        {   // compact
            hDcpt->_heapTail->units += ptr->units;
        }
        else
        {
            hDcpt->_heapTail->next = ptr;
            ptr->next = 0;
            hDcpt->_heapTail = ptr;
        }
    }
    else
    {   // somewhere in the middle
        fail = 1;
        for(hdr = hDcpt->_heapHead; hdr != 0; hdr = hdr->next)
        {
            if(hdr<ptr && ptr<hdr->next)
            {   // found a place
                if(ptr+ptr->units == hdr->next)
                {   // compact right
                    ptr->units += hdr->next->units;
                    ptr->next = hdr->next->next;
                    if(hDcpt->_heapTail == hdr->next)
                    {
                        hDcpt->_heapTail = ptr;
                    }
                }
                else
                {
                #ifdef TCPIP_STACK_DRAM_DEBUG_ENABLE
                    if(ptr+ptr->units > hdr->next)
                    {
                        break;  // corrupted pointer!!!
                    }
                #endif
                    ptr->next = hdr->next;
                }
                
                // compact left
                if(hdr+hdr->units == ptr)
                {
                    hdr->units += ptr->units;
                    hdr->next = ptr->next;
                    if(hDcpt->_heapTail == ptr)
                    {
                        hDcpt->_heapTail = hdr;
                    }
                }
                else
                {
                #ifdef TCPIP_STACK_DRAM_DEBUG_ENABLE
                    if(hdr+hdr->units > ptr)
                    {
                        break;      // corrupted pointer!!!
                    }
                #endif
                    hdr->next = ptr;
                }

                fail = 0;   // everything OK
                break;                
            }
        }
        
    }

    if(fail)
    {
        hDcpt->_lastHeapErr = TCPIP_HEAP_RES_PTR_ERR;   // corrupted pointer!!!
        if (hDcpt->_heapSemaphoreInit == OSAL_RESULT_TRUE)
        {
            if (OSAL_SEM_Post(&hDcpt->_heapSemaphore) != OSAL_RESULT_TRUE)
            {
                // SYS_DEBUG message
            }
        }
        return 0;
    }
    
    hDcpt->_heapAllocatedUnits -= freedUnits;
    if (hDcpt->_heapSemaphoreInit == OSAL_RESULT_TRUE)
    {
        if (OSAL_SEM_Post(&hDcpt->_heapSemaphore) != OSAL_RESULT_TRUE)
        {
            // SYS_DEBUG message
        }
    }
    return freedUnits;
}


size_t TCPIP_HEAP_Size(TCPIP_HEAP_HANDLE heapH)
{
    HEAP_DCPT*      hDcpt;

    hDcpt = (HEAP_DCPT*)heapH;

    if(hDcpt)
    {
        return hDcpt->_heapUnits * sizeof(_headNode);   
    }

    return 0;
}

size_t TCPIP_HEAP_FreeSize(TCPIP_HEAP_HANDLE heapH)
{
    HEAP_DCPT*      hDcpt;

    hDcpt = (HEAP_DCPT*)heapH;

    if(hDcpt)
    {
        return (hDcpt->_heapUnits - hDcpt->_heapAllocatedUnits) * sizeof(_headNode);   
    }
    return 0;
}


size_t TCPIP_HEAP_MaxSize(TCPIP_HEAP_HANDLE heapH)
{
    HEAP_DCPT   *hDcpt;
    _headNode	*ptr;
    size_t      max_nunits;

    max_nunits = 0;
    hDcpt = (HEAP_DCPT*)heapH;

    if(hDcpt)
    {
        if (hDcpt->_heapSemaphoreInit == OSAL_RESULT_TRUE)
        {
            OSAL_SEM_Pend(&hDcpt->_heapSemaphore, OSAL_WAIT_FOREVER);
        }

        for(ptr = hDcpt->_heapHead; ptr != 0; ptr = ptr->next)
        {
            if(ptr->units >= max_nunits)
            {   // found block
                max_nunits = ptr->units;
            }
        }
        if (hDcpt->_heapSemaphoreInit == OSAL_RESULT_TRUE)
        {
            OSAL_SEM_Post(&hDcpt->_heapSemaphore);
        }
    }

    return max_nunits * sizeof(_headNode);   

}


TCPIP_HEAP_RES TCPIP_HEAP_LastError(TCPIP_HEAP_HANDLE heapH)
{
    HEAP_DCPT*      hDcpt;
    TCPIP_HEAP_RES  res;

    hDcpt = (HEAP_DCPT*)heapH;
    if(hDcpt)
    {
        res = hDcpt->_lastHeapErr;
        hDcpt->_lastHeapErr = TCPIP_HEAP_RES_OK;
        return res;
    }

    return TCPIP_HEAP_RES_NO_HEAP;

}

#if defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) 
void* _TCPIP_HEAP_MallocDebug(TCPIP_HEAP_HANDLE heapH, size_t nBytes, int moduleId, int lineNo)
{
    void* ptr = _TCPIP_HEAP_Malloc(heapH, nBytes);
    if(ptr == 0)
    {
        SYS_ERROR_PRINT(SYS_ERROR_WARNING, _heapFailMessage, nBytes, moduleId, lineNo);
        if(((HEAP_DCPT*)heapH)->_noMemHandler != 0)
        {
            (*((HEAP_DCPT*)heapH)->_noMemHandler)(heapH, nBytes, moduleId, lineNo);
        }
    }
#if defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)
    nBytes = TCPIP_HEAP_AddToEntry((HEAP_DCPT*)heapH, moduleId, nBytes, ptr);
#endif  // defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)

#if defined(_TCPIP_STACK_DRAM_DIST_ENABLE)
    TCPIP_HEAP_DistAdd((HEAP_DCPT*)heapH, moduleId, nBytes);
#endif  // defined(_TCPIP_STACK_DRAM_DIST_ENABLE)

    return ptr;
}

void* _TCPIP_HEAP_CallocDebug(TCPIP_HEAP_HANDLE heapH, size_t nElems, size_t elemSize, int moduleId, int lineNo)
{
    size_t nBytes = nElems * elemSize;
    void* ptr = _TCPIP_HEAP_Calloc(heapH, nElems, elemSize);

    if(ptr == 0)
    {
        SYS_ERROR_PRINT(SYS_ERROR_WARNING, _heapFailMessage, nBytes, moduleId, lineNo);
        if(((HEAP_DCPT*)heapH)->_noMemHandler != 0)
        {
            (*((HEAP_DCPT*)heapH)->_noMemHandler)(heapH, nBytes, moduleId, lineNo);
        }
    }
#if defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)
    nBytes = TCPIP_HEAP_AddToEntry((HEAP_DCPT*)heapH, moduleId, nBytes, ptr);
#endif  // defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)

#if defined(_TCPIP_STACK_DRAM_DIST_ENABLE)
    TCPIP_HEAP_DistAdd((HEAP_DCPT*)heapH, moduleId, nBytes);
#endif  // defined(_TCPIP_STACK_DRAM_DIST_ENABLE)

    return ptr;
}

int _TCPIP_HEAP_FreeDebug(TCPIP_HEAP_HANDLE heapH,  const void* pBuff, int moduleId)
{
    int nUnits = _TCPIP_HEAP_Free(heapH, pBuff);

#if defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)
    if(nUnits)
    {
        TCPIP_HEAP_RemoveFromEntry((HEAP_DCPT*)heapH, moduleId, nUnits);
#if defined(_TCPIP_STACK_DRAM_DIST_ENABLE)
        TCPIP_HEAP_DistRem((HEAP_DCPT*)heapH, moduleId, nUnits * sizeof(_headNode));
#endif  // defined(_TCPIP_STACK_DRAM_DIST_ENABLE)
    }
#endif  // defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)

    return nUnits;
}

TCPIP_HEAP_RES TCPIP_HEAP_SetNoMemHandler(TCPIP_HEAP_HANDLE heapH, TCPIP_HEAP_NO_MEM_HANDLER handler)
{
    HEAP_DCPT   *hDcpt = (HEAP_DCPT*)heapH;

    if(hDcpt)
    {
        hDcpt->_noMemHandler = handler;
    }
    return TCPIP_HEAP_RES_OK;
}

#endif  // defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) 

#if defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)
static TCPIP_HEAP_TRACE_ENTRY* TCPIP_HEAP_FindEntry(HEAP_DCPT* hDcpt, int moduleId, bool addNewSlot)
{
    int ix;
    TCPIP_HEAP_TRACE_ENTRY    *freeEntry,*pEntry;
#if _TCPIP_STACK_DRAM_TRACE_NDP_COLLAPSE
    if(moduleId == TCPIP_MODULE_NDP)
    {
        moduleId = TCPIP_MODULE_IPV6;
    }
#endif  // _TCPIP_STACK_DRAM_TRACE_NDP_COLLAPSE

    freeEntry = 0;
    for(ix = 0, pEntry = hDcpt->_heapTraceTbl; ix < sizeof(hDcpt->_heapTraceTbl)/sizeof(*hDcpt->_heapTraceTbl); ix++, pEntry++)
    {
        if(pEntry->moduleId == moduleId)
        {
            return pEntry;
        }
        else if(addNewSlot && freeEntry == 0 && pEntry->moduleId == 0)
        {
            freeEntry = pEntry;
        }
    }

    if(freeEntry)
    {
        memset(freeEntry, 0x0, sizeof(*freeEntry));
        freeEntry->moduleId = moduleId;
    }

    return freeEntry;
}

static size_t TCPIP_HEAP_AddToEntry(HEAP_DCPT* hDcpt, int moduleId, size_t nBytes, void* ptr)
{
    TCPIP_HEAP_TRACE_ENTRY *pEntry = TCPIP_HEAP_FindEntry(hDcpt, moduleId, true);

    if(pEntry)
    {
        if(ptr)
        {   // successful
            _headNode* hPtr = (_headNode*)ptr -1;
            nBytes = hPtr->units * sizeof(_headNode);
            pEntry->totAllocated += nBytes;
            pEntry->currAllocated += nBytes;
        }
        else
        {
            size_t units = (nBytes+sizeof(_headNode)-1)/sizeof(_headNode)+1;    // allocation units
            nBytes = units * sizeof(_headNode);
            pEntry->totFailed += nBytes;
            if(nBytes > pEntry->maxFailed)
            {
                pEntry->maxFailed = nBytes;
            }
        }
    }

    return nBytes;
}

static void TCPIP_HEAP_RemoveFromEntry(HEAP_DCPT* hDcpt, int moduleId, size_t nUnits)
{
    TCPIP_HEAP_TRACE_ENTRY *pEntry = TCPIP_HEAP_FindEntry(hDcpt, moduleId, false);

    if(pEntry)
    {
        pEntry->currAllocated -= nUnits * sizeof(_headNode);
    }
}

bool TCPIP_HEAP_TraceGetEntry(TCPIP_HEAP_HANDLE heapH, unsigned int entryIx, TCPIP_HEAP_TRACE_ENTRY* tEntry)
{
    TCPIP_HEAP_TRACE_ENTRY *pEntry;
    HEAP_DCPT   *hDcpt = (HEAP_DCPT*)heapH;

    if(entryIx < sizeof(hDcpt->_heapTraceTbl)/sizeof(*hDcpt->_heapTraceTbl))
    {   // valid index
        pEntry = hDcpt->_heapTraceTbl + entryIx;
        if(pEntry->moduleId > 0)
        {
            *tEntry = *pEntry;
            return true;
        }
    }

    return false;
}

unsigned int TCPIP_HEAP_TraceGetEntriesNo(TCPIP_HEAP_HANDLE heapH, bool getUsed)
{
    HEAP_DCPT   *hDcpt = (HEAP_DCPT*)heapH;

    if(getUsed)
    {
        TCPIP_HEAP_TRACE_ENTRY *pEntry;
        int ix, nUsed;

        nUsed = 0;

        for(ix = 0, pEntry = hDcpt->_heapTraceTbl; ix < sizeof(hDcpt->_heapTraceTbl)/sizeof(*hDcpt->_heapTraceTbl); ix++, pEntry++)
        {
            if(pEntry->moduleId > 0)
            {
                nUsed++;
            }
        }

        return nUsed;
    }

    return sizeof(hDcpt->_heapTraceTbl)/sizeof(*hDcpt->_heapTraceTbl);
}


#endif  // defined(_TCPIP_STACK_DRAM_TRACE_ENABLE)

#if defined (_TCPIP_STACK_DRAM_DIST_ENABLE)
bool TCPIP_HEAP_DistGetEntry(TCPIP_HEAP_HANDLE heapH, unsigned int entryIx, TCPIP_HEAP_DIST_ENTRY* pEntry)
{
    HEAP_DCPT   *hDcpt = (HEAP_DCPT*)heapH;

    if(pEntry)
    {
        if(entryIx < sizeof(hDcpt->_tcpip_heap_dist_array)/sizeof(*hDcpt->_tcpip_heap_dist_array))
        {   // valid index
           *pEntry =  *(hDcpt->_tcpip_heap_dist_array + entryIx);
           return true;
        }
    }

    return false;
}

unsigned int TCPIP_HEAP_DistGetEntriesNo(TCPIP_HEAP_HANDLE heapH)
{
    HEAP_DCPT   *hDcpt = (HEAP_DCPT*)heapH;
    return sizeof(hDcpt->_tcpip_heap_dist_array)/sizeof(*hDcpt->_tcpip_heap_dist_array);
}


static int TCPIP_HEAP_DistCompare(const void *a, const void *b)
{
    TCPIP_HEAP_DIST_ENTRY* pA = (TCPIP_HEAP_DIST_ENTRY*)a;
    TCPIP_HEAP_DIST_ENTRY* pB = (TCPIP_HEAP_DIST_ENTRY*)b;

    if(pA->lowLimit > pB->highLimit)
    {
        return 1;
    }
    else if(pA->highLimit < pB->lowLimit)
    {
        return -1;
    }

    return 0;
}

static void TCPIP_HEAP_DistAdd(HEAP_DCPT* hDcpt, int moduleId, size_t nBytes)
{
    int     modIx;
    TCPIP_HEAP_DIST_ENTRY* pEntry;
    TCPIP_HEAP_DIST_ENTRY nEntry =
    {
        .lowLimit = nBytes,
        .highLimit = nBytes,
        { {0} },
        0
    };
    
    pEntry = bsearch(&nEntry, hDcpt->_tcpip_heap_dist_array, sizeof(hDcpt->_tcpip_heap_dist_array)/sizeof(*hDcpt->_tcpip_heap_dist_array), sizeof(*hDcpt->_tcpip_heap_dist_array), TCPIP_HEAP_DistCompare);

    pEntry->currHits++;
    // search for module
    struct moduleDist* pFreeMod = 0;
    struct moduleDist* pmDist = pEntry->modDist;
    for(modIx = 0; modIx < sizeof(pEntry->modDist)/sizeof(*pEntry->modDist); modIx++, pmDist++)
    {
        if(pmDist->modId == moduleId)
        {   // found module slot
            pmDist->modHits++;
            return;
        }
        else if(pmDist->modId == 0 && pFreeMod == 0)
        {
            pFreeMod = pmDist;
        }
    }

    // module slot not found
    if(pFreeMod)
    {
        pFreeMod->modId = moduleId;
        pFreeMod->modHits = 1;
    }
    else
    {   // all slots occupied, use the generic entry
        pEntry->gHits++;
    }
}


static void TCPIP_HEAP_DistRem(HEAP_DCPT* hDcpt, int moduleId, size_t nBytes)
{
    TCPIP_HEAP_DIST_ENTRY* pEntry;
    TCPIP_HEAP_DIST_ENTRY nEntry =
    {
        .lowLimit = nBytes,
        .highLimit = nBytes,
        { {0} },
        0
    };
    
    pEntry = bsearch(&nEntry, hDcpt->_tcpip_heap_dist_array, sizeof(hDcpt->_tcpip_heap_dist_array)/sizeof(*hDcpt->_tcpip_heap_dist_array), sizeof(*hDcpt->_tcpip_heap_dist_array), TCPIP_HEAP_DistCompare);

    pEntry->currHits--;
}



#endif  // defined (_TCPIP_STACK_DRAM_DIST_ENABLE)

