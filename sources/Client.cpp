#include <iostream>
#include <cstring>
#include "../headers/Client.h"

Client::Client(std::string _hostname, int _port) {
    this->udpSocket = new UDPClientSocket();
    this->udpSocket->initializeClient(_hostname, _port);
}

Message *Client::execute(Message *_message) {
    std::string reply;
    this->udpSocket->writeToSocket(_message->getMessage(), _message->getMessageSize());
    this->udpSocket->readSocketWithTimeout(reply, 1000, 5, 5000);
    std::cout << "Reply From Server: " << reply << std::endl;
    return new Message(0, reply, reply.size(), 1);
}

Client::~Client() {
}

