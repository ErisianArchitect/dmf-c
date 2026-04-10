#ifndef DMF_LIBRARY_MEM
#define DMF_LIBRARY_MEM

#include <stddef.h>

// ────🮤TYPE DECLARATIONS🮥──────────────────────────────────────

typedef struct DMF_Allocator DMF_Allocator;
typedef void *(DMF_Malloc)(DMF_Allocator *allocator, size_t size);
typedef void *(DMF_Realloc)(DMF_Allocator *allocator, void *mem, size_t new_size);
typedef void *(DMF_Calloc)(DMF_Allocator * allocator, size_t elem_count, size_t elem_size);
typedef void (DMF_Free)(DMF_Allocator *allocator, void *mem);

struct DMF_Allocator {
    void *data;
    DMF_Malloc *malloc;
    DMF_Realloc *realloc;
    DMF_Calloc *calloc;
    DMF_Free *free;
};

// ────🮤FUNCTION DECLARATIONS🮥──────────────────────────────────

DMF_Allocator  *dmf_get_global_allocator();
void            dmf_set_global_allocator(DMF_Allocator *allocator);

DMF_Allocator  *dmf_get_allocator_or(DMF_Allocator *fallback);
DMF_Allocator  *dmf_get_allocator_or_default();

DMF_Allocator  *dmf_allocator_or(DMF_Allocator *allocator, DMF_Allocator *fallback);
DMF_Allocator  *dmf_allocator_or_default(DMF_Allocator *allocator);
DMF_Allocator  *dmf_allocator_or_global(DMF_Allocator *allocator);
DMF_Allocator  *dmf_allocator_or_global_or_default(DMF_Allocator *allocator);

void           *dmf_malloc_in(DMF_Allocator *allocator, size_t size);
void           *dmf_malloc(size_t size);
void           *dmf_realloc_in(DMF_Allocator *allocator, void *mem, size_t new_size);
void           *dmf_realloc(void *mem, size_t new_size);
void           *dmf_calloc_in(DMF_Allocator *allocator, size_t elem_count, size_t elem_size);
void           *dmf_calloc(size_t elem_count, size_t elem_size);
void            dmf_free_in(DMF_Allocator *allocator, void *mem);
void            dmf_free(void *mem);

// ────🮤DO NOT WRITE CODE PAST THIS POINT🮥──────────────────────
#endif