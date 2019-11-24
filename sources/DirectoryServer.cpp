#include "../headers/DirectoryServer.h"

DirectoryServer::DirectoryServer() {

}

DirectoryServer::DirectoryServer(const std::string &hostname, int port, std::string databasePath, std::string directoryFile) : Server(hostname, port) {
    this->databasePath = databasePath;
    this->directoryFile = directoryFile;
}

void DirectoryServer::init() {

}

void DirectoryServer::loadDatabase() {

}

void DirectoryServer::saveDatabase() {

}

std::string DirectoryServer::generateAuthToken() {
    return std::__cxx11::string();
}

bool DirectoryServer::authenticate(std::string username, std::string hashedPassword) {
    return false;
}

void DirectoryServer::handleRequest(Message message) {

}

SearchReply DirectoryServer::searchUser(SearchRequest req) {
    return SearchReply();
}

LoginReply DirectoryServer::loginUser(LoginRequest req) {
    return LoginReply();
}

LogoutReply DirectoryServer::logoutUser(LogoutRequest req) {
    return LogoutReply();
}

RegisterReply DirectoryServer::registerUser(RegisterRequest req) {
    return RegisterReply();
}

AddImageReply DirectoryServer::addImage(AddImageRequest req) {
    return AddImageReply();
}

DeleteImageReply DirectoryServer::delImage(DeleteImageRequest req) {
    return DeleteImageReply();
}

FeedReply DirectoryServer::feed(FeedRequest req) {
    return FeedReply();
}

DirectoryServer::~DirectoryServer() {

}
