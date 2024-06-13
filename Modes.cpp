#include "Irc.hpp"

/* 
 * Handling Error Modes #2
 * Check if there is enough/ not too much params, if flags are valid
*/
void    Polls::errorLenModes(VEC_LIST& split)
{
    const char* init[] = {"+k", "-k", "+l", "-l", "+i", "-i", "+t", "-t", "+o", "-o"};
    VEC_LIST flags(init, init+ sizeof(init) / sizeof(init[0]));
    std::cout << "CHECK MODES: " << isFourArgs(split) << " and size : " << split.size() << std::endl;
    if ((isFourArgs(split) && split.size() < 4) || split.size() < 3 || split[2].length() < 2) {
        msg.response = msg.prefixNick + " 461 " + split[1] + " MODE :Not enough parameters\r\n"; } //ERR_NEEDMOREPARAMS 461
    else if ((isFourArgs(split) && split.size() > 4) || (isFourArgs(split) == 0 && split.size() > 3)) {
        msg.response = msg.prefixNick + " 407 " + split[1] + " MODE :Too much parameters\r\n"; } //ERR_TOOMANYTARGETS 407 // BIG PROBLEM dans les conditions je pense !!
    else if (std::find(flags.begin(), flags.end(), split[2]) == flags.end()) {
        msg.response = msg.prefixNick + " 501 " + split[1] + " MODE :Unknown MODE flag\r\n"; } //ERR_UMODEUNKNOWNFLAG 501
}

/* 
 * Handling Error Modes #1
 * Check if the channel exists, if the user is an operator
 * Print actives modes from current channel
*/
void    Polls::errorModes(VEC_LIST& split)
{
    if (!isChanExists(split[1]) || split[1].find("#", 0)) {
        msg.response = msg.prefixNick + " 403 " + split[1] + " MODE :No such channel\r\n"; } //ERR_NOSUCHCHANNEL 403
    else if (tabChan[msg.currentChan].usersInChan[tab[msg.currentIndex].nickName] == 0) {
        msg.response = msg.prefixNick + " 482 " + tab[msg.currentIndex].nickName + " " + split[1] + " :You're not channel operator\r\n"; } //ERR_CHANOPRIVSNEEDED 482
    else if (split.size() == 2 && isChanExists(split[1]) && split[1] == tabChan[msg.currentChan].name) {
        msg.response = msg.prefixServer + "324 " + tab[msg.currentIndex].nickName + " " + split[1] + \
        printModes(tabChan[msg.currentChan].modes) + "\r\n"; } //Afficher les modes actifs du channel: RPL_CHANNELMODEIS 324
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
int	Polls::modesHandle()
{
    msg.response = "";
	VEC_LIST split = cutModeCommand();
    msg.prefixNick = ":" + tab[msg.currentIndex].nickName;
    std::string linkPrint = split[1] + " " + split[2] + " " + split[3];
    
    //Modes handling
    errorModes(split);
    if (msg.response.find("MODE") == std::string::npos)
    {
        //Traitement de mode option
        modesOptions(split);

        //Affichage pour le client 
        if (split.size() == 4) {
            msg.response = msg.prefixServer + "MODE " + linkPrint + "\r\n"; }
        else if (split.size() == 3) {
            msg.response = msg.prefixServer + "MODE " + split[1] + " " + split[2] + "\r\n";}
        
    }
    std::cout << "Response MODE: " << msg.response << std::endl;
	return(0);
}

/* 
 * Handle JOIN (back up)
 * Handling of channel list, channel users, channel limit users, current channel, 
 *      channel password or not, channel modes
 * Useful for MODES !!
 * TO HANDLE : limitUsers restrict the channel access !!
*/
int  Polls::channelHandle()
{
    VEC_LIST split = cutModeCommand();
    Channel temp;
    if (split.size() != 2 && split.size() != 3)
        return (1);
    // else if (limitUsers != 0 &&  ) // si la limite existe et quelle n'est pas depassee, le client peut join
    else if (!isChanExists(split[1])) { //Creation du channel
        temp.name = split[1];
        if (split.size() == 2) {
            temp.pwd = ""; } // Entree libre dans le channel
        else {
            temp.pwd = split[2]; // Mot de passe pour rentrer dans le channel
        }
        temp.usersInChan[tab[msg.currentIndex].nickName] = 1; //Add user into Users channel's list
        tabChan.push_back(temp);
    }
    else { //Check if its an INVITE ONLY channel
        temp.usersInChan[tab[msg.currentIndex].nickName] = 0; 
        tabChan.push_back(temp); }
    std::cout << "NEW CHANNEL ENTERING . . . " << std::endl;
    return (0);
}