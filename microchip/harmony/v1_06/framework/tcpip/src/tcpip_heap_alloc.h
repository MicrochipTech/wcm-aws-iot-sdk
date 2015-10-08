/*******************************************************************************
  TCPIP Heap Allocation Manager

  Company:
    Microchip Technology Inc.
    
  File Name:
    tcpip_heap_alloc.h

  Summary:
    
  Description:
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright © 2012 released Microchip Technology Inc.  All rights reserved.

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
// DOM-IGNORE-END

#ifndef _TCPIP_HEAP_ALLOC_H_
#define _TCPIP_HEAP_ALLOC_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    TCPIP_HEAP_RES_OK               = 0,   // everything OK
    
    // error codes

    TCPIP_HEAP_RES_BUFF_ERR         = -1,   // invalid supplied heap buffer
    
    TCPIP_HEAP_RES_BUFF_ALIGN_ERR   = -2,   // supplied heap buffer not properly aligned
    
    TCPIP_HEAP_RES_BUFF_SIZE_ERR    = -3,   // supplied heap buffer too small
    
    TCPIP_HEAP_RES_NO_MEM           = -4,   // out of memory
    
    TCPIP_HEAP_RES_IN_USE           = -5,   // heap in use, cannot be deallocated
    
    TCPIP_HEAP_RES_PTR_ERR          = -6,   // corrupt pointer

    TCPIP_HEAP_RES_NO_HEAP          = -7,   // no such heap exists

}TCPIP_HEAP_RES;    // result of a heap operation


typedef enum
{
    TCPIP_HEAP_FLAG_NONE      = 0x00,
    
}TCPIP_HEAP_FLAGS;  // heap creation/allocation flags


// handle to a heap
typedef const void* TCPIP_HEAP_HANDLE;

// heap trace
// only if TCPIP_STACK_DRAM_DEBUG_ENABLE and TCPIP_STACK_DRAM_TRACE_ENABLE are enabled
// the moduleId is the one from tcpip.h::TCPIP_STACK_MODULE
typedef struct
{
    int         moduleId;           // info belonging to this module; <0 means slot free
    uint32_t    totAllocated;       // total number of bytes allocated successfully by this module
    uint32_t    currAllocated;      // number of bytes still allocated by this module
    uint32_t    totFailed;          // total number of bytes that failed for this module
    uint32_t    maxFailed;          // maximum number of bytes that could not be allocated
}TCPIP_HEAP_TRACE_ENTRY;

// heap distribution entry
// useful for checking the distribution of allocated memory blocks in the heap
// only if TCPIP_STACK_DRAM_DEBUG_ENABLE and TCPIP_STACK_DRAM_DIST_ENABLE are enabled
//
typedef struct
{
    int         lowLimit;           // lower limit of the memory block
    int         highLimit;          // upper limit of the memory block
    struct moduleDist
    {
        int     modId;              // module id
        int     modHits;            // number of module hits
    }modDist[4];
    int         gHits;              // number of global allocations hits, no module info
    int         currHits;           // current number of allocations hits
}TCPIP_HEAP_DIST_ENTRY;

/********************************
 * Interface Functions
*******************************************/ 

// Creates a TCPIP memory heap
// A memory heap consists of block of data that is
// allocated in chunks using a first fit algorithm.
// Allocation and deallocation operations are not very fast
// (but reasonably fast).
// However, the allocation granularity is pretty good.

