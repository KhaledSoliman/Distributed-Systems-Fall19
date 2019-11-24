#include "../headers/MessageStructures.h"
#include "../headers/Message.h"

template<class Archive>
void MessageStructures::Control::Error::serialize(Archive &ar, const unsigned int version) {
    ar & flag;
    ar & msg;
}

bool MessageStructures::Control::Error::isFlag() const {
    return flag;
}

void MessageStructures::Control::Error::setFlag(bool flag) {
    this->flag = flag;
}

const std::string &MessageStructures::Control::Error::getMsg() const {
    return msg;
}

void MessageStructures::Control::Error::setMsg(const std::string &msg) {
    this->msg = msg;
}

template<class Archive>
void MessageStructures::Control::realSockAddr::serialize(Archive &ar, const unsigned int version) {
    ar & this->sa_data;
    ar & this->sa_family;
    //ar & this->sa_len;
}

MessageStructures::Control::realSockAddr::realSockAddr() {

}

MessageStructures::Control::realSockAddr::realSockAddr(const MessageStructures::Control::realSockAddr &other) {
    memcpy(sa_data, other.sa_data, sizeof(other.sa_data));
    sa_family = other.sa_family;
    //sa_len = other.sa_len;
}

MessageStructures::Control::realSockAddr::realSockAddr(const sockaddr &s) : sockaddr(s) {

}

template<class Archive>
void MessageStructures::User::Authentication::AuthRequest::serialize(Archive &ar, const unsigned int version) {
    ar & userName;
    ar & token;
}

const std::string &MessageStructures::User::Authentication::AuthRequest::getUserName() const {
    return userName;
}

void MessageStructures::User::Authentication::AuthRequest::setUserName(const std::string &userName) {
    this->userName = userName;
}

const std::string &MessageStructures::User::Authentication::AuthRequest::getToken() const {
    return token;
}

void MessageStructures::User::Authentication::AuthRequest::setToken(const std::string &token) {
    this->token = token;
}

template<class Archive>
void MessageStructures::User::Authentication::LoginRequest::serialize(Archive &ar, const unsigned int version) {
    ar & userName;
    ar & hashedPassword;
}

const std::string &MessageStructures::User::Authentication::LoginRequest::getUserName() const {
    return userName;
}

void MessageStructures::User::Authentication::LoginRequest::setUserName(const std::string &userName) {
    this->userName = userName;
}

const std::string &MessageStructures::User::Authentication::LoginRequest::getHashedPassword() const {
    return hashedPassword;
}

void MessageStructures::User::Authentication::LoginRequest::setHashedPassword(const std::string &hashedPassword) {
    this->hashedPassword = hashedPassword;
}

template<class Archive>
void MessageStructures::User::Authentication::LoginReply::serialize(Archive &ar, const unsigned int version) {
    ar & token;
}

const std::string &MessageStructures::User::Authentication::LoginReply::getToken() const {
    return token;
}

void MessageStructures::User::Authentication::LoginReply::setToken(const std::string &token) {
    this->token = token;
}

template<class Archive>
void MessageStructures::User::Authentication::RegisterRequest::serialize(Archive &ar, const unsigned int version) {
    ar & user_name;
    ar & hashed_password;
}

const std::string &MessageStructures::User::Authentication::RegisterRequest::getUserName() const {
    return user_name;
}

void MessageStructures::User::Authentication::RegisterRequest::setUserName(const std::string &userName) {
    user_name = userName;
}

const std::string &MessageStructures::User::Authentication::RegisterRequest::getHashedPassword() const {
    return hashed_password;
}

void MessageStructures::User::Authentication::RegisterRequest::setHashedPassword(const std::string &hashedPassword) {
    hashed_password = hashedPassword;

}

template<class Archive>
void MessageStructures::User::Authentication::RegisterReply::serialize(Archive &ar, const unsigned int version) {
    ar & registered;
}

