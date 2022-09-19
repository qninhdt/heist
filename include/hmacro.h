#ifndef __HMACRO_H__
#define __HMACRO_H__

#define FIRST(a, ...) a
#define SECOND(a, b, ...) b

#define IS_PROBE(...) SECOND(__VA_ARGS__, _0)
#define PROBE() ~, _1

#define CAT(a,b) CAT_(a,b)
#define CAT_(a,b) a ## b
#define CAT3(a, b, ...) a ## b ## __VA_ARGS__

#define NOT(x) IS_PROBE(_x())
#define _0() PROBE()

#define BOOL(x) NOT(NOT(x))

#define HAS_ARGS(...) BOOL(FIRST(__VA_ARGS__ _END_OF_ARGUMENTS_)())
#define _END_OF_ARGUMENTS_() _0

#define IF(c) _IF(BOOL(c))
#define _IF(c) CAT(_IF,c)

#define IF_ELSE(c) _IF_ELSE(BOOL(c))
#define _IF_ELSE(c) CAT(_IF, c)

#define _IF_1(...) __VA_ARGS__ _IF_1_ELSE
#define _IF_0(...)             _IF_0_ELSE

#define _IF_1_ELSE(...)
#define _IF_0_ELSE(...) __VA_ARGS__

#define EVAL(...) EVAL1024(__VA_ARGS__)
#define EVAL1024(...) EVAL512(EVAL512(__VA_ARGS__))
#define EVAL512(...) EVAL256(EVAL256(__VA_ARGS__))
#define EVAL256(...) EVAL128(EVAL128(__VA_ARGS__))
#define EVAL128(...) EVAL64(EVAL64(__VA_ARGS__))
#define EVAL64(...) EVAL32(EVAL32(__VA_ARGS__))
#define EVAL32(...) EVAL16(EVAL16(__VA_ARGS__))
#define EVAL16(...) EVAL8(EVAL8(__VA_ARGS__))
#define EVAL8(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL4(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL1(...) __VA_ARGS__

#define PASS(...) __VA_ARGS__
#define EMPTY()
#define COMMA() ,
#define PLUS() +
#define ZERO() 0
#define ONE() 1

#define DEFER2(id) id EMPTY EMPTY()()
#define DEFER3(id) id EMPTY EMPTY EMPTY()()()
#define DEFER4(id) id EMPTY EMPTY EMPTY EMPTY()()()()
#define DEFER5(id) id EMPTY EMPTY EMPTY EMPTY EMPTY()()()()()
#define DEFER6(id) id EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY()()()()()()
#define DEFER7(id) id EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY()()()()()()()
#define DEFER8(id) id EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY()()()()()()()()

#define MAP_PAIRS(op,sep,...) \
    IF(HAS_ARGS(__VA_ARGS__))(EVAL(MAP_PAIRS_INNER(op,sep,__VA_ARGS__)))

#define MAP_PAIRS_INNER(op, sep, cur_val_1, cur_val_2, ...) \
    op(cur_val_1,cur_val_2) \
    IF(HAS_ARGS(__VA_ARGS__))( \
        sep() DEFER2(_MAP_PAIRS_INNER)()(op, sep, __VA_ARGS__) \
    )
    
#define _MAP_PAIRS_INNER() MAP_PAIRS_INNER

#define UNIQUE_VAR() CAT(__u_, __FUNCTION__)

#define HEIST_API extern

#define H_COPY_FUNC(T, func, arg) \
    T func(T arg);\
    void __heist_copy_func_##func(T* src, T* dst) { \
        *dst = func(*src); \
    }\
    T func(T arg)

#define H_FREE_FUNC(T, func, arg) \
    void func(T arg);\
    void __heist_free_func_##func(T* src) { \
        func(*src); \
    }\
    void func(T arg)

#define H_HASH_FUNC(K, func, key)                  \
    uint32_t func(K key);                          \
    uint32_t __heist_hash_func_##func(K* key_ptr) {\
        return func(*key_ptr);                     \
    }                                              \
    uint32_t func(K key)

#define H_EQUAL_FUNC(K, func, a, b)                     \
    bool func(K a, K b);                                \
    bool __heist_equal_func_##func(K* a_ptr, K* b_ptr) {\
        return func(*a_ptr, *b_ptr);                    \
    }                                                   \
    bool func(K a, K b)  

#define __heist_copy_func_NULL NULL
#define __heist_free_func_NULL NULL
#define __heist_equak_func_NULL NULL
#define __heist_hash_func_NULL NULL

#endif // __HMACRO_H__
