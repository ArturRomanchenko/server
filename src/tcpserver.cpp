#include "tcpserver.h"


#include <iostream>
#include <sstream>
#include <unistd.h>


namespace
{

    const int BUFFER_SIZE = 30720;

    void log(const std::string &message)
    {
        std::cout << message << '\n';
    }


    void exitWithError(const std::string &error_message)
    {
        log("ERROR: " + error_message);
        exit(1);
    }

}; //namecpace


namespace http
{

    TcpServer::TcpServer(const std::string ip_address, int port): server_ip_address(ip_address), server_port(port), server_socket(),
                                                                  server_new_socket(), incomming_message(), socket_address(),
                                                                  socket_address_length(sizeof(socket_address)), server_message(buildResponse())
    {
        socket_address.sin_family = AF_INET;
        socket_address.sin_port = htons(server_port);
        socket_address.sin_addr.s_addr = inet_addr(server_ip_address.c_str());

        if (start_server() != 0)
        {
            std::ostringstream ss;
            ss << "Failed to start server with PORT: " << ntohs(socket_address.sin_port);
            log(ss.str());
        }
    }

    TcpServer::~TcpServer()
    {
        close_server();
    }

    int TcpServer::start_server()
    {
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0)
        {
            exitWithError("Cannot create socket");
            return 1;
        }

        if (bind(server_socket, (sockaddr *)&socket_address, socket_address_length) < 0)
        {
            exitWithError("Cannot connect socket to address");
            return 1;
        }

        return 0;
    }

    void TcpServer::close_server()
    {
        close(server_socket);
        close(server_new_socket);
        exit(0);
    }

    void TcpServer::startListen()
    {
        if (listen(server_socket, 20) < 0)
        {
            exitWithError("Socket listen failed");
        }

        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << inet_ntoa(socket_address.sin_addr) << " PORT: " << ntohs(socket_address.sin_port) << " ***\n\n";
        log(ss.str());

        int bytesReceived;

        while (true)
        {
            log("~~~Waiting for a new connection\n\n");
            acceptConnection(server_new_socket);

            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(server_new_socket, buffer, BUFFER_SIZE);
            if (bytesReceived < 0)
            {
                exitWithError("Failed to read bytes from client socket connection");
            }

            std::ostringstream ss;
            ss << "~~~Received Request from client\n";
            log(ss.str());

            sendResponse();

            close(server_new_socket);
        }
    }

    void TcpServer::acceptConnection(int &new_socket)
    {
        new_socket = accept(server_socket, (sockaddr *)&socket_address, &socket_address_length);
        if (server_new_socket < 0)
        {
            std::ostringstream ss;
            ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(socket_address.sin_addr) << "; PORT: " << ntohs(socket_address.sin_port);
            exitWithError(ss.str());
        }
    }

    std::string TcpServer::buildResponse()
    {
        std::string htmlFile = R"(
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Welcome to Our Server</title>
                <style>
                    body {
                        background-color: #1f1f1f; /* Цвет фона */
                        color: #ffffff; /* Цвет текста */
                        font-family: Arial, sans-serif; /* Шрифт текста */
                        text-align: center; /* Выравнивание текста по центру */
                        margin: 0;
                        padding: 0;
                        height: 100vh;
                        display: flex;
                        flex-direction: column;
                        justify-content: center;
                    }
                    h1 {
                        color: #00ff00; /* Зеленый цвет заголовка */
                    }
                    p {
                        font-size: 18px; /* Размер текста */
                        margin-top: 20px; /* Отступ сверху */
                    }
                </style>
            </head>
            <body>
                <h1>Welcome to Our Server</h1>
                <p>Hello from your Server :)</p>
            </body>
            </html>
            )";

        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n"
           << htmlFile;

        return ss.str();
    }
    
    void TcpServer::sendResponse()
    {
        long bytesSent;

        bytesSent = write(server_new_socket, server_message.c_str(), server_message.size());

        if (bytesSent == server_message.size())
        {
            log("~~~Server Response sent to client\n");
        }
        else
        {
            log("~~~Error sending response to client");
        }
    }
    
}; // namespace http