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

int main(int argc, char *argv[])
{
    int sock, state;
    int snd_buf, rcv_buf;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if(state == -1) error_handing("getsockopt() error");    //16384
    printf("Input buf size: %d\n", snd_buf);

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if(state == -1) error_handing("getsockopt() error");
    printf("Output buf size: %d\n", rcv_buf);              //87380
    return 0;
}

