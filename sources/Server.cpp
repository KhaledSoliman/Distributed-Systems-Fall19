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

bool Server::initBroadcast(int _broadcastPort) {
    return this->udpServerSocket->initializeBroadcastServer(_broadcastPort);
}

void Server::sendReply(Message *_message) {
    this->udpServerSocket->writeToSocket(&_message->marshal()[0], _message->marshal().length());
}

Message *Server::getRequest() {
    char *message = static_cast<char *>(malloc(MAX_MESSAGE_SIZE));
    this->udpServerSocket->readFromSocketWithBlock(message, MAX_MESSAGE_SIZE);
    return new Message(message);  //in new thread
}

Message *Server::doOperation() {
    return nullptr;
}

void Server::serveRequest() {
    Message *msg = receive();
    std::ofstream out;
    out.open("server.jpg");
    out << msg->getMessage();
    out.close();
    std::cout << "Server Message Received: " << msg->getMessage() << std::endl;
    send(msg);
    if (msg->getMessage() == "q") {
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
            std::string fragment_marshalled = msgs[i]->marshal();
            this->udpServerSocket->writeToSocket(&fragment_marshalled[0], fragment_marshalled.length());
            bool isAcknowledged = awaitAck();
            if (isAcknowledged)
                i++;
        } while (i < msgs.size());

        return true;
    } else {
        this->udpServerSocket->writeToSocket(&marshalled[0], marshalled.length());
        return true;
    }
}

Message *Server::listenToBroadcasts() {
    char *broadcast = static_cast<char *>(malloc(MAX_READ_MESSAGE_SIZE));
    this->udpServerSocket->readSocketBroadcast(broadcast, MAX_READ_MESSAGE_SIZE);
    return new Message(broadcast);
}

Message *Server::receive() {
    std::map<int, Message *> msgs;
    Message *fragment = nullptr;
    char *reply = static_cast<char *>(malloc(MAX_READ_MESSAGE_SIZE));


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
        std::string stringHolder;
        for (auto &msg : msgs) {
            stringHolder += msg.second->getMessage();
        }
        return new Message(&stringHolder[0]);
    } else {
        return msgs[0];
    }
}

bool Server::awaitAck() {
    char *reply = static_cast<char *>(malloc(MAX_READ_MESSAGE_SIZE));
    this->udpServerSocket->readSocketWithTimeout(reply, MAX_READ_MESSAGE_SIZE, 0, 2000);
    if (strcmp(reply, "Server Timed Out!") == 0) {
        std::cout << "Server timed out" << std::endl;
        return false;
    } else {
        Ack message = load<Ack>(Message(reply).getMessage());
        std::cout << message.getFragmentId() << std::endl;
        std::cout << "ACK RECEIVED" << std::endl;
        return true;
    }
}

void Server::ack(const Message::RPC_ID &rpcId) {
    Ack acknowledgment = Ack();
    acknowledgment.setMessageId(rpcId.getMessageId());
    acknowledgment.setFragmentId(rpcId.getFragmentId());
    auto *request = this->saveAndGetMessage(acknowledgment, Message::MessageType ::Reply, Message::OperationType::ACK);
    this->send(request);
    std::cout << "ACK Transmitted" << std::endl;
}

Message::RPC_ID *Server::constructRPC() {
    return new Message::RPC_ID(this->hostname, this->port);
}

Server::~Server() {

}
