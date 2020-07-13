#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdarg.h>
#include "my_err.h"

#define MAXLINE 4096

/*
 * struct sockaddr_in {
 *     sa_familt_t     sin_family;   //地址族(address family)
 *     uint16_t        sin_port;     //16为TCP/UDP端口号
 *     struct in_addr  sin_addr;     //32位IP地址
 *     char            sin_zero[8];  //不使用
 * }
 */

int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    if (argc != 2)
        err_quit("usage: a.out <IPaddress>");
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13); /*daytime server*/
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        err_sys("connect error");
    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF)
            err_sys("fputs error");
    }
    if (n < 0)
        err_sys("read error");
    
    return 0;
}
