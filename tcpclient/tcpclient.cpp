#include "tcpclient.h"

//#ifdef defined(PLATFORM_LINUX)
#include "linux/linux.h"
//#endif

TCPClient *createclient()
{
  //#ifdef defined(PLATFORM_LINUX)
    return new LinuxClient();
  //#endif
}
