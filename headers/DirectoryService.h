#ifndef PROJ_DIRECTORYSERVICE_H
#define PROJ_DIRECTORYSERVICE_H


#include "Client.h"
#include "DirectoryServer.h"

class DirectoryService : public DirectoryServer, Client {
public:
    DirectoryService(const std::string& hostname);
    DirectoryService(const std::string &hostname, int port, const std::string &databaseFilePath, const std::string &directoryFilePath, const std::string&usersFilePath);
};


#endif //PROJ_DIRECTORYSERVICE_H
