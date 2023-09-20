// Benjamin Belandres

#include "rbuf.h"
#include <stdlib.h>
#include <stdio.h>  // For debugging

struct RingBuffer
{
    int at;         // The index of the next byte to read
    int size;       // The total number of unread bytes in the ring buffer
    int capacity;   // The total number of bytes that can be stored in this ring buffer
    int __reserved; // Used to move buffer to an 8-byte boundary. If this is omitted, the structure will be padded anyway
    char *buffer;   // The actual data storage
};

// Makes a new buffer of size 0 and whatever capacity specified
// Returns a pointer to the new RingBuffer or NULL if there isn't enough memory or the capacity is invalid
struct RingBuffer* rb_new(int capacity) {
    struct RingBuffer *rb;
    if (capacity <= 0) {
        // Capacity is invalid.
        return NULL;
    }
    if (!(rb = (struct RingBuffer *)malloc(sizeof(*rb)))) {
        // Out of memory.
        return NULL;
    }
    // If we get here, all is well with the memory.
    rb->at = 0;
    rb->size = 0;
    rb->capacity = capacity;
    rb->buffer = (char *)calloc(rb->capacity, sizeof(char));
    if (!rb->buffer) {
        // We were able to create the structure in memory,
        // but not the buffer. We need both, so free the
        // structure and return NULL (error).
        free(rb);
        return NULL;
    }
    return rb;
}

// Deallocates memory
void rb_free(struct RingBuffer *rb) {
    free(rb->buffer);
    free(rb);
}

// Fetch funcitons
int rb_at(const struct RingBuffer *rb) {
    return rb->at;
}

int rb_size(const struct RingBuffer *rb) {
    return rb->size;
}

int rb_capacity(const struct RingBuffer *rb) {
    return rb->capacity;
}

// Advances the at index to the next value and wraps around
// the ring if it goes past the end
static void incrementAt(struct RingBuffer *rb) {
    rb->at++;
    
    // Wrapping the at index around if it has reached capacity
    if (rb->at > rb->capacity - 1) {
        rb->at = rb->at % rb->capacity;
    }
    // DEBUG printf("At is currently at index: %d\n", rb->at);
}

// Adds data to the RingBuffer where the at pointer is looking
// Returns true if the data was added successfully, false if not
bool rb_push(struct RingBuffer *rb, char data) {
    // Check if the capacity can fit the new value
    if (rb->size < rb->capacity) {
        
        // Place the new value into buffer[at]
        rb->buffer[rb->at] = data;

        // printf("Added %c to the buffer at %d\n", rb->buffer[rb->at], rb->at);

        incrementAt(rb);
        
        rb->size++;
        return true;
    }
    else {
        return false;
    }

}


// Pops the next value off from the buffer and stores it in data
// If data is NULL, nothing is stored in data
// Returns true if it removed data, false if it didn't
bool rb_pop(struct RingBuffer *rb, char *data) {
    
    if (rb->size > 0) {

        if (data != NULL) {
            // Data actually points to somewhere in memory
            *data = rb->buffer[rb->at];
        }

        rb->size--;

        incrementAt(rb);

        return true;
    }
    else {
        return false;
    }
}


// Returns the value of the next byte
// Returns 0 if nothing is found
char rb_peek(const struct RingBuffer *rb) {

}
