#include "Server.hpp"

/* 
 * Handling Error Modes #2
 * Check if there is enough/ not too much params, if flags are valid
*/
void    Server::errorLenModes(STR_VEC& split)
{
    const char* init[] = {"+k", "-k", "+l", "-l", "+i", "-i", "+t", "-t", "+o", "-o"};
    STR_VEC flags(init, init+ sizeof(init) / sizeof(init[0]));
    if ((isFourArgs(split) && split.size() < 4) || split.size() < 3 || split[2].length() < 2) {
        _msg.response = _msg.prefixNick + " 461 " + split[1] + " MODE :Not enough parameters\r\n"; } //ERR_NEEDMOREPARAMS 461
    else if ((isFourArgs(split) && split.size() > 4) || (!isFourArgs(split) && split.size() > 3)) {
        _msg.response = _msg.prefixNick + " 407 " + split[1] + " MODE :Too much parameters\r\n"; } //ERR_TOOMANYTARGETS 407 // BIG PROBLEM dans les conditions je pense !!
    else if (std::find(flags.begin(), flags.end(), split[2]) == flags.end()) {
        _msg.response = _msg.prefixNick + " 501 " + split[1] + " MODE :Unknown MODE flag\r\n"; } //ERR_UMODEUNKNOWNFLAG 501
}

/* 
 * Handling Error Modes #1
 * Check if the channel exists, if the user is an operator
 * Print actives modes from current channel
*/
void    Server::errorModes(STR_VEC& split)
{
    if (!isChanExists(split[1]) || split[1].find("#", 0)) {
        _msg.response = _msg.prefixNick + " 403 " + split[1] + " MODE :No such channel\r\n"; } //ERR_NOSUCHCHANNEL 403
    // else if (tabChan[_msg.currentChan].usersInChan[tab[_msg.currentIndex].nickName] == 0) {
        // _msg.response = _msg.prefixNick + " 482 " + tab[_msg.currentIndex].nickName + " " + split[1] + " :You're not channel operator\r\n"; } //ERR_CHANOPRIVSNEEDED 482
    // else if (split.size() == 2 && isChanExists(split[1]) && split[1] == _channels[_msg.currentChan].gName()) {
        // _msg.response = _msg.prefixServer + "324 " + _clients[_msg.currentIndex].getNickname() + " " + split[1] + 
        //printModes(_channels[_msg.currentChan].gModes()) + "\r\n"; } //Afficher les modes actifs du channel: RPL_CHANNELMODEIS 324
    else {
        errorLenModes(split); }
}

/* 
 * Main handling MODES
 * First, there is a command split
 * 
 * GOOD TO KNOW:
    * There is 2 types of Modes: CHANNEL and USER
    * We use ONLY the CHANNEL mode
    * Errors Modes with a code
    * Modes +/-k, +/-l, +/-i, +/-t, +/-o to handle
 * Add User/Channel modes to their respective structures
*/
int	Server::modesHandle(void)
{
    _msg.response = "";
	STR_VEC split = cutModeCommand();
    // std::cout << "CHECK MODES: " << split.back() << " and size : " << split.size() << std::endl;
    _msg.prefixNick = ":" + _clients[_msg.currentIndex].getNickname();
    std::string linkPrint = split[1] + " " + split[2] + " " + split[3];
    
    //Modes handling
    errorModes(split);
    if (_msg.response.find("MODE") == std::string::npos)
    {
        //Traitement de mode option
        modesOptions(split);

        //Affichage pour le client 
        if (split.size() == 4) {
            _msg.response = _msg.prefixServer + "MODE " + linkPrint + "\r\n"; }
        else if (split.size() == 3) {
            _msg.response = _msg.prefixServer + "MODE " + split[1] + " " + split[2] + "\r\n"; }
        
    }
    std::cout << "Response MODE: " << _msg.response << std::endl;
    for (STR_VEC::iterator it=split.begin(); it != split.end(); ++it) {
        it = split.erase(it);
    }
	return(0);
}

/* 
 * Handle JOIN (back up)
 * Handling of channel list, channel users, channel limit users, current channel, 
 *      channel password or not, channel modes
 * Useful for MODES !!
 * TO HANDLE : limitUsers restrict the channel access !!
*/
// int  Server::channelHandle()
// {
//     STR_VEC split = cutModeCommand();
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