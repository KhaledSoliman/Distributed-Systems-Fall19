#include "../headers/ImageBody.h"
#include "../headers/MessageStructures.h"
#include <boost/beast/core/detail/base64.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>
#include <utility>

ImageBody::ImageBody() : ownerUsername(""), viewerUsername(""), remainingViews(0) {
    this->updateTrigger();
    this->createTrigger();
}

ImageBody::~ImageBody() = default;

ImageBody::ImageBody(const std::string &encodedString) {
    std::string decoded = boost::beast::detail::base64_decode(encodedString);
    auto imageBody = load<ImageBody>(decoded);
    *this = imageBody;
    //todo::if createdAt is null update triggers
}

const std::string &ImageBody::getOwnerUsername() const {
    return ownerUsername;
}

void ImageBody::setOwnerUsername(const std::string &ownerUsername) {
    ImageBody::ownerUsername = ownerUsername;
    this->updateTrigger();
}

const std::string &ImageBody::getViewerUsername() const {
    return viewerUsername;
}

void ImageBody::setViewerUsername(const std::string &viewerUsername) {
    ImageBody::viewerUsername = viewerUsername;
    this->updateTrigger();
}

int ImageBody::getRemainingViews() const {
    return remainingViews;
}

void ImageBody::setRemainingViews(int remainingViews) {
    ImageBody::remainingViews = remainingViews;
    this->updateTrigger();
}

const boost::posix_time::ptime &ImageBody::getCreatedAt() const {
    return createdAt;
}

const boost::posix_time::ptime &ImageBody::getUpdatedAt() const {
    return updatedAt;
}


ImageBody::ImageBody(std::string ownerUsername, std::string viewerUsername, int remainingViews)
        : ownerUsername(std::move(ownerUsername)), viewerUsername(std::move(viewerUsername)),
          remainingViews(remainingViews) {
    this->createTrigger();
    this->updateTrigger();
}

void ImageBody::updateTrigger() {
    this->updatedAt = boost::posix_time::second_clock::local_time();
}

void ImageBody::createTrigger() {
    this->createdAt = boost::posix_time::second_clock::local_time();
}
