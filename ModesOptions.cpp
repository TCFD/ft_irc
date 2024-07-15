#include "Server.hpp"
#include "Channel.hpp"

/* Use of function pointer
 * Access of each mode by their own functions
 * For now, there is only modeO and modeK
*/
void    Server::modesOptions(STR_LIST& split)
{
    std::string init[10] = {"+k", "-k", "+l", "-l", "+i", "-i", "+t", "-t", "+o", "-o"};

    void (Server::*funcPtr[2]) (STR_LIST& split) = {&Server::modeK};
    for (int i=0; !init[i].empty(); i++) {if (init[i] == split[2]) (this->*funcPtr[i])(split); return;}
}

/* 
 * Handle k MODE
 * If +k --> add the password to the vector; add the mode to the channel
 * If -k --> remove the password from the vector; if mode was active, remove it from the channel
*/
void    Server::modeK(STR_LIST& split)
{
    // sendToChan();
    if (split[2].find("+") != std::string::npos) {
        _channels[_msg.currentChan].gPassword() = split[3];
        if (!foundModeInChan(split[2].substr(1), _channels[_msg.currentChan].gModes())) {
            _channels[_msg.currentChan].addModes(split[2][1]); }
    }
    else {
        // if (foundModeInChan(split[2].substr(1), _channels[_msg.currentChan].gModes()))
            // tabChan[_msg.currentChan].modes.erase(0); A REVOIR
    }
}

/* 
 * Handle o MODE
 * Check if the nick chosen exists and is in the channel
 * If +o --> add the operator status to nick; add the mode to the channel;
 * If -o --> remove the operator status to nick; if mode was active, remove it from the channel
*/
// void    Server::modeO(STR_LIST& split)
// {   // A REVOIR !!!
    // if (isUserInChan(split[3])) {
    //     if (split[2].find("+")) {
    //         if (!foundModeInChan(split[2].substr(1), _channels[_msg.currentChan].gModes())) {
    //             _channels.addOperator();
    //             // _channels[_msg.currentChan]._clients[split[3]] = 1; 
                
    //         }
    //     }
    //     else {
    //         if (foundModeInChan(split[2].substr(1), tabChan[_msg.currentChan].modes)) {
    //             tabChan[_msg.currentChan].usersInChan[split[3]] = 0; }
    //     }
    // }
// }

// void    Server::sendToChan(void)
// {
//     std::cout << BLUE "Current Chan Name: " << _channels[_msg.currentChan].gName() << NC << std::endl;
//     for (MAP_TAB::iterator it = _channels[_msg.currentChan]._clients.begin(); it != _channels[_msg.currentChan]._clients.end(); it ++)
//     {
//         // if (it->first != tab[_msg.currentIndex].nickName) {
//             _msg.response = "Message to " + it->first + ": Ce message est un test\r\n";
//             send_response(userInChanFd(it->first));
//     }
// }
