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
    this->rpcId = rpcId;
}

Message::Message(char* marshalled_base64) {
    std::string decoded = boost::beast::detail::base64_decode(std::string(marshalled_base64));
    auto msg = load<Message>(decoded);
    *this = msg;
}

std::string Message::marshal() {
    std::string serialized = save<Message>(*this);
    std::string encoded = boost::beast::detail::base64_encode(serialized);
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


bool Message::verifyFragmentation() {
        return (this->message.length()/MAX_MESSAGE_SIZE > 1);
}

Message** Message::fragment() {
    char* buffer = &this->message[0];
    int bufferLen = std::strlen(buffer);
    int factor = bufferLen / MAX_MESSAGE_SIZE;
    Message* msgs[factor];
    if (factor > 1) {
        int fragmentNum = factor;
        while (fragmentNum > 0) {
            char *newBuffer = buffer + (factor - fragmentNum) * MAX_MESSAGE_SIZE;
            RPC_ID rpc = RPC_ID(rpcId.time, rpcId.address, rpcId.portNumber);
            rpc.setFragmentId(factor);
            rpc.setMessageId(1);
            msgs[(factor - fragmentNum)] = new Message(this->messageType, this->operation, newBuffer, bufferLen, rpc);
            fragmentNum--;
        }
    }
    return msgs;
}

bool Message::isFragmented() {
    return this->rpcId.fragmentId != 0;
}
