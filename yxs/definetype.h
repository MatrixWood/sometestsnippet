#include <stdio.h>
#include <string>
#include <string.h> 
#include <map>
#include <vector>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <list>
#include <fstream>
#include <fcntl.h>



#ifdef _WIN32
#include <pthreads/pthread.h>
#define WIN32_LEAN_AND_MEAN	
#define  _WINSOCKAPI_
#include<windows.h>
#include<WinSock2.h>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib,"pthreadVC2.lib")
#else
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif







#define LOG(...){\
	fprintf(stderr,"%s:line%d:\t",__FUNCTION__,__LINE__);\
	fprintf(stderr,__VA_ARGS__);\
	fprintf(stderr,"\n");\
}

typedef char sint8;
typedef unsigned char uint8;
typedef short sint16;
typedef unsigned short uint16;
#ifndef uint8
#define uint8 unsigned char		
#endif
#ifndef uint16
#define uint16 unsigned short		
#endif
typedef int sint32;
typedef unsigned int uint32;
typedef long sint64;
typedef unsigned long uint64;
typedef  int  SOCKET; 

#define SWAP_16(x) \
((uint16)( \
    (((uint16)(x) & (uint16)0x00ffU) << 8) | \
    (((uint16)(x) & (uint16)0xff00U) >> 8) ))
#define SWAP_16_ON_LE(x)                    SWAP_16(x)     


//#define LOCAL_ADDRESS      "127.0.0.1"

#define MAXCONTENTLEN 		   65536
#define MSG_LEN                32767
#define INVALID_SOCKET -1

//状态机
typedef enum CLIENTMGR_STATUE_S
{
    CLI_INIT_S = 0,
    CLI_CONN_S,
    CLI_BCAST_S,
    CLI_DATA_RW_S,
    CLI_EXIT_S,
    CLI_PINGGW
}clentmgr_st;


enum PORT
{
    SERVER_PORT = 20000,

};

enum  ERROR_CODE
{
    ERR_NEW = -4,
    ERR_JSON = -3,
    ERR_OFFLINE = -2,
    ERROR_STATS = -1,
    SUCCESS = 0,
    ERR_FAILED,
    CHANGE_STAT,

    ERR_SELECT = 100,
    ERR_SEND,
    ERR_RECV,
    ERR_TIMEOUT,
    ERR_CLOSE
};