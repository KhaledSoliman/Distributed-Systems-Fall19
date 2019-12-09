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
public:
    Peer(const std::string &hostname, int port, const std::string &directoryServerHostname, int directoryServerPort);

    void init(const std::string &username);

    std::string createThumbnail(const std::string &imagePath);

    void handleRequest(Message message);

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
};


#endif //PROJ_PEER_H
