#include "../headers/ImageBody.h"

const std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> &
ImageBody::getImageName() const {
    return ImageName;
}

void ImageBody::setImageName(const std::string &imageName) {
    ImageName = imageName;
}

const std::string &ImageBody::getImagePath() const {
    return ImagePath;
}

void ImageBody::setImagePath(const std::string &imagePath) {
    ImagePath = imagePath;
}
ImageBody::ImageBody() {

}
void ImageBody::StringBase46(std::string String_base64) {

}
ImageBody::~ImageBody() {

}
