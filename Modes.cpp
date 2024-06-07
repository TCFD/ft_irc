#include "Irc.hpp"

int	Polls::modesHandle(std::string& response, int index, std::string command, std::string chann)
{
	std::string delim = " ";
	std::vector<std::string> split; 
	for (size_t j=0; command.length() > 0; j++)
	{
		split.push_back(command.substr(0, command.find(delim)));
		command.erase(0, (int)split[j].size() +1);
	}
	if (split.size() < 3 || split[1] != tab[index].nickName)
	{
		std::cout << "Error case" << std::endl;
		return (1); } //Error case to handle: when commmand is more or less than 3, or if the nickname is not the one of the message sender
    else if (split.size() == 4 && split[2] == "+k")
        { channels[chann] = split[3]; std::cout << "Key Mode: activated" << std::endl; } //Add a password to access the channel
    else if (split.size() == 3 && split[2] == "-k")
        {channels[chann] = ""; std::cout << "Key Mode: desactivated" << std::endl; }
    else if (split.size() == 4 && split[2] == "+l")
        { limitUsers = atoi(split[3].c_str()); std::cout << "Limit Users: activated at " << limitUsers << std::endl; } //Gerer encore l'acces au serveur en fonction de ca
    else if (split.size() == 3 && split[2] == "-l")
        { limitUsers = 0; std::cout << "Limit Users: desactivated" << std::endl; }
	return(0);
}

std::string  Polls::channelHandle(int index, std::string command)
{
    (void) index;
    std::string delim = " ";
	std::vector<std::string> split;
	for (size_t j=0; command.length() > 0; j++) {
		split.push_back(command.substr(0, command.find(delim)));
		command.erase(0, (int)split[j].size() +1);
	}
    if (split.size() != 2 && split.size() != 3)
        return (NULL);
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
    std::cout << "Channel: " << split[1] << "| Mdp: " << channels[split[1]] << "%" << std::endl;
    return (split[1]);
}