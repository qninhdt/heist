#ifndef __HMAP_H__
#define __HMAP_H__

#include "hcommon.h"
#include "harray.h"
#include "hlist.h"

#define hmap(K, V)                \
    struct {                      \
        size_t length;            \
        __hmap __base_type[0];    \
        struct {                  \
            K key;                \
            V value;              \
        } __hmap_entry_type[0];   \
    } *

#define hmap_entry(map) typeof(map->__hmap_entry_type[0])

#define hmap_find_entry(map, key) ((hmap_entry(map)) __hmap_find_entry(map, key))

#define hmap_find_or_create_entry(map, key) ((hmap_entry(map)) __hmap_find_or_create_entry(map, key))

#define hmap_get(map, key) (hmap_find_entry(map, key).value)

#define hmap_set(map, key, value) (hmap_find_or_create_entry(map, key).value = value)

typedef int8_t* __hmap_entry;

typedef hlist(__hmap_entry) __hmap_bucket;

typedef struct __hmap_s {
    size_t length;

    h_hash_func hash_func;
    h_equal_func equal_func;

    h_copy_func value_copy_func;
    h_free_func value_free_func; 

    h_copy_func key_copy_func;
    h_free_func key_free_func;

    harray(__hmap_bucket) buckets;
} * __hmap;

#endif // __HMAP_H__