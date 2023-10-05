// Benjamin Belandres
// This program implements memory management on the heap similar to malloc()


#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include "pageAlloc.h"

void *memoryPool;
int numPages;

bool page_init(int pages) {
	const int PAGE_SIZE = 4096;		// How large a single page is in bytes
	const int MAX_PAGES = 262144;	// The maximum amount of pages allowed on the machine

	// The user must ask for more than 1 page and less than MAX_PAGES
	if (pages < 2) {
		return false;
	}
	else if (pages > MAX_PAGES) {
		return false;
	}
	else {
		memoryPool = mmap(NULL, pages * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
		if (memoryPool == MAP_FAILED) {	// MAP_FAILED is just (void *)-1
			perror("Couldn't map memory");
			return false;
		}
		
		numPages = pages;
		return true;
	}
}

void page_deinit(void) {
	const int PAGE_SIZE = 4096;		// How large a single page is in bytes
	if (munmap(memoryPool, numPages * PAGE_SIZE) == -1) {
		perror("Couldn't unmap memory");
	}
}

int main() {

	printf("I like chicken and dizza\n");
	
	page_init(4);
	page_deinit();
	
	return 0;
}
