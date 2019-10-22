#include "headers/UDPSocket.h"

UDPSocket::UDPSocket() {

}

void UDPSocket::setFilterAddress(char *_filterAddress) {

}

char *UDPSocket::getFilterAddress() {
    return nullptr;
}

bool UDPSocket::initializeServer(char *_myAddr, int _myPort) {
    return false;
}

bool UDPSocket::initializeClient(char *_peerAddr, int _peerPort) {
    return false;
}

int UDPSocket::writeToSocket(char *buffer, int maxBytes) {
    return 0;
}

int UDPSocket::writeToSocketAndWait(char *buffer, int maxBytes, int microSec) {
    return 0;
}

int UDPSocket::readFromSocketWithNoBlock(char *buffer, int maxBytes) {
    return 0;
}

int UDPSocket::readFromSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec, int timeoutMilli) {
    return 0;
}

int UDPSocket::readFromSocketWithBlock(char *buffer, int maxBytes) {
    return 0;
}

int UDPSocket::readSocketWithNoBlock(char *buffer, int maxBytes) {
    return 0;
}

int UDPSocket::readSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec, int timeoutMilli) {
    return 0;
}

int UDPSocket::readSocketWithBlock(char *buffer, int maxBytes) {
    return 0;
}

int UDPSocket::getMyPort() {
    return 0;
}

int UDPSocket::getPeerPort() {
    return 0;
}

void UDPSocket::enable() {

}

void UDPSocket::disable() {

}

bool UDPSocket::isEnabled() {
    return false;
}

void UDPSocket::lock() {

}

void UDPSocket::unlock() {

}

int UDPSocket::getSocketHandler() {
    return 0;
}

UDPSocket::~UDPSocket() {

}
