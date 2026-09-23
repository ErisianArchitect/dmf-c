#include <stdio.h>
// #include <string.h>
#include "dmf/core.h"
#define DMF_STRIP_PREFIXES
#include "dmf/vec.h" // IWYU pragma: keep
#include <stdnoreturn.h>

// ────🮤MACROS🮥─────────────────────────────────────────────────

#define EXIT_SUCCESS ((int)0)
#define EXIT_FAILURE ((int)1)
#define asm __asm__

// ────🮤FUNCTIONS🮥──────────────────────────────────────────────

bool test_disposer(__unused VecStrat *strat, void *elem, __unused void *data) {
    char *s = *(char**)elem;
    if (s)
        printf("disposing: \"%s\"\n", s);
    else
        printf("null string during dispose.");
    return true;
}

noreturn void sys_exit(int code) {
    __asm__ __volatile__ (
        "syscall"
            :
            : "a" (0x3c), "D" (code)
            : "rcx", "r11"
    );
    __builtin_unreachable();
}

noreturn void sys_exec(const char *program, const char **argv, const char **envp) {
    __asm__ __volatile__ __inline__ (
        "syscall"
            :
            :
                "a" (0x3b),
                "D" (program),
                "S" (argv),
                "d" (envp)
            : "rcx", "r11"
    );
    __builtin_unreachable();
}

int main(__unused int argc, __unused char **argv) {
    printf("Calling bash...\n");
    static const char *ARGV[5] = {
        "/usr/bin/env",
        "bash",
        "-c",
        "echo \"PID: $$\"",
        0,
    };
    sys_exec(ARGV[0], ARGV, 0);
    printf("This is a test2...\n");
#define fail(code) fail_code = (int)(code); goto failure
    int fail_code = 1;
    void *fail_return = &&fail_return;
    goto start;
failure:
    printf("\x1b[31mFailure.\x1b[0m\n");
    goto *fail_return;
start:
    VecStrat strat = dmf_vec_type_strat(char*);
    strat.disposer = (VecElemDisposer){
        .data = NULL,
        .dispose = &test_disposer,
    };
    Vec vec;
    if (!vec_init(&strat, 0, &vec)) {
        printf("vec_init failure.");
        fail(EXIT_FAILURE);
    }
    vec_push(&vec, &(char*){"Hello, world!"});
    vec_push(&vec, &(char*){"This is a test."});
    vec_push(&vec, &(char*){"The quick brown fox jumps over the lazy dog."});
    vec_push(&vec, &(char*){"foo bar baz"});
    vec_push(&vec, &(char*){"This is the end."});
    vec_swap_remove(&vec, 1, NULL);
    printf("Freeing...\n");
    vec_free(&vec);
    printf("Success\n");
    return 0;
fail_return:
    return fail_code;
#undef fail
}
