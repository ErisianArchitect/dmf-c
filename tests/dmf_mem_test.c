#include <stdbool.h>
#include <stddef.h>
#include "dmf/test.h"
#include "dmf/mem.h"

int main() {
    void *ptr = dmf_malloc(16);
    if (!ptr) {
        printfail("dmf_malloc: pointer is null.");
        return 1;
    } else {
        printpass("dmf_malloc: pointer is valid.");
    }
    void *new_ptr = dmf_realloc(ptr, 32);
    if (!new_ptr) {
        printfail("dmf_realloc: pointer is null.");
        dmf_free(ptr);
    } else {
        printpass("dmf_realloc: pointer is valid.");
        dmf_free(new_ptr);
    }
    char *zeroed = dmf_calloc(1024, 1);
    if (!zeroed) {
        printfail("dmf_calloc: pointer is null.");
    } else {
        printpass("dmf_calloc: pointer is valid.");
        for (int i = 0; i < 1024; i++) {
            if (zeroed[i] != 0) {
                printfail("dmf_calloc: not zeroed memory.");
                goto dmf_calloc_fail;
            }
        }
        printpass("dmf_calloc: memory was zeroed.");
    dmf_calloc_fail:
        dmf_free(zeroed);
    }

    return 0;
}