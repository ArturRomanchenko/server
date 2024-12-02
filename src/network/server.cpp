#include "network/server.hpp"
#include "utils/logs.hpp"

#define BUFFER_SIZE 30720

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
            ss << "Failed to start server with PORT: " << ntohs(socket_address.sin_port);
            utils::logs::log(utils::logs::level::ERROR, ss.str());
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
            utils::logs::log(utils::logs::level::ERROR, "Cannot create socket");
            return 1;
        }

        if (bind(server_socket, (sockaddr *)&socket_address, socket_address_length) < 0)
        {
            utils::logs::log(utils::logs::level::ERROR, "Cannot connect socket to address");
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
            utils::logs::log(utils::logs::level::ERROR, "Socket listen failed");
        }

        std::ostringstream ss;
        ss << "Listening on ADDRESS: " << inet_ntoa(socket_address.sin_addr) << " PORT: " << ntohs(socket_address.sin_port) << "\n";
        utils::logs::log(utils::logs::level::INFO, ss.str());

        int bytesReceived;

        while (true)
        {
            acceptConnection(server_new_socket);
            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(server_new_socket, buffer, BUFFER_SIZE);

            if (bytesReceived < 0)
            {
                utils::logs::log(utils::logs::level::ERROR, "Failed to read bytes from client socket connection");
            }

            std::ostringstream ss;
            ss << "Received Request from client\n";
            utils::logs::log(utils::logs::level::INFO, ss.str());

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
            utils::logs::log(utils::logs::level::ERROR, ss.str());
        }
    }

    std::string TcpServer::buildResponse()
    {
        std::ifstream htmlFile("../src/web/index.html");
        if (!htmlFile.is_open())
        {
            utils::logs::log(utils::logs::level::ERROR, "File ./src/index.html is not opened");
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
            utils::logs::log(utils::logs::level::INFO, "Server Response sent to client\n");
        }
        else
        {
            utils::logs::log(utils::logs::level::ERROR, "Error sending response to client");
        }
    }
    
}; // namespace http