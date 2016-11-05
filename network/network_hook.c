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
#define HOSTENT_POOL_SIZE 4
struct hostent *hostent_allocate() {
    static __thread int index = 0;
    static __thread struct hostent pool[HOSTENT_POOL_SIZE];
    return &pool[index++ % HOSTENT_POOL_SIZE];
}

// allocate memory in thread local statically
#define IN_ADDR_PTR_ARRAY_POOL_SIZE 8
#define IN_ADDR_PTR_ARRAY_SIZE 8
struct in_addr ** in_addr_ptr_array_allocate() {
    static __thread int index = 0;
    static __thread struct in_addr *pool[IN_ADDR_PTR_ARRAY_POOL_SIZE][IN_ADDR_PTR_ARRAY_SIZE];
    return pool[index++ % IN_ADDR_PTR_ARRAY_POOL_SIZE];
}

// allocate memory in thread local statically
#define ALIAS_PTR_ARRAY_POOL_SIZE 8
#define ALIAS_PTR_ARRAY_SIZE 8
struct char ** alias_ptr_array_allocate() {
    static __thread int index = 0;
    static __thread struct char *pool[ALIAS_PTR_ARRAY_POOL_SIZE][ALIAS_PTR_ARRAY_SIZE];
    return pool[index++ % ALIAS_PTR_ARRAY_POOL_SIZE];
}

// allocate memory in thread local statically
#define IN_ADDR_POOL_SIZE 64
struct in_addr *in_addr_allocate() {
    static __thread int index = 0;
    static __thread struct in_addr pool[IN_ADDR_POOL_SIZE];
    return &pool[index++ % IN_ADDR_POOL_SIZE];
}

// convert []in_addr_t to hostent*
struct hostent *hostent_new(char *name, in_addr_t *ip_list, size_t ip_list_len) {
    struct hostent *host = hostent_allocate();
    struct in_addr **in_addr_ptr_array = in_addr_ptr_array_allocate();
    if (ip_list_len > IN_ADDR_PTR_ARRAY_SIZE-1) {
        ip_list_len = IN_ADDR_PTR_ARRAY_SIZE-1;
    }
    for (int i = 0; i < ip_list_len; i++) {
        in_addr_ptr_array[i] = in_addr_allocate();
        in_addr_ptr_array[i]->s_addr = ip_list[i];
    }
    in_addr_ptr_array[ip_list_len] = NULL; // null terminated array
    char **alias_ptr_array = alias_ptr_array_allocate();
    alias_ptr_array[0] = NULL;
    host->h_name = name;
    host->h_aliases = alias_ptr_array;
    host->h_addrtype = AF_INET;
    host->h_length = 4;
    host->h_addr_list = (char**)in_addr_ptr_array;
    return host;
}