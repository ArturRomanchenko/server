#include "server.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <iomanip>
#include <chrono>

#define BUFFER_SIZE 30720

namespace
{

    void log(const std::string &message)
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm* tm = std::localtime(&time);

        std::ostringstream timeStream;
        timeStream << std::put_time(tm, "%Y-%m-%d %H:%M:%S");

        std::string timestamp = timeStream.str();
        std::string logMessage = "[" + timestamp + "] " + message;

        std::ofstream out("../logs/server.log", std::ios::app);
        if (out.is_open())
        {
            out << logMessage << '\n';
        }
        else
        {
            std::cerr << "Error opening '.log' file for writing!" << std::endl;
        }
        out.close();
    }


    void exitWithError(const std::string &error_message)
    {
        log(error_message);
        exit(1);
    }

}; //namecpace


namespace network::http
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
            ss << "[ERORR] [Server] Failed to start server with PORT: " << ntohs(socket_address.sin_port);
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
            exitWithError("[ERORR] [Server] Cannot create socket");
            return 1;
        }

        if (bind(server_socket, (sockaddr *)&socket_address, socket_address_length) < 0)
        {
            exitWithError("[ERORR] [Server] Cannot connect socket to address");
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

    void TcpServer::start()
    {
        if (listen(server_socket, 20) < 0)
        {
            exitWithError("[ERORR] [Server] Socket listen failed");
        }

        std::ostringstream ss;
        ss << "[INFO] [Server] Listening on ADDRESS: " << inet_ntoa(socket_address.sin_addr) << " PORT: " << ntohs(socket_address.sin_port) << "\n";
        log(ss.str());

        int bytesReceived;

        while (true)
        {
            acceptConnection(server_new_socket);
            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(server_new_socket, buffer, BUFFER_SIZE);

            if (bytesReceived < 0)
            {
                exitWithError("[ERORR] [Server] Failed to read bytes from client socket connection");
            }

            std::ostringstream ss;
            ss << "[INFO] [Server] Received Request from client\n";
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
            ss << "[ERORR] [Server] Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(socket_address.sin_addr) << "; PORT: " << ntohs(socket_address.sin_port);
            exitWithError(ss.str());
        }
    }

    std::string TcpServer::buildResponse()
    {
        std::ifstream htmlFile("../src/index.html");
        if (!htmlFile.is_open())
        {
            log("[ERORR] [Server] File ./src/index.html is not opened");
            return "HTTP/1.1 500 Internal Server Error\r\n"
                "Content-Type: text/plain\r\n\r\n"
                "Failed to load HTML file.";
        }

        // Читаем содержимое файла в строку
        std::ostringstream buffer;
        buffer << htmlFile.rdbuf();
        std::string htmlContent = buffer.str();

        // Формируем HTTP-ответ
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\r\n"
        << "Content-Type: text/html\r\n"
        << "Content-Length: " << htmlContent.size() << "\r\n\r\n"
        << htmlContent;

        return ss.str();
    }

    
    void TcpServer::sendResponse()
    {
        long bytesSent = write(server_new_socket, server_message.c_str(), server_message.size());

        if (bytesSent == server_message.size())
        {
            log("[INFO] [Server] Server Response sent to client\n");
        }
        else
        {
            log("[ERORR] [Server] Error sending response to client");
        }
    }
    
}; // namespace http