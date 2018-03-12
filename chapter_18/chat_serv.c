#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<pthread.h>
#include<semaphore.h>


#define BUF_SIZE 100
#define MAX_CLNT 256

void *handle_clnt(void *arg);
void send_msg(char *msg, int len);
void error_handling(char *message);


int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;


int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t adr_sz = sizeof(clnt_addr);
    pthread_t t_id;
    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    int op = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&op, sizeof(op));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) error_handling((char*)"bind() error");
    if(listen(serv_sock, 5) == -1) error_handling((char*)"listern() error");


    while(1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutex);
        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id);
        printf("Connected client IP: %d\n", clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void *handle_clnt(void *arg)
{
    int sock = *(int*)arg;
    int i, len;
    char msg[BUF_SIZE];
    while((len = read(sock, msg, BUF_SIZE)) != 0)
        send_msg(msg, len);
    pthread_mutex_lock(&mutex);
    for(i = 0; i < clnt_cnt; i++)
    {
        if(sock == clnt_socks[i])
        {
            while(i + 1 < clnt_cnt) clnt_socks[i] = clnt_socks[i+1], i++;
            clnt_cnt--;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    close(sock);
    return NULL;
}



void send_msg(char *msg, int len)
{
    pthread_mutex_lock(&mutex);
    int i;
    for(i = 0; i < clnt_cnt; i++)
        write(clnt_socks[i], msg, len);
    pthread_mutex_unlock(&mutex);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}







