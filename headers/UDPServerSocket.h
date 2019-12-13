#ifndef DISTRIBUTED_SYSTEMS_FALL19_UDPSERVERSOCKET_H
#define DISTRIBUTED_SYSTEMS_FALL19_UDPSERVERSOCKET_H

#include <string>
#include "UDPSocket.h"

class UDPServerSocket : public UDPSocket {
public:
    UDPServerSocket();

    bool initializeServer(std::string _myAddr, int _myPort);

    bool initializeBroadCastServer(int _broadcastPort);

    ~UDPServerSocket();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_UDPSERVERSOCKET_H
