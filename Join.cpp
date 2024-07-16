#include "Server.hpp"

/* 
 * Handle JOIN (back up)
 * Handling of channel list, channel users, channel limit users, current channel, 
 *      channel password or not, channel modes
 * Useful for MODES !!
 * TO HANDLE : limitUsers restrict the channel access !!
*/

// int  Server::channelHandle()
// {
//     STR_LIST split = cutModeCommand();
//     Channel temp(split[1]);
//     if (split.size() != 2 && split.size() != 3)
//         return (1);
//     // else if (limitUsers != 0 &&  ) // si la limite existe et quelle n'est pas depassee, le client peut join
//     else if (!isChanExists(split[1])) { //Creation du channel
//         temp.name = split[1];
//         if (split.size() == 2) {
//             temp.pwd = ""; } // Entree libre dans le channel
//         else {
//             temp.pwd = split[2]; // Mot de passe pour rentrer dans le channel
//         }
//         temp.usersInChan[tab[_msg.currentIndex].nickName] = 1; //Add user into Users channel's list
//         _channels.push_back(temp);
//     }
//     else { //Check if its an INVITE ONLY channel
//         // temp.usersInChan[tab[_msg.currentIndex].nickName] = 0; 
//         tabChan[_msg.currentChan].usersInChan[tab[_msg.currentIndex].nickName] = 0; 
//         std::cout << "coucou je suis la bitch et NICK = " << tab[_msg.currentIndex].nickName << "\n ";
//         // sendToChan(); }
//     }
//     std::cout << "ChanName: " << tabChan[_msg.currentChan].name << " | ChanUsers: ";
//     for (MAP_TAB::iterator it = tabChan[_msg.currentChan].usersInChan.begin(); it != tabChan[_msg.currentChan].usersInChan.end(); ++it) {
//         std::cout << it->first << " "; }
//     std::cout << "\nNEW CHANNEL ENTERING . . . " << std::endl;
//     return (0);
// }