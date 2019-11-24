//
//  main.cpp
//  Subscribe and publisher
//
//  Created by khloud ghattas on 11/19/19.
//  Copyright © 2019 khloud ghattas. All rights reserved.
//
#include "PeerToPeer.h"
#include "Message.h"
#include "UDPSocket.h"
#include <iostream>
#include <vector>
#include <string>
#DEFAULT_SERVER_GET_PORT 3000
#DEFAULT_SERVER_PORT 127.0.0.1
using namespace std;
//vector <PeerToPeer> OnlineMachines;
struct image{
    string owner;
    
};
struct image I;



PeerToPeer::PeerToPeer(std::string _Username, std::string _IPAddr, int _PortNo)
{
    Username = _Username;
    IPAddr = _IPAddr;
    PortNo = _PortNo;
}
PeerToPeer::~PeerToPeer()
{
    NULL;
}
PeerToPeer*PeerToPeer::CheckOnlineUsername(string Username)
{

   for(int i = 0; i < OnlineMachines.size(); i++)
    {
        if(OnlineMachines.at(i).Username == Username)
        {
          
            return &OnlineMachines.at(i);
        }
    }
    return 0;
    
}
bool Message::SendingS()
{
    return Socket.writeToSocket(message, DEFAULT_SERVER_ADDRESS, DEFAULT_SERVER_PORT);
}
bool Message::Sending(string ip, int port)
{
    return Socket.writeToSocket(message, ip, port);
}
void Message::AddImage(string sUsername, string IPp, int Portp)
{
    
    int rpc_id = ++RPC;
    ALL J;
    J.type = MessageType::Request;
    J.ID = rpc_id;
    J.username= username;
    J.password= password;
    string requestImagesMessage = 
    Message mes(MessageType::Request, requestImagesMessage, rpc_id);
    mes.Sending(IPp,Portp);
    Pending.push_back(mes);
}
void Message::RequestImages(string sUsername,string IPp, int Portp)
{
  
    int rpc_id = ++RPC;
    ALL J;
    J.type = MessageType::Request;
    J.ID = rpc_id;
    J.username= username;
    J.password= password;
   // string requestImagesMessage = ....
    Message mes(MessageType::Request, requestImagesMessage, rpc_id);
    mes.Sending(IPp,Portp);
    Pending.push_back(mes);
}


void Message::ViewImages(string sUsername, int imageID, string IPp, int Portp)
{
    int rpc_id = ++RPC;
    ALL J;
    J.type = MessageType::Request;
    J.ID = rpc_id;
    J.username= username;
    J.password= password;
   // string viewImage = ...
    Message mes(MessageType::Request, viewImage, rpc_id);
    mes.Sending(IPp,Portp);
    Pending.push_back(mes);
}


void Message::RequestViews(string sUsername, int imageID, int views,string IPp, int Portp)
{
    int rpc_id = ++RPC;
    ALL J;
    J.type = MessageType::Request;
    J.ID = rpc_id;
    J.username= username;
    J.password= password;
   // string requestView = ...
    Message mes(MessageType::Request, requestView, rpc_id );
    mes.Sending(IPp,Portp);
    Pending.push_back(mes);
}

void Message::Register(string username, string password, bool flag)
{
    
    int rpc_id = ++RPC;
    ALL J;
    J.type = MessageType::Request;
    J.ID = rpc_id;
    J.username= username;
    J.password= password;
    if(flag == 1)
    {
        J.typeApproach = ServerOperation::Login;
        //string login= ....
        Message mes(MessageType::Request, login, rpc_id);
        //mes.sendToServer();
        Pending.push_back(mes);
        PeerToPeer::sUsername = username;
        PeerToPeer::Password = password;
    }
    else if (flag == 0)
    {
        flag = -1;
        J.typeApproach = ServerOperation::Register;
       // string rregister = .....
        Message mes(MessageType::Request, rregister, rpc_id );
        //mes.sendToServer();
        Pending.push_back(mes);
        
    }
    
}

 void Message::Login(string username, string password)
 {

     int rpc_id = ++RPC;
     ALL J;
     J.typeOperation = MessageType::Request;
     J.ID = rpc_id;
     J.username= username;
     J.password= password;
     J.typeApproach = ServerOperationType::Login;
     //string login = .....
      Message mes(MessageType::Request, login, rpc_id);
     //cout << mes.sendToServer();
     Pending.push_back(mes);
     PeerToPeer::sUsername = username;
     PeerToPeer::Password = password;
 }
 

