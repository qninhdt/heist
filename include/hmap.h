#ifndef __HMAP_H__
#define __HMAP_H__

#include "hcommon.h"
#include "harray.h"

#define hmap(K, V)                \
    struct {                      \
        size_t length;            \
        size_t capacity;          \
        __hmap __base_type[0];    \
        struct {                  \
            K key;                \
            V value;              \
        } * __hmap_entry_type[0]; \
    } *

#define hmap_new(hash_func, equal_func)                 \
    ((hpointer) __hmap_new(                             \
        (h_hash_func) &__heist_hash_func_##hash_func,   \
        (h_equal_func) &__heist_equal_func_##equal_func \
    ))

#define hmap_entry(map) typeof(map->__hmap_entry_type[0])

#define hmap_find_entry(map, key_) ({ \
    typeof(map->__hmap_entry_type[0]->key) __key = key_; \
    ((hmap_entry(map)) __hmap_find_entry((__hmap) map, &__key, sizeof(map->__hmap_entry_type[0]->key))); \
})

#define hmap_find_or_create_entry(map, key_) ({ \
    typeof(map->__hmap_entry_type[0]->key) __key = key_; \
    ((hmap_entry(map)) __hmap_find_or_create_entry((__hmap) map, &__key, sizeof(map->__hmap_entry_type[0]->key), sizeof(map->__hmap_entry_type[0]->value))); \
})

#define hmap_get(map, key) (hmap_find_entry(map, key)->value)

#define hmap_set(map, key_, value_) \
    (hmap_find_or_create_entry(map, key_)->value = value_)      

#define hmap_reserve(map, length) \
    __hmap_reserve((__hmap) map, length, sizeof(map->__hmap_entry_type[0]->key), sizeof(map->__hmap_entry_type[0]->value))       

#define hmap_delete(map, key_) ({ \
    typeof(map->__hmap_entry_type[0]->key) __key = key_; \
    __hmap_delete((__hmap) map, &__key, sizeof(map->__hmap_entry_type[0]->key)); \
})

typedef int8_t* __hmap_entry;

typedef struct __hmap_s {
    size_t length;
    size_t capacity;
    size_t capacity_idx;
    size_t ndeleted;

    h_hash_func hash_func;
    h_equal_func equal_func;

    h_copy_func value_copy_func;
    h_free_func value_free_func; 

    h_copy_func key_copy_func;
    h_free_func key_free_func;

    __hmap_entry* bucket;
} * __hmap;

HEIST_API __hmap __hmap_new(h_hash_func hash_func, h_equal_func free_func);
HEIST_API __hmap_entry __hmap_find_entry(__hmap map, hpointer key, size_t key_size);
HEIST_API __hmap_entry __hmap_find_or_create_entry(__hmap map, hpointer key, size_t key_size, size_t value_size);
HEIST_API void __hmap_reserve(__hmap map, size_t new_length, size_t key_size, size_t value_size);
HEIST_API void __hmap_delete(__hmap map, hpointer key, size_t key_size);

#endif // __HMAP_H__