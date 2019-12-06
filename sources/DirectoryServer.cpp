#include "../headers/DirectoryServer.h"

DirectoryServer::DirectoryServer() {

}

DirectoryServer::DirectoryServer(const std::string &hostname, int port, std::string databasePath, std::string directoryFile) : Server(hostname, port) {
    this->databasePath = databasePath;
    this->directoryFile = directoryFile;
}

void DirectoryServer::init() {

}

void DirectoryServer::loadDatabase(std::string username, std::string password) {
    std::string databasePath = "users.txt";
    std::ofstream out;
    out.open(databasePath);
    out << username << password;
    out.close();
}

void DirectoryServer::saveDatabase(std::string username, std::string password) {
    bool here = false;
    std::string databasePath = "../users.txt";
    std::ofstream out;
    out.open(databasePath);
    out << username << password;
    for (User user :this->users && User password :this->users) {
        user.username = username;
        user.password = password;
    }
    out.close();
    if (!here)
    users.push_back(username);
    else
        cout << "Not valid" << endl;
}

std::string DirectoryServer::generateAuthToken() {
    return std::string();
}

bool DirectoryServer::authenticate(std::string username, std::string hashedPassword) {
    return false;
}

void DirectoryServer::handleRequest(Message message) {

}

SearchReply DirectoryServer::searchUser(SearchRequest req) {
    req.getUserName();
    for (User user :this->users) {
        user.username = req.getUserName();
    }
    SearchReply reply = SearchReply();
    reply.setFlag(false);
    return reply;
}

LoginReply DirectoryServer::loginUser(LoginRequest req) {
    req.getUserName();
    for (User user :this->users) {
        user.username = req.getUserName();
    }
    LoginReply reply = LoginReply();
           reply.setFlag(false);
    return reply;
}

LogoutReply DirectoryServer::logoutUser(LogoutRequest req) {
    req.getUserName();
    for (User user :this->users) {
        user.username = req.getUserName();
    }
    LogoutReply reply = LogoutReply();
     reply.setFlag(false);
    return reply;
}

RegisterReply DirectoryServer::registerUser(RegisterRequest req) {
    req.getUserName();
    req.getHashedPassword();
    for (User user :this->users && User password :this->users) {
        user.username = req.getUserName();
        user.password = req.getHashedPassword();
    }
    RegisterReply reply = RegisterReply();
    reply.setFlag(false);
    return reply;
}

AddImageReply DirectoryServer::addImage(AddImageRequest req) {
    req.getImageName();
    for (Image imageName :this->images) {
        Image.imageName = req.getImageName();
    }
    AddImageReply reply = AddImageReply();
    reply.setFlag(false);
    return reply;
}

DeleteImageReply DirectoryServer::delImage(DeleteImageRequest req) {
    req.getImageName();
    for (Image imageName :this->images) {
        Image.imageName = req.getImageName();
    }
    AddImageReply reply = AddImageReply();
    reply.delete;
}

FeedReply DirectoryServer::feed(FeedRequest req) {
    return FeedReply();
}

DirectoryServer::~DirectoryServer() {

}
