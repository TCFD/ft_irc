#include "../includes/Parsing.hpp"

std::string Parsing::parsingGetCommand(void)
{
    return (this->_actualCmd);
}

std::string Parsing::parsingGetMessage(void)
{
    return (this->_infos["message"]);
}

std::string Parsing::parsingGetPassword(void)
{
    return (this->_infos["password"]);
}

std::string Parsing::parsingGetOption(void)
{
    return (this->_infos["option"]);
}

std::string Parsing::parsingGetUsername(void)
{
    return (this->_infos["username"]);
}

std::string Parsing::parsingGetChannel(void)
{
    return (this->_infos["channel"]);
}
