#ifndef PROJ_CACHE_H
#define PROJ_CACHE_H

#include <string>
#include "ImageBody.h"

class Cache {

public:

    Cache();

    virtual ~Cache();

    void insertImage(const std::string& image, const std::string& imageName, const ImageBody& imageBody);

    void removeImage(const std::string& imageName) const;

    void updateImage(const std::string& imageName, const ImageBody& imageBody);

    void updateCache() const;

};


#endif //PROJ_CACHE_H
