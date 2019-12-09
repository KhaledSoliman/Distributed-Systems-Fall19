#ifndef DISTRIBUTED_SYSTEMS_FALL19_CLIENT_H
#define DISTRIBUTED_SYSTEMS_FALL19_CLIENT_H

#include "UDPClientSocket.h"
#include "Message.h"

class Client {
private:
    UDPClientSocket *udpSocket;
public:
    Client();

    Client(std::string _hostname, int _port);

    bool connectToServer(const std::string &_hostname, int _port);

    Message *execute(Message *_message);

    bool send(Message* _message);

    Message* receive();

    ~Client();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_CLIENT_H
