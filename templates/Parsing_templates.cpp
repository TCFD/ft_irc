#include "../includes/Parsing.hpp"
/*

    Permet de chercher une clé dans un container
    ainsi que de retourner la vérification de si elle a été trouvée ou non.

*/

template <typename M>
bool    find_key_in_container(M& map_used, std::string key)
{
	typename M::iterator it = map_used.find(key);
	return (it != map_used.end());
}

/*

    Permet de checker si la valeur associé à une clé existe dans le container passé en argument.

*/

template <typename M>
std::string const check_infos(M& map_used, std::string keyword)
{
	if (!map_used[keyword].empty())
		return (map_used[keyword]);
	return (NULL);
}

/*

	Permet de retourner un tuple de deux arguments

*/

template <typename TA, typename TB>
std::pair<TA, TB> pair_it(TA a, TB b)
{
	return (std::pair<TA, TB>(a, b));
}
