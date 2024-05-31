#include "../includes/Parsing.hpp"



/*

	Default Constructor

	infos :

		MM 	= Message
		//	= Command
		##	= Channel
		PP 	= Password
		UU 	= Username
		OO 	= Options (Flags)

		When it's in double, it's an obligation,
		if not, like

			M
			/
			#
			P
			U
			O

		it's optional

		'-' operator = OR between two options
		'|' operator = OR between two uses. (syntax)

		So "UU-##" = Mandatory: Username OR Channel


*/

template <typename TA, typename TB>
static std::pair<TA, TB> pair_it(TA a, TB b)
{
	return (std::pair<TA, TB>(a, b));
}

Parsing::Parsing()
{
	_options["-i"] = 0;		// Définir/supprimer le canal sur invitation uniquement
	_options["-t"] = 0;		// Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
	_options["-k"] = 0;		// Définir/supprimer la clé du canal (mot de passe)
	_options["-o"] = 0;		// Donner/retirer le privilège de l’opérateur de canal
	_options["-l"] = 0;		// Définir/supprimer la limite d’utilisateurs pour le canal

	std::cout << "_option's map Set up." << std::endl;

	_cmd["PRIVMSG"]		= pair_it(0, "// UU-## M");			// Message privé
	_cmd["INVITE"]		= pair_it(0, "// UU ##");			// Inviter un client au channel
	_cmd["TOPIC"]		= pair_it(0, "// ## MM");			// Modifier ou afficher le thème du channel
	_cmd["NAMES"]		= pair_it(0, "// ##");
	_cmd["KICK"]		= pair_it(0, "// ## UU MM");		// Ejecter un client du channel
	_cmd["MODE"]		= pair_it(0, "// UU OO|// ## OO");	// Changer le mode du channel
	_cmd["JOIN"]		= pair_it(0, "// ## PP");
	_cmd["OPER"]		= pair_it(0, "// UU PP");
	_cmd["PASS"]		= pair_it(0, "// PP");
	_cmd["QUIT"]		= pair_it(0, "// M");

	//_cmd["USER"]	= pair_it(0, );
	
	_cmd["KILL"]		= pair_it(0, "// UU MM");
	_cmd["NICK"]		= pair_it(0, "// UU");
	_cmd["LIST"]		= pair_it(0, "// ##");

	std::cout << "_cmd's map Set up." << std::endl;

	_infos["Username"]	= ""; 	// Pour le username
	_infos["Channel"]	= ""; 	// Pour le Channel ID
	_infos["Status"]	= ""; 	// Pour le status (admin, etc)

	std::cout << "_info's map Set up." << std::endl;
	std::cout << "End of Parsing prv var set up." << std::endl;
};



/*

	Default Destructor

*/

Parsing::~Parsing(){};

////////////////////////////////////////////////////////////////////

template <typename M>
static std::string const& check_if_valid(M& map_used, std::string err_msg)
{
	(void)err_msg;

	std::string const* opt = NULL;
	int total = 0;

	typename M::iterator it = map_used.begin();
	while (it != map_used.end())
	{
		if (it->second)
		{
			if (total)
				throw std::invalid_argument(std::string(PARSING_ERR) + err_msg);
			total++;
			opt = &(it->first);
		}
		it++ ;
	}

	if (opt == NULL)
        throw std::invalid_argument(std::string(PARSING_ERR) + "Invalid option.");
	
	return (*opt);
}





template <typename M>
static std::string const check_infos(M& map_used, std::string keyword)
{
	if (!map_used[keyword].empty())
		return (map_used[keyword]);
	return (NULL);
}


////////////////////////////////////////////////////////////////////



/*

template <typename M, typename K, typename V>
void Parsing::parsing_map_insert(M& map_used, K key, V value)
{
	typename M::iterator it = map_used.insert(std::pair<K, V>(key, value)).first;
}


template <typename M, typename K, typename V>
void Parsing::parsing_map_insert(M& map_used, K key_id, V value)
{
	typename map_used[key_id] = value;
	return ;
}

*/