// There is no data maintained in this heap on a per client basis.
// What this means is that is OK to pass the heap handle to other
// clients (software modules) to use it.
// Note that this is a private TCPIP heap and no multi-threaded protection is provided!
//
/*********************************************************************
 * Function:        TCPIP_HEAP_HANDLE TCPIP_HEAP_Create(uint8_t* heapBuffer, uint8_t size_t buffSize, TCPIP_HEAP_FLAGS flags, TCPIP_HEAP_RES* pRes)
 *
 * PreCondition:    heapBuffer, buffSize    - valid buffer and size values
 *                  heapBuffer - properly aligned buffer
 *
 * Input:           heapBuffer  - buffer to contain the heap
 *                  buffSize    - size of the supplied buffer
 *                  flags       - heap creation/allocation flags.
 *                                Not used for now.
 *                  pRes        - Address to store the operation result.
 *                                Can be NULL if result not needed         
 *
 * Output:          A valid handle if the creation succeeded,
 *                  NULL otherwise.         
 *
 * Side Effects:    None
 *
 * Overview:        The function creates a heap of the maximum possible size in the supplied buffer space.
 *                  The size of the heap cannot be changed afterwards.
 *
 * Note:            The current heap alignment unit is an unsigned long, i.e. 32 bits.
 *                  The supplied heapBuffer value has to be correctly 32 bits aligned.
 *                  Other wise a TCPIP_HEAP_RES_ALIGN_ERR error code will be returned!
 *
 ********************************************************************/
TCPIP_HEAP_HANDLE   TCPIP_HEAP_Create(uint8_t* heapBuffer, size_t buffSize, TCPIP_HEAP_FLAGS flags, TCPIP_HEAP_RES* pRes);

/*********************************************************************
 * Function:        TCPIP_HEAP_RES TCPIP_HEAP_Delete(TCPIP_HEAP_HANDLE heapH);
 *
 * PreCondition:    heapH       - valid heap handle 
 *
 * Input:           heapH       - handle to a heap that was created with TCPIP_HEAP_Create()
 *
 * Output:          TCPIP_HEAP_RES_OK if deletion succeeded
 *                  TCPIP_HEAP_RES_IN_USE if the heap is still used
 *                  TCPIP_HEAP_RES_NO_HEAP if no such heap exists
 *                  
 *
 * Side Effects:    None
 *
 * Overview:        The function deletes a previously created heap.
 *
 * Note:            The call will fail if blocks are still in use.
 ********************************************************************/
TCPIP_HEAP_RES      TCPIP_HEAP_Delete(TCPIP_HEAP_HANDLE heapH);


/*********************************************************************
 * Function:        void* TCPIP_HEAP_Malloc(TCPIP_HEAP_HANDLE heapH, size_t nBytes);
 *
 * PreCondition:    heapH       - valid heap handle 
 *
 * Input:           heapH       - handle to a heap
 *                  nBytes      - size of the block to be allocated
 *
 * Output:          a valid pointer to an allocated block if allocation succeeded,
 *                  NULL otherwise.         
 *
 * Side Effects:    None
 *
 * Overview:        The function checks the heap for finding a block large enough to accommodate the request.
 *                  A first fit algorithm is used.
 *
 * Note:            None
 ********************************************************************/
void*               TCPIP_HEAP_Malloc(TCPIP_HEAP_HANDLE heapH, size_t nBytes);


/*********************************************************************
 * Function:        void* TCPIP_HEAP_Calloc(TCPIP_HEAP_HANDLE heapH, size_t nElems, size_t elemSize);
 *
 * PreCondition:    heapH       - valid heap handle 
 *
 * Input:           heapH       - handle to a heap
 *                  nElems      - number of elements to be allocated
 *                  elemSize    - size of each element, in bytes
 *
 * Output:          a valid pointer to an allocated block if allocation succeeded,
 *                  NULL otherwise.         
 *
 * Side Effects:    None
 *
 * Overview:        The function checks the heap for finding a block large enough to accommodate
 *                  nElems*elemSize request.
 *                  If the block is found, it is zero initialized and returned to user.
 *                  A first fit algorithm is used.
 *
 * Note:            None
 ********************************************************************/
void*               TCPIP_HEAP_Calloc(TCPIP_HEAP_HANDLE heapH, size_t nElems, size_t elemSize);

/*********************************************************************
 * Function:        int TCPIP_HEAP_Free(TCPIP_HEAP_HANDLE heapH, void* pBuff);
 *
 * PreCondition:    heapH       - valid heap handle 
 *
 * Input:           heapH       - handle to a heap
 *                  pBuff       - pointer to a buffer previously allocated from the heap 
 *
 * Output:          true if the operation succeeded,
 *                  false otherwise       
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the buffer to the heap.
 *                  Left and/or right defragment operations are performed if possible.
 *
 * Note:            None
 ********************************************************************/
