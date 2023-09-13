// Benjamin Belandres
#include "vector.h"
#include <stdio.h>

int main() {

    Vector* vec = vector_new();
    // vector_resize(vec, 10);

    // vector_reserve(vec, 10);

    vector_push(vec, 0);
    vector_push(vec, 2);
    vector_push(vec, 1);
    vector_push(vec, 3);

    for (int i = 0; i < vector_size(vec); i++) {
        int64_t value;
        vector_get(vec, i, &value);
        printf("%ld ", value);
    }
    printf("\n");

    vector_insert(vec, 2, 10);

    for (int i = 0; i < vector_size(vec); i++) {
        int64_t value;
        vector_get(vec, i, &value);
        printf("%ld ", value);
    }
    printf("\n");

    vector_sort(vec);

    for (int i = 0; i < vector_size(vec); i++) {
        int64_t value;
        vector_get(vec, i, &value);
        printf("%ld ", value);
    }
    printf("\n");
    // vector_push(vec, 5);
    // printf("Size = %d\n", vector_size(vec));

    // vector_push(vec, 8);
    // printf("Size = %d\n", vector_size(vec));

    // vector_push(vec, 7);
    // printf("Size = %d\n", vector_size(vec));
    // printf("Capacity = %d\n", vector_capacity(vec));

    // vector_push(vec, 2);
    // vector_push(vec, 4);
    // vector_push(vec, 1);
    // vector_push(vec, 3);
    // vector_push(vec, 6);
    // vector_push(vec, 9);
    // vector_push(vec, 0);

    // // printf("%d\n", vector_find(vec, 7));

    // int64_t value;
    // vector_get(vec, 2, &value);
    // printf("%ld\n", value);


    // vector_sort(vec);
    // printf("%d\n", vector_bsearch(vec, 1));


    // vector_remove(vec, 0);
    // vector_get(vec, 1, &value);
    // printf("%ld\n", value);
    
    // vector_set(vec, 0, 9);
    // vector_get(vec, 0, &value);
    // printf("%ld\n", value);


    // vector_resize(vec, 1);
    // printf("Size = %d\n", vector_size(vec));

    // vector_clear(vec);
    // printf("Size = %d\n", vector_size(vec));
    // printf("Capacity = %d\n", vector_capacity(vec));

    vector_free(vec);
    return 0;
}