#include "tcpserver.h"


//#ifdef defined(PLATFORM_LINUX)
#include "linux/linux.h"
//#endif


TCPServer* createServer()
{
    //#ifdef defined(PLATFORM_LINUX)
        return new LinuxServer();
    //#endif
}