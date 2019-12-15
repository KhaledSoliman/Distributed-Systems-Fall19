#include "../headers/DirectoryServer.h"
#include "../headers/MessageStructures.h"
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/thread.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#define ERROR_AUTH "Failed to authenticate credentials"
#define USER_NOT_FOUND "User does not exist"
#define USER_EXISTS "User already exists"
#define IMAGE_EXISTS "Image already exists"
#define IMAGE_DOES_NOT_EXIST "Image does not exist for this user"
#define LOGIN_FAILURE "Incorrect username or password"
#define REGISTER_SUCCESS "User registered successfully."
#define THUMBNAIL_SAVE_ERR "Couldnt save thumbnail"


DirectoryServer::DirectoryServer(const std::string &hostname) : Server(hostname, DEFAULT_LISTEN_PORT) {
    this->hostname = hostname;
    this->port = DEFAULT_LISTEN_PORT;
    this->databasePath = DATABASE_DIR;
    this->directoryFile = DIRECTORY_FILE;
    this->usersFile = USER_FILE;
}

DirectoryServer::DirectoryServer(const std::string &hostname, int port, const std::string &databasePath,
                                 const std::string &directoryFile, const std::string &usersFile) : Server(hostname,
                                                                                                          port) {
    this->hostname = hostname;
    this->port = port;
    this->usersFile = usersFile;
    this->databasePath = databasePath;
    this->directoryFile = directoryFile;
}

void DirectoryServer::init() {

    this->loadDatabase();
    boost::thread helloListener(&DirectoryServer::helloListener, boost::shared_ptr<DirectoryServer>(this));
    boost::thread serverListener(&DirectoryServer::listen, boost::shared_ptr<DirectoryServer>(this));
    boost::thread databasePersist(&DirectoryServer::databasePersistence, boost::shared_ptr<DirectoryServer>(this));
    while (true);
}

void DirectoryServer::databasePersistence(boost::shared_ptr<DirectoryServer> directoryServer) {
    for (;;) {
        std::cout << "saving databases..." << std::endl;
        directoryServer->saveDatabase();
        boost::this_thread::sleep(boost::posix_time::minutes(1));
    }
}

void DirectoryServer::listen(boost::shared_ptr<DirectoryServer> directoryServer) {
    while (true) {
        Message *message = directoryServer->Server::receive();
        std::cout << "Recieved: " << message->getOperation() << std::endl;
        boost::thread serverThread(&DirectoryServer::handleRequest, message, directoryServer);
        if (message->getOperation() == Message::OperationType::FEED ||
            message->getOperation() == Message::OperationType::FEED_PROFILE)
            boost::this_thread::sleep(boost::posix_time::seconds(5));
    }
}

void DirectoryServer::helloListener(boost::shared_ptr<DirectoryServer> directoryServer) {
    directoryServer->Server::initBroadcast(BROADCAST_PORT);

    while (true) {
        Message *message = directoryServer->listenToBroadcasts();
        auto hello = load<Hello>(message->getMessage());
        if (hello.getMessage() == "DoS")
            hello.setMessage("ME");
        Message *reply = directoryServer->Server::saveAndGetMessage(hello, Message::MessageType::Reply,
                                                                    Message::OperationType::HELLO);
        directoryServer->Server::send(reply);
    }
}

