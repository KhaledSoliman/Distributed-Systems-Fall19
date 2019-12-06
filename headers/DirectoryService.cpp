#include "DirectoryService.h"

DirectoryService::DirectoryService(const std::string &hostname, int port,const std::string &databaseFilePath, const std::string &directoryFilePath, const std::string& usersFilePath) : DirectoryServer(hostname, port, databaseFilePath, directoryFilePath, usersFilePath)
{

}

DirectoryService::DirectoryService(const std::string &hostname) : DirectoryServer(hostname) {

}

