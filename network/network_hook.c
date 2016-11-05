#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <math.h>
#include "network_hook.h"
#include "_cgo_export.h"

#define RTLD_NEXT	((void *) -1l)

void libc_hook_start() {
}

#define HOOK_SYS_FUNC(name) if( !c_##name##_func ) { c_##name##_func = (name##_pfn_t)dlsym(RTLD_NEXT,#name); }

typedef struct hostent* (*gethostbyname_pfn_t)(const char *name);
static gethostbyname_pfn_t c_gethostbyname_func;
struct hostent *gethostbyname(const char *name_) {
    HOOK_SYS_FUNC( gethostbyname );
    to_gstr(name_, name);
    return go_gethostbyname(name);
}

// allocate memory in thread local statically
struct hostent *hostent_allocate() {
    static __thread int index = 0;
    static __thread struct hostent pool[16];
    return &pool[index++%16];
}

// allocate memory in thread local statically
struct in_addr ** in_addr_ptr_array_allocate() {
    static __thread int index = 0;
    static __thread struct in_addr *pool[16][16];
    return pool[index++%16];
}

// allocate memory in thread local statically
struct in_addr *in_addr_allocate(in_addr_t ip) {
    static __thread int index = 0;
    static __thread struct in_addr pool[256];
    struct in_addr *obj = &pool[index++%256];
    obj->s_addr = ip;
    return obj;
}

// convert []in_addr_t to hostent*
struct hostent *hostent_new(in_addr_t *ip_list, size_t ip_list_len) {
    struct hostent *host = hostent_allocate();
    struct in_addr **in_addr_ptr_array = in_addr_ptr_array_allocate();
    if (ip_list_len > 15) {
        ip_list_len = 15;
    }
    for (int i = 0; i < ip_list_len; i++) {
        in_addr_ptr_array[i] = in_addr_allocate(ip_list[i]);
    }
    in_addr_ptr_array[ip_list_len] = NULL; // null terminated array
    host->h_addr_list = (char**)in_addr_ptr_array;
    return host;
}