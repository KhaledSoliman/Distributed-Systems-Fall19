#ifndef DISTRIBUTED_SYSTEMS_FALL19_UDPSOCKET_H
#define DISTRIBUTED_SYSTEMS_FALL19_UDPSOCKET_H

#include <netinet/in.h>

#define BROADCAST_ADDRESS "255.255.255.255"
#define BROADCAST_PORT 3001

class UDPSocket {
protected:
    int sock;
    int broadcastSock;
    sockaddr_in myAddr;
    sockaddr_in peerAddr;
    char *myAddress;
    char *peerAddress;
    int myPort;
    int peerPort;
    bool enabled;
    pthread_mutex_t mutex;
public:
    UDPSocket();

    void setFilterAddress(char *_filterAddress);

    char *getFilterAddress();

    bool initializeBroadcastServer(int broadcastPort);

    bool initializeBroadcastClient(char *broadcastIP, unsigned short broadcastPort);

    bool initializeServer(char *_myAddr, int _myPort);

    bool initializeClientSocket();

    bool initializeClientPeer(char *_peerAddr, int _peerPort);

    int writeBroadcastToSocket(char *buffer, int maxBytes);

    int writeToSocket(char *buffer, int maxBytes);

    int writeToSocketAndWait(char *buffer, int maxBytes, int microSec);

    int readSocketBroadcast(char* buffer, int maxBytes);

    int readFromSocketWithNoBlock(char *buffer, int maxBytes);

    int readFromSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec, int timeoutMilli);

    int readFromSocketWithBlock(char *buffer, int maxBytes);

    int readSocketWithNoBlock(char *buffer, int maxBytes);

    int readSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec, int timeoutMilli);

    int readSocketWithBlock(char *buffer, int maxBytes);

    int getMyPort();

    int getPeerPort();

    void enable();

    void disable();

    bool isEnabled();

    void lock();

    void unlock();

    int getSocketHandler();

    ~UDPSocket();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_UDPSOCKET_H
