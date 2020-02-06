//
// Created by michalh on 2/3/2020.
//
#include "TCPServer.h"
#include "TCPConnection.h"

TCPServer::TCPServer(asio::io_context& io_context):
                        acceptor_(io_context, tcp::endpoint(tcp::v4(), std::stoi(CParams.service))) {
    StartAccept();
}


void TCPServer::StartAccept() {
    TCPConnection::pointer new_connection =
            TCPConnection::Create(acceptor_.get_executor().context());

    acceptor_.async_accept(new_connection->Socket(),
                           std::bind(&TCPServer::HandleAccept, this, new_connection,
                                     std::placeholders::_1));
}

void TCPServer::HandleAccept(TCPConnection::pointer new_connection,
                  const asio::error_code& error) {
    if (!error) {
        new_connection->Start();
        std::cout << "New Connection Established" << std::endl;
    }

    StartAccept();
}