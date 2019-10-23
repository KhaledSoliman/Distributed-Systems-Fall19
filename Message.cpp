
#include <cstdlib>
#include "Message.h"
#include "base64.h"

Message::Message(int operation, char *p_message, size_t p_message_size, int p_rpc_id) {
    this->operation = operation;
    this->message = p_message;
    this->message_size = p_message_size;
    this->rpc_id = p_rpc_id;
}

Message::Message(char *marshalled_base64) {
    char* data = nullptr;
    int data_length;
    int alloc_length = Base64decode_len(marshalled_base64);
    data = static_cast<char *>(malloc(alloc_length));
    data_length = Base64decode(data, marshalled_base64);
    this->message = data;
}

char *Message::marshal() {
    char* data = static_cast<char *>(this->message);
    int data_length = this->message_size;
    int encoded_data_length = Base64encode_len(data_length);
    char* base64_string = static_cast<char *>(malloc(encoded_data_length));
    Base64encode(base64_string, data, data_length);
    return NULL;
}

int Message::getOperation() {
    return this->operation;
}

int Message::getRPCId() {
    return this->rpc_id;
}

char *Message::getMessage() {
    return this->message;
}

size_t Message::getMessageSize() {
    return this->message_size;
}

MessageType Message::getMessageType() {
    return this->message_type;
}

void Message::setOperation(int _operation) {
    this->operation = _operation;
}

void Message::setMessage(char *message, size_t message_size) {
    this->message = message;
    this->message_size = message_size;
}

void Message::setMessageType(MessageType message_type) {
    this->message_type = message_type;
}

Message::~Message() {

}
