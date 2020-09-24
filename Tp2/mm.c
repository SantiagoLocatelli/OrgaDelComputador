#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "mm.h"
#include "memlib.h"

#define WSIZE 8                                                                
#define DSIZE 16                                                                 
#define CHUNKSIZE (1<<12)              

static void *coalesce(void* bp);
static char* extend_heap(size_t extendsize);
static void mm_checkheap(int lin);
static char* find_first_fit(size_t asize);
static char* find_fit(size_t asize);
static void place(void* bp,size_t asize);


static char* heap_listp;
static char* last_place;

// Por favor completar.
team_t team = {
    "g1",                   // Número de grupo,
    "Gonzalo Albornoz",     // Nombre integrante 1
    "gjalbornoz@fi.uba.ar", // E-mail integrante 1
    "Santiago Locatelli",   // Nombre integrante 2 (si lo hay)
    "slocatelli@fi.uba.ar"  // E-mail integrante 2 (si lo hay)
};

static unsigned pack(unsigned size, unsigned alloc){
  return (size | alloc);
}

static unsigned get(void* bp){
  return (*(unsigned *) (bp));
}

static void put(void* bp, unsigned value){
  (*(unsigned *) (bp)) = value;
}

static unsigned get_size(void* bp){
  return (get(bp) & ~0x7);
}

static unsigned get_alloc(void* bp){
  return (get(bp) & 0x1);
}

static unsigned max(unsigned x,unsigned y){
  return ((x) > (y)? (x) : (y)) ;
}

static void* get_header(void* bp){
  return (void*)bp - WSIZE;
}

static void* get_footer(void* bp){
  return (void*)bp + get_size(get_header(bp)) - DSIZE;
}

static void* get_next_block(void* bp){
  return (bp) + get_size(((bp) - WSIZE));
}

static void* get_previous_block(void* bp){
  return (bp) - get_size(((bp) - DSIZE));
}

static bool is_free(void* bp){
  return (get_alloc(get_header(bp)) == 0);
}

static bool is_big_enough(void* bp, unsigned size){
  return (get_size(get_header(bp)) >= size);
}

static void *coalesce(void* bp) {
  size_t prev_alloc = get_alloc(get_footer(get_previous_block(bp)));
  size_t next_alloc = get_alloc(get_header(get_next_block(bp)));
  size_t size = get_size(get_header(bp));

  if (prev_alloc && next_alloc) {
    return bp;
  } else if (prev_alloc && !next_alloc) {
    size += get_size(get_header(get_next_block(bp)));
    put(get_header(bp), pack(size, 0));
    put(get_footer(bp), pack(size,0));
  } else if (!prev_alloc && next_alloc) {
    
    size += get_size(get_header(get_previous_block(bp)));
    put(get_footer(bp), pack(size, 0));
    put(get_header(get_previous_block(bp)), pack(size, 0));
    bp = get_previous_block(bp);
  } else { 
    size += get_size(get_header(get_previous_block(bp))) +
    get_size(get_footer(get_next_block(bp)));
    put(get_header(get_previous_block(bp)), pack(size, 0));
    put(get_footer(get_next_block(bp)), pack(size, 0));
    bp = get_previous_block(bp);
  }
  return bp;
}

static char* extend_heap(size_t extendsize){
  char* bp;
  size_t size;
  size = (extendsize % 2) ? (extendsize + 1)*WSIZE : extendsize * WSIZE;
  if ((long)(bp = mem_sbrk(size)) == -1)
  	return NULL;

  put(get_header(bp), pack(size, 0));
  put(get_footer(bp), pack(size, 0));
  put(get_header(get_next_block(bp)), pack(0,1));
  return coalesce(bp);
}

static void mm_checkheap(int line){
  int errorcode = 0;
  for(char* bp = heap_listp; get_size(get_header(bp)) > 0; bp = get_next_block(bp)) {
    if (get_alloc(get_header(bp)) != get_alloc(get_footer(bp))) {
      fprintf(stderr,"Header/Footer allocation info not match\n");
      errorcode = -1;
    }   
    if (get_size(get_header(bp)) != get_size(get_footer(bp))) {
      fprintf(stderr,"Header/Footer size field not match\n");
      errorcode = -1;
    }  
    if (get_size(get_header(bp)) % DSIZE) {
      errorcode = -1;
      fprintf(stderr,"Block is not DSIZE (16 byte) aligned\n");
    }
  }
  if(errorcode == -1){
    fprintf(stderr,"Error detectado en la linea %d\n", line);
    exit(errorcode);
  }
}


