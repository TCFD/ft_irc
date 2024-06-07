#include "includes/Parsing.hpp"

int main(int argc, char **argv)
{
	std::string cmdtest = (argc > 1) ? std::string(argv[1]) : "";
	
	std::cout << cmdtest << std::endl;

	Parsing	parsingtools;

	try
	{
		if (std::string(argv[1]) == "/HELP")
			return (parsingtools.parsing_help(), 0);
		parsingtools.cmd_treat_test(cmdtest);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		parsingtools.err_write_correct_form("");
	}

	return (1);
}