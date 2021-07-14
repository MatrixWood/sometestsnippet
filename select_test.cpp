#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#define MYPORT 8100
#define MAXCLINE 5
#define BUF_SIZE 200

int fd[MAXCLINE];

int conn_amount;

void showclient() {
    std::cout << "client amount: " << conn_amount << "\n";
    for (int i = 0; i < MAXCLINE; ++i) {
        std::cout << i << ":" << fd[i] << " ";
    }
    std::cout << "\n\n";
}

int main() {
    int sock_fd, new_fd;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t sin_size;
    int yes = 1;
    char buf[BUF_SIZE];
    int ret;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt error \n");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(MYPORT);
    memset(serv_addr.sin_zero, '\0', sizeof(serv_addr.sin_zero));

    if (bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind error!\n");
        exit(1);
    }

    if (listen(sock_fd, MAXCLINE) == -1) {
        perror("listen error!\n");
        exit(1);
    }

    std::cout << "listen port: " << MYPORT << "\n";
    fd_set fdsr;
    int maxsock;
    
}