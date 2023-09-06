#include "vector.h"
// Benjamin Belandres

typedef struct Vector {
    int size;
    int capacity;
    int64_t *values;
} Vector;

Vector* vector_new(void) {
    struct Vector *v = malloc(sizeof(struct Vector));
    v->capacity = 0;
    v->size = 0;
    return v;
}

Vector* vector_new_with_capacity(int capacity) {
    Vector* v = (Vector *)malloc(sizeof(Vector));
    v->size = 0;
    v->capacity = capacity;
    v->values = (int64_t *)calloc(v->capacity, sizeof(int64_t));
    return v;
}

void vector_free(Vector* vec) {
    for()
    free(vec);
}

