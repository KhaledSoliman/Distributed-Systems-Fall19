#include "../headers/UDPSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdio>
#include <zconf.h>
#include <pthread.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <cstring>

UDPSocket::UDPSocket() {
    this->enabled = false;
    this->mutex = PTHREAD_MUTEX_INITIALIZER;
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
    this->myAddr.sin_addr.s_addr = inet_addr(this->myAddress);
    if (bind(this->sock, (struct sockaddr *) &this->myAddr, sizeof(struct sockaddr_in)) != 0) {
        perror("Server Socket binding failed\n");
        return false;
    } else {
        std::cout << "Server Socket bound on IPv4: " << this->myAddress << ", Port: " << this->myPort << std::endl;
    }
    return true;
}

bool UDPSocket::initializeClient(char *_peerAddr, int _peerPort) {
    this->peerAddress = _peerAddr;
    this->peerPort = _peerPort;
    this->myAddr.sin_family = AF_INET;
    this->myAddr.sin_port = htons(0);
    this->myAddr.sin_addr.s_addr =  htonl(INADDR_ANY);
    if (bind(this->sock, (struct sockaddr *) &this->myAddr, sizeof(struct sockaddr_in)) != 0) {
        perror("Client Socket binding failed\n");
        return false;
    } else {
        std::cout << "Client Socket bound" << std::endl;
    }
    this->peerAddr.sin_family = AF_INET;
    this->peerAddr.sin_port = htons(this->peerPort);
    this->peerAddr.sin_addr.s_addr = inet_addr(this->peerAddress);
    return true;
}

int UDPSocket::writeToSocket(char *buffer, int maxBytes) {
    int n;
    if ((n = sendto(this->sock, buffer, maxBytes, 0,  (struct sockaddr *) &this->peerAddr, sizeof(struct sockaddr_in))) < 0)
        perror("Writing to socket failed\n");
    return n;
}

int UDPSocket::writeToSocketAndWait(char *buffer, int maxBytes, int microSec) {
    int n;
    if ((n = sendto(this->sock, buffer, maxBytes, 0, (struct sockaddr *) &this->peerAddr, sizeof(struct sockaddr_in))) < 0)
        perror("Send 2 failed\n");
    return n;

}

int UDPSocket::readFromSocketWithNoBlock(char *buffer, int maxBytes) {
    int n;
    socklen_t aLength = sizeof(this->peerAddr);
    if ((n = recvfrom(this->sock, &buffer, maxBytes, 0,  (struct sockaddr *) &this->peerAddr, &aLength)) < 0)
        perror("Receiving from socket failed");
    return n;
}

int UDPSocket::readFromSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec, int timeoutMilli) {
    int n;
    socklen_t aLength;
    aLength = sizeof(myAddr);
    myAddr.sin_family = AF_INET; /* note that this is needed */
    if ((n = recvfrom(this->sock, buffer, maxBytes, 0,  (struct sockaddr *) &this->myAddr, &aLength)) < 0)
        perror("Receive 1");
    return n;
}

int UDPSocket::readFromSocketWithBlock(char *buffer, int maxBytes) {
    int n;
    socklen_t aLength = sizeof(this->peerAddr);
    peerAddr.sin_family = AF_INET;
    if((n = recvfrom(this->sock, buffer, maxBytes, 0,(struct sockaddr *)  &this->peerAddr, &aLength))<0)
        perror("Receive 1") ;
    return n;
}

int UDPSocket::readSocketWithNoBlock(char *buffer, int maxBytes) {
    int n;
    socklen_t aLength = sizeof(this->peerAddr);
    peerAddr.sin_family = AF_INET;
    if((n = recvfrom(this->sock, buffer, maxBytes, 0,(struct sockaddr *)  &this->peerAddr, &aLength))<0)
        perror("Receive 1") ;
    return n;
}

int UDPSocket::readSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec, int timeoutMilli) {
    int n;
    socklen_t aLength = sizeof(this->peerAddr);
    peerAddr.sin_family = AF_INET;
    struct pollfd pfd = {.fd = this->sock, .events = POLLIN};
    if((n = poll(&pfd, 1, timeoutMilli)) == 0) {
        strcpy(buffer, "Server Timed Out!");
        return n;
    } else {
        if((n = recvfrom(this->sock, buffer, maxBytes, 0,(struct sockaddr *)  &this->peerAddr, &aLength))<0)
            perror("Receive 1") ;
        return n;
    }
}

int UDPSocket::readSocketWithBlock(char *buffer, int maxBytes) {
    /*int n;
    socklen_t aLength = sizeof(this->peerAddr);
    peerAddr.sin_family = AF_INET;*/
    return 0;
}

int UDPSocket::getMyPort() {
    return this->myPort;
}

int UDPSocket::getPeerPort() {
    return this->peerPort;
}

void UDPSocket::enable() {
    this->enabled = true;
}

void UDPSocket::disable() {
    this->enabled = false;
}

bool UDPSocket::isEnabled() {
    return this->enabled;
}

void UDPSocket::lock() {
    pthread_mutex_lock(&this->mutex);

}

void UDPSocket::unlock() {
    pthread_mutex_unlock(&this->mutex);
}

int UDPSocket::getSocketHandler() {
    return this->sock;
}

UDPSocket::~UDPSocket() {
    close(this->sock);
}
