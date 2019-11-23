#ifndef PROJ_MESSAGESTRUCTURES_H
#define PROJ_MESSAGESTRUCTURES_H


#include <vector>
#include <map>
#include <sys/socket.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/array.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

namespace MessageStructures {
    using std::vector;
    using std::string;

    namespace Control {
        struct Error {
        private:
            bool flag;
            std::string msg;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);

        public:
            [[nodiscard]] bool isFlag() const {
                return flag;
            }

            void setFlag(bool flag) {
                this->flag = flag;
            }

            [[nodiscard]] const std::string &getMsg() const {
                return msg;
            }

            void setMsg(const std::string &msg) {
                this->msg = msg;
            }
        };

        struct realSockAddr : public sockaddr {
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & this->sa_data;
                ar & this->sa_family;
                //ar & this->sa_len;
            }

            realSockAddr() {}

            realSockAddr(const realSockAddr &other) {
                memcpy(sa_data, other.sa_data, sizeof(other.sa_data));
                sa_family = other.sa_family;
                //sa_len = other.sa_len;
            }

            realSockAddr(const sockaddr &s) : sockaddr(s) {}
        };

        struct Ack {
        public:
            bool flag;
            string msg;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & flag;
                ar & msg;
            }
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
                [[nodiscard]] const std::string &getUserName() const {
                    return userName;
                }

                void setUserName(const std::string &userName) {
                    this->userName = userName;
                }

                [[nodiscard]] const std::string &getToken() const {
                    return token;
                }

                void setToken(const std::string &token) {
                    this->token = token;
                }
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
                [[nodiscard]] const std::string &getUserName() const {
                    return userName;
                }

                void setUserName(const std::string &userName) {
                    LoginRequest::userName = userName;
                }

                [[nodiscard]] const std::string &getHashedPassword() const {
                    return hashedPassword;
                }

                void setHashedPassword(const std::string &hashedPassword) {
                    LoginRequest::hashedPassword = hashedPassword;
                }
            };

            struct LoginReply : public Control::Error {
            private:
                std::string token;

                friend class boost::serialization::access;

                template<class Archive>
                void serialize(Archive &ar, const unsigned int version) {
                    ar & token;
                }

            public:
                [[nodiscard]] const std::string &getToken() const {
                    return token;
                }

                void setToken(const std::string &token) {
                    LoginReply::token = token;
                }
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
                [[nodiscard]] const std::string &getUserName() const {
                    return user_name;
                }

                void setUserName(const std::string &userName) {
                    user_name = userName;
                }

                [[nodiscard]] const std::string &getHashedPassword() const {
                    return hashed_password;
                }

                void setHashedPassword(const std::string &hashedPassword) {
                    hashed_password = hashedPassword;
                }
            };

            struct RegisterReply : public Control::Error {
            private:
                bool registered;

                friend class boost::serialization::access;

                template<class Archive>
                void serialize(Archive &ar, const unsigned int version) {
                    ar & registered;
                }

            public:
                bool isRegistered() const {
                    return registered;
                }

                void setRegistered(bool registered) {
                    RegisterReply::registered = registered;
                }
            };
        }

        struct Image {
            std::string owner;
            int limit;
        };

        //Add image
        struct AddImageRequest : public Authentication::AuthRequest {
        private:
            std::string image_name;
            std::vector<unsigned char> img;
            std::string viewer;
            int limit;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & image_name;
                ar & img;
                ar & viewer;
                ar & limit;
            }

        public:
            [[nodiscard]] const std::string &getImageName() const {
                return image_name;
            }

            void setImageName(const std::string &imageName) {
                image_name = imageName;
            }

            [[nodiscard]] const vector<unsigned char> &getImg() const {
                return img;
            }

            void setImg(const vector<unsigned char> &img) {
                AddImageRequest::img = img;
            }

            [[nodiscard]] const std::string &getViewer() const {
                return viewer;
            }

            void setViewer(const std::string &viewer) {
                AddImageRequest::viewer = viewer;
            }

            [[nodiscard]] int getLimit() const {
                return limit;
            }

            void setLimit(int limit) {
                AddImageRequest::limit = limit;
            }
        };

        struct AddImageReply : public Control::Error {
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
                ar & userName;
                ar & imageName;
            }

        public:
            [[nodiscard]] const std::string &getUserName() const {
                return userName;
            }

            void setUserName(const std::string &userName) {
                ViewImageRequest::userName = userName;
            }

            [[nodiscard]] const std::string &getImageName() const {
                return imageName;
            }

            void setImageName(const std::string &imageName) {
                ViewImageRequest::imageName = imageName;
            }
        };

        struct ViewImageReply : public Control::Error {
        private:
            Image image;
        public:

        };

        // Delete Image
        struct DeleteImageRequest : public Authentication::AuthRequest {
        private:
            std::string userName;
            std::string imageName;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & userName;
                ar & imageName;
            }

        public:
            [[nodiscard]] const std::string &getUserName1() const {
                return userName;
            }

            void setUserName1(const std::string &userName) {
                DeleteImageRequest::userName = userName;
            }

            [[nodiscard]] const string &getImageName() const {
                return imageName;
            }

            void setImageName(const std::string &imageName) {
                DeleteImageRequest::imageName = imageName;
            }
        };

        struct DeleteImageReply : public Control::Error {
        private:
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
                ar & userName;
                ar & imageName;
                ar & viewerName;
            }

        public:
            const string &getUserName1() const {
                return userName;
            }

            void setUserName1(const string &userName) {
                AddViewerRequest::userName = userName;
            }

            const string &getImageName() const {
                return imageName;
            }

            void setImageName(const string &imageName) {
                AddViewerRequest::imageName = imageName;
            }

            const string &getViewerName() const {
                return viewerName;
            }

            void setViewerName(const string &viewerName) {
                AddViewerRequest::viewerName = viewerName;
            }
        };

        struct AddViewerReply : Control::Error {
        private:
        public:
        };

        struct RemoveViewerRequest : Authentication::AuthRequest {
        private:
            std::string token;
            std::string name;
            vector<std::string> to_remove;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & token;
                ar & name;
                ar & to_remove;
            }

        public:
            const string &getToken1() const {
                return token;
            }

            void setToken1(const string &token) {
                RemoveViewerRequest::token = token;
            }

            const string &getName() const {
                return name;
            }

            void setName(const string &name) {
                RemoveViewerRequest::name = name;
            }

            const vector<std::string> &getToRemove() const {
                return to_remove;
            }

            void setToRemove(const vector<std::string> &toRemove) {
                to_remove = toRemove;
            }
        };

        struct RemoveViewerReply : Control::Error {
        private:
        public:
        };

        // Update limit
        struct UpdateLimitRequest : Authentication::AuthRequest {
        private:
            std::string token;
            std::string name;
            int newLimit;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version) {
                ar & token;
                ar & name;
                ar & newLimit;
            }

        public:
            [[nodiscard]] const std::string &getToken1() const {
                return token;
            }

            void setToken1(const std::string &token) {
                UpdateLimitRequest::token = token;
            }

            [[nodiscard]] const string &getName() const {
                return name;
            }

            void setName(const std::string &name) {
                UpdateLimitRequest::name = name;
            }

            [[nodiscard]] int getNewLimit() const {
                return newLimit;
            }

            void setNewLimit(int newLimit) {
                UpdateLimitRequest::newLimit = newLimit;
            }
        };

        struct UpdateLimitReply : public Control::Error {
        private:
        public:
        };

        // Get all messages
        struct SearchRequest : public Authentication::AuthRequest {
        private:
        public:
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
                ar & message;
            }

        public:
            const string &getMessage() const {
                return message;
            }

            void setMessage(const string &message) {
                SendMessageRequest::message = message;
            }
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

