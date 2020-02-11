//
// Created by michalh on 2/3/2020.
//
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <functional>
#include <memory>
#include "connection_globals.h"

#ifndef ASIO_TEST_SERVER_2_TCPCONNECTION_H
#define ASIO_TEST_SERVER_2_TCPCONNECTION_H

using asio::ip::tcp;

class TCPServer;

//Handles communication with TCPClient declared in TCPClient.h
class TCPConnection {
public:
    TCPConnection(asio::io_context& io_context);
    ~TCPConnection();

    //
    tcp::socket& Socket();
    //Starts work loop of the connection
    void Start();

    //
    tcp::socket socket_;
    //Incoming data buffer
    asio::streambuf recieved_message_buffer_;
    //Outgoing data
    std::string message_to_send_;

private:
    //Asynchronously writes data to socket_
    void Write();
    //Write handler - Calls Read() after Write() finished writing to socket_
    void HandleWrite(const asio::error_code& error, std::size_t bytes_transferred);
    //Asynchronously reads data from socket_
    void Read();
    //Read handler - Calls Read() after Write() finished writing to socket_
    void HandleRead(const asio::error_code& error, std::size_t bytes_transferred);

    //
    std::string ProcessRequest(unsigned int service_index);
    //Constructs formatted date and time string from system time
    std::string MakeDayTimeString();
};

#endif //ASIO_TEST_SERVER_2_TCPCONNECTION_H
