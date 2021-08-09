#include "clientmgr.h"

int main()
{
    ClientManger Client;
    Client.Start_TCP();//与 server通讯线程

    return SUCCESS;
}