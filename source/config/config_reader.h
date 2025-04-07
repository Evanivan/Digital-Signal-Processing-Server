#pragma once
#include <string>
#include <nlohmann/json.hpp>


/**
 * @struct ServerConfig
 * @brief Структура конфигурации сервера.
 */
struct ServerConfig {
    uint16_t port;              // Порт сервера.
    std::string log_path;       // Путь к лог-файлу.
    int median_window_x;        // Размер окна медианного фильтра по оси X.
    int median_window_y;        // Размер окна медианного фильтра по оси Y.
    double gaussian_sigma_x;    // Сигма гауссовского фильтра по оси X.
    double gaussian_sigma_y;    // Сигма гауссовского фильтра по оси Y.
    int moving_window;          // Размер скользящего окна усреднения.
};

class ConfigManager {
public:
    /**
     * @brief Загружает конфигурацию сервера из файла.
     *
     * @param filename Путь к файлу конфигурации.
     * @return ServerConfig Объект конфигурации сервера.
     * @throws std::runtime_error Если файл конфигурации не найден.
     */
    static ServerConfig loadConfig(const std::string& filename);
};
