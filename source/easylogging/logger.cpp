#include "./logger.h"
#include "logger.h"

INITIALIZE_EASYLOGGINGPP

/**
 * @var logger_
 * @brief Указатель на глобальный логгер.
 */
std::shared_ptr<el::Logger> logger_;

void LoggerWrapper::init(const std::string& logFilePath) {
        el::Configurations conf;
        conf.setToDefault();
        
        // Настройка логирования в файл
        if (!logFilePath.empty()) {
            conf.setGlobally(el::ConfigurationType::Filename, logFilePath);
            conf.setGlobally(el::ConfigurationType::ToFile, "true");
        } else {
            // Отключение логирования в файл, если путь не указан
            conf.setGlobally(el::ConfigurationType::ToFile, std::string("false"));
        }

        el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
        conf.setGlobally(el::ConfigurationType::ToStandardOutput, std::string("true"));
        conf.setGlobally(el::ConfigurationType::PerformanceTracking, std::string("true"));
        // Установка максимального размера лог-файла (100 МБ)
        conf.setGlobally(el::ConfigurationType::MaxLogFileSize, std::string("104857600"));
        conf.setGlobally(el::ConfigurationType::LogFlushThreshold, std::string("0"));

        conf.setGlobally(el::ConfigurationType::Format, std::string("%datetime %level [%logger] %msg"));
        conf.set(el::Level::Debug, el::ConfigurationType::Format,
            std::string("%datetime %level [%logger] [%user@%host] [%func] [%loc] %msg"));
        // INFO and WARNING are set to default by Level::Global
        conf.set(el::Level::Error, el::ConfigurationType::Format, std::string("%datetime %level [%logger] %msg"));
        conf.set(el::Level::Fatal, el::ConfigurationType::Format, std::string("%datetime %level [%logger] %msg"));
        conf.set(el::Level::Verbose, el::ConfigurationType::Format, std::string("%datetime %level-%vlevel [%logger] %msg"));
        conf.set(el::Level::Trace, el::ConfigurationType::Format, std::string("%datetime %level [%logger] [%func] [%loc] %msg"));
        
        // Переконфигурация логгера по умолчанию
        el::Loggers::reconfigureLogger("default", conf);
        el::Loggers::addFlag(el::LoggingFlag::LogDetailedCrashReason);
        
        // Инициализация глобальной переменной
        logger_ = std::shared_ptr<el::Logger>(
            el::Loggers::getLogger("default"),
            [](el::Logger*){} // Пустой делитер
        );
}