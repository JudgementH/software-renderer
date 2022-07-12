cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project(software_renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -fopenmp")


find_package(Eigen3 CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.h" ,
    "${PROJECT_SOURCE_DIR}/src/*.hpp" ,
    "${PROJECT_SOURCE_DIR}/src/*.c" ,
    "${PROJECT_SOURCE_DIR}/src/*.cpp")

# add_executable(${PROJECT_NAME}
# src/main.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyobjloader::tinyobjloader)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/models"
    "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/models")