// Benjamin Belandres
// This program implements memory management on the heap similar to malloc()


#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include "pageAlloc.h"

#define PAGE_SIZE 4096		// How large a single page is in bytes
#define MAX_PAGES 262144	// The maximum amount of pages allowed on the machine

void *memoryPool;	// Holds the address where the memory has been mapped
int numPages;		// Holds the amount of pages the user has chosen to make

static int calcMappedBytes(const int pages) {
		
		const int pagesSize = pages * PAGE_SIZE;	// How many bytes are going to be used for the pages
		const int bookkeepingBytes = ((pages - 1) / 4) + 1;	// How many bytes should actively be used to track the pages
		const int bookkeepingPadded = ((bookkeepingBytes + (PAGE_SIZE - 1)) / PAGE_SIZE) * PAGE_SIZE;	// Aligned to the nearest page
		
		printf("The pages begin at %d: Total of %d bytes initialized for the pages\n", bookkeepingPadded, pagesSize);
		printf("%d bytes total\n", bookkeepingPadded + pagesSize);

		return pagesSize + bookkeepingPadded;
}

// Initializes the memory with the amount of pages passed to it
// It also compensates for bookkeeping bytes to keep track
bool page_init(int pages) {

	// The user must ask for more than 1 page and less than MAX_PAGES
	if (pages < 2) {
		return false;
	}
	else if (pages > MAX_PAGES) {
		return false;
	}
	else {

		// I'm not sure if we should MAP_PRIVATE or MAP_SHARED, but we only have one process running so I'll assume MAP_PRIVATE
		memoryPool = mmap(NULL, calcMappedBytes(pages), PROT_READ | PROT_WRITE, 
						  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);	
		
		if (memoryPool == MAP_FAILED) {	// MAP_FAILED is just (void *)-1
			perror("Couldn't map memory");
			return false;
		}
		
		numPages = pages;
		return true;
	}
}

// Unmaps the memory initialized by page_init()
void page_deinit(void) {
	if (munmap(memoryPool, calcMappedBytes(numPages)) == -1) {
		perror("Couldn't unmap memory");
	}
}

void *page_alloc(int pages) {
	(void) pages;
	return 0;
}

int main() {

	printf("I like chicken and dizza\n");
	
	page_init(3);
	page_deinit();
	
	return 0;
}
