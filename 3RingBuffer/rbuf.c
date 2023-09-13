// Benjamin Belandres

#include "rbuf.h"

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
RingBuffer* rb_new(int capacity) {
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

void rb_free(RingBuffer *rb) {
    free(rb->buffer);
    free(rb);
}
