#include <stdlib.h> // malloc
#include <string.h> // memcpy

#include "hlist.h"

__hlist __hlist_new() {
    __hlist list = malloc(sizeof(struct __hlist_s));
    list->length = 0;
    list->front = NULL;
    list->back  = NULL;
    list->copy_func = NULL;
    list->free_func = NULL;

    return list;
}

__hlist __hlist_new_copy(__hlist src, size_t element_size) {
    __hlist dst = malloc(sizeof(struct __hlist_s));
    __hlist_copy(src, dst, element_size);
    return dst;
}

__hlist_node __hlist_node_new(size_t element_size) {
    __hlist_node node = malloc(sizeof(struct __hlist_node_s) + element_size);
    node->next = NULL;
    node->prev = NULL;
    return node;
}

__hlist_node __hlist_push_front_node(__hlist list, size_t element_size) {
    __hlist_node node = __hlist_node_new(element_size);

    if (list->front) {
        node->next = list->front;
        list->front->prev = node;
    }

    list->front = node;

    if (list->back == NULL) {
        list->back = node;
    }

    list->length += 1;

    return node;
}

__hlist_node __hlist_push_back_node(__hlist list, size_t element_size) {
    __hlist_node node = __hlist_node_new(element_size);

    if (list->back)  {
        list->back->next = node;
        node->prev = list->back;
    }

    list->back = node;

    if (list->front == NULL) {
        list->front = node;
    }

    list->length += 1;

    return node;
}

void __hlist_node_free(__hlist list, __hlist_node node) {
    if (list->free_func) {
        list->free_func(node->value);
    }
    free(node);
}

__hlist_node __hlist_node_copy(__hlist list, __hlist_node src, size_t element_size) {
    __hlist_node dst = __hlist_node_new(element_size);
    memcpy(dst->value, src->value, element_size);
    
    if (list->copy_func) {
        list->copy_func(src->value, dst->value);
    }

    return dst;
}

void __hlist_pop_front(__hlist list) {
    if (list->front == NULL) {
        // handle error
    }

    __hlist_node old = list->front;

    list->front = old->next;

    if (list->front) {
        list->front->prev = NULL;
    }

    if (list->length == 1) {
        list->back = NULL;
    }

    free(old);
    list->length -= 1;
}

void __hlist_pop_back(__hlist list) {
    if (list->back == NULL) {
        // handle error
    }

    __hlist_node old = list->back;

    list->back = list->back->prev;

    if (list->back) {
        list->back->next = NULL;
    }

    if (list->length == 1) {
        list->front = NULL;
    }

    free(old);
    list->length -= 1;
}

void __hlist_free(__hlist list) {
    __hlist_node node = list->front;
    while (node) {
        __hlist_node next = node->next;
        __hlist_node_free(list, node);
        node = next;
    }

    free(list);
}

void __hlist_copy(__hlist src, __hlist dst, size_t element_size) {
    memcpy(dst, src, sizeof(struct __hlist_s));
    dst->front = NULL;
    dst->back  = NULL;
    
    __hlist_node node = src->front;
    while (node) {
        __hlist_node copy_node = __hlist_node_copy(src, node, element_size);

        if (dst->back)  {
            dst->back->next = copy_node;
            copy_node->prev = dst->back;
        }

        dst->back = copy_node;

        if (dst->front == NULL) {
            dst->front = copy_node;
        }

        node = node->next;
    }
}
