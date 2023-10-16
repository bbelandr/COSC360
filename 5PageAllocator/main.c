// Benjamin Belandres
// This program implements memory management similar to malloc()
// Something that is worth noting:
	// I have made page_alloc() initialize the memory that it passes to the user to 0
	// I wasn't sure if this was the desired behavior, but I decided that there would
	// be no harm done if I did it.
		// I used memset() to initialize the memory to 0
// Otherwise, there are a plethora of static helper functions that I have used for
// the calculation of memory sizes and also bit setting.

#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h> // For memset and initializing the allocated pages to 0
#include "pageAlloc.h"

#define PAGE_SIZE 4096	 // How large a single page is in bytes
#define MAX_PAGES 262144 // The maximum amount of pages allowed on the machine

void *memoryPool; // Holds the address where the memory has been mapped
int numPages;	  // Holds the amount of pages the user has chosen to make

// The bit tester functions:
static bool test_bit(char bitset, int index)
{
	return (bitset >> index) & 1;
}

static void set_bit(char *bitset, int index)
{
	*bitset |= 1 << index;
}

static void clear_bit(char *bitset, int index)
{
	*bitset &= ~(1 << index);
}

// Page Allocator functions:
// Returns the amount of bytes taken up by the pages ONLY
static int calcPagesSize(const int pages)
{
	return pages * PAGE_SIZE;
}

// Returns the amount of bytes taken up by the used section of the bookkeeping pages
static int calcBookkeepingBytes(const int pages)
{
	return ((pages - 1) / 4) + 1;
}

// Returns the total amount of bytes taken up by the whole bookkeeping section
static int calcBookkeepingPadded(const int pages)
{
	return ((calcBookkeepingBytes(pages) + (PAGE_SIZE - 1)) / PAGE_SIZE) * PAGE_SIZE;
}

// Returns the total amount of bytes that have been mapped, including the bookkeeping region
static int calcMappedBytes(const int pages)
{

	const int pagesSize = calcPagesSize(pages); // How many bytes are going to be used for the pages
	// const int bookkeepingBytes = calcBookkeepingBytes(pages);	// How many bytes should actively be used to track the pages
	const int bookkeepingPadded = calcBookkeepingPadded(pages); // Aligned to the nearest page

	printf("The pages begin at %d: Total of %d bytes initialized for the pages\n", bookkeepingPadded, pagesSize);
	printf("%d bytes total\n", bookkeepingPadded + pagesSize);

	return pagesSize + bookkeepingPadded;
}

