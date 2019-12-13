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
    /* Create a best-effort datagram socket using UDP */
    if ((this->broadcastSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        perror("socket() failed");

}

bool UDPSocket::initializeBroadcastServer(int broadcastPort) {
    /* Construct bind structure */
    memset(&this->peerAddr, 0, sizeof(this->peerAddr));   /* Zero out structure */
    this->peerAddr.sin_family = AF_INET;                 /* Internet address family */
    this->peerAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
    this->peerAddr.sin_port = htons(broadcastPort);      /* Broadcast port */
    /* Bind to the broadcast port */
    if (bind(this->broadcastSock, (struct sockaddr *) &this->peerAddr, sizeof(struct sockaddr_in)) != 0) {
        perror("bind() failed");
        return false;
    } else {
        std::cout << "Broadcast Socket bound on Port: " << broadcastPort << std::endl;
    }
    return false;
}

bool UDPSocket::initializeBroadcastClient(char *broadcastIP, unsigned short broadcastPort) {
    /* Set socket to allow broadcast */
    int broadcastPermission = 1;
    if (setsockopt(this->sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,
                   sizeof(broadcastPermission)) < 0)
        perror("setsockopt() failed");

    /* Construct local address structure */
    memset(&this->peerAddr, 0, sizeof(this->peerAddr));
    this->peerAddr.sin_family = AF_INET;
    this->peerAddr.sin_addr.s_addr = inet_addr(broadcastIP);
    this->peerAddr.sin_port = htons(broadcastPort);
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

bool UDPSocket::initializeClientSocket(){
    this->myAddr.sin_family = AF_INET;
    this->myAddr.sin_port = htons(0);
    this->myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(this->sock, (struct sockaddr *) &this->myAddr, sizeof(struct sockaddr_in)) != 0) {
        perror("Client Socket binding failed\n");
        return false;
    } else {
        std::cout << "Client Socket bound" << std::endl;
    }
    return true;
}

bool UDPSocket::initializeClientPeer(char *_peerAddr, int _peerPort) {
    this->peerAddress = _peerAddr;
    this->peerPort = _peerPort;
    this->peerAddr.sin_family = AF_INET;
    this->peerAddr.sin_port = htons(this->peerPort);
    this->peerAddr.sin_addr.s_addr = inet_addr(this->peerAddress);
}

int UDPSocket::writeBroadcastToSocket(char *buffer, int maxBytes) {
    int n;
    /* Broadcast sendString in datagram to clients every 3 seconds*/
    if ((n = sendto(this->sock, buffer, maxBytes, 0, (struct sockaddr *) &this->peerAddr, sizeof(this->peerAddr))) !=
        maxBytes)
        perror("sendto() sent a different number of bytes than expected");
    return n;
}

int UDPSocket::writeToSocket(char *buffer, int maxBytes) {
    int n;
    if ((n = sendto(this->sock, buffer, maxBytes, 0, (struct sockaddr *) &this->peerAddr, sizeof(struct sockaddr_in))) <
        0)
        perror("Writing to socket failed\n");
    return n;
}

int UDPSocket::writeToSocketAndWait(char *buffer, int maxBytes, int microSec) {
    int n;
    if ((n = sendto(this->sock, buffer, maxBytes, 0, (struct sockaddr *) &this->peerAddr, sizeof(struct sockaddr_in))) <
        0)
        perror("Send 2 failed\n");
    return n;

}

int UDPSocket::readFromSocketWithNoBlock(char *buffer, int maxBytes) {
    int n;
    socklen_t aLength = sizeof(this->peerAddr);
    if ((n = recvfrom(this->sock, &buffer, maxBytes, 0, (struct sockaddr *) &this->peerAddr, &aLength)) < 0)
        perror("Receiving from socket failed");
    return n;
}

int UDPSocket::readFromSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec, int timeoutMilli) {
    int n;
    socklen_t aLength;
    aLength = sizeof(myAddr);
    myAddr.sin_family = AF_INET; /* note that this is needed */
    if ((n = recvfrom(this->sock, buffer, maxBytes, 0, (struct sockaddr *) &this->myAddr, &aLength)) < 0)
        perror("Receive 1");
    return n;
}

int UDPSocket::readFromSocketWithBlock(char *buffer, int maxBytes) {
    int n;
    socklen_t aLength = sizeof(this->peerAddr);
    peerAddr.sin_family = AF_INET;
    if ((n = recvfrom(this->sock, buffer, maxBytes, 0, (struct sockaddr *) &this->peerAddr, &aLength)) < 0)
        perror("Receive 1");
    return n;
}

int UDPSocket::readSocketBroadcast(char *buffer, int maxBytes) {
    int n;
    socklen_t aLength = sizeof(this->peerAddr);
    /* Receive a single datagram from the server */
    if ((n = recvfrom(broadcastSock, buffer, maxBytes, 0, (struct sockaddr *) &this->peerAddr, &aLength)) < 0)
        perror("recvfrom() failed");
    //recvString[recvStringLen] = '\0';
    return n;
}

int UDPSocket::readSocketWithNoBlock(char *buffer, int maxBytes) {
    int n;
    socklen_t aLength = sizeof(this->peerAddr);
    peerAddr.sin_family = AF_INET;
    if ((n = recvfrom(this->sock, buffer, maxBytes, 0, (struct sockaddr *) &this->peerAddr, &aLength)) < 0)
        perror("Receive 1");
    return n;
}

int UDPSocket::readSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec, int timeoutMilli) {
    int n;
    socklen_t aLength = sizeof(this->peerAddr);
    peerAddr.sin_family = AF_INET;
    struct pollfd pfd = {.fd = this->sock, .events = POLLIN};
    if ((n = poll(&pfd, 1, timeoutMilli)) == 0) {
        strcpy(buffer, "Server Timed Out!");
        return n;
    } else {
        if ((n = recvfrom(this->sock, buffer, maxBytes, 0, (struct sockaddr *) &this->peerAddr, &aLength)) < 0)
            perror("Receive 1");
        return n;
    }
}

int UDPSocket::readSocketWithBlock(char *buffer, int maxBytes) {
    int n;
    socklen_t aLength;
    aLength = sizeof(myAddr);
    myAddr.sin_family = AF_INET; /* note that this is needed */
    if ((n = recvfrom(this->sock, buffer, maxBytes, 0, (struct sockaddr *) &this->myAddr, &aLength)) < 0)
        perror("Receive 1");
    return n;
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


void UDPSocket::setFilterAddress(char *_filterAddress) {

}

char *UDPSocket::getFilterAddress() {
    return &std::string("127.0.0.1")[0];
}

UDPSocket::~UDPSocket() {
    close(this->sock);
    close(this->broadcastSock);
}



