#ifndef PROJ_PEER_H
#define PROJ_PEER_H

#include "Client.h"
#include "Server.h"
#include "MessageStructures.h"
#include "Cache.h"
#include <new>
#include <memory>
#include <boost/thread.hpp>

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
    int feedIndex;
    std::map<std::string, int> profileIndices;
    bool DoSOnline;
    bool helloProtocol;
    std::string listenHostname;
    int listenPort;
    std::string peerHostname;
    int peerPort;
    Cache myCache;


public:
    Peer(const std::string &listenHostname, int listenPort);

    Peer(const std::string &hostname, int port, const std::string &directoryServerHostname, int directoryServerPort);

    void init();

    std::string createThumbnail(const std::string &imagePath,  const std::string& imageName);

    void static cache( boost::shared_ptr<Peer> peer);

    void static DoSChecker( boost::shared_ptr<Peer> peer);

    void static listen( boost::shared_ptr<Peer> peer);

    void static handleRequest(Message* message, boost::shared_ptr<Peer> peer);

    void static handleChoice(boost::shared_ptr<Peer> peer);

    void static authHello(boost::shared_ptr<Peer> peer);

    bool discoverDirectoryService();

    bool connectToDoS();

    bool connectToPeer();

    Message* sendDoS(Message *message);

    Message* sendPeer(Message *message, const std::string& peerUsername);

    bool pingDoS();

    ShowOnlineRequest showOnline();

    UpdateLimitRequest updateLimit(const std::string &imageName, const std::string &username, int newLimit);

    ViewImageRequest viewImage(const std::string& username, const std::string &imageName, int viewNum);

    RemoveViewerRequest removeViewer(const std::string &imageName, const std::string &username);

    RemoveViewerRequest getRemainingViews(const std::string &imageName, const std::string &username);

    SearchRequest searchUser(const std::string &username);

    LoginRequest loginUser(const std::string &password);

    LogoutRequest logoutUser();

    RegisterRequest registerUser(const std::string &username, const std::string &password);

    AddImageRequest addImage(const std::string &imagePath, const std::string& imageName);

    DownloadImageRequest downloadImage(const std::string& imageName);

    DeleteImageRequest delImage(const std::string &imageName);

    FeedRequest feed(int imageNum);

    FeedProfileRequest feedProfile(const std::string& username, int imageNum);

    GetRequests getRequests();

    AddViewerRequest acceptRequest(const std::string& imageName, const std::string& viewerName, int viewNum);

    DenyViewerRequest denyRequest(const std::string& imageName, const std::string& viewerName, int viewNum);

    GetPendingRequests getPendingRequests();

    DownloadImageReply sendImage(DownloadImageRequest req);

    UpdateLimitReply updateLimit(UpdateLimitRequest req);

    RemoveViewerReply removeViewer(RemoveViewerRequest req);

    GetRemainingViewsReply getRemainingViews(GetRemainingViewsRequest req);

    int getMyRemainingViews(const std::string& imageName, const std::string& username);

    bool isDoSOnline() const;

    void setDoSOnline(bool doSOnline);

    const std::string &getDirectoryServerHostname() const;

    void setDirectoryServerHostname(const std::string &directoryServerHostname);

    int getDirectoryServerPort() const;

    void setDirectoryServerPort(int directoryServerPort);

    const std::string &getUsername() const;

    void setUsername(const std::string &username);

    const std::string &getToken() const;

    void setToken(const std::string &token);

    bool isAuthenticated() const;

    void setAuthenticated(bool authenticated);

    int getFeedIndex() const;

    void setFeedIndex(int feedIndex);

    bool isHelloProtocol() const;

    void setHelloProtocol(bool helloProtocol);

    const std::string &getListenHostname() const;

    void setListenHostname(const std::string &listenHostname);

    int getListenPort() const;

    void setListenPort(int listenPort);

    const std::string &getPeerHostname() const;

    void setPeerHostname(const std::string &peerHostname);

    int getPeerPort() const;

    void setPeerPort(int peerPort);

    void addProfileIndex(const std::string& username, int index);

    const std::map<std::string, int> &getProfileIndices() const;

    void setProfileIndices(const std::map<std::string, int> &profileIndices);

    const Cache &getMyCache() const;

    void setMyCache(const Cache &myCache);

};


#endif //PROJ_PEER_H
