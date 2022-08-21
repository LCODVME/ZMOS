/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Memory.c
*
* DESCRIPTION:
*     ZMOS memory management.
* AUTHOR:
*     zm
* CREATED DATE:
*     2021/5/16
* REVISION:
*     v0.1
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*
*****************************************************************/
 
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "ZMOS_Types.h"
#include "ZMOS_Config.h"
#include "ZMOS_Memory.h"

#if ZMOS_USE_MEM_MGR
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
#define ZMOS_MEM_ALIGN_SIZE     ZMOS_ALIGN_SIZE
     
#define ZMOS_HEAP_MAGIC         0x1EA0

#define ZMOS_MIN_SIZE           12

#define ZMOS_ALIGN_GET(size)    ZMOS_ALIGN(size, ZMOS_MEM_ALIGN_SIZE)

#define MIN_SIZE_ALIGNED        ZMOS_ALIGN(ZMOS_MIN_SIZE, ZMOS_MEM_ALIGN_SIZE)
#define MEM_STRUCT_SIZE         ZMOS_ALIGN(sizeof(zmosMem_t), ZMOS_MEM_ALIGN_SIZE)


#define ZMOS_MEM_ASSERT(EX)     \
if(!(EX))                       \
{                               \
    while(1);                   \
}
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/

typedef struct zmosMem
{
    zm_uint16_t magic;
    zm_uint16_t used;
    zm_size_t prev;
    zm_size_t next;
}zmosMem_t;

typedef struct
{
    //zm_size_t surpSize;
    zm_size_t usedSize;
    zm_size_t maxSize;
}zmosMemStats_t;
/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/
#if ZMOS_MEM_USE_HEAP
     
#define ZMOS_MEM_HEAP_BEGIN         ZMOS_HEAP_BEGIN
#define ZMOS_MEM_HEAP_END           ZMOS_HEAP_END
     
/*#if (ZMOS_MEM_HEAP_END <= ZMOS_MEM_HEAP_BEGIN)
#error "Error heap addr"
#endif*/
     
#else
static zm_uint8_t zmos_pool[ZMOS_MEM_SIZE];
#endif

/** pointer to the heap: for alignment, heap_ptr is now a pointer instead of an array */
static zm_uint8_t *zmosMemHeap;
/** the last entry, always unused! */
static zmosMem_t *zmosMemEnd;
/** pointer to the lowest free block */
static zmosMem_t *lfree;

static zm_size_t zmosMemSize;

#if ZMOS_MEM_STATS
static zmosMemStats_t memStats;
#endif
/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
static void zmos_mem_free(void *ptr);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/

static void zmos_putTogether(zmosMem_t *pMem)
{
    zmosMem_t *nextMem;
    zmosMem_t *prevMem;
    
    nextMem = (zmosMem_t *)&zmosMemHeap[pMem->next];
    
    if(nextMem->magic == ZMOS_HEAP_MAGIC && nextMem != pMem &&
       nextMem->used == 0 && nextMem != zmosMemEnd)
    {
        if(lfree == nextMem)
        {
            lfree = pMem;
        }
        pMem->next = nextMem->next;
        ((zmosMem_t *)&zmosMemHeap[nextMem->next])->prev = (zm_uint8_t *)pMem - zmosMemHeap;
    }
    
    prevMem = (zmosMem_t *)&zmosMemHeap[pMem->prev];
    
    if(prevMem->magic == ZMOS_HEAP_MAGIC &&
       nextMem != pMem && prevMem->used == 0)
    {
        if(lfree == pMem)
        {
            lfree = prevMem;
        }
        prevMem->next = pMem->next;
        ((zmosMem_t *)&zmosMemHeap[pMem->next])->prev = (zm_uint8_t *)prevMem - zmosMemHeap;
    }
}

