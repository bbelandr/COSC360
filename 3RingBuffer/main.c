// Benjamin Belandres
#include "rbuf.h"
#include <stdio.h>

int main() {

    RingBuffer *rb = rb_new(3);

    printf("Capacity:       %3d\n", rb_capacity(rb));
    printf("Size:           %3d\n", rb_size(rb));

    // printf("%d Added 4 to the buffer\n", rb_push(rb, 'a'));
    rb_push(rb, 'a');
    rb_push(rb, 's');
    rb_push(rb, 's');
    rb_push(rb, 'r');
    rb_push(rb, 'r');

    printf("Capacity:       %3d\n", rb_capacity(rb));
    printf("Size:           %3d\n", rb_size(rb));
    printf("Current index:  %3d\n", rb_at(rb));


    char val;
    bool ret;

    ret = rb_pop(rb, &val);
    printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    ret = rb_pop(rb, &val);
    printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    ret = rb_pop(rb, &val);
    printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    ret = rb_pop(rb, &val);
    printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    ret = rb_pop(rb, &val);
    printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    ret = rb_pop(rb, &val);
    printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);
    ret = rb_pop(rb, &val);
    printf("%d Popped (Size %d): %c\n", ret, rb_size(rb), val);

    rb_free(rb);
    return 0;
}