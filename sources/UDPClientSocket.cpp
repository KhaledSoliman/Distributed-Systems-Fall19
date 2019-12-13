#include <string>
#include "../headers/UDPClientSocket.h"

UDPClientSocket::UDPClientSocket() : UDPSocket() {
    UDPSocket::initializeClientSocket();
}

bool UDPClientSocket::initializeClient(std::string _peerAddr, int _peerPort) {
     UDPSocket::initializeClientPeer(&_peerAddr[0], _peerPort);
     return true;
}

bool UDPClientSocket::initializeBroadcastClient(std::string _peerAddr, int _peerPort) {
    return UDPSocket::initializeBroadcastClient(&_peerAddr[0], _peerPort);
}

UDPClientSocket::~UDPClientSocket() {

}



