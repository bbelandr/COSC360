// Benjamin Belandres

#pragma once

#include <stdbool.h>
#include <stdint.h>

// Forward declaration for the opaque structure.
struct Vector;

// Remember C doesn't name a type via struct.
typedef struct Vector Vector;

// Allocation/creation of a vector
Vector *vector_new(void);                               // Creates a new empty vector on the heap
Vector *vector_new_with_capacity(int capacity);         // Creates a new vector with capacity amount of elements
void    vector_free(Vector *vec);
void    vector_resize(Vector *vec, int new_size);
void    vector_reserve(Vector *vec, int new_capacity);

// Element functions
void    vector_push(Vector *vec, int64_t value);
void    vector_insert(Vector *vec, int index, int64_t value);
bool    vector_remove(Vector *vec, int index);
bool    vector_get(Vector *vec, int index, int64_t *value);
bool    vector_set(Vector *vec, int index, int64_t value);
void    vector_clear(Vector *vec);

// Finding/sorting
int     vector_find(Vector *vec, int64_t value);
void    vector_sort(Vector *vec);
void    vector_sort_by(Vector *vec, bool (*comp)(int64_t left, int64_t right));
int     vector_bsearch(Vector *vec, int64_t value);

// Accessors for fields in the vector
int     vector_capacity(Vector *vec);
int     vector_size(Vector *vec);

// Convenience macros for pointers
#define vector_push_ptr(vec, ptr)         vector_push(vec, (int64_t)ptr)
#define vector_find_ptr(vec, ptr)         vector_find(vec, (int64_t)ptr)
#define vector_insert_ptr(vec, idx, ptr)  vector_insert(vec, idx, (int64_t)ptr)
#define vector_set_ptr(vec, idx, ptr)     vector_set(vec, idx, (int64_t)ptr)
#define vector_bsearch_ptr(vec, ptr)      vector_bsearch(vec, (int64_t)ptr)