#ifndef __TCP_MSG_H__
#define __TCP_MSG_H__

#include "definetype.h"

using namespace std;
class TcpMsg
{
public:
    TcpMsg();
    virtual ~TcpMsg();

public:
    SOCKET   m_tcpsockfd;
    uint16   m_port;
    //sockaddr_in m_sin;

public:

    uint8 buffer[MAXCONTENTLEN];
    int offset;

    SOCKET  Initsocket();
    int     Bind();
    int     Connect();
    int		Recv();
    int		Send();
    void	Close();
    void     Hexdump(unsigned char* buffer, int iLen);
};

#endif