int                 TCPIP_HEAP_Free(TCPIP_HEAP_HANDLE heapH, const void* pBuff);



/*********************************************************************
 * Function:        size_t TCPIP_HEAP_Size(TCPIP_HEAP_HANDLE heapH);
 *
 * PreCondition:    heapH       - valid heap handle 
 *
 * Input:           heapH       - handle to a heap
 *
 * Output:          the size of the heap as it was created
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the size of the heap.
 *                  This is the size that was specified when the heap was created.
 *
 * Note:            None
 ********************************************************************/
size_t                 TCPIP_HEAP_Size(TCPIP_HEAP_HANDLE heapH);


/*********************************************************************
 * Function:        size_t TCPIP_HEAP_MaxSize(TCPIP_HEAP_HANDLE heapH);
 *
 * PreCondition:    heapH       - valid heap handle 
 *
 * Input:           heapH       - handle to a heap
 *
 * Output:          the max size of a block that can be allocated from the heap
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the maximum size of a block that can be 
 *                  currently allocated from this heap.
 *
 * Note:            This is info only.
 *                  It can change is the heap has multiple clients.
 *
 *                  The call is expensive.
 *                  The whole heap has to be traversed to find the maximum.
 *                  If the heap is really fragmented this might take some time.
 *
 ********************************************************************/
size_t                 TCPIP_HEAP_MaxSize(TCPIP_HEAP_HANDLE heapH);



/*********************************************************************
 * Function:        size_t TCPIP_HEAP_FreeSize(TCPIP_HEAP_HANDLE heapH);
 *
 * PreCondition:    heapH       - valid heap handle 
 *
 * Input:           heapH       - handle to a heap
 *
 * Output:          the size of the available space in the heap
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the size of the space currently
 *                  available in the heap.
 *
 * Note:            This is a cumulative number, counting all the existent free space.
 *                  It is not the maximum blocks size that could be allocated from the heap.
 ********************************************************************/
size_t                 TCPIP_HEAP_FreeSize(TCPIP_HEAP_HANDLE heapH);


/*********************************************************************
 * Function:      TCPIP_HEAP_RES TCPIP_HEAP_LastError(TCPIP_HEAP_HANDLE heapH)
 *
 * PreCondition:    heapH       - valid heap handle
 *
 * Input:           heapH       - handle of a heap
 *
 * Output:          The last error encountered in an operation
 *                  or TCPIP_HEAP_RES_OK if no error occurred
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the last error encountered in a heap operation.
 *                  It clears the value of the last error variable.
 *
 * Note:            The heap holds an error variable storing the last error
 *                  encountered in an operation.
 *                  This should be consulted by the caller after each operation
 *                  that returns an invalid result for checking what the error condition
 *                  was.
 ********************************************************************/
TCPIP_HEAP_RES      TCPIP_HEAP_LastError(TCPIP_HEAP_HANDLE heapH);


// simple out of memory notification mechanism
// only one client/handler supported per heap
typedef void    (*TCPIP_HEAP_NO_MEM_HANDLER)(TCPIP_HEAP_HANDLE, size_t nBytes,  int moduleId, int lineNo);

/*********************************************************************
 * Function:      TCPIP_HEAP_RES TCPIP_HEAP_SetNoMemHandler(TCPIP_HEAP_HANDLE heapH, TCPIP_HEAP_NO_MEM_HANDLER handler)
 *
 * PreCondition:    heapH       - valid heap handle
 *
 * Input:           heapH       - handle of a heap
 *                  handler     - notification handler to be called when a memory allocation failed
 *                                use 0 to clear
 *
 * Output:          TCPIP_HEAP_RES_OK if no error occurred
 *
 * Side Effects:    None
 *
 * Overview:        The function sets the out of memory notification handler.
 *                  It is a very simple notification and only one client is supported,
 *                  basically it is meant for the creator of the heap.
 *                  When the debug is enabled and some allocation fails
 *                  some system warning message will be sent to the debug port.
 *                  Also if the handler is not null it will be called.
 *
 * Note:            The handler could be a signaling function, short
 *                  and just setting an event of some sort.
 *                  However since it is not called from ISR context this is not really a requirement and
 *                  some more extensive processing can be done.
 *
 *                  This function is implemented only when the system debug and
 *                  heap debug (TCPIP_STACK_DRAM_DEBUG_ENABLE) are enabled 
 ********************************************************************/
