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
    int fds[2];
    char mes[10];
    pipe(fds);
    pid_t p_id = fork();
    if(p_id) write(fds[1], "thank you", 9);
    else
    {
        int len = read(fds[0], mes, 10);
        mes[len] = 0;
        puts(mes);
        return 0;
    }
    return 0;
}

