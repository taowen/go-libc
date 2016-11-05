#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void example_gethostbyname() {
    struct hostent *host = gethostbyname("example.com");
    struct in_addr **addr_list = (struct in_addr **)host->h_addr_list;
    for(int i = 0; addr_list[i] != NULL; i++) {
        printf("%s\n", inet_ntoa(*addr_list[i]));
    }
    fflush(stdout);
}