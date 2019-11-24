#ifndef DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H
#define DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H

#include <cstdio>
#include <string>

class Message {
public:
    enum MessageType {
        Request = 0,
        Reply = 1
    };
    enum OperationType {
        ECHO,
        ACK,
        ERROR,
        HELLO,
        FEED,
        LOGIN,
        LOGOUT,
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
        time_t time;
        std::string address;
        int portNumber;

        RPC_ID();
        RPC_ID(time_t time, std::string address, int portNumber);
        template<class Archive>
        void serialize(Archive &ar, const unsigned int version){
            ar & address & portNumber & time;
        }
    };

    Message(MessageType messageType, OperationType operation, std::string message, size_t messageSize, RPC_ID rpcId);

    Message();

    Message(char* marshalled_base64);

    std::string marshal();

    int getOperation();

    RPC_ID getRPCId();

    std::string getMessage();

    size_t getMessageSize();

    MessageType getMessageType();

    void setOperation(OperationType _operation);

    void setMessage(std::string message, size_t message_size);

    void setMessageType(MessageType message_type);

    template<class Archive>
    void serialize(Archive & ar, const unsigned int /* file_version */) {
        ar & messageType & operation & message & messageSize & rpcId;
    }

    ~Message();

private:
    MessageType messageType;
    OperationType operation;
    std::string message;
    size_t messageSize;
    RPC_ID rpcId;
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H
