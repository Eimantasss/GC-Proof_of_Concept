#include <stdio.h>

#include "test.h"
#include "gc/gc.h"

typedef struct node
{
	int data;
	struct node* next;
} node_t;

void run_basic_allocation_test(void);
void run_heap_exhaust_test(void);
void run_linked_list_test(void);

void run_tests(void)
{
	run_basic_allocation_test();
	run_heap_exhaust_test();
	run_linked_list_test();
}

void run_basic_allocation_test(void)
{
	clock_t st = clock();
	
	printf("Basic allocation test\n");
	printf("GC heap before allocations: %u of %u bytes free\n", gc_get_free_heap(), gc_get_total_heap());
	
	void* p = gc_alloc(10);
	
	printf("GC heap after allocations: %u of %u bytes free\n", gc_get_free_heap(), gc_get_total_heap());
	
	gc_collect();
	
	printf("GC heap after garbage collection: %u of %u bytes free\n", gc_get_free_heap(), gc_get_total_heap());
	
	printf("Basic allocation test took %gs of CPU time\n", gc_debug_get_duration(st));
}

void run_heap_exhaust_test(void)
{
	clock_t st = clock();
	
	printf("Heap exhaust test\n");
	
	const unsigned block_size = 10;
	unsigned alloc_count = (gc_get_free_heap() / block_size);
	
	printf("GC heap before allocations: %u of %u bytes free\n", gc_get_free_heap(), gc_get_total_heap());
	
	void *p = NULL;
	for (unsigned i = 0; i < alloc_count; i++)
		p = gc_alloc(block_size);
	
	printf("GC heap after allocations: %u of %u bytes free\n", gc_get_free_heap(), gc_get_total_heap());
	
	gc_collect();
	
	printf("Pointer %p has survived collection: %s\n", p, gc_debug_get_allocation_size(p) > 0 ? "true" : "false");
	
	printf("GC heap after garbage collection: %u of %u bytes free\n", gc_get_free_heap(), gc_get_total_heap());
	
	printf("Heap exhaust test took %gs of CPU time\n", gc_debug_get_duration(st));
}

void run_linked_list_test(void)
{
	clock_t st = clock();
	
	printf("Linked list test\n");
	
	gc_collect();
	
	printf("GC heap after garbage collection: %u of %u bytes free\n", gc_get_free_heap(), gc_get_total_heap());
	
	node_t* head = (node_t*)gc_alloc(sizeof(node_t));
	head->next = (node_t*)gc_alloc(sizeof(node_t));
	
	printf("GC heap after allocations: %u of %u bytes free\n", gc_get_free_heap(), gc_get_total_heap());
	
	gc_collect();
	
	printf("Pointer head %p has survived collection: %s\n", head, gc_debug_get_allocation_size(head) > 0 ? "true" : "false");
	printf("Pointer head->next %p has survived collection: %s\n", head->next, gc_debug_get_allocation_size(head->next) > 0 ? "true" : "false");
	
	printf("GC heap after garbage collection: %u of %u bytes free\n", gc_get_free_heap(), gc_get_total_heap());
	
	printf("Linked list test took %gs of CPU time\n", gc_debug_get_duration(st));
}
