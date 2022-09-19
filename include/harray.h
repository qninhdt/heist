#ifndef __HARRAY_H__
#define __HARRAY_H__

#include "hcommon.h"

#ifndef HARRAY_DEFAULT_CAPACITY 
    #define HARRAY_DEFAULT_CAPACITY 1
#endif

#ifndef HARRAY_COMPUTE_NEXT_CAPACITY 
    #define HARRAY_COMPUTE_NEXT_CAPACITY(capacity) (capacity ? capacity << 1 : 1)
#endif

/**
 * @brief Array type
 * 
 * @param T Element type
 */
#define harray(T)               \
    struct {                     \
        size_t length ;          \
        size_t capacity;         \
        T* data;                 \
        __harray __base_type[0]; \
    } *

#define harray_new() ((hpointer) __harray_new_empty())

#define harray_new_copy(src) ((hpointer) __harray_new_copy((__harray) src, sizeof(src->data[0])))

/**
 * @brief Requests that the vector capacity be at least enough to 
 *        contain n elements
 * 
 * @param arr Current array
 * @param n   New length
 */
#define harray_reserve(arr, n) __harray_reserve((__harray) arr, n, sizeof(arr->data[0]))

#define harray_get(arr, pos) (arr->data[pos])
#define harray_set(arr, pos, val) (arr->data[pos] = val)

#define harray_at(arr, pos) (arr->data[pos])

#define harray_insert(arr, pos, value) ({            \
    size_t __pos = pos;                               \
    __harray_insert_empty((__harray) arr, __pos, 1, sizeof(arr->data[0]));\
    arr->data[__pos] = (typeof(arr->data[0])) value;  \
})

#define harray_push_front(arr, value) harray_insert(arr, 0, value)

#define harray_push_back(arr, value) harray_insert(arr, arr->length, value)

#define harray_erase(arr, pos) \
    __harray_erase((__harray) arr, pos, 1, true, sizeof(arr->data[0]))

#define harray_erase_n(arr, pos, length) \
    __harray_erase((__harray) arr, pos, length, true, sizeof(arr->data[0]))

#define harray_insert_n(arr, pos, val) \
    __harray_insert_n((__harray) arr, pos, val, sizeof(arr->data[0]))

#define harray_foreach(arr, val, i)                                  \
    for (register size_t i=0, __b0=1; i < arr->length; ++i, __b0=1)  \
        for (typeof(arr->data[0]) val=arr->data[i]; __b0; __b0=!__b0) 

#define harray_front(arr) (arr->data[0])

#define harray_back(arr) (arr->data[arr->length-1])

#define harray_pop_front(arr) ({                    \
    typeof(arr->data[0]) front = arr->data[0];       \
    __harray_erase((__harray) arr, 0, 1, false, sizeof(arr->data[0]));   \
    front;                                           \
})

#define harray_pop_back(arr) ({                           \
    typeof(arr->data[0]) back = arr[arr->length-1];        \
    __harray_erase((__harray) arr, 0, arr->length, fasle, sizeof(arr->data[0]))\
    back;                                                  \
})

#define harray_set_copy_func(arr, func) \
    ((__harray) arr)->copy_func = (h_copy_func) __heist_copy_func_##func;

#define harray_set_free_func(arr, func) \
    ((__harray) arr)->free_func = (h_free_func) __heist_free_func_##func;

#define harray_empty(arr) (arr->length == 0)

#define harray_free(arr) ({        \
    __harray_free((__harray)arr, sizeof(arr->data[0])); \
    arr = NULL;                     \
})

#define harray_resize(arr, new_length) \
    __harray_resize((__harray) arr, new_length, sizeof(arr->data[0]))

#define harray_resize_with_init(arr, new_length, value) ({ \
    typeof(arr->data[0]) __value = value; \
    __harray_resize_with_init((__harray) arr, new_length, &__value, sizeof(arr->data[0]));\
})

#define harray_copy(src, dst) __harray_copy((__harray) src, (__harray) dst, sizeof(src->data[0]))

typedef struct __harray_s {
    size_t length;
    size_t capacity;
    uint8_t* data;
    h_copy_func copy_func;
    h_free_func free_func;
} * __harray;

HEIST_API __harray __harray_new_empty();
HEIST_API __harray __harray_new_copy(__harray src, size_t element_size);
HEIST_API void __harray_insert_empty(__harray arr, size_t pos, size_t length, size_t element_size);
HEIST_API void __harray_erase(__harray arr, size_t pos, size_t length, bool free_element, size_t element_size);
HEIST_API void __harray_resize(__harray arr, size_t new_length, size_t element_size);
HEIST_API void __harray_insert_n(__harray arr, size_t pos, __harray val, size_t element_size);
HEIST_API void __harray_free(__harray arr, size_t element_size);
HEIST_API void __harray_copy(__harray src, __harray dst, size_t element_size);
HEIST_API void __harray_reserve(__harray arr, size_t n, size_t element_size);
HEIST_API void __harray_resize(__harray arr, size_t new_length, size_t element_size);
HEIST_API void __harray_resize_with_init(__harray arr, size_t new_length, hpointer value_ptr, size_t element_size);

#endif // __HARRAY_H__