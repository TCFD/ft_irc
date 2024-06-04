#include "includes/Parsing.hpp"

int main(int argc, char **argv)
{
	std::string cmdtest = (argc > 1) ? std::string(argv[1]) : "";
	
	Parsing	parsingtools;

	try
	{

		parsingtools.cmd_treat_test(cmdtest);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	return (0);
}