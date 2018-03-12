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



int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t adr_sz;
    fd_set reads, cpy_reads;

    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, sizeof(option));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) error_handing((char*)"bind() error");
    if(listen(serv_sock, 5) == -1) error_handing((char*)"listen() error");

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    int fd_mx = serv_sock, fd_num, i;
    struct timeval timeout;
    char buf[BUF_SIZE];

    while(1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        if((fd_num = select(fd_mx+1, &cpy_reads, 0, 0, &timeout)) == -1) break;
        if(fd_num == 0)
        {
            puts("Time-out");
            continue;
        }

        for(i = 0; i <= fd_mx; i++)
        {
            if(FD_ISSET(i, &cpy_reads))
            {
                if(i == serv_sock)
                {
                    adr_sz = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
                    if(clnt_sock == -1) error_handing((char*)"accept() error");
                    FD_SET(clnt_sock, &reads);
                    fd_mx = fd_mx > clnt_sock ? fd_mx : clnt_sock;
                    printf("connected client: %d\n", clnt_sock);
                }
                else
                {
                    int len = read(i, buf, BUF_SIZE);
                    if(len == 0)
                    {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed connect: %d\n", i);
                    }
                    else write(i, buf, len);
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}



















