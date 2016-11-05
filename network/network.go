package dlsym

// #cgo LDFLAGS: -ldl
// #include <stddef.h>
// #include <netinet/in.h>
// #include "network_hook.h"
import "C"
import (
	"unsafe"
	"net"
	"encoding/binary"
)

func HookStart() {
	C.libc_hook_start()
}

//export go_gethostbyname
func go_gethostbyname(name_ C.struct__gstr) *C.struct_hostent {
	name := cstr(name_)
	ipList, _ := net.LookupIP(name)
	ipAsIntList := make([]C.in_addr_t, 0, 4)
	for _, ip := range ipList {
		asInt := ip2int(ip)
		if asInt == 0 {
			continue // ipv6 is ignored
		}
		ipAsIntList = append(ipAsIntList, C.in_addr_t(asInt))
	}
	if len(ipAsIntList) == 0 {
		return C.hostent_new(nil, C.size_t(0))
	} else {
		ptr := unsafe.Pointer(&ipAsIntList[0])
		return C.hostent_new((*C.in_addr_t)(ptr), C.size_t(len(ipAsIntList)))
	}
}

func ip2int(ip net.IP) uint32 {
	if len(ip) == 16 {
		return binary.BigEndian.Uint32(ip[12:16])
	}
	return 0
}

func gstr(val string) C.struct__gstr {
	ptr := unsafe.Pointer(&([]byte(val)[0]))
	return C.struct__gstr{
		Data: (*C.char)(ptr),
		Len: C.size_t(len(val)),
	}
}

func cstr(val C.struct__gstr) string {
	bytes := (*[1 << 30]byte)(unsafe.Pointer(val.Data))[:val.Len:val.Len]
	return string(bytes)
}
