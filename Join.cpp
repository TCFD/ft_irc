#include "Server.hpp"

/* 
 * Handle JOIN (back up)
 * Handling of channel list, channel users, channel limit users, current channel, 
 *      channel password or not, channel modes
 * Useful for MODES !!
 * TO HANDLE : limitUsers restrict the channel access !!
*/

int  Server::channelHandle(void)
{
    STR_LIST split = cutModeCommand();
    Channel temp(split[1]);
    if (split.size() != 2 && split.size() != 3)
        return (1);
    // else if (limitUsers != 0 &&  ) // si la limite existe et quelle n'est pas depassee, le client peut join
    else if (!isChanExists(split[1])) { //Creation du channel
        temp.sName(split[1]);
        if (split.size() == 2) {
            temp.sPwd(""); } // Entree libre dans le channel
        else {
            temp.sPwd(split[2]); // Mot de passe pour rentrer dans le channel
        }
        temp.addClient(_clients[_msg.currentIndex]);
        // temp.usersInChan[tab[_msg.currentIndex].nickName] = 1; //Add user into Users channel's list
        _channels.push_back(temp);
    }
    else { 
        //Check if its an INVITE ONLY channel
        // temp.usersInChan[tab[_msg.currentIndex].nickName] = 0; 
        _channels[_msg.currentChan].addClient(_clients[_msg.currentIndex]);
        // tabChan[_msg.currentChan].usersInChan[tab[_msg.currentIndex].nickName] = 0; 
        // std::cout << "coucou je suis la bitch et NICK = " << tab[_msg.currentIndex].nickName << "\n ";
        // sendToChan(); }
    }
    std::cout << "ChanName: " << _channels[_msg.currentChan].gName() << " | ChanUsers: ";
    for (CLIENT_IT it = _channels[_msg.currentChan].gClients().begin(); it != _channels[_msg.currentChan].gClients().end(); ++it) {
        std::cout << it->getNickname() << " "; }
    std::cout << "\nNEW CHANNEL ENTERING . . . " << std::endl;
    return (0);
}