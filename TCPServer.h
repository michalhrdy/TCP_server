//
// Created by michalh on 2/3/2020.
//
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <functional>
#include <memory>
#include <vector>
#include <array>

#include "TCPConnection.h"


#ifndef ASIO_TEST_SERVER_2_TCPSERVER_H
#define ASIO_TEST_SERVER_2_TCPSERVER_H

using asio::ip::tcp;

//Accepts connections from TCPClient's declared in TCPClient.h
//and creates instances of TCPConnection declared in TCPConnection.h
class TCPServer {
public:
    //Runs StartAccept()
    TCPServer(asio::io_context& io_context);

private:
    tcp::acceptor acceptor_;

    std::array<std::unique_ptr<TCPConnection>, CParams::max_number_of_connections> connections_;
    typedef std::array<std::unique_ptr<TCPConnection>, CParams::max_number_of_connections>::iterator connections_iterator;

    void StartAccept();
    //Starts work on newly created connection and calls StartAccept()
    void HandleAccept(connections_iterator new_connection, const asio::error_code& error);

    //Starts work loop for new connection
    void StartConnection(connections_iterator connection);
    //Asynchronously writes data to socket_
    void Write(connections_iterator connection);
    //Write handler - Calls Read() after Write() finished writing to socket_
    void HandleWrite(connections_iterator connection, const asio::error_code& error, std::size_t bytes_transferred);
    //Asynchronously reads data from socket_
    void Read(connections_iterator connection);
    //Read handler - Calls Read() after Write() finished writing to socket_
    void HandleRead(connections_iterator connection, const asio::error_code& error, std::size_t bytes_transferred);

    void ProcessRequest(connections_iterator connection, std::string request);
};

#endif //ASIO_TEST_SERVER_2_TCPSERVER_H
