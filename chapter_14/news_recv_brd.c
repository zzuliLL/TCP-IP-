#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define TTL 64
#define BUF_SIZE 30

void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}


int main(int argc, char *argv[])
{
    int recv_sock;
    struct sockaddr_in adr;
    char buf[BUF_SIZE];
    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[1]));

    if(bind(recv_sock, (struct sockaddr*)&adr, sizeof(adr)) == -1) error_handing((char*)"bind() error");

    while(1)
    {
        int len = recvfrom(recv_sock, buf, BUF_SIZE, 0, NULL, 0);
        if(len < 0) break;
        buf[len] = 0;
        fputs(buf, stdout);
    }
    close(recv_sock);
    return 0;
}


















