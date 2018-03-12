#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netdb.h>

void error_handing(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

//61.135.169.121  www.baidu.com
int main(int argc, char *argv[])
{
    int i;
    struct hostent *host;
    if(argc != 2)
    {
        printf("Usage : %s <addr>\n", argv[0]);
        exit(1);
    }
    host = gethostbyname(argv[1]);

//    struct sockaddr_in addr;
//    addr.sin_family = AF_INET;
//    addr.sin_addr.s_addr = inet_addr(argv[1]);
//    host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);

    if(!host) error_handing("gethost...() error");

    printf("Offices name: %s\n", host->h_name);
    for(i = 0; host->h_aliases[i]; i++)
        printf("Aliases name : %s\n", host->h_aliases[i]);
    printf("Address type: %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
    for(i = 0; host->h_addr_list[i]; i++)
        printf("IP addr %d: %s\n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    return 0;

}






