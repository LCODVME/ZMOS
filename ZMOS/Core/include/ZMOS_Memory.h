/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* ZMOS_Memory.h
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
#ifndef __ZMOS_MEMORY_H__
#define __ZMOS_MEMORY_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/ 
    
/*    
#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$RW_IRAM1$ZI$Limit;
#define HEAP_BEGIN      ((void *)&Image$RW_IRAM1$ZI$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      ((void *)&__bss_end)
#endif*/
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/

/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/

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
void zmos_memoryMgrInit(void);
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
void *zmos_malloc(zm_size_t size);
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
void *zmos_realloc(void *ptr, zm_size_t newsize);
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
void *zmos_calloc(zm_size_t count, zm_size_t size);
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
void zmos_free(void *ptr);
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
zm_size_t zmos_getMemTotal(void);
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
zm_size_t zmos_getMemUsed(void);
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
zm_size_t zmos_getMemMaxUsed(void);



#ifdef __cplusplus
}
#endif
#endif /* ZMOS_Memory.h */
