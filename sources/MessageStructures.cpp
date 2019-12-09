#include "../headers/MessageStructures.h"
#include "../headers/ImageBody.h"

using namespace MessageStructures;
using namespace MessageStructures::Control;
using namespace MessageStructures::User;
using namespace MessageStructures::User::Authentication;

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


realSockAddr::realSockAddr() {

}

realSockAddr::realSockAddr(const realSockAddr &other) {
    memcpy(sa_data, other.sa_data, sizeof(other.sa_data));
    sa_family = other.sa_family;
    //sa_len = other.sa_len;
}

realSockAddr::realSockAddr(const sockaddr &s) : sockaddr(s) {

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

const std::string &LoginReply::getToken() const {
    return token;
}

void LoginReply::setToken(const std::string &token) {
    this->token = token;
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

bool RegisterReply::isRegistered() const {
    return registered;
}

void RegisterReply::setRegistered(bool registered) {
    this->registered = registered;
}

const std::string &AddImageRequest::getImageName() const {
    return image_name;
}

void AddImageRequest::setImageName(const std::string &imageName) {
    image_name = imageName;
}

const std::string &AddImageRequest::getThumbnail() const {
    return thumbnail;
}

void AddImageRequest::setThumbnail(const std::string &thumbnail) {
    AddImageRequest::thumbnail = thumbnail;
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

const Image &ViewImageReply::getImage() const {
    return image;
}

void ViewImageReply::setImage(const Image &image) {
    this->image = image;
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

int FeedRequest::getImageNum() const {
    return imageNum;
}

void FeedRequest::setImageNum(int imageNum) {
    FeedRequest::imageNum = imageNum;
}

int FeedRequest::getLastIndex() const {
    return lastIndex;
}

void FeedRequest::setLastIndex(int lastIndex) {
    FeedRequest::lastIndex = lastIndex;
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

const std::string &RemoveViewerRequest::getImageName() const {
    return imageName;
}

void RemoveViewerRequest::setImageName(const std::string &imageName) {
    RemoveViewerRequest::imageName = imageName;
}

const std::string &RemoveViewerRequest::getToRemove() const {
    return to_remove;
}

void RemoveViewerRequest::setToRemove(const std::string &toRemove) {
    to_remove = toRemove;
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

const std::string &UpdateLimitRequest::getTargetUsername() const {
    return targetUsername;
}

void UpdateLimitRequest::setTargetUsername(const std::string &targetUsername) {
    UpdateLimitRequest::targetUsername = targetUsername;
}

const std::string &SendMessageRequest::getMessage() const {
    return message;
}

void SendMessageRequest::setMessage(const std::string &message) {
    this->message = message;
}

const std::string &Echo::getMsg() const {
    return msg;
}

void Echo::setMsg(const std::string &msg) {
    Echo::msg = msg;
}

int FeedReply::getCurrentIndex() const {
    return currentIndex;
}

void FeedReply::setCurrentIndex(int currentIndex) {
    FeedReply::currentIndex = currentIndex;
}

const std::unordered_map<std::string, std::string> &FeedReply::getImages() const {
    return images;
}

void FeedReply::setImages(const std::unordered_map<std::string, std::string> &images) {
    FeedReply::images = images;
}

const std::string &SearchRequest::getTargetUsername() const {
    return targetUsername;
}

void SearchRequest::setTargetUsername(const std::string &targetUsername) {
    SearchRequest::targetUsername = targetUsername;
}

