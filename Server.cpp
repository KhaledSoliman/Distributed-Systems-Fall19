#include "Message.h"
#include "Server.h"

void Server::sendReply(Message *_message) {

}

Message *Server::getRequest() {
    return nullptr;
}

Message *Server::doOperation() {
    return nullptr;
}

Server::Server(char *_listen_hostname, int _listen_port) {

}

void Server::serveRequest() {

}

Server::~Server() {

}
