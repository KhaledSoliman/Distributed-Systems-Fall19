#ifndef PROJ_SENG_H
#define PROJ_SENG_H

#include <string>


class Seng {
public:
    static void stringToImage(const std::string& imagePath, const std::string& textPath, const std::string& text, const std::string& password);

    static std::string imageToString(const std::string& imagePath, const std::string& password);
};

#endif //PROJ_SENG_H
