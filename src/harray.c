#include <stdlib.h> // malloc, realloc
#include <string.h> // memcpy, memmove

#include "harray.h"

__harray __harray_new_empty() {
    __harray arr = malloc(sizeof(struct __harray_s));
    
    arr->length    = 0;
    arr->capacity  = 0;
    arr->data      = NULL;
    arr->copy_func = NULL;
    arr->free_func = NULL;

    return arr;
}

void __harray_reserve(__harray arr, size_t n, size_t element_size) {
    size_t new_capacity = arr->capacity;
    while (new_capacity < n) {
        new_capacity = HARRAY_COMPUTE_NEXT_CAPACITY(new_capacity);
    }

    hpointer new_data_addr = arr->data == NULL
        ? malloc(new_capacity * element_size)
        : realloc(arr->data, new_capacity * element_size);

    if (new_data_addr == NULL) {
        // throw error
    }

    arr->data = new_data_addr;
    arr->capacity = new_capacity;
}

void __harray_insert_empty(__harray arr, size_t pos, size_t length, size_t element_size) {

    if (pos>arr->length) {
        // handle error
    }

    __harray_reserve(arr, arr->length + length, element_size);

    memmove(
        arr->data + (pos+ length)  * element_size, // to
        arr->data + (pos)          * element_size, // from 
        (arr->length-pos-length+1) * element_size  // total bytes
    );

    arr->length += length;
}

void __harray_insert_n(__harray arr, size_t pos, __harray val, size_t element_size) {
    __harray_insert_empty(arr, pos, val->length, element_size);

    memcpy(
        arr->data + (pos) * element_size, // to
        val->data                       , // from 
        val->length       * element_size  // total bytes
    );
}

void __harray_free_elements(__harray arr, size_t pos, size_t length, size_t element_size) {
    if (arr->free_func == NULL) return;

    hpointer ptr = arr->data;
    for (size_t idx = 0; idx < length; ++idx, ptr += element_size) {
        arr->free_func(ptr);
    }
}

void __harray_erase(__harray arr, size_t pos, size_t length, bool free_element, size_t element_size) {

    if (pos+length > arr->length) {
        // handle error
    }

    if (free_element) {
        __harray_free_elements(arr, pos, length, element_size);
    }

    memmove(
        arr->data + (pos)            * element_size, // to
        arr->data + (pos+length)     * element_size, // from
        (arr->length - pos - length) * element_size  // total bytes
    );

    arr->length -= length;
}

void __harray_free(__harray arr, size_t element_size) {

    __harray_free_elements(arr, 0, arr->length, element_size);

    free(arr->data);
    free(arr);
}

void __harray_copy(__harray src, __harray dst, size_t element_size) {
    if (src->copy_func == NULL) {
        // handle error
    } 

    memcpy(dst, src, sizeof(struct __harray_s));
    dst->data = malloc(dst->capacity * element_size);
    
    for (size_t idx = 0; idx < src->length; ++idx) {
        src->copy_func(src->data + idx * element_size, dst->data + idx * element_size);
    }
}

__harray __harray_new_copy(__harray src, size_t element_size) {
    __harray dst = malloc(sizeof(struct __harray_s));
    __harray_copy(src, dst, element_size);
    return dst;
}

void __harray_resize(__harray arr, size_t new_length, size_t element_size) {
    __harray_reserve(arr, new_length, element_size);
    arr->length = new_length;
}

void __harray_resize_with_init(__harray arr, size_t new_length, hpointer value_ptr, size_t element_size) {
    __harray_reserve(arr, new_length, element_size);

    for (size_t pos = arr->length; pos < new_length; ++pos) {
        memcpy(arr->data + pos*element_size, value_ptr, element_size);
    }

    arr->length = new_length;
}