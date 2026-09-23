#ifndef DMF_LIBRARY_VEC
#define DMF_LIBRARY_VEC

#include <stddef.h>
#include <stdbool.h>
#include "dmf/mem.h"

// ────🮤MACROS🮥─────────────────────────────────────────────────

#define dmf_vec_type_strat(type) dmf_vec_sized_strat_new(sizeof(type))
#define slice_args(slice) slice.elems, slice.len
#define VEC_EMPTY_COND(v) (!(v) || !(v)->len)

// ────🮤TYPE DECLARATIONS🮥──────────────────────────────────────

typedef struct DMF_VecMapper DMF_VecMapper;
typedef struct DMF_VecStrat DMF_VecStrat;
typedef struct DMF_Vec_Slice DMF_Vec_Slice;
typedef struct DMF_Vec DMF_Vec;

typedef bool (DMF_VecMapFn)(DMF_Vec *v, size_t index, void *elem, void *data);

typedef bool (DMF_VecElemDispose)(DMF_VecStrat *strat, void *elem, void *data);
typedef struct DMF_VecElemDisposer DMF_VecElemDisposer;

// ────🮤TYPE DEFINITIONS🮥───────────────────────────────────────

struct DMF_VecMapper {
    DMF_VecMapFn *map;
    void *data;
};

struct DMF_VecElemDisposer {
    DMF_VecElemDispose *dispose;
    void *data;
};

struct DMF_VecStrat {
    DMF_Allocator *allocator;
    DMF_VecElemDisposer disposer;
    size_t elem_size;
};

struct DMF_Vec_Slice {
    void *elems;
    size_t len;
};

struct DMF_Vec {
    union {
        struct {
            void *elems;
            size_t len;
        };
        DMF_Vec_Slice slice;
    };
    size_t cap;
    DMF_VecStrat *strat;
};

// ────🮤FUNCTION DECLARATIONS🮥──────────────────────────────────

DMF_VecStrat    dmf_vec_strat_new(DMF_Allocator *allocator, DMF_VecElemDisposer disposer, size_t elem_size);
DMF_VecStrat    dmf_vec_sized_strat_new(size_t elem_size);
bool            dmf_vec_init(DMF_VecStrat *strat, size_t capacity, DMF_Vec *out);
bool            dmf_vec_ensure_capacity(DMF_Vec *v, size_t capacity);
bool            dmf_vec_reserve(DMF_Vec *v, size_t more);
void           *dmf_vec_get(DMF_Vec *v, size_t index);
bool            dmf_vec_set(DMF_Vec *v, size_t index, void *elem);
bool            dmf_vec_insert_many(DMF_Vec *v, size_t index, size_t count, void *elems);
bool            dmf_vec_insert(DMF_Vec *v, size_t index, void *elem);
bool            dmf_vec_remove(DMF_Vec *v, size_t index, void *elem_out);
bool            dmf_vec_swap_remove(DMF_Vec *v, size_t index, void *elem_out);
bool            dmf_vec_push(DMF_Vec *v, void *elem);
bool            dmf_vec_pop(DMF_Vec *v, void *elem_out);
bool            dmf_vec_extend(DMF_Vec *v, size_t count, void *elems);
void            dmf_vec_free(DMF_Vec *v);

// ────🮤STRIP PREFIXES🮥─────────────────────────────────────────

#if defined(DMF_STRIP_PREFIXES) || defined(DMF_STRIP_PREFIXES_ONCE)
// ────🮤TYPES🮥──────────────────────────────────────────────────
#   define  VecElemDisposer     DMF_VecElemDisposer
#   define  VecStrat            DMF_VecStrat
#   define  Vec                 DMF_Vec
#   define  VecMapFn            DMF_VecMapFn
// ────🮤FUNCTIONS🮥──────────────────────────────────────────────
#   define  vec_strat_new       dmf_vec_strat_new
#   define  vec_sized_strat_new dmf_vec_sized_strat_new
#   define  vec_init            dmf_vec_init
#   define  vec_ensure_capacity dmf_vec_ensure_capacity
#   define  vec_reserve         dmf_vec_reserve
#   define  vec_get             dmf_vec_get
#   define  vec_set             dmf_vec_set
#   define  vec_insert_many     dmf_vec_insert_many
#   define  vec_insert          dmf_vec_insert
#   define  vec_remove          dmf_vec_remove
#   define  vec_swap_remove     dmf_vec_swap_remove
#   define  vec_push            dmf_vec_push
#   define  vec_pop             dmf_vec_pop
#   define  vec_extend          dmf_vec_extend
#   define  vec_free            dmf_vec_free
// ────🮤UNDEF🮥──────────────────────────────────────────────────
#undef DMF_STRIP_PREFIXES_ONCE
#endif

#endif