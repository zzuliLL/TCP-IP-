#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>

void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int tcp_sock, udp_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    printf("SOCK_STREAM: %d\n", SOCK_STREAM);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCK_DGRAM: %d\n", SOCK_DGRAM);

    optlen = sizeof(sock_type);
    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state == -1) error_handing("getsockopt() error");
    printf("Socket type one: %d\n", sock_type);

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state == -1) error_handing("getsockopt() error");
    printf("Socket type two: %d\n", sock_type);
}

