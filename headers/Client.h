#ifndef DISTRIBUTED_SYSTEMS_FALL19_CLIENT_H
#define DISTRIBUTED_SYSTEMS_FALL19_CLIENT_H

#include "UDPClientSocket.h"
#include "Message.h"

class Client {
private:
    UDPClientSocket *udpSocket;
public:
    Client(char *_hostname, int _port);

    Message *execute(Message *_message);

    ~Client();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_CLIENT_H
