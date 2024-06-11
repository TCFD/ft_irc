#include "../includes/Parsing.hpp"

std::string Parsing::parsing_get_command(void)
{
    return (this->_actual_cmd);
}

std::string Parsing::parsing_get_message(void)
{
    return (this->_infos["message"]);
}

std::string Parsing::parsing_get_password(void)
{
    return (this->_infos["password"]);
}

std::string Parsing::parsing_get_option(void)
{
    return (this->_infos["option"]);
}

std::string Parsing::parsing_get_username(void)
{
    return (this->_infos["username"]);
}

std::string Parsing::parsing_get_channel(void)
{
    return (this->_infos["channel"]);
}
