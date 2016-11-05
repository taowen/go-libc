
#ifndef __DLSYM_WRAPPER_H__
#define __DLSYM_WRAPPER_H__

typedef struct _gstr {
    char *Data;
    size_t Len;
} gstr;

void intercept(gstr symbol);

#endif