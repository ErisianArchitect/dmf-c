#include <stdatomic.h>
#include "dmf/core.h"
#include "dmf/refcount.h"

#define make_incr_fn(width) \
    uint##width##_t dmf_refcount##width##_incr(volatile dmf_refcount##width *count) { \
        return atomic_fetch_add(count, 1); \
    }

make_incr_fn(8);
make_incr_fn(16);
make_incr_fn(32);
make_incr_fn(64);

__force_inline
size_t dmf_refcount_incr(volatile dmf_refcount *count) {
    return atomic_fetch_add(count, 1);
}

#define make_decr_fn(width) \
    uint##width##_t dmf_refcount##width##_decr(volatile dmf_refcount##width *count) { \
        uint##width##_t cur_count = atomic_load_explicit(count, memory_order_relaxed); \
        if (cur_count == 0) \
            return cur_count; \
        loop { \
            size_t new_count = cur_count - 1; \
            if (atomic_compare_exchange_strong(count, &cur_count, new_count)) \
                return new_count; \
        } \
    }

make_decr_fn(8);
make_decr_fn(16);
make_decr_fn(32);
make_decr_fn(64);

size_t dmf_refcount_decr(volatile dmf_refcount *count) {
    size_t cur_count = atomic_load_explicit(count, memory_order_relaxed);
    if (cur_count == 0)
        return cur_count;
    loop {
        size_t new_count = cur_count - 1;
        if (atomic_compare_exchange_strong(count, &cur_count, new_count))
            return new_count;
    }
}

#define make_store_fn(width) \
    __force_inline \
    void dmf_refcount##width##_store(volatile dmf_refcount##width *count, uint##width##_t value) { \
        atomic_store(count, value); \
    }

make_store_fn(8);
make_store_fn(16);
make_store_fn(32);
make_store_fn(64);

__force_inline
void dmf_refcount_store(volatile dmf_refcount *count, size_t value) {
    atomic_store(count, value);
}

#define make_reset_fn(width) \
    __force_inline \
    void dmf_refcount##width##_reset(volatile dmf_refcount##width *count) { \
        atomic_store(count, 0);\
    }

make_reset_fn(8);
make_reset_fn(16);
make_reset_fn(32);
make_reset_fn(64);

__force_inline
void dmf_refcount_reset(volatile dmf_refcount *count) {
    atomic_store(count, 0);
}