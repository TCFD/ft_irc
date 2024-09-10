#include "../includes/Parsing.hpp"

/*

    Les fonctions qui suivent permettent de gérer le cas par cas
    des différents champs utilisés. (Username, Channel, ...)

*/

//////////////// INFO CHANNEL ////////////////

bool	Parsing::_attribution_info_channel(std::string& CMDSplit_value)
{
	if (CMDSplit_value.size() >= 2)
	{
		if (CMDSplit_value[0] == '#')
		{
			if ((CMDSplit_value[1] >= 'A' && CMDSplit_value[1] <= 'Z') ||
				(CMDSplit_value[1] >= 'a' && CMDSplit_value[1] <= 'z') ||
				(CMDSplit_value[1] >= '0' && CMDSplit_value[1] <= '9'))
			{
				_infos["channel"] = CMDSplit_value;
				return (true);
			}
		}
	}
	if (!_duplicates_found)
		throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <channel> form.");
	return (false);
}

//////////////// INFO OPTION ////////////////

bool	Parsing::_attribution_info_option(std::string& CMDSplit_value)
{
	if (CMDSplit_value[0] == '-')
	{
		if (!find_key_in_container(_options, CMDSplit_value))
			throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <option>. (Must be one of : -i -t -k -o -l)");
		
		_infos["option"] = CMDSplit_value;
		_options[CMDSplit_value] = 1;

		return (true);
	}
	if (!_duplicates_found)
		throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <option> form.");
	return (false);
}

//////////////// INFO MESSAGE ////////////////

bool	Parsing::_attribution_info_message(std::string& CMDSplit_value)
{
	int end_idx   = _brut_cmd.find(']');

	if (CMDSplit_value[0] == ':' && CMDSplit_value[1] == '[' && (long unsigned int)end_idx != std::string::npos)
	{
		int start_idx = _brut_cmd.find('[') + 1;

		_infos["message"] = str_cut(_brut_cmd, start_idx, end_idx);;
		return (true);
	}
	if (!_duplicates_found)
		throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <message> form.");
	return (false);
}

//////////////// INFO USERNAME ////////////////

bool	Parsing::_attribution_info_username(std::string& CMDSplit_value)
{
	if (!find_one_of_them("/#:-", CMDSplit_value))
	{
		_infos["username"] = CMDSplit_value;
		return (true);
	}
	if (!_duplicates_found)
		throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <username> form.");
	return (false);
}

//////////////// INFO PASSWORD ////////////////

bool	Parsing::_attribution_info_password(std::string& CMDSplit_value)
{
	if (!find_one_of_them("/#:-", CMDSplit_value))
	{
		_infos["password"] = CMDSplit_value;
		return (true);
	}
	if (!_duplicates_found)
		throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <password> form.");
	return (false);
}

/*

    Permet d'initialiser les valeurs associés aux différentes clés pour _infos.
    Cela permettra plus tard de les récupérer facilement.

*/

bool	Parsing::_elmt_attribution(char identifier, std::string CMDSplit_value)
{
	switch (identifier)
	{
		case '#':
			return (_attribution_info_channel(CMDSplit_value));
		case 'U':
			return (_attribution_info_username(CMDSplit_value));
		case 'M':
			return (_attribution_info_message(CMDSplit_value));
		case 'P':
			return (_attribution_info_password(CMDSplit_value));
		case 'O':
			return (_attribution_info_option(CMDSplit_value));
	}
	return (false);
}

/*

    Permet de lancer toutes les vérifications nécessaires pour la forme.

*/

bool	Parsing::form_verification(PARSING_VECTOR_SPLIT& cmd_split,
            PARSING_VECTOR_SPLIT& form_split)
{
	int t_min = 1;
	int t_max = 1;
	int msg_found = 0;

	char		c;
	std::string	str;

	long unsigned int cmd_len = len_of_tab_with_intervals(cmd_split, '[', ']');

	if (cmd_len > form_split.size())
		return (false);

	for (long unsigned int i=1; i < form_split.size(); i++)
	{
		if (i - msg_found < cmd_split.size())
		{
			c = form_split[i][0];
			str = cmd_split[i - msg_found];

			if (!_elmt_attribution(c, str))
				return (false);

			if (c == 'M')
			{
				cmd_split = removeBetweenAngles(cmd_split);
				msg_found++ ;
			}
		}
		else
		{
			if (byidx(form_split, i).size() == 2)
				return (false);
		}

		if (byidx(form_split, i).size() == 2)
			t_min++ ;
		t_max++ ;
	}

	if (!((long unsigned int)t_min <= cmd_len && cmd_len <= (long unsigned int)t_max))
		throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid syntax.");
	
	return (true);
}

/*

	Permet d'afficher la forme correcte que l'on attend.

*/

void Parsing::_err_form_writing(PARSING_VECTOR_SPLIT& form)
{
	std::string	cara;

	std::cout << "\n";

	if (_duplicates_found)
		std::cout << "\t";
	
	for (long unsigned int i=0; i < form.size(); i++)
	{
		cara = byidx(form, i)[0];
		std::cout << _err_map[cara];
		std::cout << " ";
	}
	std::cout << std::endl;
}

void	Parsing::err_write_correct_form(std::string gap)
{

	if (_actual_cmd.empty())
		return ;


	_err_map["/"] = "🔸 /" + _actual_cmd + gap;

	if (_duplicates_found)
	{
		PARSING_VECTOR_SPLIT cmd_all_form = split(_actual_brut_form, ',');
		
		std::cout << "\n\033[35m// Multiple syntaxe\033[0m" << std::endl;

		for (long unsigned int i=0; i < cmd_all_form.size(); i++)
		{
			std::cout << (i+1) << ".";
			PARSING_VECTOR_SPLIT cmd_all_form_split = split(byidx(cmd_all_form, i), ' ');
			_err_form_writing(cmd_all_form_split);
		}

		std::cout << "\n\033[35m-------------------\033[0m" << std::endl;

		return ;
	}
	
	_err_form_writing(_actual_split_form);
	return ;

	
}

void	Parsing::err_miss_elmt(PARSING_VECTOR_SPLIT& cmd_split)
{
	std::string elmt = "";

	for (long unsigned int i=0; i < cmd_split.size(); i++)
	{
		elmt = byidx(cmd_split, i);
		if (elmt.size() < 2)
			throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "The elements must have a minimum length of 2.");
	}
}

/*

	Permet de gérer le cas où plusieurs syntaxes sont correctes pour une seule et même commande.
	Renvoie la forme utilisée parmis les autres.

*/

std::string	Parsing::_any_duplicates(PARSING_VECTOR_SPLIT& cmd_split, std::string& cmd_form)
{
	PARSING_VECTOR_SPLIT	separator_found;
	PARSING_VECTOR_SPLIT	form_split;

	if (cmd_form.find(',') == std::string::npos)
		return ("");

	_duplicates_found = 1;

	separator_found = split(cmd_form, ',');
	for (long unsigned int i=0; i < separator_found.size(); i++)
	{
		form_split = split(byidx(separator_found, i), ' ');

		_actual_split_form = form_split;
		if (form_verification(cmd_split, form_split))
		{
			return (byidx(separator_found, i));
		}
	}
	throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Form invalid.");
}