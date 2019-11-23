#ifndef DISTRIBUTED_PROJECT_MSGS_H
#define DISTRIBUTED_PROJECT_MSGS_H

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/array.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/serialization.hpp>
#include <map>
#include <vector>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <map>
#include <boost/serialization/map.hpp>
#include <sys/socket.h>

using std::vector;
using std::string;
namespace rpc {
    struct Error {
    public:
        bool flag;
        string msg;
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & flag;
            ar & msg;
        }
    };
}

struct Image {
    string owner;
    int limit;

};

using rpc::Error;

struct realSockAddr:public sockaddr{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & this->sa_data;
        ar & this->sa_family;
        //ar & this->sa_len;
    }
    realSockAddr(){}
    realSockAddr(const realSockAddr& other){
        memcpy(sa_data, other.sa_data, sizeof(other.sa_data));
        sa_family = other.sa_family;
        //sa_len = other.sa_len;
    }
    realSockAddr(const sockaddr& s) : sockaddr(s){}
};

struct RequestWithToken {
public:
    string user_name;
    string token;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & user_name;
        ar & token;
    }
};

struct LoginRequest {
    string user_name;
    string hashed_password;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & user_name;
        ar & hashed_password;
    }
};

struct LoginReply:rpc::Error {
    string token;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & token;
        ar & flag;
        ar & msg;
    }
};

struct RegisterRequest {
    string user_name;
    string hashed_password;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & user_name;
        ar & hashed_password;
    }
};

struct RegisterReply:rpc::Error {
    bool registered;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & registered;
        ar & flag;
        ar & msg;
    }
};

//Add image
struct AddImageRequest:RequestWithToken{
    string image_name;
    std::vector<unsigned char> img;
    string viewer;
    int limit;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & image_name;
        ar & img;
        ar & viewer;
        ar & limit;
    }
};

struct AddImageReply:rpc::Error{
};

// View Image

struct ViewImageRequest {
    string user_name;
    string image_name;
};

struct ViewImageReply {
    struct Error error;
    QPixmap image;
};

// Delete Image

struct DeleteImageRequest {
    string user_name;
    string image_name;
};

struct DeleteImageReply {
    struct Error error;
};

// Add viewers

struct AddViewerRequest {
    string user_name;
    string image_name;
    string viewer_name;
};

struct AddViewerReply {
    struct Error error;
};

// Remove Viewers

struct RemoveViewerRequest {
    string token;
    string name;
    vector<string> to_remove;
};

struct RemoveViewersReply {
    struct Error error;
};

// Update limit

struct UpdateLimitRequest {
    string token;
    string name;
    int new_limit;
};

struct UpdateLimitReply {
    struct Error error;
};

// Get all messages

struct SearchRequest:RequestWithToken {
};

struct SearchReply:rpc::Error {
public:
    std::map<std::string, realSockAddr> users_ips;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & users_ips;
        ar & flag;
        ar & msg;
    }
};

// Logout

struct LogoutRequest:RequestWithToken {
};

struct LogoutReply: rpc::Error {
};

// String Message

struct StringMessage {
public:
    string message;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & message;
    }
};

#endif //DISTRIBUTED_PROJECT_MSGS_H
