#include "../../Includes/Server.hpp"
// #include "../../Includes/Channel.hpp"

/* Use of function pointer
 * Access of each mode by their own functions
 * For now, there is only modeO and modeK
*/
void	Server::modesOptions(STR_VEC& split)
{
	std::string init[10] = {"+k", "-k", "+l", "-l", "+i", "-i", "+t", "-t", "+o", "-o"};

	void (Server::*funcPtr[5]) (STR_VEC& split) = { &Server::modeK, &Server::modeL, &Server::modeI, &Server::modeT, &Server::modeO };
	int j = 0;
	for (int i=0; !init[i].empty(); i++) {
		if (init[i] == split[2]) {
			(this->*funcPtr[j])(split);
			return; }
		if (i % 2 != 0)
			j ++;
		}
}

/* 
 * Handle k MODE
 * If +k --> add the password to the vector; add the mode to the channel
 * If -k --> remove the password from the vector; if mode was active, remove it from the channel
*/
void	Server::modeK(STR_VEC& split)
{
	Channel *current = &_channels[_msg.currentChan];

	if (split[2].find("+") != std::string::npos) {
		current->sPwd(split[3]);
		if (!found_mode_in_chan(split[2][1], current->gModes())) {
			current->addMode('k'); }
	}
	else {
		if (found_mode_in_chan(split[2][1], current->gModes())) {
			current->sPwd("");
			current->dltMode('k'); }
	}
}

/* 
 * Handle o MODE
 * Check if the nick chosen exists and is in the channel
 * If +o --> add the operator status to nick; add the mode to the channel;
 * If -o --> remove the operator status to nick; if mode was active, remove it from the channel
*/
void	Server::modeO(STR_VEC& split)
{
	if (is_user_in_chan(split[3], &_channels[_msg.currentChan])) {
		if (split[2].find("+") != std::string::npos) {
			_channels[_msg.currentChan].addOperatorByName(split[3], _clients);
			if (!found_mode_in_chan(split[2][1], _channels[_msg.currentChan].gModes())) {
				_channels[_msg.currentChan].addMode('o'); }
		}
		else {
			_channels[_msg.currentChan].dltOperator(split[3]);
			_channels[_msg.currentChan].dltMode('o');
		}
	}
}
// Just add 'i' to the modList
void			Server::modeI(STR_VEC& split) {
	if (split[2].find("+") != std::string::npos) {
		if (!found_mode_in_chan(split[2][1], _channels[_msg.currentChan].gModes())) {
			_channels[_msg.currentChan].addMode('i'); }
	}
	else {
		_channels[_msg.currentChan].dltMode('i'); }
}

// Just add 't' to the modList
void			Server::modeT(STR_VEC& split) {

	if (split[2].find("+") != std::string::npos) {
		if (!found_mode_in_chan(split[2][1], _channels[_msg.currentChan].gModes())) {
			_channels[_msg.currentChan].addMode('t'); }
	}
	else {
		_channels[_msg.currentChan].dltMode('t'); } 
}

/* Handle L mode
 * Check if its an add or a delete
 * If +l --> add the mode and the limit to the chan
 * If -l --> remove the mode and the limit */
void			Server::modeL(STR_VEC& split)
{
	if (split[2].find("+") != std::string::npos) {
		_channels[_msg.currentChan].sLimit(atoi(split[3].c_str()));
		if (!found_mode_in_chan(split[2][1], _channels[_msg.currentChan].gModes())) {
			_channels[_msg.currentChan].addMode('l'); }
	}
	else {
		if (found_mode_in_chan(split[2][1], _channels[_msg.currentChan].gModes())) {
			_channels[_msg.currentChan].dltMode('l');
			_channels[_msg.currentChan].sLimit(0);
		}
	}
}

void	Server::sendToChan(std::string message)
{
	for (CLIENT_ITC it = _channels[_msg.currentChan].gClients().begin(); it != _channels[_msg.currentChan].gClients().end(); it ++)
	{
			_msg.response = message;
			sendResponse(user_in_chan_fd(it->getNickname(), _channels[_msg.currentChan].gClients()));
	}
}
