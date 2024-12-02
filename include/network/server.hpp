#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace network::http
{
    
    class TcpServer
    {
    public:
        TcpServer(std::string server_ip_address, int port);
        ~TcpServer();
        void start();

    private:
        std::string server_ip_address;
        int server_port;
        int server_socket;
        int server_new_socket;
        long incomming_message;
        struct sockaddr_in socket_address;
        unsigned int socket_address_length;
        std::string server_message;
    
        int start_server();
        void close_server();
        void acceptConnection(int &server_new_socket);
        std::string buildResponse();
        void sendResponse();
    };

}; // namespace network::http


#endif //TCPSERVER_HPP