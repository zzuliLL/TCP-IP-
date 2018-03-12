#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<fcntl.h>
#define BUF_SIZE 30


void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_child(int sig)
{
    int state;
    pid_t p_id = waitpid(-1, &state, WNOHANG);
    printf("removed child proc: %d\n", p_id);
}


int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    struct sigaction act;
    char buf[BUF_SIZE];
    socklen_t adr_sz;
    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    act.sa_handler = read_child;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, sizeof(option));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handing((char*)"bind() error");
    if(listen(serv_sock, 5) == -1) error_handing((char*)"listen() error");


    int fds[2];
    pipe(fds);
    pid_t p_id = fork();
    if(p_id == 0)
    {
        FILE *fp = fopen("fromeclientdata.dat", "wt");

        int i;
        char mes[BUF_SIZE];
        for(i = 0; i < 10; i++)
        {
            int len = read(fds[0], mes, BUF_SIZE);
            fwrite((void*)mes, 1, len, fp);
        }
        printf("close fp\n");
        fclose(fp);
        return 0;
    }


    adr_sz = sizeof(clnt_addr);
    while(1
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
        if(clnt_sock == -1) continue;
        else puts("new client connected...");
        p_id = fork();
        if(p_id == -1)
        {
            close(clnt_sock);
            continue;
        }
        if(p_id) close(clnt_sock);
        else
        {
            close(serv_sock);
            while(1)
            {
                int len = read(clnt_sock, buf, BUF_SIZE);
                if(len == 0) break;
                write(clnt_sock, buf, len);
                write(fds[1], buf, len);
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
    }
    close(serv_sock);
    return 0;
}





















