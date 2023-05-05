#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

STRUCT(allocator_t)
{
	void* mem;
	size_t cap;
	size_t current_index;
};

allocator_t create_allocator(size_t cap)
{
	void* mem = malloc(cap);
	if (mem == NULL)
	{
		bail("Failed to allocate memory");
	}
	return { mem, cap, 0 };
}

size_t allocate(allocator_t* allocator, size_t num_bytes)
{
	if (num_bytes == 0)
	{
		bail("It does not make any sense to allocate 0 bytes");
	}
	if ((allocator->current_index + num_bytes) > allocator->cap)
	{
		allocator->cap = (allocator->cap + num_bytes) * 2;
		allocator->mem = realloc(allocator->mem, allocator->cap);
		if (allocator->mem == NULL)
		{
			bail("Failed to re-allocate memory");
		}
	}

	size_t result = allocator->current_index;
	allocator->current_index += num_bytes;
	return result;
}
