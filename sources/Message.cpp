#include <cstdlib>
#include <utility>
#include "../headers/Message.h"
#include "../headers/MessageStructures.h"
#include <boost/beast/core/detail/base64.hpp>

Message::Message(Message::MessageType messageType, Message::OperationType operation, std::string message,
                 size_t messageSize,
                 Message::RPC_ID rpcId) {
    this->operation = operation;
    this->messageType = messageType;
    this->message = std::move(message);
    this->messageSize = messageSize;
    this->rpcId = rpcId;
}

Message::Message(char *marshalled_base64) {
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


bool Message::verifyFragmentation(const std::string &marshalled) {
    return (marshalled.length() / MAX_MESSAGE_SIZE > 1);
}

std::vector<std::string> Message::split(const std::string& str, int splitLength)
{
    int NumSubstrings = str.length() / splitLength;
    std::vector<std::string> ret;

    for (auto i = 0; i < NumSubstrings; i++)
    {
        ret.push_back(str.substr(i * splitLength, splitLength));
    }

    // If there are leftover characters, create a shorter item at the end.
    if (str.length() % splitLength != 0)
    {
        ret.push_back(str.substr(splitLength * NumSubstrings));
    }


    return ret;
}

std::vector<Message *> Message::fragment(std::string &marshalled) {
    std::vector<Message *> msgs;
    std::vector<std::string> newBuffers = Message::split(marshalled, MAX_MESSAGE_SIZE);
    int i = 0;
    for(const std::string& newBuffer: newBuffers) {
        RPC_ID rpc = RPC_ID(rpcId.address, rpcId.portNumber);
        rpc.setFragmented(true);
        rpc.setFragmentId(i);
        rpc.setMessageId(1);
        msgs.push_back(new Message(this->messageType, this->operation, newBuffer, marshalled.length(), rpc));
        i++;
    }
    return msgs;
}

int Message::RPC_ID::currentMessageId = 0;
