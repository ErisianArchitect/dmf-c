#ifndef DMF_TEST_LIB
#define DMF_TEST_LIB
#include <stdio.h> // IWYU pragma: export

#define printpass(message) printf("\033[32m[PASS]\033[0m %s -> \033[36m\"%s\":%d\033[0m\n", message, __FILE__, __LINE__)
#define printfail(message) printf("\033[31m[FAIL]\033[0m %s -> \033[36m\"%s\":%d\033[0m\n", message, __FILE__, __LINE__)

#endif