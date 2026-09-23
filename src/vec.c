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
    if (alloc_capacity) {
        DMF_Allocator *allocator = dmf_allocator_or_global(strat->allocator);
        void *allocation = dmf_malloc_in(allocator, alloc_capacity);
        if (!allocation) return false;
        out->elems = allocation;
    } else {
        out->elems = NULL;
    }
    out->cap = capacity;
    out->len = 0;
    out->strat = strat;
    return true;
}

bool dmf_vec_ensure_capacity(DMF_Vec *v, size_t capacity) {
    if (!v) return false;
    if (capacity <= v->cap) return true;
    size_t alloc_size = capacity * v->strat->elem_size;
    // void *new_ptr = NULL;
    void *new_ptr = (v->elems)
        ? dmf_realloc_in(v->strat->allocator, v->elems, alloc_size)
        : dmf_malloc_in(v->strat->allocator, alloc_size);
    if (!new_ptr)
        return false;
    v->elems = new_ptr;
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
    return DMF_ELEM_GET(v->elems, v->strat->elem_size, index);
}

bool _vec_elem_dispose(DMF_VecStrat *strat, void *elem) {
    // sometimes you just write weird stuff
    if (!strat || !strat->disposer.dispose)
        return true;
    return strat->disposer.dispose(strat, elem, strat->disposer.data);
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

bool dmf_vec_insert_many(DMF_Vec *v, size_t index, size_t count, void *elems) {
    if (!dmf_vec_reserve(v, count)) return false;
    if (index > v->len)
        return false;
    size_t move_size = count * v->strat->elem_size;
    if (move_size)
    {
        size_t move_count = v->len - index;
        size_t vec_move_size = move_count * v->strat->elem_size;
        void *move_start = DMF_ELEM_GET(v->elems, v->strat->elem_size, index);
        if (vec_move_size) {
            void *move_end = DMF_ELEM_GET(v->elems, v->strat->elem_size, index + count);
            memmove(move_end, move_start, vec_move_size);
        }
        memmove(move_start, elems, move_size);
    }
    v->len += count;
    return true;
}

__force_inline
bool dmf_vec_insert(DMF_Vec *v, size_t index, void *elem) {
    return dmf_vec_insert_many(v, index, 1, elem);
}

bool dmf_vec_push(DMF_Vec *v, void *elem) {
    if (!dmf_vec_reserve(v, 1)) return false;
    size_t index = v->len;
    void *v_elem = DMF_ELEM_GET(v->elems, v->strat->elem_size, index);
    if (!v_elem) return false;
    memcpy(v_elem, elem, v->strat->elem_size);
    v->len++;
    return true;
}

bool dmf_vec_remove_many(DMF_Vec *v, size_t index, size_t count, void *elems_out) {
    if (!count) return true;
    size_t remove_end = index + count;
    if (!v || remove_end > v->len) return false;
    void *elems_start = DMF_ELEM_GET(v->elems, v->strat->elem_size, index);
    if (elems_out) {
        size_t move_out_size = count * v->strat->elem_size;
        memcpy(elems_out, elems_start, move_out_size);
    } else if (v->strat->disposer.dispose) {
        for (size_t i = 0; i < count; i++) {
            void *dispose_elem = DMF_ELEM_GET(elems_start, v->strat->elem_size, i);
            if (!v->strat->disposer.dispose(v->strat, dispose_elem, v->strat->disposer.data))
                // TODO: I think maybe all elements should attempt to dispose before returning false.
                //|      Perhaps just ignore failures?
                return false;
        }
    }
    size_t remain_count = v->len - remove_end;
    if (remain_count) {
        size_t remain_move_size = remain_count * v->strat->elem_size;

    }
    v->len -= count;
    return true;
}

bool dmf_vec_remove(DMF_Vec *v, size_t index, void *elem_out) {
    if (!v || index >= v->len) return false;
    void *elem = DMF_ELEM_GET(v->elems, v->strat->elem_size, index);
    if (elem_out)
        memcpy(elem_out, elem, v->strat->elem_size);
    else if(!_vec_elem_dispose(v->strat, elem))
        return false;
    size_t move_size = v->len - (index + 1);
    if (move_size)
        memmove(elem, elem + v->strat->elem_size, move_size * v->strat->elem_size);
    v->len--;
    return true;
}

bool dmf_vec_swap_remove(DMF_Vec *v, size_t index, void *elem_out) {
    void *elem = NULL;
    if (!(elem = dmf_vec_get(v, index))) return false;
    if (elem_out)
        memcpy(elem_out, elem, v->strat->elem_size);
    else if (!_vec_elem_dispose(v->strat, elem))
        return false;
    size_t last_index = v->len - 1;
    if (last_index == index) {
        v->len--;
        return true;
    }
    void *last_elem = DMF_ELEM_GET(v->elems, v->strat->elem_size, last_index);
    memcpy(elem, last_elem, v->strat->elem_size);
    v->len--;
    return true;
}

__flatten
bool dmf_vec_pop(DMF_Vec *v, void *elem_out) {
    if VEC_EMPTY_COND(v) return false;
    return dmf_vec_remove(v, v->len - 1, elem_out);
}

// TODO: dmf_vec_extend_at

bool dmf_vec_extend(DMF_Vec *v, size_t count, void *elems) {
    return dmf_vec_insert_many(v, v->len, count, elems);
}

__flatten
void dmf_vec_free(DMF_Vec *v) {
    if (!v) return;
    while (dmf_vec_pop(v, NULL));
    DMF_Allocator *allocator = dmf_allocator_or_global(v->strat->allocator);
    dmf_free_in(allocator, v->elems);
}