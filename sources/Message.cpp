#include <cstdlib>
#include <utility>
#include "../headers/Message.h"
#include "../headers/MessageStructures.h"
#include <boost/beast/core/detail/base64.hpp>

Message::Message(Message::MessageType messageType, Message::OperationType operation, std::string message, size_t messageSize,
                 Message::RPC_ID rpcId) {
    this->operation = operation;
    this->messageType = messageType;
    this->message = std::move(message);
    this->messageSize = messageSize;
    this->rpcId = RPC_ID(rpcId);
}

Message::Message(char* marshalled_base64) {
    std::string decoded = boost::beast::detail::base64_decode(std::string(marshalled_base64));
    auto msg = load<Message>(decoded);
    *this = msg;
}

std::string Message::marshal() {
    std::string serialized = save<Message>(*this);
    std::string encoded = boost::beast::detail::base64_decode(serialized);
    return encoded;
}

int Message::getOperation() {
    return this->operation;
}

Message::RPC_ID Message::getRPCId() {
    return this->rpcId;
}

std::string Message::getMessage() {
    return this->message;
}

size_t Message::getMessageSize() {
    return this->messageSize;
}

Message::MessageType Message::getMessageType() {
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

Message::Message() {}
