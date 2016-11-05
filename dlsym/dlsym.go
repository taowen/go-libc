package dlsym

// #cgo CFLAGS: -I.
// #include <stddef.h>
// #include "dlsym_wrapper.h"
import "C"
import "unsafe"

func Intercept(symbol string) {
	C.intercept(gstr(symbol))
}

func gstr(str string) C.struct__gstr {
	ptr := unsafe.Pointer(&([]byte(str)[0]))
	return C.struct__gstr{
		Data: (*C.char)(ptr),
		Len: C.size_t(len(str)),
	}
}
