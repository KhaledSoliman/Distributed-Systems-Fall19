#ifndef PROJ_CACHE_H
#define PROJ_CACHE_H

#include <ctime>
#include <string>

class cache {
private:

    time_t updatedAt;

public:

    cache();

    virtual ~cache();

    void InsertImage(std::string img, std::string imgName);

    void RemoveImage(std::string imgName);

    void UpdateImage();

};


#endif //PROJ_CACHE_H
