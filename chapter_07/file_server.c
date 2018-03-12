#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30

void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    char message[BUF_SIZE];
    socklen_t clnt_adr_sz;
    FILE *fp;
    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    fp = fopen("/home/ll/code/server/TCP/file_server.c", "rb");
    if(fp == NULL) error_handing("fopen() error");
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) error_handing("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) error_handing("bind() error");

    if(listen(serv_sock, 5) == -1) error_handing("listen() error");

    clnt_adr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
    if(clnt_sock == -1) error_handing("accept() error");

    while(1)
    {
        int read_cnt = fread((void*)message,1, BUF_SIZE, fp);
        if(read_cnt < BUF_SIZE)
        {
            write(clnt_sock, message, read_cnt);
            break;
        }
        write(clnt_sock, message, BUF_SIZE);
    }
    shutdown(clnt_sock, SHUT_WR);
    int len = read(clnt_sock, message, BUF_SIZE);
    message[len] = 0;
    printf("Message from client: %s\n", message);
    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;

}

