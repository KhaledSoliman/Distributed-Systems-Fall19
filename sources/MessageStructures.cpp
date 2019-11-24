#include "../headers/MessageStructures.h"
#include "../headers/Message.h"

using namespace MessageStructures;
using namespace MessageStructures::Control;
using namespace MessageStructures::User;
using namespace MessageStructures::User::Authentication;

template<class Archive>
void Error::serialize(Archive &ar, const unsigned int version) {
    ar & flag;
    ar & msg;
}

bool Error::isFlag() const {
    return flag;
}

void Error::setFlag(bool flag) {
    this->flag = flag;
}

const std::string &Error::getMsg() const {
    return msg;
}

void Error::setMsg(const std::string &msg) {
    this->msg = msg;
}

template<class Archive>
void realSockAddr::serialize(Archive &ar, const unsigned int version) {
    ar & this->sa_data;
    ar & this->sa_family;
    //ar & this->sa_len;
}

realSockAddr::realSockAddr() {

}

realSockAddr::realSockAddr(const realSockAddr &other) {
    memcpy(sa_data, other.sa_data, sizeof(other.sa_data));
    sa_family = other.sa_family;
    //sa_len = other.sa_len;
}

realSockAddr::realSockAddr(const sockaddr &s) : sockaddr(s) {

}

template<class Archive>
void AuthRequest::serialize(Archive &ar, const unsigned int version) {
    ar & userName;
    ar & token;
}

const std::string &AuthRequest::getUserName() const {
    return userName;
}

void AuthRequest::setUserName(const std::string &userName) {
    this->userName = userName;
}

const std::string &AuthRequest::getToken() const {
    return token;
}

void AuthRequest::setToken(const std::string &token) {
    this->token = token;
}

template<class Archive>
void LoginRequest::serialize(Archive &ar, const unsigned int version) {
    ar & userName;
    ar & hashedPassword;
}

const std::string &LoginRequest::getUserName() const {
    return userName;
}

void LoginRequest::setUserName(const std::string &userName) {
    this->userName = userName;
}

const std::string &LoginRequest::getHashedPassword() const {
    return hashedPassword;
}

void LoginRequest::setHashedPassword(const std::string &hashedPassword) {
    this->hashedPassword = hashedPassword;
}

template<class Archive>
void LoginReply::serialize(Archive &ar, const unsigned int version) {
    ar & token;
}

const std::string &LoginReply::getToken() const {
    return token;
}

void LoginReply::setToken(const std::string &token) {
    this->token = token;
}

template<class Archive>
void RegisterRequest::serialize(Archive &ar, const unsigned int version) {
    ar & user_name;
    ar & hashed_password;
}

const std::string &RegisterRequest::getUserName() const {
    return user_name;
}

void RegisterRequest::setUserName(const std::string &userName) {
    user_name = userName;
}

const std::string &RegisterRequest::getHashedPassword() const {
    return hashed_password;
}

void RegisterRequest::setHashedPassword(const std::string &hashedPassword) {
    hashed_password = hashedPassword;

}

template<class Archive>
void RegisterReply::serialize(Archive &ar, const unsigned int version) {
    ar & registered;
}

bool RegisterReply::isRegistered() const {
    return registered;
}

void RegisterReply::setRegistered(bool registered) {
    this->registered = registered;
}

template<class Archive>
void AddImageRequest::serialize(Archive &ar, const unsigned int version) {
    ar & image_name;
    ar & img;
    ar & viewer;
    ar & limit;
}

const std::string &AddImageRequest::getImageName() const {
    return image_name;
}

void AddImageRequest::setImageName(const std::string &imageName) {
    image_name = imageName;
}

const std::vector<unsigned char> &AddImageRequest::getImg() const {
    return img;
}

void AddImageRequest::setImg(const std::vector<unsigned char> &img) {
    this->img = img;
}

const std::string &AddImageRequest::getViewer() const {
    return viewer;
}

void AddImageRequest::setViewer(const std::string &viewer) {
    this->viewer = viewer;
}

int AddImageRequest::getLimit() const {
    return limit;
}

void AddImageRequest::setLimit(int limit) {
    this->limit = limit;
}

template<class Archive>
void ViewImageRequest::serialize(Archive &ar, const unsigned int version) {
    ar & userName;
    ar & imageName;
}

const std::string &ViewImageRequest::getUserName() const {
    return userName;
}

void ViewImageRequest::setUserName(const std::string &userName) {
    this->userName = userName;
}

const std::string &ViewImageRequest::getImageName() const {
    return imageName;
}

void ViewImageRequest::setImageName(const std::string &imageName) {
    this->imageName = imageName;
}

template<class Archive>
void ViewImageReply::serialize(Archive &ar, const unsigned int version) {
    ar & image;
}

const Image &ViewImageReply::getImage() const {
    return image;
}

void ViewImageReply::setImage(const Image &image) {
    this->image = image;
}

