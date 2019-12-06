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
    std::string username;
public:
    Peer(const std::string &hostname, int port);
    void init();
    Message handleRequest(Message msg);
    UpdateLimitRequest updateLimit(std::string imageName, std::string username, int newLimit);
    AddViewerRequest addViewer(std::string imageName, std::string username);
    RemoveViewerRequest removeViewer(std::string imageName, std::string username);
    SearchRequest searchUser(std::string username);
    LoginRequest loginUser(std::string password);
    LogoutRequest logoutUser();
    RegisterRequest registerUser(std::string username, std::string password);
    AddImageRequest addImage(std::string imageName);
    DeleteImageRequest delImage(std::string imageName);
    FeedRequest feed(int imageNum, time_t lastSeenImage);
};


#endif //PROJ_PEER_H
