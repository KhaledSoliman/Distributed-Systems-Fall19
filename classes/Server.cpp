#include <cstring>
#include <iostream>
#include "../headers/Message.h"
#include "../headers/Server.h"

Server::Server(char *_listen_hostname, int _listen_port) {
    this->udpServerSocket = new UDPServerSocket();
    this->udpServerSocket->initializeServer(_listen_hostname, _listen_port);
}

void Server::sendReply(Message *_message) {
    this->udpServerSocket->writeToSocket(_message->getMessage(), _message->getMessageSize());
}

Message *Server::getRequest() {
    char *request = static_cast<char *>(malloc(1000));
    this->udpServerSocket->readFromSocketWithBlock(request, 1000);
    return new Message(0,request,strlen(request),1);  ;
}

Message *Server::doOperation() {
    return nullptr;
}

void Server::serveRequest() {
    Message* msg = getRequest();
    std::cout << "Server Message Received: " << msg->getMessage() << std::endl;
    sendReply(msg);
    if(strncmp(msg->getMessage(), "q",1) == 0) {
        std::cout << "Server found exit message...\nTerminating Process" << std::endl;
        exit(EXIT_SUCCESS);
    }
}

Server::~Server() {

}
