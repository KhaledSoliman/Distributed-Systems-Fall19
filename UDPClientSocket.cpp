#include "headers/UDPClientSocket.h"

UDPClientSocket::UDPClientSocket() {

}

bool UDPClientSocket::initializeClient(char *_peerAddr, int _peerPort) {
    return UDPSocket::initializeClient(_peerAddr, _peerPort);
}

UDPClientSocket::~UDPClientSocket() {

}

