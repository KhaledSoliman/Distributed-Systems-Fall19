#include "UDPSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <cstdio>
#include <zconf.h>

extern "C" {
char *inet_ntoa(struct in_addr);
}

UDPSocket::UDPSocket() {
    this->enabled = false;
    if ((this->sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed\n");
    }
}

void UDPSocket::setFilterAddress(char *_filterAddress) {
}

char *UDPSocket::getFilterAddress() {
    return nullptr;
}

bool UDPSocket::initializeServer(char *_myAddr, int _myPort) {
    this->myAddress = _myAddr;
    this->myPort = _myPort;
    this->myAddr.sin_family = AF_INET;
    this->myAddr.sin_port = htons(_myPort);
    this->myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(this->sock, reinterpret_cast<const sockaddr *>(&myAddr), sizeof(struct sockaddr_in)) != 0){
        perror("Socket binding failed\n");
        return false;
    }
    return true;
}

bool UDPSocket::initializeClient(char *_peerAddr, int _peerPort) {
    struct hostent *host;
    this->peerAddress = _peerAddr;
    this->peerPort = _peerPort;
    this->peerAddr.sin_family = AF_INET;
    if ((host = gethostbyname(this->peerAddress)) == nullptr) {
        printf("Unknown host name\n");
        return false;
    }
    this->peerAddr.sin_port = htons(_peerPort);
    this->peerAddr.sin_addr = *(struct in_addr *) (host->h_addr);
    if(bind(this->sock, reinterpret_cast<const sockaddr *>(&peerAddr), sizeof(struct sockaddr_in)) != 0){
        perror("Socket binding failed\n");
        return false;
    }
    return true;
}

int UDPSocket::writeToSocket(char *buffer, int maxBytes) {

    makeDestSA(&yourSocketAddress,machine, port);
    if( (n = sendto(s, message1, strlen(message1), 0, &yourSocketAddress,
                    sizeof(struct sockaddr_in))) < 0) perror("Send 2 failed\n");
    if( (n = sendto(s, message2, strlen(message2), 0, &yourSocketAddress,
                    sizeof(struct sockaddr_in))) < 0) perror("Send 2 failed\n");
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
    return this->myPort;
}

int UDPSocket::getPeerPort() {
    return this->peerPort;
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
    close(this->sock);
}
