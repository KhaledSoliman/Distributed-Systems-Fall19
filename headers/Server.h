#ifndef DISTRIBUTED_SYSTEMS_FALL19_SERVER_H
#define DISTRIBUTED_SYSTEMS_FALL19_SERVER_H

#include "UDPServerSocket.h"
#include "Message.h"
#include "MessageStructures.h"

class Server {
private:
    UDPServerSocket *udpServerSocket;

    Message *doOperation();

    std::string hostname;
    int port;

protected:
    Message *getRequest();

public:
    void sendReply(Message *_message);

    Server();

    Server(const std::string &_listen_hostname, int _listen_port);

    bool initBroadcast(int _broadcastPort);

    bool send(Message *_message);

    Message *listenToBroadcasts();

    Message *receive();

    Message::RPC_ID *constructRPC();

    template<typename T>
    Message *
    saveAndGetMessage(const T &messageStructure, Message::MessageType messageType, Message::OperationType operation) {
        std::string serialized = save<T>(messageStructure);
        return new Message(messageType, operation, serialized, serialized.length(), *(this->constructRPC()));
    }

    void serveRequest();

    bool awaitAck();

    void ack(const Message::RPC_ID &rpcId);

    ~Server();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_SERVER_H
