cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "E:/Program Files/vcpkg/scripts/buildsystems/vcpkg.cmake")
 
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

include_directories("E:/Program Files/vcpkg/installed/x64-windows/include")

project(software_renderer)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/gui/window.hpp
    src/gui/window.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/models"
    "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/models")