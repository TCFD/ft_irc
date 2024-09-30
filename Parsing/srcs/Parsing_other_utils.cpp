#include "../includes/Parsing.hpp"

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
    	throw Parsing::ParsingInvalidSyntax(std::string("[Vector Error]: ") + std::string("Index out of range baby."));
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


std::string str_cut(std::string str, int start_idx, int end_idx)
{
    return (str.substr(start_idx, (end_idx - start_idx)));
}

int tab_idx_elmt(PARSING_VECTOR_SPLIT tab, char cara)
{
    for (long unsigned int i=0; i < tab.size(); i++)
    {
        if (byidx(tab, i).find(cara) != std::string::npos)
            return (i);
    }
    return (-1);
}

int len_of_tab_with_intervals(PARSING_VECTOR_SPLIT tab, char cara1, char cara2)
{
    int sidx = tab_idx_elmt(tab, cara1);
    int eidx = tab_idx_elmt(tab, cara2);

    return (tab.size() - (eidx - sidx));
}

std::string concat_vector_elmt(PARSING_VECTOR_SPLIT tab)
{
    std::string concat;

    for (size_t i=0; i < tab.size(); i++)
        concat = concat + " " + std::string(tab[i]);
    return (concat.substr(1));
}

PARSING_VECTOR_SPLIT remove_between_angles(const PARSING_VECTOR_SPLIT& input)
{
    PARSING_VECTOR_SPLIT result;
    bool inAngleBrackets = false;

    for (size_t i = 0; i < input.size(); ++i)
    {
        const std::string& element = input[i];

        if (inAngleBrackets)
            result.push_back(element);

        if (element.find(':') != std::string::npos && !inAngleBrackets)
        {
            result.push_back(element.substr(1));
            inAngleBrackets = true;
        }
    }
    if (!inAngleBrackets)
        throw Parsing::ParsingInvalidSyntax(std::string(FORM_ERR) + "Invalid syntax.");
    return (result);
}
