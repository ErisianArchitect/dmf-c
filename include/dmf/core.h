#ifndef DMF_LIBRARY_CORE
#define DMF_LIBRARY_CORE
#include <stddef.h>
#include <stdarg.h>
#include "dmf/syntax.h"         // IWYU pragma: export
#include "dmf/type_aliases.h"   // IWYU pragma: export

// ───── MACROS ────────────────────────────────────────────────

#define STRINGIFY(token) #token
#define SEMVER(major, minor, patch) STRINGIFY(major)"."STRINGIFY(minor)"."STRINGIFY(PATCH)

// ───── TYPE DECLARATIONS ─────────────────────────────────────

// ───── TYPE DEFINITIONS ──────────────────────────────────────

// ───── FUNCTION DECLARATIONS ─────────────────────────────────

void *first_nonnull(size_t argc, ...);

void *first_nonnull_a(size_t argc, void **ptrs);

#endif
