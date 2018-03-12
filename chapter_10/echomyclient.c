#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<arpa/inet.h>

#define BUF_SIZE 30

void error_handing(char *messge)
{
    fputs(messge, stderr);
    fputc('\n', stderr);
    exit(1);
}

void write_file(int sock)
{
    char buf[BUF_SIZE];
    while(1)
    {
        fputs("input message: ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n")) break;
        write(sock, buf, sizeof(buf));
    }
}

void read_file(int sock)
{
    char buf[BUF_SIZE];
    while(1)
    {
        int len = read(sock, buf, BUF_SIZE);
        buf[len-1] = 0;
        printf("message from server: %s\n", buf);
    }
}


int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    if(argc != 3)
    {
        printf("Usage : %s <address> <port>\n", argv[0]);
        exit(1);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) error_handing((char*)"connect() error");
    pid_t p_id = fork();
    if(p_id) write_file(sock);
    else
    {
        read_file(sock);
        return 0;
    }
    close(sock);
    return 0;
}













