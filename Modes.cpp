#include "Irc.hpp"

int	Polls::modesHandle(Msg& msg)
{
	std::string delim = " ";
	std::vector<std::string> split;
    const char* init[] = {"+k", "-k", "+l", "-l", "+i", "-i", "+t", "-t", "+o", "-o"};
    std::vector<std::string> flags(init, init+ sizeof(init) / sizeof(init[0]));
	for (size_t j=0; msg.command.length() > 0; j++)
	{
		split.push_back(msg.command.substr(0, msg.command.find(delim)));
		msg.command.erase(0, (int)split[j].size() +1);
	}

    //Error handling, a rearranger !!

    //MODE USER
    if (split.size() == 2 && std::find(tab.begin(), tab.end(), split[1]) == tab.end() && split[1] == tab[msg.currentIndex].nickName) {
        msg.response = msg.prefix + " 221 " + split[1] + " " + split[0] + " :Modes activated: \r\n"; } //Afficher les modes actifs de nick: RPL_UMODEIS 221
    else if (split[1] != tab[msg.currentIndex].nickName) {
        msg.response = msg.prefix + " 502 " + split[1] + " " + split[0] + " :Cant change mode for other users\r\n"; } //ERR_USERSDONTMATCH 502
    else if (tab[msg.currentIndex].operators == 0 && split[1].rfind("#", 0)) {
        msg.response = msg.prefix + " 482 " + tab[msg.currentIndex].nickName + " " + split[1] + " :You're not channel operator\r\n"; } //ERR_CHANOPRIVSNEEDED 482

    //BOTH
	else if (((split[2] == "+k" || split [2] == "+l") && split.size() < 4) || split.size() < 3 || split[2].length() < 2) {
        msg.response = msg.prefix + " 461 " + split[1] + " " + split[0] + " :Not enough parameters\r\n"; } //ERR_NEEDMOREPARAMS 461
    else if (((split[2] == "+k" || split [2] == "+l") && split.size() > 4) || split.size() > 3 || split[2].length() > 2) {
        msg.response = msg.prefix + " 407 " + split[1] + " " + split[0] + " :Too much parameters\r\n"; } //ERR_TOOMANYTARGETS 407
    else if (channels.find(split[1]) == channels.end() && std::find(tab.begin(), tab.end(), split[1]) == tab.end()) {
        if (channels.find(split[1]) == channels.end()) {
            msg.response = msg.prefix + " 403 " + split[1] + " " + split[0] + " :No such channel\r\n"; } //ERR_NOSUCHCHANNEL 403
        else if (std::find(tab.begin(), tab.end(), split[1]) == tab.end()) {
            msg.response = msg.prefix + " 401 " + split[1] + " " + split[0] + " :No such nick\r\n"; } }//ERR_NOSUCHNICK 401
    else if (std::find(flags.begin(), flags.end(), split[2]) == flags.end()) {
        msg.response = msg.prefix + " 501 " + split[1] + " " + split[0] + " :Unknown MODE flag\r\n"; } //ERR_UMODEUNKNOWNFLAG 501
    
    //MODE CHANNEL
    else if (split.size() == 2 && channels.find(split[1]) == channels.end() && split[1] == msg.currentChan) {
        msg.response = msg.prefix + " 324 " + tab[msg.currentIndex].nickName + " " + split[1] + " " + split[0] + " :Modes activated: \r\n"; } //Afficher les modes actifs du channel: RPL_CHANNELMODEIS 324

    //Modes handling
    else if (split.size() == 4 && split[2] == "+k")
        { channels[msg.currentChan] = split[3]; std::cout << "Key Mode: activated" << std::endl; } //Add a password to access the channel
    else if (split.size() == 3 && split[2] == "-k")
        {channels[msg.currentChan] = ""; std::cout << "Key Mode: desactivated" << std::endl; }
    else if (split.size() == 4 && split[2] == "+l")
        { limitUsers = atoi(split[3].c_str()); std::cout << "Limit Users: activated at " << limitUsers << std::endl; } //Gerer encore l'acces au serveur en fonction de ca
    else if (split.size() == 3 && split[2] == "-l")
        { limitUsers = 0; std::cout << "Limit Users: desactivated" << std::endl; }
	return(0);
}

int  Polls::channelHandle(Msg& msg)
{
    std::string delim = " ";
	std::vector<std::string> split;
	for (size_t j=0; msg.command.length() > 0; j++) {
		split.push_back(msg.command.substr(0, msg.command.find(delim)));
		msg.command.erase(0, (int)split[j].size() +1);
	}
    if (split.size() != 2 && split.size() != 3)
        return (1);
    // else if (limitUsers != 0 &&  ) // si la limite existe et quelle n'est pas depassee, le client peut join
    else if (channels.find(split[1]) == channels.end()) {
        if (split.size() == 2) {
            channels[split[1]] = ""; } // Entree libre dans le channel
        else if (split.size() == 3) {
            channels[split[1]] = split[2]; // Mot de passe pour rentrer dans le channel
        }
        //ajouter le user dans la map<channel, users>
    }
    else {
        //juste ajouter le user a la map<channel, users>
    }
    // std::cout << "Channel: " << split[1] << "| Mdp: " << channels[split[1]] << "%" << std::endl;
    std::cout << "NEW CHANNEL ENTERING . . . " << std::endl;
    msg.currentChan = split[1];
    return (0);
}