/*****************************************************************
* FUNCTION: zmos_mem_init
*
* DESCRIPTION: 
*     ZMOS dynamic memory init.
* INPUTS:
*     beginAddr : The beginning address of system heap memory.
*     endAddr   : The end address of system heap memory.
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
static void zmos_mem_init(void *beginAddr, void *endAddr)
{
    zmosMem_t *pMem;
    
    zm_size_t beginAlign = ZMOS_ALIGN((zm_size_t)beginAddr, ZMOS_MEM_ALIGN_SIZE);
    zm_size_t endAlign = ZMOS_ALIGN_DOWN((zm_size_t)endAddr, ZMOS_MEM_ALIGN_SIZE);
    
    if(endAlign > (2 * MEM_STRUCT_SIZE) &&
       (endAlign - 2 * MEM_STRUCT_SIZE) >= beginAlign)
    {
        zmosMemSize = endAlign - beginAlign - 2 * MEM_STRUCT_SIZE;
    }
    else
    {
        //memory error begin address and end address.
        return;
    }
    
    zmosMemHeap = (zm_uint8_t *)beginAlign;
    
    pMem = (zmosMem_t *)zmosMemHeap;
    pMem->magic = ZMOS_HEAP_MAGIC;
    pMem->used = 0;
    pMem->next = zmosMemSize + MEM_STRUCT_SIZE;
    pMem->prev = 0;
    
    zmosMemEnd = (zmosMem_t *)&zmosMemHeap[pMem->next];
    zmosMemEnd->magic = ZMOS_HEAP_MAGIC;
    zmosMemEnd->used = 1;
    zmosMemEnd->next = zmosMemSize + MEM_STRUCT_SIZE;
    zmosMemEnd->prev = zmosMemSize + MEM_STRUCT_SIZE;
    
    lfree = pMem;

#if ZMOS_MEM_STATS
    memStats.maxSize = 0;
    memStats.usedSize = 0;
    //memStats.surpSize = zmosMemSize;
#endif
}

/*****************************************************************
* FUNCTION: zmos_mem_malloc
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation.
* INPUTS:
*     size : The number of bytes to allocate from the HEAP.
* RETURNS:
*     The first address of the allocated memory space.
*     NULL : faild, It may be out of memory.
* NOTE:
*     null
*****************************************************************/
static void *zmos_mem_malloc(zm_size_t size)
{
    zm_size_t idx;
    zmosMem_t *pMem;
    
    if(size == 0) return NULL;
    
    size = ZMOS_ALIGN_GET(size);
    
    if(size > zmosMemSize) return NULL;
    
    if(size < MIN_SIZE_ALIGNED) size = MIN_SIZE_ALIGNED;
    
    for(idx = (zm_uint8_t *)lfree - zmosMemHeap;
        idx < (zmosMemSize - size);
        idx = ((zmosMem_t *)&zmosMemHeap[idx])->next)
    {
        pMem = (zmosMem_t *)&zmosMemHeap[idx];
        
        if(!pMem->used && (pMem->next - idx - MEM_STRUCT_SIZE) >= size)
        {
            zmosMem_t *mem;
            if((pMem->next - idx - MEM_STRUCT_SIZE) >= (size + MEM_STRUCT_SIZE + MIN_SIZE_ALIGNED))
            {
                zm_size_t ptr = idx + MEM_STRUCT_SIZE + size;
                
                mem = (zmosMem_t *)&zmosMemHeap[ptr];
                mem->magic = ZMOS_HEAP_MAGIC;
                mem->used = 0;
                mem->next = pMem->next;
                mem->prev = idx;
                
                pMem->next = ptr;
                pMem->used = 1;
                
                if(mem->next != (zmosMemSize + MEM_STRUCT_SIZE))
                {
                    ((zmosMem_t *)&zmosMemHeap[mem->next])->prev = ptr;
                }
#if ZMOS_MEM_STATS
                memStats.usedSize += (size + MEM_STRUCT_SIZE);
                if(memStats.maxSize < memStats.usedSize)
                {
                    memStats.maxSize = memStats.usedSize;
                }
#endif
            }
            else
            {
                pMem->used = 1;
#if ZMOS_MEM_STATS
                memStats.usedSize += (pMem->next - idx);
                if(memStats.maxSize < memStats.usedSize)
                {
                    memStats.maxSize = memStats.usedSize;
                }
#endif
            }
            pMem->magic = ZMOS_HEAP_MAGIC;
            
            if(pMem == lfree)
            {
                while(lfree->used && lfree != zmosMemEnd)
                {
                    lfree = (zmosMem_t *)&zmosMemHeap[lfree->next];
                }
                
                ZMOS_MEM_ASSERT(lfree == zmosMemEnd || !lfree->used);
            }
            
            return (zm_uint8_t *)pMem + MEM_STRUCT_SIZE;
        }
    }
    return NULL;
}

