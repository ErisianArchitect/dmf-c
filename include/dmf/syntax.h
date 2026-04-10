#ifndef DMF_LIBRARY_SYNTAX
#define DMF_LIBRARY_SYNTAX


// ───── ATTRIBUTES ────────────────────────────────────────────

#define __attr(attrib) __attribute__((attrib))

#define __inline                inline
#define __pure                  __attr(pure)
#define __const                 __attr(const)
#define __force_inline          __attr(always_inline) inline
#define __align(alignment)      __attr(aligned(alignment))
#define __flatten               __attr(flatten)
#define __noinline              __attr(noinline)
#define __hot                   __attr(hot)
#define __cold                  __attr(cold)

// ───── SYNTAX EXTENSIONS ─────────────────────────────────────

#define loop for(;;)
#define forto(type, arg_name, count) for(type arg_name = (type)0; arg_name < (count); arg_name += (type)1)

#endif