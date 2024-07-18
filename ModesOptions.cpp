#include "Server.hpp"
#include "Channel.hpp"

/* Use of function pointer
 * Access of each mode by their own functions
 * For now, there is only modeO and modeK
*/
void    Server::modesOptions(STR_VEC& split)
{
    std::string init[10] = {"+k", "-k", "+l", "-l", "+i", "-i", "+t", "-t", "+o", "-o"};

    void (Server::*funcPtr[5]) (STR_VEC& split) = { &Server::modeK, &Server::modeL, &Server::modeI, &Server::modeT, &Server::modeO };
    int j = 0;
    for (int i=0; !init[i].empty(); i++) {
        // std::cout << "split[2] = " << split[2] << " | and init[i] == " << init[i] << std::endl;
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
void    Server::modeK(STR_VEC& split)
{
    // sendToChan();
    // std::cout << RED "N* CHAN: " << _msg.currentChan << NC << std::endl;
    // std::cout << CYAN "Je suis ici et split vaut : " << split[2] << NC << std::endl;
    if (split[2].find("+") != std::string::npos) {
        _channels[_msg.currentChan].sPwd(split[3]);
        if (!foundModeInChan(split[2].substr(1), _channels[_msg.currentChan].gModes())) {
            _channels[_msg.currentChan].addMode('k');
            // MODES_VEC modes = _channels[_msg.currentChan].gModes();
            // for (MODES_VEC::const_iterator it = modes.begin(); it != modes.end(); it++)
            //     std::cout << "Mode active: " << *it << std::endl; 
        }
    }
    else {
        if (foundModeInChan(split[2].substr(1), _channels[_msg.currentChan].gModes())) {
            _channels[_msg.currentChan].sPwd("");
            _channels[_msg.currentChan].dltMode('k'); }
    }
}

/* 
 * Handle o MODE
 * Check if the nick chosen exists and is in the channel
 * If +o --> add the operator status to nick; add the mode to the channel;
 * If -o --> remove the operator status to nick; if mode was active, remove it from the channel
*/
void    Server::modeO(STR_VEC& split)
{
    if (isUserInChan(split[3], _channels, _msg.currentChan)) {
        if (split[2].find("+")) {
            _channels[_msg.currentChan].addOperator(_clients[_msg.currentIndex]);
            if (!foundModeInChan(split[2].substr(1), _channels[_msg.currentChan].gModes())) {
                _channels[_msg.currentChan].addMode('o'); }
        }
        else {
            if (foundModeInChan(split[2].substr(1), _channels[_msg.currentChan].gModes())) {
                _channels[_msg.currentChan].dltOperator(_clients[_msg.currentIndex]);
                _channels[_msg.currentChan].dltMode('o'); }
        }
    }
}

void            Server::modeI(STR_VEC& split) {(void)split;}

void            Server::modeT(STR_VEC& split) {(void)split;}

/* Handle L mode
 * Check if its an add or a delete
 * If +l --> add the mode and the limit to the chan
 * If -l --> remove the mode and the limit */
void            Server::modeL(STR_VEC& split)
{
    std::cout << "split == " << split[0] << " | " << split[1] << " | " << split[2] << " | " << split[3] << std::endl;
    if (split[2].find("+") != std::string::npos) {
        std::cout << "new limit is " << atoi(split[3].c_str()) << std::endl;
        _channels[_msg.currentChan].sLimit(atoi(split[3].c_str()));
        if (!foundModeInChan(split[2].substr(1), _channels[_msg.currentChan].gModes())) {
            _channels[_msg.currentChan].addMode('l'); }
    }
    else {
        if (foundModeInChan(split[2].substr(1), _channels[_msg.currentChan].gModes())) {
            _channels[_msg.currentChan].dltMode('l');
            _channels[_msg.currentChan].sLimit(0);
        }
    }
}

void    Server::sendToChan(void)
{
    std::cout << BLUE "Current Chan Name: " << _channels[_msg.currentChan].gName() << NC << std::endl;
    for (CLIENT_IT it = _channels[_msg.currentChan].gClients().begin(); it != _channels[_msg.currentChan].gClients().end(); it ++)
    {
        if (it->getNickname() != _clients[_msg.currentIndex].getNickname()) {
            _msg.response = "Message to " + it->getNickname() + ": Ce message est un test\r\n";
            sendResponse(userInChanFd(it->getNickname(), _channels[_msg.currentChan].gClients()));}
    }
}