/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
    return -1;
  put(heap_listp, 0); 
  put(heap_listp + (1*WSIZE), pack(DSIZE, 1)); 
  put(heap_listp + (2*WSIZE), pack(DSIZE, 1)); 
  put(heap_listp + (3*WSIZE), pack(0, 1)); 
  heap_listp += (2*WSIZE);
  last_place = heap_listp;

  if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
    return -1;
  
  return 0;
}

static char* find_best_fit(size_t asize){
  char* min_bp = heap_listp;
  for(char* bp = heap_listp; get_size(get_header(bp)) > 0; bp = get_next_block(bp)) {
    if(is_free(bp) && is_big_enough(bp,asize) && get_size(get_header(min_bp)) > get_size(get_header(bp))){
      min_bp = bp;
    }
  }
  if(min_bp == heap_listp && !is_free(min_bp)){
    return NULL;
  }
  return min_bp;
}

static char* find_first_fit(size_t asize){

  for(char* bp = heap_listp; get_size(get_header(bp)) > 0; bp = get_next_block(bp)) {
    if(is_free(bp) && is_big_enough(bp,asize)){
      return bp;
    }
  }
  return NULL;
}

static char* find_next_fit(size_t asize){
  for(char* bp = last_place; get_size(get_header(bp)) > 0; bp = get_next_block(bp)) {
    if(is_free(bp) && is_big_enough(bp,asize)){
      last_place = bp;
      return bp;
    }
  }
  if(last_place != heap_listp){
    for(char* bp = heap_listp; bp == last_place; bp = get_next_block(bp)){
      if(is_free(bp) && is_big_enough(bp,asize)){
        last_place = bp;
        return bp;
      }
    }
  }
  return NULL;
}

static char* find_worst_fit(size_t asize){
  char* max_bp = heap_listp;
  for(char* bp = heap_listp; get_size(get_header(bp)) > 0; bp = get_next_block(bp)) {
    if(is_free(bp) && is_big_enough(bp,asize) && get_size(get_header(max_bp)) < get_size(get_header(bp))){
      max_bp = bp;
    }
  }
  if(max_bp == heap_listp && !is_free(max_bp)){
    return NULL;
  }
  return max_bp;
}

static char* find_fit(size_t asize){
  return find_first_fit(asize);
}


static void place(void* bp,size_t asize) {
  size_t csize = get_size(get_header(bp));
  if ((csize - asize) >= (2*DSIZE)) {
    put(get_header(bp), pack(asize, 1));
    put(get_footer(bp), pack(asize, 1));
    bp = get_next_block(bp);
    put(get_header(bp), pack(csize-asize, 0));
    put(get_footer(bp), pack(csize-asize, 0));
  } else {
    put(get_header(bp), pack(csize, 1));
    put(get_footer(bp), pack(csize, 1));
  }
}

/*
 * mm_malloc - Allocate a block.
 */
void *mm_malloc(size_t size) {

    size_t asize;       
    size_t extendsize; 
    char *bp;

    if (size == 0)
        return NULL;

    if (size <= DSIZE)
        asize = 2*DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        mm_checkheap(__LINE__);
        return bp;
    }

    extendsize = max(asize,CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    mm_checkheap(__LINE__);
    return bp;

}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {

    size_t size = get_size(get_header(ptr));
    
    put(get_header(ptr), pack(size, 0));
    put(get_footer(ptr), pack(size, 0));
    coalesce(ptr);
    mm_checkheap(__LINE__);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
  if(ptr == NULL){
    return mm_malloc(size);
  }
  if(size <= 0){
    mm_free(ptr);
    return NULL;
  }
  if(!is_big_enough(ptr,size)){
    mm_free(ptr);
    ptr = mm_malloc(size);
    return ptr;
  }
  return ptr;
}