// MAKE SURE THAT THIS COMPILES WITHOUT ANY HEADER FILES OR MAIN BEFORE SUBMISSION
// Benjamin Belandres

// int math(int a, int b) {
//     return a + b;
// }
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

// Thread Pool Functions
void *thread_pool_init(int num_threads) {	// Creates threads and places them into the waiting state
	if (num_threads < 1 || num_threads > 32) {	// The specified number of threads can't be created
		return NULL;
	}
	
	void *handle;
	return handle;
}

bool thread_pool_hash(void *handle, const char *directory, int hash_size) { // Assigns work to the pool
	return false;
}

void thread_pool_shutdown(void *handle) {	// Shuts down all threads waiting for work and frees memory
	return;
}


// Hashing Functions
unsigned int fnv1a_32(char *buffer) {	// 32 bit hash function
	return 0;
}

unsigned long fnv1a_64(char *buffer) {	// 64 bit hash function
	return 0;
}

