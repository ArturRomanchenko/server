#include "tcpserver.h"

int main()
{
    http::TcpServer server = http::TcpServer("0.0.0.0", 8080);
    server.startListen();

    return 0;
}