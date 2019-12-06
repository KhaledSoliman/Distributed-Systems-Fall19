#ifndef PROJ_DIRECTORYSERVER_H
#define PROJ_DIRECTORYSERVER_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "MessageStructures.h"
#include "Message.h"
#include "Server.h"

#define TIMEOUT 30000
#define INTERVAL 1000
#define DEFAULT_LISTEN_PORT 3000
#define DATABASE_DIR "/Users/snappy/CLionProjects/Distributed-Systems-Fall19/database"
#define THUMBNAILS_DIR "thumbnails/"
#define DIRECTORY_FILE "directory.txt"
#define USER_FILE "users.txt"

using namespace MessageStructures;
using namespace MessageStructures::Control;
using namespace MessageStructures::User;
using namespace MessageStructures::User::Authentication;

class DirectoryServer : Server {
private:
    struct Image {
        std::string image;
        std::string imageThumbnail;
        std::string imageName;
    };

    struct User {
        std::string username;
        std::string password;
        std::string address;
        std::string token;
        bool authenticated;
        int portNum = DEFAULT_LISTEN_PORT;
        std::vector<std::string> images;
        time_t lastSeen;

        const std::string &getUsername() const;

        void setUsername(const std::string &username);

        const std::string &getPassword() const;

        void setPassword(const std::string &password);

        const std::string &getAddress() const;

        void setAddress(const std::string &address);

        const std::string &getToken() const;

        void setToken(const std::string &token);

        bool isAuthenticated() const;

        void setAuthenticated(bool isAuthenticated);

        int getPortNum() const;

        void setPortNum(int portNum);

        const std::vector<std::string> &getImages() const;

        void setImages(const std::vector<std::string> &images);

        time_t getLastSeen() const;

        void setLastSeen(time_t lastSeen);

        bool imageExists(const std::string& imageName);

        void addImage(const std::string& imageName);
    };

    std::unordered_map<std::string, User> users;
    std::unordered_map<std::string, User> onlineUsers;
    std::string hostname;
    int port;
    std::string databasePath;
    std::string directoryFile;
    std::string usersFile;
public:
    explicit DirectoryServer(const std::string &hostname);

    DirectoryServer(const std::string &hostname, int port, const std::string &databasePath,
                    const std::string &directoryFile, const std::string &usersFile);

    void init();

    void listen();

    void loadDatabase();

    void saveDatabase();

    Ack handleHello(Hello req);

    bool userExists(const std::string& username);

    static std::string generateAuthToken();

    bool authorize(const std::string& username, const std::string &token);

    bool authenticate(const std::string &username, const std::string &hashedPassword);

    void handleRequest(Message message);

    SearchReply searchUser(const SearchRequest& req);

    LoginReply loginUser(const LoginRequest& req);

    LogoutReply logoutUser(const LogoutRequest& req);

    RegisterReply registerUser(const RegisterRequest& req);

    AddImageReply addImage(const AddImageRequest& req);

    DeleteImageReply delImage(const DeleteImageRequest& req);

    FeedReply feed(const FeedRequest& req);

    ~DirectoryServer();
};


#endif //PROJ_DIRECTORYSERVER_H