#define OBJECT_SERIALIZATION(T) template std::string save(const T &t); template T load(const std::string &message);
//Register
OBJECT_SERIALIZATION(MessageStructures::User::Authentication::RegisterRequest)

OBJECT_SERIALIZATION(MessageStructures::User::Authentication::RegisterReply)
//Login
OBJECT_SERIALIZATION(MessageStructures::User::Authentication::LoginRequest)

OBJECT_SERIALIZATION(MessageStructures::User::Authentication::LoginReply)
//Logout
OBJECT_SERIALIZATION(MessageStructures::User::Authentication::LogoutRequest)

OBJECT_SERIALIZATION(MessageStructures::User::Authentication::LogoutReply)
//Add Image
OBJECT_SERIALIZATION(MessageStructures::User::AddImageRequest)

OBJECT_SERIALIZATION(MessageStructures::User::AddImageReply)
//Delete Image
OBJECT_SERIALIZATION(MessageStructures::User::DeleteImageRequest)

OBJECT_SERIALIZATION(MessageStructures::User::DeleteImageReply)
//Add Viewer
OBJECT_SERIALIZATION(MessageStructures::User::AddViewerRequest)

OBJECT_SERIALIZATION(MessageStructures::User::AddViewerReply)
//Remove Viewer
OBJECT_SERIALIZATION(MessageStructures::User::RemoveViewerRequest)

OBJECT_SERIALIZATION(MessageStructures::User::RemoveViewerReply)
//Update Limit
OBJECT_SERIALIZATION(MessageStructures::User::UpdateLimitRequest)

OBJECT_SERIALIZATION(MessageStructures::User::UpdateLimitReply)
//Search
OBJECT_SERIALIZATION(MessageStructures::User::SearchRequest)

OBJECT_SERIALIZATION(MessageStructures::User::SearchReply)
//Search
OBJECT_SERIALIZATION(MessageStructures::User::SendMessageRequest)

#endif //PROJ_MESSAGESTRUCTURES_H