template<class Archive>
void DeleteImageRequest::serialize(Archive &ar, const unsigned int version) {
    ar & userName;
    ar & imageName;
}

const std::string &DeleteImageRequest::getUserName1() const {
    return userName;
}

void DeleteImageRequest::setUserName1(const std::string &userName) {
    this->userName = userName;
}

const std::string &DeleteImageRequest::getImageName() const {
    return imageName;
}

void DeleteImageRequest::setImageName(const std::string &imageName) {
    this->imageName = imageName;
}

template<class Archive>
void AddViewerRequest::serialize(Archive &ar, const unsigned int version) {
    ar & userName;
    ar & imageName;
    ar & viewerName;
}

const std::string &AddViewerRequest::getUserName1() const {
    return userName;
}

void AddViewerRequest::setUserName1(const std::string &userName) {
    this->userName = userName;
}

const std::string &AddViewerRequest::getImageName() const {
    return imageName;
}

void AddViewerRequest::setImageName(const std::string &imageName) {
    this->imageName = imageName;
}

void AddViewerRequest::setViewerName(const std::string &viewerName) {
    this->viewerName = viewerName;
}

const std::string &AddViewerRequest::getViewerName() const {
    return viewerName;
}

template<class Archive>
void RemoveViewerRequest::serialize(Archive &ar, const unsigned int version) {
    ar & token;
    ar & name;
    ar & to_remove;
}

const std::string &RemoveViewerRequest::getToken1() const {
    return token;
}

void RemoveViewerRequest::setToken1(const std::string &token) {
    RemoveViewerRequest::token = token;
}

const std::string &RemoveViewerRequest::getName() const {
    return name;
}

void RemoveViewerRequest::setName(const std::string &name) {
    this->name = name;
}

const std::vector<std::string> &RemoveViewerRequest::getToRemove() const {
    return to_remove;
}

void RemoveViewerRequest::setToRemove(const std::vector<std::string> &toRemove) {
    to_remove = toRemove;
}

template<class Archive>
void UpdateLimitRequest::serialize(Archive &ar, const unsigned int version) {
    ar & token;
    ar & name;
    ar & newLimit;
}

const std::string &UpdateLimitRequest::getToken1() const {
    return token;
}

void UpdateLimitRequest::setToken1(const std::string &token) {
    UpdateLimitRequest::token = token;
}

const std::string &UpdateLimitRequest::getName() const {
    return name;
}

void UpdateLimitRequest::setName(const std::string &name) {
    this->name = name;
}

int UpdateLimitRequest::getNewLimit() const {
    return newLimit;
}

void UpdateLimitRequest::setNewLimit(int newLimit) {
    this->newLimit = newLimit;
}

template<class Archive>
void SearchReply::serialize(Archive &ar, const unsigned int version) {
    ar & users_ips;
}

template<class Archive>
void SendMessageRequest::serialize(Archive &ar, const unsigned int version) {
    ar & message;
}

const std::string &SendMessageRequest::getMessage() const {
    return message;
}

void SendMessageRequest::setMessage(const std::string &message) {
    this->message = message;
}

template<typename T>
std::string save(const T &t) {
    std::ostringstream archiveStream;
    boost::archive::text_oarchive archive(archiveStream, boost::archive::no_header);
    archive << t;
    return archiveStream.str();
}

template<typename T>
T load(const std::string &message) {
    T t;
    std::stringstream archiveStream;
    archiveStream << message;
    boost::archive::text_iarchive archive(archiveStream, boost::archive::no_header);
    archive >> t;
    return t;
}

#define OBJECT_SERIALIZATION(T) template std::string save(const T &t); template T load(const std::string &message);
//Register
OBJECT_SERIALIZATION(RegisterRequest)

OBJECT_SERIALIZATION(RegisterReply)
//Login
OBJECT_SERIALIZATION(LoginRequest)

OBJECT_SERIALIZATION(LoginReply)
//Logout
OBJECT_SERIALIZATION(LogoutRequest)

OBJECT_SERIALIZATION(LogoutReply)
//Add Image
OBJECT_SERIALIZATION(AddImageRequest)

OBJECT_SERIALIZATION(AddImageReply)
//Delete Image
OBJECT_SERIALIZATION(DeleteImageRequest)

OBJECT_SERIALIZATION(DeleteImageReply)
//Add Viewer
OBJECT_SERIALIZATION(AddViewerRequest)

OBJECT_SERIALIZATION(AddViewerReply)
//Remove Viewer
OBJECT_SERIALIZATION(RemoveViewerRequest)

OBJECT_SERIALIZATION(RemoveViewerReply)
//Update Limit
OBJECT_SERIALIZATION(UpdateLimitRequest)

OBJECT_SERIALIZATION(UpdateLimitReply)
//Search
OBJECT_SERIALIZATION(SearchRequest)

OBJECT_SERIALIZATION(SearchReply)
//Search
OBJECT_SERIALIZATION(SendMessageRequest)

OBJECT_SERIALIZATION(Message)


