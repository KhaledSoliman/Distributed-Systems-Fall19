#ifndef DISTRIBUTED_SYSTEMS_FALL19_CLIENT_H
#define DISTRIBUTED_SYSTEMS_FALL19_CLIENT_H

#include "UDPClientSocket.h"
#include "Message.h"
#include "MessageStructures.h"

class Client {
private:
    UDPClientSocket *udpSocket;
public:
    Client();

    Client(std::string _hostname, int _port);

    bool connectToServer(const std::string &_hostname, int _port);

    bool initBroadcast(std::string _broadcastAddress, int _broadcastPort);

    Message::RPC_ID *constructRPC();

    Message *execute(Message *_message);

    bool send(Message *_message);

    bool broadcast(Message *_message);

    Message *receiveWithBlock();

    Message *receiveWithTimeout();

    bool awaitAck();

    void ack(const Message::RPC_ID &rpcId);

    template<typename T>
    Message *
    saveAndGetMessage(const T &messageStructure, Message::MessageType messageType, Message::OperationType operation) {
        std::string serialized = save<T>(messageStructure);
        return new Message(messageType, operation, serialized, serialized.length(), *this->constructRPC());
    }

    ~Client();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_CLIENT_H
