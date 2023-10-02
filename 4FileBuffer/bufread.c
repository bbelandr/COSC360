#include "bufread.h"
#include "rbuf.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>  // for debugging


struct RingBuffer {
    int at;
    int size;
    int capacity;
    int __reserved;
    char *buffer;
};

struct BufReader {
    struct RingBuffer *rb;
    int fill;
    int fd;
};


// Returns where the head would be if there were a head variable
static int head(struct RingBuffer *rb) {
    return (rb->at + rb->size) % rb->capacity;
}

// Returns the amount of bytes filled into the buffer from the file descriptor
static int refill(int fd, RingBuffer *rb) {
    int totalBytesRead = 0;

    totalBytesRead += read(fd, &rb->buffer[head(rb)], rb->capacity - rb->size - rb->at);
    // printf("    at %d, total = %d\n", rb->at, totalBytesRead);

    if (rb->size != rb->capacity && totalBytesRead > 0) { // looping must be done
        totalBytesRead += read(fd, rb->buffer, rb->at); 
        // printf("    at %d, total = %d\n", rb->at, totalBytesRead);

    }

    rb->size += totalBytesRead;
    return totalBytesRead;

}

struct BufReader *br_open(const char *path, int capacity, int fill_trigger) {

    // Initializing the reader
    struct BufReader *newBufReader = malloc(sizeof(BufReader));

    newBufReader->fd = open(path, O_RDONLY);
    if (newBufReader->fd < 0) {    // The file wasn't opened
        return NULL;    
    }

    newBufReader->rb = rb_new(capacity);
    newBufReader->fill = fill_trigger;


    // Filling the ringbuffer with information from the file
    newBufReader->rb->size = (int)read(newBufReader->fd, newBufReader->rb->buffer, newBufReader->rb->capacity);
    

    // printf("New size of buffer = %d\n", newBufReader->rb->size);
    // printf("Capacity of buffer = %d\n", newBufReader->rb->capacity);

    // char output[256];
    // int ret = rb_read(newBufReader->rb, output, 100);
    // printf("%d   %s\n", ret, output);

    return newBufReader;


}


void  br_close(struct BufReader *br) {

    rb_free(br->rb);
    close(br->fd);
    free(br);

}


// Moves within the opened file
// Refills the ringbuffer with information pertinent to where the filepointer was moved to
void  br_seek(struct BufReader *br, int offset, int whence) {
    
    int startIndex = br_tell(br);
    
    // Moving the file pointer
    int endIndex = lseek(br->fd, offset, whence);
    


    if (startIndex > endIndex) {   // We have moved backwards, so we have to clear the buffer to read new data
        rb_clear(br->rb);

        // Reading the data
        br->rb->size = read(br->fd, br->rb->buffer, br->rb->capacity - br->rb->size); 
        
        // char output[256];
        // int ret = rb_read(br->rb, output, 100);
        // printf("%s\n", output);

    }
    else if (startIndex < endIndex) {
        rb_ignore(br->rb, endIndex - startIndex);    // Removes the characters that have been passed by the file pointer

        lseek(br->fd, br->rb->size, SEEK_CUR);  // Readjusting the file pointer to the end of the ring buffer
        
        if (br->fill >= br->rb->size) { // Time to refill the buffer
            // printf("Refilled %d bytes\n", refill(br->fd, br->rb));
            refill(br->fd, br->rb);

            // char output[256];
            // int ret = rb_read(br->rb, output, 100);
            // printf("%s\n", output);
        }
    }

}


// Returns where in the file the beginning of the ringbuffer is coming from
int   br_tell(struct BufReader *br) {
    // printf("The size is %d and the file pointer is at %d right now\n", br->rb->size, lseek(br->fd, 0, SEEK_CUR));
    return lseek(br->fd, 0, SEEK_CUR) - br->rb->size;
}


// Places a line of text from the file buffer to the string s
// Stops after these cases:
    // size characters have been read
    // a '\n' has been read
    // it has hit EOF
// This function compensates for the null terminator
char *br_getline(char s[], int size, struct BufReader *br) {
    return 0;
}


// Pops a single char from the buffer and returns it as an int
int   br_getchar(struct BufReader *br) {
    char ret; 
    if (rb_pop(br->rb, &ret)) {
        // printf("    Popped a char\n");

        // Checking if the buffer needs refilled
        // printf("    Size = %d\n", br->rb->size);
        if (br->fill >= br->rb->size) {
            // printf("    Refilling. . .\n");
            refill(br->fd, br->rb);
        }

        return (int)ret;
    }
    else {
        return -1;
    }
}


int   br_read(struct BufReader *br, char *dest, int max_bytes) {
    return 0;
}