TCPIP_HEAP_RES TCPIP_HEAP_SetNoMemHandler(TCPIP_HEAP_HANDLE heapH, TCPIP_HEAP_NO_MEM_HANDLER handler);


/*********************************************************************
 * Function:      bool  TCPIP_HEAP_TraceGetEntry(TCPIP_HEAP_HANDLE heapH, unsigned int entryIx, TCPIP_HEAP_TRACE_ENTRY* tEntry)
 *
 * PreCondition:    None
 *
 * Input:           heapH       - handle of a heap
 *                  entryIx     - index of the requested heap trace entry
 *                  tEntry      - address of a TCPIP_HEAP_TRACE_ENTRY that will be updated with corresponding info
 *
 * Output:          true if tEntry was populated with the info
 *                  false if entryIx record does not exist or if tracing is not enabled
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the heap trace info for the requested entry.
 *
 * Note:            
 *                  Trace info is recorded only when
 *                  TCPIP_STACK_DRAM_DEBUG_ENABLE and TCPIP_STACK_DRAM_TRACE_ENABLE are enabled
 *                  Also, the number of defined slots TCPIP_STACK_DRAM_TRACE_SLOTS has to be 
 *                  big enough to accommodate all the modules.
 *
 *                  The moduleId is the one from tcpip.h::TCPIP_STACK_MODULE
 ********************************************************************/
bool  TCPIP_HEAP_TraceGetEntry(TCPIP_HEAP_HANDLE heapH, unsigned int entryIx, TCPIP_HEAP_TRACE_ENTRY* tEntry);

/*********************************************************************
 * Function:      unsigned int  TCPIP_HEAP_TraceGetEntriesNo(TCPIP_HEAP_HANDLE heapH, bool getUsed)
 *
 * PreCondition:    None
 *
 * Input:           heapH       - handle of a heap
 *                  getUsed     - if true, only the currently used record slots will be returned
 *                                otherwise the whole number of slots will be returned  
 *
 * Output:          number of entries the heap has trace slots for
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the number of slots that hold trace info.
 *                  By using the "getUsed" parameter, only the number of slots
 *                  that hold recorded info will be returned.
 *
 * Note:            
 *                  Trace info is recorded only when
 *                  TCPIP_STACK_DRAM_DEBUG_ENABLE and TCPIP_STACK_DRAM_TRACE_ENABLE are enabled
 *                  Also, the number of defined slots TCPIP_STACK_DRAM_TRACE_SLOTS has to be 
 *                  big enough to accommodate all the modules.
 *
 *                  The moduleId is the one from tcpip.h::TCPIP_STACK_MODULE
 *
 *                  If trace is enabled the function will return the value of TCPIP_STACK_DRAM_TRACE_SLOTS
 ********************************************************************/
unsigned int     TCPIP_HEAP_TraceGetEntriesNo(TCPIP_HEAP_HANDLE heapH, bool getUsed);


/*********************************************************************
 * Function:      bool  TCPIP_HEAP_DistGetEntry(TCPIP_HEAP_HANDLE heapH, unsigned int entryIx, TCPIP_HEAP_DIST_ENTRY* pEntry)
 *
 * PreCondition:    None
 *
 * Input:           heapH       - handle of a heap
 *                  entryIx     - index of the requested heap distribution entry
 *                  pEntry      - address of a TCPIP_HEAP_DIST_ENTRY that will be updated with corresponding info
 *
 * Output:          true if pEntry was populated with the info
 *                  false if entryIx record does not exist or if heap distribution is not enabled
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the heap distribution info for the requested entry.
 *
 * Note:            
 *                  Heap distribution info is recorded only when
 *                  TCPIP_STACK_DRAM_DEBUG_ENABLE and TCPIP_STACK_DRAM_DIST_ENABLE are enabled
 *
 ********************************************************************/
