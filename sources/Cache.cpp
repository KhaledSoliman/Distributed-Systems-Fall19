//
// Created by khloud ghattas on 12/5/19.
//

#include "../headers/Cache.h"

int Cache::getViews() const {
    return views;
}

void Cache::setViews(int views) {
    Cache::views = views;
}

int Cache::getImageName() const {
    return ImageName;
}

void Cache::setImageName(int imageName) {
    ImageName = imageName;
}

const boost::posix_time::ptime &Cache::getCreatedAt() const {
    return createdAt;
}

void Cache::setCreatedAt(const boost::posix_time::ptime &createdAt) {
    Cache::createdAt = createdAt;
}

const boost::posix_time::ptime &Cache::getUpdatedAt() const {
    return updatedAt;
}

void Cache::setUpdatedAt(const boost::posix_time::ptime &updatedAt) {
    Cache::updatedAt = updatedAt;
}

Cache::Cache() {

}

void Cache::UpdateCache() {

}
void Cache::InsertCache() {

}
void Cache::RemoveCache() {

}

Cache::~Cache() {

}
