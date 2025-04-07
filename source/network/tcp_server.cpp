#include "./tcp_server.h"
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <vector>

using namespace boost::asio;
using namespace boost::asio::ip;

TCPServer::TCPServer(boost::asio::io_context& io_ctx, const ServerConfig& config)
    : acceptor_(io_ctx, {boost::asio::ip::tcp::v4(), config.port}),
      config_(config) {
    start_accept();
}

void TCPServer::start_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
        if (!ec) handle_accept(std::move(socket));
        start_accept();
    });
}

void TCPServer::handle_accept(boost::asio::ip::tcp::socket socket) {
    std::thread([this, sock = std::move(socket)]() mutable {
        process_client(std::move(sock));
    }).detach();
}

void TCPServer::process_client(boost::asio::ip::tcp::socket socket) const {
    try {
        // Чтение заголовка
        struct { uint32_t x, y, z; } header{};
        boost::asio::read(socket, boost::asio::buffer(&header, sizeof(header)));
        
        // Чтение данных
        const size_t data_size = header.x * header.y * header.z;
        std::vector<float> data(data_size);
        boost::asio::read(socket, boost::asio::buffer(data));
        
        // Обработка
        DataProcessor processor(std::move(data), header.x, header.y, header.z);
        processor.apply_median_filter(0, config_.median_window_x);
        processor.apply_gaussian_filter(1, config_.gaussian_sigma_y);
        processor.apply_moving_average(config_.moving_window);
        
        // Статистика
        double mean, stddev;
        processor.calculate_stats(mean, stddev);
        
        // Формирование JSON
        nlohmann::json response;
        response["processed_data"] = processor.get_processed_data();
        response["stats"]["mean"] = mean;
        response["stats"]["stddev"] = stddev;
        
        // Отправка
        const std::string response_str = response.dump();
        boost::asio::write(socket, boost::asio::buffer(response_str));
        
    } catch (const std::exception& e) {
        logger_->error("Client error: ", e.what());
    }
}