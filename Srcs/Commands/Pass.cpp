#include "../../Server.hpp"

void    Server::pass(void)
{
    STR_VEC cmdVec(splitCmd(_msg.command));
    if (cmdVec[1] == _mdp)
        std::cout << GREEN "CONNEXION AUTORISEE" NC << std::endl;
    else
        std::cout << RED "C'est ciao\n" NC;
}