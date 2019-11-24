//
//  PeerToPeer.h
//  Subscribe
//
//  Created by khloud ghattas on 11/19/19.
//  Copyright © 2019 khloud ghattas. All rights reserved.
//

#ifndef PeerToPeer_h
#define PeerToPeer_h
#include <vector>
#include <string>
#include "Message.h"

using namespace std;
class PeerToPeer
{
    public:
    struct ALL
    {
        int typeOperation;
        int typeApproach;
        int ID;
        string username;
        string password;
    };
    struct ALL J;
    string sUsername;
    string Password;
    vector <PeerToPeer> OnlineMachines;
    string Username;
    string IPAddr;
    int PortNo;
    PeerToPeer*CheckOnlineUsername(string Username);
    PeerToPeer(string _Username, string _IPAddr, int _PortNo);
    PeerToPeer();
    ~PeerToPeer();
     Message(MessageType message_type, char* message, int rpc_id);
     bool Sending(string ip, int port);
     void AddImage(string sUsername, string IPp, int Portp);
     void ViewImage(string sUsername, int imageID, string IPp, int Portp);
     void RequestImages(string sUsername, string IPp, int Portp);
     void RequestViews(string sUsername, int imageID, int views, string IPp, int Portp);
    
    
    
  // void prepareRequestAllowedImagesMessage(string ip, int port);
   //void prepareViewImageMessage( string ip, int port);
  // void prepareRequestImagesMessage( string ip, int port);
  // void prepareRequestViewsMessage( string ip, int port);
  // void CheckingType(string message, string ip, int port);
   // bool sendToServer();
     void Register(string username , string password, bool flag);
     void Login(string username, string password);
     void StatusNow(string username, string password);
     void CheckingType(string message, string ip, int port);
    
    void HandleRegister(string ip, int port);
    void HandleLogin(string ip, int port);
    void HandleStatus(string ip, int port);
    void HandleAddImages(string ip, int port);
    void HandleViewImage(string ip, int port);
    void HandleRequestImages(string ip, int port);

    
    
    
};

#endif /* PeerToPeer_h */
