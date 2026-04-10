#ifndef DMF_LIBRARY_STR
#define DMF_LIBRARY_STR

#include <stdint.h>
#include <stdatomic.h>

#include "dmf/core.h"
#include "dmf/refcount.h"

// ───── MACROS ────────────────────────────────────────────────

#define INDIRECT_TAG 0xF

// ───── TYPE DEFINITIONS ──────────────────────────────────────

typedef struct dmf_str_slice dmf_str_slice;
typedef struct dmf_str_dummy dmf_str_dummy;
typedef struct dmf_str_inline dmf_str_inline;
typedef struct dmf_str_indirect dmf_str_indirect;
typedef struct dmf_str dmf_str;

// ───── TYPE ALIASES ──────────────────────────────────────────

typedef atomic uint32_t atomic32;

// ───── CONSTANTS ─────────────────────────────────────────────

#define I_FLAG(bits) ((uint8_t)(bits))
#define I_EMPTY I_FLAG(0)
#define I_STATIC I_FLAG(1)
#define I_HEAP I_FLAG(2)
#define I_RC I_FLAG(3)

// ───── TYPE DEFINITIONS ──────────────────────────────────────

struct dmf_str_slice {
    const char *s;
    size_t      len;
};

struct __align(8) dmf_str_dummy {
    uint64_t inner[2];
};

struct __align(8) dmf_str_inline {
    char    bytes[15];
    uint8_t len;
};

struct __align(8) dmf_str_indirect {
    const char *s;
    alignas(8)
    uint32_t    len;
    uint16_t    flags;
    uint8_t     type;
    uint8_t     _footer;
};

struct __align(8) dmf_str {
    union {
        dmf_str_inline      inline_str;
        dmf_str_indirect    indirect_str;
    };
};

// ───── FUNCTION DECLARATIONS ─────────────────────────────────

dmf_str         dmf_str_new_static  (const char     *s);
dmf_str         dmf_str_new         (const char     *s);
dmf_str         dmf_str_new_rc      (const char     *s);
dmf_str_slice   dmf_str_to_slice    (dmf_str        *s);
dmf_str         dmf_slice_to_str    (dmf_str_slice  *s);

const char     *dmf_str_ptr         (dmf_str        *s);
uint32_t        dmf_str_len         (dmf_str        *s);

#endif