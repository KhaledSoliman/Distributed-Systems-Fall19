#include "../headers/UDPClientSocket.h"

UDPClientSocket::UDPClientSocket() : UDPSocket() {

}

bool UDPClientSocket::initializeClient(std::string _peerAddr, int _peerPort) {
    return UDPSocket::initializeClient(_peerAddr, _peerPort);
}

UDPClientSocket::~UDPClientSocket() {

}

