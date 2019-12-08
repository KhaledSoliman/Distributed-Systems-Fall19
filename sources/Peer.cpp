#include "../headers/Peer.h"
#include "../headers/MessageStructures.h"

Peer::Peer(const std::string &hostname, int port, const std::string &directoryServerHostname, int directoryServerPort)
        : Server(hostname, port) {
    this->directoryServerHostname = directoryServerHostname;
    this->directoryServerPort = directoryServerPort;
}

void Peer::init(const std::string &username) {
    this->username = username;
    while (1) {
        //this->listen();
        //Message msg;
        //this ->handleRequest(msg);
    }
}

Message Peer::handleRequest(Message message) {
    switch (message.getMessageType()) {
        case Message::MessageType::Request:
            switch (message.getOperation()) {
                case Message::OperationType::ECHO:
                    this->saveAndGetMessage(load<Echo>(message.getMessage()), Message::MessageType::Reply,
                                            Message::OperationType::ECHO);
                    break;
                case Message::OperationType::DOWNLOAD_IMAGE:
                    this->saveAndGetMessage(load<DownloadImageRequest>(message.getMessage()),
                                            Message::MessageType::Reply, Message::OperationType::DOWNLOAD_IMAGE);
                    break;
                case Message::OperationType::VIEW_IMAGE:
                    this->saveAndGetMessage(load<AddViewerRequest>(message.getMessage()), Message::MessageType::Reply,
                                            Message::OperationType::ADD_VIEWER);
                    break;
                case Message::OperationType::ADD_VIEWER:
                    this->saveAndGetMessage(load<AddViewerRequest>(message.getMessage()), Message::MessageType::Reply,
                                            Message::OperationType::ADD_VIEWER);
                    break;
                case Message::OperationType::REMOVE_VIEWER:
                    this->saveAndGetMessage(load<AddViewerRequest>(message.getMessage()), Message::MessageType::Reply,
                                            Message::OperationType::ADD_VIEWER);
                    break;
                case Message::OperationType::UPDATE_VIEW_LIMIT:
                    this->saveAndGetMessage(load<AddViewerRequest>(message.getMessage()), Message::MessageType::Reply,
                                            Message::OperationType::ADD_VIEWER);
                    break;
                default:
                    break;
            }
            break;
        case Message::MessageType::Reply:
            switch (message.getOperation()) {
                case Message::OperationType::ECHO:
                    break;
                case Message::OperationType::ACK:
                    load<Ack>(message.getMessage());
                    break;
                case Message::OperationType::FEED:
                    break;
                case Message::OperationType::LOGIN:
                    break;
                case Message::OperationType::LOGOUT:
                    break;
                case Message::OperationType::REGISTER:
                    break;
                case Message::OperationType::ADD_IMAGE:
                    break;
                case Message::OperationType::DOWNLOAD_IMAGE:
                    break;
                case Message::OperationType::VIEW_IMAGE:
                    break;
                case Message::OperationType::DELETE_IMAGE:
                    break;
                case Message::OperationType::ADD_VIEWER:
                    break;
                case Message::OperationType::REMOVE_VIEWER:
                    break;
                case Message::OperationType::UPDATE_VIEW_LIMIT:
                    UpdateLimitReply updateLimitReply = load<UpdateLimitReply>(message.getMessage());
                    UpdateLimitReply.getnewlimit(); //made up
                    break;
            }
            break;
        default:
            break;
    }
}

UpdateLimitRequest Peer::updateLimit(const std::string &imageName, const std::string &username, int newLimit) {
    UpdateLimitRequest request = UpdateLimitRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setName(imageName);
    request.setTargetUsername(username);
    request.setNewLimit(newLimit);
    return request;
}

AddViewerRequest Peer::addViewer(const std::string &imageName, const std::string &username) {
    AddViewerRequest request = AddViewerRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageName(imageName);
    request.setViewerName(username);
    return request;
}

RemoveViewerRequest Peer::removeViewer(const std::string &imageName, const std::string &username) {
    RemoveViewerRequest request = RemoveViewerRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageName(imageName);
    request.setToRemove(username);
    return request;
}

SearchRequest Peer::searchUser(const std::string &username) {
    SearchRequest request = SearchRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setTargetUsername(username);
    return request;
}

LoginRequest Peer::loginUser(const std::string &password) {
    LoginRequest request = LoginRequest();
    request.setUserName(this->username);
    request.setHashedPassword(password);
    return request;
}

LogoutRequest Peer::logoutUser() {
    LogoutRequest request = LogoutRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    return request;
}

RegisterRequest Peer::registerUser(const std::string &username, const std::string &password) {
    RegisterRequest request = RegisterRequest();
    request.setUserName(username);
    request.setHashedPassword(password);
    return request;
}

AddImageRequest Peer::addImage(const std::string &imageName) {
    AddImageRequest request = AddImageRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageName(imageName);
    request.setThumbnail(this->createThumbnail(imageName));
    return request;
}

DeleteImageRequest Peer::delImage(const std::string &imageName) {
    DeleteImageRequest request = DeleteImageRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageName(imageName);
    return request;
}

FeedRequest Peer::feed(int imageNum, int lastIndex) {
    FeedRequest request = FeedRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageNum(imageNum);
    request.setLastIndex(lastIndex);
    return request;
}

std::string Peer::createThumbnail(const std::string &imagePath) {
    //TODO::
    return std::string();
}
