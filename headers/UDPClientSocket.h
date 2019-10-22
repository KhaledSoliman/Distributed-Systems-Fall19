#ifndef DISTRIBUTED_SYSTEMS_FALL19_UDPCLIENTSOCKET_H
#define DISTRIBUTED_SYSTEMS_FALL19_UDPCLIENTSOCKET_H

#include "UDPSocket.h"

class UDPClientSocket : public UDPSocket {
public:
    UDPClientSocket();

    bool initializeClient(char *_peerAddr, int _peerPort);

    ~UDPClientSocket();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_UDPCLIENTSOCKET_H
