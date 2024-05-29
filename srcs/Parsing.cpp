#include "../includes/Parsing.hpp"



/*

	Default Constructor

*/

Parsing::Parsing()
{
	_options["-i"] = 0;		// Définir/supprimer le canal sur invitation uniquement
	_options["-t"] = 0;		// Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
	_options["-k"] = 0;		// Définir/supprimer la clé du canal (mot de passe)
	_options["-o"] = 0;		// Donner/retirer le privilège de l’opérateur de canal
	_options["-l"] = 0;		// Définir/supprimer la limite d’utilisateurs pour le canal

	std::cout << "_option's map Set up." << std::endl;

	_cmd["KICK"]	= 0;	// Ejecter un client du channel
	_cmd["INVITE"]	= 0;	// Inviter un client au channel
	_cmd["TOPIC"]	= 0;	// Modifier ou afficher le thème du channel
	_cmd["MODE"]	= 0;	// Changer le mode du channel

	std::cout << "_cmd's map Set up." << std::endl;

	_infos["Username"]	= nullptr; 	// Pour le username
	_infos["Channel"]	= nullptr; 	// Pour le Channel ID
	_infos["Status"]	= nullptr; 	// Pour le status (admin, etc)

	std::cout << "_info's map Set up." << std::endl;
	std::cout << "End of Parsing prv var set up." << std::endl;
};



/*

	Default Destructor

*/

Parsing::~Parsing(){};

////////////////////////////////////////////////////////////////////

template <typename M>
static std::string const& check_if_valid(M& type_map, M& map_used, std::string checktype)
{
	std::string const* opt = nullptr;
	int total = 0;

	typename M::iterator it = map_used.begin();
	while (it != map_used.end())
	{
		if (it->second):
		{
			if (total)
				throw std::invalid_argument(PARSING_ERR << "Only one " << checktype << " is required.");
			total++;
			opt = &(it->first);
		}
		it++ ;
	}

	if (opt == nullptr)
        throw std::invalid_argument(PARSING_ERR + "No valid " + checktype + " found.");
	
	return (*opt);
}





template <typename M>
static std::string const check_infos(M& map_used, std::string keyword)
{
	if (map_used[keyword] != nullptr)
		return (map_used[keyword]);
	return (nullptr);
}


////////////////////////////////////////////////////////////////////



template <typename M, typename K, typename V>
void Parsing::parsing_map_insert(M& map_used, K key, V value)
{
	typename map_used.insert(std::pair<K, V>(key, value));
}



// get option used
std::string const Parsing::parsing_get_option(void)
{
	return (check_if_valid(PARSING_MAP_OPT, _options, "option"));
}

// get command
std::string const Parsing::parsing_get_cmd(void)
{
	return (check_if_valid(PARSING_MAP_CMD, _cmd, "command"));
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