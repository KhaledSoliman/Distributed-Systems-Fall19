#include "../headers/UDPServerSocket.h"

UDPServerSocket::UDPServerSocket() : UDPSocket() {

}

bool UDPServerSocket::initializeServer(std::string _myAddr, int _myPort) {
    return UDPSocket::initializeServer(_myAddr, _myPort);
}

UDPServerSocket::~UDPServerSocket() {

}
