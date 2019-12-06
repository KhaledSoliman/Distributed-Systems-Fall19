#include "Peer.h"

Peer::Peer(const std::string &hostname, int port) : Server(hostname, port) {

}

void Peer::init() {

}

Message Peer::handleRequest(Message msg) {
    return Message();
}

UpdateLimitRequest Peer::updateLimit(std::string imageName, std::string username, int newLimit) {
    return UpdateLimitRequest();
}

AddViewerRequest Peer::addViewer(std::string imageName, std::string username) {
    return AddViewerRequest();
}

RemoveViewerRequest Peer::removeViewer(std::string imageName, std::string username) {
    return RemoveViewerRequest();
}

SearchRequest Peer::searchUser(std::string username) {
    return SearchRequest();
}

LoginRequest Peer::loginUser(std::string password) {
    return LoginRequest();
}

LogoutRequest Peer::logoutUser() {
    return LogoutRequest();
}

RegisterRequest Peer::registerUser(std::string username, std::string password) {
    return RegisterRequest();
}

AddImageRequest Peer::addImage(std::string imageName) {
    return AddImageRequest();
}

DeleteImageRequest Peer::delImage(std::string imageName) {
    return DeleteImageRequest();
}

FeedRequest Peer::feed(int imageNum, time_t lastSeenImage) {
    return FeedRequest();
}
