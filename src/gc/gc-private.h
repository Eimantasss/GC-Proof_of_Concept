#pragma once
#include <stdint.h>
#include <map>
#include <set>
#include "gc.h"

// Maps pointer to it's size
typedef std::map<void*, unsigned> allocations_t;

// Main book keeping struct
struct GC_info
{
	void* stack_start;
	unsigned free_heap;
	unsigned total_heap;
	
	allocations_t allocated;
};


unsigned gc_mark_internal(std::set<void*>& roots, std::set<void*>& reachable);
void gc_sweep_internal(std::set<void*>& reachable);
void gc_batch_free_internal(std::set<void*>& allocations_to_cleanup, allocations_t& all_allocations);
void gc_batch_free_internal(allocations_t& allocations);
void gc_scan_range_internal(void* st, void* end, std::set<void*>& candidates, std::set<void*>& reachable);

inline bool gc_is_allocated_pointer(void *p, GC_info* info)
{
	// all pointers should be aligned
	if (((uintptr_t)p & (sizeof(void*)-1)) != 0)
		return false;
	
	if (info->allocated.find(p) != info->allocated.end())
		return true;
	
	return false;
}

