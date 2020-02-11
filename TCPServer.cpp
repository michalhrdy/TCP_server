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

    for(auto it = connections_.begin(); it != connections_.end(); ++it) {

        if(it->get() == nullptr) {
            *it = std::make_unique<TCPConnection>(acceptor_.get_executor().context());

            acceptor_.async_accept(it->get()->Socket(),
                    std::bind(&TCPServer::HandleAccept,this,
                            it,
                            std::placeholders::_1));
            break;
        }
    }
    //TODO handle if there are no free slots
}

void TCPServer::HandleAccept(connections_iterator new_connection, const asio::error_code& error) {
    if (!error) {
        new_connection->get()->Start();
        std::cout << "New Connection Established" << std::endl;
    } else {
        std::cout << error.message() << std::endl;
        new_connection->reset();
    }
    StartAccept();
}