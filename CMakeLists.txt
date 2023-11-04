cmake_minimum_required(VERSION 3.27.1)

file(READ "VERSION" ver)
file(GLOB SOURCES "src/*.cpp")
project(holup VERSION ${ver})

find_package(ftxui REQUIRED)
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS boost_beast)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
    PRIVATE spdlog::spdlog
    PRIVATE ${Boost_LIBRARIES})
