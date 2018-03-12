#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int read_cnt;
    FILE *fp;
    if(argc != 3)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    fp = fopen("receive.dat", "wb");
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while((read_cnt = read(sock, message, BUF_SIZE)) != 0)
        fwrite((void*)message, 1, read_cnt, fp);
    puts("Recived file data");
    write(sock, "Thank you", 10);
    fclose(fp);
    close(sock);
    return 0;

}

