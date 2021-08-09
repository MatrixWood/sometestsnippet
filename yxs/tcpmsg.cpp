#include "tcpmsg.h"


TcpMsg::TcpMsg()
{
    //m_port = 0;
    m_tcpsockfd = INVALID_SOCKET;
    offset = 0;
    memset(buffer, 0, sizeof(buffer));

}

TcpMsg::~TcpMsg()
{
    m_tcpsockfd = INVALID_SOCKET;
}

SOCKET TcpMsg::Initsocket()
{
#ifdef _WIN32
    //启动Windows socket 2.x环境
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    WSAStartup(ver, &dat);
#endif
    //初始化Socket
    m_tcpsockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == m_tcpsockfd)
    {
        LOG(" tcp socket error...");
    }
    else
    {
        LOG("TCP socket=<%d> success...", (int)m_tcpsockfd);
    }

    return m_tcpsockfd;
}

//连接服务器
int TcpMsg::Connect()
{
    if (INVALID_SOCKET == m_tcpsockfd)
    {
        Initsocket();
    }
    // 2 连接服务器 connect
#ifdef _WIN32
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(SERVER_PORT);
    _sin.sin_addr.S_un.S_addr = inet_addr("47.97.181.98");
    int ret = ::connect(m_tcpsockfd, (sockaddr*)&_sin, sizeof(sockaddr_in));
    int ret2 = WSAGetLastError();
    LOG("conn error %d", ret2);
#else
    struct sockaddr_in _sin; 
    _sin.sin_family = AF_INET;  
    _sin.sin_addr.s_addr = inet_addr("47.97.181.98");
	_sin.sin_port = htons(SERVER_PORT);
    int ret = connect(m_tcpsockfd, (sockaddr*)&_sin, sizeof(_sin));
#endif

    if (0 > ret)
    {
        LOG("<socket=%d>错误，连接服务器<%s:%d>失败...", m_tcpsockfd, "47.97.181.98", SERVER_PORT);
    }
    else {
        LOG("<socket=%d>连接服务器，<%s:%d>成功...", m_tcpsockfd, "47.97.181.98", SERVER_PORT);
    }
    return ret;
}





int TcpMsg::Send()
{
    int ret = ERROR_STATS;
    unsigned char tmpBuff[MSG_LEN] = { 0 };
    memset(tmpBuff, 0, sizeof(tmpBuff));

    ret = send(m_tcpsockfd, (const char*)tmpBuff, 65535, 0);
    //Hexdump(tmpBuff, sendPackageSize);
    if (ret < 0)
    {
        LOG("tcp send <socket=%d> error......", m_tcpsockfd);
    }
    else
    {
        LOG("tcp send socket=<%d> success......", m_tcpsockfd);
    }
    return ret;
}


void TcpMsg::Hexdump(unsigned char* buffer, int iLen)
{
    for (int i = 0; i < iLen; i++)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");
}

int	TcpMsg::Recv()
{
    int iret = recv(m_tcpsockfd, (char*)buffer + offset, (sizeof(buffer) - offset), 0);
    if (iret < 0)
    {
        return ERR_RECV;
    }
    else if (0 == iret)
    {
        return ERROR_STATS;
    }
    else
    {
    
    }
    offset = offset + iret;
    return 1;
}

void TcpMsg::Close()
{
    if (m_tcpsockfd != INVALID_SOCKET)
    {
#ifdef _WIN32
        //关闭套节字closesocket
        closesocket(m_tcpsockfd);
        //------------
        //清除Windows socket环境
        WSACleanup();
#else
        //关闭套节字closesocket
        close(m_tcpsockfd);
#endif
        m_tcpsockfd = INVALID_SOCKET;
    }
}