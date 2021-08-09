#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#define SERVER_IP "47.97.181.98"
#define SERVER_PORT 20000

using namespace std;

#define MAXBUF 1024
int main(int argc, char **argv)
{
    int sockfd, len;
    struct sockaddr_in dest;
    char buffer[MAXBUF + 1];
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd = -1;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("Socket");
        exit(-1);
    }

    int flags;
    //no_block 
    flags = fcntl(sockfd, F_GETFL, 0);
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0 ) 
    {
        perror("no_block");
    }

    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(SERVER_PORT);
    dest.sin_addr.s_addr = inet_addr(SERVER_IP);
    int ret = connect(sockfd, (struct sockaddr *) &dest, sizeof(dest));
    if (ret = -1)
    {
        if(errno != EINPROGRESS)
        {
            perror("Connect ");
            exit(-1);
        }
    }
    else if (ret < 0)    
    {
        perror("Connect ");
        exit(-1);
    }
    cout << "con suc!" <<endl;
    while (1) 
    {
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(sockfd, &rfds);
        maxfd = sockfd;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1) 
        {
            cout << "select err:" << strerror(errno) << endl;
            break;
        } 
        else if (retval == 0)
        {
            continue;
        }
        else
        {
            if (FD_ISSET(sockfd, &rfds)) 
            {
                bzero(buffer, MAXBUF + 1);
                len = recv(sockfd, buffer, MAXBUF, 0);
                if (len > 0)
                {
                    cout << "msg: " << buffer << endl;
                    cout << "msgLen: " << len << endl;
                }
                else 
                {
                    if (len < 0)
                    {
                        //
                        cout << "recv failed" << endl;
                    }
                    else
                    {
                        cout << "recv failed other!" << endl;
                        break;
                    }
                }
            }
        }
    }
    close(sockfd);
    return 0;
} 

