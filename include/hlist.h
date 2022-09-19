#ifndef __HLIST_H__
#define __HLIST_H__

#include "hcommon.h"

#define hlist(T)                                                   \
    struct {                                                       \
        size_t length;                                             \
        __hlist __base_type[0];                                    \
        struct CAT(__heist_hlist_node_##T##_,__LINE__) {           \
            struct CAT(__heist_hlist_node_##T##_,__LINE__)* next;  \
            struct CAT(__heist_hlist_node_##T##_,__LINE__)* prev;  \
            T value;                                               \
        } *front, *back, *__node_type[0];                          \
    } *

#define hlist_node(list) typeof(list->__node_type[0])

#define hlist_new() ((hpointer) __hlist_new())

#define hlist_new_copy(src) ((hpointer) __hlist_new_copy((__hlist) (src), sizeof((src)->__node_type[0]->value)))

#define hlist_push_front(list, val) \
    (((hlist_node(list)) __hlist_push_front_node((__hlist) list, sizeof(list->__node_type[0]->value)))->value = val)

#define hlist_push_back(list, val) \
    (((hlist_node(list)) __hlist_push_back_node((__hlist) list, sizeof(list->__node_type[0]->value)))->value = val)

#define hlist_pop_front(list) ({                                 \
    typeof(list->__node_type[0]->value) val = list->front->value; \
    __hlist_pop_front((__hlist) list);                          \
    val;                                                          \
})

#define hlist_pop_back(list) ({                                 \
    typeof(list->__node_type[0]->value) val = list->back->value; \
    __hlist_pop_back((__hlist) list);                          \
    val;                                                         \
})

#define hlist_foreach(list, val, i)                                \
    for (                                                           \
        struct {                                                    \
            typeof(list->__node_type[0]) n0;                        \
            size_t i0, b0, b1;                                      \
        } __l0 = { list->front, 0, 1, 1 };                          \
        __l0.n0 != NULL;                                            \
        __l0.n0 = __l0.n0->next,                                    \
        __l0.i0++,                                                  \
        __l0.b0 = 1,                                                \
        __l0.b1 = 1                                                 \
    )                                                               \
    for (                                                           \
        size_t i = __l0.i0;                                         \
        __l0.b0;                                                    \
        __l0.b0 = !__l0.b0                                          \
    )                                                               \
    for (                                                           \
        typeof(list->__node_type[0]->value) val = __l0.n0->value;   \
        __l0.b1;                                                    \
        __l0.b1 = !__l0.b1                                          \
    )

#define hlist_empty(list) (list->length == 0)

#define hlist_free(list) ({        \
    __hlist_free((__hlist) list);  \
    list = NULL;                   \
})

#define hlist_set_copy_func(list, func) \
    ((__hlist) list)->copy_func = (h_copy_func) &__heist_copy_func_##func;

#define hlist_set_free_func(list, func) \
    ((__hlist) list)->free_func = (h_free_func) &__heist_free_func_##func;



#define hlist_copy(src, dst) __hlist_copy((__hlist) src, (__hlist) dst)

typedef struct __hlist_node_s* __hlist_node;

typedef struct __hlist_node_s {
    __hlist_node next;
    __hlist_node prev;
    int8_t value[0];
} * __hlist_node;

typedef struct __hlist_s {
    size_t length;
    __hlist_node front;
    __hlist_node back;
    h_copy_func copy_func;
    h_free_func free_func;
} * __hlist;

HEIST_API __hlist __hlist_new();
HEIST_API __hlist_node __hlist_push_front_node(__hlist list, size_t element_size);
HEIST_API __hlist_node __hlist_push_back_node(__hlist list, size_t element_size);
HEIST_API void __hlist_pop_front(__hlist list);
HEIST_API void __hlist_pop_back(__hlist list);
HEIST_API void __hlist_free(__hlist list);
HEIST_API void __hlist_copy(__hlist src, __hlist dst, size_t element_size);
HEIST_API __hlist __hlist_new_copy(__hlist src, size_t element_size);
HEIST_API void __hlist_node_free(__hlist list, __hlist_node node);

#endif // __HLIST_H__