// Initializes the memory with the amount of pages passed to it
// It also compensates for bookkeeping bytes to keep track
bool page_init(int pages)
{

	// The user must ask for more than 1 page and less than MAX_PAGES
	if (pages < 2)
	{
		return false;
	}
	else if (pages > MAX_PAGES)
	{
		return false;
	}
	else
	{

		// I'm not sure if we should MAP_PRIVATE or MAP_SHARED, but we only have one process running so I'll assume MAP_PRIVATE
		memoryPool = mmap(NULL, calcMappedBytes(pages), PROT_READ | PROT_WRITE,
						  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

		if (memoryPool == MAP_FAILED)
		{ // MAP_FAILED is just (void *)-1
			perror("Couldn't map memory");
			return false;
		}

		numPages = pages;
		return true;
	}
}

// Unmaps the memory initialized by page_init()
void page_deinit(void)
{
	if (munmap(memoryPool, calcMappedBytes(numPages)) == -1)
	{
		perror("Couldn't unmap memory");
	}
}

// Marks an amount of pages as allocated within the bookkeeping section
void *page_alloc(int pages)
{

	if (pages <= 0)
	{
		return NULL; // Can't allocate 0 or less pages
	}
	else if (pages_taken() + pages > numPages)
	{
		return NULL; // Not enough room to allocate the pages
	}

	// Looking for the closest set of contiguous pages that can fit the desired allocation
	int numContiguousEmptyPages = 0;
	int beginningPageNum; // Records the page # that is the beginning of the contiguous pages
	for (int i = 0; i < numPages && numContiguousEmptyPages < pages; i++)
	{
		for (int j = 0; j < 4 && numContiguousEmptyPages < pages; j++)
		{

			if (test_bit(((char *)memoryPool)[i], j * 2) == false)
			{ // The page hasn't been set
				if (numContiguousEmptyPages == 0)
				{ // This was the start of an empty line of pages
					beginningPageNum = i * 4 + j;
				}
				numContiguousEmptyPages++;
			}
			else
			{ // The page was set
				numContiguousEmptyPages = 0;
			}
		}
	}

	printf("A line of unallocated pages was found at page %d of length %d\n", beginningPageNum, numContiguousEmptyPages);

	// Setting the bookkeeping bits
	for (int i = 0; i < pages - 1; i++)
	{
		int bookkeepingByte = (beginningPageNum + i) / 4;
		int bookkeepingBit = (beginningPageNum + i) % 4 * 2;

		printf("Setting this byte and bit: %d %d\n", bookkeepingByte, bookkeepingBit);
		set_bit(((char *)memoryPool) + bookkeepingByte, bookkeepingBit);
	}

	// Setting the final bit and marking it as the end of the continuous pages
	int finalByte = (beginningPageNum + pages - 1) / 4;
	int finalBit = (beginningPageNum + pages - 1) % 4 * 2;

	printf("Setting the final byte and bits: %d %d and %d %d\n", finalByte, finalBit, finalByte, finalBit + 1);

	set_bit(((char *)memoryPool) + ((beginningPageNum + pages - 1) / 4), (beginningPageNum + pages - 1) % 4 * 2);	  // Marking it as allocated
	set_bit(((char *)memoryPool) + ((beginningPageNum + pages - 1) / 4), (beginningPageNum + pages - 1) % 4 * 2 + 1); // Setting the end of the pages

	// Finding where the actual page is in memory
	void *beginningOfPage = (char *)memoryPool + beginningPageNum * PAGE_SIZE + calcBookkeepingPadded(numPages);

	// Initializing the new space in memory to 0 in order to clear any garbage sitting in the pages
	// (I'm not entirely sure if this is necessary)
	memset(beginningOfPage, 0, PAGE_SIZE * pages);

	return beginningOfPage;
}

// Takes a memory address returned from page_alloc() and frees the whole contiguous page allocation from the memory pool
void page_free(void *addr)
{
	int currentPageNum = (int)(addr - memoryPool - calcBookkeepingPadded(numPages)) / PAGE_SIZE;
	// printf("The beginning page number of the freed section: %d\n", currentPageNum);

	int bookkeepingByte = currentPageNum / 4;
	int bookkeepingBit = currentPageNum % 4 * 2;
	while (test_bit(((char *)memoryPool)[bookkeepingByte], bookkeepingBit + 1) != 1)
	{ // Checking if we've hit the last page

		printf("Clearing this byte and bit: %d %d and %d %d\n", bookkeepingByte, bookkeepingBit, bookkeepingByte, bookkeepingBit + 1);

		// Clearing both bookkeeping bits related to this page
		clear_bit((char *)memoryPool + bookkeepingByte, bookkeepingBit);
		clear_bit((char *)memoryPool + bookkeepingByte, bookkeepingBit + 1);

		currentPageNum++;

		bookkeepingByte = currentPageNum / 4;
		bookkeepingBit = currentPageNum % 4 * 2;
	}
	// Clearing the final bookkeeping bits
	printf("Clearing this byte and bit: %d %d and %d %d\n", bookkeepingByte, bookkeepingBit, bookkeepingByte, bookkeepingBit + 1);
	clear_bit((char *)memoryPool + bookkeepingByte, bookkeepingBit);
	clear_bit((char *)memoryPool + bookkeepingByte, bookkeepingBit + 1);
}

// Returns the amount of pages that have been allocated by page_alloc()
// This function simply iterates through the entirety of the bookkeeping bytes and
// keeps track of the pages that have been marked as allocated
int pages_taken(void)
{
	int totalPages = 0;
	for (int i = 0; i < numPages; i++)
	{
		for (int j = 0; j < 4; j++)
		{

			// printf("We are here: %x\n", (char*)memoryPool + i);
			if (test_bit(((char *)memoryPool)[i], j * 2) == 1)
			{ // This checks the leftmost bit that records if the page has been allocated
				totalPages++;
			}
		}
	}

	return totalPages;
}

// Returns the amount of pages yet to be allocated within the pool of memory
int pages_free(void)
{
	return numPages - pages_taken();
}

int main()
{

	printf("I like chicken and dizza\n");

	page_init(11);

	void *startOfPage = page_alloc(3);
	printf("The beginning of my new pages is at %p\n\n", startOfPage);

	void *startOfPage2 = page_alloc(2);
	printf("The beginning of my new pages is at %p\n\n", startOfPage2);

	page_free(startOfPage);

	startOfPage = page_alloc(4);
	printf("The beginning of my new pages is at %p\n\n", startOfPage);

	printf("this is the amount of allocated pages: %d\n", pages_taken());
	printf("This is the amount of unallocated pages: %d\n", pages_free());

	page_deinit();

	return 0;
}
