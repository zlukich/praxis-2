#include <os_malloc.h>

static void * MEM;
static size_t MEM_SIZE;
static alloc_type ALLOC_TYPE;

void os_init(void * mem, size_t mem_size, alloc_type type){
	MEM = mem;
	MEM_SIZE = mem_size;
	ALLOC_TYPE = type;

	/* Beginning of memory is also where the first block is located */
	mem_block * beginning = MEM;

	/* Initialize first block
	 * Currently its the only block and entire memory available
	 */
	beginning->next = NULL;
	beginning->free = 1;
	beginning->size = MEM_SIZE;
}

void * os_malloc_first_fit(size_t size){
	// TODO
	return NULL;
}

void * os_malloc_best_fit(size_t size){
	// TODO
	return NULL;
}

void * os_malloc_worst_fit(size_t size){
	// TODO
	return NULL;
}

void * os_malloc(size_t size) {
	switch(ALLOC_TYPE) {
		case FIRST_FIT: return os_malloc_first_fit(size);
		case BEST_FIT: return os_malloc_best_fit(size);
		case WORST_FIT: return os_malloc_worst_fit(size);
		default: return NULL;
	}
}

void os_free(void *ptr){
	// TODO
}
