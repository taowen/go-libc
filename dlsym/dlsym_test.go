package dlsym

import "testing"

func Test_intercept(t *testing.T) {
	Intercept("gethostbyname")
}
