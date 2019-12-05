#ifndef PROJ_CACHE_H
#define PROJ_CACHE_H

#include <boost/date_time/posix_time/ptime.hpp>

class Cache {
public:
    int getViews() const;

    void setViews(int views);

    int getImageName() const;

    void setImageName(int imageName);

    const boost::posix_time::ptime &getCreatedAt() const;

    void setCreatedAt(const boost::posix_time::ptime &createdAt);

    const boost::posix_time::ptime &getUpdatedAt() const;

    void setUpdatedAt(const boost::posix_time::ptime &updatedAt);

private:
    int views;
    int ImageName;
    boost::posix_time::ptime createdAt;
    boost::posix_time::ptime updatedAt;

    Cache();

    ~Cache();

    void UpdateCache(const boost::posix_time::ptime createdAt);

    void InsertCache();

    void RemoveCache();
};

#endif //PROJ_CACHE_H