bool MessageStructures::User::Authentication::RegisterReply::isRegistered() const {
    return registered;
}

void MessageStructures::User::Authentication::RegisterReply::setRegistered(bool registered) {
    this->registered = registered;
}

template<class Archive>
void MessageStructures::User::AddImageRequest::serialize(Archive &ar, const unsigned int version) {
    ar & image_name;
    ar & img;
    ar & viewer;
    ar & limit;
}

const std::string &MessageStructures::User::AddImageRequest::getImageName() const {
    return image_name;
}

void MessageStructures::User::AddImageRequest::setImageName(const std::string &imageName) {
    image_name = imageName;
}

const std::vector<unsigned char> &MessageStructures::User::AddImageRequest::getImg() const {
    return img;
}

void MessageStructures::User::AddImageRequest::setImg(const std::vector<unsigned char> &img) {
    this->img = img;
}

const std::string &MessageStructures::User::AddImageRequest::getViewer() const {
    return viewer;
}

void MessageStructures::User::AddImageRequest::setViewer(const std::string &viewer) {
    this->viewer = viewer;
}

int MessageStructures::User::AddImageRequest::getLimit() const {
    return limit;
}

void MessageStructures::User::AddImageRequest::setLimit(int limit) {
    this->limit = limit;
}

template<class Archive>
void MessageStructures::User::ViewImageRequest::serialize(Archive &ar, const unsigned int version) {
    ar & userName;
    ar & imageName;
}

const std::string &MessageStructures::User::ViewImageRequest::getUserName() const {
    return userName;
}

void MessageStructures::User::ViewImageRequest::setUserName(const std::string &userName) {
    this->userName = userName;
}

const std::string &MessageStructures::User::ViewImageRequest::getImageName() const {
    return imageName;
}

void MessageStructures::User::ViewImageRequest::setImageName(const std::string &imageName) {
    this->imageName = imageName;
}

template<class Archive>
void MessageStructures::User::ViewImageReply::serialize(Archive &ar, const unsigned int version) {
    ar & image;
}

const MessageStructures::User::Image &MessageStructures::User::ViewImageReply::getImage() const {
    return image;
}

void MessageStructures::User::ViewImageReply::setImage(const MessageStructures::User::Image &image) {
    this->image = image;
}

template<class Archive>
void MessageStructures::User::DeleteImageRequest::serialize(Archive &ar, const unsigned int version) {
    ar & userName;
    ar & imageName;
}

const std::string &MessageStructures::User::DeleteImageRequest::getUserName1() const {
    return userName;
}

void MessageStructures::User::DeleteImageRequest::setUserName1(const std::string &userName) {
    this->userName = userName;
}

const std::string &MessageStructures::User::DeleteImageRequest::getImageName() const {
    return imageName;
}

void MessageStructures::User::DeleteImageRequest::setImageName(const std::string &imageName) {
    this->imageName = imageName;
}

template<class Archive>
void MessageStructures::User::AddViewerRequest::serialize(Archive &ar, const unsigned int version) {
    ar & userName;
    ar & imageName;
    ar & viewerName;
}

const std::string &MessageStructures::User::AddViewerRequest::getUserName1() const {
    return userName;
}

void MessageStructures::User::AddViewerRequest::setUserName1(const std::string &userName) {
    this->userName = userName;
}

const std::string &MessageStructures::User::AddViewerRequest::getImageName() const {
    return imageName;
}

void MessageStructures::User::AddViewerRequest::setImageName(const std::string &imageName) {
    this->imageName = imageName;
}

void MessageStructures::User::AddViewerRequest::setViewerName(const std::string &viewerName) {
    this->viewerName = viewerName;
}

const std::string &MessageStructures::User::AddViewerRequest::getViewerName() const {
    return viewerName;
}

template<class Archive>
void MessageStructures::User::RemoveViewerRequest::serialize(Archive &ar, const unsigned int version) {
    ar & token;
    ar & name;
    ar & to_remove;
}

