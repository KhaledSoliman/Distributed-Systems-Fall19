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
#include <unordered_map>

namespace MessageStructures {

    namespace Control {
        struct Error {
        private:
            bool flag;
            std::string msg;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);

        public:
            [[nodiscard]] bool isFlag() const;

            void setFlag(bool flag);

            [[nodiscard]] const std::string &getMsg() const;

            void setMsg(const std::string &msg);
        };

        struct realSockAddr : public sockaddr {
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);

            realSockAddr();

            realSockAddr(const realSockAddr &other);

            realSockAddr(const sockaddr &s);
        };

        struct Ack : Error {
        private:
            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);
        public:

        };

        struct Echo {
        private:
            std::string msg;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);

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
                void serialize(Archive &ar, const unsigned int version);

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
                void serialize(Archive &ar, const unsigned int version);

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
                void serialize(Archive &ar, const unsigned int version);

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
                void serialize(Archive &ar, const unsigned int version);

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
                void serialize(Archive &ar, const unsigned int version);

            public:
                bool isRegistered() const;

                void setRegistered(bool registered);
            };
        }

        struct Image {
            std::string owner;
            int limit;
        };


        struct Hello : public Authentication::AuthRequest {
        private:
        public:
        };

        //Add image
        struct AddImageRequest : public Authentication::AuthRequest {
        private:
            std::string image_name;
            std::string thumbnail;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);

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

        // View Image
        struct ViewImageRequest : public Control::Error {
        private:
            std::string userName;
            std::string imageName;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);

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
            void serialize(Archive &ar, const unsigned int version);

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
            void serialize(Archive &ar, const unsigned int version);

        public:
            [[nodiscard]] const std::string &getUserName1() const;

            void setUserName1(const std::string &userName);

            [[nodiscard]] const std::string &getImageName() const;

            void setImageName(const std::string &imageName);
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
            void serialize(Archive &ar, const unsigned int version);

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
        public:
        };

        struct RemoveViewerRequest : Authentication::AuthRequest {
        private:
            std::string imageName;
            std::string to_remove;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);

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
            void serialize(Archive &ar, const unsigned int version);

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
            void serialize(Archive &ar, const unsigned int version);

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
            void serialize(Archive &ar, const unsigned int version);
        public:
            int getCurrentIndex() const;

            void setCurrentIndex(int currentIndex);

            const std::unordered_map<std::string, std::string> &getImages() const;

            void setImages(const std::unordered_map<std::string, std::string> &images);
        };
        // Get all messages
        struct SearchRequest : public Authentication::AuthRequest {
        private:
            std::string targetUsername;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);
        public:
            const std::string &getTargetUsername() const;

            void setTargetUsername(const std::string &targetUsername);

        };

        struct SearchReply : public Control::Error {
        private:
            std::map<std::string, Control::realSockAddr> users_ips;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);
        };

        struct SendMessageRequest : public Authentication::AuthRequest {
        private:
            std::string message;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive &ar, const unsigned int version);

        public:
            const std::string &getMessage() const;

            void setMessage(const std::string &message);
        };
    }
}

template<typename T>
std::string save(const T &t);

template<typename T>
T load(const std::string &message);

#endif //PROJ_MESSAGESTRUCTURES_H
