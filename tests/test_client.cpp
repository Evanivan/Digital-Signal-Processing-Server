#include <boost/asio.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <cstdint>
#include <vector>
#include <gtest/gtest.h>

using namespace boost::asio;
using namespace ip;

struct TestData {
    struct {
        uint32_t x;
        uint32_t y;
        uint32_t z;
        uint32_t data_size;
    } header;
    std::vector<float> data;
};

constexpr uint16_t TEST_PORT = 54321;

TestData generate_test_data(uint32_t x, uint32_t y, uint32_t z) {
    TestData td;
    td.header = {x, y, z, x*y*z};
    td.data.resize(x*y*z);
    
    for (size_t i = 0; i < td.data.size(); ++i) {
        td.data[i] = static_cast<float>(i);
    }
    
    return td;
}

std::string read_response(boost::asio::ip::tcp::socket& socket) {
    boost::asio::streambuf buf;
    read_until(socket, buf, "\n");
    
    std::istream is(&buf);
    return std::string(
        std::istreambuf_iterator<char>(is),
        std::istreambuf_iterator<char>()
    );
}

void test_processing_flow() {
    try {
        io_context io;
        tcp::socket socket(io);
        socket.connect(tcp::endpoint(
            address::from_string("127.0.0.1"), 
            TEST_PORT
        ));

        // Подготовка тестовых данных 2x2x1
        TestData td = generate_test_data(2, 2, 1);
        
        // Отправка данных
        socket.write_some(buffer(&td.header, sizeof(td.header)));
        socket.write_some(buffer(td.data));
        
        // Получение ответа
        std::string response = read_response(socket);
        auto json = nlohmann::json::parse(response);
        
        // Проверка структуры ответа
        EXPECT_FALSE(json.find("processed_data") == json.end());
        EXPECT_FALSE(json["stats"].find("mean") == json["stats"].end());
        EXPECT_FALSE(json["stats"].find("stddev") == json["stats"].end());
        
    } catch (const std::exception& e) {
        FAIL() << "Integration test failed: " << e.what();
    }
}

int main() {
    test_processing_flow();
    return 0;
}