/*****************************************************************
* FUNCTION: zmos_mem_realloc
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation.
* INPUTS:
*     ptr : pointer to memory allocated by zmos_mem_malloc.
*     newsize : The number of new size to allocate from the HEAP.
* RETURNS:
*     The first address of the allocated memory space.
*     NULL : faild, It may be out of memory.
* NOTE:
*     null
*****************************************************************/
static void *zmos_mem_realloc(void *ptr, zm_size_t newsize)
{
    zm_size_t idx;
    zm_size_t size;
    zmosMem_t *pMem;
    void *newMem;
    
    newsize = ZMOS_ALIGN_GET(newsize);
    
    if(newsize > zmosMemSize) return NULL;
    
    if(newsize == 0)
    {
        zmos_mem_free(ptr);
        return NULL;
    }
    
    if(ptr == NULL) return zmos_mem_malloc(newsize);
    
    if((zm_uint8_t *)ptr < (zm_uint8_t *)zmosMemHeap ||
       (zm_uint8_t *)ptr >= (zm_uint8_t *)zmosMemEnd)
    {
        //illegal memory
        return ptr;
    }
    
    pMem = (zmosMem_t *)((zm_uint8_t *)ptr - MEM_STRUCT_SIZE);
    
    idx = (zm_uint8_t *)pMem - zmosMemHeap;
    size = pMem->next - idx - MEM_STRUCT_SIZE;
    
    if(size == newsize)
    {
        return ptr;
    }
    
    if((newsize + MEM_STRUCT_SIZE + MIN_SIZE_ALIGNED) < size)
    {
        zm_size_t idx2;
        zmosMem_t *mem;
        
        idx2 = idx + MEM_STRUCT_SIZE + size;
        mem = (zmosMem_t *)&zmosMemHeap[idx2];
        mem->magic = ZMOS_HEAP_MAGIC;
        mem->used = 0;
        mem->next = pMem->next;
        mem->prev = idx;
        
        pMem->next = idx2;
        
        if(mem->next != (zmosMemSize + MEM_STRUCT_SIZE))
        {
            ((zmosMem_t *)&zmosMemHeap[mem->next])->prev = idx2;
        }
#if ZMOS_MEM_STATS
        memStats.usedSize -= (size - newsize);
#endif
        if(mem < lfree) lfree = mem;
        
        zmos_putTogether(mem);
        
        return ptr;
    }
    
    newMem = zmos_mem_malloc(newsize);
    
    if(newMem)
    {
        memcpy(newMem, ptr, size < newsize ? size : newsize);
        zmos_mem_free(ptr);
    }
        
    return newMem;
}
/*****************************************************************
* FUNCTION: zmos_mem_calloc
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation.
*     This function will contiguously allocate enough space for count objects
*     that are size bytes of memory each and returns a pointer to the allocated.
* memory.
* INPUTS:
*     count : Number of objects to allocate.
*     size : The number of size to allocate from the HEAP.
* RETURNS:
*     The first address of the allocated memory space.
*     NULL : faild, It may be out of memory.
* NOTE:
*     null
*****************************************************************/
void *zmos_mem_calloc(zm_size_t count, zm_size_t size)
{
    void *ptr;
    
    ptr = zmos_mem_malloc(count * size);
    
    if(ptr) memset(ptr, 0, count * size);
    
    return ptr;
}
/*****************************************************************
* FUNCTION: zmos_mem_free
*
* DESCRIPTION: 
*       ZMOS dynamic memory de-allocation.
* INPUTS:
*     ptr : The first address assigned by zmos_mem_malloc().
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
static void zmos_mem_free(void *ptr)
{
    zmosMem_t *pMem;
    
    if(ptr == NULL) return;
    
    if((zm_uint8_t *)ptr < (zm_uint8_t *)zmosMemHeap ||
       (zm_uint8_t *)ptr >= (zm_uint8_t *)zmosMemEnd)
    {
        //illegal memory
        return;
    }
    
    pMem = (zmosMem_t *)((zm_uint8_t *)ptr - MEM_STRUCT_SIZE);
    
    if(pMem->magic != ZMOS_HEAP_MAGIC || !pMem->used)
    {
        ZMOS_MEM_ASSERT(0);
        //return;
    }
    pMem->used = 0;
    
    if(pMem < lfree) lfree = pMem;
    
#if ZMOS_MEM_STATS
    memStats.usedSize -= (pMem->next - ((zm_uint8_t *)pMem - zmosMemHeap));
#endif
    
    zmos_putTogether(pMem);
}

/*****************************************************************
* FUNCTION: zmos_memoryMgrInit
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation init.
* INPUTS:
*     null
* RETURNS:
*     null.
* NOTE:
*     null
*****************************************************************/
void zmos_memoryMgrInit(void)
{
#if ZMOS_MEM_USE_HEAP
    zmos_mem_init((void *)ZMOS_MEM_HEAP_BEGIN, (void *)ZMOS_MEM_HEAP_END);
#else
    zmos_mem_init((void *)&zmos_pool[0], (void *)((zm_uint8_t *)&zmos_pool[ZMOS_MEM_SIZE - 1]));
#endif
}
/*****************************************************************
* FUNCTION: zmos_malloc
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation.
* INPUTS:
*     size : The number of bytes to allocate from the HEAP.
* RETURNS:
*     The first address of the allocated memory space.
*     NULL : faild, It may be out of memory.
* NOTE:
*     null
*****************************************************************/
void *zmos_malloc(zm_size_t size)
{
    return zmos_mem_malloc(size);
}
/*****************************************************************
* FUNCTION: zmos_realloc
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation.
*     This function will change the previously allocated memory block.
* INPUTS:
*     ptr : pointer to memory allocated by zmos_mem_malloc.
*     newsize : The number of new size to allocate from the HEAP.
* RETURNS:
*     The first address of the allocated memory space.
*     NULL : faild, It may be out of memory.
* NOTE:
*     null
*****************************************************************/
void *zmos_realloc(void *ptr, zm_size_t newsize)
{
    return zmos_mem_realloc(ptr, newsize);
}
/*****************************************************************
* FUNCTION: zmos_mem_calloc
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation.
*     This function will contiguously allocate enough space for count objects
*     that are size bytes of memory each and returns a pointer to the allocated.
* memory.
* INPUTS:
*     count : Number of objects to allocate.
*     size : The number of size to allocate from the HEAP.
* RETURNS:
*     The first address of the allocated memory space.
*     NULL : faild, It may be out of memory.
* NOTE:
*     null
*****************************************************************/
void *zmos_calloc(zm_size_t count, zm_size_t size)
{
    return zmos_mem_calloc(count, size);
}
/*****************************************************************
* FUNCTION: zmos_free
*
* DESCRIPTION: 
*       ZMOS dynamic memory de-allocation.
* INPUTS:
*     ptr : The first address assigned by zmos_malloc().
* RETURNS:
*     null
* NOTE:
*     null
*****************************************************************/
void zmos_free(void *ptr)
{
    zmos_mem_free(ptr);
}
/*****************************************************************
* FUNCTION: zmos_getMemTotal
*
* DESCRIPTION: 
*       Get zmos memory management total size.
* INPUTS:
*     null
* RETURNS:
*     memory total size.
* NOTE:
*     null
*****************************************************************/
zm_size_t zmos_getMemTotal(void)
{
    return zmosMemSize;
}
/*****************************************************************
* FUNCTION: zmos_getMemUsed
*
* DESCRIPTION: 
*       Get zmos memory management used size.
* INPUTS:
*     null
* RETURNS:
*     memory used size.
* NOTE:
*     If no set ZMOS_MEM_STATS to 1, It always returns 0.
*****************************************************************/
zm_size_t zmos_getMemUsed(void)
{
#if ZMOS_MEM_STATS
    return memStats.usedSize;
#else
    return 0;
#endif
}
/*****************************************************************
* FUNCTION: zmos_getMemMaxUsed
*
* DESCRIPTION: 
*       Get zmos memory management max used size.
* INPUTS:
*     null
* RETURNS:
*     memory max used size.
* NOTE:
*     If no set ZMOS_MEM_STATS to 1, It always returns 0.
*****************************************************************/
zm_size_t zmos_getMemMaxUsed(void)
{
#if ZMOS_MEM_STATS
    return memStats.maxSize;
#else
    return 0;
#endif
}

