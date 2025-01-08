cmake_minimum_required(VERSION 3.30)
project(net)

set(CMAKE_CXX_STANDARD 20)

#add_definitions(-D_WIN32_WINNT=0x0A00)
# 设置 Boost 的路径
set(ASIO_DIR ${CMAKE_SOURCE_DIR}/third_party/asio-1.30.2)
include_directories(${ASIO_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(net main.cpp
        include/Net.h
        src/Net.cpp
        src/Acceptor.cpp
        include/Acceptor.h
        src/Session.cpp
        include/Session.h
)
#target_link_libraries(net ws2_32)
