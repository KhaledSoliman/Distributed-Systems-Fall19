#ifndef DISTRIBUTED_SYSTEMS_FALL19_SERVER_H
#define DISTRIBUTED_SYSTEMS_FALL19_SERVER_H

#include "UDPServerSocket.h"
#include "Message.h"

class Server {
private:
    UDPServerSocket *udpServerSocket;

    Message *getRequest();

    Message *doOperation();

public:
    void sendReply(Message *_message);

    Server(char *_listen_hostname, int _listen_port);

    void serveRequest();

    ~Server();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_SERVER_H
