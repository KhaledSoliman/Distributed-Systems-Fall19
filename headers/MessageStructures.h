#ifndef PROJ_MESSAGESTRUCTURES_H
#define PROJ_MESSAGESTRUCTURES_H


#include <vector>
#include <map>
#include <sys/socket.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/array.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <unordered_map>
#include <sstream>

#include "Message.h"
#include "ImageBody.h"

namespace MessageStructures {

    namespace Control {
        struct Error {
        private:
            bool flag;
            std::string msg;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & flag;
                ar & msg;
            }

        public:
            [[nodiscard]] bool isFlag() const;

            void setFlag(bool flag);

            [[nodiscard]] const std::string &getMsg() const;

            void setMsg(const std::string &msg);
        };

        struct realSockAddr : public sockaddr {
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & this->sa_data;
                ar & this->sa_family;
                //ar & this->sa_len;
            }

            realSockAddr();

            realSockAddr(const realSockAddr &other);

            realSockAddr(const sockaddr &s);
        };

        struct Ack : Error {
        private:
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar;
            }

        public:

        };

        struct Echo {
        private:
            std::string msg;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & msg;
            }

        public:
            const std::string &getMsg() const;

            void setMsg(const std::string &msg);
        };
    }

    namespace User {
        namespace Authentication {
            struct AuthRequest {
            private:
                friend class boost::serialization::access;

                std::string userName;
                std::string token;

                template<class Archive>
                void serialize(Archive &ar, const unsigned int version) {
                    ar & userName;
                    ar & token;
                }

            public:
                [[nodiscard]] const std::string &getUserName() const;

                void setUserName(const std::string &userName);

                [[nodiscard]] const std::string &getToken() const;

                void setToken(const std::string &token);
            };

            struct LoginRequest {
            private:
                std::string userName;
                std::string hashedPassword;

                friend class boost::serialization::access;

                template<class Archive>
                void serialize(Archive &ar, const unsigned int version) {
                    ar & userName;
                    ar & hashedPassword;
                }

            public:
                [[nodiscard]] const std::string &getUserName() const;

                void setUserName(const std::string &userName);

                [[nodiscard]] const std::string &getHashedPassword() const;

                void setHashedPassword(const std::string &hashedPassword);
            };

            struct LoginReply : public Control::Error {
            private:
                std::string token;

                friend class boost::serialization::access;

                template<class Archive>
                void serialize(Archive &ar, const unsigned int version) {
                    ar & boost::serialization::base_object<Control::Error>(*this);
                    ar & token;
                }

            public:
                [[nodiscard]] const std::string &getToken() const;

                void setToken(const std::string &token);
            };

            // Logout
            struct LogoutRequest : public Authentication::AuthRequest {
            private:
            public:
            };

            struct LogoutReply : public Control::Error {
            private:
            public:
            };

            struct RegisterRequest {
            private:
                std::string user_name;
                std::string hashed_password;

                friend class boost::serialization::access;

                template<class Archive>
                void serialize(Archive &ar, const unsigned int version) {
                    ar & user_name;
                    ar & hashed_password;
                }

            public:
                [[nodiscard]] const std::string &getUserName() const;

                void setUserName(const std::string &userName);

                [[nodiscard]] const std::string &getHashedPassword() const;

                void setHashedPassword(const std::string &hashedPassword);
            };

            struct RegisterReply : public Control::Error {
            private:
                bool registered;

                friend class boost::serialization::access;

                template<class Archive>
                void serialize(Archive &ar, const unsigned int version) {
                    ar & boost::serialization::base_object<Control::Error>(*this);
                    ar & registered;
                }

            public:
                bool isRegistered() const;

                void setRegistered(bool registered);
            };
        }

        struct Image {
            std::string owner;
            int limit;
        };


        struct Hello {
        private:
            std::string message;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & message;
            }

        public:
            const std::string &getMessage() const {
                return message;
            }

            void setMessage(const std::string &message) {
                Hello::message = message;
            }
        };

        //Add image
        struct AddImageRequest : public Authentication::AuthRequest {
        private:
            std::string image_name;
            std::string thumbnail;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Authentication::AuthRequest>(*this);
                ar & image_name;
                ar & thumbnail;
            }

        public:
            [[nodiscard]] const std::string &getImageName() const;

            void setImageName(const std::string &imageName);

            const std::string &getThumbnail() const;

            void setThumbnail(const std::string &thumbnail);

        };

        struct AddImageReply : public Control::Error {
        private:
        public:
        };
          //  Download Image
          struct DownloadImageRequest : public Authentication::AuthRequest {
          private:
              std::string image_name;

              friend class boost::serialization::access;

              template<class Archive>
              void serialize(Archive &ar, const unsigned int version) {
                  ar & boost::serialization::base_object<Authentication::AuthRequest>(*this);
                  ar & image_name;
              }

          public:
              const std::string &getImageName() const {
                  return image_name;
              }

              void setImageName(const std::string &imageName) {
                  image_name = imageName;
              }

          };

        struct DownloadImageReply : public Control::Error {
        private:
        public:
        };

        // View Image
        struct ViewImageRequest : public Control::Error {
        private:
            std::string userName;
            std::string imageName;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Control::Error>(*this); //gded
                ar & userName;
                ar & imageName;
            }

        public:
            [[nodiscard]] const std::string &getUserName() const;

            void setUserName(const std::string &userName);

            [[nodiscard]] const std::string &getImageName() const;

            void setImageName(const std::string &imageName);
        };

        struct ViewImageReply : public Control::Error {
        private:
            Image image;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Control::Error>(*this);
                ar & image;
            }

        public:
            [[nodiscard]] const Image &getImage() const;
            void setImage(const Image &image);
        };

        // Delete Image
        struct DeleteImageRequest : public Authentication::AuthRequest {
        private:
            std::string userName;
            std::string imageName;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Authentication::AuthRequest>(*this);
                ar & userName;
                ar & imageName;
            }

        public:
            [[nodiscard]] const std::string &getUserName1() const;

            void setUserName1(const std::string &userName);

            [[nodiscard]] const std::string &getImageName() const;

            void setImageName(const std::string &imageName);
        };

        struct DeleteImageReply : public Control::Error {
        private:
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Control::Error>(*this);
            }
        public:
        };

        // Add viewers
        struct AddViewerRequest : public Authentication::AuthRequest {
        private:
            std::string userName;
            std::string imageName;
            std::string viewerName;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Authentication::AuthRequest>(*this);
                ar & userName;
                ar & imageName;
                ar & viewerName;
            }

        public:
            const std::string &getUserName1() const;

            void setUserName1(const std::string &userName);

            const std::string &getImageName() const;

            void setImageName(const std::string &imageName);

            const std::string &getViewerName() const;

            void setViewerName(const std::string &viewerName);
        };

        struct AddViewerReply : Control::Error {
        private:
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Control::Error>(*this);
            }
        public:
        };

        struct RemoveViewerRequest : Authentication::AuthRequest {
        private:
            std::string imageName;
            std::string to_remove;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Authentication::AuthRequest>(*this);
                ar & imageName;
                ar & to_remove;
            }


        public:
            const std::string &getImageName() const;

            void setImageName(const std::string &imageName);

            const std::string &getToRemove() const;

            void setToRemove(const std::string &toRemove);

        };

        struct RemoveViewerReply : Control::Error {
        private:
        public:
        };

        // Update limit
        struct UpdateLimitRequest : Authentication::AuthRequest {
        private:
            std::string targetUsername;
            std::string name;
            int newLimit;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Authentication::AuthRequest>(*this);
                ar & targetUsername;
                ar & name;
                ar & newLimit;
            }

        public:
            const std::string &getTargetUsername() const;

            void setTargetUsername(const std::string &targetUsername);

            [[nodiscard]] const std::string &getName() const;

            void setName(const std::string &name);

            [[nodiscard]] int getNewLimit() const;

            void setNewLimit(int newLimit);
        };

        struct UpdateLimitReply : public Control::Error {
        private:
        public:
        };

        struct FeedRequest : public Authentication::AuthRequest {
        private:
            int lastIndex;
            int imageNum;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Authentication::AuthRequest>(*this);
                ar & imageNum & lastIndex;
            }


        public:
            int getLastIndex() const;

            void setLastIndex(int lastIndex);

            int getImageNum() const;

            void setImageNum(int imageNum);
        };

        struct FeedReply : public Control::Error {
        private:
            int currentIndex;
            std::unordered_map<std::string, std::string> images;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Control::Error>(*this);
                ar & currentIndex;
                ar & images;
            }

        public:
            int getCurrentIndex() const;

            void setCurrentIndex(int currentIndex);

            const std::unordered_map<std::string, std::string> &getImages() const;

            void setImages(const std::unordered_map<std::string, std::string> &images);
        };

        struct GetRequestsReply : public Authentication::AuthRequest {
        private:
            std::vector<ViewImageRequest> requests;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & requests;
            }
        public:
            const std::vector<ViewImageRequest> &getRequests() const {
                return requests;
            }

            void setRequests(const std::vector<ViewImageRequest> &requests) {
                GetRequestsReply::requests = requests;
            }
        };

        struct GetRequests : public Authentication::AuthRequest {
        private:
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Authentication::AuthRequest>(*this);
                ar;
            }
        };

        // Get all messages
        struct SearchRequest : public Authentication::AuthRequest {
        private:
            std::string targetUsername;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Authentication::AuthRequest>(*this);
                ar & targetUsername;
            }

        public:
            const std::string &getTargetUsername() const;

            void setTargetUsername(const std::string &targetUsername);

        };

        struct SearchReply : public Control::Error {
        private:
            std::map<std::string, Control::realSockAddr> users_ips;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & users_ips;
            }

        };

        struct SendMessageRequest : public Authentication::AuthRequest {
        private:
            std::string message;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & boost::serialization::base_object<Authentication::AuthRequest>(*this);
                ar & message;
            }

        public:
            const std::string &getMessage() const;

            void setMessage(const std::string &message);
        };
    }
}


