#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
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

#define SERVER_ADDR "47.97.181.98"
#define SERVER_PORT 20000

typedef int SOCKET;