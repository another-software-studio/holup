#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/config.hpp>

#include <spdlog/spdlog.h>

#include <thread>
#include <string>
#include <memory>

#define ARG_ERROR 1
#define UNX_ERROR 2
#define DEF_PORT 9999
#define DEF_HOST "0.0.0.0"

namespace beast = boost::beast;
namespace http  = beast::http;
namespace net   = boost::asio;
    using tcp   = boost::asio::ip::tcp;

template<typename Body, typename Allocator>
http::message_generator handle_request(http::request<Body, http::basic_fields<Allocator>>&& req) {
    spdlog::info("handling request");
    http::response<http::empty_body> res{http::status::ok, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/text");
    res.keep_alive(req.keep_alive());
    spdlog::info("sending response back");

    return res;
}

void do_session(tcp::socket& socket) {
    spdlog::info("request received");
    beast::error_code ec;
    beast::flat_buffer buffer;

    while(true) {
        http::request<http::string_body> req;
        spdlog::info("reading request");
        http::read(socket, buffer, req, ec);

        if (http::error::end_of_stream == ec) {
            break;
        }
        if (ec) {
            return spdlog::error("{0} read", ec.message());
        }

        spdlog::info("after stream check");
        http::message_generator msg =
            handle_request(std::move(req));

        bool keep_alive = msg.keep_alive();
        beast::write(socket, std::move(msg), ec);

        if (ec) {
            return spdlog::error("{0} write", ec.message());
        }
        if (!keep_alive) {
            break;
        }
    }
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
        spdlog::info("initialization done");

        while(true) {
            spdlog::info("listening...");
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
