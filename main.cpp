#include "TCPServer.h"

int main() {
    asio::io_context io_context;
    TCPServer server(io_context);
    io_context.run();

    std::cout << "end od main" << std::endl;
    return 0;
}