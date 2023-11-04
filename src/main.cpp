#include "request.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <memory>
#include <list>

#define AUTHOR "anothersoftware.studio"
#define INT_MODE "--INTERCEPT--"
#define EDT_MODE "--EDIT--"
#define LOG_FILENAME "holup.log"

#define DEBUG
#define R_SUCCESS 0

std::list<holup::RequestEntry> request_entries{
    {8, holup::Method::POST,    "testwb.com",    "/"},
    {7, holup::Method::GET,     "testwb.com", "search=xyz"},
    {6, holup::Method::CONNECT, "testwb.com", "search=hkj"},
    {5, holup::Method::DELETE,  "testwb.com", "search=abzs"},
    {4, holup::Method::OPTIONS, "testwb.com", "search=ass2"},
    {3, holup::Method::PUT,     "testwb.com", "search=dsadsa"},
    {2, holup::Method::TRACE,   "testwb.com", "search=dsa"},
    {1, holup::Method::HEAD,    "testwb.com", "search=dsa"}
};

int main(int argc, char* argv[]) noexcept {
    try { // Setup logger - TODO: consider adding switch to disable logging
        auto logger = spdlog::basic_logger_mt("basic_logger", LOG_FILENAME);
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::info);
    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }

    auto render = [&]() -> ftxui::Element {
        std::vector<ftxui::Element> entries;
        for (auto& entry : request_entries) {
            entries.push_back(entry.Render());
        }
        // auto table = ftxui::Table(entries);

        // TODO: try table
        return ftxui::vbox(std::move(entries));
    };

    auto component = ftxui::Renderer([&]() -> ftxui::Element {
        return ftxui::vbox({
                ftxui::hbox({
                    render() | ftxui::border,
                    ftxui::vbox({
                        ftxui::text("body editor") | ftxui::flex,
                        ftxui::separator(),
                        ftxui::text("header editor") | ftxui::flex
                    }) | ftxui::border | ftxui::flex
                }), ftxui::hbox({
                    ftxui::text(INT_MODE) | ftxui::bold | ftxui::flex,
                    ftxui::filler(),
                    ftxui::text(AUTHOR)
                        | ftxui::hyperlink("https://anothersoftware.studio")
                })
        }) | ftxui::flex;
    });
    auto screen = ftxui::ScreenInteractive::Fullscreen();
    screen.Loop(component);
    
    return R_SUCCESS;
}
