#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>

void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

#define TRUE 1

int main(int argc, char *argv[])
{
    int option, serv_sock;
    socklen_t opt_len;
    option = TRUE;
    opt_len = sizeof(option);
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)option, opt_len);

    return 0;
}

