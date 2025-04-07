#pragma once
#include <boost/asio.hpp>
#include <functional>
#include "./proccessing/signal_proccessing.h"
#include "./easylogging/easylogging++.h"
#include "./config/config_reader.h"
#include "../include/globals.h"


/**
 * @class TCPServer
 * @brief TCP-сервер для обработки сигналов.
 */
class TCPServer {
public:

    /**
     * @brief Конструктор сервера.
     *
     * @param io_ctx Контекст ввода-вывода Asio.
     * @param config Конфигурация сервера.
     */
    TCPServer(boost::asio::io_context& io_ctx, const ServerConfig& config);

private:
    /**
     * @brief Начинает асинхронное ожидание новых подключений.
     */
    void start_accept();

    /**
     * @brief Обрабатывает новое подключение.
     *
     * @param socket Сокет нового подключения.
     */
    void handle_accept(boost::asio::ip::tcp::socket socket);

    /**
     * @brief Обрабатывает запрос клиента.
     *
     * @param socket Сокет клиента.
     */
    void process_client(boost::asio::ip::tcp::socket socket) const;

    boost::asio::ip::tcp::acceptor acceptor_; // Акцептор для новых подключений.
    ServerConfig config_;                     // Конфигурация сервера.
};