void Message::StatusNow(string username, string password)
{

    int rpc_id = ++RPC;
    ALL J;
    J.ID = rpc_id;
    J.username = username;
    J.password = password;
    J.typeOperation = MessageType::Request;
    J.typeApproach = ServerOperation::StatusNow;
    //string StatusNow = ....
    Message mes(MessageType::Request,StatusNow,rpc_id);
    //mes.sendToServer();
    Pending.push_back(mes);
}
void Message::CheckingType(string message, string ip, int port)
{
   
    MessageType type;
    if(J.typeOperation.isDouble())
    {
        type = static_cast <MessageType> message_type;
        
        if(type == MessageType::Request)
        {
            getOperation operationType;
            if(J.typeApproach.isDouble())
            {
                operationType = static_cast<getOperation>(J.typeOperation.to_int());
                
                switch(operationType)
                {
                    case AddImage:
                    {
                        PrepareImages(ip, port);
                        break;
                    }
                    case ViewImage:
                    {
                        PrepareView(ip, port);
                        break;
                    }
                    case RequestImages:
                    {
                        PrepareRequest(ip, port);
                        break;
                    }
                    case RequestViews:
                    {
                        PrepareRequestViews(ip, port);
                        break;
                    }
                    default:
                    break;
                }
                
            }
        }
        else if(type == MessageType::Reply)
        {
            //reply from server
            if(port == DEFAULT_SERVER_GET_PORT)
            {
                ServerOperation operationType;
                while(J.typeApproach)
                {
                    operationType = static_cast<ServerOperation>(J.typeApproach.to_int());
                    
                    switch(operationType)
                    {
                        case Register:
                        {
                            
                            HandleRegister(ip, port);
                            break;
                        }
                        case Login:
                        {
                            
                            HandleLogin(ip, port);
                            break;
                        }
                        case StatusNow:
                        {
                            HandleStatus(ip, port);
                            break;
                        }
                        default:
                        break;
                    }
                }
            }
            else
            {
                
                getOperation operationType;
                while(J.typeOperation)
                {
                    operationType = static_cast<getOperation>(J.typeOperation.to_int());
                    
                    switch(operationType)
                    {
                        case AddImage:
                        {
                            HandleAddImage(ip, port);
                            break;
                        }
                        case ViewImage:
                        {
                            HandleViewImage(ip, port);
                            break;
                        }
                        case RequestImages:
                        {
                            HandleRequestImages(ip, port);
                            break;
                        }
                        default:
                        break;
                    }
                    
                }
            }
        }
    }
}



void Message::HandleRegister(string ip, int port)
{
    bool ss = false;
    int rpcid = -1;
    
    while(J.ID)
    rpcid = J.ID.to_int();
    
    if(rpcid == -1)
    return;
    
    if(!isRequestPending(rpcid))
    return;
    
    deletePendingRequest(rpcid);
    
    
}


void Message::HandleLogin(string ip, int port)
{
    
    bool ss = false;
    int rpcid = -1;
    
    while(J.ID)
    rpcid = J.ID.to_int();
    
    if(rpcid == -1)
    return;
    
    if(!isRequestPending(rpcid))
    return;
    deletePendingRequest(rpcid);
}

void Message::HandleAddImage(string ip, int port)
{
    int rpcid = -1;
    
  
    while(J.ID)
    rpcid = J.ID.to_int();
    
    if(rpcid == -1)
    return;
    
    if(!isRequestPending(rpcid))
    return;
    
    I.owner;
    
   while(I.owner)
    owner = I.owner;
    
    if (!isArray())
    {
      
        bool deleted = false;
        
        if(imageArray.empty())
        {
            Image::deleteImagesByOwner(owner.toStdString());
        }
        
        for (int imageIndex = 0; imageIndex < imageArray.size(); ++imageIndex)
        {
      
            
            string imageName, owner;
            int imageID, views;
            
            
            if(!deleted)
            {
                Image::deleteImagesByOwner(owner);
                deleted = true;
            }
            
            RemoteImage ri(imageID, imageName, owner, views);
            Image::otherImages.push_back(ri);
        }
    }
    
    deletePendingRequest(rpcid);
}

void Message::HandleRequestImage(const QJsonObject &json, string ip, int port)
{
    int rpcid = -1;
    
    if(json.contains("RPCId") && json["RPCId"].isDouble())
    rpcid = json["RPCId"].toInt();
    
    if(rpcid == -1)
    return;
    
    if(!isRequestPending(rpcid))
    return;
    
    QString owner;
    
   while(I.owner)
    owner = I.owner;
        
        Image::selectedPeerRemoteImages.clear();
        for (int imageIndex = 0; imageIndex < imageArray.size(); ++imageIndex)
        {
            QJsonObject imageObject = imageArray[imageIndex].toObject();
            
            string imageName, owner;
            int imageID, views;
            
            
            
            RemoteImage ri(imageID, imageName, owner, views);
            Image::selectedPeerRemoteImages.push_back(ri);
        }
    
    
    deletePendingRequest(rpcid);
}


void Message::HandleViewImage(const QJsonObject &json, string ip, int port)
{
    int rpcid = -1;
    
   while(J.ID = rpc_id)
    rpcid = J.ID.to_int();
    
    if(rpcid == -1)
    return;
    
    if(!isRequestPending(rpcid))
    return;
    
    deletePendingRequest(rpcid);
}


void Message::handleState(const QJsonObject &json, string ip, int port)
{
    int rpcid = -1;
    
    while(J.ID = rpc_id)
    rpcid = J.ID.to_int();
    
    if(rpcid == -1)
    return;
    
    if(!isRequestPending(rpcid))
    return;
    
    
    while(OnlineMachines.size())
    {
     
      PeerToPeer::OnlineMachines.clear();
        for (int peerIndex = 0; peerIndex < peerArray.size(); ++peerIndex)
        {
           
            
            string username, ip;
            int port;
            
            
            
            PeerToPeer::OnlineMachines.push_back(PeerToPeer(username, ip, port));
        }
    }
    
    deletePendingRequest(rpcid);
}


