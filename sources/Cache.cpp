#include "../headers/cache.h"
#include <iostream>
#include <fstream>

cache::cache() {

}

cache::~cache() {

}

void cache::InsertImage(std::string img, std::string imgName) {

    std::ofstream outfile;
    std::string imagePath;
    imagePath = "../Cache/" + imgName;
    outfile.open(imagePath);
    outfile << img;
    outfile.close();
}

void cache::RemoveImage(std::string imgName) {

    std::string imagePath;
    imagePath = "../Cache/" + imgName;
    const char *imagePath2 = imagePath.c_str();
    remove(imagePath2);

}

void cache::UpdateImage() {


}