const std::string &MessageStructures::User::RemoveViewerRequest::getToken1() const {
    return token;
}

void MessageStructures::User::RemoveViewerRequest::setToken1(const std::string &token) {
    RemoveViewerRequest::token = token;
}

const std::string &MessageStructures::User::RemoveViewerRequest::getName() const {
    return name;
}

void MessageStructures::User::RemoveViewerRequest::setName(const std::string &name) {
    this->name = name;
}

const std::vector<std::string> &MessageStructures::User::RemoveViewerRequest::getToRemove() const {
    return to_remove;
}

void MessageStructures::User::RemoveViewerRequest::setToRemove(const std::vector<std::string> &toRemove) {
    to_remove = toRemove;
}

template<class Archive>
void MessageStructures::User::UpdateLimitRequest::serialize(Archive &ar, const unsigned int version) {
    ar & token;
    ar & name;
    ar & newLimit;
}

const std::string &MessageStructures::User::UpdateLimitRequest::getToken1() const {
    return token;
}

void MessageStructures::User::UpdateLimitRequest::setToken1(const std::string &token) {
    UpdateLimitRequest::token = token;
}

const std::string &MessageStructures::User::UpdateLimitRequest::getName() const {
    return name;
}

void MessageStructures::User::UpdateLimitRequest::setName(const std::string &name) {
    this->name = name;
}

int MessageStructures::User::UpdateLimitRequest::getNewLimit() const {
    return newLimit;
}

void MessageStructures::User::UpdateLimitRequest::setNewLimit(int newLimit) {
    this->newLimit = newLimit;
}

template<class Archive>
void MessageStructures::User::SearchReply::serialize(Archive &ar, const unsigned int version) {
    ar & users_ips;
}

template<class Archive>
void MessageStructures::User::SendMessageRequest::serialize(Archive &ar, const unsigned int version) {
    ar & message;
}

const std::string &MessageStructures::User::SendMessageRequest::getMessage() const {
    return message;
}

void MessageStructures::User::SendMessageRequest::setMessage(const std::string &message) {
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
OBJECT_SERIALIZATION(MessageStructures::User::Authentication::RegisterRequest)

OBJECT_SERIALIZATION(MessageStructures::User::Authentication::RegisterReply)
//Login
OBJECT_SERIALIZATION(MessageStructures::User::Authentication::LoginRequest)

OBJECT_SERIALIZATION(MessageStructures::User::Authentication::LoginReply)
//Logout
OBJECT_SERIALIZATION(MessageStructures::User::Authentication::LogoutRequest)

OBJECT_SERIALIZATION(MessageStructures::User::Authentication::LogoutReply)
//Add Image
OBJECT_SERIALIZATION(MessageStructures::User::AddImageRequest)

OBJECT_SERIALIZATION(MessageStructures::User::AddImageReply)
//Delete Image
OBJECT_SERIALIZATION(MessageStructures::User::DeleteImageRequest)

OBJECT_SERIALIZATION(MessageStructures::User::DeleteImageReply)
//Add Viewer
OBJECT_SERIALIZATION(MessageStructures::User::AddViewerRequest)

OBJECT_SERIALIZATION(MessageStructures::User::AddViewerReply)
//Remove Viewer
OBJECT_SERIALIZATION(MessageStructures::User::RemoveViewerRequest)

OBJECT_SERIALIZATION(MessageStructures::User::RemoveViewerReply)
//Update Limit
OBJECT_SERIALIZATION(MessageStructures::User::UpdateLimitRequest)

OBJECT_SERIALIZATION(MessageStructures::User::UpdateLimitReply)
//Search
OBJECT_SERIALIZATION(MessageStructures::User::SearchRequest)

OBJECT_SERIALIZATION(MessageStructures::User::SearchReply)
//Search
OBJECT_SERIALIZATION(MessageStructures::User::SendMessageRequest)

OBJECT_SERIALIZATION(Message)


