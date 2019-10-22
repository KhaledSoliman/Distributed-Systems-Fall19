#include "headers/UDPServerSocket.h"

UDPServerSocket::UDPServerSocket() {

}

bool UDPServerSocket::initializeServer(char *_myAddr, int _myPort) {
    return UDPSocket::initializeServer(_myAddr, _myPort);
}

UDPServerSocket::~UDPServerSocket() {

}
