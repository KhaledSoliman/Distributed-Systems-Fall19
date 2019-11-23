#ifndef DISTRIBUTED_SYSTEMS_FALL19_UDPSOCKET_H
#define DISTRIBUTED_SYSTEMS_FALL19_UDPSOCKET_H

#include <netinet/in.h>
#include <string>

class UDPSocket {
protected:
    int sock;
    sockaddr_in myAddr;
    sockaddr_in peerAddr;
    std::string myAddress;
    std::string peerAddress;
    int myPort;
    int peerPort;
    bool enabled;
    pthread_mutex_t mutex;
public:
    UDPSocket();

    void setFilterAddress(std::string _filterAddress);

    std::string getFilterAddress();

    bool initializeServer(std::string _myAddr, int _myPort);

    bool initializeClient(std::string _peerAddr, int _peerPort);

    int writeToSocket(std::string buffer, int maxBytes);

    int writeToSocketAndWait(std::string buffer, int maxBytes, int microSec);

    int readFromSocketWithNoBlock(std::string buffer, int maxBytes);

    int readFromSocketWithTimeout(std::string buffer, int maxBytes, int timeoutSec, int timeoutMilli);

    int readFromSocketWithBlock(std::string buffer, int maxBytes);

    int readSocketWithNoBlock(std::string buffer, int maxBytes);

    int readSocketWithTimeout(std::string buffer, int maxBytes, int timeoutSec, int timeoutMilli);

    int readSocketWithBlock(std::string buffer, int maxBytes);

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
