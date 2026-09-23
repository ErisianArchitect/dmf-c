#include <stddef.h>
#include <stdlib.h>
#include "dmf/core.h"
#include "dmf/mem.h"

#if defined (_WIN32)
#   include <malloc.h>
#endif

__attribute__((malloc, alloc_size(2)))
static void *dmf_default_malloc(DMF_Allocator *allocator, size_t size) {
    (void)allocator;
    return malloc(size);
}

__attribute__((malloc, alloc_size(3)))
static void *dmf_default_realloc(DMF_Allocator *allocator, void *mem, size_t new_size) {
    (void)allocator;
    return realloc(mem, new_size);
}

__attribute__((malloc, alloc_size(2, 3)))
static void *dmf_default_calloc(DMF_Allocator *allocator, size_t elem_count, size_t elem_size) {
    (void)allocator;
    return calloc(elem_count, elem_size);
}

__attribute__((malloc, alloc_size(3)))
static void *dmf_default_aligned_alloc(DMF_Allocator *allocator, size_t alignment, size_t size) {
    (void)allocator;
    #if defined (_WIN32)
        return _aligned_malloc(alignment, size);
    #elif defined (__linux__) || defined(__APPLE__)
        return aligned_alloc(alignment, size);
    #else
    #error "Unknown platform"
    #endif
}

static void dmf_default_free(DMF_Allocator *allocator, void *mem) {
    (void)allocator;
    free(mem);
}

static void dmf_default_aligned_free(DMF_Allocator *allocator, void *mem) {
    (void)allocator;
    #if defined (_WIN32)
        _aligned_free(mem);
    #elif defined (__linux__) || defined(__APPLE__)
        free(mem);
    #else
    #error "Unknown platform"
    #endif
}

DMF_Allocator DEFAULT_ALLOCATOR = (DMF_Allocator){
    .data = NULL,
    .malloc = &dmf_default_malloc,
    .realloc = &dmf_default_realloc,
    .calloc = &dmf_default_calloc,
    .aligned_alloc = &dmf_default_aligned_alloc,
    .free = &dmf_default_free,
    .aligned_free = &dmf_default_aligned_free,
};

static DMF_Allocator *GLOBAL_ALLOCATOR = &DEFAULT_ALLOCATOR;

// FUNCTION DEFINITIONS

__force_inline
DMF_Allocator *dmf_get_global_allocator() {
    return GLOBAL_ALLOCATOR;
}

__force_inline
void dmf_set_global_allocator(DMF_Allocator *allocator) {
    if (allocator)
        GLOBAL_ALLOCATOR = allocator;
    else
        GLOBAL_ALLOCATOR = &DEFAULT_ALLOCATOR;
}

__force_inline
DMF_Allocator *dmf_get_allocator_or(DMF_Allocator *fallback) {
    if (GLOBAL_ALLOCATOR) return GLOBAL_ALLOCATOR;
    return fallback;
}

__force_inline
DMF_Allocator *dmf_get_allocator_or_default() {
    if (GLOBAL_ALLOCATOR) return GLOBAL_ALLOCATOR;
    return &DEFAULT_ALLOCATOR;
}

__force_inline
DMF_Allocator *dmf_allocator_or_else(DMF_Allocator *allocator, DMF_Allocator *fallback) {
    if (allocator) return allocator;
    return fallback;
}

__force_inline
DMF_Allocator *dmf_allocator_or_default(DMF_Allocator *allocator) {
    if (allocator) return allocator;
    return &DEFAULT_ALLOCATOR;
}

__force_inline
DMF_Allocator *dmf_allocator_or_global(DMF_Allocator *allocator) {
    if (allocator) return allocator;
    return GLOBAL_ALLOCATOR;
}

__force_inline
DMF_Allocator *dmf_allocator_or_global_or_default(DMF_Allocator *allocator) {
    if (allocator) return allocator;
    if (GLOBAL_ALLOCATOR) return GLOBAL_ALLOCATOR;
    return &DEFAULT_ALLOCATOR;
}

__force_inline
__attribute__((malloc, alloc_size(2)))
void *dmf_malloc_in(DMF_Allocator *allocator, size_t size) {
    if (allocator && allocator->malloc)
        return allocator->malloc(allocator, size);
    return malloc(size);
}

__force_inline
__attribute__((malloc, alloc_size(1)))
void *dmf_malloc(size_t size) {
    return dmf_malloc_in(GLOBAL_ALLOCATOR, size);
}

__force_inline
__attribute__((malloc, alloc_size(3)))
void *dmf_realloc_in(DMF_Allocator *allocator, void *mem, size_t new_size) {
    if (allocator && allocator->realloc)
        return allocator->realloc(allocator, mem, new_size);
    return realloc(mem, new_size);
}

__force_inline
__attribute__((malloc, alloc_size(2)))
void *dmf_realloc(void *mem, size_t new_size) {
    return dmf_realloc_in(GLOBAL_ALLOCATOR, mem, new_size);
}

__force_inline
__attribute__((malloc, alloc_size(2, 3)))
void *dmf_calloc_in(DMF_Allocator *allocator, size_t elem_count, size_t elem_size) {
    if (allocator && allocator->calloc)
        return allocator->calloc(allocator, elem_count, elem_size);
    return calloc(elem_count, elem_size);
}

__force_inline
__attribute__((malloc, alloc_size(1, 2)))
void *dmf_calloc(size_t elem_count, size_t elem_size) {
    return dmf_calloc_in(GLOBAL_ALLOCATOR, elem_count, elem_size);
}

__force_inline
__attribute__((malloc, alloc_size(3)))
void *dmf_aligned_alloc_in(DMF_Allocator *allocator, size_t alignment, size_t size) {
    if (allocator && allocator->aligned_alloc)
        return allocator->aligned_alloc(allocator, alignment, size);
    return dmf_default_aligned_alloc(allocator, alignment, size);
}

__force_inline
__attribute__((malloc, alloc_size(2)))
void *dmf_aligned_alloc(size_t alignment, size_t size) {
    return dmf_aligned_alloc_in(GLOBAL_ALLOCATOR, alignment, size);
}

__force_inline
void dmf_free_in(DMF_Allocator *allocator, void *mem) {
    if (allocator && allocator->free)
        allocator->free(allocator, mem);
    else
        free(mem);
}

__force_inline
void dmf_free(void *mem) {
    dmf_free_in(GLOBAL_ALLOCATOR, mem);
}

__force_inline
void dmf_aligned_free_in(DMF_Allocator *allocator, void *mem) {
    if (allocator && allocator->aligned_free)
        allocator->aligned_free(allocator, mem);
    else
        dmf_default_aligned_free(allocator, mem);
}

__force_inline
void dmf_aligned_free(void *mem) {
    dmf_free_in(GLOBAL_ALLOCATOR, mem);
}