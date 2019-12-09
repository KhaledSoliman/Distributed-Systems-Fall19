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
    send(_message);
    return receive();
}

Client::~Client() {
}

bool Client::send(Message *_message)  {
    std::string marshalled = _message->marshal();
    if(Message::verifyFragmentation(marshalled)) {
        std::vector<Message *> msgs = _message->fragment(marshalled);
        Message* reply;
        int i = 0;
        do {
            std::cout << msgs[i]->getMessage().length() << std::endl;
            std::string fragment_marshalled = msgs[i]->marshal();
            this->udpSocket->writeToSocket(&fragment_marshalled[0], fragment_marshalled.size());
            reply = receive();
            if(reply->getOperation() == Message::OperationType::ACK) {
                std::cout << "ACK RECEIVED" << std::endl;
                          i++;
            }
        } while(i < msgs.size());
        return true;
    } else {
        this->udpSocket->writeToSocket(&marshalled[0], marshalled.size());
        return true;
    }
}

Message* Client::receive() {
    std::map<int, Message *> msgs;
    char *reply = static_cast<char *>(malloc(MAX_READ_MESSAGE_SIZE));
    Message *fragment = nullptr;
    Message* request = nullptr;
    do {
        this->udpSocket->readFromSocketWithBlock(reply, MAX_READ_MESSAGE_SIZE);
        fragment = new Message(reply);
        msgs[fragment->getRPCId().getFragmentId()] = fragment;
        if(fragment->getRPCId().isFragmented()) {
            std::cout << "Fragment ID: " << fragment->getRPCId().getFragmentId() << std::endl;
            std::cout << "Fragment Size: " << fragment->getMessage().length() << std::endl;
            Message::RPC_ID rpcId = fragment->getRPCId();
            Message::RPC_ID rpc = Message::RPC_ID(rpcId.time, rpcId.address, rpcId.portNumber);
            rpc.setMessageId(1);
            request = new Message(Message::MessageType::Reply, Message::OperationType::ACK, "OK", 2, rpc);
            send(request);
            std:: cout<< "ACK Transmitted" << std::endl;
        }
    } while(fragment->getRPCId().isFragmented() && (msgs.size()*MAX_MESSAGE_SIZE) < fragment->getMessageSize());
    if(msgs.size() > 1) {
        std::string reply;
        for(auto & msg : msgs) {
            reply += msg.second->getMessage();
        }
        return new Message(&reply[0]);
    } else {
        return msgs[0];
    }
}




