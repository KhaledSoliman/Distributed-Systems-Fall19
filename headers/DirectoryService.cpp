#include "DirectoryService.h"

DirectoryService::DirectoryService(const std::string &hostname, int port,const std::string &databaseFilePath, const std::string &directoryFilePath) : DirectoryServer(hostname, port, databaseFilePath, directoryFilePath)
{

}
