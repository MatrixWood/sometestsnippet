#include "clientmgr.h"

ClientManger::ClientManger()
{
    
}

ClientManger::~ClientManger()
{
    int iRet;
    iRet = pthread_join(m_tcpth, NULL);
    {
        LOG("thread recycle fail!");
    }
}



int ClientManger::fillFdset_TCP(SOCKET fd, fd_set& rset)
{
	SOCKET maxfd = 0;
	FD_ZERO(&rset);	//创建后  清空一下
	if (fd > 0)
	{
		FD_SET(fd, &rset);//将vCon[n].m_sockfd添加到rset
		maxfd = maxfd < fd ? fd : maxfd;
	}
	return maxfd;
}


/*TCP 接收数据*/
int ClientManger::handlerMsg_TCP()
{
	int iret = 0;

	fd_set rset;
	int maxfdudp = fillFdset_TCP(ptcp.m_tcpsockfd, rset);
	struct timeval timeout_;
	timeout_.tv_sec = 0;
	timeout_.tv_usec = 0;
	int retval = select(maxfdudp + 1, &rset, NULL, NULL, NULL);
	if (retval < 0)
	{
		LOG("Bcast message select error......\n");
		return ERR_SELECT;
	}
	else if (retval == 0)
	{
		LOG("Bcast message select timeout......\n");
		return ERR_TIMEOUT;
	}
	else
	{
		if (FD_ISSET(ptcp.m_tcpsockfd, &rset))
		{
			while (1)
			{
				int ret = ptcp.Recv();
				if (ret == ERR_RECV)
				{
					return ERR_RECV;
				}
				else if (ERROR_STATS == ret)
				{
					return ERROR_STATS;
				}
				else {
					while (1)
					{
                        LOG("buff %s...",ptcp.buffer);

                        sleep(2000);
					}
					break;
				}
			}
		}
	}
	return SUCCESS;
}


/*tcp  loop*/
void ClientManger::TcpMessageLoop()
{
	int iret = 0;
	int count = 0;

	clentmgr_st clientst = CLI_INIT_S;
	while (CLI_EXIT_S != clientst)
	{
		switch (clientst)
		{
			case CLI_INIT_S:
				{
					if (INVALID_SOCKET != ptcp.m_tcpsockfd)
					{
						LOG("<socket=%d>关闭旧连接...", ptcp.m_tcpsockfd);
						ptcp.Close();
					}

					SOCKET tcpfd = ptcp.Initsocket();
					if (0 == tcpfd)
					{
						clientst = CLI_INIT_S;
					}
					else
					{
						clientst = CLI_CONN_S;
					}
				}
				break;

			case CLI_CONN_S:
				{
                    iret = ptcp.Connect();
                    if (iret < 0)
                    {
                        ptcp.Close();
                        clientst = CLI_INIT_S;
                    }
                    else {
                        clientst = CLI_BCAST_S;
                    }
				}
				break;

			case CLI_BCAST_S:
				{/*整理*/
					iret = handlerMsg_TCP();
					if (iret == ERR_RECV)
					{
						LOG("重新 conn");
						clientst = CLI_CONN_S;
					}
					else if(iret < 0) 
					{
						clientst = CLI_BCAST_S;
						LOG("TCP handlerMessage ret=%d, errno=%s", iret, strerror(errno));
					}
					else
					{
						clientst = CLI_DATA_RW_S;
					}
				}
				break;

			case CLI_DATA_RW_S:
				{
					clientst = CLI_BCAST_S;
                    sleep(2000);
				}
				break;

			default:
				break;
		}
	}
}

static void* TcpClient(void *args)
{
	ClientManger* pSt = (ClientManger*)args;
	pSt->TcpMessageLoop();
	return NULL;
}

int ClientManger::Start_TCP()
{
	return pthread_create(&m_tcpth, NULL, TcpClient, (void*)this);
}
