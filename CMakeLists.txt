cmake_minimum_required(VERSION 3.15)
project(SignalProccessing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Кросс-платформенные настройки
if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    add_definitions(-DELPP_THREAD_SAFE)
endif()

find_package(Boost 1.67 REQUIRED COMPONENTS system)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

if(NOT googletest_POPULATED)
    message(FATAL_ERROR "Failed to fetch googletest")
endif()

if(WIN32)
    add_subdirectory(deps/easyloggingpp)
else()
    find_package(PkgConfig REQUIRED)
endif()

add_executable(SignalProccessing
    source/ini.cpp
    source/network/tcp_server.cpp
    source/proccessing/signal_proccessing.cpp
    source/easylogging/easylogging++.cpp
    source/config/config_reader.cpp
    source/easylogging/logger.cpp
)

target_include_directories(SignalProccessing PRIVATE
    source
    source/network
    source/processing
    source/config
    source/easylogging
    source/include
)

target_link_libraries(SignalProccessing
    PRIVATE 
        Boost::boost 
        Boost::system
        $<IF:$<BOOL:${UNIX}>,Threads::Threads,>
        ${CMAKE_THREAD_LIBS_INIT}
)

# Установка для Windows
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        NOMINMAX 
        _WIN32_WINNT=0x0A00
    )
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()

target_compile_definitions(SignalProccessing PRIVATE JSON_THREAD_SAFE)

option(BUILD_TESTING "Build tests" OFF)

# Настройка CTest
include(CTest)
if(BUILD_TESTING)
    enable_testing()
    # Подключение тестов
    add_subdirectory(tests)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
endif()
