#pragma once

#include <ftxui/screen/color.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

namespace holup {

enum Method {
    CONNECT = 0,
    DELETE,
    GET,
    HEAD,
    OPTIONS,
    POST,
    PUT,
    TRACE
};

class RequestEntry {
public:
    unsigned int no;
    Method method;
    std::string host;
    std::string params;

    ftxui::Color getMethodtoColor() const noexcept;
    std::string  getMethodName()    const noexcept;

    ftxui::Element Render() const noexcept;
};

}
