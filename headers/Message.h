#ifndef DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H
#define DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H

#include <cstdio>
#include <string>
#include <boost/date_time/posix_time/time_serialize.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/serialization/access.hpp>

#define MAX_MESSAGE_SIZE 4096
#define MAX_READ_MESSAGE_SIZE 10240
#define USER_MESSAGE_MAX_SIZE 10000000

class Message {
public:
    enum MessageType {
        Request = 0,
        Reply = 1
    };

    enum OperationType {
        ECHO,
        ACK,
        HELLO,
        FEED,
        LOGIN,
        LOGOUT,
        SEARCH,
        REGISTER,
        ADD_IMAGE,
        DOWNLOAD_IMAGE,
        VIEW_IMAGE,
        DELETE_IMAGE,
        ADD_VIEWER,
        REMOVE_VIEWER,
        GET_REQUESTS,
        UPDATE_VIEW_LIMIT,
    };

    struct RPC_ID {
        static int currentMessageId;
        int messageId;
        boost::posix_time::ptime time;
        bool fragmented;
        int fragmentId;
        std::string address;
        int portNumber;

        RPC_ID() {
            this->messageId = RPC_ID::incrementMessageId();
            this->fragmentId = 0;
            this->fragmented = false;
            this->time = boost::posix_time::second_clock::local_time();
        }

        RPC_ID(const std::string& address, int portNumber) {
            this->address = address;
            this->portNumber = portNumber;
            this->messageId = RPC_ID::incrementMessageId();
            this->fragmentId = 0;
            this->fragmented = false;
            this->time = boost::posix_time::second_clock::local_time();
        }

        const boost::posix_time::ptime &getTime() const {
            return time;
        }

        void setTime(const boost::posix_time::ptime &time) {
            this->time = time;
        }

        const std::string &getAddress() const {
            return address;
        }

        void setAddress(const std::string &address) {
            this->address = address;
        }

        int getPortNumber() const {
            return portNumber;
        }

        void setPortNumber(int portNumber) {
            this->portNumber = portNumber;
        }

        static int getCurrentMessageId() {
            return currentMessageId;
        }

        static void setCurrentMessageId(int currentMessageId) {
            RPC_ID::currentMessageId = currentMessageId;
        }

        int static incrementMessageId() {
            return RPC_ID::currentMessageId++;
        }

        bool isFragmented() const {
            return fragmented;
        }

        void setFragmented(bool fragmented) {
            this->fragmented = fragmented;
        }

        int getFragmentId() const {
            return fragmentId;
        }

        void setFragmentId(int fragmentId) {
            this->fragmentId = fragmentId;
        }

        int getMessageId() const {
            return messageId;
        }

        void setMessageId(int messageId) {
            this->messageId = messageId;
        }

    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & messageId & address & portNumber & time & fragmentId & fragmented;
        }
    };


    Message(MessageType messageType, OperationType operation, std::string message, size_t messageSize, RPC_ID rpcId);

    Message();

    explicit Message(char *marshalled_base64);

    std::string marshal();

    int getOperation();

    RPC_ID getRPCId();

    std::string getMessage();

    size_t getMessageSize();

    MessageType getMessageType();

    void setOperation(OperationType _operation);

    void setMessage(std::string message, size_t message_size);

    void setMessageType(MessageType message_type);

    static bool verifyFragmentation(const std::string &marshalled);

    static std::vector<std::string> split(const std::string &str, int splitLength);

    std::vector<Message> fragment(std::string &marshalled);

    ~Message();

private:
    MessageType messageType;
    OperationType operation;
    std::string message;
    size_t messageSize;
    RPC_ID rpcId;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int /* file_version */) {
        ar & messageType & operation & message & messageSize & rpcId;
    }

};


#endif //DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H

