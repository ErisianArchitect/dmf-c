#ifndef DMF_LIBRARY_REFCOUNT
#define DMF_LIBRARY_REFCOUNT

#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>

// ───── MACROS ────────────────────────────────────────────────

#define atomic _Atomic

// ───── TYPE ALIASES ──────────────────────────────────────────

typedef atomic uint8_t     dmf_refcount8;
typedef atomic uint16_t    dmf_refcount16;
typedef atomic uint32_t    dmf_refcount32;
typedef atomic uint64_t    dmf_refcount64;
typedef atomic size_t      dmf_refcount;

// ────🮤TYPE DECLARATIONS🮥──────────────────────────────────────

// ────🮤TYPE DEFINITIONS🮥───────────────────────────────────────

// ────🮤FUNCTION DECLARATIONS🮥──────────────────────────────────

uint8_t     dmf_refcount8_incr  (volatile   dmf_refcount8   *count);
uint16_t    dmf_refcount16_incr (volatile   dmf_refcount16  *count);
uint32_t    dmf_refcount32_incr (volatile   dmf_refcount32  *count);
uint64_t    dmf_refcount64_incr (volatile   dmf_refcount64  *count);
size_t      dmf_refcount_incr   (volatile   dmf_refcount    *count);

uint8_t     dmf_refcount8_decr  (volatile   dmf_refcount8   *count);
uint16_t    dmf_refcount16_decr (volatile   dmf_refcount16  *count);
uint32_t    dmf_refcount32_decr (volatile   dmf_refcount32  *count);
uint64_t    dmf_refcount64_decr (volatile   dmf_refcount64  *count);
size_t      dmf_refcount_decr   (volatile   dmf_refcount    *count);

void        dmf_refcount8_store (volatile   dmf_refcount8   *count, uint8_t     value);
void        dmf_refcount16_store(volatile   dmf_refcount16  *count, uint16_t    value);
void        dmf_refcount32_store(volatile   dmf_refcount32  *count, uint32_t    value);
void        dmf_refcount64_store(volatile   dmf_refcount64  *count, uint64_t    value);
void        dmf_refcount_store  (volatile   dmf_refcount    *count, size_t      value);


void        dmf_refcount8_reset (volatile   dmf_refcount8   *count);
void        dmf_refcount16_reset(volatile   dmf_refcount16  *count);
void        dmf_refcount32_reset(volatile   dmf_refcount32  *count);
void        dmf_refcount64_reset(volatile   dmf_refcount64  *count);
void        dmf_refcount_reset  (volatile   dmf_refcount    *count);

#if defined(DMF_STRIP_PREFIXES) || defined(DMF_STRIP_PREFIXES_ONCE)
#   define  refcount8           dmf_refcount8
#   define  refcount16          dmf_refcount16
#   define  refcount32          dmf_refcount32
#   define  refcount64          dmf_refcount64
#   define  refcount            dmf_refcount
#   define  refcount8_incr      dmf_refcount8_incr
#   define  refcount16_incr     dmf_refcount16_incr
#   define  refcount32_incr     dmf_refcount32_incr
#   define  refcount64_incr     dmf_refcount64_incr
#   define  refcount_incr       dmf_refcount_incr
#   define  refcount8_decr      dmf_refcount8_decr
#   define  refcount16_decr     dmf_refcount16_decr
#   define  refcount32_decr     dmf_refcount32_decr
#   define  refcount64_decr     dmf_refcount64_decr
#   define  refcount_decr       dmf_refcount_decr
#   define  refcount8_store     dmf_refcount8_store
#   define  refcount16_store    dmf_refcount16_store
#   define  refcount32_store    dmf_refcount32_store
#   define  refcount64_store    dmf_refcount64_store
#   define  refcount_store      dmf_refcount_store
#   define  refcount8_reset     dmf_refcount8_reset
#   define  refcount16_reset    dmf_refcount16_reset
#   define  refcount32_reset    dmf_refcount32_reset
#   define  refcount64_reset    dmf_refcount64_reset
#   define  refcount_reset      dmf_refcount_reset
#undef DMF_STRIP_PREFIXES_ONCE
#endif

#endif