#include "Server.hpp"

/* 
 * Handle JOIN (back up)
 * Handling of channel list, channel users, channel limit users, current channel, 
 *      channel password or not, channel modes, channel invite-only or not
 * Useful for MODES !!
 * TO HANDLE : +l/ +k/ +i !!
*/

int  Server::channelHandle(void)
{
    STR_VEC split = cutModeCommand();
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
        // (+i) Check if its an INVITE ONLY channel
        if (_channels[_msg.currentChan].gPassword() != "")
        {
            // Gestion d'erreurs !
            if (split.size() == 3 && split[2] == _channels[_msg.currentChan].gPassword())
                _channels[_msg.currentChan].addClient(_clients[_msg.currentIndex]);
            else
            {
                _msg.response = _msg.prefixNick + " 475 " + _clients[_msg.currentIndex].getNickname() + " " + _channels[_msg.currentChan].gName() + " :Cannot join channel (+k)\r\n";
                return (1);
            }
        }
    }
    std::cout << "ChanName: " << _channels[_msg.currentChan].gName() << " | ChanUsers: ";
    for (CLIENT_IT it = _channels[_msg.currentChan].gClients().begin(); it != _channels[_msg.currentChan].gClients().end(); ++it) {
        std::cout << it->getNickname() << " "; }
    std::cout << "\nNEW CHANNEL ENTERING . . . " << std::endl;
    
    for (CLIENT_IT it=_channels[_msg.currentChan].gClients().begin(); it != _channels[_msg.currentChan].gClients().end(); it++)
    {
        _msg.response += ":" + _clients[_msg.currentIndex].getNickname() + " JOIN " + _channels[_msg.currentChan].gName() + "\r\n";
        std::cout << GREEN "NICK loop: " << it->getNickname() << NC << std::endl;
        // if (it->getNickname() != _clients[_msg.currentIndex].getNickname())
        sendResponse(it->getFd());
    }

    return (0);
}