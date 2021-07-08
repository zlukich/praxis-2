#include <os_malloc.h>
#include <stdlib.h>
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
    mem_block * s = MEM;
    //int siz = MEM_SIZE;
    if(s->free==1 && s->size>=size){
        s->free = 0;
        int siz = s->size;
        s->size = size;
        mem_block* next = (mem_block*) malloc(siz-s->size+sizeof(mem_block));
        next->next = s->next;
        next->size = siz-s->size+sizeof(mem_block);
        next->free = 1;
        s->next = next;
        return s;
    }
    while(s->next!=NULL)
    {
        if(s->free == 1 && s->size>=size){
            s->free = 0;
            int siz = s->size;
            s->size = size;
            mem_block* next= (mem_block*) malloc(siz-s->size+sizeof(mem_block));
            next->next = s->next;
            next->size = size-s->size + sizeof(mem_block);
            next->free = 1;
            s->next = next;
            return s;
        }
        s = s->next;
    }
    
    
	return NULL;
}

void * os_malloc_best_fit(size_t size){
	 mem_block * s = MEM;
    int siz = MEM_SIZE;
    int tempsize = s->size;
    mem_block * temp_s = NULL;
    while(s->next!=NULL){
        if(s->free == 1 && s->size >= size && s->size<=tempsize){
            temp_s = s;
        }
        siz = siz-s->size;
        s = s->next;
    }
    if(temp_s != NULL){
        temp_s->free = 0;
        return temp_s;
    }
    else {return NULL;}
}

void * os_malloc_worst_fit(size_t size){
	// TODO
		 mem_block * s = MEM;
    int siz = MEM_SIZE;
    int tempsize = s->size;
    mem_block * temp_s = NULL;
    while(s->next!=NULL){
        if(s->free == 1 && s->size >= size && s->size>=tempsize){
            temp_s = s;
        }
        siz = siz-s->size;
        s = s->next;
    }
    if(temp_s!=NULL){
        temp_s->free = 0;
        return temp_s;
    }
	return s;
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
	
    if(ptr == NULL){
        return;
    }
    mem_block* s = MEM;
    while(s->next !=NULL){
        if(s->next==ptr)
        {
            if(s->free == 0 && s->next->next!=NULL)
            {
                if(s->next->next->free == 0)
                {
                    s->next->free = 1;
                }
            }
            if((s->free == 0 && s->next->next->free == 1))
            {
                int size = s->next->next->size;
                s->next->next = s->next;
                s->next->next->next->size = s->next->size+ size;
                s->next->free = 0;
            }
            if(s->free == 1 && s->next->next->free == 0)
            {
                int size = s->next->next->size;
                s->next = s->next->next;
                s->next->size = s->size + size;
                s->free = 1;
            }
            if(s->free == 1 && s->next->next->free == 1)
            {
                int size1 = s->next->size;
                int size2 = s->next->next->size;
                s->next = s->next->next->next;
                s->size = s->size+size1+size2;

            }
        }
    }




}
