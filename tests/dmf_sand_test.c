#include <stdio.h>
#include <assert.h>
#include "dmf/core.h"
#include "dmf/test.h"
#define DMF_STRIP_PREFIXES
#include "dmf/vec.h"

static VecStrat CSTR_STRAT = (VecStrat){
    NULL,
    {0},
    sizeof(char *),
};

int main() {

    void *cleanup_return = &&return_success;

    Vec vec;
    if (!vec_init(&CSTR_STRAT, 0, &vec))
    {
        printfail("dmf_vec.h: Failed to create a new vec.");
    } else {
        printpass("dmf_vec.h: Initialized vec successfully.");
    }
    char *s = NULL;
    vec_push(&vec, &(char *){"Hello, world!"});
    vec_push(&vec, &(char *){"This is a test."});
    while (vec_pop(&vec, &s)) {
        if (!s)
            continue;
        printf("%s\n", s);
    }
    goto cleanup;
return_success:
    return 0;
cleanup:
    vec_free(&vec);
    if (cleanup_return) goto *cleanup_return;
    else return 0;
}