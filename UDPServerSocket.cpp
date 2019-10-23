#include "UDPServerSocket.h"

UDPServerSocket::UDPServerSocket() : UDPSocket() {

}

bool UDPServerSocket::initializeServer(char *_myAddr, int _myPort) {
    return UDPSocket::initializeServer(_myAddr, _myPort);
}

UDPServerSocket::~UDPServerSocket() {

}
