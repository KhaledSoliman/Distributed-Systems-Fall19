cmake_minimum_required(VERSION 3.14)
project(Distributed_Systems_Fall19 C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Proj headers/Client.h headers/Server.h headers/UDPSocket.h headers/Message.h headers/UDPServerSocket.h headers/UDPClientSocket.h main.cpp Message.cpp)
set_target_properties(Proj PROPERTIES LINKER_LANGUAGE CXX)