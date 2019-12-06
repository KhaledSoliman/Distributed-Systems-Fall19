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
    Peer(const std::string &hostname, int port, const std::string& directoryServerHostname, int directoryServerPort);
    void init(const std::string& username);
    std::string createThumbnail(const std::string& imagePath);
    Message handleRequest(Message message);
    UpdateLimitRequest updateLimit(std::string imageName, std::string username, int newLimit);
    AddViewerRequest addViewer(std::string imageName, std::string username);
    RemoveViewerRequest removeViewer(std::string imageName, std::string username);
    SearchRequest searchUser(std::string username);
    LoginRequest loginUser(const std::string& password);
    LogoutRequest logoutUser();
    RegisterRequest registerUser(std::string username, std::string password);
    AddImageRequest addImage(std::string imageName);
    DeleteImageRequest delImage(std::string imageName);
    FeedRequest feed(int imageNum, time_t lastSeenImage);
};


#endif //PROJ_PEER_H
