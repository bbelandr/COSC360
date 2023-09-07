#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
// #include <sys/mman.h>
// #inlcude <errno.h>

// struct MyStruct {
//     int i;
//     int j;
//     int k;
// };

// typedef struct MyStruct MyStruct;   // This creates a new data type called MyStruct

// // typedef unsigned long uint64_t;



// Here's another way to do this
// typedef struct {
//     int i;
//     int j;
//     int k;
// } MyStruct;



// OPAQUE STRUCTURES
    // They force the user or programmer from the outside to go through your own functions to change values

// void dosomething(struct MyStruct *ms);  // You could even use a void*, but it is confusing

typedef struct {
    int i;
    int j;
    int k;
} MyStruct;

// These are a part of string because they were originally made to manipulate c style strings
// These all return the destination pointer
void* memset(void *dst, char val, int size);        // Goes to each byte and writes whatever you want in the destination with whatever you put in char val
void* memcpy(void* dst, const void* src, int size); // This moves one byte at a time to whereever you are copying stuff to (undefined behavior for overlapping memory) 
                                                    // Most times you can do this faster using a single instruction if you know the size beforehand (this takes 8 assembly
                                                    // instructions for a long while just doing longInt1 = longInt2 takes one)
void* memmove(void* dest, const void* src, int size);   // This does the same thing but works for overalpping memory

int main() {
    MyStruct ms1;    // Making new data types makes this work without the struct keyword in front of it 
    MyStruct ms2;

    ms1.i = 100;
    ms1.j = 200;
    ms1.k = 300;

    ms2 = ms1;

    memset(ms1.p, 1, sizeof(*ms1.p));
    memcpy(&ms2, &ms1, sizeof(MyStruct));

    printf("%d %d %d\n", ms2.i, ms2.j, ms2.k);

}