void DirectoryServer::loadDatabase() {
    std::ifstream in;
    std::string username;
    in.open(databasePath + usersFile);
    if (in.is_open()) {
        std::string line;
        std::string password;
        while (std::getline(in, line) && !line.empty()) {
            std::stringstream lineStream(line);
            while (lineStream >> username >> password) {
                User user = User();
                user.setUsername(username);
                user.setPassword(password);
                user.setAuthenticated(false);
                this->users[username] = user;
            }
        }
    }
    in.close();
    in.open(databasePath + directoryFile);
    if (in.is_open()) {
        std::string imageList;
        std::string line;
        while (std::getline(in, line) && !line.empty()) {
            std::stringstream lineStream(line);
            while (lineStream >> username >> imageList) {
                if (imageList != "none") {
                    std::vector<std::string> images;
                    boost::split(images, imageList, boost::is_any_of(","));
                    this->users[username].setImages(images);
                }
            }
        }
        in.close();
    }
}

    void DirectoryServer::saveDatabase() {
        std::ofstream out;
        out.open(databasePath + usersFile);
        if (out.is_open())
            for (const User &user : this->users | boost::adaptors::map_values) {
                out << user.getUsername() << " " << user.getPassword() << std::endl;
            }
        out.close();

        out.open(databasePath + directoryFile);
        if (out.is_open())
            for (const User &user : this->users | boost::adaptors::map_values) {
                std::string imageList;
                if (!user.getImages().empty()) {
                    imageList = boost::algorithm::join(user.getImages(), ",");
                } else {
                    imageList = "none";
                }
                out << user.getUsername() << " " << imageList << std::endl;
            }
        out.close();
    }

    bool DirectoryServer::userExists(const std::string &username) {
        return this->users.find(username) != this->users.end();
    }

    bool DirectoryServer::authorize(const std::string &username, const std::string &token) {
        return this->users[username].isAuthenticated() && this->users[username].getToken() == token;
    }

    std::string DirectoryServer::generateAuthToken() {
        std::string chars(
                "abcdefghijklmnopqrstuvwxyz"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "1234567890");
        boost::random::random_device rng;
        boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
        std::string token;
        for (int i = 0; i < 256; ++i) {
            token += chars[index_dist(rng)];
        }
        return token;
    }

    bool DirectoryServer::authenticate(const std::string &username, const std::string &hashedPassword) {
        return this->userExists(username) && this->users[username].getPassword() == hashedPassword;
    }

    void DirectoryServer::handleRequest(Message *message, boost::shared_ptr<DirectoryServer> directoryServer) {
        switch (message->getMessageType()) {
            case Message::MessageType::Request:
                Message *reply;
                switch (message->getOperation()) {
                    case Message::OperationType::REGISTER:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->registerUser(load<RegisterRequest>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::REGISTER);
                        break;
                    case Message::OperationType::LOGIN:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->loginUser(load<LoginRequest>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::LOGIN);
                        break;
                    case Message::OperationType::LOGOUT:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->logoutUser(load<LogoutRequest>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::LOGOUT);
                        break;
                    case Message::OperationType::SHOW_ONLINE:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->showOnline(load<ShowOnlineRequest>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::SHOW_ONLINE);
                        break;
                    case Message::OperationType::FEED:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->feed(load<FeedRequest>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::FEED);
                        break;
                    case Message::OperationType::FEED_PROFILE:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->feedProfile(load<FeedProfileRequest>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::FEED_PROFILE);
                        break;
                    case Message::OperationType::SEARCH:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->searchUser(load<SearchRequest>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::SEARCH);
                        break;
                    case Message::OperationType::ADD_IMAGE:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->addImage(load<AddImageRequest>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::ADD_IMAGE);
                        break;
                    case Message::OperationType::DELETE_IMAGE:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->delImage(load<DeleteImageRequest>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::DELETE_IMAGE);
                        break;
                    case Message::OperationType::VIEW_IMAGE:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->viewImage(load<ViewImageRequest>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::VIEW_IMAGE);
                        break;
                    case Message::OperationType::GET_REQUESTS:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->getRequests(load<GetRequests>(message->getMessage())),
                                Message::MessageType::Reply, Message::OperationType::GET_REQUESTS);
                        break;
                    case Message::OperationType::ECHO:
                        reply = directoryServer->Server::saveAndGetMessage(load<Echo>(message->getMessage()),
                                                                           Message::MessageType::Reply,
                                                                           Message::OperationType::ECHO);
                        break;
                    case Message::OperationType::HELLO:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->handleHello(load<Hello>(message->getMessage())),
                                Message::MessageType::Reply,
                                Message::OperationType::ACK);
                        break;
                    case Message::OperationType::AUTH_HELLO:
                        reply = directoryServer->Server::saveAndGetMessage(
                                directoryServer->handleAuthHello(load<AuthenticatedHello>(message->getMessage())),
                                Message::MessageType::Reply,
                                Message::OperationType::ACK);
                        break;
                    default:
                        break;
                }
                directoryServer->Server::send(reply);
                break;
            case Message::MessageType::Reply:
                switch (message->getOperation()) {
                    case Message::OperationType::ACK:
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    Ack DirectoryServer::handleHello(Hello req) {
        return Ack();
    }

    SearchReply DirectoryServer::searchUser(const SearchRequest &req) {
        SearchReply reply = SearchReply();
        const std::string &username = req.getUserName();
        const std::string &token = req.getToken();
        if (this->authorize(username, token)) {
            const std::string &targetUsername = req.getTargetUsername();
            if (this->userExists(targetUsername)) {
                reply.setAddress(this->users[targetUsername].getAddress());
                reply.setPort(this->users[targetUsername].getPortNum());
                reply.setFlag(false);
            } else {
                reply.setFlag(true);
                reply.setMsg(USER_NOT_FOUND);
            }
        } else {
            reply.setFlag(true);
            reply.setMsg(ERROR_AUTH);
        }
        return reply;
    }

    LoginReply DirectoryServer::loginUser(const LoginRequest &req) {
        LoginReply reply = LoginReply();
        const std::string &username = req.getUserName();
        const std::string &password = req.getHashedPassword();
        bool authenticated = this->authenticate(username, password);
        if (authenticated) {
            std::string token = this->generateAuthToken();
            this->users[username].setToken(token);
            this->users[username].setAuthenticated(true);
            reply.setFlag(false);
            reply.setToken(token);
        } else {
            reply.setFlag(true);
            reply.setMsg(LOGIN_FAILURE);
        }

        return reply;
    }

    LogoutReply DirectoryServer::logoutUser(const LogoutRequest &req) {
        LogoutReply reply = LogoutReply();
        const std::string &username = req.getUserName();
        const std::string &token = req.getToken();
        if (this->userExists(username)) {
            if (this->authorize(username, token)) {
                this->users[username].setToken("");
                this->users[username].setAuthenticated(false);
                reply.setFlag(false);
            } else {
                reply.setFlag(true);
                reply.setMsg(ERROR_AUTH);
            }
        } else {
            reply.setFlag(true);
            reply.setMsg(USER_NOT_FOUND);
        }
        return reply;
    }

    RegisterReply DirectoryServer::registerUser(const RegisterRequest &req) {
        const std::string &username = req.getUserName();
        const std::string &password = req.getHashedPassword();
        RegisterReply reply = RegisterReply();
        if (!this->userExists(username)) {
            User user = User();
            user.setUsername(username);
            user.setPassword(password);
            user.setAuthenticated(false);
            this->users[username] = user;
            reply.setRegistered(true);
            reply.setFlag(false);
            reply.setMsg(REGISTER_SUCCESS);
        } else {
            reply.setRegistered(false);
            reply.setFlag(true);
            reply.setMsg(USER_EXISTS);
        }
        return reply;
    }

    AddImageReply DirectoryServer::addImage(const AddImageRequest &req) {
        AddImageReply reply = AddImageReply();
        const std::string &username = req.getUserName();
        const std::string &token = req.getToken();
        if (this->userExists(username)) {
            if (this->authorize(username, token)) {
                const std::string &imageName = req.getImageName();
                if (!this->users[username].imageExists(imageName)) {
                    this->users[username].addImage(imageName);
                    std::ofstream out;
                    std::string path = THUMBNAILS_DIR + username + "-" + imageName;
                    out.open(path);
                    if (out.is_open()) {
                        out << req.getThumbnail();
                        out.close();
                        reply.setFlag(false);
                    } else {
                        reply.setFlag(true);
                        reply.setMsg(THUMBNAIL_SAVE_ERR);
                    }
                } else {
                    reply.setFlag(true);
                    reply.setMsg(IMAGE_EXISTS);
                }
            } else {
                reply.setFlag(true);
                reply.setMsg(ERROR_AUTH);
            }
        } else {
            reply.setFlag(true);
            reply.setMsg(USER_NOT_FOUND);
        }
        return reply;
    }

    DeleteImageReply DirectoryServer::delImage(const DeleteImageRequest &req) {
        DeleteImageReply reply = DeleteImageReply();
        const std::string &username = req.getUserName();
        const std::string &token = req.getToken();
        if (this->userExists(username)) {
            if (this->authorize(username, token)) {
                const std::string &imageName = req.getImageName();
                if (this->users[username].imageExists(imageName)) {
                    std::string path = THUMBNAILS_DIR + username + "-" + imageName;
                    remove(path.c_str());
                    this->users[username].delImage(imageName);
                    reply.setFlag(false);
                } else {
                    reply.setFlag(true);
                    reply.setMsg(IMAGE_DOES_NOT_EXIST);
                }
            } else {
                reply.setFlag(true);
                reply.setMsg(ERROR_AUTH);
            }
        } else {
            reply.setFlag(true);
            reply.setMsg(USER_NOT_FOUND);
        }
        return reply;
    }

    FeedReply DirectoryServer::feed(const FeedRequest &req) {
        FeedReply reply = FeedReply();
        const std::string &username = req.getUserName();
        const std::string &token = req.getToken();
        int index = 0;
        std::unordered_map<std::string, std::pair<std::string, std::string>> thumbnails;
        if (this->userExists(username)) {
            if (this->authorize(username, token)) {
                for (const User &user : this->users | boost::adaptors::map_values) {
                    if (user.getUsername() != username) {
                        for (const std::string &image : user.getImages()) {
                            if (index < req.getLastIndex()) {
                                index++;
                                continue;
                            }
                            std::ifstream in;
                            std::string path = THUMBNAILS_DIR + user.getUsername() + "-" + image;
                            in.open(path);
                            std::string thumbnail((std::istreambuf_iterator<char>(in)),
                                                  std::istreambuf_iterator<char>());
                            thumbnails[user.getUsername()] = std::pair(image, thumbnail);
                            in.close();
                            index++;
                        }
                    }
                }
                reply.setCurrentIndex(index);
                reply.setImages(thumbnails);
                reply.setFlag(false);
            } else {
                reply.setFlag(true);
                reply.setMsg(ERROR_AUTH);
            }
        } else {
            reply.setFlag(true);
            reply.setMsg(USER_NOT_FOUND);
        }
        return reply;
    }

    DirectoryServer::~DirectoryServer()
    {
        this->saveDatabase();
    }

    Ack DirectoryServer::handleAuthHello(AuthenticatedHello req) {
        const std::string &username = req.getUserName();
        const std::string &token = req.getToken();
        if (this->userExists(username)) {
            if (this->authorize(username, token)) {
                this->users[username].setAddress(req.getIpAddress());
                this->users[username].setPortNum(req.getPort());
            }
        }
        return Ack();
    }

    ShowOnlineReply DirectoryServer::showOnline(const ShowOnlineRequest &req) {
        ShowOnlineReply reply = ShowOnlineReply();
        const std::string &username = req.getUserName();
        const std::string &token = req.getToken();
        if (this->userExists(username)) {
            if (this->authorize(username, token)) {
                std::vector<std::string> onlineUsers;
                for (const User &user : this->users | boost::adaptors::map_values) {
                    if (user.getUsername() != username && user.isAuthenticated()) {
                        onlineUsers.push_back(user.getUsername());
                    }
                }
                reply.setFlag(false);
                reply.setUsers(onlineUsers);
            } else {
                reply.setFlag(true);
                reply.setMsg(ERROR_AUTH);
            }
        } else {
            reply.setFlag(true);
            reply.setMsg(USER_NOT_FOUND);
        }
        return reply;
    }

    FeedProfileReply DirectoryServer::feedProfile(const FeedProfileRequest &req) {
        FeedProfileReply reply = FeedProfileReply();
        const std::string &username = req.getUserName();
        const std::string &token = req.getToken();
        const std::string &targetUsername = req.getTargetUsername();
        if (this->userExists(username) && this->userExists(targetUsername)) {
            if (this->authorize(username, token)) {
                if (targetUsername != username) {
                    std::vector<std::pair<std::string, std::string>> thumbnails;
                    int index = 0;
                    for (const std::string &image : this->users[targetUsername].getImages()) {
                        if (index < req.getLastIndex()) {
                            index++;
                            continue;
                        }
                        std::ifstream in;
                        std::string path = THUMBNAILS_DIR + targetUsername + "-" + image;
                        in.open(path);
                        std::string thumbnail((std::istreambuf_iterator<char>(in)),
                                              std::istreambuf_iterator<char>());
                        thumbnails.push_back(std::pair(image, thumbnail));
                        in.close();
                        index++;
                    }
                    reply.setFlag(false);
                    reply.setTargetUsername(targetUsername);
                    reply.setImages(thumbnails);
                    reply.setCurrentIndex(index);
                } else {
                    reply.setFlag(true);
                    reply.setMsg("Cannot feed yourself.");
                }
            } else {
                reply.setFlag(true);
                reply.setMsg(ERROR_AUTH);
            }
        } else {
            reply.setFlag(true);
            reply.setMsg(USER_NOT_FOUND);
        }
        return reply;
    }

    ViewImageReply DirectoryServer::viewImage(const ViewImageRequest &req) {
        ViewImageReply reply = ViewImageReply();
        const std::string &username = req.getUserName();
        const std::string &token = req.getToken();
        const std::string &targetUsername = req.getTargetUsername();
        if (this->userExists(username) && this->userExists(targetUsername)) {
            if (this->authorize(username, token)) {
                this->users[targetUsername].addRequest(req);
                reply.setFlag(false);
            } else {
                reply.setFlag(true);
                reply.setMsg(ERROR_AUTH);
            }
        } else {
            reply.setFlag(true);
            reply.setMsg(USER_NOT_FOUND);
        }
        return reply;
    }

    GetRequestsReply DirectoryServer::getRequests(const GetRequests &req) {
        GetRequestsReply reply = GetRequestsReply();
        const std::string &username = req.getUserName();
        const std::string &token = req.getToken();
        if (this->userExists(username)) {
            if (this->authorize(username, token)) {
                reply.setFlag(false);
                reply.setRequests(this->users[username].getImageRequests());
            } else {
                reply.setFlag(true);
                reply.setMsg(ERROR_AUTH);
            }
        } else {
            reply.setFlag(true);
            reply.setMsg(USER_NOT_FOUND);
        }
        return reply;
    }

    const std::string &DirectoryServer::User::getUsername() const {
        return username;
    }

    void DirectoryServer::User::setUsername(const std::string &username) {
        this->username = username;
    }

    const std::string &DirectoryServer::User::getPassword() const {
        return password;
    }

    void DirectoryServer::User::setPassword(const std::string &password) {
        this->password = password;
    }

    const std::string &DirectoryServer::User::getAddress() const {
        return address;
    }

    void DirectoryServer::User::setAddress(const std::string &address) {
        this->address = address;
    }

    const std::string &DirectoryServer::User::getToken() const {
        return token;
    }

    void DirectoryServer::User::setToken(const std::string &token) {
        this->token = token;
    }

    bool DirectoryServer::User::isAuthenticated() const {
        return authenticated;
    }

    void DirectoryServer::User::setAuthenticated(bool isAuthenticated) {
        this->authenticated = isAuthenticated;
    }

    int DirectoryServer::User::getPortNum() const {
        return portNum;
    }

    void DirectoryServer::User::setPortNum(int portNum) {
        this->portNum = portNum;
    }

    const std::vector<std::string> &DirectoryServer::User::getImages() const {
        return images;
    }

    void DirectoryServer::User::setImages(const std::vector<std::string> &images) {
        this->images = images;
    }

    time_t DirectoryServer::User::getLastSeen() const {
        return lastSeen;
    }

    void DirectoryServer::User::setLastSeen(time_t lastSeen) {
        this->lastSeen = lastSeen;
    }

    void DirectoryServer::User::addImage(const std::string &imageName) {
        this->images.push_back(imageName);
    }

    bool DirectoryServer::User::imageExists(const std::string &imageName) {
        return std::find(this->images.begin(), this->images.end(), imageName) != this->images.end();
    }

    void DirectoryServer::User::addRequest(MessageStructures::User::ViewImageRequest request) {
        this->requests.push_back(request);
    }

    void DirectoryServer::User::delImage(const std::string &imageName) {
        images.erase(std::find(this->images.begin(), this->images.end(), imageName));
    }

    const std::vector<MessageStructures::User::ViewImageRequest> &DirectoryServer::User::getImageRequests() const {
        return requests;
    }

    void DirectoryServer::User::setRequests(const std::vector<MessageStructures::User::ViewImageRequest> &requests) {
        User::requests = requests;
    }

#pragma clang diagnostic pop