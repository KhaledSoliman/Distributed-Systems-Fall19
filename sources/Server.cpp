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
    this->hostname = _listen_hostname;
    this->port = _listen_port;
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
    Message* msg = receive();
    std::ofstream out;
    out.open("server.jpg");
    out << msg->getMessage();
    out.close();
    std::cout << "Server Message Received: " << msg->getMessage() << std::endl;
    send(msg);
    if(msg->getMessage() == "q") {
        std::cout << "Server found exit message...\nTerminating Process" << std::endl;
        exit(EXIT_SUCCESS);
    }
}

bool Server::send(Message *_message) {
    std::string marshalled = _message->marshal();
    if (Message::verifyFragmentation(marshalled)) {
        std::vector<Message *> msgs = _message->fragment(marshalled);
        int i = 0;

        do {
            std::cout << msgs[i]->getMessage().length() << std::endl;
            std::string fragment_marshalled = msgs[i]->marshal();
            this->udpServerSocket->writeToSocket(&fragment_marshalled[0], fragment_marshalled.size());
            bool isAcknowledged = awaitAck();
            if(isAcknowledged)
                i++;
        } while (i < msgs.size());

        return true;
    } else {
        this->udpServerSocket->writeToSocket(&marshalled[0], marshalled.size());
        return true;
    }
}

Message *Server::receive() {
    std::map<int, Message *> msgs;
    char *reply = static_cast<char *>(malloc(MAX_READ_MESSAGE_SIZE));
    Message *fragment = nullptr;

    do {
        this->udpServerSocket->readFromSocketWithBlock(reply, MAX_READ_MESSAGE_SIZE);
        fragment = new Message(reply);
        msgs[fragment->getRPCId().getFragmentId()] = fragment;
        if (fragment->getRPCId().isFragmented()) {
            std::cout << "Fragment ID: " << fragment->getRPCId().getFragmentId() << std::endl;
            std::cout << "Fragment Size: " << fragment->getMessage().length() << std::endl;
            ack(fragment->getRPCId());
        }
    } while (fragment->getRPCId().isFragmented() && (msgs.size() * MAX_MESSAGE_SIZE) < fragment->getMessageSize());

    if (msgs.size() > 1) {
        std::string reply;
        for (auto &msg : msgs) {
            reply += msg.second->getMessage();
        }
        return new Message(&reply[0]);
    } else {
        return msgs[0];
    }
}

bool Server::awaitAck() {
    char *reply = static_cast<char *>(malloc(MAX_READ_MESSAGE_SIZE));
    Message *message = nullptr;
    this->udpServerSocket->readSocketWithTimeout(reply, MAX_READ_MESSAGE_SIZE, 0, 500);
    message = new Message(reply);
    if (strcmp(reply, "Server Timed Out!") == 0) {
        std::cout << "Server timed out" << std::endl;
        return false;
    } else if (message->getOperation() == Message::OperationType::ACK) {
        std::cout << "ACK RECEIVED" << std::endl;
        return true;
    }
}

void Server::ack(const Message::RPC_ID &rpcId) {
    Message::RPC_ID rpc = Message::RPC_ID(rpcId.time, rpcId.address, rpcId.portNumber);
    rpc.setMessageId(1);
    auto *request = new Message(Message::MessageType::Reply, Message::OperationType::ACK, "OK", 2, rpc);
    std::string marshalled = request->marshal();
    this->udpServerSocket->writeToSocket(&marshalled[0], marshalled.size());
    std::cout << "ACK Transmitted" << std::endl;
}


Server::~Server() {

}
