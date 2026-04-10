#include <stdint.h>
#include <string.h>
#include "dmf/str.h"
#include "dmf/mem.h"

dmf_str dmf_str_new_inline(const char *s, uint8_t len) {
    dmf_str new = {0};
    memcpy(&new, s, len);
    // This is a trick that makes it so that
    // all inline strings are null-terminated.
    if (len == 0xF) new.inline_str.len = 0;
    else new.inline_str.len = len;
    return new;
}

dmf_str dmf_str_new_static(const char *s) {
    size_t slen = strlen(s);
    return (dmf_str){
        .indirect_str = {
            .s = s,
            .len = (uint32_t)slen,
            .flags = 0,
            .type = I_STATIC,
            ._footer = INDIRECT_TAG,
        }
    };
}

dmf_str dmf_str_new(const char *s) {
    size_t slen = strlen(s);
    if (slen < 16) {
        if (slen == 0) {
            dmf_str new = {0};
            new.indirect_str._footer = INDIRECT_TAG;
            return new;
        } else {
            return dmf_str_new_inline(s, slen);
        }
    }
    char* bytes = dmf_malloc(slen + 1);
    memcpy(bytes, s, slen);
    bytes[slen] = 0;
    return (dmf_str){
        .indirect_str = {
            .s = bytes,
            .len = (uint32_t)slen,
            .flags = 0,
            .type = I_HEAP,
            ._footer = INDIRECT_TAG,
        }
    };
}