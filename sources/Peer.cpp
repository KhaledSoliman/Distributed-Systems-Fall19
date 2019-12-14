#include "../headers/Peer.h"
#include "../headers/MessageStructures.h"

#include <boost/gil.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/extension/numeric/sampler.hpp>
#include <boost/gil/extension/numeric/resample.hpp>

#include <boost/thread.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

Peer::Peer(const std::string &hostname, int port, const std::string &directoryServerHostname, int directoryServerPort)
        : Server(hostname, port) {
    this->authenticated = false;
    this->directoryServerHostname = directoryServerHostname;
    this->directoryServerPort = directoryServerPort;
}

Peer::Peer(const std::string &listenHostname, int listenPort) : Server(listenHostname, listenPort), listenHostname(listenHostname) {
    this->listenPort = listenPort;
    token = "";
    username = "";
    feedIndex = 0;
    myCache = Cache();
    authenticated = false;
    DoSOnline = false;
}

bool Peer::connectToDoS() {
    this->connectToServer(this->directoryServerHostname, this->directoryServerPort);
}

bool Peer::connectToPeer(const std::string &peerHostname, int peerPort) {
    this->connectToServer(peerHostname, peerPort);
}

bool Peer::discoverDirectoryService() {
    Hello hello = Hello();
    hello.setMessage("DoS");
    std::string broadcastIP = "255.255.255.255";
    int broadcastPort = 3001;
    this->Client::initBroadcast(broadcastIP, broadcastPort);
    Message *message = this->Client::saveAndGetMessage(hello, Message::MessageType::Request,
                                                       Message::OperationType::HELLO);
    if (this->Client::broadcast(message)) {
        Message *reply = this->Client::receiveWithTimeout();
        if (reply == nullptr)
            return false;
        auto helloReply = load<Hello>(reply->getMessage());
        if (helloReply.getMessage() == "ME") {
            this->DoSOnline = true;
            this->directoryServerHostname = reply->getRPCId().getAddress();
            this->directoryServerPort = reply->getRPCId().getPortNumber();
            return true;
        }
    }
    return false;
}

void Peer::DoSChecker(boost::shared_ptr<Peer> peer) {
    while (true) {
        while (!peer->discoverDirectoryService()) {
            std::cout << "Discovering DoS..." << std::endl;
            boost::this_thread::sleep(boost::posix_time::milliseconds(750));
        }
        int pingMissCounter = 0;
        while (pingMissCounter < 3) {
            if (!peer->pingDoS()) {
                peer->setDoSOnline(false);
                pingMissCounter += 1;
                std::cout << "DoS went offline please hold while we try to connect again..." << std::endl;
            } else {
                peer->setDoSOnline(true);
            }
            boost::this_thread::sleep(boost::posix_time::milliseconds(22500));
        }
    }
}

bool Peer::pingDoS() {
    Echo echo = Echo();
    echo.setMsg("PING");
    this->connectToDoS();
    Message *message = this->Client::saveAndGetMessage(echo, Message::MessageType::Request,
                                                       Message::OperationType::ECHO);
    if (this->Client::send(message)) {
        Message *reply = this->Client::receiveWithTimeout();
        if (reply == nullptr)
            return false;
        Echo echoReply = load<Echo>(reply->getMessage());
        if (echoReply.getMsg() == echoReply.getMsg())
            return true;
    }
    return false;
}

void Peer::init() {
    boost::thread DoSThread(&Peer::DoSChecker, boost::shared_ptr<Peer>(this));
    boost::thread serverThread(&Peer::listen, boost::shared_ptr<Peer>(this));
    boost::thread clientThread(&Peer::handleChoice, boost::shared_ptr<Peer>(this));
    boost::thread cacheThread(&Peer::cache, boost::shared_ptr<Peer>(this));
    while (true) {
        if(this->authenticated && !this->helloProtocol) {
            boost::thread authHelloThread(&Peer::authHello, boost::shared_ptr<Peer>(this));
            this->setHelloProtocol(true);
        }
    };
}

void Peer::cache(boost::shared_ptr<Peer> peer) {
    while (true) {

    }
}

void Peer::listen(boost::shared_ptr<Peer> peer) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        Message *message = peer->Server::receive();
        boost::thread handleRequest(&Peer::handleRequest, message, peer);
    }
#pragma clang diagnostic pop
}

