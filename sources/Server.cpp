#include <cstring>
#include <iostream>
#include "../headers/Message.h"
#include "../headers/Server.h"
#include "../headers/MessageStructures.h"

Server::Server() {
    this->udpServerSocket = new UDPServerSocket();
}

Server::Server(const std::string &_listen_hostname, int _listen_port) {
    this->udpServerSocket = new UDPServerSocket();
    this->udpServerSocket->initializeServer(_listen_hostname, _listen_port);
    this->hostname = _listen_hostname;
    this->port = _listen_port;
}

bool Server::listen(const std::string &_listen_hostname, int _listen_port) {
    return this->udpServerSocket->initializeServer(_listen_hostname, _listen_port);
}

template <typename T>
Message * Server::saveAndGetMessage(const T& messageStructure, Message::MessageType messageType, Message::OperationType operation) {
    std::string serialized = save<T>(messageStructure);
    Message::RPC_ID rpc = Message::RPC_ID(boost::posix_time::second_clock::local_time(), this->hostname, this->port);
    return new Message(messageType, operation, serialized, serialized.length(), rpc);
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