#include <iostream>
#include <cstring>
#include "headers/Server.h"
#include "headers/Client.h"
#include "headers/DirectoryServer.h"
#include "headers/Peer.h"
#include "headers/Message.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void milestone_2();

void milestone_1();


int main() {
    //  milestone_1();
    milestone_2();
}


void milestone_2() {
    int choice;
    std::cout << "Hello kind sir, Do you want to act as a directory server or a peer?\n"
              << "1: Peer, 2: Directory Server\n";
    std::cin >> choice;
    switch (choice) {
        case 1: {
            //Input IPs and Ports
            std::string peerHostname;
            int  peerPort;
            std::cout << "Enter peer server IPv4:" << std::endl;
            std::cin >> peerHostname;
            std::cout << "Enter peer server port:" << std::endl;
            std::cin >> peerPort;
            std::cin.ignore();
            Peer peer = Peer(peerHostname, peerPort);
            peer.init();
        }
        case 2: {
            std::string listenHostname;
            std::cout << "Enter server listening IPv4: " << std::endl;
            std::cin >> listenHostname;
            DirectoryServer directoryServer = DirectoryServer(listenHostname);
            directoryServer.init();
        }
        default:
            std::cout << "Option is invalid try again";
    }
}

void milestone_1() {
    int choice;
    std::cout << "(Multithreading not enabled)\n"

              << "Hello kind sir, Do you want to act as a client or a server?\n"
              << "1: Client, 2: Server\n";
    std::cin >> choice;
    switch (choice) {
        case 1: {
            while (1) {
                std::string serverHostname;
                int serverPort;
                std::cout << "Enter destination server IPv4:" << std::endl;
                std::cin >> serverHostname;
                std::cout << "Enter destination port:" << std::endl;
                std::cin >> serverPort;
                std::cin.ignore();
                Client c = Client(serverHostname, serverPort);
                while (1) {
                    char *message = static_cast<char *>(malloc(10000000));
                    do {
                        std::cout << "Input the message to be sent to the destination server:\n";
                        std::cin.getline(message, 10000000, '\n');
                    } while (strncmp(message, "", 1) == 0);
                    //strcpy(message, "flood");
                    Message::RPC_ID rpc = Message::RPC_ID();
                    std::ifstream input;
                    input.open("../tests/dragon.jpg");
                    std::string image((std::istreambuf_iterator<char>(input)),
                                      std::istreambuf_iterator<char>());
                    input.close();
                    Message msg = Message(Message::MessageType::Request, Message::OperationType::ECHO, image,
                                          strlen(message), rpc);
                    Message *reply = c.execute(&msg);
                    std::cout << "Reply From Server: " << reply->getMessage() << std::endl;
                    if (strncmp(message, "q", 1) == 0)
                        break;
                }
            }
        }
        case 2: {
            std::string listenHostname;
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
        }
        default:
            std::cout << "Option is invalid try again";
    }
}
#pragma clang diagnostic pop