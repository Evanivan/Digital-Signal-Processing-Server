#pragma once

#include <memory>
#include "easylogging++.h"


/**
 * @class LoggerWrapper
 * @brief Обертка для инициализации и управления логированием с помощью easylogging++.
 */
class LoggerWrapper {
public:

    /**
     * @brief Инициализирует логирование с указанием пути к лог-файлу.
     *
     * @param logFilePath Путь к файлу логирования. Если пусто, логирование в файл отключается.
     */
    static void init(const std::string& logFilePath);
    
private:
    std::string logFilePath{};
};
