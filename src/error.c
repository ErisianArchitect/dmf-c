#include "dmf/core.h"
#include "dmf/type_aliases.h"
#include "dmf/error.h"

struct StaticError {
    DMF_Error       last_error;
    DMF_ErrorStrat  *last_strat;
    bool            error_set;
};

static struct StaticError last_error = {0};

DMF_Error dmf_error_new(char *message, void *data, int exit_code, u16 flags) {
    return (DMF_Error){
        .message = message,
        .data = data,
        .exit_code = exit_code,
        .flags = flags,
    };
}

DMF_ErrorStrat dmf_error_strat_new(DMF_ErrorPrint *print, DMF_ErrorDispose *dispose) {
    return (DMF_ErrorStrat){
        .print = print,
        .dispose = dispose,
    };
}

void dmf_handle_last_error() {
    if (!last_error.error_set) return;
}

void dmf_set_error(DMF_Error err, DMF_ErrorStrat *strat) {
    dmf_handle_last_error();

}