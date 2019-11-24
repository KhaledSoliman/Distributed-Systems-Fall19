#ifndef PROJ_DIRECTORYSERVICE_H
#define PROJ_DIRECTORYSERVICE_H


#include "Client.h"
#include "DirectoryServer.h"

class DirectoryService : DirectoryServer, Client {
public:
    DirectoryService();
    DirectoryService(const std::string &hostname, int port, const std::string &databaseFilePath, const std::string &directoryFilePath);
};


#endif //PROJ_DIRECTORYSERVICE_H
