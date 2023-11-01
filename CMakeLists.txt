cmake_minimum_required(VERSION 3.27.1)

file(READ "VERSION" ver)
project(holup VERSION ${ver})

find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS boost_beast)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog ${Boost_LIBRARIES})
