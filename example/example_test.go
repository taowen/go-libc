package example

import (
	"testing"
	"github.com/taowen/go-libc/network"
)

func Test_gethostbyname(t *testing.T) {
	dlsym.HookStart()
	example_gethostbyname()
}
