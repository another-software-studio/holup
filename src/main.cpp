#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <spdlog/spdlog.h>
#include <sys/socket.h>

#include <iostream>
#include <thread>
#include <string>
#include <memory>

#define ARG_ERROR 1
#define UNX_ERROR 2

#define MIN_ARGC 2
#define DEF_PORT 9999
#define DEF_HOST "0.0.0.0"

namespace beast = boost::beast;
namespace http  = beast::http;
namespace net   = boost::asio;
using tcp       = boost::asio::ip::tcp;

void do_session(tcp::socket& socket) {
    beast::error_code ec;

    spdlog::info("request received");

    socket.shutdown(tcp::socket::shutdown_send, ec);
}

int main(int argc, char* argv[]) noexcept {
    try {
        /* temporarily commented out - we will use default values for most of the time
        if (argc != 2) {
            spdlog::critical("invalid argument count; got {0}, expected {1}",
                argc - 1, MIN_ARGC - 1);    
            spdlog::info("Usage: holup [address] [port]");

            return ARG_ERROR;
        }*/

        auto const address = net::ip::make_address(DEF_HOST);
        auto const port    = static_cast<unsigned short>(DEF_PORT);
        
        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {address, port}};

        while(true) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            std::thread{std::bind(
                &do_session,
                std::move(socket)
            )}.detach();
        }

    } catch(const std::exception& e) {
        spdlog::critical("error: {0}", e.what());

        return UNX_ERROR;
    }
    
    return 0;
}
