cmake_minimum_required(VERSION 3.15)
project(ASIO_test_server_2)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DASIO_STANDALONE)

include_directories(C:/ASIO/asio-1.12.2/include C:/Users/michalh/CLionProjects/includes)

add_executable(ASIO_test_server_2 main.cpp TCPServer.h TCPConnection.h TCPConnection.cpp TCPServer.cpp)

if(WIN32)
    target_link_libraries(ASIO_test_server_2 wsock32 ws2_32)
endif()