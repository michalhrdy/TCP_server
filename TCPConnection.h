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

//Handles communication with TCPClient declared in TCPClient.h
class TCPConnection
        : public std::enable_shared_from_this<TCPConnection> {
public:
    TCPConnection(asio::io_context& io_context);
    ~TCPConnection();

    typedef std::shared_ptr<TCPConnection> pointer;

    //Creates shared_ptr to this class
    static pointer Create(asio::io_context& io_context);
    //
    tcp::socket& Socket();
    //Starts work loop of the connection
    void Start();

private:
    tcp::socket socket_;

    //Incoming data buffer
    asio::streambuf recieved_message_buffer_;
    //Outgoing data
    std::string message_to_send_;

    //Asynchronously writes data to socket_
    void Write();
    //Write handler - Calls Read() after Write() finished writing to socket_
    void HandleWrite(const asio::error_code&, std::size_t);
    //Asynchronously reads data from socket_
    void Read();
    //Read handler - Calls Read() after Write() finished writing to socket_
    void HandleRead(const asio::error_code& error, std::size_t bytes_transferred);

    //
    std::string ProcessRequest(std::string request);
    //Constructs formatted date and time string from system time
    std::string MakeDayTimeString();
};

#endif //ASIO_TEST_SERVER_2_TCPCONNECTION_H
