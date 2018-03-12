#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<arpa/inet.h>

#define BUF_SIZE 30

int main()
{
    int fds1[2], fds2[2];
    char mes[109];
    pipe(fds1); pipe(fds2);
    pid_t p_id = fork();
    if(p_id)
    {
        write(fds1[1], "hello child!", 12);
        sleep(2);
        int len = read(fds2[0], mes, 109);
        mes[len] = 0;
        printf("message from child: %s\n", mes);
    }
    else
    {
        int len = read(fds1[0], mes, 109);
        mes[len] = 0;
        printf("message from farther: %s\n", mes);
        write(fds2[1], "hello farther!", 14);
        return 0;
    }
    return 0;
}

