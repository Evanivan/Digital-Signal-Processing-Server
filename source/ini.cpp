#include "./network/tcp_server.h"
#include "./config/config_reader.h"
#include "./include/globals.h"
#include "./easylogging/logger.h"

#include <boost/asio.hpp>
#include <memory>

/**
 * @brief Точка входа в программу
 * 
 * @return int 
 */
int main() {
    try {
        auto config = ConfigManager::loadConfig("config.json");

        // Инициализация логгера
        LoggerWrapper::init(config.log_path);
        
        boost::asio::io_context io_context;
        TCPServer server(io_context, config);

        logger_->info("Server started on port %v", (int)config.port);
        
        io_context.run();
    } catch (const std::exception& e) {
        logger_->error("Server initialization FAILED: %v", std::string(e.what()));
        return 1;
    }
    return 0;
}
