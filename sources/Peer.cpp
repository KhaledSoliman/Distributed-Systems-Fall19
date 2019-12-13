#include "../headers/Peer.h"
#include "../headers/MessageStructures.h"

#include <boost/thread.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
Peer::Peer(const std::string &hostname, int port, const std::string &directoryServerHostname, int directoryServerPort)
        : Server(hostname, port) {
    this->authenticated = false;
    this->directoryServerHostname = directoryServerHostname;
    this->directoryServerPort = directoryServerPort;
}

Peer::Peer(const std::string &listenHostname, int listenPort) : Server(listenHostname, listenPort) {
    authenticated = false;
    DoSOnline = false;
}

bool Peer::connectToDoS() {
    this->connectToServer(directoryServerHostname, directoryServerPort);
}

bool Peer::connectToPeer(const std::string& peerHostname, int peerPort) {
    this->connectToServer(peerHostname, peerPort);
}

bool Peer::discoverDirectoryService() {
    Hello hello = Hello();
    hello.setMessage("DoS");
    std::string broadcastIP = "255.255.255.255";
    int broadcastPort = 3001;
    this->Client::initBroadcast(broadcastIP, broadcastPort);
    Message* message = this->Client::saveAndGetMessage(hello, Message::MessageType::Request, Message::OperationType::HELLO);
    if(this->Client::broadcast(message)) {
        Message* reply = this->Client::receiveWithTimeout();
        if(reply == nullptr)
            return false;
        auto helloReply = load<Hello>(reply->getMessage());
        if(helloReply.getMessage() == "ME") {
            this->DoSOnline = true;
            this->directoryServerHostname = reply->getRPCId().getAddress();
            this->directoryServerPort = reply->getRPCId().getPortNumber();
            return true;
        }
    }
    return false;
}

void Peer::DoSChecker(Peer &peer) {
    while(true) {
        while(!peer.discoverDirectoryService()) {
            std::cout << "Discovering DoS..." << std::endl;
            boost::this_thread::sleep(boost::posix_time::milliseconds(750));
        }
        int pingMissCounter = 0;
        while(pingMissCounter < 3) {
            if(!peer.pingDoS()) {
                peer.setDoSOnline(false);
                pingMissCounter +=  1;
                std::cout << "DoS went offline please hold while we try to connect again..." << std::endl;
            } else {
                peer.setDoSOnline(true);
            }
            boost::this_thread::sleep(boost::posix_time::milliseconds(2500));
        }
    }
}

bool Peer::pingDoS() {
    std::cout << "Pinging directory of server..." << std::endl;
    Echo echo = Echo();
    echo.setMsg("PING");
    this->connectToDoS();
    Message* message = this->Client::saveAndGetMessage(echo, Message::MessageType::Request, Message::OperationType::ECHO);
    if(this->Client::send(message)) {
        Message* reply = this->Client::receiveWithTimeout();
        if(reply == nullptr)
            return false;
        Echo echoReply = load<Echo>(reply->getMessage());
        if(echoReply.getMsg() == echoReply.getMsg())
            return true;
    }
    return false;
}

void Peer::init() {
    boost::thread DoSThread(&Peer::DoSChecker, *this);
    boost::thread serverThread(&Peer::listen, *this);
    boost::thread clientThread(&Peer::handleChoice, *this);
}


void  Peer::listen(Peer &peer) {
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        Message* message = peer.Server::receive();
        boost::thread handleRequest(&Peer::handleRequest, message, peer);
    }
    #pragma clang diagnostic pop
}

