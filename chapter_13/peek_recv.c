#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/select.h>
#include<signal.h>
#include<fcntl.h>


#define BUF_SIZE 30

int acpt_sock, recv_sock;

void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}


int main(int argc, char *argv[])
{

    struct sockaddr_in recv_addr, serv_addr;
    char buf[BUF_SIZE];
    if(argc != 2)
    {
        printf("Usage : %s <address> <port>\n", argv[0]);
        exit(1);
    }

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1) error_handing((char*)"bind() error");
    if(listen(acpt_sock, 5) == -1) error_handing((char*)"listen() error");

    socklen_t adr_sz = sizeof(serv_addr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_addr, &adr_sz);

    int len;
    while(1)
    {
        len = recv(recv_sock, buf, sizeof(buf), MSG_PEEK|MSG_DONTWAIT);
        if(len > 0) break;
    }
    buf[len] = 0;
    printf("%d %s\n", len, buf);
    len = recv(recv_sock, buf, sizeof(buf), 0);
    buf[len] = 0;
    printf("read_again: %s\n", buf);
    close(acpt_sock);
    close(recv_sock);
    return 0;

}



















