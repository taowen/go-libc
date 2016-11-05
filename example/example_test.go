package example

import (
	"testing"
	"net"
	"fmt"
)

func Test_gethostbyname(t *testing.T) {
	//dlsym.HookStart()
	example_gethostbyname()
}

func Test_udp(t *testing.T) {
	addr, _ := net.ResolveUDPAddr("udp", ":8888")
	conn, _ := net.ListenUDP("udp", addr)
	file, _ := conn.File()
	conn2, _ := net.FileConn(file)
	conn3 := conn2.(*net.UDPConn)
	fmt.Print(file.Fd())
	fmt.Print(conn3)
}
