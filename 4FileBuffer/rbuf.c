// Benjamin Belandres

#include "rbuf.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>  // For debugging

// Normally a ringbuffer like this would have a head and tail index, but this ringbuffer
// only uses a tail (int at). 
// To learn the loaction of the head, I implemented a static head function
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
static void incrementAt(struct RingBuffer *rb, int amount) {
    rb->at += amount;
    
    // Wrapping the at index around if it has reached capacity
    // if (rb->at > rb->capacity - 1) {
    rb->at = rb->at % rb->capacity;
    // }
    // DEBUG printf("At is currently at index: %d\n", rb->at);
}

// Returns where the head would be if there were a head variable
static int head(struct RingBuffer *rb) {
    return (rb->at + rb->size) % rb->capacity;
}


// Adds data to the RingBuffer where the at pointer is looking
// Returns true if the data was added successfully, false if not
bool rb_push(struct RingBuffer *rb, char data) {
    // Check if the capacity can fit the new value
    if (rb->size < rb->capacity) {
        
        // Place the new value at the head
        rb->buffer[head(rb)] = data;

        // printf("Added %c to the buffer at %d\n", rb->buffer[rb->size + rb->at], rb->size + rb->at);
        rb->size++;

        // incrementAt(rb);
        
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
        // rb->at++; 
        incrementAt(rb, 1);



        return true;
    }
    else {
        return false;
    }
}


// Returns the value of the next byte
// Returns 0 if nothing is found
    // Be aware that this function can also return 0 if it sees '\0' 
    // on the ring buffer. Checking the size of the buffer before 
    // using this function is the user's responsibility.
// This function does NOT change the at index or the size
char rb_peek(const struct RingBuffer *rb) {
    
    if (rb->size > 0) {
        return rb->buffer[rb->at];
    }
    else {
        return 0;
    }
}


// Skips num amount of values from the at index
// This effectively removes num values from the buffer
// Instead of calling pop over and over again, this function simply
// changes the size and the at index, saving processing time.
void rb_ignore(struct RingBuffer *rb, int num) {
    if (num > rb->size) {
        num = rb->size;
    }

    rb->size = rb->size - num;

    incrementAt(rb, num);
    // rb->at = (rb->at + num) % rb->capacity;
}


// Places information from the ringbuffer to the memory address
// pointed to by buf. 
// Decrements the size for every byte read from the ringbuffer and
// increments the at pointer.
// Returns the actual number of bytes read and put into buf.
int rb_read(struct RingBuffer *rb, char *buf, int max_bytes) {
    
    if (max_bytes > rb->size) {
        max_bytes = rb->size;
    }

    // We're going to have to loop around the buffer
    if (rb->at + max_bytes > rb->capacity) {
        // Doing the right side
        int bytes_read = rb->size - rb->at;
        memcpy(buf, &rb->buffer[rb->at], bytes_read);

        // Doing the left side
        memcpy(&buf[bytes_read], rb->buffer, max_bytes - bytes_read);
    }
    else {
        // No looping necessary
        memcpy(buf, &rb->buffer[rb->at], max_bytes);
    }


    incrementAt(rb, max_bytes);
    rb->size = rb->size - max_bytes;

    return max_bytes;
}


// Writes as many bytes to the RingBuffer as can fit from buf
// bytes is the max amount of bytes to write
// Returns the number of bytes actually written to the buffer
int rb_write(struct RingBuffer *rb, const char *buf, int bytes) {
    if (rb->capacity - rb->size < bytes) {
        bytes = rb->capacity - rb->size;    // The actual number of bytes we're going to be able to write
    }

    if (head(rb) + bytes > rb->capacity) {
        // We have to loop
        // Doing the right side
        int bytes_written = rb->capacity - head(rb);
        memcpy(&rb->buffer[head(rb)], buf, bytes_written);

        // Doing the left side
        memcpy(rb->buffer, &buf[bytes_written], bytes - bytes_written);
    }
    else {
        // No looping necessary
        memcpy(&rb->buffer[head(rb)], buf, bytes);
    }
    
    rb->size += bytes;
    return bytes;
}


void rb_clear(struct RingBuffer *rb) {
    rb->at = 0;
    rb->size = 0;
}