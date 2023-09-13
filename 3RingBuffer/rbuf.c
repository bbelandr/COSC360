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


