#ifndef DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H
#define DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H

#include <cstdio>
#include <string>
#include <boost/date_time/posix_time/time_serialize.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/serialization/access.hpp>

#define MAX_MESSAGE_SIZE 10240

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
        UPDATE_VIEW_LIMIT,
    };

    struct RPC_ID {
        boost::posix_time::ptime time;
        std::string address;
        int portNumber;
        int fragmentId;
        int messageId;

        RPC_ID() {}

        RPC_ID(boost::posix_time::ptime time, std::string address, int portNumber) {
            this->time = time;
            this->address = address;
            this->portNumber = portNumber;
        }

        int getFragmentId() const {
            return fragmentId;
        }

        void setFragmentId(int fragmentId) {
            RPC_ID::fragmentId = fragmentId;
        }

        int getMessageId() const {
            return messageId;
        }

        void setMessageId(int messageId) {
            RPC_ID::messageId = messageId;
        }

    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & address & portNumber & time;
        }
    };

    Message(MessageType messageType, OperationType operation, std::string message, size_t messageSize, RPC_ID rpcId);

    Message();

    Message(char *marshalled_base64);

    std::string marshal();

    int getOperation();

    RPC_ID getRPCId();

    std::string getMessage();

    size_t getMessageSize();

    MessageType getMessageType();

    void setOperation(OperationType _operation);

    void setMessage(std::string message, size_t message_size);

    void setMessageType(MessageType message_type);

    bool verifyFragmentation();

    Message** fragment();


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
