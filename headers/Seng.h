#ifndef PROJ_SENG_H
#define PROJ_SENG_H

#include <string>


class Seng {

public:
    Seng();

    ~Seng();

    static void StringToImage(const std::string& imagePath,const std::string& textPath, const std::string& password);

    static std::string ImageToString(const std::string& imagePath,const std::string& password);
};

#endif //PROJ_SENG_H
