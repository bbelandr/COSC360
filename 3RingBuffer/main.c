// Benjamin Belandres
#include "rbuf.h"
#include <stdio.h>
#include <string.h>

int main() {

    RingBuffer *rb = rb_new(10);

    printf("Capacity:       %3d\n", rb_capacity(rb));
    printf("Size:           %3d\n", rb_size(rb));

    // printf("%d Added 4 to the buffer\n", rb_push(rb, 'a'));
    rb_push(rb, 'a');
    rb_push(rb, 's');
    rb_push(rb, 'k');
    rb_push(rb, 'e');
    rb_push(rb, 'r');

    printf("\nAfter adding values:\n");
    printf("Capacity:       %3d\n", rb_capacity(rb));
    printf("Size:           %3d\n", rb_size(rb));
    printf("Current index:  %3d\n", rb_at(rb));


    char val;
    int ret;

    // rb_clear(rb);
    rb_ignore(rb, 2);

    printf("\nAfter ignoring:\n");
    printf("Capacity:       %3d\n", rb_capacity(rb));
    printf("Size:           %3d\n", rb_size(rb));
    printf("Current index:  %3d\n", rb_at(rb));


    // // CHECKING WRITE
    // char inputString[] = "pizaPie";
    // char outputString[strlen(inputString)];
    // rb_ignore(rb, 2);

    // printf("\nAfter ignoring vals:\n");
    // printf("Capacity:       %3d\n", rb_capacity(rb));
    // printf("Size:           %3d\n", rb_size(rb));
    // printf("Current index:  %3d\n", rb_at(rb));


    // ret = rb_write(rb, inputString, 6);
    // printf("Successfully wrote %d bytes\n", ret);

    // printf("Successfully read %d bytes\n", rb_read(rb, outputString, 9));
    // printf("%s\n", outputString);

    // rb_ignore(rb, 3);

    // printf("\nAfter ignoring vals:\n");
    // printf("Capacity:       %3d\n", rb_capacity(rb));
    // printf("Size:           %3d\n", rb_size(rb));
    // printf("Current index:  %3d\n", rb_at(rb));

    // ret = rb_write(rb, inputString, 6);
    // printf("Successfully wrote %d bytes\n", ret);

    // printf("Successfully read %d bytes\n", rb_read(rb, outputString, 9));
    // printf("%s\n", outputString);


    // // CHECKING READ
    // char myString[5];

    // rb_ignore(rb, 2);
    // rb_push(rb, 't');
    // rb_push(rb, 'i');

    // printf("Successfully read %d bytes\n", rb_read(rb, myString, 9));

    // printf("%s\n", myString);
    // printf("At is currently looking at: %c\n", rb_peek(rb));

    // printf("\nAfter reading:\n");
    // printf("Capacity:       %3d\n", rb_capacity(rb));
    // printf("Size:           %3d\n", rb_size(rb));
    // printf("Current index:  %3d\n", rb_at(rb));



    // // CHECKING IGNORE
    // rb_ignore(rb, 3);
    // printf("\nIgnored Values\n");
    // printf("Capacity:       %3d\n", rb_capacity(rb));
    // printf("Size:           %3d\n", rb_size(rb));
    // printf("Current index:  %3d\n", rb_at(rb));
    // printf("At is currently looking at: %c\n", rb_peek(rb));


    // // CHECKING POP
    // bool ret;
    // ret = rb_pop(rb, &val);
    // printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    // printf("Current index:  %3d\n", rb_at(rb));
    // ret = rb_pop(rb, &val);
    // printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    // printf("Current index:  %3d\n", rb_at(rb));
    // ret = rb_pop(rb, &val);
    // printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    // printf("Current index:  %3d\n", rb_at(rb));
    // ret = rb_pop(rb, &val);
    // printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    // printf("Current index:  %3d\n", rb_at(rb));
    // ret = rb_pop(rb, &val);
    // printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    // printf("Current index:  %3d\n", rb_at(rb));
    // ret = rb_pop(rb, &val);
    // printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    // printf("Current index:  %3d\n", rb_at(rb));
    // ret = rb_pop(rb, &val);
    // printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    // printf("Current index:  %3d\n", rb_at(rb));

    // rb_push(rb, 'p');
    // printf("\nCurrent index:  %3d\n", rb_at(rb));

    // ret = rb_pop(rb, &val);
    // printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    // printf("Current index:  %3d\n", rb_at(rb));


    rb_free(rb);
    return 0;
}