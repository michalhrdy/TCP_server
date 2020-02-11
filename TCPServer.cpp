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
                    std::bind(&TCPServer::HandleAccept,
                            this, it,
                            std::placeholders::_1));
            break;
        }
    }
    //TODO handle if there are no free slots
}

void TCPServer::HandleAccept(connections_iterator new_connection, const asio::error_code& error) {
    if (!error) {
        //new_connection->get()->Start();
        StartConnection(new_connection);
        std::cout << "New Connection Established" << std::endl;
    } else {
        std::cout << error.message() << std::endl;
        new_connection->reset();
    }
    StartAccept();
}

void TCPServer::StartConnection(TCPServer::connections_iterator connection) {
    std::string c = "Connected";
    connection->get()->message_to_send_ = c + CParams::message_delimiter;
    Write(connection);
}

void TCPServer::Write(connections_iterator connection) {
    asio::async_write(connection->get()->Socket(), asio::buffer(connection->get()->message_to_send_),
                      std::bind(&TCPServer::HandleWrite,
                                this,connection,
                                std::placeholders::_1,
                                std::placeholders::_2));
}

void TCPServer::HandleWrite(connections_iterator connection, const asio::error_code& error, std::size_t bytes_transferred) {
    if(error){
        std::cout << error.message() << std::endl;
    } else {
        Read(connection);
    }
}

void TCPServer::Read(connections_iterator connection) {
    asio::async_read_until(connection->get()->Socket(), connection->get()->recieved_message_buffer_,
                           CParams::message_delimiter,
                           std::bind(&TCPServer::HandleRead,
                                     this,connection,
                                     std::placeholders::_1,
                                     std::placeholders::_2));
}

void TCPServer::HandleRead(connections_iterator connection, const asio::error_code& error, std::size_t bytes_transferred) {
    if(error) {
        std::cout << error.message() << std::endl;
        return;
    } else {
        std::string s( (std::istreambuf_iterator<char>(&connection->get()->recieved_message_buffer_)),
                       std::istreambuf_iterator<char>() );
        //Remove delimiter
        s.erase(s.size() - 1);
        ProcessRequest(connection, s);

        if(!connection->get()->message_to_send_.compare(CParams::close_command_string) ||
                !connection->get()->message_to_send_.compare(CParams::close_command_string)) {
            std::cout << "Closing connection" << std::endl;
            return;
        }
    }

    Write(connection);
}

void TCPServer::ProcessRequest(connections_iterator connection, std::string request) {

    connection->get()->message_to_send_ = CParams::unknown_command_string;

    for(auto it = service_table.begin(); it != service_table.end(); ++it) {
        if(!request.compare(it->first)) {
            std::cout << "Request: " << it->second << std::endl;
            connection->get()->message_to_send_ = service_lambdas[std::distance(service_table.begin(), it)]() + CParams::message_delimiter;
            break;
        }
    }
}
