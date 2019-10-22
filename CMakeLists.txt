cmake_minimum_required(VERSION 3.15)
project(Distributed_Systems_Fall19)

set(CMAKE_CXX_STANDARD 14)

add_executable(Distributed_Systems_Fall19 headers/Client.h headers/Server.h headers/UDPSocket.h headers/Message.h headers/UDPServerSocket.h headers/UDPClientSocket.h)
