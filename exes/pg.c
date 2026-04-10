#include <stdio.h>
#include <string.h>
#define DMF_STRIP_PREFIXES
#include "dmf/vec.h" // IWYU pragma: keep

bool foo(int i, bool r) {
    printf("%d\n", i);
    return r;
}

#define dreturn(retval) ret = retval; goto clean_return

int main(int argc, char **argv) {
    VecStrat ptr_strat = vec_sized_strat_new(sizeof(char*));
    int ret = 0;
    Vec args;
    vec_init(&ptr_strat, argc, &args);
    if (argc <= 1)
        dreturn(1);
    if (!vec_extend(&args, &argv[1], argc-1))
        goto failure;
    for (size_t i = 0; i < args.len; i++) {
        char **s = vec_get(&args, i);
        if (s) {
            if (strcmp(*s, "foo") == 0) {
                goto failure;
            }
            printf("%s\n", *s);
        }
        else
            goto failure;
    }
clean_return:
    vec_free(&args);
    printf("\x1b[32mResources cleaned up.\x1b[0m\n");
    return ret;
failure:
    printf("\x1b[31mThere was a failure. I don't care what it was. fuck it. And while we're at it, fuck you. Now your terminal is broken!");
    ret = 1;
    goto clean_return;
}