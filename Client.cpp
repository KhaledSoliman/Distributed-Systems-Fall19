#include <iostream>
#include <cstring>
#include "Client.h"

Client::Client(char *_hostname, int _port) {
    this->udpSocket = new UDPClientSocket();
    this->udpSocket->initializeClient(_hostname, _port);
}

Message *Client::execute(Message *_message) {
    char *reply = static_cast<char *>(malloc(300));
    this->udpSocket->writeToSocket(_message->getMessage(), _message->getMessageSize());
    this->udpSocket->readSocketWithTimeout(reply, 1000, 5, 5000);
    std::cout << "Reply From Server: " << reply << std::endl;
    return new Message(0, reply, strlen(reply), 1);
}

Client::~Client() {
}

