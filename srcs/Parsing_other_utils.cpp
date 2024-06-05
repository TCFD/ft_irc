#include "Parsing.hpp"

/*
    ID: 1
    
                    *******************************
                    *    Type : (std::vector)     *
                    *_____________________________*
                    *******************************
*/

/*
    1.

    Permet de reproduire un split en alimentant un container de type vector ici.

*/

PARSING_VECTOR_SPLIT split(const std::string &str, char delimiter)
{
    PARSING_VECTOR_SPLIT result;
    std::string token;
    
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
        if (*it != delimiter)
		{
            token += *it;
        }
		else
		{
            if (!token.empty())
			{
                result.push_back(token);
                token.clear();
            }
        }
    }
    if (!token.empty())
        result.push_back(token);

    return (result);
}

/*
    ID: 2
    
                    **************************************
                    *    Type : (std::string const&)     *
                    *____________________________________*
                    **************************************
*/

/*

    2.

    Permet de retourner la valeur à l'emplacement "index" du container. (Toujours de type vector)

*/

std::string const& byidx(PARSING_VECTOR_SPLIT& v, int index)
{
    if (index < 0 || (unsigned int)index >= v.size())
    	throw Parsing::ParsingInvalidSyntax(std::string("[Vector Error]: ") + std::string("Index out of range."));
    return v[index];
}

/*
    ID: 3
    
                    ***********************************
                    *          Type : (bool)          *
                    *_________________________________*
                    ***********************************
*/

/*
    3.

    Permet de trouver l'occurence d'un caractère contenu dans une chaîne.
    Ici on vient tester tous les caractères de la chaîne "cara_search",
    et on retourne un booléen selon si un de ces derniers a été trouvé ou non dans "str_origin".

*/

bool	find_one_of_them(std::string cara_search, std::string& str_origin)
{
	for (long unsigned int i=0; i < cara_search.size(); i++)
	{
		if (str_origin.find(cara_search[i]) != std::string::npos)
			return (true);
	}
	return (false);
}
