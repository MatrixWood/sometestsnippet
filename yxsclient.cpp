#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>

#define BUF_SIZE 200

#define SERVER_ADDR "47.97.181.98"
#define SERVER_PORT 20000

int main()
{
    int clientfd;
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0) == -1) {
        perror("create client socket error.");
        exit(-1);
    }

    int socket_flag = fcntl(clientfd, F_GETFL, 0);// | O_NONBLOCK;
    if (fcntl(clientfd, F_SETFL, socket_flag) == -1) {
        close(clientfd);
        std::cout << "set socket to nonblock error." << std::endl;
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    addr.sin_port = htons(SERVER_PORT);

    while (true) {
        int ret = connect(clientfd, (struct sockaddr*)&addr, sizeof(addr));
        if (ret == 0) {
            std::cout << "connect to server success." << std::endl;
            break;
        }
        if (ret == -1) {
            if (errno == EINTR) {
                std::cout << "connect interruptted." << std::endl;
                continue;
            } else if (errno == EINPROGRESS) {
                break;
            } else {
                close(clientfd);
                return -1;
            }
        }
    }

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

        int err;
        socklen_t len = static_cast<socklen_t>(sizeof(err));
        if (::getsockopt(clientfd, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
            close(clientfd);
            return -1;
        }

        if (err == 0) {
            int is_set = FD_ISSET(clientfd, &readset);

            if (is_set) {
                char buf[BUF_SIZE];
                memset(buf, 0, sizeof(buf));
                int ret = recv(clientfd, buf, sizeof(buf), 0);

                if (ret == -1) {
                    std::cout << "recv data error." << std::endl;
                    return EWOULDBLOCK;
                } else {
                    std::cout << "recv data: " << buf << std::endl;
                    is_recv = true;
                }
            }
        } else {
            std::cout << "connect to server error." << std::endl;
        }
    }
    
    close(clientfd);

    return 0;
}