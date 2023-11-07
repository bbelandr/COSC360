// THIS DOES NOT GET SUBMITTED
// Benjamin Belandres
#include <stdbool.h>

// int math(int a, int b);

// Thread Pool Functions
void *thread_pool_init(int num_threads);    // Creates threads and places them into the waiting state
                                            // Returns a handle to the new threads
bool thread_pool_hash(void *handle, const char *directory, int hash_size);  // Assigns work to the pool,
                                                                            // hashing the files 
                                                                            // provided in directory.
void thread_pool_shutdown(void *handle);    // Shuts down all threads waiting for work and frees memory


// Hashing functions
unsigned int fnv1a_32(char *buffer);    // 32 bit hash function
unsigned long fnv1a_64(char *buffer);   // 64 bit hash function
