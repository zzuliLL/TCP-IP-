#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>


#define BUF_SIZE 30

void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}



int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;

    char buf[BUF_SIZE];

    int str_len;

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

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) error_handing((char*)"connect() error");

    while(1)
    {
        fputs("input message: ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) break;
        write(sock, buf, sizeof(buf));
        str_len = read(sock, buf, BUF_SIZE);
        buf[str_len] = '\0';
        printf("Message from server: %s\n", buf);
    }
    close(sock);
    return 0;
}
















