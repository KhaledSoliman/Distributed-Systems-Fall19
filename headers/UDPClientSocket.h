#ifndef DISTRIBUTED_SYSTEMS_FALL19_UDPCLIENTSOCKET_H
#define DISTRIBUTED_SYSTEMS_FALL19_UDPCLIENTSOCKET_H

#include "UDPSocket.h"
#include <string>

class UDPClientSocket : public UDPSocket {
public:
    UDPClientSocket();

    bool initializeClient(std::string _peerAddr, int _peerPort);

    bool initializeBroadcastClient(std::string _peerAddr, int _peerPort);

    ~UDPClientSocket();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_UDPCLIENTSOCKET_H
