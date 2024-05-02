/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "team1",
    /* First member's full name */
    "MinjungKim",
    /* First member's email address */
    "91minjung@gmail.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

// 초기 설정값
#define WORD 4
#define DWORD (2*WORD)
#define CHUNKSIZE (1<<12)
#define MAX(x, y)   ((x) > y ? (x) : (y))
#define PACK(size, alloc)     ((size) | (alloc))
#define GET(p)      (*(unsigned int *) (p))
#define PUT(p, val)     (*(unsigned int *) (p) = (val))
#define GET_SIZE(p)     (GET(p) & ~0x7)
#define GET_ALLOC(p)    (GET(p) & 0x1)

#define HP(bp)      ((char *)(bp) - WORD)
#define FP(bp)      ((char *)(bp) + GET_SIZE(HP(bp)) -DWORD)

#define NEXT_BLOCK(bp)      ((char *)(bp) + GET_SIZE(((char *)(bp) - WORD)))
#define PREV_BLOCK(bp)      ((char *)(bp) - GET_SIZE(((char *)(bp) - DWORD)))


/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8


/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

int mm_init(void);
void *mm_malloc(size_t size);
void mm_free(void *ptr);
void *mm_realloc(void *ptr, size_t size);
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
void *find_firstfit(size_t asize);
void place(void *bp, size_t asize);

static char* heap_listp;
static char* last_alloc;
static char* headbp;
void *find_nextfit(size_t asize);
void *find_bestfit(size_t asize);

static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    size = (words % 2) ? (words+1) * WORD : words * WORD;
    if ((long)(bp = mem_sbrk(size)) == -1) return NULL;

    PUT(HP(bp), PACK(size, 0));
    PUT(FP(bp), PACK(size, 0));
    PUT(HP(NEXT_BLOCK(bp)), PACK(0,1));

    return coalesce(bp);
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4*WORD)) == (void *) -1) return -1;
    PUT(heap_listp, 0);
    PUT(heap_listp + 1*WORD, PACK(DWORD,1));
    PUT(heap_listp + 2*WORD, PACK(DWORD,1));
    PUT(heap_listp + 3*WORD, PACK(0,1));
    heap_listp += DWORD;
    last_alloc = heap_listp;
    if (extend_heap(CHUNKSIZE/WORD) == NULL) return -1;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    char *bp;

    if (size == 0) return NULL;

    if (size < DWORD) asize = 2*DWORD;
    else asize = DWORD * ((size + (DWORD) +(DWORD-1)) / DWORD);

    if ((bp = find_bestfit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WORD)) == NULL) return NULL;

    place(bp, asize);
    return bp;
}


// void *mm_malloc(size_t size)
// {
//     int newsize = ALIGN(size + SIZE_T_SIZE);
//     void *p = mem_sbrk(newsize);
//     if (p == (void *)-1)
// 	return NULL;
//     else {
//         *(size_t *)p = size;
//         return (void *)((char *)p + SIZE_T_SIZE);
//     }
// }

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
    size_t size = GET_SIZE(HP(bp));
    char **freed;
    PUT(HP(bp), PACK(size, 0));
    PUT(FP(bp), PACK(size, 0));
    freed = coalesce(bp);
    *freed = headbp;
    headbp = freed;
    return freed;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */

void *mm_realloc(void *bp, size_t size)
{
    void *oldbp = bp;
    void *newbp;
    size_t copySize;
    
    if ((newbp = mm_malloc(size)) == NULL) return NULL;

    copySize = GET_SIZE(HP(bp));
    if (size < copySize) copySize = size;
    memcpy(newbp, oldbp, copySize);
    mm_free(oldbp);
    return newbp;
}


// void *mm_realloc(void *ptr, size_t size)
// {
//     void *oldptr = ptr;
//     void *newptr;
//     size_t copySize;
    
//     newptr = mm_malloc(size);
//     if (newptr == NULL)
//       return NULL;
//     copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
//     if (size < copySize)
//       copySize = size;
//     memcpy(newptr, oldptr, copySize);
//     mm_free(oldptr);
//     return newptr;
// }

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FP(PREV_BLOCK(bp)));
    size_t next_alloc = GET_ALLOC(HP(NEXT_BLOCK(bp)));
    size_t size = GET_SIZE(HP(bp));

    if (prev_alloc && next_alloc) return bp; // case 1

    else if (prev_alloc && !next_alloc) { // case 2
        size += GET_SIZE(HP(NEXT_BLOCK(bp)));
        PUT(HP(bp), size);
        PUT(FP(bp), size);
    }

    else if (!prev_alloc && next_alloc) { // case 3
        size += GET_SIZE(HP(PREV_BLOCK(bp)));
        PUT(FP(bp), size);
        PUT(HP(PREV_BLOCK(bp)), size);
        bp = PREV_BLOCK(bp);
    }
    
    else {                              // case 4
        size += GET_SIZE(HP(PREV_BLOCK(bp))) + GET_SIZE(HP(NEXT_BLOCK(bp)));
        PUT(HP(PREV_BLOCK(bp)), size);
        PUT(FP(NEXT_BLOCK(bp)), size);
        bp = PREV_BLOCK(bp);
    }
    return bp;
}

void *find_firstfit(size_t asize)
{
    char *bp = NEXT_BLOCK(heap_listp);
    while (GET(HP(bp)) != 1) {
        if((GET_ALLOC(HP(bp)) == 0) && (GET_SIZE(HP(bp)) >= asize)) return bp;
        else bp = NEXT_BLOCK(bp);
    }

    return NULL;
}

void *find_nextfit(size_t asize)
{
    char *bp = last_alloc;
    while (GET(HP(bp)) != 1) {
        if((GET_ALLOC(HP(bp)) == 0) && (GET_SIZE(HP(bp)) >= asize)) return bp;
        else bp = NEXT_BLOCK(bp);
    }

    bp = heap_listp;
    while (bp != last_alloc && GET(HP(bp)) != 1) {
        if((GET_ALLOC(HP(bp)) == 0) && (GET_SIZE(HP(bp)) >= asize)) return bp;
        else bp = NEXT_BLOCK(bp);
    }

    return NULL;
}

void *find_bestfit(size_t asize)
{
    char *bp = NEXT_BLOCK(heap_listp);
    void *best = NULL;
    while (GET(HP(bp)) != 1) {
        if((GET_ALLOC(HP(bp)) == 0) && (GET_SIZE(HP(bp)) >= asize)) {
            if (best == NULL) best = bp;
            else if (GET_SIZE(HP(best)) > GET_SIZE(HP(bp))) best = bp;
        }
        bp = NEXT_BLOCK(bp);
    }
    return best;
}

void place(void *bp, size_t asize)
{
    last_alloc = bp;
    size_t o_size = GET(HP(bp));
    if (asize <= o_size - 2*DWORD) {
        PUT(HP(bp), PACK(asize,1));
        PUT(FP(bp), PACK(asize,1));
        bp = NEXT_BLOCK(bp);
        PUT(HP(bp), PACK(o_size-asize,0));
        PUT(FP(bp), PACK(o_size-asize,0));
    }
    else {
        *HP(bp) += 1;
        *FP(bp) += 1;
    }
}