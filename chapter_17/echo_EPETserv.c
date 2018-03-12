#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/epoll.h>
#include<fcntl.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50

void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void setnonblockmode(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t adr_sz = sizeof(clnt_addr);
    char buf[BUF_SIZE];
    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    int op = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&op, sizeof(op));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) error_handing((char*)"bind() error");
    if(listen(serv_sock, 5) == -1) error_handing((char*)"listen() error");

    setnonblockmode(serv_sock);
    struct epoll_event *ep_events, event;
    ep_events = (struct epoll_event*)malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    int epfd = epoll_create(EPOLL_SIZE);
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    int i;
    while(1)
    {
        int ep_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(ep_cnt == -1)
        {
            printf("epoll_wait() error\n");
            break;
        }
        puts("return epoll_wait");
        for(i = 0; i < ep_cnt; i++)
        {
            int sock = ep_events[i].data.fd;
            if(sock == serv_sock)
            {
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
                setnonblockmode(clnt_sock);
                event.events = EPOLLIN|EPOLLET;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client %d\n", clnt_sock);
            }
            else
            {
                while(1)
                {
                    int len = read(sock, buf, BUF_SIZE);
                    if(len < 0)
                    {
                        if(errno == EAGAIN) break;
                    }
                    if(len) write(sock, buf, len);
                    else
                    {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, sock, NULL);
                        close(sock);
                        printf("closed client %d\n", sock);
                        break;
                    }
                }
            }
        }
    }
    close(epfd);
    close(serv_sock);
    return 0;

}
















