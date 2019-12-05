#include "DirectoryService.h"

bool Done = false;
DirectoryService::DirectoryService(const std::string &hostname, int port,const std::string &databaseFilePath, const std::string &directoryFilePath) : DirectoryServer(hostname, port, databaseFilePath, directoryFilePath)
{
// sending a request!
}
LoginReply loginUser(std::string username,std::string password)
{
     bool Done = true;
     int idRPC = -1;
     if(username == username1)
        rpcid = username.is_string();
    if(rpcid == -1)
        return;
    if(!isRequestPending(username))
        return;
    if()
        success = json["Success"].toBool();

    deletePendingRequest(rpcid);
}
void handleRequest(Message message)
{




}