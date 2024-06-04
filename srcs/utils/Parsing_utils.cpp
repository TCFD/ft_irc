#include "../../includes/Parsing.hpp"

/*

    Les fonctions qui suivent permettent de gérer le cas par cas
    des différents champs utilisés. (Username, Channel, ...)

*/

//////////////// INFO CHANNEL ////////////////

void	Parsing::_attribution_info_channel(std::string& CMDSplit_value)
{
	if (CMDSplit_value[0] == '#')
	{
		_infos["channel"] = CMDSplit_value;
		return;
	}
	throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <channel> form.");
}

//////////////// INFO OPTION ////////////////

void	Parsing::_attribution_info_option(std::string& CMDSplit_value)
{
	if (CMDSplit_value[0] == '-')
	{
		_infos["option"] = CMDSplit_value;
		return;
	}
	throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <option> form.");
}

//////////////// INFO MESSAGE ////////////////

void	Parsing::_attribution_info_message(std::string& CMDSplit_value)
{
	if (CMDSplit_value[0] == ':')
	{
		_infos["message"] = CMDSplit_value;
		return;
	}
	throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <message> form.");
}

//////////////// INFO USERNAME ////////////////

void	Parsing::_attribution_info_username(std::string& CMDSplit_value)
{
	if (!find_one_of_them("/#:", CMDSplit_value))
	{
		_infos["Username"] = CMDSplit_value;
		return;
	}
	throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <username> form.");
}

//////////////// INFO PASSWORD ////////////////

void	Parsing::_attribution_info_password(std::string& CMDSplit_value)
{
	if (!find_one_of_them("/#:", CMDSplit_value))
	{
		_infos["Password"] = CMDSplit_value;
		return;
	}
	throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <password> form.");
}

/*

    Permet d'initialiser les valeurs associés aux différentes clés pour _infos.
    Cela permettra plus tard de les récupérer facilement.

*/

void	Parsing::_elmt_attribution(char identifier, std::string& CMDSplit_value)
{
	switch (identifier)
	{
		case '#':
			_attribution_info_channel(CMDSplit_value);
			break;
		case 'U':
			_attribution_info_username(CMDSplit_value);
			break;		
		case 'M':
			_attribution_info_message(CMDSplit_value);
			break;
		case 'P':
			_attribution_info_password(CMDSplit_value);
			break;
		case 'O':
			_attribution_info_option(CMDSplit_value);
			break;
	}
}

/*

    Permet de lancer toutes les vérifications nécessaires pour la forme.

*/

bool	Parsing::form_verification(PARSING_VECTOR_SPLIT& cmd_split,
            PARSING_VECTOR_SPLIT& form_split)
{
	int t_min = 0;
	int t_max = 0;

	for (long unsigned int i=1; i < form_split.size(); i++)
	{
		if (i < cmd_split.size())
			_elmt_attribution(form_split[i][0], cmd_split[i]);
		
		if (byidx(form_split, i).size() == 2)
			t_min++ ;
		t_max++ ;
	}

	if (!((long unsigned int)t_min >= cmd_split.size() && cmd_split.size() <= (long unsigned int)t_max))
		throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid syntax.");
	
	return (true);
}

/*

	Permet d'afficher la forme correcte que l'on attend.

*/

void	Parsing::err_write_correct_form()
{
	std::cout << "Need form :" << std::endl;

	std::cout << "\n\t";
	for (long unsigned int i=0; i < _actual_split_form.size(); i++)
	{
		std::cout << _err_map[byidx(_actual_split_form, i)];
		std::cout << " ";
	}
	std::cout << std::endl;
}

/*

	Permet de gérer le cas où plusieurs syntaxes sont correctes pour une seule et même commande.
	Renvoie la forme utilisée parmis les autres.

*/

std::string	Parsing::_any_duplicates(PARSING_VECTOR_SPLIT& cmd_split, std::string& cmd_form)
{
	PARSING_VECTOR_SPLIT	separator_found;
	PARSING_VECTOR_SPLIT	form_split;

	if (!cmd_form.find(','))
		return ("");

	separator_found = split(cmd_form, ',');
	for (long unsigned int i=0; i < separator_found.size(); i++)
	{
		form_split = split(cmd_form, ' ');

		if (form_verification(cmd_split, form_split))
		{
			_actual_split_form = form_split;
			return (byidx(separator_found, i));
		}
	}
	throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Form invalid.");
}