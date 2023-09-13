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

int rb_at(const struct RingBuffer *rb) {
    return rb->at;
}

int rb_size(const struct RingBuffer *rb) {
    return rb->size;
}

int rb_capacity(const struct RingBuffer *rb) {
    return rb->capacity;
}

// Adds data to the RingBuffer
// Returns true if the data was added successfully, false if not
bool rb_push(struct RingBuffer *rb, char data) {
    // Check if the capacity can fit the new value
    if (rb->size < rb->capacity) {
        
        // Place the new value into buffer[at]
        rb->buffer[rb->at] = data;

        // increment at and the size
        rb->at++;
        rb->size++;

        return true;
    }
    else {
        return false;
    }

}
