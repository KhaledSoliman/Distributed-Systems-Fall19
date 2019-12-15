#ifndef PROJ_CACHE_H
#define PROJ_CACHE_H

#include <string>
#include "ImageBody.h"

class Cache {

public:

    Cache();

    virtual ~Cache();

    void insertImage(const std::string& viewerName, const std::string& image, const std::string& imageName, const ImageBody& imageBody);

    void insertImage(const std::string& viewerName, const std::string& image, const std::string& imageName);

    void removeImage(const std::string& imageName) const;

    void updateImage(const std::string& viewerName, const std::string& imageName, const ImageBody& imageBody);

    std::string getImage(const std::string viewerName, const std::string& imageName);

    ImageBody getImageBody(const std::string viewerName, const std::string& imageName);

    void updateCache() const;

};


#endif //PROJ_CACHE_H
