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
    int sock;
    struct sockaddr_in serv_addr, from_addr;
    char message[BUF_SIZE];
    socklen_t adr_sz;
    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    adr_sz = sizeof(from_addr);
    while(1)
    {
        fputs("Inputs message: ", stdout);
        fgets(message, BUF_SIZE, stdin);
//        sendto(sock, message, sizeof(message), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
//        recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_addr, &adr_sz);
        write(sock, message, sizeof(message));
        read(sock, message, BUF_SIZE);
        printf("Message: %s\n", message);
    }

    return 0;
}

