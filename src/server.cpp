#include "tcpserver.h"

int main()
{
    network::http::TcpServer server = network::http::TcpServer("0.0.0.0", 8080);
    server.startListen();

    return 0;
}