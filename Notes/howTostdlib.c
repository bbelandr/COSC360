// 
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

struct MyStruct {   // This takses up 12 bytes
    int i;
    int j;
    int k;
};

// MALLOC
// int main() {
//     struct MyStruct *ms;

//     ms = (struct MyStruct *)malloc(sizeof((*ms)));  // sizeof() will automatically calculate the amount of memory during compile time (in this case, 12 bytes)
//                                                     // It is also put onto the heap because this memory is allocated dynamically. Never assume anything from malloc is initialized to 0

//     // ms++;       // A line like this would add 12 to whatever memory address ms is pointing to (becasue MyStruct is 12 bytes)

//     printf("%d\n", ms->i);  // This line prints garbage if i hasn't been assigned any values, (because malloc doesn't initialize values)

//     free(ms);   // This is the C version of delete

//     // printf("%lu\n", sizeof(ms));    // This will print 8 because ms is a pointer

// }

// CALLOC
// int main() {
//     struct MyStruct *ms;

//     ms = (struct MyStruct *)calloc(10, sizeof((*ms)));  // calloc(numberOfMembers, sizeOf1Element)
//                                                         // Also initializes everything to 0
//                                                         // all the memory from calloc also goes to the heap
//     printf("%d\n", ms->i);  // now this will print 0 instead of garbage
//     printf("%d\n", ms[2].i);    // this will print the i value of the 3rd element of the array
//     free(ms);   
// }

// System Management
int main() {
    
    void *mem;

    mem = mmap(NULL, 120, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);

    perror("mmap"); // This will print the most recent error (even if there are multiple processes running)
                    // The most recent error is stored in a global variable called errno (you can look at it in <errno.h>)
                    // 

    munmap(mem, 120);   // technically you don't need to do this on a single process, but for multiprocessing you have to or else you get a memory leak

    // mmap
        // memory map
        // void* mmap(void* addr,   // linux can ignore the address where you ask it to put the memory
            // size_t length,
            // int prot,
            // int flags,           // MAP_FIXED makes it so that Linux has to place the memory at the address you give it
            // int fd,
            // off_t offset);

    // protection for read write and execute
    // PROT_EXEC
    // PROT_READ
    // PROT_WRITE
    // PROT_NONE
        // in order to combine these, you need to use bitwise operations ( | )
    
    // flags
    // MAP_ANONYMOUS
        // If you literally only want memory
    // MAP_SHARED
    // MAP_PRIVATE
    // MAP_FIXED
        // Fixes the memory to a specific address (Linux can ignore you though)
    // MAP_STACK
        // Useful for multiprocessing because different processes running at once need their own memory

    // munmap
        // memory unmap
}