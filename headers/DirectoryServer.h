#ifndef PROJ_DIRECTORYSERVER_H
#define PROJ_DIRECTORYSERVER_H

#include <string>
#include <vector>
#include <map>
#include "MessageStructures.h"
#include "Message.h"
#include "Server.h"

#define TIMEOUT 30000
#define INTERVAL 1000
#define DEFAULT_LISTEN_PORT 3000

using namespace MessageStructures;
using namespace MessageStructures::Control;
using namespace MessageStructures::User;
using namespace MessageStructures::User::Authentication;

class DirectoryServer : Server {
private:
    struct Image {
    };
    struct User {
        std::string username;
        std::string address;
        std::string token;
        bool isAuthenticated;
        int portNum = DEFAULT_LISTEN_PORT;
        std::map<std::string, Image> images;
        time_t lastSeen;
    };
    std::vector<User> onlineUsers;
    std::vector<User> users;
    std::string hostname;
    int port;
    std::string databasePath;
    std::string directoryFile;
public:
    DirectoryServer();

    DirectoryServer(const std::string &hostname, int port, std::string databasePath, std::string directoryFile);

    void init();

    void loadDatabase();

    void saveDatabase();

    std::string generateAuthToken();

    bool authenticate(std::string username, std::string hashedPassword);

    void handleRequest(Message message);

    SearchReply searchUser(SearchRequest req);

    LoginReply loginUser(LoginRequest req);

    LogoutReply logoutUser(LogoutRequest req);

    RegisterReply registerUser(RegisterRequest req);

    AddImageReply addImage(AddImageRequest req);

    DeleteImageReply delImage(DeleteImageRequest req);

    FeedReply feed(FeedRequest req);

    ~DirectoryServer();
};


#endif //PROJ_DIRECTORYSERVER_H
