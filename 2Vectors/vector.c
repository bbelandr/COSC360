// Benjamin Belandres
// I HAVE PURPOSEFULLY DECIDED TO CHANGE THE VECTOR_INSERT FUNCTION FROM THE WRITEUP BECAUSE THE WRITEUP'S VERSION
// HAS LIMITED FUNCTIONALITY.
// THERE ARE MORE DETAILS AT THE FUNCTION DECLARATION.

#include "vector.h"
typedef struct Vector {
    int size;
    int capacity;
    int64_t *values;
} Vector;

Vector* vector_new(void) {
    Vector *v = (Vector *)malloc(sizeof(Vector));
    v->capacity = 0;
    v->size = 0;
    v->values = (int64_t *)malloc(sizeof(int64_t)); // Assigning some memory to values so that you don't end up doing things with this uninitialized 
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
    free(vec->values);
    free(vec);
}

// Calloc automatically initializes new values to 0, so you're good on that front
// If the capacity is smaller than the size
    // reallocate memory and make the capacity == to new_size
// If the capacity fits the new size
    // Change the size value and that's it
void vector_resize(Vector *vec, int new_size) {
    
    if (vec->capacity < new_size) { // Doesn't fit the capacity
        // making a new array of values that fits the capacity
        int64_t *new_values = (int64_t *)calloc(new_size, sizeof(int64_t));

        // Copying over the values from the old array to the new one
        for (int i = 0; i < vec->size; i++) {
            new_values[i] = vec->values[i];
        }

        // Freeing the old memory and pointing the values array to the new one
        free(vec->values);
        vec->values = new_values;

        vec->capacity = new_size;
        vec->size = new_size;
        
    }
    else {  // New size fits the capacity
        vec->size = new_size;
    }
}

// If the new capacity would be smaller than the already existant size,
    // The capacity is set to the size
// If the new capacity is bigger than the already existant size or previous capacity,
    // The vector is reallocated with a new capacity, but the size stays the same
void vector_reserve(Vector *vec, int new_capacity) {
    if (new_capacity < vec->size) { // Smaller than the size
        vec->capacity = vec->size;
    }
    else if (new_capacity > vec->size || new_capacity > vec->capacity) {
        // making a new array of values that fits the capacity
        int64_t *new_values = (int64_t *)calloc(new_capacity, sizeof(int64_t));

        // Copying over the values from the old array to the new one
        for (int i = 0; i < vec->size; i++) {
            new_values[i] = vec->values[i];
        }

        
        free(vec->values);
        

        vec->values = new_values;

        vec->capacity = new_capacity;

    }
}

// Adds a new value to the back of the vector
    // If there's no room, the vector is resized to accomodate for the new value
void vector_push(Vector *vec, int64_t value) {

    if (vec->size >= vec->capacity) {   // The vector is full
        vector_resize(vec, vec->size + 1);
        vec->values[vec->size - 1] = value;
    }
    else {                              // The vector has space

        vec->values[vec->size] = value;
        vec->size++;

    }

}

// Places the new value at the specified index
    // If there's no room, the vector is resized to accomodate
// As per the writeup, the function should
    // Insert the value "to the end of the vector if the location is >= the last element."
    // Because of this, it is impossible to insert a value one space before the last element using this function.
    // I have purposefully neglected to follow the writeup and instead allow this functionality to happen.
void vector_insert(Vector *vec, int index, int64_t value) {
    if (index >= vec->size) {
        vector_push(vec, value); // Inserting at or after the end of the vector
        return;
    }
    else if (index < 0) {
        return;
    }
    else {
        if (vec->size >= vec->capacity) { // The vector is full
            vector_resize(vec, vec->size + 1);

            int64_t temp = vec->values[index]; // Save the value at index

            // Moving everything to the right of the index one position to the right
            for (int i = vec->size - 2; i >= index; i--) {
                vec->values[i + 1] = vec->values[i];
            }

            // Inserting the new value at the index
            vec->values[index] = value;

            // Insert the saved value at the next index
            vec->values[index + 1] = temp;
        }
        else { // The vector has space

            // Moving everything to the right of the index one position to the right
            for (int i = vec->size - 1; i >= index; i--) {
                vec->values[i + 1] = vec->values[i];
            }

            // Inserting the new value at the index
            vec->values[index] = value;

            vec->size++;
        }
    }
}


// Returns true if the element exists and was removed, false otherwise
bool vector_remove(Vector *vec, int index) {
    
    if (index >= vec->size || index < 0) {   // Element doesn't exist
        return 0;
    }

    for (int i = index; i < vec->size - 1; i++) {
        vec->values[i] = vec->values[i + 1];
    }

    vec->size--;
    return 1;
}

// Returns true if the value is valid, false if not
bool vector_get(Vector *vec, int index, int64_t *value) {
    if (index >= vec->size) {
        return false;
    }
    else {
        *value = vec->values[index];
        return true;
    }
}

// Returns true if the index exists, false if not
bool vector_set(Vector *vec, int index, int64_t value) {
    if (index >= vec->size || index < 0) {
        return false;
    }

    else {
        vec->values[index] = value;
        return true;
    }
}

// Clears the vector by setting the size to 0, doesn't do anything with memory or capacity
void vector_clear(Vector *vec) {
    vec->size = 0;
}

// Returns the index of the value searched for using linear search
// Returns -1 if it is not present
int vector_find(Vector *vec, int64_t value) {

    for (int i = 0; i < vec->size; i++) {
        if (vec->values[i] == value) {
            return i;
        }
    }
    return -1;
}


// used in vector_sort_by for ascending order
static bool comp_ascending(int64_t left, int64_t right) {
    // Consider left <= right to mean left is in place with right. Meaning
    // we swap if left > right.
    return left <= right;
}

// Uses selection sort to sort the vector in ascending order
void vector_sort(Vector *vec) {
    vector_sort_by(vec, comp_ascending);
}

// Sorts the vector according to a specific comparison function through selection sort
void vector_sort_by(Vector *vec, bool (*comp)(int64_t left, int64_t right))
{
    int i;
    int j;
    int min;
    int64_t tmp;

    for (i = 0; i < vec->size - 1; i++) {
        min = i;
        for (j = i + 1; j < vec->size; j++) {
            if (!comp(vec->values[min], vec->values[j])) {
                min = j;
            }
        }
        if (min != i) {
            tmp = vec->values[min];
            vec->values[min] = vec->values[i];
            vec->values[i]   = tmp;
        }
    }
}

// Uses binary search to find value
    // Assumes the vector has been sorted
// Returns the index of the value if found or -1 if not
int vector_bsearch(Vector *vec, int64_t value) {
    
    int min = 0;
    int max = vec->size - 1;

    while (min <= max) {

        int currentIndex = ((max - min) / 2) + min;
        // DEBUG printf("currentIndex = %d\n", currentIndex);

        if (vec->values[currentIndex] == value) {
            return currentIndex;
        }
        else if (vec->values[currentIndex] < value) {
            min = currentIndex + 1;
        }
        else if (vec->values[currentIndex] > value) {
            max = currentIndex - 1;
        }
        

    }

    return -1;
}


int vector_capacity(Vector *vec) {
    return vec->capacity;
}

int vector_size(Vector *vec) {
    return vec->size;
}