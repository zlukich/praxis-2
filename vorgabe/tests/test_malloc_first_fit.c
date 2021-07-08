#include <stdio.h>

#include "malloc_helpers.h"

// Simple first fit test
const char * str_a = "AAAA";
const char * str_b = "BB";
const char * str_c = "CCCCCCCC";
const char * expected = "AAAABBCCCCCCCC";

#define MEM_SIZE (1000)
static char mem[MEM_SIZE] __attribute__((aligned(8)));

int main() {

	os_init(mem, MEM_SIZE, FIRST_FIT);

	char * ptr = os_malloc(300);

	char * ptr_c = os_malloc(strlen(str_c)+1);
	strcpy(ptr_c, str_c);

	os_free(ptr);

	char * ptr_a = os_malloc(strlen(str_a)+1);
	strcpy(ptr_a, str_a);

	char * ptr_b = os_malloc(strlen(str_b)+1);
	strcpy(ptr_b, str_b);

	char * result = mem_concat_data(mem);

	mem_print_blocks(mem);

	printf("Expected: %s\n", expected);
	printf("Found   : %s\n", result);

	if(strcmp(expected, result) == 0) {
		fprintf(stderr, ">>> Test passed");
	} else {
		fprintf(stderr, ">>> Test failed");
		exit(1);
	}
}
