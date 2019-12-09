#include "../headers/DirectoryServer.h"
#include "../headers/MessageStructures.h"
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#define ERROR_AUTH "Failed to authenticate credentials"
#define USER_NOT_FOUND "User does not exist"
#define IMAGE_EXISTS "Image already exists"
#define IMAGE_DOES_NOT_EXIST "Image does not exist for this user"

DirectoryServer::DirectoryServer(const std::string &hostname) : Server(hostname, DEFAULT_LISTEN_PORT) {
    this->hostname = hostname;
    this->port = DEFAULT_LISTEN_PORT;
    this->databasePath = DATABASE_DIR;
    this->directoryFile = DIRECTORY_FILE;
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
}

void DirectoryServer::loadDatabase() {
    std::ifstream in;
    in.open(databasePath + usersFile);
    std::string username;
    std::string password;
    while (in >> username >> password) {
        User user = User();
        user.setUsername(username);
        user.setPassword(password);
        user.setAuthenticated(false);
        this->users[username] = user;
    }
    in.open(databasePath + directoryFile);
    std::string imageList;
    while (in >> username >> imageList) {
        std::vector<std::string> images;
        boost::split(images, imageList, boost::is_any_of(", "));
        this->users[username].setImages(images);
    }
    in.close();
}

void DirectoryServer::saveDatabase() {
    std::ofstream out;
    out.open(databasePath + usersFile);
    for (const User &user : this->users | boost::adaptors::map_values) {
        out << user.getUsername() << user.getPassword();
    }
    out.open(databasePath + directoryFile);
    for (const User &user : this->users | boost::adaptors::map_values) {
        std::string imageList = boost::algorithm::join(user.getImages(), ", ");
        out << user.getUsername() << imageList;
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

void DirectoryServer::handleRequest(Message message) {
    switch (message.getMessageType()) {
        case Message::MessageType::Request:
            Message *reply;
            switch (message.getOperation()) {
                case Message::OperationType::REGISTER:
                    reply = this->saveAndGetMessage(this->registerUser(load<RegisterRequest>(message.getMessage())),
                                                    Message::MessageType::Reply, Message::OperationType::REGISTER);
                    break;
                case Message::OperationType::LOGIN:
                    reply = this->saveAndGetMessage(this->loginUser(load<LoginRequest>(message.getMessage())),
                                                    Message::MessageType::Reply, Message::OperationType::LOGIN);
                    break;
                case Message::OperationType::LOGOUT:
                    reply = this->saveAndGetMessage(this->logoutUser(load<LogoutRequest>(message.getMessage())),
                                                    Message::MessageType::Reply, Message::OperationType::LOGOUT);
                    break;
                case Message::OperationType::FEED:
                    reply = this->saveAndGetMessage(this->feed(load<FeedRequest>(message.getMessage())),
                                                    Message::MessageType::Reply, Message::OperationType::FEED);
                    break;
                case Message::OperationType::SEARCH:
                    reply = this->saveAndGetMessage(this->searchUser(load<SearchRequest>(message.getMessage())),
                                                    Message::MessageType::Reply, Message::OperationType::SEARCH);
                    break;
                case Message::OperationType::ADD_IMAGE:
                    reply = this->saveAndGetMessage(this->addImage(load<AddImageRequest>(message.getMessage())),
                                                    Message::MessageType::Reply, Message::OperationType::ADD_IMAGE);
                    break;
                case Message::OperationType::DELETE_IMAGE:
                    reply = this->saveAndGetMessage(this->delImage(load<DeleteImageRequest>(message.getMessage())),
                                                    Message::MessageType::Reply, Message::OperationType::DELETE_IMAGE);
                    break;
                case Message::OperationType::ECHO:
                    reply = this->saveAndGetMessage(load<Echo>(message.getMessage()), Message::MessageType::Reply,
                                                    Message::OperationType::ECHO);
                    break;
                case Message::OperationType::HELLO:
                    reply = this->saveAndGetMessage(this->handleHello(load<Hello>(message.getMessage())),Message::MessageType::Reply, Message::OperationType::HELLO);
                    break;
                default:
                    break;
            }
            this->sendReply(reply);
            break;
        case Message::MessageType::Reply:
            switch (message.getOperation()) {
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
    Ack reply = Ack();
    const std::string &username = req.getUserName();
    const std::string &token = req.getToken();
    if (this->userExists(username)) {
        if (this->authorize(username, token)) {
            reply.setFlag(false);

        } else {
            reply.setFlag(true);
            reply.setMsg(ERROR_AUTH);
        }
    } else {
        reply.setFlag(true);
        reply.setMsg(USER_NOT_FOUND);
    }
    return Ack();
}

SearchReply DirectoryServer::searchUser(const SearchRequest &req) {
    SearchReply reply = SearchReply();
    const std::string &username = req.getUserName();
    const std::string &token = req.getToken();
    if (this->authorize(username, token)) {
        const std::string &targetUsername = req.getTargetUsername();
        if (this->userExists(targetUsername)) {
            std::string address = this->users[targetUsername].getAddress();
            int portNum = this->users[targetUsername].getPortNum();
            //TODO::get user address
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
        reply.setMsg("Incorrect username or password.");
        reply.setFlag(true);
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
        reply.setMsg("User registered successfully.");
    } else {
        reply.setRegistered(false);
        reply.setFlag(true);
        reply.setMsg("User already exists.");
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
                out.open(THUMBNAILS_DIR + username + "-" + imageName);
                out << req.getThumbnail();
                out.close();
                reply.setFlag(false);
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

    if (this->userExists(username)) {
        if (this->authorize(username, token)) {
            for (const User &user : this->users | boost::adaptors::map_values) {
                for (const std::string &image : user.getImages()) {
                    //TODO::FEED
                }
            }
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

DirectoryServer::~DirectoryServer() {
    //this->saveDatabase();
}

void DirectoryServer::listen() {
    while (true) {
        Message *message = this->getRequest();
        this->handleRequest(*message);
    }
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
