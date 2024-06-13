#include "Irc.hpp"

/*
 * Check if the current channel exists in the database tabChan<CHANNEL>
*/
bool    Polls::isChanExists(std::string target)
{
    int len=0;
    for (CHAN_ITERATOR it=tabChan.begin(); it != tabChan.end(); len++, ++it) {
        if (target == it->name)
        { msg.currentChan = len; return true; }
    }
    msg.currentChan = len;
    return false;
}

/*
 * Check if the current user exists in the database tab<USER>
*/
bool    Polls::isUserExists(std::string target)
{
    for (USER_ITERATOR it=tab.begin(); it != tab.end(); ++it) {
        if (target == it->nickName)
            return true;
    }
    return false;
}

bool    Polls::isUserInChan(std::string target)
{
    MAP_TAB::iterator it=tabChan[msg.currentChan].usersInChan.find(target);
    if (it != tabChan[msg.currentChan].usersInChan.end())
        return true;
    return false;
}

bool    Polls::foundModeInChan(std::string mod, VEC_LIST modList)
{
    for (VEC_LIST::iterator it=modList.begin(); it != modList.end(); ++it) {
        if (*it == mod)
            return true;
    }
    return false;
}

/* 
 * Check if the third arg is +k, +l or +/-o
*/
//probablement un probleme !!
bool    Polls::isFourArgs(VEC_LIST& split)
{
    if ((split[2] == "+k" || split[2] == "+l" || split[2] == "+o" || split[2] == "-o"))
        return true;
    std::cout << "je susi la bitch\n";
    return false;
}

/* 
 * Get the command line and split it per part
 * Exception when the third arg == -l --> forced split
*/
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
	}
    return split;
}

