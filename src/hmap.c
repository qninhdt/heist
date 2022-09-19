#include <stdlib.h> // malloc
#include <string.h> // memcpy, memcmp

#include "hmap.h"

static size_t good_primes[] = {
    11, 23, 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157,
    98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917,
    25165843, 50331653, 100663319, 201326611, 201326611, 805306457, 1610612741
};

static size_t i_dont_know = sizeof(good_primes)/sizeof(size_t);

static const char __DELETED;
static const void* DELETED = &__DELETED;

__hmap __hmap_new(h_hash_func hash_func, h_equal_func free_func) {
    __hmap map = malloc(sizeof(struct __hmap_s));

    map->capacity_idx    = 0;
    map->length          = 0;
    map->capacity        = good_primes[map->capacity_idx];
    map->bucket          = calloc(map->capacity, sizeof(__hmap_entry));
    map->ndeleted        = 0;

    map->hash_func       = hash_func;
    map->equal_func      = free_func;

    map->key_copy_func   = NULL;
    map->key_free_func   = NULL;

    map->value_copy_func = NULL;
    map->value_free_func = NULL;

    return map;
}

void __hmap_reserve(__hmap map, size_t new_length, size_t key_size, size_t value_size) {
    size_t required_size = (new_length + map->ndeleted) << 1;

    if (map->capacity < required_size) {
        __hmap_entry* tbucket = map->bucket;

        size_t old_capacity = map->capacity;
        while (map->capacity < required_size) {
            map->capacity = good_primes[++map->capacity_idx];
        }

        map->bucket = calloc(map->capacity, sizeof(__hmap_entry));
        map->ndeleted = 0;

        for (size_t idx = 0; idx < old_capacity; ++idx) {
            __hmap_entry entry = tbucket[idx];
            if (entry == NULL || entry == DELETED) continue;

            uint32_t hashcode = map->hash_func(entry);
            size_t index = hashcode % map->capacity, i = 1;

            while (map->bucket[index] != NULL) {
                index = (hashcode + i*i) % map->capacity;
                i += 1;
            }

            map->bucket[index] = entry;
        }

        free(tbucket);
    }
}

__hmap_entry __hmap_find_or_create_entry(__hmap map, hpointer key, size_t key_size, size_t value_size) {
    uint32_t hashcode = map->hash_func(key);
    size_t index = hashcode % map->capacity, i = 1;

    __hmap_reserve(map, map->length + 1, key_size, value_size);
    __hmap_entry entry;
    while ((entry = map->bucket[index]) != NULL) {
        if (entry != DELETED && map->equal_func(entry, key)) {
            return entry;
        }
        index = (hashcode + i*i) % map->capacity;
        i += 1;
    }

    entry = malloc(key_size + value_size);

    memcpy(entry, key, key_size);
    map->length += 1;

    map->bucket[index] = entry;

    return entry;
}

__hmap_entry __hmap_find_entry(__hmap map, hpointer key, size_t key_size) {
    uint32_t hashcode = map->hash_func(key);
    size_t index = hashcode % map->capacity, i = 1;

    __hmap_entry entry;
    while ((entry = map->bucket[index]) != NULL) {
        if (entry != DELETED && map->equal_func(entry, key)) {
            return entry;
        }
        index = (hashcode + i*i) % map->capacity;
        i += 1;
    }

    // handle error

    return NULL;
}

void __hmap_delete(__hmap map, hpointer key, size_t key_size) {
    uint32_t hashcode = map->hash_func(key);
    size_t index = hashcode % map->capacity, i = 1;

    __hmap_entry entry;
    while ((entry = map->bucket[index]) != NULL) {
        if (entry != DELETED && map->equal_func(entry, key)) {
            if (map->key_free_func) {
                map->key_free_func(entry);
            }
            if (map->value_free_func) {
                map->value_free_func(entry + key_size);
            }
            map->bucket[index] = (__hmap_entry) DELETED;
            map->length -= 1;
            map->ndeleted += 1;

            return;
        }
        index = (hashcode + i*i) % map->capacity;
        i += 1;
    }

    // handle error;
}

void __hmap_free(__hmap map, size_t key_size) {
    for (int idx = 0; idx < map->capacity; ++idx) {
        __hmap_entry entry = map->bucket[idx];

        if (entry == NULL || entry == DELETED) continue;

        if (map->key_free_func) {
            map->key_free_func(entry);
        }

        if (map->value_free_func) {
            map->value_free_func(entry + key_size);
        }

        free(entry);
    }

    free(map);
}

void __hmap_copy(__hmap src, __hmap dst, size_t key_size, size_t value_size) {
    memcpy(dst, src, sizeof(struct __hmap_s));

    dst->bucket = calloc(dst->capacity, sizeof(__hmap_entry));

    for (int idx = 0; idx < src->capacity; ++idx) {
        __hmap_entry entry = src->bucket[idx];

        if (entry == NULL || entry == DELETED) {
            dst->bucket[idx] = entry;
        } else {
            dst->bucket[idx] = malloc(key_size + value_size);

            if (src->key_copy_func) {
                src->key_copy_func(entry, dst->bucket[idx]);
            } else {
                memcpy(dst->bucket[idx], entry, key_size);
            }

            if (src->value_copy_func) {
                src->value_copy_func(entry + key_size, dst->bucket[idx] + key_size);
            } else {
                memcpy(dst->bucket[idx] + key_size, entry + key_size, value_size);
            }
        }
    }
}

__hmap __hmap_new_copy(__hmap src, size_t key_size, size_t value_size) {
    __hmap dst = malloc(sizeof(struct __hmap_s));
    __hmap_copy(src, dst, key_size, value_size);
    return dst;
}