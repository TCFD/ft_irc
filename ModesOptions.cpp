#include "Irc.hpp"

/* Use of function pointer
 * Access of each mode by their own functions
 * For now, there is only modeO and modeK
*/
void    Polls::modesOptions(VEC_LIST& split)
{
    std::string init[10] = {"+k", "-k", "+l", "-l", "+i", "-i", "+t", "-t", "+o", "-o"};

    void (Polls::*funcPtr[2]) (VEC_LIST& split) = {&Polls::modeK, &Polls::modeO};
    for (int i=0; !init[i].empty(); i++) {if (init[i] == split[2]) (this->*funcPtr[i])(split); return;}
}

/* 
 * Handle k MODE
 * If +k --> add the password to the vector; add the mode to the channel
 * If -k --> remove the password from the vector; if mode was active, remove it from the channel
*/
void    Polls::modeK(VEC_LIST& split)
{
    if (split[2].find("+") != std::string::npos) {
        tabChan[msg.currentChan].pwd = split[3];
        if (!foundModeInChan(split[2].substr(1), tabChan[msg.currentChan].modes)) {
            tabChan[msg.currentChan].modes.push_back(split[2].substr(1)); }
    }
    else {
        if (foundModeInChan(split[2].substr(1), tabChan[msg.currentChan].modes)) {
            tabChan[msg.currentChan].pwd.erase(0); }
    }
}

/* 
 * Handle o MODE
 * Check if the nick chosen exists and is in the channel
 * If +o --> add the operator status to nick; add the mode to the channel
 * If -o --> remove the operator status to nick; if mode was active, remove it from the channel
*/
void    Polls::modeO(VEC_LIST& split)
{
    if (split[2].find("+") != std::string::npos) {
        if (isUserExists(split[3]) && isUserInChan(split[3])) {
            if (split[2].find("+")) {
                if (!foundModeInChan(split[2].substr(1), tabChan[msg.currentChan].modes)) {
                tabChan[msg.currentChan].usersInChan[split[3]] = 1; }
            }
            else {
                if (foundModeInChan(split[2].substr(1), tabChan[msg.currentChan].modes)) {
                    tabChan[msg.currentChan].usersInChan[split[3]] = 0; }
            }
        }
    }
}