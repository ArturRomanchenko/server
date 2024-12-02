#include "network/server.hpp"
#include "utils/logs.hpp"

#include <stdexcept>

int main(void)
{
    bool restart = false;

    do {
        try {
            network::http::TcpServer server("0.0.0.0", 8080);
            server.start();
            break;
        } catch (std::runtime_error& err) {
            utils::logs::log(utils::logs::level::ERROR, err.what());
        } catch (...) {
            utils::logs::log(utils::logs::level::ERROR, "Unknown error occurred.");
        }
        
        utils::logs::log(utils::logs::level::INFO, "Attempting to restart the server...");
        restart = true;
    } while (restart);
    
    return 0;
}