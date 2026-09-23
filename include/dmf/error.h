#ifndef DMF_LIBRARY_ERROR
#define DMF_LIBRARY_ERROR
#include "dmf/core.h"
#include "dmf/type_aliases.h"

// ────🮤MACROS🮥─────────────────────────────────────────────────

// ───🮤 FLAGS 🮥─────────────────────────────────────────────────

#define EF_ABORT                ((u16)0x01) /* Error demands that the program must abort. */
#define EF_EXIT                 ((u16)0x02) /* Error demands that the program is exited using the error's exit code. */
#define EF_NEEDS_HANDLE         ((u16)0x04) /* Error must be handled. */
#define EF_TEST_FLAG(e, flag)   (((e) & (flag)) == (flag))

// ────🮤TYPE DECLARATIONS🮥──────────────────────────────────────

typedef struct DMF_ErrorStrat   DMF_ErrorStrat;
typedef struct DMF_Error        DMF_Error;

typedef void (DMF_ErrorPrint)(DMF_Error *err, bool color);
typedef void (DMF_ErrorDispose)(DMF_Error *err);

// ────🮤TYPE DEFINITIONS🮥───────────────────────────────────────

struct DMF_ErrorStrat {
    DMF_ErrorPrint *print;
    DMF_ErrorDispose *dispose;
};

struct DMF_Error {
    char *message;
    void *data;
    int exit_code;
    u32 flags;
};

// ────🮤FUNCTION DEFINITIONS🮥───────────────────────────────────

// These are general-purpose library functions, which means that
// the names don't follow the typical
// `library_namespace_identifier` convention.
// Instead, they are `library_identifier[_error]`.

// DMF_Error   dmf_error_new(char *message, void *data, int exit_code, int flags);
// void        dmf_error_print(DMF_Error *err);
// bool        dmf_error_dispose(DMF_Error *err);
DMF_Error       dmf_error_new(char *message, void *data, int exit_code, u16 flags);
DMF_ErrorStrat  dmf_error_strat_new(DMF_ErrorPrint *print, DMF_ErrorDispose *dispose);

void            dmf_set_error(DMF_Error err, DMF_ErrorStrat *strat); 
bool            dmf_has_error();
void            dmf_handle_last_error();
bool            dmf_last_error_has_flags(u16 flags);
int             dmf_last_error_exit_code();

void            dmf_print_last_error();
void            dmf_dispose_last_error();

// ────🮤DO NOT WRITE CODE PAST THIS POINT🮥──────────────────────
#endif