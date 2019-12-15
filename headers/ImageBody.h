#ifndef PROJ_IMAGEBODY_H
#define PROJ_IMAGEBODY_H

#include <string>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/serialization/access.hpp>

class ImageBody {
private:
    std::string ownerUsername;
    std::string viewerUsername;
    int remainingViews;
    boost::posix_time::ptime createdAt;
    boost::posix_time::ptime updatedAt;

    void updateTrigger();

    void createTrigger();

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int /* file_version */) {
        ar & ownerUsername & viewerUsername & remainingViews & createdAt & updatedAt;
    }

public:

    ImageBody(std::string ownerUsername, std::string viewerUsername, int remainingViews);

    explicit ImageBody(const std::string &encodedString);

    ImageBody();

    ~ImageBody();

    const std::string &getOwnerUsername() const;

    void setOwnerUsername(const std::string &ownerUsername);

    const std::string &getViewerUsername() const;

    void setViewerUsername(const std::string &viewerUsername);

    int getRemainingViews() const;

    void setRemainingViews(int remainingViews);

    const boost::posix_time::ptime &getCreatedAt() const;

    const boost::posix_time::ptime &getUpdatedAt() const;
};

#endif //PROJ_IMAGEBODY_H