// get option used
std::string const Parsing::parsing_get_option(void)
{
	return (check_if_valid(_options, "Only one option is required."));
}

// get command
std::pair<std::string, std::string> Parsing::parsing_get_cmd(void)
{
	std::string cmd_title = "";
	std::string cmd_form = "";
	
	int total = 0;

	for (PARSING_MAP_CMD::const_iterator it = _cmd.begin(); it != _cmd.end(); ++it)
	{
		if (it->second.first)
		{
			if (total)
				throw std::invalid_argument(std::string(PARSING_ERR) + "Only one command at time.");
			total++ ;
			cmd_form = it->second.second;
			cmd_title = it->first;
		}
    }

    if (cmd_form.empty() || cmd_title.empty())
        throw std::invalid_argument(std::string(PARSING_ERR) + "Invalid command field.");

    return (pair_it(cmd_title, cmd_form));
}


// get username
std::string const Parsing::parsing_get_username(void)
{
	return (check_infos(_infos, "Username"));
}


// get channel
std::string const Parsing::parsing_get_channel(void)
{
	return (check_infos(_infos, "Channel"));
}


// -> Dans UTILS.CPP
std::vector<std::string> split(const std::string &str, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (*it != delimiter) {
            token += *it;
        } else {
            if (!token.empty()) {
                result.push_back(token);
                token.clear();
            }
        }
    }
    if (!token.empty()) {
        result.push_back(token);
    }
    return result;
}

static std::string const& byidx(std::vector<std::string>& v, int index)
{
    if (index < 0 || (unsigned int)index >= v.size())
    	throw std::invalid_argument("Index out of range.");
    return v[index];
}


void	Parsing::_cmd_reset_status(void)
{
	PARSING_MAP_CMD::iterator it = _cmd.begin();
	while (it != _cmd.end())
	{
		(*it).second.first = 0;
		it++;
	}
}


template <typename M>
static bool find_it(M& map_used, std::string key)
{
	typename M::iterator it = map_used.find(key);
	return (it != map_used.end());
}

void	Parsing::_check_form(std::vector<std::string> cmd_split, std::vector<std::string> form_split)
{
	int optional=0;

	std::map<std::string, std::string> keyparing;

	for (unsigned int i=0; i < form_split.size(); i++)
	{
		if (form_split[i].size() == 1)
			optional++ ;

		if (form_split[i][0] == '#' && cmd_split[i][0] != '#')
			throw std::invalid_argument(std::string(FORM_ERR) + "Not correct cmd form. (channel)");

		std::string letter(1, form_split[i][0]);
		if (i != cmd_split.size())
			keyparing.insert(pair_it(letter, cmd_split[i]));

		std::cout << form_split[i] << std::endl;
	}

	if (cmd_split.size() == form_split.size() || cmd_split.size() == form_split.size() - optional)
	{
		if (find_it(keyparing, "U"))
			_infos["Username"] = keyparing["U"];
		if (find_it(keyparing, "#"))
			_infos["Channel"] = keyparing["#"];
	}
	else
		throw std::invalid_argument(std::string(FORM_ERR) + "Not correct cmd form.");


}



void	Parsing::cmd_treat_test(std::string brut_cmd)
{
	std::cout << "\n\tOriginal input : [ " << brut_cmd << " ]" << std::endl;
	std::vector<std::string> string_split = split(brut_cmd, ' ');

	std::string const command = byidx(string_split, 0).substr(1);

	_cmd_reset_status();

	if (_cmd.find(command) != _cmd.end())
	{
		_cmd[command].first = 1;

		std::pair<std::string, std::string> result = parsing_get_cmd();
		std::cout << "\n\t\033[32mCommand found\033[0m : [Title: " << result.first << "], [form: " << result.second << "]" << std::endl;
		std::cout << std::endl;


		//:::::: TEST ::::::://

		std::vector<std::string> form_split = split(result.second, ' ');
		_check_form(string_split, form_split);

		//::::::::::::::::::://

		return ;
	}

	throw std::invalid_argument(std::string(CMD_ERR) + "Command not found.");
}

