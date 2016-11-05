#ifndef __DLSYM_WRAPPER_H__
#define __DLSYM_WRAPPER_H__

// convert golang string to c-style string
// to avoid C.CString heap allocation, by using c99 stack allocation
#define to_cstr(gstr_name, cstr_name) \
    char cstr_name[gstr_name.Len+1]; \
    cstr_name[gstr_name.Len] = 0; \
    memcpy(cstr_name, gstr_name.Data, gstr_name.Len)

#define to_gstr(cstr_name, gstr_name) \
    gstr gstr_name; \
    gstr_name.Data = cstr_name; \
    gstr_name.Len = strlen(cstr_name)

typedef struct _gstr {
    const char *Data;
    size_t Len;
} gstr;

void libc_hook_start();
struct hostent *hostent_new(in_addr_t *ip_list, size_t ip_list_len);

#endif