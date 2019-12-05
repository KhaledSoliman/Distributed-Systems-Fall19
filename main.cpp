#include <iostream>
#include <cstring>
#include "headers/Server.h"
#include "headers/Client.h"
#include <boost/date_time/posix_time/time_serialize.hpp>

int main() {
    std::ifstream in;
    in.open("/Users/khloudghattas/CLionProjects/Distributed-Systems-Fall19/sources/Space.jpg");
    std::string image((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());
    std::ofstream out;
    out.open("/Users/khloudghattas/CLionProjects/Distributed-Systems-Fall19/sources/Space-out.jpg");
    out << image;
    out.close();
    in.close();
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
                std::string serverHostname;
                int serverPort;
                std::cout << "Enter destination server IPv4:" << std::endl;
                std::cin >> serverHostname;
                std::cout << "Enter destination port:" << std::endl;
                std::cin >> serverPort;
                std::cin.ignore();
                Client c = Client(serverHostname, serverPort);
                while (1) {
                    char* message = static_cast<char*>(malloc(1024));
                    do {
                        std::cout << "Input the message to be sent to the destination server:\n";
                        std::cin.getline(message, 1024, '\n');
                    } while (strncmp(message, "", 1) == 0);
                    //strcpy(message, "flood");
                    Message::RPC_ID rpc = Message::RPC_ID(boost::posix_time::second_clock::local_time(), "1212", 500);
                    Message msg = Message(Message::MessageType::Request, Message::OperationType::ECHO, message, strlen(message), rpc);
                    Message * reply = c.execute(&msg);
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
#pragma clang diagnostic pop
        }
        default:
            std::cout << "Option is invalid try again";
    }
}
