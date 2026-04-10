#include "dmf/core.h"

// FUNCTION DEFINITIONS

void *first_nonnull(size_t argc, ...) {
    va_list args;
    va_start(args, argc);

    for (size_t i = 0; i < argc; i++) {
        void *ptr = va_arg(args, void*);
        if (ptr) {
            va_end(args);
            return ptr;
        }
    }

    va_end(args);
    return NULL;
}

void *first_nonnull_a(size_t argc, void **ptrs) {
    forto(size_t, i, argc) {
        if (ptrs[i]) {
            return ptrs[i];
        }
    }
    return 0;
}