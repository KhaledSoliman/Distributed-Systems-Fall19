#include <iostream>
#include <cstring>
#include "headers/Server.h"
#include "headers/Client.h"

int main() {
    int choice;
    std::cout << "(Multithreading not enabled)\n"
              << "Hello kind sir, Do you want to act as a client or a server?\n"
              << "1: Client, 2: Server\n";
    std::cin >> choice;
    switch (choice) {
        case 1: {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
            while(1) {
                char *serverHostname = static_cast<char *>(malloc(300));
                int serverPort;
                std::cout << "Enter destination server IPv4:" << std::endl;
                std::cin >> serverHostname;
                std::cout << "Enter destination port:" << std::endl;
                std::cin >> serverPort;
                std::cin.ignore();
                Client c = Client(serverHostname, serverPort);
                while (1) {
                    char *message = static_cast<char *>(malloc(300));
                    do {
                        std::cout << "Input the message to be sent to the destination server:\n";
                        std::cin.getline(message, 300, '\n');
                    } while (strncmp(message, "", 1) == 0);
                    //strcpy(message, "flood");
                    Message msg = Message(0, message, strlen(message), 1);
                    msg.setMessageType(MessageType::Request);
                    c.execute(&msg);
                    if (strncmp(message, "q", 1) == 0)
                        break;
                }
            }
        }
        case 2: {
            char *listenHostname = static_cast<char *>(malloc(300));
            int listenPort;
            std::cout << "Enter server listening IPv4: " << std::endl;
            std::cin >> listenHostname;
            std::cout << "Enter desired Port to listen on: " << std::endl;
            std::cin >> listenPort;
            Server s = Server(listenHostname, listenPort);
            std::cout << "Listening for requests from clients...\n";
            while (1) {
                s.serveRequest();
            }
#pragma clang diagnostic pop
        }
        default:
            std::cout << "Option is invalid try again";
    }
}
