#include <cstring>
#include <iostream>
#include "../headers/Message.h"
#include "../headers/Server.h"

Server::Server() {
    this->udpServerSocket = new UDPServerSocket();
}

Server::Server(const std::string &_listen_hostname, int _listen_port) {
    this->udpServerSocket = new UDPServerSocket();
    this->udpServerSocket->initializeServer(_listen_hostname, _listen_port);
}

bool Server::listen(const std::string &_listen_hostname, int _listen_port) {
    return this->udpServerSocket->initializeServer(_listen_hostname, _listen_port);
}


void Server::sendReply(Message *_message) {
    this->udpServerSocket->writeToSocket(&_message->marshal()[0],_message->marshal().length());
}

Message *Server::getRequest() {
    char* message = static_cast<char *>(malloc(MAX_MESSAGE_SIZE));
    this->udpServerSocket->readFromSocketWithBlock(message, MAX_MESSAGE_SIZE);
    return new Message(message);  //in new thread
}

Message *Server::doOperation() {
    return nullptr;
}

void Server::serveRequest() {
    Message* msg = getRequest();
    std::cout << "Server Message Received: " << msg->getMessage() << std::endl;
    sendReply(msg);
    if(msg->getMessage() == "q") {
        std::cout << "Server found exit message...\nTerminating Process" << std::endl;
        exit(EXIT_SUCCESS);
    }
}

Server::~Server() {

}