#ifndef __HCOMMON_H__
#define __HCOMMON_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "hmacro.h"

typedef void* hpointer;

typedef void (*h_copy_func)(hpointer src, hpointer dst);
typedef void (*h_free_func)(hpointer element);
typedef uint32_t (*h_hash_func)(hpointer key);
typedef bool (*h_equal_func)(hpointer x, hpointer y);


#endif // __HCOMMON_H__
