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
		HH	= Host

		When it's in double, it's an obligation,
		if not, like

			M
			/
			#
			P
			U
			O

		it's optional

		',' operator = OR between two uses. (syntax)

		So "UU-##" = Mandatory: Username OR Channel


*/

Parsing::Parsing(void)
{

	_duplicates_found = 0;	// Pour identifier si on est sur une piste de commande a syntaxe multiple.

	_options["-i"] = 0;		// Définir/supprimer le canal sur invitation uniquement
	_options["-t"] = 0;		// Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
	_options["-k"] = 0;		// Définir/supprimer la clé du canal (mot de passe)
	_options["-o"] = 0;		// Donner/retirer le privilège de l’opérateur de canal
	_options["-l"] = 0;		// Définir/supprimer la limite d’utilisateurs pour le canal
	_options["-0"] = 0;
	_options["+0"] = 0;
	_options["+i"] = 0;
	_options["+t"] = 0;
	_options["+k"] = 0;
	_options["+o"] = 0;
	_options["+l"] = 0;

	// std::cout << "_option's map Set up." << std::endl;

	_cmd["PRIVMSG"]		= pair_it(0, "// UU MM,// ## MM");			// Message privé
	_cmd["INVITE"]		= pair_it(0, "// UU ##");					// Inviter un client au channel
	_cmd["TOPIC"]		= pair_it(0, "// ## MM");
	
	//USER asalic asalic localhost :Awena Salic
	_cmd["USER"]		= pair_it(0, "// UU UU HH MM,// UU OO UU MM");
	
	//_cmd["USER"]		= pair_it(0, "// ");

	_cmd["KICK"]		= pair_it(0, "// ## UU MM,// UU");					// Ejecter un client du channel
	_cmd["MODE"]		= pair_it(0, "// UU O,// ## O");			// Changer le mode du channel
	_cmd["JOIN"]		= pair_it(0, "// ## P");
	_cmd["OPER"]		= pair_it(0, "// UU PP");
	_cmd["PASS"]		= pair_it(0, "// PP");
	_cmd["QUIT"]		= pair_it(0, "// MM");

	_cmd["KILL"]		= pair_it(0, "// UU MM");
	_cmd["NICK"]		= pair_it(0, "// UU");
	_cmd["LIST"]		= pair_it(0, "// ##");
	_cmd["NAMES"]		= pair_it(0, "// ##");

	// std::cout << "_cmd's map Set up." << std::endl;

	_infos["username"]	= ""; 	// Pour le username
	_infos["password"]	= "";
	_infos["message"]	= "";
	_infos["channel"]	= ""; 	// Pour le Channel ID
	_infos["option"]	= "";
	_infos["host"]		= "";

	//_infos["status"]	= ""; 	// Pour le status (admin, etc)

	// std::cout << "_info's map Set up." << std::endl;

	_err_map["U"] = std::string(HELP_USERNAME);
	_err_map["P"] = std::string(HELP_PASSWORD);
	_err_map["O"] = std::string(HELP_OPTION);
	_err_map["M"] = std::string(HELP_MESSAGE);
	_err_map["/"] = std::string(HELP_COMMAND);
	_err_map["#"] = std::string(HELP_CHANNEL);
	_err_map["H"] = std::string(HELP_HOST);


	// std::cout << "_err_map's map Set up." << std::endl;

	// std::cout << "End of Parsing prv var set up." << std::endl;
	// std::cout << std::endl;
};


/*

	Default Destructor

*/

Parsing::~Parsing(void){};


/*

		Utilisé pour savoir si la commande passée et valide et si oui,
		Renvoyer son ID et sa Forme d'utilisation.

*/

std::pair<std::string, std::string> Parsing::parsingGetCmd(void)
{
	std::string cmd_title	= "";
	std::string cmd_form	= "";
	
	int total = 0;

	for (PARSING_MAP_CMD::const_iterator it = _cmd.begin(); it != _cmd.end(); ++it)
	{
		if (it->second.first)
		{
			if (total)
				throw Parsing::ParsingInvalidSyntax(std::string(PARSING_ERR) + "Only one command at time.");
			total++ ;

			cmd_form  = it->second.second;
			cmd_title = it->first;
		}
    }

    if (cmd_form.empty() || cmd_title.empty())
        throw Parsing::ParsingInvalidSyntax(std::string(PARSING_ERR) + "Invalid command field.");

    return (pair_it(cmd_title, cmd_form));
}


/*

		Permet de reset à 0 tous les status actifs associées aux commandes.
		Exemple:

			Si /KICK a été utilisé, alors son status est égal à 1 car actif.
			Quand on reset, toutes valeurs confondues repassent à 0.

*/

void	Parsing::cmdResetStatus(void)
{
	// Pour les commandes
	PARSING_MAP_CMD::iterator it = _cmd.begin();
	while (it != _cmd.end())
	{
		(*it).second.first = 0;
		it++;
	}

	// Pour les options
	PARSING_MAP_OPT::iterator optit = _options.begin();
	while (optit != _options.end())
	{
		(*it).second.first = 0;
		optit++;
	}
}

/*

		Permet de traiter la commande de son état brut et procède à toutes les vérifications
		et initialisations nécessaires pour la suite.

*/


void	Parsing::cmdStatus(void)
{
	std::cout << "\t | Command  : "  << parsingGetCommand()	 << std::endl;
	std::cout << "\t | Channel  : "  << parsingGetChannel()	 << std::endl;
	std::cout << "\t | Username : "  << parsingGetUsername() << std::endl;
	std::cout << "\t | Host     : "  << parsingGetHost()	 << std::endl;
	std::cout << "\t | Msg      : "  << parsingGetMessage()  << std::endl;
	std::cout << "\t | Option   : "  << parsingGetOption()   << std::endl;
	std::cout << "\t | Password : "  << parsingGetPassword() << std::endl;
	std::cout << "\n" << std::endl;
}

std::string	Parsing::cmdTreatTest(std::string brut_cmd)
{
	_brutCmd = brut_cmd;

	PARSING_VECTOR_SPLIT string_split = split(brut_cmd, ' ');

	//errMissElmt(string_split);

	std::string command = byidx(string_split, 0);
	if (byidx(string_split, 0)[0] == '/')
		command = command.substr(1);

	cmdResetStatus();

	if (find_key_in_container(_cmd, command))
	{
		_cmd[command].first = 1;

		std::pair<std::string, std::string> result = parsingGetCmd();

		_actualCmd = result.first;
		_actualBrutForm = result.second;

		//:::::: TEST ::::::://

		if (anyDuplicates(string_split, result.second).empty())
		{
			_duplicates_found = 0;
			PARSING_VECTOR_SPLIT form_split = split(result.second, ' ');
			_actual_split_form = form_split;
			if(!formVerification(string_split, form_split))
				throw Parsing::ParsingInvalidSyntax(std::string(CMD_ERR) + "Invalid syntax.");
		}
		return result.first ;
	}
	std::cout << CMD_ERR << (string_split[0]) << " > Command not found." << std::endl;
	throw Parsing::ParsingInvalidCommand(std::string(CMD_ERR));
	return "";
}

