#include "../headers/Cache.h"
#include "../headers/MessageStructures.h"
#include "../headers/Seng.h"
#include <fstream>
#include <cstdio>
#include "boost/filesystem.hpp"
#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>

#define CACHE_DIR "/Users/khloudghattas/CLionProjects/Distributed-Systems-Fall19/Cache"
#define TIMEOUT 72
#define PASSWORD "UNO,DOS,TRES"

namespace BFS = boost::filesystem;

Cache::Cache() {

}

Cache::~Cache() {

}

void Cache::insertImage(const std::string &image, const std::string &imageName, const ImageBody &imageBody) {
    std::string serialized = save<ImageBody>(imageBody);
    std::string imagePath = CACHE_DIR + imageName;
    std::ofstream out;
    out.open(imagePath);
    out << image;
    out.close();
    std::string textPath = CACHE_DIR + imageName + ".txt";
    Seng::stringToImage(imagePath, textPath, serialized, PASSWORD);
}

void Cache::updateImage(const std::string &imageName, const ImageBody &imageBody) {
    std::string serialized = save<ImageBody>(imageBody);
    std::string imagePath = CACHE_DIR + imageName;
    std::string textPath = CACHE_DIR + imageName + ".txt";
    Seng::stringToImage(imagePath, textPath, serialized, PASSWORD);
}

void Cache::removeImage(const std::string &imageName) {
    std::string imagePath = CACHE_DIR + imageName;
    remove(imagePath.c_str());
}

void Cache::updateCache() {
    BFS::path targetDir(CACHE_DIR);
    BFS::directory_iterator it(targetDir), eod;
    BOOST_FOREACH(BFS::path const &p, std::make_pair(it, eod)) {
        if (BFS::is_regular_file(p)) {
            std::string encoded = Seng::imageToString(p.string(), PASSWORD);
            ImageBody imageBody(encoded);
            boost::posix_time::time_duration deltaTime = boost::posix_time::second_clock::local_time() - imageBody.getUpdatedAt();
            if(deltaTime.hours() >= TIMEOUT) {
                this->removeImage(p.filename().string());
            }
        }
    }
}

