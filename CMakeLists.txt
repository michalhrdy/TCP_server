cmake_minimum_required(VERSION 3.15)
project(ASIO_test_server_2)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DASIO_STANDALONE)

add_executable(ASIO_test_server_2 main.cpp TCPServer.h TCPConnection.h TCPConnection.cpp TCPServer.cpp)

if(WIN32)
    include_directories(C:/Users/michalh/CLionProjects/ASIO/asio-1.12.2/include C:/Users/michalh/CLionProjects/includes)
    target_link_libraries(ASIO_test_server_2 wsock32 ws2_32)
endif(WIN32)
if(UNIX)
    include_directories(/home/michal/share/ASIO/asio-1.12.2/include/ /home/michal/share/includes/)
    target_link_libraries(ASIO_test_server_2 pthread)
endif(UNIX)