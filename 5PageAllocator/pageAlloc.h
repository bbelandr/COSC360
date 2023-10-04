#pragma once
#include <stdbool.h>

// Allocator functions
bool page_init(int pages);
void page_deinit(void);

// Allocation functions
void *page_alloc(int pages);
void  page_free(void *addr);

// Status functions
int pages_taken(void);
int pages_free(void);