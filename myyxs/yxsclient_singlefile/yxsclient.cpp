#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <string>
#include <string.h> 
#include <map>
#include <vector>
#include <stdlib.h>
#include <errno.h>
#include <list>
#include <fstream>

#define BUF_SIZE 200
#define MAX_BUF_LENGTH 4096

#define SERVER_ADDR "47.97.181.98"
#define SERVER_PORT 20000

typedef int SOCKET;

SOCKET initSocket() {
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1) {
        close(clientfd);
        perror("create client socket error.");
        exit(-1);
    }

    return clientfd;
}

int setNonblock(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) == -1) {
        close(fd);
        perror("set socket to nonblock error.");
        exit(-1);
    }
    return 0;
}

int connectServer(int fd) {
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    addr.sin_port = htons(SERVER_PORT);

    while (true) {
        int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
        if (ret == 0) {
            std::cout << "connect to server success." << std::endl;
            break;
        }
        if (ret == -1) {
            if (errno == EINTR) {
                perror("connect interruptted.");
                continue;
            } else if (errno == EINPROGRESS) {
                break;
            } else {
                close(fd);
                return -1;
            }
        }
    }
    return 0;
}

int recvData(int fd) {
    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    int ret = ::recv(fd, buf, sizeof(buf), 0);
    
    if (ret > 0) {
        std::cout << "recv data: " << buf << std::endl;
        return ret;
    }
    if (ret == 0) {
        std::cout << "peer shutdown." << std::endl;
        return 0;
    }
    if (ret == -1) {
        if (!(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)) {
            perror("recv error.");
            return -1;
        }
    }

    return ret;
}

int sendData(int fd) {
    // TODO
    return 0;
}

int isConnectSucc(int clientfd) {
    fd_set writeset;
    FD_ZERO(&writeset);
    FD_SET(clientfd, &writeset);
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    if (select(clientfd + 1, NULL, &writeset, NULL, &tv) != 1) {
        std::cout << "[select] connect to server error." << std::endl;
        close(clientfd);

        return -1;
    }

    int err;
    socklen_t len = static_cast<socklen_t>(sizeof(err));
    if (::getsockopt(clientfd, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
        close(clientfd);
        return -1;
    }

    if (err == 0) {
        std::cout << "After check, connect to server successfully." << std::endl;
        return 0;
    } else {
        std::cout << "After check, connect to server error." << std::endl;
        close(clientfd);
        return -1;
    }
}

int getMsg(int clientfd) {
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    bool is_recv = false;

    while (!is_recv) {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(clientfd, &readset);
        int select_ret = select(clientfd + 1, &readset, nullptr, nullptr, &tv);
        if (select_ret == 0) {
            std::cout << "[select] timeout." << std::endl;
            continue;
        } else if (select_ret < 0) {
            std::cout << "[select] connect to server error." << std::endl;
            close(clientfd);
            return -1;
        }
        if (err == 0) {
            int is_set = FD_ISSET(clientfd, &readset);

            if (is_set) {
                int ret = recvData(clientfd);
                if (ret > 0)
                    is_recv = true;
            }
        }
    }

    return 0;
}

int main()
{
    int clientfd;
    
    clientfd = initSocket();

    setNonblock(clientfd);

    if (connectServer(clientfd) != 0) {
        perror("connect to server failed.");
        return -1;
    }

    if (isConnectSucc(clientfd) < 0) {
        perror("check connection: connect failed.");
        return -1;
    }

    getMsg(clientfd);
    
    close(clientfd);

    return 0;
}