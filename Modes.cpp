#include "Irc.hpp"

/* Check if the current channel exists in the database tabChan<CHANNEL> */
bool    Polls::isChanExists(std::string target)
{
    int len=0;
    for (CHAN_ITERATOR it=tabChan.begin(); it != tabChan.end(); len++, ++it) {
        if (target == it->name)
        { msg.currentChan = len -1;
            return true; }
    }
    return false;
}

/* Check if the current user exists in the database tab<USER> */
bool    Polls::isUserExists(std::string target)
{
    for (USER_ITERATOR it=tab.begin(); it != tab.end(); ++it) {
        if (target == it->nickName)
            return true;
    }
    return false;
}

/* 
 * Make a message to print channel modes, or not \
*/
template <typename V>
std::string    printModes(V& modes)
{
    std::string currentModes="+";
    for (typename V::iterator it=modes.begin(); it != modes.end(); ++it) {
        currentModes += *it; }
    if (currentModes.length() == 1)
        currentModes = ":No modes are set";
    return (currentModes);
}

/* 
 * Handling Error Modes #2
 * Check if there is enough/ not too much params, if flags are valid
*/
void    Polls::errorLenModes(VEC_LIST& split)
{
    const char* init[] = {"+k", "-k", "+l", "-l", "+i", "-i", "+t", "-t", "+o", "-o"};
    VEC_LIST flags(init, init+ sizeof(init) / sizeof(init[0]));
    // if (!split[2].find("-l") && split[2].length() > 2) {
    //     split.push_back(split[2].substr(2)); }
    if (((split[2] == "+k" || split [2] == "+l") && split.size() < 4) || split.size() < 3 || split[2].length() < 2) {
        msg.response = msg.prefixNick + " 461 " + split[1] + " MODE :Not enough parameters\r\n"; } //ERR_NEEDMOREPARAMS 461
    else if (((split[2] == "+k" || split [2] == "+l") && split.size() > 4) || split.size() > 3 || split[2].length() > 2) { //CHECK WHY HERE ALWAYS ???????????????????????
        msg.response = msg.prefixNick + " 407 " + split[1] + " MODE :Too much parameters\r\n"; } //ERR_TOOMANYTARGETS 407
    else if (std::find(flags.begin(), flags.end(), split[2]) == flags.end()) {
        msg.response = msg.prefixNick + " 501 " + split[1] + " MODE :Unknown MODE flag\r\n"; } //ERR_UMODEUNKNOWNFLAG 501
}

/* 
 * Handling Error Modes #1
 * Check if the channel exists, if the user is an operator
 * Print actives modes from current channel
*/
void    Polls::errorModes(VEC_LIST split)
{
    if (!isChanExists(split[1]) || split[1].find("#", 0)) {
        msg.response = msg.prefixNick + " 403 " + split[1] + " MODE :No such channel\r\n"; } //ERR_NOSUCHCHANNEL 403
    else if (tab[msg.currentIndex].operators == 0) {
        msg.response = msg.prefixNick + " 482 " + tab[msg.currentIndex].nickName + " " + split[1] + " :You're not channel operator\r\n"; } //ERR_CHANOPRIVSNEEDED 482
    else if (split.size() == 2 && isChanExists(split[1]) && split[1] == tabChan[msg.currentChan].name) {
        msg.response = msg.prefixServer + "324 " + tab[msg.currentIndex].nickName + " " + split[1] + \
        printModes(tabChan[msg.currentChan].modes) + "\r\n"; } //Afficher les modes actifs du channel: RPL_CHANNELMODEIS 324
    else {
        errorLenModes(split); }
}

VEC_LIST    Polls::cutModeCommand()
{
    std::string delim = " ";
	VEC_LIST split;
	for (size_t j=0; msg.command.length() != 0; j++)
	{
        if (!msg.command.find("-l")) {
            split.push_back(msg.command.substr(0, 2));
		    msg.command.erase(0, (int)split[j].size());
        }
        else {
    		split.push_back(msg.command.substr(0, msg.command.find(delim)));
		    msg.command.erase(0, (int)split[j].size() +1);}
        std::cout << "split: " << split[j] << "$" << std::endl;
	}
    return split;
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
	VEC_LIST split = cutModeCommand();
    msg.prefixNick = ":" + tab[msg.currentIndex].nickName;
    std::string linkPrint = split[1] + " " + split[2] + " " + split[3];
    
    //Modes handling
    errorModes(split);
    std::cout << "bfore response: " << msg.response << std::endl;
    if (!msg.response.find("MODE"))
    {
        std::cout << "hey toi\n";
        if (split.size() == 4) {
            msg.response = msg.prefixServer + "MODE " + linkPrint + "\r\n"; }
        else if (split.size() == 3) {
            msg.response = msg.prefixServer + "MODE " + split[1] + " " + split[2] + "\r\n"; }
    }
    std::cout << "Response MODE: " << msg.response << std::endl;
    // if (split.size() == 4 && split[2] == "+k")
        // { tabChan[]= split[3]; std::cout << "Key Mode: activated" << std::endl; } //Add a password to access the channel
    // else if (split.size() == 3 && split[2] == "-k")
        // {tabChan[]= ""; std::cout << "Key Mode: desactivated" << std::endl; }
    // else if (split.size() == 4 && split[2] == "+l")
    //     { limitUsers = atoi(split[3].c_str()); std::cout << "Limit Users: activated at " << limitUsers << std::endl; } //Gerer encore l'acces au serveur en fonction de ca
    // else if (split.size() == 3 && split[2] == "-l")
    //     { limitUsers = 0; std::cout << "Limit Users: desactivated" << std::endl; }
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
    else if (!isChanExists(split[1])) {
        msg.currentChan ++;
        temp.name = split[1];
        if (split.size() == 2) {
            temp.pwd = ""; } // Entree libre dans le channel
        else {
            temp.pwd = split[2]; // Mot de passe pour rentrer dans le channel
        }
        temp.usersInChan.push_back(tab[msg.currentIndex].nickName); //Add user into Users channel's list
        tab[msg.currentIndex].operators = 1;
        tabChan.push_back(temp);
    }
    else {
        temp.usersInChan.push_back(tab[msg.currentIndex].nickName); 
        tabChan.push_back(temp); }
    std::cout << "NEW CHANNEL ENTERING . . . " << std::endl;
    return (0);
}