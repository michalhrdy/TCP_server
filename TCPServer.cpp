//
// Created by michalh on 2/3/2020.
//
#include "TCPServer.h"
#include "TCPConnection.h"

TCPServer::TCPServer(asio::io_context& io_context):
                        acceptor_(io_context, tcp::endpoint(tcp::v4(), std::stoi(CParams::service))) {
    StartAccept();
}


void TCPServer::StartAccept() {
//    TCPConnection::pointer new_connection =
//            TCPConnection::Create(acceptor_.get_executor().context());


    connections_.push_back(
            std::make_unique<TCPConnection>(acceptor_.get_executor().context()));

    acceptor_.async_accept(connections_.back()->Socket(),
                           std::bind(&TCPServer::HandleAccept, this,
                                     std::placeholders::_1));
}

void TCPServer::HandleAccept(const asio::error_code& error) {
    if (!error) {
        connections_.back().get()->Start();
        std::cout << "New Connection Established" << std::endl;
    }

    StartAccept();
}