#else

/*****************************************************************
* FUNCTION: zmos_malloc
*
* DESCRIPTION: 
*     ZMOS dynamic memory allocation.
* INPUTS:
*     size : The number of bytes to allocate from the HEAP.
* RETURNS:
*     The first address of the allocated memory space.
*     NULL : faild, It may be out of memory.
* NOTE:
*     It's weak functions, you can redefine it.
*****************************************************************/
__weak void *zmos_malloc(zm_size_t size)
{
    return malloc(size);
}
/*****************************************************************
* FUNCTION: zmos_free
*
* DESCRIPTION: 
*       ZMOS dynamic memory de-allocation.
* INPUTS:
*     ptr : The first address assigned by zmos_mem_malloc().
* RETURNS:
*     null
* NOTE:
*     It's weak functions, you can redefine it.
*****************************************************************/
__weak void zmos_free(void *ptr)
{
    free(ptr);
}
/*****************************************************************
* FUNCTION: zmos_getMemTotal
*
* DESCRIPTION: 
*       Get zmos memory management total size.
* INPUTS:
*     null
* RETURNS:
*     memory total size.
* NOTE:
*     null
*****************************************************************/
__weak zm_size_t zmos_getMemTotal(void)
{
    return 0;
}
/*****************************************************************
* FUNCTION: zmos_getMemUsed
*
* DESCRIPTION: 
*       Get zmos memory management used size.
* INPUTS:
*     null
* RETURNS:
*     memory used size.
* NOTE:
*     
*****************************************************************/
__weak zm_size_t zmos_getMemUsed(void)
{
    return 0;
}
/*****************************************************************
* FUNCTION: zmos_getMemMaxUsed
*
* DESCRIPTION: 
*       Get zmos memory management max used size.
* INPUTS:
*     null
* RETURNS:
*     memory max used size.
* NOTE:
*     
*****************************************************************/
__weak zm_size_t zmos_getMemMaxUsed(void)
{
    return 0;
}
#endif
/****************************************************** END OF FILE ******************************************************/
