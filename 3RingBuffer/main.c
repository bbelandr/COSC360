// Benjamin Belandres
#include "rbuf.h"

int main() {

    RingBuffer *rb = rb_new(10);

    rb_push(rb, 1);

    rb_free(rb);
    return 0;
}