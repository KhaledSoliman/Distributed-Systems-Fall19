#ifndef DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H
#define DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H

#include <cstdio>

enum MessageType {
    Request, Reply
};

class Message {
private:
    MessageType message_type;
    int operation;
    char* message;
    size_t message_size;
    int rpc_id;
public:
    Message(int operation, char *p_message, size_t p_message_size, int p_rpc_id);

    Message(char *marshalled_base64);

    char *marshal();

    int getOperation();

    int getRPCId();

    char *getMessage();

    size_t getMessageSize();

    MessageType getMessageType();

    void setOperation(int _operation);

    void setMessage(char *message, size_t message_size);

    void setMessageType(MessageType message_type);

    ~Message();
};

#endif //DISTRIBUTED_SYSTEMS_FALL19_MESSAGE_H
