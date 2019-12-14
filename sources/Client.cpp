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

bool Client::initBroadcast(std::string _broadcastAddress, int _broadcastPort) {
    return this->udpSocket->initializeBroadcastClient(_broadcastAddress, _broadcastPort);
}

Message *Client::execute(Message *_message) {
    send(_message);
    return receiveWithBlock();
}

Client::~Client() {
}

bool Client::send(Message *_message) {
    std::string marshalled = _message->marshal();
    if (Message::verifyFragmentation(marshalled)) {
        std::vector<Message> msgs = _message->fragment(marshalled);

        int i = 0;
        do {
            std::string fragment_marshalled = msgs[i].marshal();
            this->udpSocket->writeToSocket(&fragment_marshalled[0], fragment_marshalled.length());
            bool isAcknowledged = awaitAck();
            if (isAcknowledged)
                i++;
        } while (i < msgs.size());

        return true;
    } else {
        this->udpSocket->writeToSocket(&marshalled[0], marshalled.length());
        return true;
    }
}


bool Client::awaitAck() {
    char *reply = static_cast<char *>(malloc(MAX_READ_MESSAGE_SIZE));
    this->udpSocket->readSocketWithTimeout(reply, MAX_READ_MESSAGE_SIZE, 0, 2000);
    std::cout << std::strlen(reply) << std::endl;
    std::cout << reply << std::endl;
    if (strcmp(reply, "Server Timed Out!") == 0) {
        std::cout << "Server timed out" << std::endl;
        return false;
    } else {
        auto *message = new Message(reply);
        if (message->getOperation() == Message::OperationType::ACK) {
            std::cout << "ACK RECEIVED" << std::endl;
            return true;
        } else {
            return awaitAck();
        }
    }
}

void Client::ack(const Message::RPC_ID &rpcId) {
    Ack acknowledgment = Ack();
    acknowledgment.setMessageId(rpcId.getMessageId());
    acknowledgment.setFragmentId(rpcId.getFragmentId());
    auto *request = this->saveAndGetMessage(acknowledgment, Message::MessageType ::Reply, Message::OperationType::ACK);
    this->send(request);
    std::cout << "ACK Transmitted" << std::endl;
}

Message::RPC_ID *Client::constructRPC() {
    return new Message::RPC_ID(this->udpSocket->getFilterAddress(), this->udpSocket->getMyPort());
}

bool Client::broadcast(Message *_message) {
    std::string marshalled = _message->marshal();
    this->udpSocket->writeBroadcastToSocket(&marshalled[0], marshalled.length());
    return true;
}

Message *Client::receiveWithBlock() {
    std::map<int, Message> msgs;
    char *reply = static_cast<char *>(malloc(MAX_READ_MESSAGE_SIZE));
    Message fragment;
    do {
        this->udpSocket->readFromSocketWithBlock(reply, MAX_READ_MESSAGE_SIZE);
        fragment = Message(reply);
        msgs[fragment.getRPCId().getFragmentId()] = fragment;
        if (fragment.getRPCId().isFragmented()) {
            std::cout << "Fragment ID: " << fragment.getRPCId().getFragmentId() << std::endl;
            std::cout << "Fragment Size: " << fragment.getMessage().length() << std::endl;
            ack(fragment.getRPCId());
        }
    } while (fragment.getRPCId().isFragmented() && (msgs.size() * MAX_MESSAGE_SIZE) < fragment.getMessageSize());

    if (msgs.size() > 1) {
        std::string stringHolder;
        for (auto &msg : msgs) {
            stringHolder += msg.second.getMessage();
        }
        return new Message(&stringHolder[0]);
    } else {
        return &msgs[0];
    }
}

Message *Client::receiveWithTimeout() {
    std::map<int, Message> msgs;
    char *reply = static_cast<char *>(malloc(MAX_READ_MESSAGE_SIZE));
    bool timedOut = false;
    Message fragment;
    do {
        this->udpSocket->readSocketWithTimeout(reply, MAX_READ_MESSAGE_SIZE, 0, 1000);
        if(strcmp(reply, "Server Timed Out!") == 0) {
            timedOut = true;
            break;
        }
        fragment = Message(reply);
        msgs[fragment.getRPCId().getFragmentId()] = fragment;
        if (fragment.getRPCId().isFragmented()) {
            std::cout << "Fragment ID: " << fragment.getRPCId().getFragmentId() << std::endl;
            std::cout << "Fragment Size: " << fragment.getMessage().length() << std::endl;
            ack(fragment.getRPCId());
        }
    } while (fragment.getRPCId().isFragmented() && (msgs.size() * MAX_MESSAGE_SIZE) < fragment.getMessageSize());
    if(timedOut) {
        return nullptr;
    }
    if (msgs.size() > 1) {
        std::string stringHolder;
        for (auto &msg : msgs) {
            stringHolder += msg.second.getMessage();
        }
        return new Message(&stringHolder[0]);
    } else {
        return &msgs[0];
    }
}



