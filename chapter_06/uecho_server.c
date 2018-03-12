#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 5

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
    socklen_t clnt_addr_sz;
    char message[BUF_SIZE];
    if(argc != 2)
    {
        printf("Asage : %s <port>\n", argv[0]);
        exit(1);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1) error_handing("UDP socket() error");
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) error_handing("bind() error");
    clnt_addr_sz = sizeof(clnt_addr);
    while(1)
    {
        int str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_addr, clnt_addr_sz);
        message[str_len] = 0;
        printf("Message from UDP client: %s\n", message);
    }
    close(serv_sock);
    return 0;
}

