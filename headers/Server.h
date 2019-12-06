#ifndef DISTRIBUTED_SYSTEMS_FALL19_SERVER_H
#define DISTRIBUTED_SYSTEMS_FALL19_SERVER_H

#include "UDPServerSocket.h"
#include "Message.h"

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

    template <typename T>
    Message * saveAndGetMessage(const T& messageStructure, Message::MessageType messageType, Message::OperationType operation);

    void serveRequest();

    ~Server();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_SERVER_H
