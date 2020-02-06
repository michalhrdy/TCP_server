//
// Created by michalh on 2/3/2020.
//

#include <connection_globals.h>
#include "TCPConnection.h"

TCPConnection::TCPConnection(asio::io_context& io_context)
                            : socket_(io_context) {
}

TCPConnection::~TCPConnection() {
    std::cout << std::endl << "Connection Closed" << std::endl;
}

TCPConnection::pointer TCPConnection::Create(asio::io_context& io_context) {
    return TCPConnection::pointer(new TCPConnection(io_context));
}

tcp::socket& TCPConnection::Socket() {
    return socket_;
}

void TCPConnection::Start() {
    message_to_send_ = "Connected" + CParams.message_delimiter;
    Write();
}

void TCPConnection::Write() {

//    auto LHandleWrite = [shared_from_this()](const asio::error_code& error, std::size_t bytes_transferred) {
//
//    };

//    asio::async_write(socket_, asio::buffer(message_to_send_),
//            [this](const asio::error_code&, std::size_t) {
//        Read();
//    });

    asio::async_write(socket_, asio::buffer(message_to_send_),
                      std::bind(&TCPConnection::HandleWrite,
                                shared_from_this(),
                                 std::placeholders::_1,
                                 std::placeholders::_2));
}

void TCPConnection::HandleWrite(const asio::error_code&, std::size_t) {
    Read();
}

void TCPConnection::Read() {
    asio::async_read_until(socket_, recieved_message_buffer_,
                           CParams.message_delimiter,
                           std::bind(&TCPConnection::HandleRead,
                                     shared_from_this(),
                                     std::placeholders::_1,
                                     std::placeholders::_2));
}

void TCPConnection::HandleRead( const asio::error_code& error, std::size_t bytes_transferred) {

    std::string s( (std::istreambuf_iterator<char>(&recieved_message_buffer_)),
                   std::istreambuf_iterator<char>() );

    std::cout << "Request: " << s << std::flush;
    std::cout << error.message() << std::endl;

    if(!s.compare("DayTime" + CParams.message_delimiter)){
        message_to_send_ = MakeDayTimeString() + CParams.message_delimiter;
    }else if(!s.compare("Message" + CParams.message_delimiter)){
        message_to_send_ = "Hello from server" + CParams.message_delimiter;
    }else{
        std::cout << "Closing connection" << std::endl;
        return;
    }

    Write();
}

std::string TCPConnection::MakeDayTimeString() {
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}