#include <cstdlib>
#include <utility>
#include "../headers/Message.h"
#include "../headers/base64.h"
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>




Message::Message(MessageType messageType, OperationType operation, std::string message, size_t messageSize,
                 RPC_ID rpcId) {
    this->operation = operation;
    this->messageType = messageType;
    this->message = std::move(message);
    this->messageSize = messageSize;
    this->rpcId = RPC_ID(rpcId);
}

Message::Message(const std::string &marshalled_base64) {
    char *data = nullptr;
    int alloc_length = Base64decode_len(marshalled_base64.c_str());
    data = static_cast<char *>(malloc(alloc_length));
    Base64decode(data, marshalled_base64.c_str());
    this->message = std::string(data);
}

std::string Message::marshal() {
    std::string data = static_cast<std::string>(this->message);
    int data_length = this->messageSize;
    int encoded_data_length = Base64encode_len(data_length);
    std::string base64_string = static_cast<std::string>(malloc(encoded_data_length));
    Base64encode(base64_string, data, data_length);
    return NULL;
}

int Message::getOperation() {
    return this->operation;
}

int Message::getRPCId() {
    return this->rpcId;
}

std::string Message::getMessage() {
    return this->message;
}

size_t Message::getMessageSize() {
    return this->messageSize;
}

MessageType Message::getMessageType() {
    return this->messageType;
}

void Message::setOperation(OperationType _operation) {
    this->operation = _operation;
}

void Message::setMessage(std::string message, size_t message_size) {
    this->message = message;
    this->messageSize = message_size;
}

void Message::setMessageType(MessageType message_type) {
    this->messageType = message_type;
}

Message::~Message() {

}
