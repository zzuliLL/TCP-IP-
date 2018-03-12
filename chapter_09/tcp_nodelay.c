#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/tcp.h>

void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

#define TRUE 1

int main(int argc, char *argv[])
{
    int option = TRUE;
    int sock;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&option, sizeof(option));

    return 0;
}

