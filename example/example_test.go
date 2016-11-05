package example

import (
	"testing"
	"github.com/taowen/go-libc/network"
)

func Test_gethostbyname(t *testing.T) {
	dlsym.HookStart()
	for i := 0; i < 10; i++ {
		example_gethostbyname()
	}
}
