cmake_minimum_required(VERSION 2.8)

project(SMARTHOME)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(inc)
file(GLOB SOURCES "src/*.cpp")

add_executable(smarthome ${SOURCES})

target_link_libraries(smarthome ${OpenCV_LIBS})

target_compile_options(smarthome PRIVATE -Wall -Wextra)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")