bool  TCPIP_HEAP_DistGetEntry(TCPIP_HEAP_HANDLE heapH, unsigned int entryIx, TCPIP_HEAP_DIST_ENTRY* pEntry);

/*********************************************************************
 * Function:      unsigned int  TCPIP_HEAP_DistGetEntriesNo(TCPIP_HEAP_HANDLE heapH)
 *
 * PreCondition:    None
 *
 * Input:           heapH       - handle of a heap
 *
 * Output:          number of entries the heap has distribution slots for
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the number of slots that hold distribution info.
 *
 * Note:            
 *                  Heap distribution info is recorded only when
 *                  TCPIP_STACK_DRAM_DEBUG_ENABLE and TCPIP_STACK_DRAM_DIST_ENABLE are enabled
 *
 ********************************************************************/
unsigned int     TCPIP_HEAP_DistGetEntriesNo(TCPIP_HEAP_HANDLE heapH);

// debugging facilities
#if defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) 

    // debug allocation functions
    
    typedef void*       (*_TCPIP_HEAP_MallocDebugPtr)(TCPIP_HEAP_HANDLE heapH, size_t nBytes, int moduleId, int lineNo);
    typedef void*       (*_TCPIP_HEAP_CallocDebugPtr)(TCPIP_HEAP_HANDLE heapH, size_t nElems, size_t elemSize, int moduleId, int lineNo);
    typedef int         (*_TCPIP_HEAP_FreeDebugPtr)(TCPIP_HEAP_HANDLE heapH,  const void* pBuff, int moduleId);
    #define             _TCPIP_HEAP_MALLOC_PTR  _TCPIP_HEAP_MallocDebugPtr
    #define             _TCPIP_HEAP_CALLOC_PTR  _TCPIP_HEAP_CallocDebugPtr
    #define             _TCPIP_HEAP_FREE_PTR    _TCPIP_HEAP_FreeDebugPtr


    void*               _TCPIP_HEAP_MallocDebug(TCPIP_HEAP_HANDLE heapH, size_t nBytes, int moduleId, int lineNo);
    void*               _TCPIP_HEAP_CallocDebug(TCPIP_HEAP_HANDLE heapH, size_t nElems, size_t elemSize, int moduleId, int lineNo);
    int                 _TCPIP_HEAP_FreeDebug(TCPIP_HEAP_HANDLE heapH,  const void* pBuff, int moduleId);

    // for pointer calls
    #define             _TCPIP_HEAP_MALLOC_FNC  _TCPIP_HEAP_MallocDebug
    #define             _TCPIP_HEAP_CALLOC_FNC  _TCPIP_HEAP_CallocDebug
    #define             _TCPIP_HEAP_FREE_FNC    _TCPIP_HEAP_FreeDebug

    #define             _TCPIP_HEAP_MALLOC_BY_PTR(ptr, heapH, nBytes)             (*ptr)(heapH, nBytes, TCPIP_THIS_MODULE_ID, __LINE__)
    #define             _TCPIP_HEAP_CALLOC_BY_PTR(ptr, heapH, nElems, elemSize)   (*ptr)(heapH, nElems, elemSize, TCPIP_THIS_MODULE_ID, __LINE__)
    #define             _TCPIP_HEAP_FREE_BY_PTR(ptr, heapH, buff)                 (*ptr)(heapH, buff, TCPIP_THIS_MODULE_ID)

    
    // for direct calls
    #define             TCPIP_HEAP_Malloc(heapH, nBytes)            _TCPIP_HEAP_MallocDebug(heapH, nBytes, TCPIP_THIS_MODULE_ID, __LINE__)
    #define             TCPIP_HEAP_Calloc(heapH, nElems, elemSize)  _TCPIP_HEAP_CallocDebug(heapH, nElems, elemSize, TCPIP_THIS_MODULE_ID, __LINE__)
    #define             TCPIP_HEAP_Free(heapH, ptr)                 _TCPIP_HEAP_FreeDebug(heapH, ptr, TCPIP_THIS_MODULE_ID)

