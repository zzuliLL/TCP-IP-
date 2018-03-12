#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<pthread.h>
#include<semaphore.h>


#define BUF_SIZE 100
#define NAME_SIZE 256

void *recv_msg(void *arg);
void *send_msg(void *arg);
void error_handling(char *message);


char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];


int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;

    if(argc != 4)
    {
        printf("Usage : %s <name> <address> <port>\n", argv[0]);
        exit(1);
    }
    sprintf(name, "[%s]", argv[1]);

    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[2]);
    serv_addr.sin_port = htons(atoi(argv[3]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) error_handling((char*)"connect() error");
    pthread_t send_thread, recv_thread;
    pthread_create(&send_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&recv_thread, NULL, recv_msg, (void*)&sock);
    pthread_join(send_thread, NULL);
    pthread_join(recv_thread, NULL);
    close(sock);
    return 0;
}

void *recv_msg(void *arg)
{
    int sock = *(int*)arg;
    char name_msg[BUF_SIZE+NAME_SIZE];
    while(1)
    {
        int len = read(sock, name_msg, NAME_SIZE+BUF_SIZE);
        if(len == -1) return (void*)-1;
        name_msg[len] = 0;
        fputs(name_msg, stdout);
    }
    return NULL;
}



void *send_msg(void *arg)
{
    int sock = *(int*)arg;
    char name_mag[NAME_SIZE+BUF_SIZE];
    while(1)
    {
        fputs("input msg: ", stdin);
        fgets(msg, BUF_SIZE, stdin);
        if(!strcmp(msg, "q\n")) break;
        sprintf(name_mag, "%s: %s", name, msg);
        write(sock, name_mag, strlen(name_mag));
    }
    return NULL;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}







