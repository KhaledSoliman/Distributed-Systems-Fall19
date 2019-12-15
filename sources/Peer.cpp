#include "../headers/Peer.h"
#include "../headers/MessageStructures.h"
#include "../headers/Seng.h"

#include <boost/gil.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/extension/numeric/sampler.hpp>
#include <boost/gil/extension/numeric/resample.hpp>

#include <boost/thread.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#define FEED_DIR "../feed/"
#define PROFILE_DIR "../feed/profile/"
#define TEMP_DIR "../temp/"
#define IMAGE_DIR "../images/"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

Peer::Peer(const std::string &hostname, int port, const std::string &directoryServerHostname, int directoryServerPort)
        : Server(hostname, port) {
    this->authenticated = false;
    this->directoryServerHostname = directoryServerHostname;
    this->directoryServerPort = directoryServerPort;
}

Peer::Peer(const std::string &listenHostname, int listenPort) : Server(listenHostname, listenPort) {
    this->listenHostname = listenHostname;
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

bool Peer::connectToPeer() {
    this->connectToServer(this->peerHostname, this->peerPort);
}

Message *Peer::sendDoS(Message *message) {
    this->connectToDoS();
    this->Client::send(message);
    return this->Client::receiveWithTimeout();
}

Message *Peer::sendPeer(Message *message, const std::string &peerUsername) {
    auto reply = this->sendDoS(this->Client::saveAndGetMessage(this->searchUser(peerUsername),
                                                               Message::MessageType::Request,
                                                               Message::OperationType::SEARCH));
    if (reply == nullptr) {
        std::cout << "DoS didn't respond" << std::endl;
    } else {
        auto searchReply = load<SearchReply>(reply->getMessage());
        if (!searchReply.isFlag()) {
            this->setPeerHostname(searchReply.getAddress());
            this->setPeerPort(searchReply.getPort());
            this->connectToPeer();
            this->Client::send(message);
            return this->Client::receiveWithTimeout();
        } else {
            std::cout << "Search Reply: " << searchReply.getMsg() << std::endl;
        }
    }
    return nullptr;
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
            boost::this_thread::sleep(boost::posix_time::minutes(30));
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
        if (this->authenticated && !this->helloProtocol) {
            boost::thread authHelloThread(&Peer::authHello, boost::shared_ptr<Peer>(this));
            this->setHelloProtocol(true);
        }
    };
}

void Peer::cache(boost::shared_ptr<Peer> peer) {
    while (true) {
        peer->getMyCache().updateCache();
        boost::this_thread::sleep(boost::posix_time::hours(1));
    }
}

void Peer::listen(boost::shared_ptr<Peer> peer) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        Message *message = peer->Server::receive();
        boost::thread handleRequest(&Peer::handleRequest, message, peer);
        if (message->getOperation() == Message::OperationType::DOWNLOAD_IMAGE)
            boost::this_thread::sleep(boost::posix_time::seconds(15));
    }
#pragma clang diagnostic pop
}

