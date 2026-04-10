#include <stddef.h>
#include <stdlib.h>
#include "dmf/core.h"
#include "dmf/mem.h"

static void *dmf_default_malloc(DMF_Allocator *allocator, size_t size) {
    (void)allocator;
    return malloc(size);
}

static void *dmf_default_realloc(DMF_Allocator *allocator, void *mem, size_t new_size) {
    (void)allocator;
    return realloc(mem, new_size);
}

static void *dmf_default_calloc(DMF_Allocator *allocator, size_t elem_count, size_t elem_size) {
    (void)allocator;
    return calloc(elem_count, elem_size);
}

static void  dmf_default_free(DMF_Allocator *allocator, void *mem) {
    (void)allocator;
    free(mem);
}

DMF_Allocator DEFAULT_ALLOCATOR = (DMF_Allocator){
    .data = NULL,
    .malloc = &dmf_default_malloc,
    .realloc = &dmf_default_realloc,
    .calloc = &dmf_default_calloc,
    .free = &dmf_default_free,
};

static DMF_Allocator *GLOBAL_ALLOCATOR = &DEFAULT_ALLOCATOR;

// FUNCTION DEFINITIONS

__force_inline
DMF_Allocator *dmf_get_global_allocator() {
    return GLOBAL_ALLOCATOR;
}

__force_inline
void dmf_set_global_allocator(DMF_Allocator *allocator) {
    GLOBAL_ALLOCATOR = allocator;
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
void dmf_free_in(DMF_Allocator *allocator, void *mem) {
    if (allocator && allocator->free)
        allocator->free(allocator, mem);
    else
        free(mem);
}

__force_inline
void  dmf_free(void *mem) {
    dmf_free_in(GLOBAL_ALLOCATOR, mem);
}