#else

    typedef void*       (*_TCPIP_HEAP_MallocPtr)(TCPIP_HEAP_HANDLE heapH, size_t nBytes);
    typedef void*       (*_TCPIP_HEAP_CallocPtr)(TCPIP_HEAP_HANDLE heapH, size_t nElems, size_t elemSize);
    typedef int         (*_TCPIP_HEAP_FreePtr)(TCPIP_HEAP_HANDLE heapH,  const void* pBuff);
    #define             _TCPIP_HEAP_MALLOC_PTR  _TCPIP_HEAP_MallocPtr
    #define             _TCPIP_HEAP_CALLOC_PTR  _TCPIP_HEAP_CallocPtr
    #define             _TCPIP_HEAP_FREE_PTR    _TCPIP_HEAP_FreePtr

    void*               _TCPIP_HEAP_Malloc(TCPIP_HEAP_HANDLE heapH, size_t nBytes);
    void*               _TCPIP_HEAP_Calloc(TCPIP_HEAP_HANDLE heapH, size_t nElems, size_t elemSize);
    int                 _TCPIP_HEAP_Free(TCPIP_HEAP_HANDLE heapH,  const void* pBuff);
    
    // for pointer calls
    #define             _TCPIP_HEAP_MALLOC_FNC  _TCPIP_HEAP_Malloc
    #define             _TCPIP_HEAP_CALLOC_FNC  _TCPIP_HEAP_Calloc
    #define             _TCPIP_HEAP_FREE_FNC    _TCPIP_HEAP_Free
    
    #define             _TCPIP_HEAP_MALLOC_BY_PTR(ptr, heapH, nBytes)             (*ptr)(heapH, nBytes)
    #define             _TCPIP_HEAP_CALLOC_BY_PTR(ptr, heapH, nElems, elemSize)   (*ptr)(heapH, nElems, elemSize)
    #define             _TCPIP_HEAP_FREE_BY_PTR(ptr, heapH, buff)                 (*ptr)(heapH, buff)

    // for direct calls
    #define             TCPIP_HEAP_Malloc(heapH, nBytes)            _TCPIP_HEAP_Malloc(heapH, nBytes)
    #define             TCPIP_HEAP_Calloc(heapH, nElems, elemSize)  _TCPIP_HEAP_Calloc(heapH, nElems, elemSize)
    #define             TCPIP_HEAP_Free(heapH, ptr)                 _TCPIP_HEAP_Free(heapH, ptr)

    #define             TCPIP_HEAP_SetNoMemHandler(h, handler)  (TCPIP_HEAP_RES_OK)

    
#endif  // defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) 



#if !defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) || !defined(TCPIP_STACK_DRAM_TRACE_ENABLE) 
extern __inline__ bool __attribute__((always_inline)) TCPIP_HEAP_TraceGetEntry(TCPIP_HEAP_HANDLE heapH, unsigned int entryIx, TCPIP_HEAP_TRACE_ENTRY* tEntry)
{
    return false;
}

extern __inline__ unsigned int __attribute__((always_inline)) TCPIP_HEAP_TraceGetEntriesNo(TCPIP_HEAP_HANDLE heapH, bool used)
{
    return 0;
}

#endif  // !defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) || !defined(TCPIP_STACK_DRAM_TRACE_ENABLE) 

#if !defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) || !defined(TCPIP_STACK_DRAM_DIST_ENABLE) 


extern __inline__ bool __attribute__((always_inline)) TCPIP_HEAP_DistGetEntry(TCPIP_HEAP_HANDLE heapH, unsigned int entryIx, TCPIP_HEAP_DIST_ENTRY* pEntry)
{
    return false;
}

extern __inline__ unsigned int __attribute__((always_inline)) TCPIP_HEAP_DistGetEntriesNo(TCPIP_HEAP_HANDLE heapH)
{
    return 0;
}


#endif  // !defined(TCPIP_STACK_DRAM_DEBUG_ENABLE) || !defined(TCPIP_STACK_DRAM_DIST_ENABLE) 

#endif  // _TCPIP_HEAP_ALLOC_H_

