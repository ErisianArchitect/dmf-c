#ifndef DMF_LIBRARY_SYNTAX
#define DMF_LIBRARY_SYNTAX


// ───── ATTRIBUTES ────────────────────────────────────────────
#define __attr(attrib) __attribute__((attrib))

#define __inline                inline
#define __must_use              __attr(warn_unused_result)
#define __no_return             __attr(noreturn)
#define __constructor           __attr(constructor)
#define __destructor            __attr(destructor)
#define __pure                  __attr(pure)
#define __const                 __attr(const)
#define __force_inline          __attr(always_inline) inline
#define __packed                __attr(packed)
#define __align(alignment)      __attr(aligned(alignment))
#define __flatten               __attr(flatten)
#define __no_inline             __attr(noinline)
#define __hot                   __attr(hot)
#define __cold                  __attr(cold)
// warn if null pointer is passed.
#define __non_null                __attr(nonnull)
// when return value is newly allocated pointer and not aliasing anything.
#define __malloc                __attr(malloc)
#define __unused                __attr(unused)
#define __fallthrough           __attr(fallthrough)

// ───── SYNTAX EXTENSIONS ─────────────────────────────────────

#define loop for(;;)
#define forto(type, arg_name, count) for(type arg_name = (type)0; arg_name < (count); arg_name += (type)1)

#endif
