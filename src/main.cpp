#include "server.hpp"

int main(void)
{
    network::http::TcpServer server = network::http::TcpServer("0.0.0.0", 8080);
    server.start();
    return 0;
}