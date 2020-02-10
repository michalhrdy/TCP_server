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

    std::vector<std::unique_ptr<TCPConnection>> connections_;

    void StartAccept();
    //Starts work on newly created connection and calls StartAccept()
    void HandleAccept(const asio::error_code& error);
};

#endif //ASIO_TEST_SERVER_2_TCPSERVER_H
