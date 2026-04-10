#include "dmf/core.h"
#include "dmf/mem.h"
#include "dmf/vec.h"
#include <stddef.h>
#include <string.h>

// ───── MACROS ────────────────────────────────────────────────

#define DMF_ELEM_GET(elems, elem_size, index) (void*)(((void*)elems) + ((elem_size) * (index)))

// FUNCTION IMPLEMENTATIONS

__force_inline
DMF_VecStrat dmf_vec_strat_new(DMF_Allocator *allocator, DMF_VecElemDisposer disposer, size_t elem_size) {
    return (DMF_VecStrat){
        allocator,
        disposer,
        elem_size,
    };
}

__force_inline
DMF_VecStrat dmf_vec_sized_strat_new(size_t elem_size) {
    return (DMF_VecStrat){
        NULL,
        {0},
        elem_size,
    };
}

bool dmf_vec_init(DMF_VecStrat *strat, size_t capacity, DMF_Vec *out) {
    if (!strat) return false;
    if (!out) return false;
    size_t alloc_capacity = capacity * strat->elem_size;
    // DMF_Allocator *allocator = first_nonnull(2, strat->allocator, dmf_get_allocator());
    DMF_Allocator *allocator = dmf_allocator_or_global(strat->allocator);
    void *allocation = dmf_malloc_in(allocator, alloc_capacity);
    if (!allocation) return false;
    out->cap = capacity;
    out->elem = allocation;
    out->len = 0;
    out->strat = strat;
    return true;
}

bool dmf_vec_ensure_capacity(DMF_Vec *v, size_t capacity) {
    if (!v) return false;
    if (capacity <= v->cap) return true;
    size_t realloc_size = capacity * v->strat->elem_size;
    void *new_ptr = dmf_realloc_in(v->strat->allocator, v->elem, realloc_size);
    if (!new_ptr) return false;
    v->elem = new_ptr;
    v->cap = capacity;
    return true;
}

__force_inline
bool dmf_vec_reserve(DMF_Vec *v, size_t more) {
    return dmf_vec_ensure_capacity(v, v->len + more);
}

__force_inline
void *dmf_vec_get(DMF_Vec *v, size_t index) {
    if (!v || index >= v->len) return NULL;
    return DMF_ELEM_GET(v->elem, v->strat->elem_size, index);
}

bool _vec_elem_dispose(DMF_VecStrat *strat, void *elem) {
    if(strat
    && strat->disposer.dispose
    && strat->disposer.dispose(strat, elem, strat->disposer.data)
    )
        return true;
    return false;
}

bool dmf_vec_set(DMF_Vec *v, size_t index, void *elem) {
    void* v_elem = dmf_vec_get(v, index);
    if (!v_elem)
        return false;
    if (!_vec_elem_dispose(v->strat, v_elem))
        return false;
    memcpy(v_elem, elem, v->strat->elem_size);
    return true;
}

bool dmf_vec_insert_many(DMF_Vec *v, size_t index, void *elems, size_t count) {
    if (!dmf_vec_reserve(v, count)) return false;
    size_t move_size = count * v->strat->elem_size;
    if (move_size)
    {
        size_t move_count = v->len - index;
        size_t vec_move_size = move_count * v->strat->elem_size;
        void *move_start = DMF_ELEM_GET(v->elem, v->strat->elem_size, index);
        if (vec_move_size) {
            void *move_end = DMF_ELEM_GET(v->elem, v->strat->elem_size, index + count);
            memmove(move_end, move_start, vec_move_size);
        }
        memmove(move_start, elems, move_size);
    }
    v->len += count;
    return true;
}

__force_inline
bool dmf_vec_insert(DMF_Vec *v, size_t index, void *elem) {
    return dmf_vec_insert_many(v, index, elem, 1);
}

bool dmf_vec_push(DMF_Vec *v, void *elem) {
    if (!dmf_vec_reserve(v, 1)) return false;
    size_t index = v->len;
    void *v_elem = DMF_ELEM_GET(v->elem, v->strat->elem_size, index);
    if (!v_elem) return false;
    memcpy(v_elem, elem, v->strat->elem_size);
    v->len++;
    return true;
}

bool dmf_vec_remove(DMF_Vec *v, size_t index, void *elem_out) {
    if (!v || !v->len) return false;
    void *elem = DMF_ELEM_GET(v->elem, v->strat->elem_size, index);
    if (elem_out)
        memcpy(elem_out, elem, v->strat->elem_size);
    else if (v->strat->disposer.dispose)
        v->strat->disposer.dispose(v->strat, elem, v->strat->disposer.data);
    size_t move_size = v->len - (index + 1);
    if (move_size)
        memmove(elem, elem + v->strat->elem_size, move_size * v->strat->elem_size);
    v->len--;
    return true;
}

__flatten
bool dmf_vec_pop(DMF_Vec *v, void *elem_out) {
    if (!v || !v->len) return false;
    return dmf_vec_remove(v, v->len - 1, elem_out);
}

bool dmf_vec_extend(DMF_Vec *v, void *elems, size_t count) {
    if (!dmf_vec_reserve(v, count)) return false;
    for (size_t i = 0; i < count; i++) {
        void *elem = elems + i * v->strat->elem_size;
        if (!dmf_vec_push(v, elem)) return false;
    }
    return true;
}

__flatten
void dmf_vec_free(DMF_Vec *v) {
    if (!v) return;
    while (dmf_vec_pop(v, NULL));
    DMF_Allocator *allocator = dmf_allocator_or_global(v->strat->allocator);
    dmf_free_in(allocator, v->elem);
}