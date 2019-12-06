#include <iostream>
#include <cstring>
#include "../headers/Client.h"

Client::Client() {
    this->udpSocket = new UDPClientSocket();
}

Client::Client(std::string _hostname, int _port) {
    this->udpSocket = new UDPClientSocket();
    this->udpSocket->initializeClient(_hostname, _port);
}

bool Client::connectToServer(const std::string &_hostname, int _port) {
    return this->udpSocket->initializeClient(_hostname, _port);
}

Message *Client::execute(Message *_message) {
    std::string marshalled = _message->marshal();
    this->udpSocket->writeToSocket(&marshalled[0], marshalled.size());
    char* reply = static_cast<char *>(malloc(MAX_MESSAGE_SIZE));
    this->udpSocket->readSocketWithTimeout(reply, MAX_MESSAGE_SIZE, 5, 5000);
    return new Message(reply);
}

Message* Client:: {

};

Client::~Client() {
}



