#ifndef __CLIENT_MAN_H__
#define __CLIENT_MAN_H__

#include "tcpmsg.h"

class ClientManger
{
public:
    ClientManger();
    virtual ~ClientManger();

public:
    /**********TCP message*******/
    pthread_t       m_tcpth;
    TcpMsg  		ptcp;
    void TcpMessageLoop();
    int handlerMsg_TCP();
    int fillFdset_TCP(SOCKET fd, fd_set& rset);
    int Start_TCP();
};

#endif