void Peer::handleRequest(Message *message, boost::shared_ptr<Peer> peer) {
    switch (message->getMessageType()) {
        case Message::MessageType::Request:
            switch (message->getOperation()) {
                case Message::OperationType::ECHO:
                    peer->Server::saveAndGetMessage(load<Echo>(message->getMessage()), Message::MessageType::Reply,
                                                    Message::OperationType::ECHO);
                    break;
                case Message::OperationType::DOWNLOAD_IMAGE:
                    peer->Server::saveAndGetMessage(load<Echo>(message->getMessage()),
                                                    Message::MessageType::Reply,
                                                    Message::OperationType::DOWNLOAD_IMAGE);
                    break;
                case Message::OperationType::VIEW_IMAGE:
                    peer->Server::saveAndGetMessage(load<AddViewerRequest>(message->getMessage()),
                                                    Message::MessageType::Reply,
                                                    Message::OperationType::ADD_VIEWER);
                    break;
                case Message::OperationType::ADD_VIEWER:
                    peer->Server::saveAndGetMessage(load<AddViewerRequest>(message->getMessage()),
                                                    Message::MessageType::Reply,
                                                    Message::OperationType::ADD_VIEWER);
                    break;
                case Message::OperationType::REMOVE_VIEWER:
                    peer->Server::saveAndGetMessage(load<AddViewerRequest>(message->getMessage()),
                                                    Message::MessageType::Reply,
                                                    Message::OperationType::ADD_VIEWER);
                    break;
                case Message::OperationType::UPDATE_VIEW_LIMIT:
                    peer->Server::saveAndGetMessage(load<AddViewerRequest>(message->getMessage()),
                                                    Message::MessageType::Reply,
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

void Peer::handleChoice(boost::shared_ptr<Peer> peer) {
    int peerChoice;
    enum guestChoices {
        Register = 1,
        Login
    };
    enum peerChoices {
        Logout = 1,
        Feed,
        AddImage,
        DeleteImage,
        Search,
        RequestView,
        DownloadImage,
        GetRequests
    };
    do {
        if (!peer->authenticated) {
            std::cout << "Please choose what you want to do as a peer?" << std::endl
                      << "List of choices:" << std::endl
                      << "1) Register with directory server." << std::endl
                      << "2) Already have an account then login with directory server." << std::endl;
            std::cin >> peerChoice;
            std::string username, password, confirmPassword;
            switch (peerChoice) {
                case guestChoices::Register:
                    std::cout << "Please input your username" << std::endl;
                    std::cin >> username;
                    std::cout << "Please input your password" << std::endl;
                    std::cin >> password;
                    std::cout << "Please confirm your password" << std::endl;
                    std::cin >> confirmPassword;
                    if (confirmPassword == password) {
                        Message *message = peer->Client::saveAndGetMessage(peer->registerUser(username, password),
                                                                           Message::MessageType::Request,
                                                                           Message::OperationType::REGISTER);
                        peer->connectToDoS();
                        if (peer->Client::send(message)) {
                            Message *reply = peer->Client::receiveWithTimeout();
                            if (reply == nullptr) {
                                std::cout << "DoS didn't respond" << std::endl;
                            } else {
                                auto registerReply = load<RegisterReply>(reply->getMessage());
                                std::cout << "Register Reply: " << registerReply.isRegistered() << std::endl;
                            }
                        }
                    } else {
                        std::cout << "Passwords do not match" << std::endl;
                    }
                    break;
                case guestChoices::Login:
                    std::cout << "Please input your username" << std::endl;
                    std::cin >> username;
                    std::cout << "Please input your password" << std::endl;
                    std::cin >> password;
                    {
                        peer->setUsername(username);
                        Message *message = peer->Client::saveAndGetMessage(peer->loginUser(password),
                                                                           Message::MessageType::Request,
                                                                           Message::OperationType::LOGIN);
                        peer->connectToDoS();
                        if (peer->Client::send(message)) {
                            Message *reply = peer->Client::receiveWithTimeout();
                            if (reply == nullptr) {
                                std::cout << "DoS didn't respond" << std::endl;
                            } else {
                                auto loginReply = load<LoginReply>(reply->getMessage());
                                if (!loginReply.isFlag()) {
                                    peer->setAuthenticated(true);
                                    peer->setToken(loginReply.getToken());
                                    std::cout << "Login Reply: " << loginReply.getToken() << std::endl;
                                } else {
                                    peer->setAuthenticated(false);
                                    std::cout << "Login Reply: " << loginReply.getMsg() << std::endl;
                                }
                            }
                        }
                    }
                    break;
                default:
                    std::cout << "Choice unknown" << std::endl;
            }
        } else {
            std::cout << "1) Logout from directory server." << std::endl
                      << "2) Request feed from directory server." << std::endl
                      << "3) Add image to directory server." << std::endl
                      << "4) Delete image from directory server." << std::endl
                      << "5) Search username in directory server." << std::endl
                      << "6) Request to view image from peer->" << std::endl
                      << "7) Download image from peer->" << std::endl
                      << "8) Show my requests." << std::endl;
            std::cin >> peerChoice;
            switch (peerChoice) {
                case peerChoices::Logout: {
                    Message *message = peer->Client::saveAndGetMessage(peer->logoutUser(),
                                                                       Message::MessageType::Request,
                                                                       Message::OperationType::LOGOUT);
                    peer->connectToDoS();
                    if (peer->Client::send(message)) {
                        Message *reply = peer->Client::receiveWithTimeout();
                        if (reply == nullptr) {
                            std::cout << "DoS didn't respond" << std::endl;
                        } else {
                            auto logoutReply = load<LogoutReply>(reply->getMessage());
                            if (!logoutReply.isFlag()) {
                                peer->setAuthenticated(false);
                                peer->setToken("");
                                std::cout << "Logout Reply: " << !logoutReply.isFlag() << std::endl;
                            } else {
                                std::cout << "Logout Reply: " << logoutReply.getMsg() << std::endl;
                            }
                        }
                    }
                }
                    break;
                case peerChoices::Feed: {
                    int feedSize;
                    std::cout << "Please input feed size:" << std::endl;
                    std::cin >> feedSize;
                    Message *message = peer->Client::saveAndGetMessage(peer->feed(feedSize),
                                                                       Message::MessageType::Request,
                                                                       Message::OperationType::FEED);
                    peer->connectToDoS();
                    if (peer->Client::send(message)) {
                        Message *reply = peer->Client::receiveWithBlock();
                        if (reply == nullptr) {
                            std::cout << "DoS didn't respond" << std::endl;
                        } else {
                            auto feedReply = load<FeedReply>(reply->getMessage());
                            if (!feedReply.isFlag()) {
                                std::ofstream out;
                                out.open("khaled.jpg");
                                out << feedReply.getImages().at("khaled");
                                out.close();
                                std::cout << "Feed Reply: " << !feedReply.isFlag() << std::endl;
                            } else {
                                std::cout << "Feed Reply: " << feedReply.getMsg() << std::endl;
                            }
                        }
                    }
                }
                    break;
                case peerChoices::AddImage: {
                    std::string imagePath, imageName;
                    std::cout << "Please input image path:" << std::endl;
                    std::cin >> imagePath;
                    std::cout << "Please input image name" << std::endl;
                    std::cin >> imageName;
                    Message *message = peer->Client::saveAndGetMessage(peer->addImage(imagePath, imageName),
                                                                       Message::MessageType::Request,
                                                                       Message::OperationType::ADD_IMAGE);
                    peer->connectToDoS();
                    if (peer->Client::send(message)) {
                        Message *reply = peer->Client::receiveWithTimeout();
                        if (reply == nullptr) {
                            std::cout << "DoS didn't respond" << std::endl;
                        } else {
                            auto addImageReply = load<AddImageReply>(reply->getMessage());
                            if (!addImageReply.isFlag()) {
                                std::cout << "AddImage Reply: " << !addImageReply.isFlag() << std::endl;
                            } else {
                                std::cout << "AddImage Reply: " << addImageReply.getMsg() << std::endl;
                            }
                        }
                    }
                }
                    break;
                case peerChoices::DeleteImage: {
                    std::string imageName;
                    std::cout << "Please input image name" << std::endl;
                    std::cin >> imageName;
                    Message *message = peer->Client::saveAndGetMessage(peer->delImage(imageName),
                                                                       Message::MessageType::Request,
                                                                       Message::OperationType::DELETE_IMAGE);
                    peer->connectToDoS();
                    if (peer->Client::send(message)) {
                        Message *reply = peer->Client::receiveWithTimeout();
                        if (reply == nullptr) {
                            std::cout << "DoS didn't respond" << std::endl;
                        } else {
                            auto delImageReply = load<DeleteImageReply>(reply->getMessage());
                            if (!delImageReply.isFlag()) {
                                std::cout << "DeleteImage Reply: " << !delImageReply.isFlag() << std::endl;
                            } else {
                                std::cout << "DeleteImage Reply: " << delImageReply.getMsg() << std::endl;
                            }
                        }
                    }
                }
                    break;
                case peerChoices::GetRequests: {
                    Message *message = peer->Client::saveAndGetMessage(peer->getRequests(),
                                                                       Message::MessageType::Request,
                                                                       Message::OperationType::GET_REQUESTS);
                    peer->connectToDoS();
                    if (peer->Client::send(message)) {
                        Message *reply = peer->Client::receiveWithTimeout();
                        if (reply == nullptr) {
                            std::cout << "DoS didn't respond" << std::endl;
                        } else {
                            auto viewImageRequest = load<ViewImageRequest>(reply->getMessage());
                            if (!viewImageRequest.isFlag()) {
                                std::cout << "GetRequest Reply: " << !viewImageRequest.isFlag() << std::endl;
                            } else {
                                std::cout << "GetRequest Reply: " << viewImageRequest.getMsg() << std::endl;
                            }
                        }
                    }
                }
                    break;
                case peerChoices::Search: {
                    std::string targetUsername;
                    std::cout << "Enter username to search directory for:" << std::endl;
                    std::cin >> targetUsername;
                    Message *message = peer->Client::saveAndGetMessage(peer->searchUser(targetUsername),
                                                                       Message::MessageType::Request,
                                                                       Message::OperationType::SEARCH);
                    peer->connectToDoS();
                    if (peer->Client::send(message)) {
                        Message *reply = peer->Client::receiveWithTimeout();
                        if (reply == nullptr) {
                            std::cout << "DoS didn't respond" << std::endl;
                        } else {
                            auto searchReply = load<SearchReply>(reply->getMessage());
                            if (!searchReply.isFlag()) {
                                peer->setAuthenticated(true);
                                std::cout << "Search Reply: " << !searchReply.isFlag() << std::endl;
                            } else {
                                peer->setAuthenticated(false);
                                std::cout << "search Reply: " << searchReply.getMsg() << std::endl;
                            }
                        }
                    }
                }
                    break;
                case peerChoices::RequestView: {
                    std::string ImageName;
                    int viewNo;
                    std::cout << "Enter Image Name: " << std::endl;
                    std::cin >> ImageName;
                    std::cout << "Enter the number of required Views: " << std::endl;
                    std::cin >> viewNo;
                    Message *message = peer->Client::saveAndGetMessage(peer->getRequests(),
                                                                       Message::MessageType::Request,
                                                                       Message::OperationType::ADD_VIEWER);
                    peer->connectToDoS();
                    if (peer->Client::send(message)) {
                        Message *reply = peer->Client::receiveWithTimeout();
                        if (reply == nullptr) {
                            std::cout << "DoS didn't respond" << std::endl;
                        } else {
                            auto addViewerReply = load<AddViewerReply>(reply->getMessage());
                            if (!addViewerReply.isFlag()) {
                                std::cout << "Search Reply: " << !addViewerReply.isFlag() << std::endl;
                            } else {
                                std::cout << "search Reply: " << addViewerReply.getMsg() << std::endl;
                            }
                        }
                    }
                }
                    break;
                case peerChoices::DownloadImage: {
                    std::string imageName;
                    std::cout << "Please input image name" << std::endl;
                    std::cin >> imageName;
                    Message *message = peer->Client::saveAndGetMessage(peer->downloadImage(imageName),
                                                                       Message::MessageType::Request,
                                                                       Message::OperationType::DOWNLOAD_IMAGE);
                    peer->connectToDoS();
                    if (peer->Client::send(message)) {
                        Message *reply = peer->Client::receiveWithTimeout();
                        if (reply == nullptr) {
                            std::cout << "DoS didn't respond" << std::endl;
                        } else {
                            auto downloadImageReply = load<DownloadImageReply>(reply->getMessage());
                            if (!downloadImageReply.isFlag()) {
                                std::cout << "Download Reply: " << !downloadImageReply.isFlag() << std::endl;
                            } else {
                                std::cout << "download Reply: " << downloadImageReply.getMsg() << std::endl;
                            }
                        }
                    }
                }
                    break;
                default:
                    std::cout << "Choice unknown" << std::endl;
            }
        }
    } while (true);
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

AddImageRequest Peer::addImage(const std::string &imagePath, const std::string &imageName) {
    AddImageRequest request = AddImageRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageName(imageName);
    request.setThumbnail(this->createThumbnail(imagePath));
    return request;
}

DeleteImageRequest Peer::delImage(const std::string &imageName) {
    DeleteImageRequest request = DeleteImageRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageName(imageName);
    return request;
}

FeedRequest Peer::feed(int imageNum) {
    FeedRequest request = FeedRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageNum(imageNum);
    request.setLastIndex(this->feedIndex);
    return request;
}

std::string Peer::createThumbnail(const std::string &imagePath) {
    namespace bg = boost::gil;

//    std::vector<std::string> pathList;
//
//    boost::split(imagePath, pathList, boost::is_any_of("."));
//    if(pathList[1] == "jpeg" || pathList[1] == "jpg") {
    bg::rgb8_image_t img;
    bg::read_image(imagePath, img, bg::jpeg_tag{});
    bg::rgb8_image_t square100(13583, 5417);
    bg::resize_view(bg::const_view(img), bg::view(square100), bg::bilinear_sampler{});
    bg::write_view("resize.jpg", bg::const_view(square100), bg::jpeg_tag{});
    std::ifstream in;
    in.open("resize.jpg");
    std::string image((std::istreambuf_iterator<char>(in)),
                      std::istreambuf_iterator<char>());
    in.close();
    return image;
//    } else if(pathList[1] == "png") {
//        bg::rgba8_image_t img;
//        bg::read_image(imagePath, img, bg::{});
//        bg::rgba8_image_t newSize(640, 480);
//        bg::resize_view(const_view(img), bg::view(newSize), bg::bilinear_sampler{});
//        bg::write_view("resize.png", bg::const_view(newSize), bg::png_tag{});
//        std::ifstream in;
//        in.open("resize.png");
//        std::string image;
//        in >> image;
//        in.close();
//        return image;
//    }
}


bool Peer::isDoSOnline() const {
    return DoSOnline;
}

void Peer::setDoSOnline(bool doSOnline) {
    DoSOnline = doSOnline;
}

const std::string &Peer::getDirectoryServerHostname() const {
    return directoryServerHostname;
}

void Peer::setDirectoryServerHostname(const std::string &directoryServerHostname) {
    Peer::directoryServerHostname = directoryServerHostname;
}

int Peer::getDirectoryServerPort() const {
    return directoryServerPort;
}

void Peer::setDirectoryServerPort(int directoryServerPort) {
    Peer::directoryServerPort = directoryServerPort;
}

const std::string &Peer::getUsername() const {
    return username;
}

void Peer::setUsername(const std::string &username) {
    Peer::username = username;
}

const std::string &Peer::getToken() const {
    return token;
}

void Peer::setToken(const std::string &token) {
    Peer::token = token;
}

bool Peer::isAuthenticated() const {
    return authenticated;
}

void Peer::setAuthenticated(bool authenticated) {
    Peer::authenticated = authenticated;
}

int Peer::getFeedIndex() const {
    return feedIndex;
}

void Peer::setFeedIndex(int feedIndex) {
    Peer::feedIndex = feedIndex;
}

GetRequests Peer::getRequests() {
    GetRequests message = GetRequests();
    message.setToken(this->token);
    message.setUserName(this->username);
    return message;
}

DownloadImageRequest Peer::downloadImage(const std::string &imageName) {
    DownloadImageRequest request = DownloadImageRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageName(imageName);
    return request;
}

bool Peer::isHelloProtocol() const {
    return helloProtocol;
}

void Peer::setHelloProtocol(bool helloProtocol) {
    Peer::helloProtocol = helloProtocol;
}

void Peer::authHello(boost::shared_ptr<Peer> peer) {
    while (peer->isDoSOnline() && peer->isAuthenticated()) {
        AuthenticatedHello hello = AuthenticatedHello();
        hello.setUserName(peer->getUsername());
        hello.setToken(peer->getToken());
        hello.setIpAddress(peer->getListenHostname());
        hello.setPort(peer->getListenPort());
        peer->connectToDoS();
        Message *message = peer->Client::saveAndGetMessage(hello, Message::MessageType::Request,
                                                           Message::OperationType::AUTH_HELLO);
        peer->Client::send(message);
        boost::this_thread::sleep(boost::posix_time::milliseconds(750));
    }
}

const std::string &Peer::getListenHostname() const {
    return listenHostname;
}

void Peer::setListenHostname(const std::string &listenHostname) {
    Peer::listenHostname = listenHostname;
}

int Peer::getListenPort() const {
    return listenPort;
}

void Peer::setListenPort(int listenPort) {
    Peer::listenPort = listenPort;
}


#pragma clang diagnostic pop