void Peer::handleRequest(Message* message, Peer& peer) {
    switch (message->getMessageType()) {
        case Message::MessageType::Request:
            switch (message->getOperation()) {
                case Message::OperationType::ECHO:
                    peer.Server::saveAndGetMessage(load<Echo>(message->getMessage()), Message::MessageType::Reply,
                                            Message::OperationType::ECHO);
                    break;
                case Message::OperationType::DOWNLOAD_IMAGE:
                    peer.Server::saveAndGetMessage(load<Echo>(message->getMessage()),
                                            Message::MessageType::Reply, Message::OperationType::DOWNLOAD_IMAGE);
                    break;
                case Message::OperationType::VIEW_IMAGE:
                    peer.Server::saveAndGetMessage(load<AddViewerRequest>(message->getMessage()), Message::MessageType::Reply,
                                            Message::OperationType::ADD_VIEWER);
                    break;
                case Message::OperationType::ADD_VIEWER:
                    peer.Server::saveAndGetMessage(load<AddViewerRequest>(message->getMessage()), Message::MessageType::Reply,
                                            Message::OperationType::ADD_VIEWER);
                    break;
                case Message::OperationType::REMOVE_VIEWER:
                    peer.Server::saveAndGetMessage(load<AddViewerRequest>(message->getMessage()), Message::MessageType::Reply,
                                            Message::OperationType::ADD_VIEWER);
                    break;
                case Message::OperationType::UPDATE_VIEW_LIMIT:
                    peer.Server::saveAndGetMessage(load<AddViewerRequest>(message->getMessage()), Message::MessageType::Reply,
                                            Message::OperationType::ADD_VIEWER);
                    break;
                default:
                    break;
            }
            break;
        case Message::MessageType::Reply:
            switch (message->getOperation()) {
                case Message::OperationType::ECHO:
                    break;
                case Message::OperationType::ACK:
                    load<Echo>(message->getMessage());
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
                    UpdateLimitReply updateLimitReply = load<UpdateLimitReply>(message->getMessage());
                    break;
            }
            break;
        default:
            break;
    }
}

void Peer::handleChoice(Peer& peer) {
    int peerChoice;
    bool incorrectChoice;
    enum peerChoices {
        Register = 1,
        Login,
        Logout,
        Feed,
        AddImage,
        Search,
        RequestView,
        DownloadImage,
        GetRequests
    };
    do {
        incorrectChoice = false;
        std::cin >> peerChoice;
        if(!peer.authenticated) {
            std::cout << "Please choose what you want to do as a peer?" << std::endl
                      << "List of choices:" << std::endl
                      << "1) Register with directory server." << std::endl
                      << "2) Already have an account then login with directory server." << std::endl;
            std::string username, password, confirmPassword;
            switch(peerChoice) {
                case peerChoices::Register:
                    std::cout << "Please input your username" << std::endl;
                    std::cin >> username;
                    std::cout << "Please input your password" << std::endl;
                    std::cin >> password;
                    std::cout << "Please confirm your password" << std::endl;
                    std::cin >> confirmPassword;
                    if(confirmPassword == password)
                        peer.registerUser(username, password);
                    else
                    std::cout << "Passwords do not match" << std::endl;
                    break;
                case peerChoices::Login:
                    std::cout << "Please input your username" << std::endl;
                    std::cin >> username;
                    std::cout << "Please input your password" << std::endl;
                    std::cin >> password;
                    peer.init();
                    peer.loginUser(password);

                    break;
                default:
                    std::cout << "Choice unknown" << std::endl;
                    incorrectChoice = true;
            }
        } else {
            std::cout << "1) Logout from directory server." << std::endl
                      << "2) Request feed from directory server." << std::endl
                      << "3) Add image to directory server." << std::endl
                      << "4) Search username in directory server." << std::endl
                      << "5) Request to view image from peer." << std::endl
                      << "6) Download image from peer." << std::endl
                      << "7) Show my requests." << std::endl;
            std::string targetUsername;
            switch(peerChoice) {
                case peerChoices::Logout:
                    peer.logoutUser();
                    break;
                case peerChoices::Feed:
                    peer.feed(0,20);
                    break;
                case peerChoices::AddImage:
                    peer.addImage("MEOW");
                    break;
                case peerChoices::GetRequests:
                    break;
                case peerChoices::Search:
                    peer.searchUser(targetUsername);
                    break;
                case peerChoices::RequestView:
                    break;
                case peerChoices::DownloadImage:
                    break;
                default:
                    std::cout << "Choice unknown" << std::endl;
                    incorrectChoice = true;
            }
        }
    } while(incorrectChoice);
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

bool Peer::isDoSOnline() const {
    return DoSOnline;
}

void Peer::setDoSOnline(bool doSOnline) {
    DoSOnline = doSOnline;
}



#pragma clang diagnostic pop