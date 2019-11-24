#include "Controller.hpp"
#include "Message.h"
#include "PeerToPeer.h"


void Controller::ServerState()
{
    if(PeerToPeer::sUsername.size()>0)
        if(PeerToPeer::Password.size()>0)
    Message::StatusMessage(PeerToPeer::sUsername, PeerToPeer::Password);
}


void Controller::UpdatingImagesFromMachines()
{

    for(int i = 0; i < PeerToPeer::OnlineMachines.size(); i++)
    {
        Message::AddMessage(PeerToPeer::sUsername, PeerToPeer::OnlineMachines.at(i).IPAddr, PeerToPeer::OnlineMachines.at(i).PortNo);
    }
}


