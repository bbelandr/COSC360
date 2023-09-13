#pragma once
#include <stdbool.h>
// Opaque structure defined in the C file
struct RingBuffer;
typedef struct RingBuffer RingBuffer;

// Memory management
RingBuffer *rb_new(int capacity);
void rb_free(RingBuffer *rb);

// Ring buffer values
int  rb_at(const RingBuffer *rb);
int  rb_size(const RingBuffer *rb);
int  rb_capacity(const RingBuffer *rb);

// Individual item functions
bool rb_push(RingBuffer *rb, char data);
bool rb_pop(RingBuffer *rb, char *data);
char rb_peek(const RingBuffer *rb);
void rb_ignore(RingBuffer *rb, int num);

// Buffer functions
int  rb_read(RingBuffer *rb, char *buf, int max_bytes);
int  rb_write(RingBuffer *rb, const char *buf, int bytes);
void rb_clear(RingBuffer *rb);