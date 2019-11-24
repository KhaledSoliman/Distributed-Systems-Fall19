#include <string>
#include "../headers/UDPClientSocket.h"

UDPClientSocket::UDPClientSocket() : UDPSocket() {

}

bool UDPClientSocket::initializeClient(std::string _peerAddr, int _peerPort) {
    return UDPSocket::initializeClient(&_peerAddr[0], _peerPort);
}

UDPClientSocket::~UDPClientSocket() {

}