void Peer::handleRequest(Message *message, boost::shared_ptr<Peer> peer) {
    switch (message->getMessageType()) {
        case Message::MessageType::Request:
            Message *reply;
            switch (message->getOperation()) {
                case Message::OperationType::ECHO:
                    reply = peer->Server::saveAndGetMessage(load<Echo>(message->getMessage()),
                                                            Message::MessageType::Reply,
                                                            Message::OperationType::ECHO);
                    break;
                case Message::OperationType::DOWNLOAD_IMAGE:
                    reply = peer->Server::saveAndGetMessage(
                            peer->sendImage(load<DownloadImageRequest>(message->getMessage())),
                            Message::MessageType::Reply,
                            Message::OperationType::DOWNLOAD_IMAGE);
                    break;
                case Message::OperationType::REMOVE_VIEWER:
                    reply = peer->Server::saveAndGetMessage(
                            peer->removeViewer(load<RemoveViewerRequest>(message->getMessage())),
                            Message::MessageType::Reply,
                            Message::OperationType::REMOVE_VIEWER);
                    break;
                case Message::OperationType::UPDATE_VIEW_LIMIT:
                    reply = peer->Server::saveAndGetMessage(
                            peer->updateLimit(load<UpdateLimitRequest>(message->getMessage())),
                            Message::MessageType::Reply,
                            Message::OperationType::UPDATE_VIEW_LIMIT);
                    break;
                case Message::OperationType::GET_REMAINING_VIEWS:
                    reply = peer->Server::saveAndGetMessage(
                            peer->getRemainingViews(load<GetRemainingViewsRequest>(message->getMessage())),
                            Message::MessageType::Reply,
                            Message::OperationType::GET_REMAINING_VIEWS);
                    break;
                default:
                    break;
            }
            peer->Server::send(reply);
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
        ShowOnline,
        Feed,
        FeedProfile,
        AddImage,
        DeleteImage,
        Search,
        RequestView,
        GetRequests,
        AddViewer,
        DenyViewer,
        GetPendingRequests,
        DownloadImage,
        UpdateLimit,
        RemoveViewer,
        GetOtherRemainingViews,
        GetMyRemainingViews
    };
    do {
        if (!peer->authenticated) {
            std::cout << "Please choose what you want to do as a peer?" << std::endl
                      << "List of choices:" << std::endl
                      << "1) Register with directory server." << std::endl
                      << "2) Already have an account then login with directory server." << std::endl;
            std::cin >> peerChoice;
            switch (peerChoice) {
                case guestChoices::Register: {
                    std::string username, password, confirmPassword;
                    std::cout << "Please input your username" << std::endl;
                    std::cin >> username;
                    std::cout << "Please input your password" << std::endl;
                    std::cin >> password;
                    std::cout << "Please confirm your password" << std::endl;
                    std::cin >> confirmPassword;
                    if (confirmPassword == password) {
                        auto reply = peer->sendDoS(
                                peer->Client::saveAndGetMessage(peer->registerUser(username, password),
                                                                Message::MessageType::Request,
                                                                Message::OperationType::REGISTER));
                        if (reply == nullptr) {
                            std::cout << "DoS didn't respond" << std::endl;
                        } else {
                            auto registerReply = load<RegisterReply>(reply->getMessage());
                            std::cout << "Register Reply: " << registerReply.isRegistered() << std::endl;
                        }
                    } else {
                        std::cout << "Passwords do not match" << std::endl;
                    }
                }
                    break;
                case guestChoices::Login: {
                    std::string username, password;
                    std::cout << "Please input your username" << std::endl;
                    std::cin >> username;
                    std::cout << "Please input your password" << std::endl;
                    std::cin >> password;
                    peer->setUsername(username);
                    auto reply = peer->sendDoS(peer->Client::saveAndGetMessage(peer->loginUser(password),
                                                                               Message::MessageType::Request,
                                                                               Message::OperationType::LOGIN));
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
                    break;
                default:
                    std::cout << "Choice unknown" << std::endl;
            }
        } else {
            std::cout << "1) Logout from directory server." << std::endl
                      << "2) Show online users." << std::endl
                      << "3) Request feed from directory server." << std::endl
                      << "4) Request profile from directory server." << std::endl
                      << "5) Add image to directory server." << std::endl
                      << "6) Delete image from directory server." << std::endl
                      << "7) Search username in directory server." << std::endl
                      << "8) Request to view image from peer" << std::endl
                      << "9) Show my requests." << std::endl
                      << "10) Accept viewer request." << std::endl
                      << "11) Deny viewer request." << std::endl
                      << "12) Show pending requests." << std::endl
                      << "13) Download image from peer." << std::endl
                      << "14) Update view limit on image." << std::endl
                      << "15) Remove viewer from image." << std::endl
                      << "16) Show remaining views on other's image." << std::endl
                      << "17) Show remaining views on my image." << std::endl;
            std::cin >> peerChoice;
            switch (peerChoice) {
                case peerChoices::Logout: {
                    auto reply = peer->sendDoS(peer->Client::saveAndGetMessage(peer->logoutUser(),
                                                                               Message::MessageType::Request,
                                                                               Message::OperationType::LOGOUT));
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
                    break;
                case peerChoices::ShowOnline: {
                    auto reply = peer->sendDoS(peer->Client::saveAndGetMessage(peer->showOnline(),
                                                                               Message::MessageType::Request,
                                                                               Message::OperationType::SHOW_ONLINE));
                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto showOnlineReply = load<ShowOnlineReply>(reply->getMessage());
                        if (!showOnlineReply.isFlag()) {
                            std::cout << "Online Users:" << std::endl;
                            for (const std::string &username :showOnlineReply.getUsers()) {
                                std::cout << username << " is Online." << std::endl;
                            }
                            std::cout << "Show Online Reply: " << !showOnlineReply.isFlag() << std::endl;
                        } else {
                            std::cout << "Show Online Reply: " << showOnlineReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::Feed: {
                    int feedSize;
                    std::cout << "Please input feed size:" << std::endl;
                    std::cin >> feedSize;
                    auto reply = peer->sendDoS(peer->Client::saveAndGetMessage(peer->feed(feedSize),
                                                                               Message::MessageType::Request,
                                                                               Message::OperationType::FEED));
                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto feedReply = load<FeedReply>(reply->getMessage());
                        if (!feedReply.isFlag()) {
                            std::ofstream out;
                            for (const auto &image: feedReply.getImages()) {
                                out.open(FEED_DIR + image.second.first + '-' + image.first);
                                out << image.second.second;
                                out.close();
                            }
                            peer->setFeedIndex(feedReply.getCurrentIndex());
                            std::cout << "Feed Reply: " << !feedReply.isFlag() << std::endl;
                        } else {
                            std::cout << "Feed Reply: " << feedReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::FeedProfile: {
                    std::string username;
                    int feedSize;
                    std::cout << "Please input username:" << std::endl;
                    std::cin >> username;
                    std::cout << "Please input feed size:" << std::endl;
                    std::cin >> feedSize;
                    auto reply = peer->sendDoS(peer->Client::saveAndGetMessage(peer->feedProfile(username, feedSize),
                                                                               Message::MessageType::Request,
                                                                               Message::OperationType::FEED_PROFILE));
                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto feedReply = load<FeedProfileReply>(reply->getMessage());
                        if (!feedReply.isFlag()) {
                            std::ofstream out;
                            for (const auto &image: feedReply.getImages()) {
                                out.open(PROFILE_DIR + feedReply.getTargetUsername() + '-' + image.first);

                                out << image.second;
                                out.close();
                            }
                            peer->addProfileIndex(feedReply.getTargetUsername(), feedReply.getCurrentIndex());
                            std::cout << "Feed Profile Reply: " << !feedReply.isFlag() << std::endl;
                        } else {
                            std::cout << "Feed Profile Reply: " << feedReply.getMsg() << std::endl;
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
                    auto reply = peer->sendDoS(peer->Client::saveAndGetMessage(peer->addImage(imagePath, imageName),
                                                                               Message::MessageType::Request,
                                                                               Message::OperationType::ADD_IMAGE));
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
                    break;
                case peerChoices::DeleteImage: {
                    std::string imageName;
                    std::cout << "Please input image name" << std::endl;
                    std::cin >> imageName;
                    auto reply = peer->sendDoS(peer->Client::saveAndGetMessage(peer->delImage(imageName),
                                                                               Message::MessageType::Request,
                                                                               Message::OperationType::DELETE_IMAGE));
                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto delImageReply = load<DeleteImageReply>(reply->getMessage());
                        if (!delImageReply.isFlag()) {
                            std::string path = IMAGE_DIR + imageName;
                            remove(path.c_str());
                            std::cout << "DeleteImage Reply: " << !delImageReply.isFlag() << std::endl;
                        } else {
                            std::cout << "DeleteImage Reply: " << delImageReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::Search: {
                    std::string targetUsername;
                    std::cout << "Enter username to search directory for:" << std::endl;
                    std::cin >> targetUsername;
                    auto reply = peer->sendDoS(peer->Client::saveAndGetMessage(peer->searchUser(targetUsername),
                                                                               Message::MessageType::Request,
                                                                               Message::OperationType::SEARCH));
                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto searchReply = load<SearchReply>(reply->getMessage());
                        if (!searchReply.isFlag()) {
                            peer->setPeerHostname(searchReply.getAddress());
                            peer->setPeerPort(searchReply.getPort());
                            std::cout << "Search Reply: " << searchReply.getAddress() << "  |  "
                                      << searchReply.getPort() << std::endl;
                        } else {
                            std::cout << "Search Reply: " << searchReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::RequestView: {
                    std::string imageName, username;
                    int viewNo;
                    std::cout << "Enter profile username:" << std::endl;
                    std::cin >> username;
                    std::cout << "Enter Image Name: " << std::endl;
                    std::cin >> imageName;
                    std::cout << "Enter the number of required Views: " << std::endl;
                    std::cin >> viewNo;
                    auto *reply = peer->sendDoS(
                            peer->Client::saveAndGetMessage(peer->viewImage(username, imageName, viewNo),
                                                            Message::MessageType::Request,
                                                            Message::OperationType::VIEW_IMAGE));

                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto viewImageReply = load<ViewImageReply>(reply->getMessage());
                        if (!viewImageReply.isFlag()) {
                            std::cout << "View Image Reply: " << !viewImageReply.isFlag() << std::endl;
                        } else {
                            std::cout << "View Image Reply: " << viewImageReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::GetRequests: {
                    auto *reply = peer->sendDoS(peer->Client::saveAndGetMessage(peer->getRequests(),
                                                                                Message::MessageType::Request,
                                                                                Message::OperationType::GET_REQUESTS));
                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto getRequestsReply = load<GetRequestsReply>(reply->getMessage());
                        if (!getRequestsReply.isFlag()) {
                            for (const auto &request: getRequestsReply.getRequests()) {
                                std::cout << "Request From " << request.getUserName() << ":" << std::endl;
                                std::cout << "Image: " << request.getImageName() << std::endl;
                                std::cout << "Views: " << request.getViewNum() << std::endl;
                            }
                            std::cout << "GetRequest Reply: " << !getRequestsReply.isFlag() << std::endl;
                        } else {
                            std::cout << "GetRequest Reply: " << getRequestsReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::AddViewer: {
                    std::string viewerName, imageName;
                    int viewNum;
                    std::cout << "Enter viewer username:" << std::endl;
                    std::cin >> viewerName;
                    std::cout << "Enter Image Name: " << std::endl;
                    std::cin >> imageName;
                    std::cout << "Enter the number of required Views on the specified request: " << std::endl;
                    std::cin >> viewNum;
                    auto *reply = peer->sendDoS(
                            peer->Client::saveAndGetMessage(peer->acceptRequest(imageName, viewerName, viewNum),
                                                            Message::MessageType::Request,
                                                            Message::OperationType::ADD_VIEWER));
                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto addViewerReply = load<AddViewerReply>(reply->getMessage());
                        if (!addViewerReply.isFlag()) {
                            std::cout << "AddViewer Reply: " << !addViewerReply.isFlag() << std::endl;
                        } else {
                            std::cout << "AddViewer Reply: " << addViewerReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::DenyViewer: {
                    std::string viewerName, imageName;
                    int viewNum;
                    std::cout << "Enter viewer username:" << std::endl;
                    std::cin >> viewerName;
                    std::cout << "Enter Image Name: " << std::endl;
                    std::cin >> imageName;
                    std::cout << "Enter the number of required Views on the specified request: " << std::endl;
                    std::cin >> viewNum;
                    auto *reply = peer->sendDoS(
                            peer->Client::saveAndGetMessage(peer->denyRequest(imageName, viewerName, viewNum),
                                                            Message::MessageType::Request,
                                                            Message::OperationType::DENY_VIEWER));
                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto denyViewerReply = load<DenyViewerReply>(reply->getMessage());
                        if (!denyViewerReply.isFlag()) {
                            std::cout << "DenyViewer Reply: " << !denyViewerReply.isFlag() << std::endl;
                        } else {
                            std::cout << "DenyViewer Reply: " << denyViewerReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::GetPendingRequests: {
                    auto *reply = peer->sendDoS(peer->Client::saveAndGetMessage(peer->getPendingRequests(),
                                                                                Message::MessageType::Request,
                                                                                Message::OperationType::GET_PENDING_REQUESTS));
                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto getRequestsReply = load<GetPendingRequestsReply>(reply->getMessage());
                        if (!getRequestsReply.isFlag()) {
                            for (const auto &request: getRequestsReply.getRequests()) {
                                std::cout << "Request From " << request.first.getUserName() << ":" << std::endl;
                                std::cout << "Image: " << request.first.getImageName() << std::endl;
                                std::cout << "Views: " << request.first.getViewNum() << std::endl;
                                std::cout << "Accepted: " << (request.second ? "Yes" : "No") << std::endl;
                            }
                            std::cout << "GetRequest Reply: " << !getRequestsReply.isFlag() << std::endl;
                        } else {
                            std::cout << "GetRequest Reply: " << getRequestsReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::DownloadImage: {
                    std::string imageName, username;
                    std::cout << "Please input target username" << std::endl;
                    std::cin >> username;
                    std::cout << "Please input image name" << std::endl;
                    std::cin >> imageName;
                    auto *reply = peer->sendPeer(peer->Client::saveAndGetMessage(peer->downloadImage(imageName),
                                                                                 Message::MessageType::Request,
                                                                                 Message::OperationType::DOWNLOAD_IMAGE),
                                                 username);
                    if (reply == nullptr) {
                        std::cout << "Peer didn't respond" << std::endl;
                    } else {
                        auto downloadImageReply = load<DownloadImageReply>(reply->getMessage());
                        if (!downloadImageReply.isFlag()) {
                            peer->myCache.insertImage(username, downloadImageReply.getImage(), imageName);
                            std::cout << "Download Reply: " << !downloadImageReply.isFlag() << std::endl;
                        } else {
                            std::cout << "download Reply: " << downloadImageReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::UpdateLimit: {
                    std::string imageName, username;
                    int newLimit;
                    std::cout << "Enter target username:" << std::endl;
                    std::cin >> username;
                    std::cout << "Enter Image Name: " << std::endl;
                    std::cin >> imageName;
                    std::cout << "Enter the new number of views: " << std::endl;
                    std::cin >> newLimit;
                    auto *reply = peer->sendPeer(
                            peer->Client::saveAndGetMessage(peer->updateLimit(imageName, username, newLimit),
                                                            Message::MessageType::Request,
                                                            Message::OperationType::UPDATE_VIEW_LIMIT), username);
                    if (reply == nullptr) {
                        std::cout << "Peer didn't respond" << std::endl;
                    } else {
                        auto updateLimitReply = load<UpdateLimitReply>(reply->getMessage());
                        if (!updateLimitReply.isFlag()) {
                            std::cout << "Update Limit Reply: " << !updateLimitReply.isFlag() << std::endl;
                        } else {
                            std::cout << "Update Limit Reply: " << updateLimitReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::RemoveViewer: {
                    std::string imageName, username;
                    std::cout << "Enter target username:" << std::endl;
                    std::cin >> username;
                    std::cout << "Enter Image Name: " << std::endl;
                    std::cin >> imageName;
                    auto *reply = peer->sendPeer(
                            peer->Client::saveAndGetMessage(peer->removeViewer(imageName, username),
                                                            Message::MessageType::Request,
                                                            Message::OperationType::REMOVE_VIEWER), username);
                    if (reply == nullptr) {
                        std::cout << "Peer didn't respond" << std::endl;
                    } else {
                        auto updateLimitReply = load<RemoveViewerReply>(reply->getMessage());
                        if (!updateLimitReply.isFlag()) {
                            std::cout << "Remove Viewer Reply: " << !updateLimitReply.isFlag() << std::endl;
                        } else {
                            std::cout << "Remove Viewer Reply: " << updateLimitReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::GetOtherRemainingViews: {
                    std::string imageName, username;
                    std::cout << "Please enter username" << std::endl;
                    std::cin >> username;
                    std::cout << "Please input image name" << std::endl;
                    std::cin >> imageName;
                    auto *reply = peer->sendPeer(
                            peer->Client::saveAndGetMessage(peer->getRemainingViews(imageName, username),
                                                            Message::MessageType::Request,
                                                            Message::OperationType::GET_REMAINING_VIEWS), username);

                    if (reply == nullptr) {
                        std::cout << "DoS didn't respond" << std::endl;
                    } else {
                        auto downloadImageReply = load<GetRemainingViewsReply>(reply->getMessage());
                        if (!downloadImageReply.isFlag()) {
                            std::cout << "Remaining Views: " << downloadImageReply.getViewNum() << std::endl;
                            std::cout << "Get Remaining Views Reply: " << !downloadImageReply.isFlag() << std::endl;
                        } else {
                            std::cout << "Get Remaining Views Reply: " << downloadImageReply.getMsg() << std::endl;
                        }
                    }
                }
                    break;
                case peerChoices::GetMyRemainingViews: {
                    std::string imageName, ownerName;
                    std::cout << "Please input owner name" << std::endl;
                    std::cin >> ownerName;
                    std::cout << "Please input image name" << std::endl;
                    std::cin >> imageName;
                    std::cout << "# of views left:" << peer->getMyRemainingViews(imageName, ownerName) << std::endl;
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
    request.setThumbnail(this->createThumbnail(imagePath, imageName));
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

std::string Peer::createThumbnail(const std::string &imagePath, const std::string &imageName) {
    namespace bg = boost::gil;

//    std::vector<std::string> pathList;
//
//    boost::split(imagePath, pathList, boost::is_any_of("."));
//    if(pathList[1] == "jpeg" || pathList[1] == "jpg") {
    std::string temp_path = TEMP_DIR;
    std::string image_path = IMAGE_DIR;
    temp_path += "resize.jpg";
    image_path += imageName;
    bg::rgb8_image_t img;
    bg::read_image(imagePath, img, bg::jpeg_tag{});
    bg::rgb8_image_t square100(250, 250);
    bg::resize_view(bg::const_view(img), bg::view(square100), bg::bilinear_sampler{});
    bg::write_view(std::string(temp_path), bg::const_view(square100), bg::jpeg_tag{});
    bg::write_view(std::string(image_path), bg::const_view(img), bg::jpeg_tag{});
    std::ifstream in;
    in.open(temp_path);
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
        if (peer->Client::send(message)) {
            Message *reply = peer->Client::receiveWithTimeout();
            if (reply != nullptr) {
                Ack echoReply = load<Ack>(reply->getMessage());
            }
        }
        boost::this_thread::sleep(boost::posix_time::minutes(1));
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

const std::string &Peer::getPeerHostname() const {
    return peerHostname;
}

void Peer::setPeerHostname(const std::string &peerHostname) {
    Peer::peerHostname = peerHostname;
}

int Peer::getPeerPort() const {
    return peerPort;
}

void Peer::setPeerPort(int peerPort) {
    Peer::peerPort = peerPort;
}

FeedProfileRequest Peer::feedProfile(const std::string &username, int imageNum) {
    FeedProfileRequest request = FeedProfileRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setTargetUsername(username);
    if (this->profileIndices.find(username) != this->profileIndices.end()) {
        request.setLastIndex(this->profileIndices[username]);
    } else {
        this->profileIndices[username] = 0;
        request.setLastIndex(this->profileIndices[username]);

    }
    request.setImageNum(imageNum);
    return request;
}

ShowOnlineRequest Peer::showOnline() {
    ShowOnlineRequest request = ShowOnlineRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    return request;
}

ViewImageRequest Peer::viewImage(const std::string &username, const std::string &imageName, int viewNum) {
    ViewImageRequest request = ViewImageRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setTargetUsername(username);
    request.setImageName(imageName);
    request.setViewNum(viewNum);
    return request;
}

void Peer::addProfileIndex(const std::string &username, int index) {
    this->profileIndices[username] = index;
}

AddViewerRequest Peer::acceptRequest(const std::string &imageName, const std::string &viewerName, int viewNum) {
    AddViewerRequest request = AddViewerRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    std::ifstream in;
    in.open(IMAGE_DIR + imageName);
    if (in.is_open()) {
        ImageBody imageBody = ImageBody(this->username, viewerName, viewNum);
        std::string image((std::istreambuf_iterator<char>(in)),
                          std::istreambuf_iterator<char>());
        myCache.insertImage(viewerName, image, imageName, imageBody);
    }
    in.close();
    request.setImageName(imageName);
    request.setViewerName(viewerName);
    request.setViewNum(viewNum);
    return request;
}

DenyViewerRequest Peer::denyRequest(const std::string &imageName, const std::string &viewerName, int viewNum) {
    DenyViewerRequest request = DenyViewerRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageName(imageName);
    request.setViewerName(viewerName);
    request.setViewNum(viewNum);
    return request;
}

GetPendingRequests Peer::getPendingRequests() {
    GetPendingRequests request = GetPendingRequests();
    request.setUserName(this->username);
    request.setToken(this->token);
    return request;
}

const std::map<std::string, int> &Peer::getProfileIndices() const {
    return profileIndices;
}

void Peer::setProfileIndices(const std::map<std::string, int> &profileIndices) {
    Peer::profileIndices = profileIndices;
}

const Cache &Peer::getMyCache() const {
    return myCache;
}

void Peer::setMyCache(const Cache &myCache) {
    Peer::myCache = myCache;
}

DownloadImageReply Peer::sendImage(DownloadImageRequest req) {
    DownloadImageReply reply = DownloadImageReply();
    std::string image = this->myCache.getImage(req.getUserName(), req.getImageName());
    reply.setImageName(req.getImageName());
    reply.setImage(image);
    reply.setFlag(false);
    return reply;
}

UpdateLimitReply Peer::updateLimit(UpdateLimitRequest req) {
    UpdateLimitReply reply = UpdateLimitReply();
    ImageBody imageBody = this->myCache.getImageBody(req.getUserName(), req.getName());
    imageBody.setRemainingViews(req.getNewLimit());
    myCache.updateImage(req.getUserName(), req.getName(), imageBody);
    reply.setFlag(false);
    return reply;
}

RemoveViewerReply Peer::removeViewer(RemoveViewerRequest req) {
    RemoveViewerReply reply = RemoveViewerReply();
    this->myCache.removeImage(req.getUserName() + "-" + req.getImageName());
    reply.setFlag(false);
    return reply;
}

GetRemainingViewsReply Peer::getRemainingViews(GetRemainingViewsRequest req) {
    GetRemainingViewsReply reply = GetRemainingViewsReply();
    reply.setImageName(req.getImageName());
    reply.setViewNum(this->myCache.getImageBody(req.getUserName(), req.getImageName()).getRemainingViews());
    reply.setFlag(false);
    return reply;
}

int Peer::getMyRemainingViews(const std::string &imageName, const std::string &username) {
    return this->myCache.getImageBody(username, imageName).getRemainingViews();
}

RemoveViewerRequest Peer::getRemainingViews(const std::string &imageName, const std::string &username) {
    RemoveViewerRequest request = RemoveViewerRequest();
    request.setUserName(this->username);
    request.setToken(this->token);
    request.setImageName(imageName);
    request.setToRemove(username);
    return request;
}


#pragma clang diagnostic pop