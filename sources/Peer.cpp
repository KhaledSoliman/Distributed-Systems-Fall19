#include "../headers/Peer.h"
#include "../headers/MessageStructures.h"

Peer::Peer(const std::string &hostname, int port, const std::string& directoryServerHostname, int directoryServerPort) : Server(hostname, port) {
    this->directoryServerHostname = directoryServerHostname;
    this->directoryServerPort = directoryServerPort;
}

void Peer::init(const std::string& username) {
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
                    this->saveAndGetMessage()
                    break;
                case Message::OperationType::ERROR:

                    break;
                case Message::OperationType::HELLO:

                    break;
                case Message::OperationType::DOWNLOAD_IMAGE:

                    break;
                case Message::OperationType::VIEW_IMAGE:
                    ViewImageRequest viewImageRequest = load<ViewImageRequest>(message.getMessage());
                    viewImageRequest.getImageName();
                    break;
                case Message::OperationType::ADD_VIEWER:
                    break;
                case Message::OperationType::REMOVE_VIEWER:
                    RemoveViewerRequest removeViewerRequest = load<RemoveViewerRequest>(message.getMessage());
                    removeViewerRequest.getToRemove();
                    break;
                case Message::OperationType::UPDATE_VIEW_LIMIT:
                    break;
                default:
                    break;
            }
        break;
        case Message::MessageType::Reply:
            switch (message.getOperation()) {
                case Message::OperationType::ECHO:
                     Echo =
                    break;
                case Message::OperationType::ACK:
                    Ack ack = load<Ack>(message.getMessage());
                    std::cout << ack.msg;
                    break;
                case Message::OperationType::ERROR:
                     Error error = load<Error>(message.getMessage());
                     std::cout << error.getMsg();
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

UpdateLimitRequest Peer::updateLimit(std::string imageName, std::string username, int newLimit) {
    UpdateLimitRequest request = UpdateLimitRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setName(imageName);
    request.setTargetUsername(username);
    request.setNewLimit(newLimit);
    return request;
}

AddViewerRequest Peer::addViewer(std::string imageName, std::string username) {
    AddViewerRequest request = AddViewerRequest();
    request.setImageName(this ->imageName);
    request.setViewerName(this -> username);
    return request;
}

RemoveViewerRequest Peer::removeViewer(const std::string& imageName,const std::string& username) {
    RemoveViewerRequest request = RemoveViewerRequest();
    request.setUserName(this -> username);
    request.setToken(this->token);
    request.setName(imageName);
    request.setToRemove(username);
    return request;
}

SearchRequest Peer::searchUser(std::string username) {
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
    request.setToken(token);
    return request;
}

RegisterRequest Peer::registerUser(std::string username, std::string password) {
    RegisterRequest request = RegisterRequest();
    request.setUserName(username);
    request.setHashedPassword(password);
    return request;
}

AddImageRequest Peer::addImage(std::string imageName) {
    AddImageRequest request = AddImageRequest();
    request.setImageName(imageName);
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setThumbnail(this->createThumbnail(imageName));
    return request;
}

DeleteImageRequest Peer::delImage(std::string imageName) {
    DeleteImageRequest request = DeleteImageRequest();
    request.setImageName(this -> imageName);
    return DeleteImageRequest();
}

FeedRequest Peer::feed(int imageNum, time_t lastSeenImage) {
    FeedRequest request = FeedRequest();
    request.setToken(this ->lastSeemImage);
    return FeedRequest();
}

std::string Peer::createThumbnail(const std::string &imagePath) {
    return std::string();
}
