#include "request.hpp"

namespace holup {

ftxui::Color RequestEntry::getMethodtoColor() const noexcept {
    switch(this->method) {
        case Method::CONNECT: return ftxui::Color::Blue;
        case Method::DELETE:  return ftxui::Color::Red;
        case Method::GET:     return ftxui::Color::GreenLight;
        case Method::HEAD:    return ftxui::Color::NavyBlue;
        case Method::OPTIONS: return ftxui::Color::GrayLight;
        case Method::POST:    return ftxui::Color::DarkOrange;
        case Method::PUT:     return ftxui::Color::Orange1;
        case Method::TRACE:   return ftxui::Color::White;
        default:              return ftxui::Color::Red;
    }
}

std::string RequestEntry::getMethodName() const noexcept {
    switch(this->method) {
        case Method::CONNECT: return "CONNECT";
        case Method::DELETE:  return "DELETE";
        case Method::GET:     return "GET";
        case Method::HEAD:    return "HEAD";
        case Method::OPTIONS: return "OPTIONS";
        case Method::POST:    return "POST";
        case Method::PUT:     return "PUT";
        case Method::TRACE:   return "TRACE";
        default:              return "UNKNOWN";
    }
}

ftxui::Element RequestEntry::Render() const noexcept {
    return ftxui::hbox({
        ftxui::text(std::to_string(this->no)) | size(ftxui::WIDTH, ftxui::LESS_THAN, 5),
        ftxui::separator(),
        ftxui::text(getMethodName())
            | size(ftxui::WIDTH, ftxui::EQUAL, 7) | ftxui::color(getMethodtoColor()),
        ftxui::separator(),
        ftxui::text(this->host),
        ftxui::separatorEmpty(),
        ftxui::text(this->params) | size(ftxui::WIDTH, ftxui::LESS_THAN, 20),
    });
}

}
