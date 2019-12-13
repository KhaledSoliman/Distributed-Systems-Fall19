#ifndef PROJ_PEER_H
#define PROJ_PEER_H


#include "Client.h"
#include "Server.h"
#include "MessageStructures.h"

using namespace MessageStructures;
using namespace MessageStructures::Control;
using namespace MessageStructures::User;
using namespace MessageStructures::User::Authentication;

class Peer : Server, Client {
private:
    std::string directoryServerHostname;
    int directoryServerPort;
    std::string username;
    std::string token;
    bool authenticated;
    bool DoSOnline;
public:
    Peer(const std::string &listenHostname, int listenPort);

    Peer(const std::string &hostname, int port, const std::string &directoryServerHostname, int directoryServerPort);

    void init();

    std::string createThumbnail(const std::string &imagePath);

    void static DoSChecker(Peer& peer);

    void static listen(Peer& peer);

    void static handleRequest(Message* message, Peer& peer);

    void static handleChoice(Peer& peer);

    bool discoverDirectoryService();

    bool connectToDoS();

    bool connectToPeer(const std::string& peerHostname, int peerPort);

    bool pingDoS();

    UpdateLimitRequest updateLimit(const std::string &imageName, const std::string &username, int newLimit);

    AddViewerRequest addViewer(const std::string &imageName, const std::string &username);

    RemoveViewerRequest removeViewer(const std::string &imageName, const std::string &username);

    SearchRequest searchUser(const std::string &username);

    LoginRequest loginUser(const std::string &password);

    LogoutRequest logoutUser();

    RegisterRequest registerUser(const std::string &username, const std::string &password);

    AddImageRequest addImage(const std::string &imageName);

    DeleteImageRequest delImage(const std::string &imageName);

    FeedRequest feed(int imageNum, int lastSeenImage);

    bool isDoSOnline() const;

    void setDoSOnline(bool doSOnline);
};


#endif //PROJ_PEER_H
