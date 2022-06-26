cmake_minimum_required(VERSION 3.10)

project(software_renderer)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/gui/window.hpp
    src/gui/window.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})