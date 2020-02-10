//
// Created by michalh on 2/3/2020.
//

#include <connection_globals.h>
#include "TCPConnection.h"

TCPConnection::TCPConnection(asio::io_context& io_context, TCPServer* server)
                            : socket_(io_context),
                            server_(server){
}

TCPConnection::~TCPConnection() {
    std::cout << std::endl << "Connection Destructor" << std::endl;
}

tcp::socket& TCPConnection::Socket() {
    return socket_;
}

void TCPConnection::Start() {
    std::string c = "Connected";
    message_to_send_ = c + CParams::message_delimiter;
    Write();
}

void TCPConnection::Write() {
    asio::async_write(socket_, asio::buffer(message_to_send_),
                      std::bind(&TCPConnection::HandleWrite,
                                this,
                                 std::placeholders::_1,
                                 std::placeholders::_2));
}

void TCPConnection::HandleWrite(const asio::error_code& error, std::size_t bytes_transferred) {
    if(error){
        std::cout << error.message() << std::endl;
    } else {
        Read();
    }
}

void TCPConnection::Read() {
    asio::async_read_until(socket_, recieved_message_buffer_,
                           CParams::message_delimiter,
                           std::bind(&TCPConnection::HandleRead,
                                     this,
                                     std::placeholders::_1,
                                     std::placeholders::_2));
}

void TCPConnection::HandleRead( const asio::error_code& error, std::size_t bytes_transferred) {
    if(error) {
        std::cout << error.message() << std::endl;
        return;
    } else {
        std::string s( (std::istreambuf_iterator<char>(&recieved_message_buffer_)),
                       std::istreambuf_iterator<char>() );
        //Remove delimiter
        s.erase(s.size() - 1);

        unsigned int s_index = 0;
        for(auto it = service_table.begin(); it != service_table.end(); ++it) {
            if(s == it->first) {
                std::cout << "Request: " << it->second << std::endl;
                message_to_send_ = ProcessRequest(s_index);
                break;
            }
            ++s_index;
        }
        if(s_index > service_table.size()) {
            std::cout << "Closing connection" << std::endl;
            return;
        }
    }

    Write();
}

std::string TCPConnection::ProcessRequest(unsigned int service_index) {

    return service_lambdas[service_index]() + CParams::message_delimiter;
}


std::string TCPConnection::MakeDayTimeString() {
    std::time_t now = std::time(0);
    std::string formatted = std::ctime(&now);
    formatted.erase(formatted.size() - 1);
    return formatted;
}
