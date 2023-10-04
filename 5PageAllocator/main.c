// Benjamin Belandres
// This program implements memory management on the heap similar to malloc()


#include <stdio.h>
#include <sys/mman.h>
#include "pageAlloc.h"

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
		mmap(NULL, pages * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, )
	}

	return false;
}

int main() {

	printf("I like chicken and dizza\n");
	
	page_init(1);
	
	return 0;
}
