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

    bool listen(const std::string &_listen_hostname, int _listen_port);

    bool send(Message *_message);

    Message *receive();

    template<typename T>
    Message *
    saveAndGetMessage(const T &messageStructure, Message::MessageType messageType, Message::OperationType operation) {
        std::string serialized = save<T>(messageStructure);
        Message::RPC_ID rpc = Message::RPC_ID(boost::posix_time::second_clock::local_time(), this->hostname,
                                              this->port);
        return new Message(messageType, operation, serialized, serialized.length(), rpc);
    }

    void serveRequest();

    ~Server();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_SERVER_H