template<typename T>
std::string save(const T &t) {
    std::ostringstream archiveStream;
    boost::archive::text_oarchive archive(archiveStream, boost::archive::no_header);
    archive << t;
    return archiveStream.str();
}

template<typename T>
T load(const std::string &message) {
    T t;
    std::stringstream archiveStream;
    archiveStream << message;
    boost::archive::text_iarchive archive(archiveStream, boost::archive::no_header);
    archive >> t;
    return t;
}


using namespace MessageStructures::Control;
using namespace MessageStructures::User;
using namespace MessageStructures::User::Authentication;

#define OBJECT_SERIALIZATION(T) template std::string save(const T &t); template T load(const std::string &message);
//Echo ICMP
OBJECT_SERIALIZATION(Echo)

//Directory Service Hello
OBJECT_SERIALIZATION(Hello)

//Register
OBJECT_SERIALIZATION(RegisterRequest)

OBJECT_SERIALIZATION(RegisterReply)
//Login
OBJECT_SERIALIZATION(LoginRequest)

OBJECT_SERIALIZATION(LoginReply)
//Logout
OBJECT_SERIALIZATION(LogoutRequest)

OBJECT_SERIALIZATION(LogoutReply)
//Add Image
OBJECT_SERIALIZATION(AddImageRequest)

OBJECT_SERIALIZATION(AddImageReply)
//Delete Image
OBJECT_SERIALIZATION(DeleteImageRequest)

OBJECT_SERIALIZATION(DeleteImageReply)

//Add Viewer
OBJECT_SERIALIZATION(AddViewerRequest)

OBJECT_SERIALIZATION(AddViewerReply)
//Remove Viewer
OBJECT_SERIALIZATION(RemoveViewerRequest)

OBJECT_SERIALIZATION(RemoveViewerReply)
//Update Limit
OBJECT_SERIALIZATION(UpdateLimitRequest)

OBJECT_SERIALIZATION(UpdateLimitReply)
//Search
OBJECT_SERIALIZATION(SearchRequest)

OBJECT_SERIALIZATION(SearchReply)
//Search
OBJECT_SERIALIZATION(SendMessageRequest)

OBJECT_SERIALIZATION(Message)

OBJECT_SERIALIZATION(ImageBody)


#endif //PROJ_MESSAGESTRUCTURES_H
