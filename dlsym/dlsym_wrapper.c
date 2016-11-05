#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dlsym_wrapper.h"

// convert golang string to c-style string
// to avoid C.CString heap allocation, by using c99 stack allocation
#define to_cstr(gstr_name, cstr_name) \
    char cstr_name[gstr_name.Len+1]; \
    cstr_name[gstr_name.Len] = 0; \
    memcpy(cstr_name, gstr_name.Data, gstr_name.Len)

void intercept(gstr symbol_) {
    to_cstr(symbol_, symbol);
    printf("%s\n", symbol);
    fflush(stdout);
}