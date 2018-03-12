#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/select.h>

#define BUF_SIZE 30

void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

//error

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    if(argc != 3)
    {
        printf("Usage : %s <address> <port>\n", argv[0]);
        exit(1);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof (serv_addr)) == -1) error_handing((char*)"connect() error");

    write(sock, "123", strlen("123"));
    send(sock, "4", strlen("1"), MSG_OOB);
    write(sock, "567", strlen("567"));
    send(sock, "890", strlen("890"), MSG_OOB);
    close(sock);
    return 0;

}



















