#include "./config_reader.h"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

ServerConfig ConfigManager::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Config file not found: " + filename);

    json config;
    file >> config;

    return ServerConfig {
        .port = static_cast<uint16_t>(config.value("port", 8080)),
        .log_path = config.value("log_path", "server.log"),
        .median_window_x = config.value("median_window_x", 3),
        .median_window_y = config.value("median_window_y", 3),
        .gaussian_sigma_x = config.value("gaussian_sigma_x", 1.0),
        .gaussian_sigma_y = config.value("gaussian_sigma_y", 1.0),
        .moving_window = config.value("moving_window", 5)
    };
}
