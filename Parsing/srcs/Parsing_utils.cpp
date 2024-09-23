#include "../includes/Parsing.hpp"

/*

    Les fonctions qui suivent permettent de gérer le cas par cas
    des différents champs utilisés. (Username, Channel, ...)

*/

//////////////// INFO CHANNEL ////////////////

bool	Parsing::attributionInfoChannel(std::string& CMDSplit_value)
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

bool	Parsing::attributionInfoOption(std::string& CMDSplit_value)
{
	if (CMDSplit_value[0] == '-' || CMDSplit_value[0] == '+')
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

bool	Parsing::attributionInfoMessage(std::string& CMDSplit_value)
{
	_infos["message"] = CMDSplit_value;
	return (true);

	//if (!_duplicates_found)
	//	throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid <message> form.");
	//return (false);
}

//////////////// INFO USERNAME ////////////////

bool	Parsing::attributionInfoUsername(std::string& CMDSplit_value)
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

bool	Parsing::attributionInfoPassword(std::string& CMDSplit_value)
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


bool	Parsing::attributionInfoHost(std::string& CMDSplit_value)
{
	if (CMDSplit_value == "localhost" || CMDSplit_value == "127.0.0.1")
	{
		_infos["host"] = "localhost";
		return (true);
	}
	return (false);

	//PARSING_VECTOR_SPLIT network_id;
	//network_id = split(CMDSplit_value, '.');
	//if (network_id.size() != 4)
	//	return (false);
	//
	//for (size_t i = 0; i < network_id.size(); i++)
	//{
	//	int nt_len = network_id[i].size();
	//	if (nt_len < 1 && nt_len > 3)
	//		return (false);
	//}
	//return (true);
}

/*

    Permet d'initialiser les valeurs associés aux différentes clés pour _infos.
    Cela permettra plus tard de les récupérer facilement.

*/

bool	Parsing::elmtAttribution(char identifier, std::string CMDSplit_value)
{
	switch (identifier)
	{
		case '#':
			return (attributionInfoChannel(CMDSplit_value));
		case 'U':
			return (attributionInfoUsername(CMDSplit_value));
		case 'M':
			return (attributionInfoMessage(CMDSplit_value));
		case 'P':
			return (attributionInfoPassword(CMDSplit_value));
		case 'O':
			return (attributionInfoOption(CMDSplit_value));
		case 'H':
			return (attributionInfoHost(CMDSplit_value));
	}
	return (false);
}

/*

    Permet de lancer toutes les vérifications nécessaires pour la forme.

*/

bool	Parsing::formVerification(PARSING_VECTOR_SPLIT& cmd_split,
            PARSING_VECTOR_SPLIT& form_split)
{
	int t_min = 1;
	int t_max = 1;
	int msg_found = 0;

	char		c;
	std::string	str;

	int form_len = form_split.size();
	int len_cmd = cmd_split.size();


	for (long unsigned int i=1; i < form_split.size(); i++)
	{
		if (i - msg_found < cmd_split.size())
		{
			c = form_split[i][0];
			str = cmd_split[i - msg_found];

			if (c == 'M')
			{
				cmd_split = remove_between_angles(cmd_split);

				if (cmd_split.size() > 1)
					len_cmd = (size_t)len_cmd - (size_t)cmd_split.size() + 1;
				str = concat_vector_elmt(cmd_split);
				msg_found++ ;
			}

			if (!elmtAttribution(c, str))
				return (false);
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

	if (msg_found) 
	{

		if (len_cmd != t_min && len_cmd != form_len)
			throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid syntax.");
	}
	if (!(t_min <= form_len && form_len <= t_max))
		throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid syntax.");

	return (true);
}

/*

	Permet d'afficher la forme correcte que l'on attend.

*/

void Parsing::errFormWriting(PARSING_VECTOR_SPLIT& form)
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

void	Parsing::errWriteCorrectForm(std::string gap)
{

	if (_actualCmd.empty())
		return ;


	_err_map["/"] = "🔸 /" + _actualCmd + gap;

	if (_duplicates_found)
	{
		PARSING_VECTOR_SPLIT cmd_all_form = split(_actualBrutForm, ',');
		
		std::cout << "\n\033[35m// Multiple syntaxe\033[0m" << std::endl;

		for (long unsigned int i=0; i < cmd_all_form.size(); i++)
		{
			std::cout << (i+1) << ".";
			PARSING_VECTOR_SPLIT cmd_all_form_split = split(byidx(cmd_all_form, i), ' ');
			errFormWriting(cmd_all_form_split);
		}

		std::cout << "\n\033[35m-------------------\033[0m" << std::endl;

		return ;
	}
	
	errFormWriting(_actual_split_form);
	return ;

	
}

void	Parsing::errMissElmt(PARSING_VECTOR_SPLIT& cmd_split)
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

std::string	Parsing::anyDuplicates(PARSING_VECTOR_SPLIT& cmd_split, std::string& cmd_form)
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
		if (formVerification(cmd_split, form_split))
		{
			return (byidx(separator_found, i));
		}
	}
	throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Form invalid.");
}